#include "../incl/AMessage.hpp"

AMessage::AMessage()
	: _protocol_version("HTTP/1.1")
{}

AMessage::~AMessage() {}

const std::string &AMessage::getBody() const
{
	return _body;
}

void AMessage::setBody(const std::string &body)
{
	_body = body;
}

const std::map<std::string, std::string> &AMessage::getHeaders() const
{
	return _headers;
}

const std::string &AMessage::getHeaderOrDefault(const std::string &key, const std::string &default_value)
{
	if (_headers[key].empty())
		_headers[key] = default_value;
	return _headers[key];
}

void AMessage::setHeader(const std::string &key, const std::string &value)
{
	_headers[key] = value;
}

void AMessage::setHeaders(const std::map<std::string, std::string> &headers)
{
	_headers = headers;
}

const std::string &AMessage::getFirstLine() const
{
	return _first_line;
}

const std::string &AMessage::getVersion() const
{
	return _protocol_version;
}

void AMessage::setFirstLine(const std::string &line)
{
	_first_line = line;
}

void AMessage::setVersion(const std::string &version)
{
	_protocol_version = version;
}
