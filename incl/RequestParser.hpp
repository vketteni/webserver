#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "Utils.hpp"
#include "HeaderProcessor.hpp"

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
		Response 		_response;
		RequestState	_state;

	public:
		RequestParser();

		void appendData(const std::string & data);
		bool parse(void);
		bool isComplete() const;
		void reset(void);

		RequestState getState(void) const;
		const Request & getRequest(void) const;
		const Response & getResponse(void) const;

	private:
		bool extractBody();
		bool extractRequestLine(void);
		bool extractHeaders(void);
		bool processHeadersBeforeBody(void);
};

#endif
