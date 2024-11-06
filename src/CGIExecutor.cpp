
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

    // Umwandeln von const char* in std::string
    std::string path = request.getUri();

    char *argv[3];  // Array für Interpreter, Skript und NULL
    argv[2] = NULL; // NULL für das Ende der Argumente

    // Erkennen des Interpreters und Skripts
    std::string interpreter;
    if (path.find(".py") != std::string::npos) {
        interpreter = "/usr/bin/python3"; // Python-Interpreter
    }
    else if (path.find(".php") != std::string::npos) {
        interpreter = "/usr/bin/php-cgi"; // PHP-Interpreter
    } else {
        std::cerr << "Unsupported CGI script type" << std::endl;
        return false;
    }

    // Setze die Argumente für execve
    argv[0] = const_cast<char*>(interpreter.c_str());  // Interpreter
    argv[1] = const_cast<char*>(path.c_str());        // Skript-Pfad

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

        // Führe das CGI-Skript mit dem entsprechenden Interpreter aus
        if (execve(argv[0], argv, env) == -1)
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

        // Body kommt aus dem POST-Body
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

