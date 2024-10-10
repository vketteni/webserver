#include "../incl/ConfigParser.hpp"

// Default constructor
ConfigParser::ConfigParser() {
    return ;
}
// Default destructor
ConfigParser::~ConfigParser()
{
    return ;
}
/*
    Method:         Server::parseConfig
    Description:    Parses the configuration file to extract ports
*/
bool ConfigParser::parseConfig(const std::string& filepath)
{
    std::ifstream configFile(filepath.c_str());
    if (!configFile.is_open())
    {
        std::cerr << "Failed to open configuration file: " << filepath << "\n";
        return false;
    }
    std::string line;
    while (std::getline(configFile, line))
    {
        // Remove whitespace
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        if(line.empty() || line[0] == '#') {
            continue ;
        }
        if (line == ("server{")) {
            ServerConfig server;
            if(!parseServer(configFile, server)) {
                return false;
            }
            servers.push_back(server);
        }
    }
    configFile.close();
    return true;
}
const std::vector<ServerConfig> &ConfigParser::getServer() const
{
    return servers;
}
bool    ConfigParser::parseServer(std::ifstream& configFile, ServerConfig& server) {
    std::string line;
    while (std::getline(configFile, line)) {
        // Remove whitespace
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        if(line.empty() || line[0] == '#') {
            continue ;
        }
        if (line == ("}")) {
            break;
        }
        if (line.find("listen") == 0) {
            if (line[line.size() - 1] == ';') {
                line.erase(line.size() - 1);
            }
            server.port = std::atoi(line.substr(6).c_str());
        }
        else if (line.find("host") == 0) {
            if (line[line.size() - 1] == ';') {
                line.erase(line.size() - 1);
            }
            server.host = line.substr(4);
        }
        else if (line.find("server_name") == 0) { //auf dafault localhost setzen
            if (line[line.size() - 1] == ';') {
                line.erase(line.size() - 1);
            }
            server.server_name = line.substr(11);
        }
        else if (line.find("client_max_body_size") == 0) {
            if (line[line.size() - 1] == ';') {
                line.erase(line.size() - 1);
            }
            server.client_max_body_size = std::atoi(line.substr(20).c_str());
        }
        else if (line.find("error_page") == 0) {
            int code = std::atoi(line.substr(11, 3).c_str());
            server.error_pages[code] = line.substr(15);
        }
        else if (line.find("location") == 0) {
            RouteConfig route;
            if (!parseLocation(configFile, route)) {
                return false;
            }
            server.routes[line.substr(8)] = route;
        }
    }
    return true;
}
bool ConfigParser::parseLocation(std::ifstream &configFile, RouteConfig &route)
{
    std::string line;
    while (std::getline(configFile, line)) {
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.empty() || line[0] == '#'){
            continue;
        }
        if (line == "}") {
            break;
        }
        if (line.find("root") == 0) {
            route.root = line.substr(4);
        }
        else if (line.find("index") == 0) {
            route.index = line.substr(5);
        }
        else if (line.find("methods") == 0) {
            std::stringstream ss(line.substr(7));
            std::string method;
            while (getline(ss, method, ',')) {
                route.methods.push_back(method);
            }
        }
        else if (line.find("redirect") == 0) {
            std::stringstream ss(line.substr(9));  // '9' to remove 'redirect '
            std::string status_code_str, new_path;
            ss >> status_code_str >> new_path;
            route.redirect_status = std::atoi(status_code_str.c_str());
            route.redirect_path = new_path;
        }
        else if (line.find("autoindex") == 0) {
            route.autoindex = (line.substr(9) == "on"); // eventuell auf off stellen - nicht gebraucht fuer webserv
        }
        else if (line.find("upload_dir") == 0) {
            route.upload_dir = line.substr(10);
        }
        else if (line.find("cgi_extension") == 0) {
            std::stringstream ss(line.substr(13));
            std::string ext;
            while (getline(ss, ext, ',')) {
                route.cgi_extension = ext;
            }
        }
    }
    return true;
}