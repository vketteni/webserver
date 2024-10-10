#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include "Request.hpp"

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
		std::string		_buffer;
		Request 		_request;
		RequestState	_state;

	public:
		RequestParser();

		void appendData(const std::string & data);
		bool parse(void);
		bool isComplete() const;
		void reset(void);

		RequestState getState(void) const;
		const Request & getRequest(void) const;

	private:
		bool extractBody();
		bool extractRequestLine(void);
		bool extractHeaders(void);
		bool processParsingHeaders(void);
};

void trim(std::string& s);

#endif