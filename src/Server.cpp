#include "../incl/Server.hpp"

static int setNonBlocking(int fd) {
    int flags;
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
        perror("fcntl F_GETFL");
        return -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL O_NONBLOCK");
        return -1;
    }
    return 0;
}

// Global
volatile bool keepRunning = true;

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received. Shutting down server.\n";
    keepRunning = false;
}

// Constructor
Server::Server(const std::string& configPath) : configPath(configPath), running(false) {}

// Destructor
Server::~Server() {
    stop();
}

/* 
	Method: 		Server::parseConfig
	Description: 	Parses the configuration file to extract ports
*/
bool Server::parseConfig() {
    std::ifstream configFile(configPath.c_str());
    if (!configFile.is_open()) {
        std::cerr << "Failed to open configuration file: " << configPath << "\n";
        return false;
    }

    std::string line;
    while (getline(configFile, line)) {
        // Remove whitespace
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.find("ports=") == 0) {
            std::string portsStr = line.substr(6); // Remove 'ports='
            std::stringstream ss(portsStr);
            std::string port;
            while (getline(ss, port, ',')) {
                int portNum = std::atoi(port.c_str());
                if (portNum > 0 && portNum < 65536) {
                    serverPorts.push_back(portNum);
                } else {
                    std::cerr << "Invalid port number: " << portNum << "\n";
                }
            }
        }
    }

    if (serverPorts.empty()) {
        std::cerr << "No valid ports found in configuration.\n";
        return false;
    }

    std::cout << "Configured to listen on ports: ";
    for (std::vector<int>::iterator portIt = serverPorts.begin(); portIt != serverPorts.end(); ++portIt) {
        std::cout << *portIt << " ";
    }
    std::cout << "\n";

    return true;
}

/* 
	Method: 		Server::setupServerSockets
	Description: 	Sets up listening sockets for each server port
*/
bool Server::setupServerSockets() {
    for (std::vector<int>::iterator portIt = serverPorts.begin(); portIt != serverPorts.end(); ++portIt) {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            perror("socket");
            return false;
        }

        // Allow address reuse
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
            perror("setsockopt SO_REUSEADDR");
            close(server_fd);
            return false;
        }

        // Bind to the specified port on all interfaces
		int server_port = *portIt;
        struct sockaddr_in addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
        addr.sin_port = htons(server_port);

        if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            perror("bind");
            close(server_fd);
            return false;
        }

        // Set the socket to non-blocking mode
        if (setNonBlocking(server_fd) == -1) {
            close(server_fd);
            return false;
        }

        // Start listening
        if (listen(server_fd, BACKLOG) == -1) {
            perror("listen");
            close(server_fd);
            return false;
        }

        serverFds.push_back(server_fd);

        // Add to pollFds
        struct pollfd pfd;
        pfd.fd = server_fd;
        pfd.events = POLLIN;
        pfd.revents = 0;
        pollFds.push_back(pfd);

        std::cout << "Listening on port " << server_port << " with fd " << server_fd << "\n";
    }

    return true;
}

/* 
	Method: 		Server::start
	Description: 	Starts the server
*/
bool Server::start() {
    if (!parseConfig()) {
        return false;
    }

    if (!setupServerSockets()) {
        return false;
    }

    running = true;
    eventLoop();
    return true;
}

/* 
	Method: 		Server::stop
	Description: 	Stops the server
*/
void Server::stop() {
    if (running) {
        running = false;
        closeAllSockets();
        std::cout << "Server stopped.\n";
    }
}

/* 
	Method: 		Server::closeAllSockets
	Description: 	Closes all listening and client sockets
*/
// Closes all listening and client sockets
void Server::closeAllSockets() {
    for (std::vector<int>::iterator it = serverFds.begin(); it != serverFds.end(); ++it) {
        close(*it);
    }
    serverFds.clear();
    pollFds.clear();
}

/* 
	Method: 		Server::closeAllSockets
	Description: 	Main event loop using poll()
*/
void Server::eventLoop() {
    std::cout << "Server is running. Press Ctrl+C to stop.\n";
    while (running && keepRunning) {
        int ret = poll(pollFds.data(), pollFds.size(), 1000); // 1-second timeout
        if (ret == -1) {
            if (errno == EINTR) {
                continue; // Interrupted by signal
            }
            perror("poll");
            break;
        }

        if (ret == 0) {
            continue; // Timeout, no events
        }

        for (size_t i = 0; i < pollFds.size(); ++i) {
            if (pollFds[i].revents & POLLIN) {
                if (std::find(serverFds.begin(), serverFds.end(), pollFds[i].fd) != serverFds.end()) {
                    // Event on listening socket, accept new connection
                    if (!handleNewConnection(pollFds[i].fd)) {
                        std::cerr << "Failed to handle new connection on fd " << pollFds[i].fd << "\n";
                    }
                } else {
                    // Event on client socket, handle client
                    handleClient(pollFds[i].fd);
                }
            }
        }
    }
}

/* 
	Method: 		Server::closeAllSockets
	Description: 	Handles a new incoming connection
*/
bool Server::handleNewConnection(int listen_fd) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        if (errno != EWOULDBLOCK && errno != EAGAIN) {
            perror("accept");
            return false;
        }
        return true; // No pending connections
    }

    // Set client socket to non-blocking
    if (setNonBlocking(client_fd) == -1) {
        close(client_fd);
        return false;
    }

    // Add client to pollFds
    struct pollfd pfd;
    pfd.fd = client_fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    pollFds.push_back(pfd);

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    std::cout << "Accepted connection from " << client_ip << ":" << ntohs(client_addr.sin_port)
              << " with fd " << client_fd << "\n";

    return true;
}

struct MatchClientFd {
    int client_fd;
    
    MatchClientFd(int fd) : client_fd(fd) {}
    
    bool operator()(const pollfd& pfd) const {
        return pfd.fd == client_fd;
    }
};

/* 
	Method: 		Server::closeAllSockets
	Description: 	Handles client communication
*/
void Server::handleClient(int client_fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read == -1) {
        if (errno != EWOULDBLOCK && errno != EAGAIN) {
            perror("recv");
            // Remove from pollFds and close
            pollFds.erase(std::remove_if(pollFds.begin(), pollFds.end(), MatchClientFd(client_fd)),
                         pollFds.end());
            close(client_fd);
        }
        return;
    } else if (bytes_read == 0) {
        // Connection closed by client
        std::cout << "Client fd " << client_fd << " disconnected.\n";
		pollFds.erase(std::remove_if(pollFds.begin(), pollFds.end(), MatchClientFd(client_fd)),
						pollFds.end());
        close(client_fd);
        return;
    }

    buffer[bytes_read] = '\0';
    std::string request(buffer);
    std::cout << "Received request from fd " << client_fd << ":\n" << request << "\n";

    // Simple HTTP response
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello, World!";

    ssize_t bytes_sent = send(client_fd, response.c_str(), response.size(), 0);
    if (bytes_sent == -1) {
        perror("send");
    } else {
        std::cout << "Sent response to fd " << client_fd << "\n";
    }

    // Close the connection after sending the response
    pollFds.erase(std::remove_if(pollFds.begin(), pollFds.end(), MatchClientFd(client_fd)),
                 pollFds.end());
    close(client_fd);
}