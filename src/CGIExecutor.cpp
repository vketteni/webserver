
#include "../incl/CGIExecutor.hpp"

CGIExecutor::CGIExecutor()
{ return; }

CGIExecutor::~CGIExecutor()
{ return; }


bool CGIExecutor::executeCGI(Request & request, Response & response)
{
    char **env = createCGIEnvironment(request);
    std::string path = request.getUri();

    char *argv[3];  // Array für Interpreter, Skript und NULL

    std::string interpreter;
    if (path.find(".py") != std::string::npos) {
        interpreter = "/usr/bin/python3";
    }
    else if (path.find(".php") != std::string::npos) {
        interpreter = "/usr/bin/php";
    } else {
        std::cerr << "Unsupported CGI script type" << std::endl;
        return false;
    }

    argv[0] = const_cast<char*>(interpreter.c_str());
    argv[1] = const_cast<char*>(path.c_str());
    argv[2] = NULL;
    int stdout_pipe[2];
    int stdin_pipe[2];

    if (pipe(stdout_pipe) == -1 || pipe(stdin_pipe) == -1)
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

    if (pid == 0)  // Kindprozess
    {
        close(stdout_pipe[0]);
        close(stdin_pipe[1]);

        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stdout_pipe[1], STDERR_FILENO);
        dup2(stdin_pipe[0], STDIN_FILENO);

        close(stdout_pipe[1]);
        close(stdin_pipe[0]);

        if (execve(argv[0], argv, env) == -1)
        {
            perror("CGI execution failed");
            exit(1);
        }
    }
    else
    {
        close(stdout_pipe[1]);
        close(stdin_pipe[0]);

        std::string body = request.getBody();
        write(stdin_pipe[1], body.c_str(), body.size());

        close(stdin_pipe[1]);

        int status;
        waitpid(pid, &status, 0);

        std::string output;
        char buffer[128];
        ssize_t bytes_read;
        while ((bytes_read = read(stdout_pipe[0], buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytes_read] = '\0';
            output += buffer;
        }
        close(stdout_pipe[0]);

        response.setBody(output);

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            std::cerr << "CGI failed with error code " << WEXITSTATUS(status) << std::endl;
            return false;
        }
    }

    return true;
}



char** CGIExecutor::createCGIEnvironment(Request& request)
{
    std::map<std::string, std::string> env_map;

	env_map["REQUEST_METHOD"] = request.getMethod();
    env_map["SCRIPT_NAME"] = request.getUri();
    env_map["SERVER_PROTOCOL"] = request.getVersion();

	if (request.getMethod() == "GET")
		env_map["QUERY_STRING"] = request.buildQueryString();
	else if (request.getMethod() == "POST")
	{
		env_map["CONTENT_TYPE"] = request.getHeaderOrDefault("Content-Type", "");
		env_map["CONTENT_LENGTH"] = request.getHeaderOrDefault("Content-Length", "0");
	}

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

bool isCGI(const std::string & path)
{
	return path.find("/cgi-bin/") == 0 || path.find(".cgi") != std::string::npos
		|| path.find(".py") != std::string::npos || path.find(".php") != std::string::npos;
}

