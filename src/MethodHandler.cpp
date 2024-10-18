#include "../incl/MethodHandler.hpp"


AbstractMethodHandler* getHandlerForMethod(const std::string& method)
{
    if (method == "GET")
	{
        return new GetRequestHandler();
    }
	else if (method == "POST")
	{
        return new PostRequestHandler();
    }
	else if (method == "DELETE")
	{
        return new DeleteRequestHandler();
    }

    return NULL;
}

void GetRequestHandler::invoke(Request& request, Response& response)
{
    header("GetRequestHandler");
    if (isCGI(request.getUri()))
    {
		processCGI(request, response);
        return ;  // CGI wurde erfolgreich behandelt
    }

	std::ifstream file(request.getUri().c_str());
    if (!file)
    {
        response.setStatusMessage("File not found");
        response.setStatusCode(404);
        response.setHeader("Connection", "close");
        return ;
    }

    std::ostringstream contents;
    contents << file.rdbuf();  // Read the file buffer into the stream

    file.close();

    response.setBody(contents.str());
    response.setStatusMessage("OK");
    response.setStatusCode(200);
    response.setHeader("Connection", "keep-alive");
}

void PostRequestHandler::invoke(Request& request, Response& response)
{
    if (isCGI(request.getUri()))
    {
		processCGI(request, response);
        return ;  // CGI wurde erfolgreich behandelt
    }
}

void DeleteRequestHandler::invoke(Request& request, Response& response)
{
    (void)request;
    (void)response;
}

void AbstractMethodHandler::processCGI(const Request& request, Response& response)
{
    CGIExecutor cgi_executor;
	std::vector<std::string> env_vars;
	env_vars.push_back("REQUEST_METHOD=" + request.getMethod());

	// CGI-Skript ausführen, GET oder POST spezifisch
	if (request.getMethod() == "GET")
	{
		env_vars.push_back("QUERY_STRING=" + request.getQueryString());
	}
	else if (request.getMethod() == "POST")
	{
		std::stringstream ss;
		ss << request.getBody().size();
		env_vars.push_back("CONTENT_LENGTH=" + ss.str());
	}

	// Skript ausführen
	if (!cgi_executor.executeCGI(request, response))
	{
		response.setStatusCode(500);
		response.setStatusMessage("Internal Server Error");
	}
	else
	{
		response.setStatusCode(200);
		response.setStatusMessage("OK");
	}
}

bool isCGI(const std::string & path)
{
	return path.find("/cgi-bin/") == 0 || path.find(".cgi") != std::string::npos
		|| path.find(".py") != std::string::npos;
}