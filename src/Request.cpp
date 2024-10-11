#include "../incl/Request.hpp"
// #include "Request.hpp"

Request::Request()
{
}

// Request &Request::operator=(const Request &other)
// {
// 	if (this != &other)
// 	{
// 		this->_state = other._state;
// 		this->_buffer = other._buffer;
// 		this->_method = other._method;
// 		this->_http_version = other._http_version;
// 		this->_uri = other._uri;
// 		this->_headers = other._headers;
// 		this->_body = other._body;
// 	}
// 	return (*this);
// }

const std::string Request::getRequestLine() const
{
	return (_method + " " + _uri + " " + _http_version);
}

const std::string &Request::getBody() const
{
	return (_body);
}

const std::string &Request::getMethod() const
{
	return (_method);
}

const std::string &Request::getUri() const
{
	return (_uri);
}

const std::string &Request::getVersion() const
{
	return (_http_version);
}

const std::map<std::string, std::string> &Request::getHeaders() const
{
	return (_headers);
}

size_t Request::getContentLength(void)
{
	return _content_length;
}

void Request::setMethod(const std::string &method)
{
	_method = method;
}

void Request::setBody(const std::string &body)
{
	_body = body;
}

void Request::setVersion(const std::string &version)
{
	_http_version = version;
}

void Request::setUri(const std::string &uri)
{
	_uri = uri;
}

void Request::setContentLength(size_t content_length)
{
	_content_length = content_length;
}

void Request::setHeaders(const std::map<std::string, std::string> &headers)
{
	_headers = headers;
}

void Request::setCgiEnvVariables()
{
	std::stringstream ss;
	ss << _content_length;
	std::string content_length = ss.str();

	setenv("REQUEST_METHOD", _method.c_str(), 1);
	setenv("SCRIPT_NAME", _uri.c_str(), 1);
	setenv("CONTENT_LENGTH", content_length.c_str(), 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
}
