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

#include <sstream>
#include <vector>
#include <string>

std::string Request::removeDotSegments(const std::string& path)
{
    // Edge case: empty path
    if (path.empty()) return "";

    std::vector<std::string> segments;
    std::stringstream ss(path);
    std::string segment;

    // Split the path into segments using '/' as a delimiter
    while (std::getline(ss, segment, '/'))
    {
        if (segment == "" || segment == ".") {
            // Skip empty segments or "." segments
            continue;
        } else if (segment == "..") {
            // Go up one level, if possible
            if (!segments.empty() && segments.back() != "..") {
                segments.pop_back();
            } else if (path[0] == '/') {
                // If path is absolute, ignore ".." at the root
                continue;
            } else {
                // If relative path and no previous segment, keep ".."
                segments.push_back("..");
            }
        } else {
            // Normal segment, add to stack
            segments.push_back(segment);
        }
    }

    // Join segments back into a single normalized path
    std::string result = (path[0] == '/' ? "/" : ""); // Keep leading '/' for absolute paths
    for (size_t i = 0; i < segments.size(); ++i) {
        result += segments[i];
        if (i < segments.size() - 1 || (path[path.size() - 1] == '/' && !segments.empty())) {
            // Append '/' between segments and at the end if the original path had a trailing slash
            result += "/";
        }
    }

    return result;
}


std::string Request::normalizeUri(const std::string& uri)
{
    std::string decodedUri = decodePercentEncoding(uri);

    std::string normalizedPath = removeDotSegments(decodedUri);

    return normalizedPath;
}
