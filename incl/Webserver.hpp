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

namespace http
{
	class Webserver
	{
		private:
			std::string 	m_ip_address;
			std::string 	m_serverMessage;
			int				m_socket;
			int				m_port;
			int				m_new_socket;
			long			m_incomingMessage;
			unsigned int	m_socketAddress_len;
			struct sockaddr	m_socketAddress;

			int				startServer();
			void			closeServer();

		public:
			Webserver(std::string ip_address, int port);
			~Webserver();

	};
} // namespace http

struct	sockaddr {
	short			sin_family; //for AF_Inet
	unsigned short	sin_port; // htons(8080)
	struct in_addr	sin_addr; // set to 0.0.0.0 as default IP
	char			sin_zero[8]; //buffer used to adjust the size of the sockaddr_in
};

struct in_addr {
	unsigned long	s_addr;
};

//https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7

#endif
