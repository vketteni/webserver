#include "../incl/ClientConnection.hpp"

ClientConnection::ClientConnection(int client_fd, HostConfig & server_config)
    : _lastActivity(std::time(NULL)), _host_config(server_config), fd(client_fd), timeout(TIMEOUT_DURATION)
{}

ClientConnection::~ClientConnection()
{
}

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
	if (!readAndParseRequest())
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

bool ClientConnection::readAndParseRequest()
{
    ssize_t bytes_read = recv(this->fd, this->_buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0)
	{
        // Handle disconnection or error
        return false;
    }
    _buffer[bytes_read] = '\0';

    _request_parser.appendData(std::string(_buffer));
    if (!_request_parser.parse())
	{
		this->_request_parser.reset();
        return false;
	}
	return true;
}

bool ClientConnection::processResponse(Request & request, Response & response)
{
	// Process Uri
	HostConfig & host_config = _host_config;
	std::string root = host_config.root;
	// TODO: Use host_config.route_table to check redirections
	// TODO: Use host_config.route_table to check redirections
	request.setUri(root + request.getUri());
	debug(request.getUri());

	// Process Headers
	std::map<std::string, HeaderHandler> handlers;
	std::set<std::string> required;

	HeaderProcessor hp(request, response);
	setup_post_body_handlers(handlers, required);
	if(!hp.processHeaders(handlers, required))
	{
		this->_request_parser.reset();
		return false;
	}

	// Invoke Method handler
	AbstractMethodHandler* method_handler = getHandlerForMethod(request.getMethod());
	if (method_handler)
	{
		method_handler->invoke(request, response);
		delete method_handler;
	}
	else
	{
		response.setStatusCode(405);
		response.setStatusMessage("Method Not Allowed");
	}
	return true;
}

bool ClientConnection::sendResponse(Response & response) {
	debug(response.getStatusCode());
    std::ostringstream oss;
    oss << "HTTP/1.1 " << response.getStatusCode() << " " << response.getStatusMessage() << "\r\n";
    oss << "Content-Type: " << "text/html; charset=utf-8" << "\r\n";
    oss << "Content-Length: " << response.getBody().size() << "\r\n";
	const std::map<std::string, std::string> & headers = response.getHeaders();
	for (std::map<std::string, std::string>::const_iterator headerIterator = headers.begin(); headerIterator != headers.end(); ++headerIterator)
	{
        
        oss << headerIterator->first << ": " << headerIterator->second << "\r\n";
        debug(oss.str());
    }
    oss << "\r\n";
    oss << response.getBody();
    std::string responseStr = oss.str();
    // debug(responseStr);
    ssize_t bytesSent = send(fd, responseStr.c_str(), responseStr.size(), 0);
    return bytesSent == static_cast<ssize_t>(responseStr.size());
}

bool ClientConnection::sendBasicResponse(const std::string &body, int status_code, const std::string &content_type)
{
    std::ostringstream response;
    response << "HTTP/1.1 " << status_code << " OK\r\n"
             << "Content-Type: " << content_type << "\r\n"
             << "Content-Length: " << body.size() << "\r\n"
             << "Connection: keep-alive\r\n\r\n"
             << body;

    std::string response_str = response.str();
    ssize_t bytes_sent = send(this->fd, response_str.c_str(), response_str.size(), 0);

    if (bytes_sent == -1)
	{
        perror("send");
        return false;
    }

    std::cout << "Sent response: " << response_str << "\n";
    return true;
}
