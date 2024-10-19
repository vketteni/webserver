#include "../incl/RequestParser.hpp"
RequestParser::RequestParser() : _state(READ_REQUEST_LINE) {}

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

#include <cstdio>
bool RequestParser::extractRequestLine(void)
{
	size_t pos = _buffer.find("\r\n");
	if (pos == std::string::npos) {
		return false;
	}

	std::string requestLine = _buffer.substr(0, pos);
	_buffer.erase(0, pos + 2);

	std::string method, uri, http_version;
	std::istringstream iss(requestLine);
	if (!(iss >> method >> uri >> http_version)) {
		throw std::invalid_argument("Invalid request line");
	}
	_request.setMethod(method);
	_request.setUri(uri);
	_request.setVersion(http_version);
	printf ("Method: %s\n", method.c_str());
	printf ("URI: %s\n", uri.c_str());
	printf ("HTTP Version: %s\n", http_version.c_str());
	return true;
}

bool RequestParser::extractHeaders()
{
	std::map<std::string, std::string> headers = _request.getHeaders();
	size_t pos;
	while ((pos = _buffer.find("\r\n")) != 0) {
		if (pos == std::string::npos)
		{
			return false;
		}
		std::string header_line = _buffer.substr(0, pos);
		_buffer.erase(0, pos + 2);

		size_t colon_pos = header_line.find(":");
		if (colon_pos == std::string::npos)
		{
			throw std::invalid_argument("Invalid header line");
		}
		std::string name = header_line.substr(0, colon_pos);
		std::string value = header_line.substr(colon_pos + 1);
		Utils::trim(name, WSPACE);
		Utils::trim(value, WSPACE);
		headers[name] = value;
	}
	_buffer.erase(0, 2);
	return true;
}

bool RequestParser::extractBody()
{
	std::stringstream ss;
	ss << _request.getHeaderOrDefault("Content-Length", "0");
	size_t content_length = ss.dec;

	if (_buffer.size() >= content_length) {
		_request.setBody(_buffer.substr(0, content_length));
		_buffer.erase(0, content_length);
		return true;
	}
	return false;
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

void RequestParser::appendData(const std::string& data)
{
        _buffer += data;
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

