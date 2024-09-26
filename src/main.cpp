#include "../incl/Webserver.hpp"

#define PORT 8082
#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 1024

// Helper function to set a socket to non-blocking mode
void setNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "Error getting socket flags" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::cerr << "Error setting non-blocking mode" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Global data structures to handle client data
std::map<int, std::string> read_buffers;  // Buffer for partial reads per client
std::map<int, std::string> write_buffers; // Buffer for partial writes per client

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create server socket (IPv4, TCP)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set the server socket to non-blocking mode
    setNonBlocking(server_fd);

    // Define server address (IPv4, port 8080)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified port and IP address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Set up poll structure to monitor multiple sockets
    struct pollfd poll_fds[MAX_CONNECTIONS];
    for (int i = 0; i < MAX_CONNECTIONS; ++i) {
        poll_fds[i].fd = -1; // Initialize to -1 (no file descriptor)
    }
    poll_fds[0].fd = server_fd;  // Add server socket to the poll set
    poll_fds[0].events = POLLIN; // Watch for incoming connections (readable)

    std::cout << "Server is listening on port " << PORT << std::endl;

    // Main event loop
    while (true) {
        int poll_count = poll(poll_fds, MAX_CONNECTIONS, -1); // Wait for events
        if (poll_count < 0) {
            std::cerr << "Poll failed" << std::endl;
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // Loop through all poll_fds to handle events
        for (int i = 0; i < MAX_CONNECTIONS; ++i) {
            // Check for new connections on the server socket
            if (poll_fds[i].fd == server_fd && (poll_fds[i].revents & POLLIN)) {
                if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                    std::cerr << "Accept failed" << std::endl;
                    continue;
                }

                // Set the new socket to non-blocking mode
                setNonBlocking(new_socket);

                // Add the new socket to the poll set
                for (int j = 1; j < MAX_CONNECTIONS; ++j) {
                    if (poll_fds[j].fd == -1) {
                        poll_fds[j].fd = new_socket;
                        poll_fds[j].events = POLLIN; // Monitor for readability
                        std::cout << "New connection established" << std::endl;
                        break;
                    }
                }
            }

            // Check for readability (POLLIN) on client sockets
            if (poll_fds[i].fd != server_fd && poll_fds[i].fd != -1 && (poll_fds[i].revents & POLLIN)) {
                char buffer[BUFFER_SIZE] = {0};
                int valread = read(poll_fds[i].fd, buffer, BUFFER_SIZE);

                if (valread > 0) {
                    // Append the data to the read buffer for this client
                    read_buffers[poll_fds[i].fd] += std::string(buffer, valread);

                    // Check if the message is complete (simplified logic, you can adjust for your protocol)
                if (read_buffers[poll_fds[i].fd].find("\r\n\r\n") != std::string::npos) {
                    // Process the complete request
                    std::cout << "Complete request received from client" << std::endl;
           //         write_buffers[poll_fds[i].fd] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!"; // Response
                      std::string html_response =
					"HTTP/1.1 200 OK\r\n"
					"Content-Type: text/html\r\n"
					"Content-Length: ";

					std::string html_body =
					"<!DOCTYPE html>\n"
					"<html>\n"
				    "<head>\n"
				    "<title>Welcome to Hennis World!</title>\n"
				    "<style>\n"
				    "html { color-scheme: dark green; }\n"
				    "body { width: 35em; margin: 0 auto;\n"
				    "font-family: Tahoma, Arial, Arial, serif;\n"
				    "background-color: #90ee00;\n"
				    "color: #f08080}\n"
				    "a {\n"
				    "    color: #0066cc;\n"
				    "}\n"
				    "a:hover {\n"
				    "    color: #ff0000;\n"
				    "}\n"
				    ".input-box {\n"
				    "    margin-top: 20px;\n"
				    "}\n"
				    "</style>\n"
				    "</head>\n"
				    "<body>\n"
				    "<h1>Welcome to VinHennis World!</h1>\n"
				    "<p>Think of a number between 0 and 15</p>\n"
				    "<p>That's your spirit animal: "
				    "<a href=\"https://www.tiktok.com/@bangkokpost.official/video/7415169714932043026\">click here</a>.</p>\n"
				    "<p>You as a flower: "
				    "<a href=\"https://www.google.com/search?sca_esv=7746fac465ae621b&sca_upv=1&q=fledermaus+blume&udm=2&fbs=AEQNm0A6bwEop21ehxKWq5cj-cHa02QUie7apaStVTrDAEoT1FdmHTSMJP_rxgj2yFrTtBkpn7UArWKMytkxok4kynJeftYBbqn-Ez1pg2pPqfulAAELa9yA80L_cabL9a6EJ4sNogkLT1whdNbW2y6Ka0Llqz6M0tEc1mD0fQnpgheyhLwK9mtDc-zJ2yTyPMZ0mV7RzKWn&sa=X&sqi=2&ved=2ahUKEwjF7Z_G4OCIAxUT_rsIHaGXPUoQtKgLegQIDRAB&biw=960&bih=880&dpr=1\">click here</a>.</p>\n"
				    "<div class=\"input-box\">\n"
				    "    <label for=\"userInput\">What's your superpower?:</label><br>\n"
				    "    <input type=\"text\" id=\"userInput\" name=\"userInput\" placeholder=\"Answer me please..\">\n"
				    "</div>\n"
				    "<p><em>Thank you for visiting.</em></p>\n"
				    "</body>\n"
				    "</html>\n";

						// Calculate content length
						int content_length = html_body.length();

						// Append the content length to the header
						std::ostringstream oss;
						oss << content_length; // Convert int to string
						html_response += oss.str() + "\r\n\r\n" + html_body;

    					// Set the response for the client
    						write_buffers[poll_fds[i].fd] = html_response;

    					// Switch to POLLOUT to monitor for writable events
    						poll_fds[i].events = POLLOUT;
    					}
                } else if (valread == 0) {
                    // Client disconnected, clean up
                    std::cout << "Client disconnected" << std::endl;
                    close(poll_fds[i].fd);
                    poll_fds[i].fd = -1;
                    read_buffers.erase(poll_fds[i].fd);
                    write_buffers.erase(poll_fds[i].fd);
                }
            }

            // Check for writable events (POLLOUT) on client sockets
            if (poll_fds[i].fd != server_fd && poll_fds[i].fd != -1 && (poll_fds[i].revents & POLLOUT)) {
                std::string &response = write_buffers[poll_fds[i].fd];
                int valwrite = write(poll_fds[i].fd, response.c_str(), response.size());

                if (valwrite > 0) {
                    // Erase the written part of the buffer
                    response.erase(0, valwrite);

                    // If all data is written, switch back to POLLIN
                    if (response.empty()) {
                        poll_fds[i].events = POLLIN; // Switch back to reading
                        std::cout << "Response sent, switching back to read" << std::endl;
                    }
                }
            }
        }
    }

    // Cleanup
    close(server_fd);
    return 0;
}
