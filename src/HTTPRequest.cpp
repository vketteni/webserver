#include "../incl/HTTPRequest.hpp"

HTTPRequest::HTTPRequest() : _complete(false), _method("")
{
}

HTTPRequest::HTTPRequest(const std::string &raw) : _complete(false), _method("")
{
	parseRequest(raw);
}

HTTPRequest &HTTPRequest::operator=(const HTTPRequest &other)
{
	if (this != &other)
	{
		this->_body = other._body;
		this->_headers = other._headers;
		this->_method = other._method;
		this->_uri = other._uri;
		this->_complete = other._complete;
	}
	return *this;
}

std::map<std::string, std::string> HTTPRequest::parseHeaders(const std::string &request)
{
	(void)request;
    std::map<std::string, std::string> headers;
    // Header parsing logic here (not implemented yet)
    return headers;
}

std::string HTTPRequest::parseRequestMethod(const std::string &request)
{
    std::istringstream iss(request);
    std::string method;
    iss >> method;
    return method;
}

bool HTTPRequest::parseRequest(const std::string &raw)
{
	this->_method = parseRequestMethod(raw);
	this->_headers = parseHeaders(raw);
/*




std::string ClientHandler::parseHeaderValue(const std::string &headerName)
{
	size_t	pos;

	std::istringstream request_stream(_request_buffer);
	std::string line;
	while (std::getline(request_stream, line))
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

*/

	(void)raw;
	_complete = true;
	return true;
}

const std::string& HTTPRequest::getMethod() const
{
	return _method;
}

const std::string &HTTPRequest::getUri() const
{
	return _uri;
}

bool HTTPRequest::isComplete(void) const
{
	return _complete;
}
