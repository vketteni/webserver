#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <map>
#include "HTTPRequest.hpp"

enum ResponseState {
	WRITE_STATUS_LINE,
	WRITE_HEADERS,
	WRITE_BODY,
	COMPLETE_RES
};

class HTTPResponse
{
	private:
		ResponseState						_state;
		std::string							_chunk;
		std::string							_status_line;
		std::string							_body;
		std::map<std::string, std::string>	_headers;
		int									_status_code;


	public:
		HTTPResponse();
		~HTTPResponse();

		// Generate response in steps
		void generateStatusLine(const HTTPRequest &request);
		void generateHeaders(const HTTPRequest &request);
		void generateBody(const HTTPRequest &request);

		// Status checkers
		bool isComplete() const;
		bool isValidStatusCode() const;
		bool hasRequiredHeaders() const;

		// State transition management
		ResponseState getState() const;
};

#endif