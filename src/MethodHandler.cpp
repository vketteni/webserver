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
    if (shouldRedirect(request))
    {
        setRedirect(response, "/new-location");
        return; 
    }

    if (isCGI(request.getUri()))
    {
		processCGI(request, response);
        return ;  // CGI wurde erfolgreich behandelt
    }

	std::ifstream file(request.getUri().c_str());
    if (!file)
    {
        setErrorResponse(response, 404, "File Not Found");
        return ;
    }

    std::ostringstream contents;
    contents << file.rdbuf();  // Read the file buffer into the stream

    file.close();

    buildResponse(response, 200, "OK", contents.str(), "keep-alive");
}

void PostRequestHandler::invoke(Request& request, Response& response)
{
    // CGI-Anfragen behandeln
    if (isCGI(request.getUri()))
    {
        // CGI ausführen
        processCGI(request, response);

        // Verbindung und Body je nach Erfolg des CGI-Prozesses setzen
        if (response.getStatusCode() == 200)  // CGI erfolgreich
        {
            response.setHeader("Connection", "keep-alive");
            // Hier wird angenommen, dass der Body im response bereits von processCGI gesetzt wurde.
        }
        else  // CGI fehlgeschlagen
        {
            response.setHeader("Connection", "close");
            response.setBody("CGI execution failed.");
        }
        
        return;  // CGI wurde erfolgreich oder nicht erfolgreich behandelt
    }

    // Einfache Antwort für nicht-CGI POST-Anfragen
    response.setStatusMessage("POST request received");
    response.setStatusCode(200);
    response.setHeader("Connection", "keep-alive");
}




void DeleteRequestHandler::invoke(Request& request, Response& response)
{
    (void)request;
    (void)response;
}

void AbstractMethodHandler::processCGI(Request& request, Response& response)
{
    CGIExecutor cgi_executor;
	std::vector<std::string> env_vars;
	env_vars.push_back("REQUEST_METHOD=" + request.getMethod());

	// CGI-Skript ausführen, GET oder POST spezifisch
	if (request.getMethod() == "GET")
	{
		env_vars.push_back("QUERY_STRING=" + request.buildQueryString());
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

bool AbstractMethodHandler::shouldRedirect(const Request& request) {
    // Beispiel: Wenn die URI "/old-path" ist, leite auf "/new-path" um
    if (request.getUri() == "/old-path") {
        return true;
    }
    return false;
}

void AbstractMethodHandler::setRedirect(Response& response, const std::string& location) {
    response.setStatusCode(301);
    response.setStatusMessage("Moved Permanently");
    response.setHeader("Location", location);
}

void AbstractMethodHandler::setErrorResponse(Response& response, int statusCode, const std::string& message) {
    response.setStatusCode(statusCode);
    response.setStatusMessage(message);
    response.setHeader("Connection", "close");
}

void AbstractMethodHandler::buildResponse(Response& response, int statusCode, const std::string& statusMessage, const std::string& body, const std::string& connection) {
    response.setStatusCode(statusCode);
    response.setStatusMessage(statusMessage);
    response.setBody(body);
    response.setHeader("Connection", connection);
}