#include "../incl/Request.hpp"

Request::Request()
	: _method("GET"), _uri("/")
{}

const std::string &Request::getMethod() const
{
	return (_method);
}

const std::string &Request::getUri() const
{
	return (_uri);
}

void Request::setMethod(const std::string &method)
{
	_method = method;
	setFirstLine(buildFirstLine());
}

void Request::setUri(const std::string &uri)
{
	_uri = uri;
	setFirstLine(buildFirstLine());
}

const std::string Request::buildFirstLine() const
{
	return (_method + " " + _uri + " " + _protocol_version);
}

const std::string Request::buildQueryString() const
{
	size_t pos = _uri.find('?');
	if (pos != std::string::npos)
	{
		return _uri.substr(pos);
	}
	return ("");
}


std::string Request::decodePercentEncoding(const std::string& uri)
{
    std::string result;
    for (std::string::size_type i = 0; i < uri.length(); ++i)
	{
        if (uri[i] == '%' && i + 2 < uri.length() && 
            std::isxdigit(uri[i + 1]) && std::isxdigit(uri[i + 2]))
			{
            // Convert hex code to a character
            int hex = std::strtol(uri.substr(i + 1, 2).c_str(), NULL, 16);
            result += static_cast<char>(hex);
            i += 2;
        } else
		{
            result += uri[i];
        }
    }
    return result;
}

std::string Request::removeDotSegments(const std::string& path)
{
    std::string result;
    std::string::size_type i = 0;

    while (i < path.length())
	{
		// If path begins with "./" or "/./", remove the leading "."
        if (path.substr(i, 2) == "./") {
			i += 2;
		}
		else if (path.substr(i, 3) == "/./") {
			i += 2;
		}
		// If path begins with "../" or "/../", move up one directory level	
        else if (path.substr(i, 3) == "../") {
			i += 3;
		}
		else if (path.substr(i, 4) == "/../")
		{
			// Remove last segment in result
            if (!result.empty())
			{
                std::string::size_type pos = result.find_last_of('/');
                if (pos != std::string::npos)
                    result.erase(pos);
                else
                    result.clear();
            }
            i += 3;
        }
		// If path is "." or "/.", remove the "."	
        else if (path.substr(i, 1) == "." && (i + 1 == path.length() || path[i + 1] == '/')) {
			++i;
		}
		// If path is ".." or "/..", move up one directory level
        else if (path.substr(i, 2) == ".." && (i + 2 == path.length() || path[i + 2] == '/'))
		{
            if (!result.empty())
			{
                std::string::size_type pos = result.find_last_of('/');
                if (pos != std::string::npos)
                    result.erase(pos);
                else
                    result.clear();
            }
            i += 2;
        }
		else
		{
			// Copy segment to result
            std::string::size_type next_slash = path.find('/', i);
            result += "/";
            if (next_slash != std::string::npos)
			{
                result += path.substr(i, next_slash - i);
                i = next_slash;
            }
			else
			{
                result += path.substr(i);
                break;
            }
        }
    }

    if (result.length() > 1 && result[result.length() - 1] == '/')
		result.erase(result.length() - 1);

	return result;
}

std::string Request::normalizeUri(const std::string& uri)
{
    std::string decodedUri = decodePercentEncoding(uri);

    std::string normalizedPath = removeDotSegments(decodedUri);

    return normalizedPath;
}
