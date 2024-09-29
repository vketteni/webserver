#include "../incl/Server.hpp"

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << "Usage ./webserv [optional:config_file]\n";
		return 1;
	}
    // Register signal handler for graceful shutdown
    signal(SIGINT, signalHandler);
	std::string configFile;

	// if (argc == 1)
	if (true)
		configFile = "server.conf";
	else
		configFile = argv[1];
    
    // Create a sample configuration file
    std::ofstream ofs(configFile.c_str());
    if (!ofs.is_open()) {
        std::cerr << "Failed to create configuration file.\n";
        return 1;
    }
    ofs << "ports=8080,9090\n";
    ofs.close();



    // Initialize and start the server
    Server server(configFile);
    if (!server.start()) {
        std::cerr << "Failed to start the server.\n";
        return 1;
    }

    return 0;
}
