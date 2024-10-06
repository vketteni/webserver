#include "../incl/HTTPRequest.hpp"

HTTPRequest::HTTPRequest()
{
	reset();
}

HTTPRequest &HTTPRequest::operator=(const HTTPRequest &other)
{
	if (this != &other)
	{
		this->_state = other._state;
		this->_buffer = other._buffer;
		this->_method = other._method;
		this->_http_version = other._http_version;
		this->_uri = other._uri;
		this->_headers = other._headers;
		this->_body = other._body;
	}
	return *this;
}

bool HTTPRequest::parseHeaders(const std::string &header_block)
{
	size_t	pos;
	std::string line;
	std::istringstream request_stream(header_block);

	while (std::getline(request_stream, line))
	{
		pos = line.find(":");
		//	** possible place for key validation function **
		if (pos != std::string::npos)
		{
			std::string key = line.substr(0, pos);
			std::string val = line.substr(pos + 1, line.size() - 2);
			//	** possible place for white space removal function for value **
			_headers[key] = val;
		}
		else
		{
			std::cerr << "Failed to parse header line." << "\n Input:\n" << "[" << line << "]" << std::endl;
			return false;
		}
	}
    return true;
}

bool HTTPRequest::parse(const std::string & data)
{
	_buffer += data;

	switch (_state) {
		case READ_REQUEST_LINE : {
			std::string::size_type pos = _buffer.find("\r\n");
			if (pos != std::string::npos) {
				std::string request_line = _buffer.substr(0, pos);
				//	** possible place for validation function **
				if (!parseRequestLine(request_line))
				{
					std::cerr << "Failed to parse request line." << "\n Input:\n" << "[" << request_line << "]" << std::endl;
					return false;
				}
				_buffer.erase(0, pos + 2);
				_state = READ_HEADERS;
			}
			else
			{
				std::cerr << "Failed to identify request line." << "\n Input:\n" << "[" << _buffer << "]" << std::endl;
			}
			break;
		}
		case READ_HEADERS : {
			std::string::size_type pos = _buffer.find("\r\n\r\n");
			if (pos != std::string::npos) {
				std::string header_block = _buffer.substr(0, pos);
				//	** possible place for validation function **
				if (!parseHeaders(header_block))
				{
					std::cerr << "Failed to parse headers." << "\n Input:\n" << "[" << header_block << "]" << std::endl;
					return false;
				}
				_buffer.erase(0, pos + 4);
				_state = READ_BODY;
			}
			else
			{
				std::cerr << "Failed to identify headers." << "\n Input:\n" << "[" << _buffer << "]" << std::endl;
			}
			break;
		}
		case READ_BODY : {
			_body = _buffer;
			_state = COMPLETE;
			break;
		}	
		case ERROR : {
			std::cerr << "Parsing error. Resetting./n";
			reset();
			return false;
		}	
		case COMPLETE : {
			debug("parsing done");
			return true;
		}
	}
	return true;
}

bool HTTPRequest::parseRequestLine(const std::string &line)
{
    std::istringstream iss(line);
	
    std::string method;
	std::string uri;
	std::string version;

    iss >> method;
	iss >> uri;
	iss >> version;

	_method = method;
	_uri = uri;
	_http_version = version;

	return true;
}

RequestState HTTPRequest::getState(void) const
{
	return _state;
}

const std::string HTTPRequest::getRequestLine() const
{
	return _method + " " + _uri + " " + _http_version;
}

const std::string & HTTPRequest::getBody() const
{
	return _body;
}

const std::string & HTTPRequest::getMethod() const
{
	return _method;
}

const std::string & HTTPRequest::getUri() const
{
	return _uri;
}

const std::string & HTTPRequest::getVersion() const
{
	return _http_version;
}

const std::map<std::string, std::string> & HTTPRequest::getHeaders() const
{
	return _headers;
}

bool HTTPRequest::isComplete() const
{
	return _state == COMPLETE;
}

void HTTPRequest::reset(void)
{
    _state = READ_REQUEST_LINE;
    _buffer.clear();
    _method.clear();
    _uri.clear();
    _http_version.clear();
    _headers.clear();
    _body.clear();
}