#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

#include <ctime>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>


#include "HTTPResponse.hpp"
#include "HTTPRequest.hpp"

# define BUFFER_SIZE 4096

const int TIMEOUT_DURATION = 10;

class ClientHandler
{
	private:
		std::string		_request_buffer;
		time_t			_lastActivity;
		HTTPRequest		_request;
		HTTPResponse	_response;

	public:
		const int		fd;
		const int		timeout;

		ClientHandler(int client_fd);
		ClientHandler(const ClientHandler & other);
		ClientHandler & operator=(const ClientHandler & other);
		~ClientHandler();

		time_t getLastActivity(void);
		void setLastActivity(time_t last_activity);

		bool readRequest(void);
		bool sendResponse(void);
		bool handleUpload(void);

		std::map<std::string, std::string> parseHeaders(const std::string& request);
		std::string parseRequestMethod(const std::string& request);
		std::string parseHeaderValue(const std::string & headerName);

		bool sendBasicResponse(const std::string& body, int status_code, const std::string& content_type);
};

#endif