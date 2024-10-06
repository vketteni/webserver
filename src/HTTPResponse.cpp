#include "../incl/HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
{
}

HTTPResponse::~HTTPResponse()
{
}

bool HTTPResponse::isComplete(void) const
{
	return _state == COMPLETE_RES;
}

bool HTTPResponse::hasRequiredHeaders(void) const
{
	return false;
}

bool HTTPResponse::isValidStatusCode(void) const
{
	return false;
}