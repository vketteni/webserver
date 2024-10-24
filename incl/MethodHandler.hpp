#ifndef HTTPREQUESTHANDLERS_HPP
#define HTTPREQUESTHANDLERS_HPP

#include <vector>
#include <sstream>

#include "Request.hpp"
#include "Response.hpp"
#include "FileManager.hpp"
#include "CGIExecutor.hpp"
#include "Debug.hpp"

class AbstractMethodHandler
{
	public:
		virtual void invoke(Request& request, Response& response) = 0;
		virtual ~AbstractMethodHandler() {}
	protected:
		void processCGI(Request& request, Response& response);
		bool shouldRedirect(const Request& request);
		void setRedirect(Response& response, const std::string& location);
		void setErrorResponse(Response& response, int statusCode, const std::string& message);
		void buildResponse(Response& response, int statusCode, const std::string& statusMessage, const std::string& body, const std::string& connection);

};

// TODO: Implement as dependency injection (*)-(*)
AbstractMethodHandler* getHandlerForMethod(const std::string& method);

/*
	Method Handlers
*/
class GetRequestHandler : public AbstractMethodHandler
{
	public:
		void invoke(Request& request, Response& response);
};

class PostRequestHandler : public AbstractMethodHandler
{
	public:
		void invoke(Request& request, Response& response);
		
};

class DeleteRequestHandler : public AbstractMethodHandler
{
	public:
		void invoke(Request& request, Response& response);
};


bool isCGI(const std::string &path);

#endif