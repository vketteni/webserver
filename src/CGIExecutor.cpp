
#include "../incl/CGIExecutor.hpp"

// Default constructor
CGIExecutor::CGIExecutor()
{ return; }

// Default destructor
CGIExecutor::~CGIExecutor()
{ return; }


bool CGIExecutor::executeCGI(const Request & request, Response & response)
{
	char **env = createCGIEnvironment(request);
	const char * path = request.getUri().c_str();
	char *argv[1];
	argv[0] = NULL;
    
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
	{
        perror("Pipe creation failed");
        return false;
    }

    pid_t pid = fork();
    if (pid < 0)
	{
        perror("Fork failed");
        return false;
    }
    if (pid == 0)
	{
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        dup2(pipe_fd[1], STDERR_FILENO);
        close(pipe_fd[1]);
        if (execve(path, argv, env) == -1)
		{
            perror("Cgi Execution failed");
            exit(1);
        }
    }
    else
	{ 
        close(pipe_fd[1]);
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			std::string output;
			char buffer[128];
			ssize_t bytes_read;
			while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1)) > 0)
			{
				buffer[bytes_read] = '\0';
				output += buffer;         
			}
			close(pipe_fd[0]);
			response.setBody(output);
			return true;
        }
    }
	return false;
}

char** CGIExecutor::createCGIEnvironment(const Request& request)
{
    std::map<std::string, std::string> env_map;

    env_map["REQUEST_METHOD"] = request.getMethod();
    env_map["SCRIPT_NAME"] = request.getUri();
    env_map["QUERY_STRING"] = request.getQueryString();
    env_map["SERVER_PROTOCOL"] = request.getVersion();
    env_map["CONTENT_TYPE"] = "text/html"; //request.getHeader("Content-Type");
    env_map["CONTENT_LENGTH"] = "100"; //request.getHeader("Content-Length");

    char** env = new char*[env_map.size() + 1];
    int i = 0;
    for (std::map<std::string, std::string>::iterator it = env_map.begin(); it != env_map.end(); ++it)
	{
        std::string var = it->first + "=" + it->second;
        env[i] = new char[var.size() + 1];
        std::strcpy(env[i], var.c_str());
        i++;
    }
    env[i] = NULL;
    return env;
}

