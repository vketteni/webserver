#include "../incl/Server.hpp"
#include "../incl/ConfigParser.hpp"


static int setNonBlocking(int fd)
{
    int flags;
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
	{
        perror("fcntl F_GETFL");
        return -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
        perror("fcntl F_SETFL O_NONBLOCK");
        return -1;
    }
    return 0;
}

// Global
volatile bool keepRunning = true;

void signalHandler(int signum)
{
    std::cout << "\nInterrupt signal (" << signum << ") received. Shutting down server.\n";
    keepRunning = false;
}

// Constructor
Server::Server(const std::string& configPath) : configPath(configPath), running(false)
{
	ConfigParser ConfigParser;
	if (!ConfigParser.parseConfig(configPath)) {
		std::cerr << "Failed to parse config file\n";
		exit(1);
	}
	this->servers = ConfigParser.getServer();
}

// Destructor
Server::~Server()
{
    stop();
}



/*
	Method: 		Server::setupServerSockets
	Description: 	Sets up listening sockets for each server port
*/
bool Server::setupServerSockets()
{
    for (std::vector<int>::iterator serverIt = servers.begin(); serverIt != servers.end(); ++serverIt)
	{
		int server_port = serverIT->port;
		int server_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (server_fd == -1)
		{
			perror("socket");
			return false;
		}

		// Allow address reuse
		int opt = 1;
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		{
			perror("setsockopt SO_REUSEADDR");
			close(server_fd);
			return false;
		}

        // Bind to the specified port on all interfaces
	//	int server_port = *portIt;
		struct sockaddr_in addr;
		std::memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
  //   addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
		addr.sin_addr.s_addr = inet_addr(serverIt->host.c_str());
		addr.sin_port = htons(server_port);

		if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		{
			perror("bind");
			close(server_fd);
			return false;
		}

        // Set the socket to non-blocking mode
		if (setNonBlocking(server_fd) == -1)
		{
			close(server_fd);
			return false;
		}

        // Start listening
		if (listen(server_fd, BACKLOG) == -1)
		{
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

		std::cout << "Listening on " << serverIt->host << ":" << server_port << " with fd " << server_fd << "\n";
	}

	return true;
}

/*
	Method: 		Server::start
	Description: 	Starts the server
*/
bool Server::start()
{
    if (!parseConfig())
	{
        return false;
    }

    if (!setupServerSockets())
	{
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
void Server::stop()
{
    if (running)
	{
        running = false;
        closeAllSockets();
        std::cout << "Server stopped.\n";
    }
}

/*
	Method: 		Server::closeAllSockets
	Description: 	Closes all listening and client sockets
*/
void Server::closeAllSockets()
{
    for (std::vector<int>::iterator it = serverFds.begin(); it != serverFds.end(); ++it)
	{
        close(*it);
    }
    serverFds.clear();
    pollFds.clear();
}

void Server::checkTimeouts(void)
{
    time_t currentTime = std::time(NULL);

    for (size_t i = 0; i < clientHandlers.size(); ++i)
    {
		time_t last_activity = clientHandlers[i].getLastActivity();
        if (difftime(currentTime, last_activity) > clientHandlers[i].timeout)
        {
            std::cout << "Client on fd " << clientHandlers[i].fd << " timed out. Disconnecting...\n";
            close(clientHandlers[i].fd);
            pollFds.erase(pollFds.begin() + i);
            clientHandlers.erase(clientHandlers.begin() + i);
            --i;
        }
    }
}

/*
	 Method: 		Server::eventLoop
	 Description: 	Main event loop using poll()
 */
void Server::eventLoop()
{
    std::cout << "Server is running. Press Ctrl+C to stop.\n";
    while (running && keepRunning)
	{
        int ret = poll(pollFds.data(), pollFds.size(), 1000); // 1-second timeout
        if (ret == -1)
		{
            if (errno == EINTR)
			{
                continue; // Interrupted by signal
            }
            perror("poll");
            break;
        }

        if (ret == 0)
		{
			checkTimeouts();
            continue;
        }

        for (size_t i = 0; i < pollFds.size(); ++i)
		{
            if (pollFds[i].revents & POLLIN)
			{
                if (std::find(serverFds.begin(), serverFds.end(), pollFds[i].fd) != serverFds.end())
				{
                    // Event on listening socket, accept new connection
                    if (!handleNewConnection(pollFds[i].fd))
					{
                        std::cerr << "Failed to handle new connection on fd " << pollFds[i].fd << "\n";
                    }
                }
				else
				{
					ClientHandler client = clientHandlers[i];
                    // Event on client socket, handle client request
                    if (!handleClient(client))
                    {
                        std::cout << "Disconnecting client on fd " << pollFds[i].fd << "\n";
                        close(client.fd);
                        pollFds.erase(pollFds.begin() + i);
                        clientHandlers.erase(clientHandlers.begin() + i);
                        --i;
                    }
                    else
                    {
                        client.setLastActivity(std::time(NULL));
                    }
                }
            }
        }

        // Check for idle clients and disconnect if necessary
        checkTimeouts();
    }
}

/*
	Method: 		Server::closeAllSockets
	Description: 	Handles a new incoming connection
*/
bool Server::handleNewConnection(int server_fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1)
	{
        if (errno != EWOULDBLOCK && errno != EAGAIN)
		{
            perror("accept");
            return false;
        }
        return true; // No pending connections
    }
    this->clientHandlers.push_back(ClientHandler(client_fd));

    // Add client to pollFds
    struct pollfd pfd;
    pfd.fd = client_fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    this->pollFds.push_back(pfd);

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    std::cout << "Accepted connection from " << client_ip << ":" << ntohs(client_addr.sin_port)
              << " with fd " << client_fd << "\n";

    return true;
}

struct MatchClientFd
{
    int client_fd;

    MatchClientFd(int fd) : client_fd(fd) {}

    bool operator()(const pollfd& pfd) const
	{
        return pfd.fd == client_fd;
    }
};

/*
	Method: 		Server::closeAllSockets
	Description: 	Handles client communication
*/
bool Server::handleClient(ClientHandler& client)
{
    if (!client.readRequest())
	{
        std::cout << "Failed to read client request.\n";
        return false;
    }

	std::string requestPath = client.

    if (!client.sendResponse())
	{
        std::cout << "Failed to send response to client.\n";
        return false;
    }

    return true;
}
