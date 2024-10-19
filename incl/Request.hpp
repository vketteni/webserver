#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "AMessage.hpp"
#include "Debug.hpp"

class Request : public AMessage
{
	private:
		std::string							_method;
		std::string							_uri;

	// Inherited members (from AMessage):
		// std::string _first_line;
		// std::string _protocol_version;
		// std::string _body;
		// std::map<std::string, std::string> _headers;

	public:
		Request();

		const std::string &getMethod() const;
		const std::string &getUri() const;
		void setMethod(const std::string & method);
		void setUri(const std::string & uri);

		const std::string buildFirstLine() const;
		const std::string buildQueryString() const;
};

#endif
