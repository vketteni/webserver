#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

#include <ctime>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h>

#include "Debug.hpp"
#include "MethodHandler.hpp"
#include "HeaderProcessor.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "RequestParser.hpp"
#include "ConfigParser.hpp"
# define BUFFER_SIZE 4096

const int TIMEOUT_DURATION = 10;

class ClientConnection
{
	private:
		char			_buffer[BUFFER_SIZE];
		time_t			_lastActivity;
		RequestParser	_request_parser;
		HostConfig &	_host_config;

	public:
		const int		fd;
		const int		host_port;
		const int		timeout;

		ClientConnection(int client_fd, HostConfig & host_config, int port);
		~ClientConnection();

		time_t getLastActivity(void);
		void setLastActivity(time_t last_activity);
		bool processRequest(void);
 void sendRedirect(const std::string& redirect_url, int statusCode = 301);
	private:
		bool readAndParseRequest();
		bool processResponse(Request & request, Response & response);
		bool sendResponse(Response & response);
		bool sendBasicResponse(const std::string& body, int status_code, const std::string& content_type);
};

#endif