#ifndef CONFIGPARSER_HPP_
#define CONFIGPARSER_HPP_
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <fstream>
struct RouteConfig
{
    std::string                 root;
    std::string                 index;
    std::vector<std::string>    methods;
    bool                        autoindex;
    std::string                 upload_dir;
    std::string                 cgi_extension;
    std::string                 redirect_path;
    int                         redirect_status;
    RouteConfig() : autoindex(false) {}
};
struct ServerConfig {
    std::string                         server_name;
    std::string                         host;
    int                                 port;
    std::map<int, std::string>          error_pages;
    size_t                              client_max_body_size;
    std::map<std::string, RouteConfig>  routes;
    std::map<std::string, std::string>  redirects;
    ServerConfig() : port(0), client_max_body_size(0) {}
};
class ConfigParser {
 public:
    ConfigParser();
    ~ConfigParser();
    bool    parseConfig(const std::string& filepath);
    const   std::vector<ServerConfig>& getServer() const;
 private:
    std::vector<ServerConfig> servers;
    bool    parseServer(std::ifstream& configFile, ServerConfig& server); //to parse one server
    bool    parseLocation(std::ifstream& configFile, RouteConfig& route);
};
#endif // CONFIGPARSER_HPP