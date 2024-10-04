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
#include "ClientHandler.hpp"
#include "Debug.hpp"

// Constants
const int BACKLOG = 10;

class Server {
	public:
		Server(const std::string& config_path);
		~Server();
		bool start();
		void stop();

	private:
		std::string config_path;
		std::vector<int> host_ports;
		std::vector<int> host_fds;
		std::vector<ClientHandler> client_handlers;
		std::vector<struct pollfd> poll_fds;
		bool running;

		// Server Setup 
		bool parseConfig();
		bool setupServerSockets();

		// Event Loop
		void eventLoop();
		void handlePollEvents();
		bool handleNewConnection(std::vector<struct pollfd>::iterator poll_iterator);
		bool handleClientSocket(std::vector<struct pollfd>::iterator poll_iterator);
		void checkTimeouts(void);
		void disconnectClient(std::vector<struct pollfd >::iterator poll_iterator);

		// Helper Functions
		void closeAllSockets();
		bool isHostSocket(int fd);
		bool isClientSocket(int fd);
};

void signalHandler(int signum);

struct MatchClientFd
{
    int fd;
    
    MatchClientFd(int fd) : fd(fd) {}
    
    bool operator()(const ClientHandler& client) const
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


// ** function noClientHandlert used :

/*
std::string ClientHandler::parseHeaderValue(const std::string &headerName)
{
	size_t	pos;

	std::istringstream request_stream(_buffer);
	std::string line;
	while (std::getline(request_stream, line))
	{
		if (line.find(headerName) != std::string::npos)
		{
			pos = line.find(": ");
			if (pos != std::string::npos)
			{
				return (line.substr(pos + 2)); // Skip ": " and get the value
			}
		}
	}
	return ("");
}
*/
