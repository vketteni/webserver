#include "../incl/Utils.hpp"

std::string Utils::trim(const std::string & str, const std::string & trim_set)
{
	const size_t strBegin = str.find_first_not_of(trim_set);
	if (strBegin == std::string::npos)
		return "";
	const size_t strEnd = str.find_last_not_of(trim_set);
	return str.substr(strBegin, strEnd - strBegin + 1);
}

std::vector<std::string> Utils::split(const std::string& str, char delimiter)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;
	while (getline(ss, token, delimiter))
	{
		token = trim(token, WSPACE);
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens;
}


std::string Utils::replaceChars(const std::string& input, const std::string& chars_to_replace, const std::string& replacement)
{
    std::string result;

    for (size_t i = 0; i < input.length(); ++i)
	{
        char current_char = input[i];

        if (chars_to_replace.find(current_char) != std::string::npos)
            result.append(replacement);
		else
            result.push_back(current_char);
    }

    return result;
}

std::string Utils::build_relative_path_from_location_match(const std::string & request_uri, const std::string & match)
{
	std::string relative_path;
	if (match[0] == '*')
	{
		size_t match_start = request_uri.find(match.substr(1));
		relative_path = (match_start != std::string::npos) ? request_uri.substr(match_start + match.size() - 1) : "";
	}
	else if (match[match.size() - 1] == '*')
	{
		size_t match_start = request_uri.find(match.substr(0, match.size() - 1));
		relative_path = (match_start != 0 || match_start == std::string::npos) ? "" : request_uri.substr(match.find('*'));
	}
	else
	{
		size_t match_start = request_uri.find(match);
		relative_path = (match_start != 0 || match_start == std::string::npos) ? "" : request_uri.substr(match_start + match.size());
	}
	return relative_path;
}
