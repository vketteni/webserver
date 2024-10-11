#include "StatusCode.hpp"

// Definiere die statische Map
std::map<int, std::string> StatusCode::statusMessages;

void StatusCode::initializeStatusMessages() {
    // Initialisiere die Map mit den Status-Codes und Nachrichten
    // 1xx Informational
    statusMessages[100] = "Continue";
    statusMessages[101] = "Switching Protocols";
    statusMessages[102] = "Processing";
    
    // 2xx Success
    statusMessages[200] = "OK";
    statusMessages[201] = "Created";
    statusMessages[202] = "Accepted";
    statusMessages[203] = "Non-Authoritative Information";
    statusMessages[204] = "No Content";
    statusMessages[205] = "Reset Content";
    statusMessages[206] = "Partial Content";
    statusMessages[207] = "Multi-Status";
    
    // 3xx Redirection
    statusMessages[300] = "Multiple Choices";
    statusMessages[301] = "Moved Permanently";
    statusMessages[302] = "Found";
    statusMessages[303] = "See Other";
    statusMessages[304] = "Not Modified";
    statusMessages[305] = "Use Proxy";
    statusMessages[307] = "Temporary Redirect";
    
    // 4xx Client Errors
    statusMessages[400] = "Bad Request";
    statusMessages[401] = "Unauthorized";
    statusMessages[402] = "Payment Required";
    statusMessages[403] = "Forbidden";
    statusMessages[404] = "Not Found";
    statusMessages[405] = "Method Not Allowed";
    statusMessages[406] = "Not Acceptable";
    statusMessages[407] = "Proxy Authentication Required";
    statusMessages[408] = "Request Timeout";
    statusMessages[409] = "Conflict";
    statusMessages[410] = "Gone";
    statusMessages[411] = "Length Required";
    statusMessages[412] = "Precondition Failed";
    statusMessages[413] = "Payload Too Large";
    statusMessages[414] = "URI Too Long";
    statusMessages[415] = "Unsupported Media Type";
    statusMessages[416] = "Range Not Satisfiable";
    statusMessages[417] = "Expectation Failed";
    statusMessages[418] = "I'm a teapot";  // Easter Egg (RFC 2324)
    statusMessages[422] = "Unprocessable Entity";
    statusMessages[423] = "Locked";
    statusMessages[424] = "Failed Dependency";
    statusMessages[426] = "Upgrade Required";
    
    // 5xx Server Errors
    statusMessages[500] = "Internal Server Error";
    statusMessages[501] = "Not Implemented";
    statusMessages[502] = "Bad Gateway";
    statusMessages[503] = "Service Unavailable";
    statusMessages[504] = "Gateway Timeout";
    statusMessages[505] = "HTTP Version Not Supported";
    statusMessages[507] = "Insufficient Storage";
    statusMessages[508] = "Loop Detected";
    statusMessages[510] = "Not Extended";
}

const std::string& StatusCode::getStatusMessage(int code) {
    // Führe die Initialisierung einmalig aus, falls notwendig
    if (statusMessages.empty()) {
        initializeStatusMessages();
    }

    static const std::string unknownStatus = "Unknown Status Code";
    
    std::map<int, std::string>::const_iterator it = statusMessages.find(code);
    if (it != statusMessages.end()) {
        return it->second;
    } else {
        return unknownStatus;
    }
}
