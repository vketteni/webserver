#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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

// Constants
const int BACKLOG = 10000;

class Server {
	public:
		Server(const std::string& config_path);
		~Server();
		bool start();
		void stop();

	private:
		std::string config_path;
		std::vector<int> host_fds;
		std::vector<ClientConnection> client_connections;
		std::vector<struct pollfd> poll_fds;
		std::vector<HostConfig> host_configs;
		bool running;

		// Server Setup
		bool parseConfig(std::vector<int> & host_ports);
		bool setupServerSockets(std::vector<int> & host_ports);

		// Event Loop
		void eventLoop();
		void processIOEvents();
		bool acceptNewClient(std::vector<struct pollfd>::iterator poll_iterator);
		bool processClientRequest(std::vector<struct pollfd>::iterator poll_iterator);
		void checkTimeouts(void);
		void disconnectClient(std::vector<struct pollfd >::iterator poll_iterator);
		// std::string checkRedirect(const std::string& requested_path);


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
