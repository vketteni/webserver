#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include "Request.hpp"

enum ResponseState {
	WRITE_STATUS_LINE,
	WRITE_HEADERS,
	WRITE_BODY,
	COMPLETE_RES
};

class Response
{
	private:
		ResponseState						_state;
		int									_status_code;
		std::string							_status_message;
		std::string							_chunk;
		std::string							_status_line;
		std::string							_body;
		std::map<std::string, std::string>	_headers;


	public:
		Response();
		~Response();
		
		// Response Getters & Setter
		int getStatusCode(void);
		const std::string & getStatusMessage(void);
		const std::map<std::string, std::string> & getHeaders(void);
		const std::string & getBody(void);
		void setStatusCode(int code);
		void setStatusMessage(const std::string & message);
		
		// Status checkers
		bool isComplete() const;
		bool isValidStatusCode() const;
		bool hasRequiredHeaders() const;

		// State transition management
		ResponseState getState() const;
};

#endif