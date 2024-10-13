#ifndef CGIEXECUTOR_HPP
#define CGIEXECUTOR_HPP

#include <ctime>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include "Request.hpp"

#include "Request.hpp"
#include "Response.hpp"



class CGIExecutor {
public:
    CGIExecutor();
    ~CGIExecutor();

    bool executeCGI(const Request & request, Response & response);
	bool isCGI(const std::string &path);

private:
    char** createCGIEnvironment(const Request& request);
};


#endif

