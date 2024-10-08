#include "../incl/ClientHandler.hpp"

ClientHandler::ClientHandler(int client_fd) : _lastActivity(std::time(NULL)),
	fd(client_fd), timeout(TIMEOUT_DURATION) //, _request(NULL), _response()
{
}

ClientHandler::ClientHandler(const ClientHandler &other) : fd(other.fd), timeout(other.timeout)
{
	 _lastActivity = other._lastActivity;
}

ClientHandler &ClientHandler::operator=(const ClientHandler &other)
{
	// TODO
	(void)other;
	return *this;
}

ClientHandler::~ClientHandler()
{
}

time_t ClientHandler::getLastActivity(void)
{
	return (_lastActivity);
}

void ClientHandler::setLastActivity(time_t last_activity)
{
	_lastActivity = last_activity;
}

bool ClientHandler::handleRequest()
{
	const size_t chunk_size = 1024;
    char buffer[chunk_size];
	ssize_t bytes_read = recv(this->fd, buffer, chunk_size - 1, 0);
	if (bytes_read == -1)
	{
		if (errno == EWOULDBLOCK || errno == EAGAIN)
		{
			// No data available for now, wait for the next poll() event
			return true;
		}
		else
		{
			perror("recv");
			return false;
		}
	}
	else if (bytes_read == 0)
	{
		// Client has closed the connection
		return false;
	}
	buffer[chunk_size] = '\0';
	debug(buffer);

	while (true)
	{
        if (!_request.parse(std::string(buffer))) {

            // TODO: Handle parsing error, possibly send a response

            return false;
        }

        // Check if the request is complete
        if (_request.isComplete()) {

            // TODO: Process the complete request

			return true;
        }

	}
    // If the request is incomplete, return true to wait for more data
    return true;
}

bool ClientHandler::handleResponse(void)
{
    // If the request is incomplete, return true to wait for more data
	if (!_request.isComplete())
		return true;

	const std::string method = _request.getMethod();
    if (method == "GET")
    {
		header("GET");

		// Use FileManager (not implemented)
		std::string filePath = "/home/hwiemann/Core/webserv/webserv/res/example.html";
		std::ifstream file(filePath.c_str());  // Open the file
		if (!file) {
			throw std::runtime_error("Could not open file: " + filePath);
		}
		std::ostringstream contents;
		contents << file.rdbuf();  // Read the file buffer into the stream

		// Simple response example
		std::string body = contents.str();
		return sendBasicResponse(body, 200, "text/html; charset=UTF-8");

        // // Handle GET request logic
        // _response.setStatusCode(200);
        // _response.setBody(body);
        // _response.addHeader("Content-Type", "text/plain");
    }
    else if (method == "POST")
    {
		header("POST");
        // Handle POST request (e.g., file upload)
        if (!handleUpload())
        {
            // _response.setStatusCode(500);  // Internal Server Error
            // _response.setBody("Error handling upload");
        }
        else
        {
            // _response.setStatusCode(200);  // OK
            // _response.setBody("Upload successful");
        }
        // _response.addHeader("Content-Type", "text/plain");
    }
    else
    {
		header("NOT SUPPORTED");
        // Method not supported
        // _response.setStatusCode(405);  // Method Not Allowed
        // _response.setBody("Method not allowed");
    }

	return true;
}

bool ClientHandler::handleUpload(void)
{
	return false;
}

bool ClientHandler::sendBasicResponse(const std::string &body, int status_code, const std::string &content_type)
{
    std::ostringstream response;
    response << "HTTP/1.1 " << status_code << " OK\r\n"
             << "Content-Type: " << content_type << "\r\n"
             << "Content-Length: " << body.size() << "\r\n"
             << "Connection: great-wifi\r\n\r\n"
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
