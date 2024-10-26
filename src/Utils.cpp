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
