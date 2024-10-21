#include "../incl/Response.hpp"

Response::Response()
{
}

Response::~Response()
{
}

int Response::getStatusCode(void)
{
	return _status_code;
}

const std::string &Response::getStatusMessage(void)
{
	return _status_message;
}

const std::map<std::string, std::string> &Response::getHeaders(void)
{
	return _headers;
}

const std::string &Response::getBody(void)
{
	return _body;
}

void Response::setBody(const std::string &body)
{
	_body = body;
}

void Response::setHeader(const std::string &key, const std::string &value)
{
	_headers[key] = value;
}

void Response::setStatusCode(int code)
{
	_status_code = code;
}

void Response::setStatusMessage(const std::string &message)
{
	_status_message = message;
}


bool Response::isComplete(void) const
{
	return (_state == COMPLETE_RES);
}

bool Response::hasRequiredHeaders(void) const
{
	return (false);
}

bool Response::isValidStatusCode(void) const
{
	return (false);
}