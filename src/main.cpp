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

	std::string config_file("conf/webserv.conf");
	if (argc != 1)
		config_file = argv[1];

    // Initialize and start the server
    Server server(config_file);
    if (!server.start()) {
        std::cerr << "Failed to start the server.\n";
        return 1;
    }

    return 0;
}











