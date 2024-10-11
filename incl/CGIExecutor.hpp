#include <ctime>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <Request.hpp>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>


#ifndef CGIEXECUTOR_HPP_
#define CGIEXECUTOR_HPP_

// Class declaration
class CGIExecutor {
 public:
  CGIExecutor();
  ~CGIExecutor();

  void executeCGI(const std::string script_path, int client_fd);

 private:

};

#endif // CGIEXECUTOR_HPP_

