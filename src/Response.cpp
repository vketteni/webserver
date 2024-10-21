#include "../incl/Response.hpp"

Response::Response()
	: _status_code(200), _status_message("OK")
{}

Response::~Response() {}

int Response::getStatusCode(void)
{
	return _status_code;
}

const std::string &Response::getStatusMessage(void)
{
	return _status_message;
}

void Response::setStatusCode(int code)
{
	_status_code = code;
}

void Response::setStatusMessage(const std::string &message)
{
	_status_message = message;
}

const std::string Response::buildFirstLine() const
{
	std::stringstream ss;
	ss << _status_code;
	std::string status_code_str = ss.str();
	
	return _protocol_version + " " + status_code_str + "" + _status_message;
}

