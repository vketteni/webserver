
#include "CGIExecutor.hpp"

// Default constructor
CGIExecutor::CGIExecutor() { return; }

// Default destructor
CGIExecutor::~CGIExecutor() { return; }



void CGIExecutor::executeCGI(const std::string script_path, int client_fd)
{
	Request request;

	request.setCgiEnvVariables(script_path);

	pid_t	pid = fork();

	if (pid < 0) {
		perror("Fork failed");
		return;
	}

	if (pid == 0) {
		dup2(client_fd, STDOUT_FILENO);
		dup2(client_fd, STDERR_FILENO);

		if(execl(script_path.c_str(), script_path.c_str(), NULL) == -1) {
			perror("Cgi Execution failed");
			exit(1);
		}
	}
	else {
		waitpid(pid, NULL, 0);
	}
}
