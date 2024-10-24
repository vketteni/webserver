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

	// Remove quotes if present
	if (!includeFile.empty() && (includeFile[0] == '"' || includeFile[0] == '\''))
		includeFile = includeFile.substr(1, includeFile.size() - 2);

	// Read the included file
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
				// Skip the rest of the line (comment)
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
					server_config.root = "";

					parseServerBlock(block_directive, server_config);
					if (server_config.ports.empty())
						server_configs[8080] = server_config;
					else
						server_configs[server_config.ports[0]] = server_config;
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
				directive_handlers.begin(),
				directive_handlers.end(),
				MatchDirectiveKey(directive->key)
			);
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
	location.root = server_config.root;
	location.path = location_block->parameters[0];
	

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
	handler["server_name"] = &handle_server_name;
	// handler["location"] = &handle_location;
	handler["error_page"] = &handle_error_page;
	handler["root"] = &handle_root;

}


void setup_directive_handlers(std::map<std::string, LocationDirectiveHandler> & handler)
{
	handler.clear();
	handler["methods"] = &handle_http_method;
	// handler["path"] = &handle_path;
	handler["root"] = &handle_root;
	handler["index"] = &handle_index;
	handler["upload_dir"] = &handle_upload_dir;
	handler["cgi_extension"] = &handle_cgi_extension;
	handler["redirect"] = &handle_redirect;
	handler["autoindex"] = &handle_autoindex;
}

void handle_client_max_body_size(std::vector<std::string> & directive_values, ServerConfig & config)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'client_max_body_size' directive requires exactly one value."); 
	
	std::string directive_value = directive_values[0];
	std::stringstream ss;
	ss << directive_value;
	int client_max_body_size;
	ss >> client_max_body_size;

	config.client_max_body_size = client_max_body_size;
}


void handle_port(std::vector<std::string> & directive_values, ServerConfig & config)
{
	if (directive_values.empty())
		throw std::runtime_error("Error: 'port' directive has no value."); 

	for (std::vector<std::string>::iterator it = directive_values.begin(); it != directive_values.end(); ++it)
	{
		std::string directive_value = *it;
		std::stringstream ss;
		ss << directive_value;
		int port;
		ss >> port;
		config.ports.push_back(port);
	}

}

void handle_host(std::vector<std::string> & directive_values, ServerConfig & config)
{
	(void)directive_values;
	(void)config;
	// Todo: implement host directive
}

void handle_server_name(std::vector<std::string> & directive_values, ServerConfig & config)
{
	if (directive_values.empty())
		throw std::runtime_error("Error: 'server_name' directive has no value."); 

	for (std::vector<std::string>::iterator it = directive_values.begin(); it != directive_values.end(); ++it)
		config.serverNames.push_back(*it);
}

void handle_error_page(std::vector<std::string> & directive_values, ServerConfig & config)
{
	if (directive_values.size() != 2)
		throw std::runtime_error("Error: 'error_page' directive requires exactly two values, code and page."); 

	std::stringstream ss;
	ss << directive_values[0];
	int error_code;
	ss >> error_code;
	config.error_pages[error_code] = directive_values[1];
}

void handle_http_method(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.empty())
		throw std::runtime_error("Error: 'method' directive was declared but has no value."); 

	for (std::vector<std::string>::iterator it = directive_values.begin(); it != directive_values.end(); ++it)
		location.methods.push_back(*it);
	
}

void handle_path(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'path' directive requires exactly one value."); 
	location.path = directive_values[0];
}

void handle_root(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'root' directive requires exactly one value."); 
	location.root = directive_values[0];
}

void handle_root(std::vector<std::string> & directive_values, ServerConfig & server)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'root' directive requires exactly one value."); 
	server.root = directive_values[0];
}

void handle_index(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'index' directive requires exactly one value."); 
	location.index = directive_values[0];
}

void handle_autoindex(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'autoindex' directive requires exactly one value."); 
	location.autoindex = directive_values[0];
}

void handle_upload_dir(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'upload_dir' directive requires exactly one value."); 
	location.upload_dir = directive_values[0];
}

void handle_cgi_extension(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 1)
		throw std::runtime_error("Error: 'cgi_extension' directive requires exactly one value."); 
	location.cgi_extension = directive_values[0];
}


void handle_redirect(std::vector<std::string> & directive_values, LocationConfig & location)
{
	if (directive_values.size() != 2)
		throw std::runtime_error("Error: 'redirect' directive requires exactly two values, status-code and redirect-path."); 

	std::stringstream ss;
	ss << directive_values[0];
	int status_code;
	ss >> status_code;
	location.redirect_status = status_code;
	location.redirect_path = directive_values[1];
}
