#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>

class HTTPResponse
{
	private:
	public:
		HTTPResponse();
		~HTTPResponse();

		void setStatusCode(int code);
		void addHeader(const std::string & name, const std::string & value);
		void setBody(const std::string & body);
};

#endif