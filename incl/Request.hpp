#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "Debug.hpp"

class Request
{
	private:
		std::string							_method;
		std::string							_http_version;
		std::string							_uri;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		size_t								_content_length;

	public:
		Request();
		// Request & operator=(const Request & other);

		const std::string getRequestLine() const;
		const std::string &getBody() const;
		const std::string &getMethod() const;
		const std::string &getUri() const;
		const std::string &getVersion() const;
		const std::string &getHeader(const std::string &key) const;
		const std::map<std::string, std::string> &getHeaders() const;
		size_t getContentLength(void);

		void setMethod(const std::string & method);
		void setBody(const std::string & body);
		void setVersion(const std::string & version);
		void setUri(const std::string & uri);
		void setContentLength(size_t content_length);
		void setHeader(const std::string &key, const std::string &value);
		void setHeaders(const std::map<std::string, std::string> & headers);

};

#endif
