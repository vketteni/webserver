#ifndef UTILS_HPP
#define UTILS_HPP

#include <sstream>
#include <vector>
#include <string>

#define WSPACE " \t\n\r\f\v"

class Utils
{
	public:
		static std::string trim(const std::string& str, const std::string &trim_set);
		static std::vector<std::string> split(const std::string& str, char delimiter);
};

#endif