#include "../incl/Server.hpp"

static int	setNonBlocking(int fd)
{
	int	flags;

	if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
	{
		perror("fcntl F_GETFL");
		return (-1);
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		perror("fcntl F_SETFL O_NONBLOCK");
		return (-1);
	}
	return (0);
}

// Global
volatile bool	g_keep_running = true;

void	signalHandler(int signum)
{
	std::cout << "\nInterrupt signal (" << signum << ") received. Shutting down server.\n";
	g_keep_running = false;
}

// Constructor
Server::Server(const std::string &config_path) : config_path(config_path),
	running(false)
{
}

// Destructor
Server::~Server()
{
	stop();
}

/*
	Method: 		Server::parseConfig
	Description: 	Parses the configuration file to extract ports
*/
bool Server::parseConfig(std::vector<int> &host_ports)
{
	
    ConfigParser config_parser;
    if (!config_parser.parseConfig(config_path))
    {
        std::cerr << "Failed to parse configuration file: " << config_path << "\n";
        return false;
    }
    host_configs = config_parser.getHostConfigs();
    if (host_configs.empty())
    {
        std::cerr << "No valid server configurations found.\n";
        return false;
    }

    for (std::map<int, HostConfig>::const_iterator server_it = host_configs.begin(); server_it != host_configs.end(); ++server_it)
    {
        const HostConfig& HostConfig = (*server_it).second;
        std::cout << "port is: " << HostConfig.port << "\n";
        if (HostConfig.port > 0 && HostConfig.port < 65536)
        {
            host_ports.push_back(HostConfig.port);
        }
        else
        {
            std::cerr << "Invalid port number in configuration: " << HostConfig.port << "\n";
            return false;
        }
    }
    if (host_ports.empty())
    {
        std::cerr << "No valid ports found in configuration.\n";
        return false;
    }
    std::cout << "Configured to listen on ports: ";
    for (std::vector<int>::iterator port_it = host_ports.begin(); port_it != host_ports.end(); ++port_it)
    {
        std::cout << *port_it << " ";
    }
    std::cout << "\n";
    return true;
}

/*
	Method: 		Server::setupServerSockets
	Description: 	Sets up listening sockets for each server port
*/
bool Server::setupServerSockets(std::vector<int> &host_ports)
{
	int	server_fd;
	int	opt;
	int	server_port;
	struct sockaddr_in addr;
	struct pollfd pfd;

	for (std::vector<int>::iterator port_it = host_ports.begin(); port_it != host_ports.end(); ++port_it)
	{
		server_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (server_fd == -1)
		{
			perror("socket");
			return (false);
		}
		// Allow address reuse
		opt = 1;
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt,
				sizeof(opt)) == -1)
		{
			perror("setsockopt SO_REUSEADDR");
			close(server_fd);
			return (false);
		}
		// Bind to the specified port on all interfaces
		server_port = *port_it;
		std::memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
		addr.sin_port = htons(server_port);
		if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		{
			perror("bind");
			close(server_fd);
			return (false);
		}
		// Set the socket to non-blocking mode
		if (setNonBlocking(server_fd) == -1)
		{
			close(server_fd);
			return (false);
		}
		// Start listening
		if (listen(server_fd, BACKLOG) == -1)
		{
			perror("listen");
			close(server_fd);
			return (false);
		}
		debug(server_port);
		host_port_and_fds.push_back(std::make_pair(server_port, server_fd));
		// Add to poll_fds
		pfd.fd = server_fd;
		pfd.events = POLLIN;
		pfd.revents = 0;
		poll_fds.push_back(pfd);
		std::cout << "Listening on port " << server_port << " with fd " << server_fd << "\n";
	}
	return (true);
}

/*
	Method: 		Server::start
	Description: 	Starts the server
*/
bool Server::start()
{
	std::vector<int> host_ports;
	if (!parseConfig(host_ports))
	{
		return (false);
	}
	if (!setupServerSockets(host_ports))
	{
		return (false);
	}
	this->running = true;
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
		Method: 		Server::eventLoop
		Description: 	Main event loop using poll()
 */
void Server::eventLoop()
{
	std::cout << "Server is running. Press Ctrl+C to stop.\n";
	while (this->running && g_keep_running)
	{
		int poll_result = poll(poll_fds.data(), poll_fds.size(), 1000);
			// 1-second timeout
		if (poll_result == -1)
		{
			if (errno == EINTR)
			{
				continue ; // Interrupted by signal
			}
			perror("poll");
			break ;
		}
		if (poll_result == 0)
		{
			checkTimeouts();
			continue ;
		}
		processIOEvents();
		checkTimeouts();
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
	//time_t	last_activity;

	/* time_t currentTime = std::time(NULL);
	for (std::vector<struct pollfd>::iterator poll_iterator = poll_fds.begin(); poll_iterator != poll_fds.end(); ++poll_iterator)
	{
		std::vector<ClientConnection>::iterator client_iterator = std::find_if(client_connections.begin(),
				client_connections.end(), MatchClientFd(poll_iterator->fd));
		if (client_iterator != client_connections.end())
		{
			last_activity = client_iterator->getLastActivity();
			if (difftime(currentTime, last_activity) > client_iterator->timeout)
			{
				std::cout << "Client on fd " << client_iterator->fd << " timed out. Disconnecting...\n";
				close(client_iterator->fd);
				client_connections.erase(client_iterator);
				poll_iterator = poll_fds.erase(poll_iterator);
			}
		}
	} */
}

bool Server::isHostSocket(int fd)
{
	for (std::vector<std::pair<int, int> >::iterator it = host_port_and_fds.begin(); it != host_port_and_fds.end(); ++it)
	{
		if (it->second == fd)
			return true;
	}
	return false;
}

bool Server::isClientSocket(int fd)
{
	std::list<ClientConnection>::iterator client = std::find_if(client_connections.begin(),
			client_connections.end(), MatchClientFd(fd));
	if (client != client_connections.end())
		return true;
	return false;
}

void Server::processIOEvents()
{
	int	fd;

	for (int i = 0; i < (int)poll_fds.size(); ++i)
	{
		fd = poll_fds[i].fd;
		if (poll_fds[i].revents & POLLIN)
		{
			if (isHostSocket(fd))
			{
				// Event on host socket, accept new connection
				if (!acceptNewClient(poll_fds.begin() + i))
				{
					std::cerr << "Failed to handle new connection on fd " << fd << "\n";
				}
			}
			else if (!processClientRequest(poll_fds.begin() + i))
			{
				std::cout << "Disconnecting client on fd " << fd << "\n";
				disconnectClient(poll_fds.begin() + i);
				--i;
			}
		}
	}
}

void Server::disconnectClient(std::vector<struct pollfd>::iterator poll_iterator)
{
	close(poll_iterator->fd);
	std::list<ClientConnection>::iterator client_iterator = std::find_if(client_connections.begin(),
			client_connections.end(), MatchClientFd(poll_iterator->fd));
	if (client_iterator != client_connections.end())
		client_connections.erase(client_iterator);
	poll_fds.erase(poll_iterator);
}

/*
	Method: 		Server::closeAllSockets
	Description: 	Handles a new incoming connection
*/
bool Server::acceptNewClient(std::vector<struct pollfd>::iterator poll_iterator)
{
	struct sockaddr_in	client_addr;

	if (!isHostSocket(poll_iterator->fd))
	{
		std::cerr << "Error: Host doesn't exist." << std::endl;
		return false;
	}

	socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(poll_iterator->fd, (struct sockaddr *)&client_addr,
			&client_len);
	if (client_fd == -1)
	{
		if (errno != EWOULDBLOCK && errno != EAGAIN)
		{
            perror("accept");
            return false;
        }
        return true; // No pending connections
    }

	// TODO: make descriptive smaller function
	for (int i = 0; i < (int)host_port_and_fds.size(); ++i)
	{
		if (host_port_and_fds[i].second == poll_iterator->fd)
		{
			int port = host_port_and_fds[i].first;
			
			ClientConnection new_connection(client_fd, host_configs[port], port);
			this->client_connections.push_back(new_connection);
		}
	}

    // Add client to poll_fds
    struct pollfd pfd;
    pfd.fd = client_fd;
    pfd.events = POLLIN | POLLOUT;
    pfd.revents = 0;
    this->poll_fds.push_back(pfd);

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    std::cout << "Accepted connection from " << client_ip << ":" << ntohs(client_addr.sin_port)
              << " with fd " << client_fd << "\n";

    return true;
}

/*
	Method: 		Server::processClientRequest
	Description: 	Handles client request reading and response sending
*/
bool Server::processClientRequest(std::vector<struct pollfd>::iterator poll_iterator)
{
	//	this check is unnessessary compute intensive -> std::vector allows random lookup via index O(1) which we should use instead
	std::list<ClientConnection>::iterator client = std::find_if(client_connections.begin(),
			client_connections.end(), MatchClientFd(poll_iterator->fd));
	if (client == client_connections.end())
	{
		// sendErrorResponse(poll_iterator->fd, 400);
		// std::cerr << "Error, client doesn't exist.\n";
		return false;
	}
	if (!client->processRequest())
	{
		client->setLastActivity(std::time(NULL));
		// sendErrorResponse(poll_iterator->fd, 500);
		//   std::cerr << "Failed to process request from client.\n";
		return false;
	}
	client->setLastActivity(std::time(NULL));
	return true;
}

// std::string Server::getRedirect(const std::string& requested_path)
// {
// 	for (std::vector<HostConfig>::iterator it = host_configs.begin(); it != host_configs.end(); ++it)
// 	{
// 		const HostConfig& config = *it;
// 		std::map<std::string, std::string>::const_iterator redirect_it = config.redirects.find(requested_path);
// 		if (redirect_it != config.redirects.end())
// 		{
// 			return redirect_it->second; // Gibt den neuen Pfad zurück
// 		}
// 	}
// 	return ""; // Kein Redirect gefunden
// }


std::string Server::translateUriToCgiPath(const std::string &path)
{
	std::string cgi_root = "/var/www/cgi-bin";
	std::string cgi_path = cgi_root + path.substr(path.find("/cgi-bin/") + 8);
	return cgi_path;
}

