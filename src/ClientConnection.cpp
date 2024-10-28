#include "../incl/ClientConnection.hpp"
#include "ClientConnection.hpp"

ClientConnection::ClientConnection(int client_fd, ServerConfig host_config, int port)
    : _lastActivity(std::time(NULL)), _host_config(host_config), fd(client_fd), host_port(port), timeout(TIMEOUT_DURATION)
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

void ClientConnection::methodHandler(Request & request, Response & response, const LocationConfig & route, const ServerConfig & server_config)
{
	// printConfigLocations(server_config);
	std::string request_method = request.getMethod();

    if (std::find_if(route.methods.begin(), route.methods.end(), MatchMethod(request_method)) == route.methods.end())
	{
        response.setStatusCode(405);
        response.setHeader("Allow", joinMethods(route.methods));
        return;
    }
	std::string root = !route.root.empty() ? route.root : server_config.root;

	std::string uri_path = request.getUri();
    if (uri_path.find('?') != std::string::npos)
	{
        uri_path = uri_path.substr(0, uri_path.find('?')); // Remove query string
    }
    std::string relative_path = uri_path.substr(route.path.length());
	std::string file_path = root + relative_path;

	request.setUri(file_path);

	if (isDirectory(file_path))
	{
        std::string index_file = !route.index.empty() ? route.index : "index.html";
        std::string index_path = file_path + "/" + index_file;
        if (fileExists(index_path))
            file_path = index_path;
		else if (route.autoindex == "on")
		{
            // Generate directory listing
            response.setStatusCode(200);
            response.setBody(generateDirectoryListing(file_path, uri_path));
            response.setHeader("Content-Type", "text/html");
            return;
        }
		else
		{
            // Directory access is forbidden
            response.setStatusCode(403);
            return;
        }
    }

	AbstractMethodHandler * method_handler = getHandlerForMethod(request.getMethod());
	if (method_handler)
	{
		method_handler->invoke(request, response);
	}

	return ;
}

void ClientConnection::handleErrorResponse(Response & response, ServerConfig & server_config)
{
	(void) server_config;
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
	std::string error_page = generateErrorPage(status_code, status_message);
	response.setBody(error_page);
	response.setHeader("Content-Type", "text/html; charset=utf-80");
	std::ostringstream oss;
	oss << error_page.size();
	response.setHeader("Content-Length", oss.str());
	response.setStatusMessage(status_message);
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
