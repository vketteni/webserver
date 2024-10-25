#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <cstdio>
#include <cstring>
#include <algorithm>

#include "Request.hpp"
#include "Response.hpp"
#include "Utils.hpp"
#include "HeaderProcessor.hpp"

# define BUFFER_SIZE 4096

enum RequestState {
    READ_REQUEST_LINE,
    READ_HEADERS,
    PROCESS_HEADERS,
    READ_BODY,
    COMPLETE,
    ERROR
};

class RequestParser
{
	private:
		std::vector<char>	_buffer;
		Request 			_request;
		Response 			_response;
		RequestState		_state;

	public:
		RequestParser();

		bool readAndParse(int client_fd);
		bool isComplete() const;
		void reset(void);

		RequestState getState(void) const;
		const Request & getRequest(void) const;
		const Response & getResponse(void) const;

	private:
		bool parse(void);
		bool extractBody();
		bool extractRequestLine(void);
		bool extractHeaders(void);
		bool processHeadersBeforeBody(void);
};

#endif
