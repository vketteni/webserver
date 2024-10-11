#include "StatusCode.hpp"

// Definiere die statische Map
std::map<int, std::string> StatusCode::statusMessages;

void StatusCode::initializeStatusMessages() {
    // Initialisiere die Map mit den Status-Codes und Nachrichten

    // 2xx Success
    statusMessages[200] = "OK"; //
    statusMessages[201] = "Created"; //
    statusMessages[204] = "No Content"; //

    // 3xx Redirection
    statusMessages[301] = "Moved Permanently"; //
    statusMessages[302] = "Found"; //
    statusMessages[304] = "Not Modified"; //

    // 4xx Client Errors
    statusMessages[400] = "Bad Request"; //
    statusMessages[401] = "Unauthorized"; //
    statusMessages[403] = "Forbidden"; //
    statusMessages[404] = "Not Found"; //
    statusMessages[405] = "Method Not Allowed"; //
    statusMessages[408] = "Request Timeout"; //
    statusMessages[418] = "I'm a teapot";  // Easter Egg (RFC 2324)

    // 5xx Server Errors
    statusMessages[500] = "Internal Server Error"; //
    statusMessages[502] = "Bad Gateway"; //
    statusMessages[503] = "Service Unavailable"; //
    statusMessages[504] = "Gateway Timeout"; //

}

const std::string& StatusCode::getStatusMessage(int code) {
    // Führe die Initialisierung einmalig aus, falls notwendig
    if (statusMessages.empty()) {
        initializeStatusMessages();
    }

    static const std::string unknownStatus = "I am a teapot";

    std::map<int, std::string>::const_iterator it = statusMessages.find(code);
    if (it != statusMessages.end()) {
        return it->second;
    } else {
        return unknownStatus;
    }
}
