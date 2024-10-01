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

bool ClientHandler::readRequest() 
{
    const size_t chunkSize = 1024;
    char buffer[chunkSize];
    ssize_t bytes_read = recv(this->fd, buffer, sizeof(buffer), 0);
    
	while (true)
	{
        if (bytes_read == -1) 
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN) 
            {
                // No data available for now, wait for the next poll() event
                break;
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
            std::cout << "Client disconnected.\n";
            return false;
        }

		_request_buffer.append(buffer, bytes_read);
		
		if (_request_buffer.find("\r\n\r\n") != std::string::npos) 
		{
			HTTPRequest httpRequest;
			if (!httpRequest.parseRequest(_request_buffer)) 
			{
				// If parsing fails, return an error
				std::cerr << "Failed to parse HTTP request.\n";
				return false;
			}

			this->_request = httpRequest; // Store the parsed request in a class member
			return true;  // Successfully parsed the request
		}
	}
    // If the request is incomplete, return true to wait for more data
    return true;
}


// {
//     const size_t chunk_size = 1024;
//     char buffer[chunk_size];
//     ssize_t bytes_read;

//     while (true) 
//     {
//         // Attempt to read from the client socket
//         bytes_read = recv(this->fd, buffer, sizeof(buffer), 0);

//         if (bytes_read == -1) 
//         {
//             if (errno == EWOULDBLOCK || errno == EAGAIN) 
//             {
//                 // No data available for now, wait for the next poll() event
//                 break;
//             }
//             else 
//             {
//                 perror("recv");
//                 return false;
//             }
//         }
//         else if (bytes_read == 0) 
//         {
//             // Client has closed the connection
//             std::cout << "Client disconnected.\n";
//             return false;
//         }

//         // Append the data to the request buffer
//         _request_buffer.append(buffer, bytes_read);

//         // Check if we've received the end of the headers (double CRLF)
//         if (_request_buffer.find("\r\n\r\n") != std::string::npos) 
//         {
//             // Headers are fully received; now we can proceed to process the request
//             std::cout << "Full HTTP headers received.\n";

//             // Handle Content-Length to know if there is a body
//             std::string content_length_str = parseHeaderValue("Content-Length");
//             if (!content_length_str.empty()) 
//             {
//                 int contentLength = std::stoi(content_length_str);
//                 if (_request_buffer.size() >= contentLength + _request_buffer.find("\r\n\r\n") + 4) 
//                 {
//                     std::cout << "Full request body received.\n";
//                     return true;
//                 }
//             }
//             else 
//             {
//                 // No Content-Length (could be a GET request), just return
//                 return true;
//             }
//         }
//     }

//     // We haven't received the full request yet, but we should return true
//     // so the event loop can continue and call readRequest() again later.
//     return true;
// }

bool ClientHandler::sendResponse(void)
{
	if (!_request.isComplete())
		return true;

	// const std::string method = _request.getMethod();
    // if (method == "GET") 
    // {
    //     // Handle GET request logic
    //     _response.setStatusCode(200);  // OK
    //     _response.setBody("Hello, World!");  // Simple response body
    //     _response.addHeader("Content-Type", "text/plain");
    // } 
    // else if (method == "POST") 
    // {
    //     // Handle POST request (e.g., file upload)
    //     if (!handleUpload()) 
    //     {
    //         _response.setStatusCode(500);  // Internal Server Error
    //         _response.setBody("Error handling upload");
    //     } 
    //     else 
    //     {
    //         _response.setStatusCode(200);  // OK
    //         _response.setBody("Upload successful");
    //     }
    //     _response.addHeader("Content-Type", "text/plain");
    // }
    // else 
    // {
    //     // Method not supported
    //     _response.setStatusCode(405);  // Method Not Allowed
    //     _response.setBody("Method not allowed");
    // }

	// Use FileManager here (not implemented)
	std::string filePath = "res/example.html";
    std::ifstream file(filePath.c_str());  // Open the file
    if (!file) {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    
    std::ostringstream contents;
    contents << file.rdbuf();  // Read the file buffer into the stream

    // Simple response example
    std::string body = contents.str();
    return sendBasicResponse(body, 200, "text/html; charset=UTF-8");
}

std::map<std::string, std::string> ClientHandler::parseHeaders(const std::string &request)
{
	(void)request;
    std::map<std::string, std::string> headers;
    // Header parsing logic here (not implemented yet)
    return headers;
}

std::string ClientHandler::parseRequestMethod(const std::string &request)
{
    std::istringstream iss(request);
    std::string method;
    iss >> method;
    return method;
}

std::string ClientHandler::parseHeaderValue(const std::string &headerName)
{
	size_t	pos;

	std::istringstream requestStream(_request_buffer);
	std::string line;
	while (std::getline(requestStream, line))
	{
		if (line.find(headerName) != std::string::npos)
		{
			pos = line.find(": ");
			if (pos != std::string::npos)
			{
				return (line.substr(pos + 2)); // Skip ": " and get the value
			}
		}
	}
	return ("");
}

bool ClientHandler::sendBasicResponse(const std::string& body, int statusCode, const std::string& contentType) 
{
    std::ostringstream response;
    response << "HTTP/1.1 " << statusCode << " OK\r\n"
             << "Content-Type: " << contentType << "\r\n"
             << "Content-Length: " << body.size() << "\r\n"
             << "Connection: close\r\n\r\n"
             << body;

    std::string responseStr = response.str();
    ssize_t bytes_sent = send(this->fd, responseStr.c_str(), responseStr.size(), 0);

    if (bytes_sent == -1) 
	{
        perror("send");
        return false;
    }

    std::cout << "Sent response: " << responseStr << "\n";
    return true;
}