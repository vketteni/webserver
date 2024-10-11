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

    const std::string method = request.getMethod();
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

    response.setStatusMessage(contents.str());
    response.setStatusCode(200);
    response.setHeader("Connection", "keep-alive");
}

void PostRequestHandler::invoke(Request& request, Response& response)
{
    (void)request;
    (void)response;
}

void DeleteRequestHandler::invoke(Request& request, Response& response)
{
    (void)request;
    (void)response;
}