#include "../incl/HTTPRequest.hpp"

HTTPRequest::HTTPRequest() : _complete(false), _method(""), _raw(std::string())
{
}

HTTPRequest::HTTPRequest(const std::string &raw) : _complete(false), _method(""), _raw(raw)
{
	parseRequest(_raw);
}

HTTPRequest &HTTPRequest::operator=(const HTTPRequest &other)
{
	if (this != &other)
	{
		this->_body = other._body;
		this->_headers = other._headers;
		this->_method = other._method;
		this->_raw = other._raw;
		this->_request_line = other._request_line;
		this->_uri = other._uri;
		this->_complete = other._complete;
	}
	return *this;
}

bool HTTPRequest::parseRequest(const std::string &raw)
{
	(void)raw;
	_complete = true;
	return true;
}

const std::string& HTTPRequest::getMethod() const
{
	return _method;
}

const std::string &HTTPRequest::getRaw() const
{
	return _raw;
}

const std::string &HTTPRequest::getUri() const
{
	return _uri;
}

bool HTTPRequest::isComplete(void) const
{
	return _complete;
}
