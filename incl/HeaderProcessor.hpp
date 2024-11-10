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
#include "ConfigParser.hpp"

class HeaderProcessor;

enum HeaderValidationStatus
{
    SUCCESS,
    MISSING,
    INVALID_VALUE
};
typedef HeaderValidationStatus (*HeaderHandler)(Request & request, Response & response, const ServerConfig & config);

class HeaderProcessor
{
	public:
		HeaderProcessor(Request & request, Response & response, const ServerConfig & config);
		bool processHeaders(std::map<std::string, HeaderHandler> & handlers, std::set<std::string> & required);

	private:
		Request	& _request;
		Response & _response;
		const ServerConfig & _config;

		void initDefaultHeaders();
		void handleInvalidValue(const std::string &header, HeaderValidationStatus status);
		void handleMissingHeaders(const std::set<std::string> &missing_headers);
};

// Handlers
HeaderValidationStatus	processHost(Request & request, Response & response, const ServerConfig & config);
HeaderValidationStatus	processConnection(Request & request, Response & response, const ServerConfig & config);
HeaderValidationStatus	processExpect(Request & request, Response & response, const ServerConfig & config);
HeaderValidationStatus	processContentLength(Request & request, Response & response, const ServerConfig & config);
HeaderValidationStatus	processTransferEncoding(Request & request, Response & response, const ServerConfig & config);
HeaderValidationStatus	processContentType(Request & request, Response & response, const ServerConfig & config);
HeaderValidationStatus	processAccept(Request & request, Response & response, const ServerConfig & config);
HeaderValidationStatus	processUserAgent(Request & request, Response & response, const ServerConfig & config);
HeaderValidationStatus	processAuthorization(Request & request, Response & response, const ServerConfig & config);
HeaderValidationStatus	processReferer(Request & request, Response & response, const ServerConfig & config);
HeaderValidationStatus	processCookie(Request & request, Response & response, const ServerConfig & config);
HeaderValidationStatus	processIfModifiedSince(Request & request, Response & response, const ServerConfig & config);
HeaderValidationStatus	processIfNoneMatch(Request & request, Response & response, const ServerConfig & config);

// Handler Setup
void					setup_pre_body_handlers(std::map<std::string, HeaderHandler> & handlers, std::set<std::string> & required);
void					setup_post_body_handlers(std::map<std::string, HeaderHandler> & handlers, std::set<std::string> & required);

// Util
std::string				getCurrentTime();
std::string				getContentType(const std::string& path);

#endif
