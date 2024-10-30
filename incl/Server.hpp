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
#include "StatusCode.hpp"
#include "Logger.hpp"

// Constants
const int BACKLOG = 10000;

class Server {
	public:
		Server(const std::string& config_path);
		~Server();
		bool start();
		void stop();

	private:
		std::string _config_path;
		std::vector<std::pair<int, int> > _host_port_and_fds;
		std::list<ClientConnection> _client_connections;
		std::vector<struct pollfd> _poll_fds;
		std::map<int, ServerConfig> _host_configs;
		bool _running;
		Logger _logger;

		// Server Setup
		bool parseConfig();
		bool setupServerSockets();

		// Event Loop
		void eventLoop();
		void processIOEvents();
		bool acceptNewClient(std::vector<struct pollfd>::iterator poll_iterator);
		bool processClientRequest(std::vector<struct pollfd>::iterator poll_iterator);
		void checkTimeouts(void);
		void disconnectClient(std::vector<struct pollfd >::iterator poll_iterator);
		// std::string getRedirect(const std::string& requested_path);


		// Helper Functions
		void closeAllSockets();
		bool isHostSocket(int fd);
		bool isClientSocket(int fd);

		// void sendErrorResponse(int client_fd, int status_code);

		bool isCGI(const std::string& path);
		std::string translateUriToCgiPath(const std::string& path);


		// routing table functions
		// lookup route bool doesRouteExist(std::string route);
		bool routeExists(std::string route);

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
