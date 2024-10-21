#ifndef CONFIGPARSER_HPP_
#define CONFIGPARSER_HPP_
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <sstream>
#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <fstream>
#include <cstdio>  // For std::remove and std::rename
#include <string>

#include "Debug.hpp"
#include "Utils.hpp"



struct RouteConfig
{
    std::string root;
    std::string index;
    std::string upload_dir;
    std::string cgi_extension;
    std::vector<std::string> methods;
    int redirect_status;
    std::string redirect_path;

    RouteConfig() : redirect_status(0)
	{}
};

struct HostConfig
{
    int port;
	std::string root;
    std::string host;
    std::string server_name;
    int client_max_body_size;
    std::map<int, std::string> error_pages;
    std::map<std::string, RouteConfig> routes;
};


class ConfigParser
{
	public:
		ConfigParser();
		~ConfigParser();
		bool    parseConfig(const std::string& file_path);
		const   std::map<int, HostConfig> & getHostConfigs() const;
		void printRedirectsAndRoutes(const std::map<int, HostConfig> &host_configs);
	private:
		std::map<int, HostConfig> host_configs;

		bool    parseConfig(std::ifstream& config_file, HostConfig& host_config);
		std::pair<std::string, std::string> parseKeyValuePair(const std::string& line);
		void parseErrorPage(const std::string& line, HostConfig& host_config);
		void parseLocation(std::ifstream& config_file, const std::string& locationLine, HostConfig& host_config);
		bool parseLocationBlock(std::ifstream& config_file, RouteConfig& route);
		void parseRedirect(const std::string& line, RouteConfig& route);
		int parsePort(const std::string& value);
		int parseSize(const std::string& value);
		void normalizeSpaces(const char* file_path);
};

#endif
