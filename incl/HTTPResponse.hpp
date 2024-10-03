#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <map>

class HTTPResponse
{
	private:
		int									_status_code;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

	public:

		HTTPResponse();
		~HTTPResponse();

		void setStatusCode(int code);
		void addHeader(const std::string & name, const std::string & value);
		void setBody(const std::string & body);
};

#endif