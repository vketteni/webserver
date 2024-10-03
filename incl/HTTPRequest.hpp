#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <map>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <sstream>

#define GET 0
#define POST 1
#define DELETE 2

class HTTPRequest
{
	private:
		bool 								_complete;
		std::string							_method;
		std::string							_http_version;
		std::string							_uri;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		std::map<std::string, std::string> parseHeaders(const std::string &request);
		std::string parseRequestMethod(const std::string &request);

	public:
		HTTPRequest();
		HTTPRequest(const std::string & raw);
		HTTPRequest & operator=(const HTTPRequest & other);

		bool parseRequest(const std::string & raw);
		const std::string& getMethod() const;
		const std::string& getRaw() const;
		const std::string& getBody() const;
		const std::string& getHeaders() const;
		const std::string& getUri() const;
		const std::string& getRequestLine() const;
		bool isComplete() const;


};

#endif