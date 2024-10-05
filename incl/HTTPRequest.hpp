#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <map>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <sstream>
#include "Debug.hpp"

#define GET 0
#define POST 1
#define DELETE 2

enum ParseState {
    READ_REQUEST_LINE,
    READ_HEADERS,
    READ_BODY,
    COMPLETE,
    ERROR
};

class HTTPRequest
{
	private:
		ParseState							_state;
		std::string							_buffer;

		std::string							_method;
		std::string							_http_version;
		std::string							_uri;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		bool parseRequestLine(const std::string & line);
		bool parseHeaders(const std::string & header_block);

	public:
		HTTPRequest();
		HTTPRequest & operator=(const HTTPRequest & other);

		bool parse(const std::string & data);
	
		const std::string getRequestLine() const;
		const std::string & getBody() const;
		const std::string & getMethod() const;
		const std::string & getUri() const;
		const std::string & getVersion() const;
		const std::map<std::string, std::string> & getHeaders() const;
		ParseState getState(void) const;
		bool isComplete() const;
		void reset(void);
};

#endif