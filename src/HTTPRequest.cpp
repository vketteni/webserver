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
	(void)other;
	return *this;
}

bool HTTPRequest::parseRequest(const std::string &raw)
{
	(void)raw;
	return false;
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
