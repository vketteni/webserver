#include "../incl/MethodHandler.hpp"

	// if (method == "GET") {
    //     // Serve the file
    //     if (fileExists(filePath)) {
    //         response.statusCode = 200; // OK
    //         response.body = readFile(filePath);
    //         response.headers["Content-Type"] = getContentType(filePath);
    //     } else {
    //         // File not found
    //         response.statusCode = 404; // Not Found
    //         response.body = "404 Not Found";
    //     }
    // } else if (method == "POST") {
    //     // Handle file upload
    //     if (!locationConfig.upload_dir.empty()) {
    //         std::string uploadPath = locationConfig.upload_dir + "/" + extractFileName(request.uri);
    //         if (saveFile(uploadPath, request.body)) {
    //             response.statusCode = 201; // Created
    //             response.body = "File uploaded successfully.";
    //         } else {
    //             response.statusCode = 500; // Internal Server Error
    //             response.body = "Failed to upload file.";
    //         }
    //     } else {
    //         response.statusCode = 403; // Forbidden
    //         response.body = "Upload directory not configured.";
    //     }
    // } else {
    //     // Other methods are not implemented
    //     response.statusCode = 501; // Not Implemented
    //     response.body = "501 Not Implemented";
    // }
	
AbstractMethodHandler* getHandlerForMethod(const std::string& method)
{
    if (method == "GET")
	{
        return new GetRequestHandler();
    }
	else if (method == "POST")
	{
        return new PostRequestHandler();
    }
	else if (method == "DELETE")
	{
        return new DeleteRequestHandler();
    }

    return NULL;
}

void GetRequestHandler::invoke(Request& request, Response& response)
{

    if (isCGI(request.getUri()))
    {
		processCGI(request, response);
        return ;  // CGI wurde erfolgreich behandelt
    }

	std::ifstream file(request.getUri().c_str());
    if (!file)
    {
        setErrorResponse(response, 404, "File Not Found");
        return ;
    }

    std::ostringstream contents;
    contents << file.rdbuf();  // Read the file buffer into the stream

    file.close();

    buildResponse(response, 200, "OK", contents.str(), "keep-alive");
}std::string extractBoundary(const std::map<std::string, std::string>& headers) {
    // Suche nach dem Content-Type Header
    std::map<std::string, std::string>::const_iterator it = headers.find("Content-Type");
    if (it == headers.end()) {
        std::cerr << "Content-Type Header nicht gefunden." << std::endl;
        return "";
    }

    // Suche nach "boundary=" in der Content-Type-Zeile
    const std::string& content_type = it->second;
    size_t boundary_pos = content_type.find("boundary=");
    if (boundary_pos == std::string::npos) {
        std::cerr << "Boundary Parameter nicht gefunden." << std::endl;
        return "";
    }

    // Extrahiere die Boundary nach "boundary="
    boundary_pos += 9; // 9 ist die Länge von "boundary="
    size_t end_of_boundary = content_type.find_first_of("\r\n", boundary_pos);
    std::string boundary;

    // Wenn kein Ende gefunden wurde, gehe bis zum Ende der Zeichenkette
    if (end_of_boundary == std::string::npos) {
        boundary = content_type.substr(boundary_pos);
    } else {
        boundary = content_type.substr(boundary_pos, end_of_boundary - boundary_pos);
    }

    return boundary;
}


bool parseMultipartData(const std::string& body, const std::string& boundary, std::string& out_filename, std::vector<char>& out_filecontent) {
    // Finde die Start-Boundary
    size_t boundary_pos = body.find("--" + boundary);
    if (boundary_pos == std::string::npos) {
        std::cerr << "Boundary nicht gefunden." << std::endl;
        return false;
    }

    // Suche nach `Content-Disposition` und Dateinamen
    size_t disposition_pos = body.find("Content-Disposition:", boundary_pos);
    if (disposition_pos == std::string::npos) {
        std::cerr << "Content-Disposition Header nicht gefunden." << std::endl;
        return false;
    }

    // Suche den Dateinamen in der `Content-Disposition` Zeile
    size_t filename_pos = body.find("filename=\"", disposition_pos);
    if (filename_pos == std::string::npos) {
        std::cerr << "Dateiname nicht gefunden." << std::endl;
        return false;
    }
    filename_pos += 10; // Überspringe `filename="`
    size_t filename_end = body.find("\"", filename_pos);
    out_filename = body.substr(filename_pos, filename_end - filename_pos);

    // Suche das Ende der Header und den Anfang der Binärdaten
    size_t content_pos = body.find("\r\n\r\n", filename_end);
    if (content_pos == std::string::npos) {
        std::cerr << "Kein Inhalt gefunden." << std::endl;
        return false;
    }
    content_pos += 4; // Überspringe die Kopfzeilenendmarkierung

    // Finde die End-Boundary
    std::string end_boundary = "\r\n--" + boundary + "--"; // mit zwei zusätzlichen Bindestrichen am Ende
    size_t end_boundary_pos = body.find(end_boundary, content_pos);
    if (end_boundary_pos == std::string::npos) {
        std::cerr << "Ende des Inhalts nicht gefunden." << std::endl;
        return false;
    }

    // Dateiinhalt (binär) extrahieren
    out_filecontent.resize(end_boundary_pos - content_pos); // Resize the vector to hold the file content
    std::copy(body.begin() + content_pos, body.begin() + end_boundary_pos, out_filecontent.begin());
    return true;
}

bool writeFileDirectly(const std::string& directory, const std::string& filename, const std::vector<char>& file_data) {
    // Kompletter Dateipfad
    std::string file_path = directory + "/" + filename;

    // Verzeichnis erstellen, falls es nicht existiert
    struct stat dir_stat;
    if (stat(directory.c_str(), &dir_stat) != 0) {
        // Verzeichnis erstellen, falls es noch nicht existiert
        if (mkdir(directory.c_str(), 0755) != 0) {
            std::cerr << "Fehler: Verzeichnis konnte nicht erstellt werden: " << directory << std::endl;
            return false;
        }
    }

    // Datei im Binärmodus öffnen, wobei bestehende Inhalte überschrieben werden
    std::ofstream file(file_path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Fehler: Datei konnte nicht geöffnet werden: " << file_path << std::endl;
        return false;
    }

    // Dateiinhalt schreiben
    file.write(file_data.data(), file_data.size());
    if (!file.good()) {
        std::cerr << "Fehler: Beim Schreiben der Datei ist ein Problem aufgetreten." << std::endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}


void PostRequestHandler::invoke(Request& request, Response& response)
{
    if (isCGI(request.getUri()))
    {
        // CGI ausführen
        processCGI(request, response);

        // Verbindung und Body je nach Erfolg des CGI-Prozesses setzen
        if (response.getStatusCode() == 200)  // CGI erfolgreich
        {
            response.setHeader("Connection", "keep-alive");
            // Hier wird angenommen, dass der Body im response bereits von processCGI gesetzt wurde.
        }
        else  // CGI fehlgeschlagen
        {
            response.setHeader("Connection", "close");
            response.setBody("CGI execution failed.");
        }

        return;  // CGI wurde erfolgreich oder nicht erfolgreich behandelt
    }

    std::string boundary = extractBoundary(request.getHeaders()); // Die Boundary extrahieren
    std::string filename;
    std::vector<char> file_content;

    if (parseMultipartData(request.getBody(), boundary, filename, file_content)) {
        if (writeFileDirectly("uploads", filename, file_content)) {
         std::cout << "Datei erfolgreich hochgeladen: " << filename << std::endl;
        } else {
            std::cerr << "Fehler beim Hochladen der Datei." << std::endl;
     }
    } else {
        std::cerr << "Fehler beim Parsen der Multipart-Daten." << std::endl;
    }
    // Einfache Antwort für nicht-CGI POST-Anfragen
    response.setStatusMessage("POST request received");
    response.setStatusCode(200);
    response.setHeader("Connection", "keep-alive");
}




void DeleteRequestHandler::invoke(Request& request, Response& response)
{
    (void)request;
    (void)response;
}

void AbstractMethodHandler::processCGI(Request& request, Response& response)
{
    CGIExecutor cgi_executor;
	std::vector<std::string> env_vars;
	env_vars.push_back("REQUEST_METHOD=" + request.getMethod());

	// CGI-Skript ausführen, GET oder POST spezifisch
	if (request.getMethod() == "GET")
	{
		env_vars.push_back("QUERY_STRING=" + request.buildQueryString());
	}
	else if (request.getMethod() == "POST")
	{
		std::stringstream ss;
		ss << request.getBody().size();
		env_vars.push_back("CONTENT_LENGTH=" + ss.str());
	}

	// Skript ausführen
	if (!cgi_executor.executeCGI(request, response))
	{
		response.setStatusCode(500);
		response.setStatusMessage("Internal Server Error");
	}
	else
	{
		response.setStatusCode(200);
		response.setStatusMessage("OK");
	}
}

bool isCGI(const std::string & path)
{
	return path.find("/cgi-bin/") == 0 || path.find(".cgi") != std::string::npos
		|| path.find(".py") != std::string::npos;
}

bool AbstractMethodHandler::shouldRedirect(const Request& request) {
    // Beispiel: Wenn die URI "/old-path" ist, leite auf "/new-path" um
    if (request.getUri() == "/old-path") {
        return true;
    }
    return false;
}

void AbstractMethodHandler::setRedirect(Response& response, const std::string& location) {
    response.setStatusCode(301);
    response.setStatusMessage("Moved Permanently");
    response.setHeader("Location", location);
}

void AbstractMethodHandler::setErrorResponse(Response& response, int statusCode, const std::string& message) {
    response.setStatusCode(statusCode);
    response.setStatusMessage(message);
    response.setHeader("Connection", "close");
}

void AbstractMethodHandler::buildResponse(Response& response, int statusCode, const std::string& statusMessage, const std::string& body, const std::string& connection) {
    response.setStatusCode(statusCode);
    response.setStatusMessage(statusMessage);
    response.setBody(body);
    response.setHeader("Connection", connection);
}
