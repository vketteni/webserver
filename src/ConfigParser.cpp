#include "../incl/ConfigParser.hpp"
#include "ConfigParser.hpp"

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
	Method: ConfigParser::getPort
	Description: Returns the configured port number.
*/

int	ConfigParser::getPort() const
{
	return _port;
}

/*
	Method: ConfigParser::getHost
	Description: Returns the configured host address.
*/

std::string	ConfigParser::getHost() const
{
	return _host;
}

/*
	Method: 		Server::parseConfig
	Description: 	Parses the configuration file to extract ports
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


bool	ConfigParser::parseServer(std::ifstrean& configFile, ServerConfig& server) {

	std::string	line;

	while (std::getline(configFile, line)) {
		// Remove whitespace
		line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
		if(line.empty() || line[0] == '#') {
			continue ;
		}
		if (line == ("server{")) {
	if (_port == 0)
	{
		std::cerr << "No valid port found in config\n";
		return false;
	}

	std::cout << "Configured to listen on port: " << _port << "\n";
	std::cout << "Configured server name: " << _host << "\n";


}
