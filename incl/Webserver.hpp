#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# include <iostream>
# include <cstring>
# include <unistd.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h> // for socket programming
# include <poll.h>
# include <fcntl.h>
# include <map>
# include <string>
# include <cstdlib>
# include <sstream>


// # include "IMessage.hpp"
// # include "URI.hpp"

class Webserver
{
	public:
		Webserver();
		~Webserver();

	private:
};

#endif
