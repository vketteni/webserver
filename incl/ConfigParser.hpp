#ifndef CONGIGPARSER_HPP
#define CONGIGPARSER_HPP

#include <unistd.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include "Debug.hpp"

struct LocationConfig
{
    std::string path;
    std::string root;
    std::string index;
    std::string autoindex;
    std::string upload_dir;
    std::string cgi_extension;
    std::vector<std::string> methods;
    int redirect_status;
    std::string redirect_path;

    LocationConfig() : redirect_status(0) {}
};

struct ServerConfig
{
    std::string root;
	int client_max_body_size;
    std::vector<int> ports;
    std::vector<std::string> serverNames;
    std::vector<LocationConfig> locations;
    std::map<int, std::string> error_pages;
};

class IConfigNode
{
	public:
		virtual ~IConfigNode() {}
		virtual void print(int indent = 0) const = 0;
};

class DirectiveNode : public IConfigNode
{
	public:
		std::string key;
		std::vector<std::string> values;

		DirectiveNode(const std::string& k) : key(k) {}
		void print(int indent = 0) const;
};

class BlockNode : public IConfigNode
{
	public:
		std::string name;
		std::vector<std::string> parameters;
		std::list<IConfigNode*> children;

		BlockNode(const std::string& n) : name(n) {}
		~BlockNode();
		void print(int indent = 0) const;
};

class ConfigParser
{
	public:
		ConfigParser(const std::string& filename);
		~ConfigParser();
		bool parse();
		void buildServerConfigs(std::map<int, ServerConfig> & server_configs);
		void printConfig() const;

	private:
		std::string filename;
		std::string fileContent;
		std::list<std::string> tokens;
		std::list<IConfigNode*> config_tree;
		std::string _cwd;

		bool readFile(const std::string& fname, std::string& content); // TODO: Use FileManager Class

		// Prepare Config Tree
		void tokenize();
		IConfigNode* parseTokens(std::list<std::string>::iterator& it);
		void parseBlockParameters(std::list<std::string>::iterator& it, std::vector<std::string>& directive_values);
		void parseBlockContents(std::list<std::string>::iterator& it, BlockNode* block);
		void parseDirectiveValues(std::list<std::string>::iterator& it, std::vector<std::string>& values);
		void skipCommentsAndWhitespace(std::list<std::string>::iterator& it);
		IConfigNode* handleInclude(std::list<std::string>::iterator& it);
		void tokenizeString(const std::string& content, std::list<std::string>& tokenList);

		// Create Server Configs
		void parseServerBlock(BlockNode* server_block, ServerConfig & server_config);
		void parseLocationBlock(BlockNode* location_block, ServerConfig & server_config);
};

struct MatchDirectiveKey
{
    const std::string & key;

    MatchDirectiveKey(const std::string & key) : key(key) {}

	template <typename T>
    bool operator()(const std::pair<const std::string, T>& key_value_pair) const
	{
        return  key_value_pair.first == key;
    }
};

typedef void (*ServerDirectiveHandler)(std::vector<std::string> & directive_values, ServerConfig & config);
typedef void (*LocationDirectiveHandler)(std::vector<std::string> & directive_values, LocationConfig & location);

void setup_directive_handlers(std::map<std::string, ServerDirectiveHandler> & handler);
void setup_directive_handlers(std::map<std::string, LocationDirectiveHandler> & handler);
void handle_client_max_body_size(std::vector<std::string> & directive_values, ServerConfig & config);
void handle_port(std::vector<std::string> & directive_values, ServerConfig & config);
void handle_host(std::vector<std::string> & directive_values, ServerConfig & config);
void handle_server_name(std::vector<std::string> & directive_values, ServerConfig & config);
void handle_root(std::vector<std::string> & directive_values, ServerConfig & server);
void handle_autoindex(std::vector<std::string> & directive_values, LocationConfig & location);
// void handle_location(std::vector<std::string> & directive_values, ServerConfig & config);
void handle_error_page(std::vector<std::string> & directive_values, ServerConfig & config);

void handle_http_method(std::vector<std::string> & directive_values, LocationConfig & location);
void handle_path(std::vector<std::string> & directive_values, LocationConfig & location);
void handle_root(std::vector<std::string> & directive_values, LocationConfig & location);
void handle_index(std::vector<std::string> & directive_values, LocationConfig & location);
void handle_upload_dir(std::vector<std::string> & directive_values, LocationConfig & location);
void handle_cgi_extension(std::vector<std::string> & directive_values, LocationConfig & location);
void handle_redirect(std::vector<std::string> & directive_values, LocationConfig & location);

#endif 