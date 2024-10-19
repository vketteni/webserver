#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <sstream>
#include "AMessage.hpp"

class Response : public AMessage
{
	private:
		int									_status_code;
		std::string							_status_message;

/* 	Inherited members:
		std::string _first_line;
		std::string _protocol_version;
		std::string _body;
		std::map<std::string, std::string> _headers; */

	public:
		Response();
		~Response();
		
		int getStatusCode(void);
		const std::string & getStatusMessage(void);
		void setStatusCode(int code);
		void setStatusMessage(const std::string & message);

		const std::string buildFirstLine() const;
};

#endif


// ResponseState						_state;
// std::string							_chunk;

// enum ResponseState {
// 	WRITE_STATUS_LINE,
// 	WRITE_HEADERS,
// 	WRITE_BODY,
// 	COMPLETE_RES
// };