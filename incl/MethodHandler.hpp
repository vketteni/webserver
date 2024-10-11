#ifndef HTTPREQUESTHANDLERS_HPP
#define HTTPREQUESTHANDLERS_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "FileManager.hpp"


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

#endif