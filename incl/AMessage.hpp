#ifndef AMESSAGE_HPP
#define AMESSAGE_HPP

#include <map>
#include <string>
#include <stdexcept>

class AMessage
{
	protected:
		std::string							_protocol_version;
		std::string							_first_line;
		std::string							_body;
		std::map<std::string, std::string>	_headers;

	public:
		AMessage();
		virtual ~AMessage();

		const std::string &getFirstLine() const;
		const std::string &getBody() const;
		const std::string &getVersion() const;
		const std::string &getHeaderOrDefault(const std::string &key, const std::string &default_value);
		const std::map<std::string, std::string> &getHeaders() const;
		void setFirstLine(const std::string &line);
		void setVersion(const std::string &version);
		void setBody(const std::string &body);
		void setHeader(const std::string &key, const std::string &value);
		void setHeaders(const std::map<std::string, std::string> &headers);

		virtual const std::string buildFirstLine() const = 0;
};

#endif