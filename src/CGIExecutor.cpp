
#include "../incl/CGIExecutor.hpp"

// Default constructor
CGIExecutor::CGIExecutor()
{ return; }

// Default destructor
CGIExecutor::~CGIExecutor()
{ return; }


bool CGIExecutor::executeCGI(Request & request, Response & response)
{
    char **env = createCGIEnvironment(request);
    // show the every value in **env
    for (int i = 0; env[i] != NULL; i++) {
        std::cout << env[i] << std::endl;
    }
    // debug body
    debug("body: " + request.getBody() + "\n");

    const char *path = request.getUri().c_str();
    char *argv[1];
    argv[0] = NULL;

    int stdout_pipe[2];  // Pipe für stdout
    int stdin_pipe[2];   // Pipe für stdin (POST Body)

    // Erstelle die Pipes
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
        // Schließe unnötige Enden der Pipes
        close(stdout_pipe[0]);  // stdout-Pipe: Lesen geschlossen
        close(stdin_pipe[1]);   // stdin-Pipe: Schreiben geschlossen

        // Umleiten von stdout und stdin
        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stdout_pipe[1], STDERR_FILENO);
        dup2(stdin_pipe[0], STDIN_FILENO);

        // Schließe die nicht mehr benötigten Enden
        close(stdout_pipe[1]);
        close(stdin_pipe[0]);

        // Führe das CGI-Skript aus
        if (execve(path, argv, env) == -1)
        {
            perror("CGI execution failed");
            exit(1);
        }
    }
    else  // Elternprozess
    {
        // Schließe unnötige Enden der Pipes
        close(stdout_pipe[1]);  // stdout: Schreiben geschlossen
        close(stdin_pipe[0]);   // stdin: Lesen geschlossen

        // Body comes from ** env
        std::string body = request.getBody(); 
        write(stdin_pipe[1], body.c_str(), body.size());

        // Schließe stdin-Pipe nach dem Schreiben
        close(stdin_pipe[1]);

        // Warte auf den CGI-Prozess
        int status;
        waitpid(pid, &status, 0);

        // Lese die Ausgabe des CGI-Skripts von stdout
        std::string output;
        char buffer[128];
        ssize_t bytes_read;
        while ((bytes_read = read(stdout_pipe[0], buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytes_read] = '\0';
            output += buffer;
        }
        close(stdout_pipe[0]);

        // Setze die CGI-Antwort im Response-Objekt
        response.setBody(output);

        // Fehlerprüfung für den CGI-Prozess
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

    env_map["REQUEST_METHOD"] = request.getMethod().substr(0);
    env_map["SCRIPT_NAME"] = request.getUri();
    env_map["QUERY_STRING"] = request.buildQueryString();
    env_map["SERVER_PROTOCOL"] = request.getVersion();
    env_map["CONTENT_TYPE"] = request.getHeaderOrDefault("Content-Type", "");
    env_map["CONTENT_LENGTH"] = request.getHeaderOrDefault("Content-Length", "0");

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
		

