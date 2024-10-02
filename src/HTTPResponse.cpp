#include "../incl/HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
{
}

HTTPResponse::~HTTPResponse()
{
}

void HTTPResponse::setStatusCode(int code)
{
	_status_code = code;
}

void HTTPResponse::addHeader(const std::string &name, const std::string &value)
{
	(void)name;
	(void)value;
}

void HTTPResponse::setBody(const std::string &body)
{
	(void)body;
}
