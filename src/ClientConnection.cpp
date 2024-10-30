#include "../incl/ClientConnection.hpp"

ClientConnection::ClientConnection(int client_fd, ServerConfig host_config, int port, Logger &logger)
    : _logger(logger), _lastActivity(std::time(NULL)), _host_config(host_config), fd(client_fd), host_port(port), timeout(TIMEOUT_DURATION)
{}


ClientConnection::~ClientConnection()
{}

time_t ClientConnection::getLastActivity(void)
{
	return (_lastActivity);
}

void ClientConnection::setLastActivity(time_t last_activity)
{
	_lastActivity = last_activity;
}

bool ClientConnection::processRequest()
{
	if (!_request_parser.readAndParse(this->fd))
		return false;
    if (_request_parser.isComplete())
	{
	    Request request = _request_parser.getRequest();
		Response response = _request_parser.getResponse();
		_request_parser.reset();
		if (!processResponse(request, response))
			return false;
		if (!sendResponse(response))
            return false;
    }
    return true;
}

bool ClientConnection::processResponse(Request & request, Response & response)
{
	headerHandler(request, response);

    std::string normalized_uri = (request.getUri());
    // std::string normalized_uri = request.normalizeUri(request.getUri());
    const LocationConfig* matching_location = findMatchingLocation(normalized_uri, _host_config.locations);

    if (matching_location && matching_location->redirect_status != 0)
	{
        response.setStatusCode(matching_location->redirect_status);
        response.setHeader("Location", matching_location->redirect_path);
        return true;
    }
    else if (matching_location)
        methodHandler(request, response, *matching_location, _host_config);
	else
        response.setStatusCode(404);

    if (response.getStatusCode() >= 400)
		handleErrorResponse(response, _host_config);
	else {
        _logger.logInfo(response.getStatusCode(), "Successfully processed request to " ); //+ response.getPath()
    }

	return true;
}

void ClientConnection::headerHandler(Request & request, Response & response)
{
	std::map<std::string, HeaderHandler> handlers;
	std::set<std::string> required;

	setup_post_body_handlers(handlers, required);

	HeaderProcessor hp(request, response);
	hp.processHeaders(handlers, required);

	return ;
}

void ClientConnection::methodHandler(Request & request, Response & response, const LocationConfig & location, const ServerConfig & server_config)
{
	// printConfigLocations(server_config);
	std::string request_method = request.getMethod();

    if (std::find_if(location.methods.begin(), location.methods.end(), MatchMethod(request_method)) == location.methods.end())
	{
        response.setStatusCode(405);
        response.setHeader("Allow", joinMethods(location.methods));
        return;
    }

	AbstractMethodHandler * method_handler = getHandlerForMethod(request.getMethod());
	if (method_handler)
	{
		method_handler->invoke(request, response, location, server_config);
	}
	free(method_handler);

	return ;
}

void ClientConnection::handleErrorResponse(Response & response, ServerConfig & server_config)
{
	//(void) server_config;
	int status_code = response.getStatusCode();
	std::string	status_message;

	switch (status_code)
	{
		case 400: status_message = "Bad Request"; break;
		case 403: status_message = "Forbidden"; break;
		case 404: status_message = "Not Found"; break;
		case 405: status_message = "Method Not Allowed"; break;
		case 500: status_message = "Internal Server Error"; break;
		default: status_message = "Error"; break;
	}
	
	_logger.logError(status_code, "Error: " + status_message + " for request to " ); // + response.getPath()

	std::string file_path = server_config.root + server_config.error_pages[status_code];
	pretty_debug(file_path);
	if (!server_config.error_pages[status_code].empty() && fileExists(file_path))
	{
		std::ifstream file(file_path.c_str());
		if (file)
		{
			std::string error_page_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			pretty_debug(error_page_content);
			response.setBody(error_page_content);
			response.setHeader("Content-Typr", "text/html; charset=utf-8");
			std::ostringstream oss;
			oss << error_page_content.size();
			response.setHeader("Content-Length", oss.str());
		}
		else
		{
			response.setBody(generateErrorPage(status_code, status_message));
			response.setHeader("Content-Type", "text/html; charset=utf-8");
		}
	}
	else
	{
		response.setBody(generateErrorPage(status_code, status_message));
		response.setHeader("Content-Type", "text/html; charset=utf-8");
	}
	response.setStatusMessage(status_message);
}

bool ClientConnection::fileExists(const std::string &path)
{
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}

std::string ClientConnection::generateErrorPage(int status_code, const std::string &status_message)
{
	std::ostringstream	html;

	html << "<!DOCTYPE html><html><head><meta charset=\"UTF-8\">";
	html << "<title>" << status_code << " " << status_message << "</title></head><body>";
	html << "<h1>" << status_code << " " << status_message << "</h1>";
	switch (status_code)
	{
		case 400:
			html << "<p>Bad Request: The server could not understand the request.</p>";
			break;
		case 403:
			html << "<p>Forbidden: Access to this resource is denied.</p>";
			break;
		case 404:
			html << "<p>Not Found: The requested resource could not be found.</p>";
			break;
		case 405:
			html << "<p>Method Not Allowed: This HTTP method is not supported for the requested URL.</p>";
			break;
		case 500:
			html << "<p>Internal Server Error: The server encountered an unexpected condition.</p>";
			break;
		default:
			html << "<p>An error occurred while processing your request.</p>";
			break;
	}
	html << "<hr><p><em>VinOmHennis Web Server</em></p></body></html>";

	return html.str();
}

bool ClientConnection::sendResponse(Response & response)
{
	const std::string new_line = "\r\n";
	std::ostringstream oss;
	oss << response.getVersion() << " " << response.getStatusCode() << " " << response.getStatusMessage() << new_line;
	oss << "Content-Type: " << "text/html; charset=utf-8" << new_line;
	oss << "Content-Length: " << response.getBody().size() << new_line;
	const std::map<std::string, std::string> & headers = response.getHeaders();
	for (std::map<std::string, std::string>::const_iterator headerIterator = headers.begin(); headerIterator != headers.end(); ++headerIterator)
	{

		oss << headerIterator->first << ": " << headerIterator->second << new_line;
	}
	oss << new_line;
	oss << response.getBody();
	std::string responseStr = oss.str();
	ssize_t bytesSent = send(fd, responseStr.c_str(), responseStr.size(), 0);
	return bytesSent == static_cast<ssize_t>(responseStr.size());
}

// bool ClientConnection::shouldRedirect(const Request& request) {
//     // Beispiel: Wenn die URI "/old-path" ist, leite auf "/new-path" um
//     if (request.getUri() == "/old-path") {
//         return true;
//     }
//     return false;
// }

// void ClientConnection::setRedirect(Response& response, const std::string& location) {
//     response.setStatusCode(301);
//     response.setStatusMessage("Moved Permanently");
//     response.setHeader("Location", location);
// }

void ClientConnection::setErrorResponse(Response& response, int statusCode, const std::string& message) {
    response.setStatusCode(statusCode);
    response.setStatusMessage(message);
    response.setHeader("Connection", "close");
}

void ClientConnection::buildResponse(Response& response, int statusCode, const std::string& statusMessage, const std::string& body, const std::string& connection) {
    response.setStatusCode(statusCode);
    response.setStatusMessage(statusMessage);
    response.setBody(body);
    response.setHeader("Connection", connection);
}