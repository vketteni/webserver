#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <iostream>

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

void printRequest(const HTTPRequest & request);
void printHTTPHeaders(std::map<std::string, std::string> & headers);
void printHTTPHeader(std::map<std::string, std::string> & headers, const std::string & name);

#endif