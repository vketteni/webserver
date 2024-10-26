#include "../incl/ClientConnection.hpp"

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

    std::string normalized_uri = request.normalizeUri(request.getUri());

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
	std::string request_method = request.getMethod();
    if (std::find(route.methods.begin(), route.methods.end(), request_method) == route.methods.end())
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
	(void)response;
	(void)server_config;
		//  response.statusCode = 500; // Internal Server Error
        //  response.statusCode = 501; // Not Implemented
		//  response.body = "403 Forbidden";
       	//  response.body = "404 Method Not Found";
       	//  response.body = "405 Method Not Allowed";






	// if (location_it != _host_config.locations.end() && location_it->redirect_status != 0)
	// {
	//     std::string redirect_url = location_it->redirect_path;
	//     request.setUri(redirect_url);
	// 	response.setHeader("Location", redirect_url);
	// 	response.setStatusCode(location_it->redirect_status);
	//     return true;
	// }
	// if (location_it != _host_config.locations.end())
	// {
	//     std::string new_route = location_it->root;
	//     request.setUri(_host_config.root + new_route);
	// }
	// else
	//     request.setUri(_host_config.root + request.getUri());
	
	// std::string newUrl = "/404.html";
	// struct stat fileInfo;
	// if (::stat(request.getUri().c_str(), &fileInfo) == -1)
	//     request.setUri(_host_config.root + newUrl);


// // Funktion zum Senden eines Redirects
// void ClientConnection::sendRedirect(const std::string& redirect_url, int status_code) {
//     std::ostringstream response;
//     response << "HTTP/1.1 " << status_code << " Moved Permanently\r\n"
//              << "Location: " << redirect_url << "\r\n"
//              << "Content-Length: 0\r\n"
//              << "Connection: close\r\n\r\n";

//     std::string response_str = response.str();
//     ssize_t bytes_sent = send(this->fd, response_str.c_str(), response_str.size(), 0);

//     if (bytes_sent == -1)
//     {
//         perror("send");
//     }

//     std::cout << "Sent redirect to: " << redirect_url << "\n";
// }
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
