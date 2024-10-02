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
		bool handleNewConnection(int server_fd);
		void handleClientSocket(int fd, int & poll_index);
		bool handleClient(int client_fd);
		void checkTimeouts(void);
		void disconnectClient(int fd, int & poll_index, int client_index);

		// Helper Functions
		void closeAllSockets();
		bool isHostSocket(int fd);
		bool Server::isClientSocket(int fd);
};

void signalHandler(int signum);

#endif
