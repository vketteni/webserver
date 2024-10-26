#ifndef HEADERPROCESSOR_HPP
#define HEADERPROCESSOR_HPP

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sstream>

#include "Debug.hpp"
#include "Request.hpp"
#include "Response.hpp"

class HeaderProcessor;

enum HeaderValidationStatus
{
    SUCCESS,
    MISSING,
    INVALID_VALUE
};
typedef HeaderValidationStatus (*HeaderHandler)(Request & request, Response & response);

class HeaderProcessor
{
	public:
		HeaderProcessor(Request & request, Response & response);
		bool processHeaders(std::map<std::string, HeaderHandler> & handlers, std::set<std::string> & required);

	private:
		Request	& _request;
		Response & _response;

		void initDefaultHeaders();
		void handleInvalidValue(const std::string &header, HeaderValidationStatus status);
		void handleMissingHeaders(const std::set<std::string> &missing_headers);
};

// Handlers 
HeaderValidationStatus processHost(Request & request, Response & response);
HeaderValidationStatus processConnection(Request & request, Response & response);
HeaderValidationStatus processExpect(Request & request, Response & response);
HeaderValidationStatus processContentLength(Request & request, Response & response);
HeaderValidationStatus processTransferEncoding(Request & request, Response & response);
HeaderValidationStatus processContentType(Request & request, Response & response);
HeaderValidationStatus processAccept(Request & request, Response & response);
HeaderValidationStatus processUserAgent(Request & request, Response & response);
HeaderValidationStatus processAuthorization(Request & request, Response & response);
HeaderValidationStatus processReferer(Request & request, Response & response);
HeaderValidationStatus processCookie(Request & request, Response & response);
HeaderValidationStatus processIfModifiedSince(Request & request, Response & response);
HeaderValidationStatus processIfNoneMatch(Request & request, Response & response);

// Handler Setup
void setup_pre_body_handlers(std::map<std::string, HeaderHandler> & handlers, std::set<std::string> & required);
void setup_post_body_handlers(std::map<std::string, HeaderHandler> & handlers, std::set<std::string> & required);

// Util
std::string getCurrentTime();
std::string getContentType(const std::string& path);

#endif