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


// Constants
const int BACKLOG = 10;

class Server {
	public:
		Server(const std::string& configPath);
		~Server();
		bool start();
		void stop();

	private:
		std::string configPath;
		std::vector<int> serverPorts;
		std::vector<int> serverFds;
		std::vector<ClientHandler> clientHandlers;
		std::vector<struct pollfd> pollFds;
		bool running;

		bool parseConfig();
		bool setupServerSockets();
		void eventLoop();
		bool handleNewConnection(int listen_fd);
		bool handleClient(ClientHandler & client);
		void closeAllSockets();
		void checkTimeouts(void);
};

void signalHandler(int signum);

#endif
