#include "../incl/ConfigParser.hpp"

// Default constructor
ConfigParser::ConfigParser() {}

ConfigParser::~ConfigParser() {}

/*
    Method:         Server::parseConfig
    Description:    Parses the configuration file to extract ports
*/
bool ConfigParser::parseConfig(const std::string& filepath)
{
	normalizeSpaces(filepath.c_str());

    std::ifstream configFile(filepath.c_str());
    if (!configFile.is_open())
	{
        std::cerr << "Failed to open configuration file: " << filepath << "\n";
        return false;
    }

    std::string line;
    while (std::getline(configFile, line))
	{
		Utils::trim(line, WSPACE);
        if(line.empty() || line[0] == '#')
            continue ;
        if (line == ("server {"))
		{
            HostConfig config;
            if(!parseConfig(configFile, config))
                return false;
            host_configs[config.port] = config;
        }
    }
    configFile.close();
    return true;
}

const std::map<int, HostConfig> & ConfigParser::getHostConfigs() const
{
    return host_configs;
}

bool ConfigParser::parseConfig(std::ifstream& configFile, HostConfig& host_config)
{
    std::string line;
    while (std::getline(configFile, line))
	{
        line = Utils::trim(line, WSPACE);
        if (line.empty() || line[0] == '#') continue;
        if (line == "}") break;

        std::pair<std::string, std::string> directive = parseKeyValuePair(line);

        if (directive.first == "listen")
            host_config.port = parsePort(directive.second);
		else if (directive.first == "host")
            host_config.host = directive.second;
		else if (directive.first == "server_name")
            host_config.server_name = directive.second;
		else if (directive.first == "client_max_body_size")
            host_config.client_max_body_size = parseSize(directive.second);
		else if (directive.first == "error_page")
            parseErrorPage(directive.second, host_config);
		else if (directive.first == "root")
        {
            debug(directive.first);
            debug(directive.second);
            debug(host_config.port);
            host_config.root = directive.second;
        }
		else if (directive.first == "location")
            parseLocation(configFile, directive.second, host_config);
		else
            throw std::runtime_error("Unknown directive: " + directive.first);
        
    }
    return true;
}

void ConfigParser::parseRedirect(const std::string& directiveValue, RouteConfig& route)
{

    std::vector<std::string> tokens = Utils::split(directiveValue, ' ');
    if (tokens.size() < 2)
        throw std::runtime_error("Invalid redirect directive");
    route.redirect_status = std::atoi(tokens[0].c_str());
    route.redirect_path = tokens[1];
}

void ConfigParser::parseErrorPage(const std::string& directiveValue, HostConfig& host)
{
	std::vector<std::string> tokens = Utils::split(directiveValue, ' ');
	if (tokens.size() < 2)
		throw std::runtime_error("Invalid error_page directive");
	int code = std::atoi(tokens[0].c_str());
	host.error_pages[code] = tokens[1];
}

std::pair<std::string, std::string> ConfigParser::parseKeyValuePair(const std::string& line)
{
    std::string::size_type pos = line.find(' ');
    if (pos == std::string::npos)
        throw std::runtime_error("Invalid configuration line: " + line);
    std::string key = Utils::trim(line.substr(0, pos), WSPACE);
    std::string value = Utils::trim(line.substr(pos + 1), ";");
    return std::make_pair(key, value);
}

int ConfigParser::parsePort(const std::string& value)
{
    int port = std::atoi(value.c_str());
    if (port <= 0 || port > 65535)
        throw std::runtime_error("Invalid port number: " + value);
    return port;
}

int ConfigParser::parseSize(const std::string& value)
{
  
    int size = std::atoi(value.c_str());
  
    if (size <= 0)
        throw std::runtime_error("Invalid size value: " + value);
    return size;
}


void ConfigParser::parseLocation(std::ifstream& configFile, const std::string& directiveValue, HostConfig& host)
{
    RouteConfig route;

    if (!parseLocationBlock(configFile, route))
		throw std::runtime_error("Failed to parse location block");

	std::string key =  Utils::trim(Utils::trim(directiveValue, "{"), WSPACE);
	host.routes[key] = route;
}

bool ConfigParser::parseLocationBlock(std::ifstream& configFile, RouteConfig& route)
{
    std::string line;
    while (std::getline(configFile, line))
	{
        line = Utils::trim(line, WSPACE);
        if (line.empty() || line[0] == '#') continue;
        if (line == "}") break;

        std::pair<std::string, std::string> directive = parseKeyValuePair(line);

        if (directive.first == "root")
            route.root = directive.second;
		else if (directive.first == "index")
            route.index = directive.second;
		else if (directive.first == "methods")
            route.methods = Utils::split(directive.second, ',');
		else if (directive.first == "redirect")
            parseRedirect(directive.second, route);
		else if (directive.first == "upload_dir")
            route.upload_dir = directive.second;
		else if (directive.first == "cgi_extension")
            route.cgi_extension = directive.second;
    }
    return true;
}

void ConfigParser::normalizeSpaces(const char* filePath) {
    // File paths
    const char* tempFile = "temp.txt";

    // Open the input file for reading
    std::ifstream ifs(filePath);
    if (!ifs) {
        std::cerr << "Error: Cannot open the input file." << std::endl;
        return;
    }

    // Open a temporary file for writing
    std::ofstream ofs(tempFile);
    if (!ofs) {
        std::cerr << "Error: Cannot open the temp file." << std::endl;
        return;
    }

    // Read the file character by character and replace spaces with tabs
    char ch;
    while (ifs.get(ch)) {
        if (ch == '\t') {
            ofs.put(' ');  // Replace space with tab
        } else {
            ofs.put(ch);    // Write the character as is
        }
    }

    // Close both files
    ifs.close();
    ofs.close();

    // Remove the original file and rename the temp file to the original file name
    if (std::remove(filePath) != 0) {
        std::cerr << "Error: Cannot remove the original file." << std::endl;
        return;
    }
    if (std::rename(tempFile, filePath) != 0) {
        std::cerr << "Error: Cannot rename the temp file." << std::endl;
        return;
    }

}