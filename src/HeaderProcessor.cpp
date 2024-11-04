#include "../incl/HeaderProcessor.hpp"

HeaderProcessor::HeaderProcessor(Request & request, Response & response, const ServerConfig & config)
	: _request(request), _response(response), _config(config)
{}

bool HeaderProcessor::processHeaders(std::map<std::string, HeaderHandler> & handlers, std::set<std::string> & required_headers)
{
	initDefaultHeaders();
	std::map<std::string, std::string> request_headers = _request.getHeaders();

	// Iterate through each header
	for (std::map<std::string, std::string>::const_iterator it = request_headers.begin(); it != request_headers.end(); ++it)
	{
		std::map<std::string, HeaderHandler>::iterator handler_it = handlers.find(it->first);
		if (handler_it != handlers.end())
		{
			// Call the handler and check for validation errors
			HeaderValidationStatus status = (*(handler_it->second))(_request, _response, _config);
			if (status != SUCCESS)
			{
				handleInvalidValue(it->first, status);
				_response.setStatusCode(400);
				_response.setStatusMessage("Bad request");
				return false;
			}
		}
		required_headers.erase(it->first);
	}

	// Check if any required headers are missing
	if (!required_headers.empty())
	{
		handleMissingHeaders(required_headers);
		_response.setStatusCode(400);
		_response.setStatusMessage("Bad request");
		return false;
	}
	return true;
}

void HeaderProcessor::initDefaultHeaders()
{
	_response.setHeader("Date", getCurrentTime());
	_response.setHeader("Server", "webserver/1.0");// TODO: Use config file server name;
	if (_request.getHeaderOrDefault("Connection", "").empty())
	{
		_response.setHeader("Connection", "close");
	}
}

void HeaderProcessor::handleInvalidValue(const std::string &header, HeaderValidationStatus status)
{
	std::string status_str = "if you read this the error has an error";

	if (status == MISSING)
		status_str = "missing";
	else if (status == INVALID_VALUE)
		status_str = "invalid value";
	
	std::cerr << "Error: Bad value for " << header << " (" << status_str << ")" << std::endl;
}

void HeaderProcessor::handleMissingHeaders(const std::set<std::string> &missing_headers)
{
	std::cerr << "Error: Missing critical headers: ";
	for (std::set<std::string>::const_iterator it = missing_headers.begin(); it != missing_headers.end(); ++it)
	{
		std::cerr << *it << " ";
	}
	std::cerr << std::endl;

	// Additional error handling (e.g., setting response status to 400 Bad Request)
}

HeaderValidationStatus processContentLength(Request & request, Response & response, const ServerConfig & config)
{
	(void)config;
	(void)response;
	std::string request_content_length = request.getHeaderOrDefault("Content-Length", "");

	std::stringstream ss;
	ss << request.getHeaderOrDefault("Content-Length", "0");
	size_t content_length = std::atoi(ss.str().c_str());

	if (content_length != request.getBody().size())
		return INVALID_VALUE;
	if (content_length > config.client_max_body_size)
		return INVALID_VALUE;
	return SUCCESS;
}

HeaderValidationStatus processHost(Request & request, Response & response, const ServerConfig & config)
{
	(void)config;	
	std::string request_host = request.getHeaderOrDefault("Host", "");
	// std::cout << "Processing Host: " << request_host << std::endl;
	if (request_host.empty()) return INVALID_VALUE;
	response.setHeader("Host", request_host);
	return SUCCESS;
}

HeaderValidationStatus processConnection(Request & request, Response & response, const ServerConfig & config)
{
	(void)config;
	// std::cout << "Processing Connection: " << request.getHeaderOrDefault("Connection", "") << std::endl;
	std::string request_header = request.getHeaderOrDefault("Connection", "");
	response.setHeader("Connection", request_header);

	if (!response.getHeaderOrDefault("Connection", "").empty() && response.getHeaderOrDefault("Connection", "") == "keep-alive")
		response.setHeader("keep-alive", "timeout=5, max=1000");

	if (request_header != "keep-alive" && request_header != "close")
		return INVALID_VALUE;
	return SUCCESS;
}

HeaderValidationStatus processExpect(Request & request, Response & response, const ServerConfig & config)
{
	(void)request;
	(void)response;
	(void)config;

	// std::cout << "Processing Expect: " << request.getHeaderOrDefault("Expect", "") << std::endl;
	// Handle Expect logic
	return SUCCESS;
}


HeaderValidationStatus processTransferEncoding(Request & request, Response & response, const ServerConfig & config)
{
	(void)request;
	(void)response;
	(void)config;

	// std::cout << "Processing Transfer-Encoding: " << request.getHeaderOrDefault("Transfer-Encoding", "") << std::endl;
	// Handle Transfer-Encoding logic
	return SUCCESS;
}

HeaderValidationStatus processContentType(Request & request, Response & response, const ServerConfig & config)
{
	(void)request;
	(void)response;
	(void)config;

	// std::cout << "Processing Content-Type: " << response.getHeaderOrDefault("Content-Type", "") << std::endl;
	// Handle Content-Type logic
	if (response.getHeaderOrDefault("Content-Type", "").empty())
	{
		response.setHeader("Content-Type", "application/octet-stream");
	}
	return SUCCESS;
}

HeaderValidationStatus processAccept(Request & request, Response & response, const ServerConfig & config)
{
	(void)config;
	// std::cout << "Processing Accept: " << request.getHeaderOrDefault("Accept", "") << std::endl;
	std::string request_header = request.getHeaderOrDefault("Accept", "");

	if (request_header.find("application/json") != std::string::npos)
	{
		response.setHeader("Content-Type", "application/json");
	} else if (request_header.find("text/html") != std::string::npos)
	{
		response.setHeader("Content-Type", "text/html");
	} else
	{
		response.setHeader("Content-Type", "application/octet-stream");
	}
	return SUCCESS;
}

HeaderValidationStatus processUserAgent(Request & request, Response & response, const ServerConfig & config)
{
	(void)request;
	(void)response;
	(void)config;

	// std::cout << "Processing User-Agent: " << request.getHeaderOrDefault("User-Agent", "") << std::endl;
	// Handle User-Agent logic
	return SUCCESS;
}

HeaderValidationStatus processAuthorization(Request & request, Response & response, const ServerConfig & config)
{
	(void)request;
	(void)response;
	(void)config;

	// std::cout << "Processing Authorization: " << request.getHeaderOrDefault("Authorization", "") << std::endl;
	// Handle Authorization logic
	return SUCCESS;
}

HeaderValidationStatus processReferer(Request & request, Response & response, const ServerConfig & config)
{
	(void)request;
	(void)response;
	(void)config;

	// std::cout << "Processing Referer: " << request.getHeaderOrDefault("Referer", "") << std::endl;
	// Handle Referer logic
	return SUCCESS;
}

HeaderValidationStatus processCookie(Request & request, Response & response, const ServerConfig & config)
{
	(void)request;
	(void)response;
	(void)config;

	// std::cout << "Processing Cookie: " << request.getHeaderOrDefault("Cookie", "") << std::endl;
	// Handle Cookie logic
	return SUCCESS;
}

HeaderValidationStatus processIfModifiedSince(Request & request, Response & response, const ServerConfig & config)
{
	(void)request;
	(void)response;
	(void)config;

	// std::cout << "Processing If-Modified-Since: " << request.getHeaderOrDefault("If-Modified-Since", "") << std::endl;
	// Handle If-Modified-Since logic
	return SUCCESS;
}

HeaderValidationStatus processLastModified(Request & request, Response & response, const ServerConfig & config)
{
	(void)request;
	(void)response;
	(void)config;

	// std::cout << "Processing Last-Modified: " << request.getHeaderOrDefault("Last-Modified", "") << std::endl;
	// Handle Last-Modified logic

	if (!request.getHeaderOrDefault("Last-Modified", "").empty())
	{
		std::string ifModifiedSince = request.getHeaderOrDefault("Last-Modified", "");
		std::string serverLastModified = getCurrentTime();

		if (ifModifiedSince == serverLastModified)
		{
			// Set 304 Not Modified if the resource has not changed
			request.setHeader("Status", "304 Not Modified");
			// Remove body-related headers if we aren't sending the body
			request.setHeader("Content-Length", "");
			request.setHeader("Content-Type", "");
		}
	}
	// TODO: caching header handling (e.g., ETag and If-None-Match) can be added here
	return SUCCESS;
}


HeaderValidationStatus processIfNoneMatch(Request & request, Response & response, const ServerConfig & config)
{
	(void)request;
	(void)response;
	(void)config;
	// std::cout << "Processing If-None-Match: " << request.getHeaderOrDefault("If-None-Match", "") << std::endl;
	// Handle If-None-Match logic
	return SUCCESS;
}

/*
	Description: Initializes handlers to be applied before the body is parsed
	Arguments:	std::map<std::string, HeaderHandler> & handlers,
				std::set<std::string> & required_headers
	Return: void
*/
void setup_pre_body_handlers(std::map<std::string, HeaderHandler> & handlers, std::set<std::string> & required_headers)
{
	(void)required_headers;
	// required_headers.insert("Host");
	// required_headers.insert("Content-Length");

	handlers["Host"] = &processHost;
	handlers["Connection"] = &processConnection;
	handlers["Expect"] = &processExpect;
	handlers["Content-Length"] = &processContentLength;
	handlers["Transfer-Encoding"] = &processTransferEncoding;
	handlers["Accept"] = &processAccept;
	handlers["Content-Type"] = &processContentType;
	handlers["User-Agent"] = &processUserAgent;
	handlers["Authorization"] = &processAuthorization;
	handlers["Referer"] = &processReferer;
	handlers["Cookie"] = &processCookie;	
}

/*
	Description: Initializes handlers to be applied after the body is parsed
	Arguments:	std::map<std::string, HeaderHandler> & handlers,
				std::set<std::string> & required_headers
	Return: void
*/
void setup_post_body_handlers(std::map<std::string, HeaderHandler> & handlers, std::set<std::string> & required)
{
	(void)required;

	handlers["Authorization"] = &processAuthorization;
	handlers["If-Modified-Since"] = &processIfModifiedSince;
}

/* Time utility */
std::string getCurrentTime()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *gmtime(&now);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %X GMT", &tstruct);
	return buf;
}

bool endsWith(const std::string& str, const std::string& suffix) {
    // Ensure suffix is not longer than the string
    if (suffix.size() > str.size()) return false;
    return str.substr(str.size() - suffix.size()) == suffix;
}

std::string getContentType(const std::string& path) {
    // Simple MIME type mapping based on file extension
    if (endsWith(path, ".html")) return "text/html";
    if (endsWith(path, ".css")) return "text/css";
    if (endsWith(path, ".js")) return "application/javascript";
    if (endsWith(path, ".png")) return "image/png";
    if (endsWith(path, ".jpg") || endsWith(path, ".jpeg")) return "image/jpeg";
    // Add more as needed
    return "application/octet-stream";
}

