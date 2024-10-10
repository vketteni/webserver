#ifndef HEADERPROCESSOR_HPP
#define HEADERPROCESSOR_HPP

#include <string>
#include "Request.hpp"

class HeaderProcessor
{
	public:
		void processHeaders(Request& request)
		{

			// _content_type = content_type;
			// _content_length = body.size();
			// response.setConnection("close");

			const std::map<std::string, std::string> & headers = request.getHeaders();
			for (std::map<std::string, std::string>::const_iterator headerIterator = headers.begin(); headerIterator != headers.end(); ++headerIterator)
			{
				// Dispatch header to appropriate handlers
				if (headerIterator->first == "Content-Type")
				{
					processContentType(headerIterator->second);
				}
				else if (headerIterator->first == "Authorization")
				{
					processAuthorization(headerIterator->second);
				}
				// Add more header-specific logic here
			}
		}

	private:
		void processContentType(const std::string& value)
		{
			// Handle Content-Type header
			(void)value;
		}

		void processAuthorization(const std::string& value)
		{
			// Handle Authorization header
			(void)value;
		}
};

#endif