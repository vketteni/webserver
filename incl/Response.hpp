#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <sstream>
#include "AMessage.hpp"

class Response : public AMessage
{
	private:
		int						_status_code;
		std::string				_status_message;

	public:
		Response();
		~Response();

		int 					getStatusCode(void);
		const std::string &		getStatusMessage(void);
		void					setStatusCode(int code);
		void					setStatusMessage(const std::string & message);

		const std::string		buildFirstLine() const;
};

#endif
