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
volatile bool keep_running = true;

void signalHandler(int signum)
{
    std::cout << "\nInterrupt signal (" << signum << ") received. Shutting down server.\n";
    keep_running = false;
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
    for (std::vector<struct pollfd>::iterator it = poll_fds.begin(); it != poll_fds.end(); ++it)
	{
        close((*it).fd);
    }
    poll_fds.clear();
}

void Server::checkTimeouts(void)
{
    time_t currentTime = std::time(NULL);

	header("Server::checkTimeouts");
	debug(currentTime);

    for (std::vector<ClientHandler>::iterator it = client_handlers.begin(); it != client_handlers.end(); ++it)
    {
		ClientHandler client = *it;
		time_t last_activity = client.getLastActivity();
        if (difftime(currentTime, last_activity) > client.timeout)
        {
            std::cout << "Client on fd " << client.fd << " timed out. Disconnecting...\n";

			close(client.fd);
			int i = host_ports.size() + std::distance(client_handlers.begin(), it);
            poll_fds.erase(poll_fds.begin() + i);
			--it;
            client_handlers.erase(it + 1);

        }
    }
}

bool Server::isHostSocket(int fd)
{
    return std::find(host_fds.begin(), host_fds.end(), fd) != host_fds.end();
}

bool Server::isClientSocket(int fd)
{
	int i = 0;
	while (i < (int)client_handlers.size())
	{
		if (client_handlers[i].fd != fd)
			return false;
		i++;
	}
	return true;
}

/*
	 Method: 		Server::eventLoop
	 Description: 	Main event loop using poll()
 */
void Server::eventLoop()
{
    std::cout << "Server is running. Press Ctrl+C to stop.\n";
    while (running && keep_running)
	{
        int poll_result = poll(poll_fds.data(), poll_fds.size(), 1000); // 1-second timeout
        if (poll_result == -1)
		{
            if (errno == EINTR)
			{
                continue; // Interrupted by signal
            }
            perror("poll");
            break;
        }

        if (poll_result == 0)
		{
			checkTimeouts();
            continue;
        }

		handlePollEvents();
		checkTimeouts();
	}
}

void Server::handlePollEvents()
{
        for (int i = 0; i < (int)poll_fds.size(); ++i)
		{
			int fd = poll_fds[i].fd;
            if (poll_fds[i].revents & POLLIN)
			{
                if (isHostSocket(fd))
				{
                    // Event on host socket, accept new connection
                    if (!handleNewConnection(fd))
					{
                        std::cerr << "Failed to handle new connection on fd " << fd << "\n";
                    }
                }
				else
				{
					handleClientSocket(fd, i);
				}
			}
		}
}

void Server::handleClientSocket(int fd, int & poll_index)
{
    int client_index = poll_index - host_fds.size();
    if (client_index < 0 || client_index >= (int)client_handlers.size())
    {
        std::cerr << "Client index out of bounds (fd: " << fd << ")\n";
        running = false;
        return;
    }

    if (!handleClient(fd))
    {
        std::cout << "Disconnecting client on fd " << fd << "\n";
        disconnectClient(fd, poll_index, client_index);
    }
    else
    {
        client_handlers[client_index].setLastActivity(std::time(NULL));
    }
}

void Server::disconnectClient(int fd, int & poll_index, int client_index)
{
    close(fd);
    client_handlers.erase(client_handlers.begin() + client_index);
    poll_fds.erase(poll_fds.begin() + poll_index);
	--poll_index;
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
    this->client_handlers.push_back(ClientHandler(client_fd));

    // Add client to poll_fds
    struct pollfd pfd;
    pfd.fd = client_fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    this->poll_fds.push_back(pfd);

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
        return client.fd == fd;
    }
};

/*
	Method: 		Server::closeAllSockets
	Description: 	Handles client communication
*/
bool Server::handleClient(ClientHandler & client)
{
    if (!client.readRequest())
	{
        std::cout << "Failed to read client request.\n";
        return false;
    }

//	std::string requestPath = client.

    if (!client.sendResponse())
	{
        std::cout << "Failed to send response to client.\n";
        return false;
    }

    return true;
}
