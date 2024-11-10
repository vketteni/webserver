#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <arpa/inet.h>
#include <csignal>
#include "ClientConnection.hpp"
#include "Debug.hpp"
#include "ConfigParser.hpp"
#include "Request.hpp"
#include "CGIExecutor.hpp"
#include "Logger.hpp"

// Constants
const int BACKLOG = 126;

class Server {
	public:
		Server(const std::string& config_path);
		~Server();
		bool start();
		void stop();

	private:
		std::string 						_config_path;
		std::vector<std::pair<int, int> >	_host_port_and_fds;
		std::list<ClientConnection>			_client_connections;
		std::vector<struct pollfd>			_poll_fds;
		std::map<int, ServerConfig>			_host_configs;
		bool								_running;
		Logger								_logger;

		// Server Setup
		bool	parseConfig();
		bool	setupServerSockets();
		void	set_non_blocking_and_start_listen(int host_fd);
		void	bind_port_to_interfaces(int host_port, int host_fd, ServerConfig & host_config);
		void	allow_address_reuse(int host_fd);


		// Event Loop
		void	eventLoop();
		void	processIOEvents();
		bool	acceptNewClient(const std::vector<struct pollfd>::const_iterator poll_iterator);
		void	checkTimeouts(void);
		void	disconnectClient(std::vector<struct pollfd >::iterator poll_iterator);


		// Helper Functions
		void	closeAllSockets();
		bool	isHostSocket(int fd);
		bool	isClientSocket(int fd);
};

void signalHandler(int signum);

struct MatchClientFd
{
    int fd;

    MatchClientFd(int fd) : fd(fd) {}

    bool operator()(const ClientConnection& client) const
	{
        return client.fd == fd;
    }
};

struct MatchHostFd
{
    int fd;

    MatchHostFd(int fd) : fd(fd) {}

    bool operator()(const int & host_fd) const
	{
        return host_fd == fd;
    }
};

#endif
