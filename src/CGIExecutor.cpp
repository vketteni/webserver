
#include "CGIExecutor.hpp"

// Default constructor
CGIExecutor::CGIExecutor() { return; }

// Default destructor
CGIExecutor::~CGIExecutor() { return; }



bool CGIExecutor::executeCGI(const std::string script_path, int client_fd)
{
	Request request;

	pid_t	pid = fork();

	if (pid < 0) {
		perror("Fork failed");
		return false;
	}

	if (pid == 0) {
		request.setCgiEnvVariables();
		dup2(client_fd, STDOUT_FILENO);
		dup2(client_fd, STDERR_FILENO);

		if(execl(script_path.c_str(), script_path.c_str(), NULL) == -1) {
			perror("Cgi Execution failed");
			exit(1);
		}
	}
	else {
		if (waitpid(pid, NULL, 0) == -1) {
			perror("Waitpid failed");
			return false;
		}
	}
	return true;
}
