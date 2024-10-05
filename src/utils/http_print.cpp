#include "../../incl/utils.hpp"
#include "../../incl/HTTPRequest.hpp"
#include "../../incl/HTTPResponse.hpp"

void printRequest(const HTTPRequest & request)
{
	if (request.getState() == COMPLETE)
	{
		// request line
		std::cout << request.getRequestLine();
		// headers
		printHTTPHeaders(request.getHeaders());
		//body
		std::cout << request.getBody();
	}
	else
		std::cout << "Request incomplete, can't print.\n";
}

void printHTTPHeaders(const std::map<std::string, std::string> & headers)
{
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
	{
		std::cout << (*it).first << ": " << (*it).second << "\n";
	}
	std::cout << "\r\n\r\n";
}

void printHTTPHeader(std::map<std::string, std::string> & headers, const std::string & name)
{
	std::map<std::string, std::string>::iterator header_iterator = headers.find(name);
	if (header_iterator != headers.end())
	{
		std::cout << (*header_iterator).second << std::endl;
	}
	else
		std::cout << "'" << name << "'" << " not a header." << std::endl;
}
