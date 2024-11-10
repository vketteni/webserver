#include "../incl/ConfigParser.hpp"

BlockNode::~BlockNode()

{
	for (std::list<IConfigNode*>::iterator it = children.begin(); it != children.end(); ++it)
		delete *it;
}

ConfigParser::ConfigParser(const std::string& filename)
 	: filename(filename)
{
	char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
        _cwd = std::string(cwd);
    } else {
        std::cerr << "Error getting current directory" << std::endl;
        _cwd = "";
    }
}

ConfigParser::~ConfigParser()
{
	config_tree.size();
	for (std::list<IConfigNode*>::iterator it = config_tree.begin(); it != config_tree.end(); ++it)
		delete *it;
}

bool ConfigParser::parse()
{
	if (!readFile(filename, fileContent))
	{
		std::cerr << "Failed to read file: " << filename << std::endl;
		return false;
	}

	tokenize();
	// std::list<std::string>::iterator print_it = tokens.begin();
	// for(; print_it != tokens.end(); ++print_it)
	// 	pretty_debug(*print_it);

	std::list<std::string>::iterator it = tokens.begin();
	while (it != tokens.end())
	{
		IConfigNode* node = parseTokens(it);
		if (node)
			config_tree.push_back(node);
	}
	return true;
}

bool ConfigParser::readFile(const std::string& fname, std::string& content)
{
	std::ifstream file(fname.c_str());
	if (!file.is_open())
		return false;
	std::stringstream ss;
	ss << file.rdbuf();
	content = ss.str();
	file.close();
	return true;
}

void ConfigParser::tokenize()
{
	std::istringstream stream(fileContent);
	char ch;
	std::string currentToken;
	bool inString = false;
	char stringDelimiter = '\0';

	while (stream.get(ch))
	{
		if (inString)
		{
			if (ch == stringDelimiter)
			{
				currentToken += ch;
				tokens.push_back(currentToken);
				currentToken.clear();
				inString = false;
			}
			else
				currentToken += ch;
		}
		else
		{
			if (ch == '"' || ch == '\'')
			{
				if (!currentToken.empty())
				{
					tokens.push_back(currentToken);
					currentToken.clear();
				}
				inString = true;
				stringDelimiter = ch;
				currentToken += ch;
			}
			else if (ch == '{' || ch == '}' || ch == ';' || ch == '(' || ch == ')')
			{
				if (!currentToken.empty())
				{
					tokens.push_back(currentToken);
					currentToken.clear();
				}
				tokens.push_back(std::string(1, ch));
			}
			else if (isspace(ch))
			{
				if (!currentToken.empty())
				{
					tokens.push_back(currentToken);
					currentToken.clear();
				}
			}
			else if (ch == '#')
			{
				// Skip the rest of the line (comment)
				if (!currentToken.empty())
				{
					tokens.push_back(currentToken);
					currentToken.clear();
				}
				std::string comment;
				std::getline(stream, comment);
			}
			else
				currentToken += ch;
		}
	}
	if (!currentToken.empty())
		tokens.push_back(currentToken);
}

IConfigNode* ConfigParser::parseTokens(std::list<std::string>::iterator& it)
{
	skipCommentsAndWhitespace(it);
	if (it == tokens.end()) return NULL;

	std::string token = *it;
	++it;

	if (token == "include")
		return handleInclude(it);
	else if (token == "server" || token == "location" || token == "if" || token == "http")
	{
		BlockNode* block = new BlockNode(token);
		parseBlockParameters(it, block->parameters);
		if (it == tokens.end() || *it != "{")
		{
			std::cerr << "Expected '{' after block declaration" << std::endl;
			delete block;
			return NULL;
		}
		++it; // Skip '{'
		parseBlockContents(it, block);
		return block;
	}
	else
	{
		// It's a directive
		DirectiveNode* directive = new DirectiveNode(token);
		parseDirectiveValues(it, directive->values);
		if (it == tokens.end() || *it != ";")
		{
			std::cerr << "Expected ';' after directive" << std::endl;
			delete directive;
			return NULL;
		}
		++it; // Skip ';'
		return directive;
	}
}

void ConfigParser::parseBlockParameters(std::list<std::string>::iterator& it, std::vector<std::string>& params)
{
	while (it != tokens.end() && *it != "{")
	{
		params.push_back(*it);
		++it;
	}
}

void ConfigParser::parseBlockContents(std::list<std::string>::iterator& it, BlockNode* block)
{
	while (it != tokens.end() && *it != "}")
	{
		IConfigNode* child = parseTokens(it);
		if (child)
			block->children.push_back(child);
		skipCommentsAndWhitespace(it);
	}
	if (it != tokens.end() && *it == "}")
		++it; // Skip '}'
	else
		std::cerr << "Expected '}' at the end of block" << std::endl;
}

void ConfigParser::parseDirectiveValues(std::list<std::string>::iterator& it, std::vector<std::string>& values)
{
	while (it != tokens.end() && *it != ";")
	{
		values.push_back(*it);
		++it;
	}
}

void ConfigParser::skipCommentsAndWhitespace(std::list<std::string>::iterator& it)
{
	while (it != tokens.end())
	{
		if ((*it).empty() || (*it)[0] == '#')
			++it;
		else
			break;
	}
}

IConfigNode* ConfigParser::handleInclude(std::list<std::string>::iterator& it)
{
	if (it == tokens.end())
	{
		std::cerr << "Expected filename after include" << std::endl;
		return NULL;
	}
	std::string includeFile = *it;
	++it;
	if (it == tokens.end() || *it != ";")
	{
		std::cerr << "Expected ';' after include directive" << std::endl;
		return NULL;
	}
	++it; // Skip ';'

	if (!includeFile.empty() && (includeFile[0] == '"' || includeFile[0] == '\''))
		includeFile = includeFile.substr(1, includeFile.size() - 2);

	std::string includedContent;
	if (!readFile(includeFile, includedContent))
	{
		std::cerr << "Failed to read included file: " << includeFile << std::endl;
		return NULL;
	}

	// Tokenize included content
	std::list<std::string> includedTokens;
	tokenizeString(includedContent, includedTokens);

	// Insert included tokens into the current token stream
	tokens.insert(it, includedTokens.begin(), includedTokens.end());

	return NULL; // Include does not produce a node
}

void ConfigParser::tokenizeString(const std::string& content, std::list<std::string>& tokenList)
{
	std::istringstream stream(content);
	char ch;
	std::string currentToken;
	bool inString = false;
	char stringDelimiter = '\0';

	while (stream.get(ch))
	{
		if (inString)
		{
			if (ch == stringDelimiter)
			{
				currentToken += ch;
				tokenList.push_back(currentToken);
				currentToken.clear();
				inString = false;
			}
			else
				currentToken += ch;
		}
		else
		{
			if (ch == '"' || ch == '\'')
			{
				if (!currentToken.empty())
				{
					tokenList.push_back(currentToken);
					currentToken.clear();
				}
				inString = true;
				stringDelimiter = ch;
				currentToken += ch;
			}
			else if (ch == '{' || ch == '}' || ch == ';' || ch == '(' || ch == ')')
			{
				if (!currentToken.empty())
				{
					tokenList.push_back(currentToken);
					currentToken.clear();
				}
				tokenList.push_back(std::string(1, ch));
			}
			else if (isspace(ch))
			{
				if (!currentToken.empty())
				{
					tokenList.push_back(currentToken);
					currentToken.clear();
				}
			}
			else if (ch == '#')
			{
				if (!currentToken.empty())
				{
					tokenList.push_back(currentToken);
					currentToken.clear();
				}
				std::string comment;
				std::getline(stream, comment);
			}
			else
				currentToken += ch;
		}
	}
	if (!currentToken.empty())
		tokenList.push_back(currentToken);
}

void ConfigParser::buildServerConfigs(std::map<int, ServerConfig> & server_configs)
{
 	DirectiveNode * directive;
	BlockNode * block_directive;

	if ((block_directive = dynamic_cast<BlockNode*> (*config_tree.begin())) && block_directive->name == "http")
	{
		std::list<IConfigNode*>::iterator http_it = block_directive->children.begin();
		for (; http_it != block_directive->children.end(); ++http_it)
		{
			BlockNode * block_directive;
			if ((block_directive = dynamic_cast<BlockNode*> (*http_it)))
			{
				if (block_directive->name == "server")
				{
					ServerConfig server_config;
					init_default_values(server_config);

					parseServerBlock(block_directive, server_config);
					server_configs[server_config.ports.front()] = server_config;
				}
			}
			else if ((directive = dynamic_cast<DirectiveNode*> (*http_it)))
			{
				// Todo: handle http directives
			}
			else
				throw std::runtime_error("Error: Unknown directive type in 'http' directive");
		}
	}
	else
		throw std::runtime_error("Error: Configuration file is missing 'http' directive");
}

void ConfigParser::parseServerBlock(BlockNode* server_block, ServerConfig & server_config)
{
	DirectiveNode * directive;
	BlockNode * block_directive;
	std::map<std::string, ServerDirectiveHandler> directive_handlers;

	setup_directive_handlers(directive_handlers);

	std::list<IConfigNode*>::iterator server_block_it = server_block->children.begin();
	for (; server_block_it != server_block->children.end(); ++server_block_it)
	{
		if ((block_directive = dynamic_cast<BlockNode*> (*server_block_it)) && block_directive->name == "location")
			parseLocationBlock(block_directive, server_config);
		else if ((directive = dynamic_cast<DirectiveNode*> (*server_block_it)))
		{
			std::map<std::string, ServerDirectiveHandler>::const_iterator handler_it = std::find_if(
			directive_handlers.begin(), directive_handlers.end(), MatchDirectiveKey(directive->key));
			if (handler_it != directive_handlers.end())
							directive_handlers[directive->key](directive->values, server_config);
			else
			{
				throw std::runtime_error("Error: Unknown directive name in 'server' directive");
			}
		}
		else
			throw std::runtime_error("Error: Unknown directive type in 'server' directive");
	}
}

void ConfigParser::parseLocationBlock(BlockNode* location_block, ServerConfig & server_config)
{
	LocationConfig location;
	DirectiveNode * directive;

	std::string location_path = location_block->parameters.front();
	location.path = location_path.find_first_of('/') == 0 ? location_path.substr(1) : location_path;

	std::map<std::string, LocationDirectiveHandler> directive_handlers;
	setup_directive_handlers(directive_handlers);

	std::list<IConfigNode*>::iterator location_block_it = location_block->children.begin();
	for (; location_block_it != location_block->children.end(); ++location_block_it)
	{
		if ((directive = dynamic_cast<DirectiveNode*> (*location_block_it)))
		{
			std::map<std::string, LocationDirectiveHandler>::iterator handler_it = std::find_if(
				directive_handlers.begin(),
				directive_handlers.end(),
				MatchDirectiveKey(directive->key)
			);

			if (handler_it != directive_handlers.end())
			{
				directive_handlers[directive->key](directive->values, location);
			}
			else
			{
				throw std::runtime_error("Error: Unknown directive name in 'location' directive");
			}

		}
		else
			throw std::runtime_error("Error: Unknown directive type in 'location' directive");
	}

	// If methods not specified
	if (location.methods.empty()) {
    location.methods.clear();
    location.methods.push_back("GET");
    location.methods.push_back("POST");
    location.methods.push_back("DELETE");
	}

	// Other not specified
	location.autoindex = location.autoindex.empty() ? "off" : location.autoindex;
	location.cgi_extension = location.cgi_extension.empty() ? ".py" : location.autoindex;
	location.index = location.index.empty() ? "" : location.index;
	location.upload_dir = location.upload_dir.empty() ? "uploads/" : location.autoindex;

	server_config.locations.push_back(location);
}

void DirectiveNode::print(int indent) const
{
	std::string indentation(indent, ' ');
	std::cout << indentation << key;
	for (size_t i = 0; i < values.size(); ++i)
	{
		std::cout << " " << values[i];
	}
	std::cout << ";" << std::endl;
}

void BlockNode::print(int indent) const
{
	std::string indentation(indent, ' ');
	std::cout << indentation << name;
	for (size_t i = 0; i < parameters.size(); ++i)
		std::cout << " " << parameters[i];
	std::cout << " {" << std::endl;

	for (std::list<IConfigNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
		(*it)->print(indent + 4);

	std::cout << indentation << "}" << std::endl;
}

void ConfigParser::printConfig() const
{
	for (std::list<IConfigNode*>::const_iterator it = config_tree.begin(); it != config_tree.end(); ++it)
		(*it)->print();
}

void setup_directive_handlers(std::map<std::string, ServerDirectiveHandler> & handler)
{
	handler.clear();
	handler["client_max_body_size"] = &handle_client_max_body_size;
	handler["listen"] = &handle_port;
	handler["host"] = &handle_host;
	handler["error_page"] = &handle_error_page;
	handler["root"] = &handle_server_root;

}


void setup_directive_handlers(std::map<std::string, LocationDirectiveHandler> & handler)
{
	handler.clear();
	handler["methods"] = &handle_http_method;
	handler["root"] = &handle_location_root;
	handler["index"] = &handle_index;
	handler["upload_dir"] = &handle_upload_dir;
	handler["cgi_extension"] = &handle_cgi_extension;
	handler["redirect"] = &handle_redirect;
	handler["rewrite"] = &handle_rewrite;
	handler["autoindex"] = &handle_autoindex;
}

void handle_client_max_body_size(std::vector<std::string> & directive_values, ServerConfig & config)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'client_max_body_size' directive requires exactly one value.");

	std::string directive_value = directive_values.front();
	std::stringstream ss;
	ss << directive_value;
	size_t client_max_body_size;
	ss >> client_max_body_size;

	config.client_max_body_size = client_max_body_size;
}


void handle_port(std::vector<std::string> & directive_values, ServerConfig & config)
{
	if (directive_values.empty())
		throw std::runtime_error("Error: 'port' directive has no value.");

	std::string directive_value = directive_values.front();
	std::stringstream ss;
	ss << directive_value;
	int port;
	ss >> port;
	config.ports.front() = port;
}

void handle_host(std::vector<std::string> & directive_values, ServerConfig & config)
{
	if (directive_values.empty())
		throw std::runtime_error("Error: 'handle_host' directive has no value.");

	config.host = std::string(directive_values.front());
}


void handle_error_page(std::vector<std::string> & directive_values, ServerConfig & config)
{
	if (directive_values.size() != 2)
		throw std::runtime_error("Error: 'error_page' directive requires exactly two values, code and page.");

	std::stringstream ss;
	ss << directive_values.front();
	int error_code;
	ss >> error_code;

	std::string error_page = directive_values[1];
	config.error_pages[error_code] = error_page[0] == '/' ? error_page.substr(1) : error_page;
	std::cout << "Error page for code " << error_code << " set to " << directive_values[1] << std::endl;
}

void handle_http_method(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.empty())
		throw std::runtime_error("Error: 'method' directive was declared but has no value.");
	for (std::vector<std::string>::iterator it = directive_values.begin(); it != directive_values.end(); ++it)
	{
		location.methods.push_back(*it);
	}

}

void handle_path(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'path' directive requires exactly one value.");
	location.path = directive_values.front();
}

void handle_location_root(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'root' directive requires exactly one value.");
	std::string root = directive_values.front();
	// Add ending slash if it's missing
	location.root = root[root.size() - 1] != '/' ? root + "/" : root;
}

void handle_server_root(std::vector<std::string> & directive_values, ServerConfig & server)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'root' directive requires exactly one value.");
	std::string root = directive_values.front();
	// Add ending slash if it's missing
	server.root = root[root.size() - 1] != '/' ? root + "/" : root;
}

void handle_index(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'index' directive requires exactly one value.");
	location.index = directive_values.front();
}

void handle_autoindex(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'autoindex' directive requires exactly one value.");

	std::string autoindex = directive_values.front();
	if (autoindex != "on" && autoindex != "off")
		throw std::runtime_error("Error: 'autoindex' directive value is not allowed. Allowed: 'on', 'off'.");

	location.autoindex = autoindex;
}

void handle_upload_dir(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'upload_dir' directive requires exactly one value.");
	location.upload_dir = directive_values.front();
}

void handle_cgi_extension(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'cgi_extension' directive requires exactly one value.");
	location.cgi_extension = directive_values.front();
}


void handle_redirect(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 2)
		throw std::runtime_error("Error: 'redirect' directive requires exactly two values, status-code and redirect-path.");

	std::stringstream ss;
	ss << directive_values.front();
	int status_code;
	ss >> status_code;
	location.redirect_status = status_code;
	location.redirect_path = directive_values[1];
}

void handle_rewrite(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'rewrite' directive requires exactly one value.");

	location.rewrite = directive_values.front();
}

const LocationConfig * findMatchingLocation(const std::string &normalized_uri, const std::vector<LocationConfig> &locations)
{
	// Longest Match Approach
	// This function selects the most specific match
	const LocationConfig * best_match = NULL;
	for (std::vector<LocationConfig>::const_iterator location_it = locations.begin(); location_it != locations.end(); ++location_it)
	{
		const std::string & path = location_it->path;
		// Allow empty path as default fallback
		if (!best_match && path.empty())
		{
			best_match = &(*location_it);
			continue;
		}

		// ** Case 1: Exact or Prefix Match (e.g., `/put_test/*`)
		if (path[path.size() - 1] == '*' && path.size() > 1) // Ensure it's a wildcard prefix
		{
			std::string prefix = path[path.size() - 2] == '/' ? path.substr(0, path.size() - 2) : path.substr(0, path.size() - 1); // Remove trailing '*'
			if (normalized_uri.find(prefix) == 0) // Matches prefix
			{
				// Ensure longest match takes precedence
				if (!best_match || best_match->path.size() < path.size())
					best_match = &(*location_it);
			}
		}
		// ** Case 2: Suffix Match (e.g., `*.bla`)
		else if (path[0] == '*' && path.size() > 1) // Ensure it's a wildcard suffix
		{
			std::string suffix = path.substr(1); // Remove leading '*'
			if (normalized_uri.size() >= suffix.size() &&
				normalized_uri.compare(normalized_uri.size() - suffix.size(), suffix.size(), suffix) == 0) // Matches suffix
			{
				// Ensure longest match takes precedence
				if (!best_match || best_match->path.size() < path.size())
					best_match = &(*location_it);
			}
		}
		// ** Case 3: Exact or Default Prefix Match
		else if ((normalized_uri.find(path)) == 0)
		{
			// Regular prefix match, prioritize longest match
			if (!best_match || best_match->path.size() < path.size())
				best_match = &(*location_it);
		}
	}
	return best_match;
}

void printConfigLocations(const ServerConfig & config)
{
    std::vector<LocationConfig>::const_iterator location_it = config.locations.begin();
    for (; location_it != config.locations.end(); ++location_it)
    {
        std::cerr << "Location: " << location_it->path << std::endl;
        std::cerr << "Root: " << location_it->root << std::endl;
        std::cerr << "Redirect: " << location_it->redirect_path << std::endl;
        std::cerr << "Redirect Status: " << location_it->redirect_status << std::endl;
		std::cerr << "Methods:";
		std::vector<std::string>::const_iterator method_it = location_it->methods.begin();
        for (; method_it != location_it->methods.end(); ++method_it)
		{
			std::cerr << " " << *method_it << std::endl;
		}
		std::cerr << std::endl;
    }
}

std::string joinMethods(const std::vector<std::string>& methods) {
    std::ostringstream oss;
    for (size_t i = 0; i < methods.size(); ++i) {
        oss << methods[i];
        if (i < methods.size() - 1)
            oss << ", ";
    }
    return oss.str();
}

void init_default_values(ServerConfig & server_config)
{
	server_config.client_max_body_size = DEFAULT_MAX_BODY_SIZE;
	server_config.ports.push_back(DEFAULT_PORT);
	server_config.root = DEFAULT_ROOT;
}
