#include "../incl/Server.hpp"

// Global
volatile bool	g_keep__running = true;

void	signalHandler(int signum)
{
	std::cout << "\nInterrupt signal (" << signum << ") received. Shutting down server.\n";
	g_keep__running = false;
}

// Constructor
Server::Server(const std::string &config_path)
	: _config_path(config_path), _running(false), _logger("access.log", "error.log", INFO)
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
bool Server::parseConfig()
{
 	ConfigParser config_parser(_config_path);

	if (!config_parser.parse())
		return false;
	config_parser.buildServerConfigs(_host_configs);


	if (_host_configs.empty())
	{
		std::cerr << "No valid server configurations found.\n";
		return false;
	}
    for (std::map<int, ServerConfig>::const_iterator server_it = _host_configs.begin(); server_it != _host_configs.end(); ++server_it)
    {
        std::cout << "port is: " << (*server_it).first << "\n";
        if ((*server_it).first > 0 && (*server_it).first < 65536)
            ;
        else
        {
            std::cerr << "Invalid port number in configuration: " << (*server_it).first << "\n";
            return false;
        }
    }
    return true;
}

/*
	Method: 		Server::setupServerSockets
	Description: 	Sets up listening sockets for each server port
*/
void Server::set_non_blocking_and_start_listen(int host_fd)
{
	int	flags;

	if ((flags = fcntl(host_fd, F_GETFL, 0)) == -1)
	{
		perror("fcntl F_GETFL");
		throw std::runtime_error("Error: Failed to get file descriptor flags: F_GETFL");
	}
	if (fcntl(host_fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		perror("fcntl F_SETFL O_NONBLOCK");
		throw std::runtime_error("Error: Failed to set flags on host file descriptor: O_NONBLOCK.");
	}

	if (listen(host_fd, BACKLOG) == -1)
	{
		perror("listen");
		close(host_fd);
		throw std::runtime_error("Error: Failed to listen to host file descriptor.");
	}
}

void Server::bind_port_to_interfaces(int host_port, int host_fd, ServerConfig & host_config)
{
		struct sockaddr_in addr;
		std::memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(host_config.host.c_str()); // 0.0.0.0
		addr.sin_port = htons(host_port);
		if (bind(host_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		{
			perror("bind");
			close(host_fd);
			throw std::runtime_error("Error: Failed to bind to a host port.");
		}
}

void Server::allow_address_reuse(int host_fd)
{
	int	opt;
	opt = 1;
	if (setsockopt(host_fd, SOL_SOCKET, SO_REUSEADDR, &opt,
			sizeof(opt)) == -1)
	{
		perror("setsockopt SO_REUSEADDR");
		close(host_fd);
		throw std::runtime_error("Error: Failed to set socket option SO_REUSEADDR.");
	}
}

bool Server::setupServerSockets()
{
	std::map<int, ServerConfig>::iterator host_it = _host_configs.begin();
	for (; host_it != _host_configs.end(); ++host_it)
	{
		ServerConfig & host_config = host_it->second;
		int host_port = host_it->first;
		int	host_fd;
		if ((host_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("socket");
			return (false);
		}
		// Allow address reuse
		allow_address_reuse(host_fd);

		// Bind to the specified port on all interfaces
		bind_port_to_interfaces(host_port, host_fd, host_config);

		// Set the socket to non-blocking mode
		// Start listening
		set_non_blocking_and_start_listen(host_fd);

		// Create internal mapping between host port and file descriptor
		_host_port_and_fds.push_back(std::make_pair(host_port, host_fd));

		// Add pollfd to this instance
		struct pollfd pfd;
		pfd.fd = host_fd;
		pfd.events = POLLIN;
		pfd.revents = 0;
		_poll_fds.push_back(pfd);

		std::cout << "Listening on port " << host_port << " with fd " << host_fd << "\n";
	}
	return (true);
}

/*
	Method: 		Server::start
	Description: 	Starts the server
*/
bool Server::start()
{
	if (!parseConfig())
	{
		_logger.logError(400, "Failed to parse config file.");
		return (false);
	}
	if (!setupServerSockets())
	{
		_logger.logError(400, "Failed to setup server sockets.");
		return (false);
	}
	this->_running = true;
	eventLoop();
	return true;
}

/*
	Method: 		Server::stop
	Description: 	Stops the server
*/
void Server::stop()
{
	if (_running)
	{
		_running = false;
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
	while (this->_running && g_keep__running)
	{
		int poll_result = poll(_poll_fds.data(), _poll_fds.size(), -1);
			// 1-second timeout
		if (poll_result == -1)
		{
			if (errno == EINTR)
			{
				continue ; // Interrupted by signal
			}
			perror("poll");
			_logger.logError(500, "Poll error.");
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
	for (std::vector<struct pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end(); ++it)
	{
		close((*it).fd);
	}
	_poll_fds.clear();
}

void Server::checkTimeouts(void)
{
time_t currentTime = std::time(NULL);

    // Verwende explizite Typen statt `auto`
    for (std::vector<struct pollfd>::iterator poll_iterator = _poll_fds.begin();
         poll_iterator != _poll_fds.end(); ++poll_iterator) {

        std::list<ClientConnection>::iterator client_iterator =
            std::find_if(_client_connections.begin(), _client_connections.end(),
                         MatchClientFd(poll_iterator->fd));

        if (client_iterator != _client_connections.end()) {
            if (difftime(currentTime, client_iterator->getLastActivity()) > client_iterator->timeout) {
                std::cout << "Client on fd " << client_iterator->fd << " timed out. Disconnecting...\n";
                close(client_iterator->fd);

                // Entferne Client aus beiden Datenstrukturen
                _client_connections.erase(client_iterator);
                poll_iterator = _poll_fds.erase(poll_iterator);
                --poll_iterator;  // Iterator anpassen, um die Schleife nicht zu überspringen
            }
        }
    }
}

	//time_t	last_activity;

	/* time_t currentTime = std::time(NULL);
	for (std::vector<struct pollfd>::iterator poll_iterator = _poll_fds.begin(); poll_iterator != _poll_fds.end(); ++poll_iterator)
	{
		std::vector<ClientConnection>::iterator client_iterator = std::find_if(_client_connections.begin(),
				_client_connections.end(), MatchClientFd(poll_iterator->fd));
		if (client_iterator != _client_connections.end())
		{
			last_activity = client_iterator->getLastActivity();
			if (difftime(currentTime, last_activity) > client_iterator->timeout)
			{
				std::cout << "Client on fd " << client_iterator->fd << " timed out. Disconnecting...\n";
				close(client_iterator->fd);
				_client_connections.erase(client_iterator);
				poll_iterator = _poll_fds.erase(poll_iterator);
			}
		}
	} */


bool Server::isHostSocket(int fd)
{
	for (std::vector<std::pair<int, int> >::iterator it = _host_port_and_fds.begin(); it != _host_port_and_fds.end(); ++it)
	{
		if (it->second == fd)
			return true;
	}
	return false;
}

bool Server::isClientSocket(int fd)
{
	std::list<ClientConnection>::iterator client;
	client = std::find_if(
		_client_connections.begin(), _client_connections.end(), MatchClientFd(fd));
	return client != _client_connections.end();
}

void Server::processIOEvents()
{
	int	fd;

	for (size_t i = 0; i < _poll_fds.size(); ++i)
	{
		fd = _poll_fds[i].fd;
		if (!(_poll_fds[i].revents & (POLLIN | POLL_OUT)))
			continue;
		if (isHostSocket(fd))
		{
			// Event on host socket, accept new connection
			if (!acceptNewClient(_poll_fds.begin() + i))
			{
				std::cerr << "Failed to handle new connection on fd " << fd << "\n";
			}
		}
		else
		{
			std::list<ClientConnection>::iterator client;
			if ((client = std::find_if(_client_connections.begin(), _client_connections.end(), MatchClientFd(fd))) == _client_connections.end())
			{
				_logger.logError(400, "Client does not exist.");
				std::cout << "Disconnecting client on fd " << fd << "\n";
				disconnectClient(_poll_fds.begin() + i);
				--i;
				continue ;
			}
			else if (!client->processRequest())
			{
				_logger.logWarning(500, "Failed to process request from client.");
				std::cout << "Disconnecting client on fd " << fd << "\n";
				disconnectClient(_poll_fds.begin() + i);
				--i;
				continue ;
			}
			else
				_logger.logDebug(200, "Processed request successfully.");

			client->setLastActivity(std::time(NULL));

		}
	}
}

/*
	Method: 		Server::acceptNewClient
	Description: 	Accepts an incoming network connection.
					Wraps the socked file descriptor and the accepting host configuration inside an instance of ClientConnection.
					Also saves the socket file descriptor inside this instance of Server.
*/
bool Server::acceptNewClient(const std::vector<struct pollfd>::const_iterator poll_iterator)
{
	struct sockaddr_in client_addr;

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
			_logger.logError(400, "Error accepting new client.");
            return false;
        }
        return true; // No pending connections
    }

	// TODO: make descriptive smaller function
	for (int i = 0; i < (int)_host_port_and_fds.size(); ++i)
	{
		if (_host_port_and_fds[i].second == poll_iterator->fd)
		{
			int port = _host_port_and_fds[i].first;
			ClientConnection new_connection(client_fd, _host_configs[port], port, _logger);
			_client_connections.push_back(new_connection);
		}
	}

    // Add client to _poll_fds
    struct pollfd pfd;
    pfd.fd = client_fd;
    pfd.events = POLLIN;  //| POLLOUT; //hier der change von POLLIN | POLLOUT zu nur POLLIN Cpu 100 problem geloest
    pfd.revents = 0;
    _poll_fds.push_back(pfd);

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
 	std::stringstream ss;
 	ss << "Accepted connection from " << client_ip << ":" << ntohs(client_addr.sin_port);
 	_logger.logInfo(200, ss.str());

    return true;
}

void Server::disconnectClient(std::vector<struct pollfd>::iterator poll_iterator)
{
	std::list<ClientConnection>::iterator client_it = std::find_if(
		_client_connections.begin(),
		_client_connections.end(),
		MatchClientFd(poll_iterator->fd)
		);
	if (client_it != _client_connections.end())
		_client_connections.erase(client_it);

	close(poll_iterator->fd);
	_poll_fds.erase(poll_iterator);
}
