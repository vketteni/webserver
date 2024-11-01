#include "../incl/RequestParser.hpp"
RequestParser::RequestParser() : _state(READ_REQUEST_LINE) {}

void printMultipartBody(const std::string& body, const std::string& boundary);
std::string getBoundaryFromHeader(const std::string& header);

bool RequestParser::parse() {
	try {
		while (true) {
			// ?? TODO ?? add validation states
			switch (_state) {
				case READ_REQUEST_LINE:
					if (!extractRequestLine()) return true;
					_state = READ_HEADERS;
					break;

				case READ_HEADERS:
					if (!extractHeaders()) return true;
					_state = PROCESS_HEADERS;
					break;

				case PROCESS_HEADERS:
					if (!processHeadersBeforeBody()) return false;
					if (_request.getHeaderOrDefault("Content-Length", "0") != "0") {
						_state = READ_BODY;
					} else {
						_state = COMPLETE;
					}
					break;

				case READ_BODY:
					if (!extractBody()) return true;
					printMultipartBody(_request.getBody(), getBoundaryFromHeader(_request.getHeaderOrDefault("Content-Length", "")));
					_state = COMPLETE;
					break;

				case COMPLETE:
					return true;

				case ERROR:
					reset();
					throw std::invalid_argument("Parsing Error: Resetting request.");
			}
		}
	} catch (const std::exception& e) {
		std::cerr << "Parsing Error: " << e.what() << std::endl;
		_state = ERROR;
		return false;
	}
	return true;
}

bool RequestParser::extractRequestLine(void)
{
	const char delimiter[] = "\r\n";
	std::vector<char>::iterator it = std::search(_buffer.begin(), _buffer.end(), delimiter, delimiter + std::strlen(delimiter));
    if (it == _buffer.end())
        return false;

    size_t pos = std::distance(_buffer.begin(), it);

    std::string requestLine(_buffer.begin(), _buffer.begin() + pos);

    _buffer.erase(_buffer.begin(), _buffer.begin() + pos + 2);

    std::string method, uri, http_version;
    std::istringstream iss(requestLine);
    if (!(iss >> method >> uri >> http_version)) {
        throw std::invalid_argument("Invalid request line");
    }
	if (uri[0] == '/')
		_request.setUri(uri.substr(1));
	else
		_request.setUri(uri);
    _request.setMethod(method);
    _request.setVersion(http_version);
    pretty_debug("Method: " + method);
    pretty_debug("URI: " + uri);
    pretty_debug("HTTP Version: " + http_version);
    return true;
}

bool RequestParser::extractHeaders()
{
    std::map<std::string, std::string> headers;

    const char delimiter[] = "\r\n";
    std::vector<char>::iterator buffer_it = _buffer.begin();

    while (true)
	{
        std::vector<char>::iterator it = std::search(buffer_it, _buffer.end(), delimiter, delimiter + std::strlen(delimiter));
        if (it == _buffer.end())
            return false;
        size_t pos = std::distance(_buffer.begin(), it);
        if (pos == 0)
            break;

        std::string header_line(_buffer.begin(), _buffer.begin() + pos);

        _buffer.erase(_buffer.begin(), it + 2);

        size_t colon_pos = header_line.find(":");
        if (colon_pos == std::string::npos)
            throw std::invalid_argument("Invalid header line");

        std::string name = header_line.substr(0, colon_pos);

        size_t value_start = header_line.find_first_not_of(WSPACE, colon_pos + 1);
        std::string value = (value_start != std::string::npos) ? header_line.substr(value_start) : "";

        headers[name] = value;

        pretty_debug(name + "=" + value);
    }

    _buffer.erase(_buffer.begin(), _buffer.begin() + 2);
    _request.setHeaders(headers);
    return true;
}


bool RequestParser::extractBody()
{
	std::stringstream ss;
	ss << _request.getHeaderOrDefault("Content-Length", "0");
	size_t content_length = std::atoi(ss.str().c_str());

	std::string new_chunk(_buffer.begin(), _buffer.end());
	_request.setBody(_request.getBody() + new_chunk);
	_buffer.erase(_buffer.begin(), _buffer.end());

	if (_request.getBody().size() > content_length)
        throw std::invalid_argument("Request body is greater than specified by Content-Length");
	else if (_request.getBody().size() < content_length)
		return false;
	else
		return true;
}

bool RequestParser::processHeadersBeforeBody(void)
{
	std::map<std::string, HeaderHandler> handlers;
	std::set<std::string> required;

	HeaderProcessor hp(_request, _response);
	setup_pre_body_handlers(handlers, required);
	
	if (!hp.processHeaders(handlers, required))
		return false;
	return true;
}

bool RequestParser::readAndParse(int client_fd)
{
	_buffer.resize(BUFFER_SIZE);
    ssize_t bytes_read = recv(client_fd, _buffer.data(), BUFFER_SIZE, 0);
    if (bytes_read <= 0)
	{
        // Handle disconnection or error
        return false;
    }
	_buffer.resize(bytes_read);
    if (!parse())
	{
		this->reset();
        return false;
	}
	return true;
}

void RequestParser::reset(void)
{
    _state = READ_REQUEST_LINE;
    _buffer.clear();
    _request = Request();
}

const Request & RequestParser::getRequest(void) const
{
	return _request;
}

const Response & RequestParser::getResponse(void) const
{
	return _response;
}

RequestState RequestParser::getState(void) const
{
	return _state;
}

bool RequestParser::isComplete() const
{
	return _state == COMPLETE;
}

#include <iostream>
#include <string>
#include <cstring>

// Helper function to extract and print relevant sections of the multipart body
void printMultipartBody(const std::string& body, const std::string& boundary) {
    // Construct start and end boundary strings
    std::string start_delimiter = "--" + boundary;
    std::string end_delimiter = "--" + boundary + "--";

    // Find positions of the start and end delimiters
    size_t start_pos = body.find(start_delimiter);
    if (start_pos == std::string::npos) {
        std::cerr << "Start delimiter not found" << std::endl;
        return;
    }
    
    size_t end_pos = body.find(end_delimiter, start_pos);
    if (end_pos == std::string::npos) {
        std::cerr << "End delimiter not found" << std::endl;
        return;
    }

    // Find the start of the headers right after the start delimiter
    size_t headers_start = start_pos + start_delimiter.size() + 2;  // Skip "\r\n"
    size_t headers_end = body.find("\r\n\r\n", headers_start);
    if (headers_end == std::string::npos) {
        std::cerr << "Headers section not found" << std::endl;
        return;
    }

    // Print the starting delimiter
    std::cout << "Start Delimiter: " << start_delimiter << std::endl;

    // Print headers section between start delimiter and binary data
    std::cout << "Headers:\n";
    std::cout << body.substr(headers_start, headers_end - headers_start) << std::endl;

    // Skip binary data section by jumping to the end delimiter
    std::cout << "Binary data section skipped.\n";

    // Print the ending delimiter
    std::cout << "End Delimiter: " << end_delimiter << std::endl;
}

#include <string>
#include <iostream>

std::string getBoundaryFromHeader(const std::string& header) {
    // Define the key we're searching for in the header
    const std::string boundary_key = "boundary=";

    // Find the position of "boundary=" in the header string
    size_t boundary_pos = header.find(boundary_key);
    if (boundary_pos == std::string::npos) {
        std::cerr << "Boundary not found in header" << std::endl;
        return "";
    }

    // Extract the boundary value after "boundary="
    size_t start = boundary_pos + boundary_key.size();
    return header.substr(start);
}