#include "../incl/ClientConnection.hpp"

ClientConnection::ClientConnection(int client_fd, HostConfig & server_config) : _lastActivity(std::time(NULL)),
	_server_config(server_config), fd(client_fd), timeout(TIMEOUT_DURATION) {}

ClientConnection::ClientConnection(const ClientConnection &other) : _server_config(other._server_config), fd(other.fd), timeout(other.timeout)
{
	 _lastActivity = other._lastActivity;
}

ClientConnection &ClientConnection::operator=(const ClientConnection &other)
{
	// TODO
	(void)other;
	return *this;
}

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
    ssize_t bytes_read = recv(this->fd, _buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0)
	{
        // Handle disconnection or error
        return false;
    }
    _buffer[bytes_read] = '\0';
	RequestParser parser = _request_parser;
    parser.appendData(std::string(_buffer));
    if (!parser.parse())
	{
        return false;
    }
    if (parser.isComplete())
	{
	    Request request = parser.getRequest();
		Response response;
		
        std::string root = _server_config.routes[0].root;
        request.setUri(root + request.getUri());

        HeaderProcessor headerProcessor;
        headerProcessor.processHeaders(request);

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

        if (!sendResponse(response))
		{
            return false;
        }
        parser.reset();
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
