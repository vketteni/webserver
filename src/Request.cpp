#include "../incl/Request.hpp"

Request::Request()
	: _method("GET"), _uri("/")
{}

const std::string &Request::getMethod() const
{
	return (_method);
}

const std::string &Request::getUri() const
{
	return (_uri);
}

void Request::setMethod(const std::string &method)
{
	_method = method;
	setFirstLine(buildFirstLine());
}

void Request::setUri(const std::string &uri)
{
	_uri = uri;
	setFirstLine(buildFirstLine());
}

const std::string Request::buildFirstLine() const
{
	return (_method + " " + _uri + " " + _protocol_version);
}

const std::string Request::buildQueryString() const
{
	size_t pos = _uri.find('?');
	if (pos != std::string::npos)
	{
		return _uri.substr(pos);
	}
	return ("");
}
