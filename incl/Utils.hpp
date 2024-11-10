#ifndef UTILS_HPP
#define UTILS_HPP

#include <sstream>
#include <vector>
#include <string>

#include "Debug.hpp"

#define WSPACE " \t\n\r\f\v"

class Utils
{
	public:
		static std::string				trim(const std::string& str, const std::string &trim_set);
		static std::vector<std::string>	split(const std::string& str, char delimiter);
		static std::string				replaceChars(const std::string& input, const std::string& charsToReplace, const std::string& replacement);
		static std::string				build_relative_path_from_location_match(const std::string & request_uri, const std::string & match);
};

#endif

