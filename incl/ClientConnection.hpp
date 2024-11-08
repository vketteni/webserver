#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

#include <ctime>
#include <string>
#include <map>
#include <vector>
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
#include "Logger.hpp"

const int TIMEOUT_DURATION = 10;

class ClientConnection
{
	private:
		Logger&			_logger;
		time_t			_lastActivity;
		const ServerConfig	_host_config;
		RequestParser	_request_parser;

	public:
		const int		fd;
		const int		host_port;
		const int		timeout;

		ClientConnection(int client_fd, const ServerConfig &host_config, int port, Logger &logger);
		~ClientConnection();

		time_t		getLastActivity(void);
		void		setLastActivity(time_t last_activity);
		bool		processRequest(void);

	private:
		bool		processResponse(Request & request, Response & response);
		bool		sendResponse(Response & response);
		bool		fileExists(const std::string & path);

		void		headerHandler(Request & request, Response & response);
		void		methodHandler(Request & request, Response & response, const LocationConfig & location, const ServerConfig & config);
		void		handleErrorResponse(Response & response, const ServerConfig & server_config);
		std::string	generateErrorPage(int status_code, const std::string &status_message);
};

struct MatchRoute
{
	const std::string & route;

	/*
		Match object to find LocationConfig.path in search collection
	*/
    MatchRoute(const std::string & route) : route(route) {}

    bool operator()(const LocationConfig & location) const
	{
		if (location.path == route)
		{
			std::cerr << (location.path) << std::endl;
			std::cerr << (location.redirect_path) << std::endl;
			std::cerr << (location.redirect_status) << std::endl;
			return  true;
		}
		return false;
    }
};

struct MatchMethod
{
    const std::string & _method;

	/*
		Match object to find LocationConfig.path in search collection
	*/
    MatchMethod(const std::string & method) : _method(method) {}

    bool operator()(const std::string & method) const
	{
			if (method == _method)
			{
				return  true;
			}

		return false;
    }
};

#endif
