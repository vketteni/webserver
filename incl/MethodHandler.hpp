#ifndef HTTPREQUESTHANDLERS_HPP
#define HTTPREQUESTHANDLERS_HPP

#include "Request.hpp"
#include "Response.hpp"


class AbstractMethodHandler
{
	public:
		virtual void invoke(Request& request, Response& response) = 0;
		virtual ~AbstractMethodHandler() {}
};

// ?? TODO ?? Implement as dependency injection (*)-(*)
AbstractMethodHandler* getHandlerForMethod(const std::string& method);

/*
	Method Handlers
*/
class GetRequestHandler : public AbstractMethodHandler
{
	public:
		void invoke(Request& request, Response& response)
		{
			// Handle GET request
			(void)request;
			(void)response;
		}
};

class PostRequestHandler : public AbstractMethodHandler
{
	public:
		void invoke(Request& request, Response& response)
		{
			// Handle POST request
			(void)request;
			(void)response;
		}
};

class DeleteRequestHandler : public AbstractMethodHandler
{
	public:
		void invoke(Request& request, Response& response)
		{
			// Handle POST request
			(void)request;
			(void)response;
		}
};

#endif