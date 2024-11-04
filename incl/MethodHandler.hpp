#ifndef HTTPREQUESTHANDLERS_HPP
#define HTTPREQUESTHANDLERS_HPP

#include <vector>
#include <sstream>

#include "Request.hpp"
#include "Response.hpp"
#include "FileManager.hpp"
#include "CGIExecutor.hpp"
#include "ConfigParser.hpp"
#include "Debug.hpp"
#include "Utils.hpp"


class AbstractMethodHandler
{
	public:
		virtual void invoke(Request& request, Response& response, const LocationConfig & location, const ServerConfig & config) = 0;
		virtual ~AbstractMethodHandler() {}
};

// TODO: Implement as dependency injection (*)-(*)
AbstractMethodHandler* getHandlerForMethod(const std::string& method);

/*
	Method Handlers
*/
class GetRequestHandler : public AbstractMethodHandler
{
	public:
		void invoke(Request& request, Response& response, const LocationConfig & location, const ServerConfig & config);
};

class PostRequestHandler : public AbstractMethodHandler
{
	public:
		void invoke(Request& request, Response& response, const LocationConfig & location, const ServerConfig & config);
		
};

class DeleteRequestHandler : public AbstractMethodHandler
{
	public:
		void invoke(Request& request, Response& response, const LocationConfig & location, const ServerConfig & config);
};

#endif