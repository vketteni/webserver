#include "../incl/Request.hpp"
// #include "Request.hpp"

Request::Request()
{
	_content_length = 0;
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

const std::string &Request::getQueryString() const
{
	return (_query_string);
}

const std::string &Request::getHeader(const std::string &key) const
{
	return (_headers.at(key));
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

void Request::setQueryString(const std::string &query_string)
{
	_query_string = query_string;
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

void Request::setHeader(const std::string &key, const std::string &value)
{
	_headers[key] = value;
}

void Request::setHeaders(const std::map<std::string, std::string> &headers)
{
	_headers = headers;
}
