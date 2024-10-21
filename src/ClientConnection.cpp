#include "../incl/ClientConnection.hpp"
#include "../incl/ConfigParser.hpp"
#include <sys/stat.h>

// Funktion zum Konvertieren von int zu std::string in C++98
std::string intToString(int number) {
    std::ostringstream oss;
    oss << number;  // Schreibe die Zahl in den Stream
    return oss.str();
}

// Funktion zum Senden eines Redirects
void ClientConnection::sendRedirect(const std::string& redirect_url, int status_code) {
    std::ostringstream response;
    response << "HTTP/1.1 " << status_code << " Moved Permanently\r\n"
             << "Location: " << redirect_url << "\r\n"
             << "Content-Length: 0\r\n"
             << "Connection: close\r\n\r\n";

    std::string response_str = response.str();
    ssize_t bytes_sent = send(this->fd, response_str.c_str(), response_str.size(), 0);

    if (bytes_sent == -1)
    {
        perror("send");
    }

    std::cout << "Sent redirect to: " << redirect_url << "\n";
}



ClientConnection::ClientConnection(int client_fd, HostConfig & server_config)
    : _lastActivity(std::time(NULL)), _host_config(server_config), fd(client_fd), timeout(TIMEOUT_DURATION) {}

ClientConnection::ClientConnection(const ClientConnection &other)
    : _host_config(other._host_config), fd(other.fd), timeout(other.timeout)
{
	 _lastActivity = other._lastActivity;
}

ClientConnection &ClientConnection::operator=(const ClientConnection &other)
{
	// TODO
	(void)other;
	return *this;
}

ClientConnection::~ClientConnection()
{
}

time_t ClientConnection::getLastActivity(void)
{
	return (_lastActivity);
}

void ClientConnection::setLastActivity(time_t last_activity)
{
	_lastActivity = last_activity;
}

bool ClientConnection::processRequest()
{
    if (!processRequestReading())
    {
        return false;
    }
    if (!processRequestParsing())
    {
        return false;
    }
    if (this->_request_parser.isComplete())
    {
        Request request = this->_request_parser.getRequest();
        this->_request_parser.reset();

        // Hol dir die Host-Konfiguration
        HostConfig &host_config = _host_config;
        std::string root = host_config.root;

        debug(host_config.port);
        debug(host_config.host);
        debug(root);

        // Zuerst die Redirect-Konfiguration auslesen
        ConfigParser config_parser_test;
        config_parser_test.parseConfig("/home/ohoro/webserver/conf/index_test.conf");
        std::map<int, HostConfig> host_configs = config_parser_test.getHostConfigs();
        config_parser_test.printRedirectsAndRoutes(host_configs);  // Optional, nur zur Anzeige

        // Prüfen, ob es einen Redirect für die angeforderte URL gibt
        std::map<std::string, RouteConfig>::iterator route_it = host_config.routes.find(request.getUri());
        if (route_it != host_config.routes.end() && route_it->second.redirect_status != 0)
        {
            // Redirect gefunden - setze die neue URL und sende den Redirect
            std::string redirect_url = route_it->second.redirect_path;
            request.setUri(redirect_url);

            // Hier würdest du den 301 Redirect senden
            sendRedirect(redirect_url, route_it->second.redirect_status);
            return true;  // Beende hier, da der Redirect gesendet wurde
        }

        // Wenn kein Redirect gefunden wurde, prüfe die Route
        if (route_it != host_config.routes.end())
        {
            // Route gefunden - setze den Pfad entsprechend der Route
            std::string new_route = route_it->second.root;
            request.setUri(root + new_route);
        }
        else
        {
            // Keine spezielle Route - setze den Standardpfad
            request.setUri(root + request.getUri());
        }

        // Fehlerbehandlung: 404, falls Datei nicht existiert
        std::string newUrl = "/404.html"; 
        struct stat fileInfo;
        if (::stat(request.getUri().c_str(), &fileInfo) == -1) {
            // Datei existiert nicht, sende 404-Seite
            request.setUri(root + newUrl);
        }

        debug(request.getUri());

        // Verarbeite die Antwort auf Basis der Anfrage
        if (!processResponse(request))
        {
            return false;
        }
    }
    return true;
}

bool ClientConnection::processRequestReading()
{
    ssize_t bytes_read = recv(this->fd, this->_buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0)
	{
        // Handle disconnection or error
        return false;
    }
    _buffer[bytes_read] = '\0';
	return true;
}

bool ClientConnection::processRequestParsing()
{
    _request_parser.appendData(std::string(_buffer));
    if (!_request_parser.parse())
	{
        return false;
    }
	return true;
}

bool ClientConnection::processResponse(Request & request)
{
	//HeaderProcessor headerProcessor;
	//headerProcessor.processHeaders(request);

	Response response;
	AbstractMethodHandler* method_handler = getHandlerForMethod(request.getMethod());
	if (method_handler)
	{
		method_handler->invoke(request, response);
		delete method_handler;
	}
	else
	{
		response.setStatusCode(405);
		response.setStatusMessage("Method Not Allowed");
	}

	if (!sendResponse(response))
	{
		return false;
	}
    return true;
}

bool ClientConnection::sendResponse(Response & response) {
	debug(response.getStatusCode());
    std::ostringstream oss;
    oss << "HTTP/1.1 " << response.getStatusCode() << " " << response.getStatusMessage() << "\r\n";
    oss << "Content-Type: " << "text/html; charset=utf-8" << "\r\n";
    oss << "Content-Length: " << response.getBody().size() << "\r\n";
	const std::map<std::string, std::string> & headers = response.getHeaders();
	for (std::map<std::string, std::string>::const_iterator headerIterator = headers.begin(); headerIterator != headers.end(); ++headerIterator)
	{
        
        oss << headerIterator->first << ": " << headerIterator->second << "\r\n";
        debug(oss.str());
    }
    oss << "\r\n";
    oss << response.getBody();
    std::string responseStr = oss.str();
    // debug(responseStr);
    ssize_t bytesSent = send(fd, responseStr.c_str(), responseStr.size(), 0);
    return bytesSent == static_cast<ssize_t>(responseStr.size());
}

bool ClientConnection::sendBasicResponse(const std::string &body, int status_code, const std::string &content_type)
{
    std::ostringstream response;
    response << "HTTP/1.1 " << status_code << " OK\r\n"
             << "Content-Type: " << content_type << "\r\n"
             << "Content-Length: " << body.size() << "\r\n"
             << "Connection: keep-alive\r\n\r\n"
             << body;

    std::string response_str = response.str();
    ssize_t bytes_sent = send(this->fd, response_str.c_str(), response_str.size(), 0);

    if (bytes_sent == -1)
	{
        perror("send");
        return false;
    }

    std::cout << "Sent response: " << response_str << "\n";
    return true;
}
