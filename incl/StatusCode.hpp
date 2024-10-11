#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP

#include <map>
#include <string>

// StatusCode.hpp
class StatusCode {
public:
    static const std::string& getStatusMessage(int code);

private:
    static std::map<int, std::string> statusMessages;  // Kein Initializer hier
    static void initializeStatusMessages();  // Methode zur Initialisierung der Map
};
#endif