
#include "../incl/CGIExecutor.hpp"

// Default constructor
CGIExecutor::CGIExecutor()
{ return; }

// Default destructor
CGIExecutor::~CGIExecutor()
{ return; }


bool CGIExecutor::executeCGI(int client_fd, const std::string& scriptPath, const Request& request, Response& response)
{
    pid_t pid = fork();
    if (pid < 0)
	{
        std::cerr << "Failed to fork CGI process.\n";
        return false;
    }

    if (pid == 0)
	{
        // Child process: Execute the CGI script
        char* args[2];
		args[0] = (char*)scriptPath.c_str();
		args[1] = NULL;
        char** env = createCGIEnvironment(request);

        // Redirect input/output streams
        if (request.getMethod() == "POST")
		{
            dup2(client_fd, STDIN_FILENO);  // Redirect stdin to the request body
        }
        dup2(client_fd, STDOUT_FILENO); // Redirect stdout to the client socket

        // Execute CGI script
        if (execve(scriptPath.c_str(), args, env) == -1)
		{
            std::cerr << "Failed to execute CGI script: " << scriptPath << "\n";
            exit(1);  // Exit child process if execve fails
        }
    }
	else
	{
        // Parent process: Wait for the child to complete
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
            response.setStatusCode(200);
            response.setStatusMessage("OK");
            return true;
        }
		else
		{
            response.setStatusCode(500);
            response.setStatusMessage("Internal Server Error");
            return false;
        }
    }

    return true;
}

char** CGIExecutor::createCGIEnvironment(const Request& request)
{
    std::map<std::string, std::string> env_map;

    env_map["REQUEST_METHOD"] = request.getMethod();
    env_map["SCRIPT_NAME"] = request.getUri();
    env_map["QUERY_STRING"] = "";
    env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
    env_map["CONTENT_TYPE"] = request.getHeader("Content-Type");
    env_map["CONTENT_LENGTH"] = request.getHeader("Content-Length");

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

