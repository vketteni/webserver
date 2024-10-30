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

void GetRequestHandler::invoke(Request& request, Response& response, const LocationConfig & location, const ServerConfig & config)
{
	std::string root = !location.root.empty() ? location.root : config.root;

	std::string uri_path = request.getUri();
	if (uri_path.find('?') != std::string::npos)
	{
		uri_path = uri_path.substr(0, uri_path.find('?')); // Remove query string
	}
	std::string relative_path = uri_path.substr(location.path.length());
	std::string file_path = root + relative_path;


    if (isCGI(file_path))
    {
		CGIExecutor cgi;
		if (!cgi.executeCGI(request, response))
			response.setStatusCode(500);
		return ;
    }


	if (isDirectory(file_path))
	{
        std::string index_file = !location.index.empty() ? location.index : "index.html";
        std::string index_path = file_path + "/" + index_file;
        if (fileExists(index_path))
            file_path = index_path;
		else if (location.autoindex == "on")
		{
            // Generate directory listing
            response.setStatusCode(200);
            response.setBody(generateDirectoryListing(file_path, uri_path));
            response.setHeader("Content-Type", "text/html");
            return;
        }
		else
		{
            // Directory access is forbidden
            response.setStatusCode(403);
            return;
        }
    }

	std::ifstream file(file_path.c_str());
    if (!file)
    {
		response.setStatusCode(404);
		return ;
    }
    std::ostringstream contents;
    contents << file.rdbuf();
    file.close();

	response.setBody(contents.str());
	response.setHeader("Connection", "keep-alive");
}

void PostRequestHandler::invoke(Request& request, Response& response, const LocationConfig & location, const ServerConfig & config)
{
	std::string root = !location.root.empty() ? location.root : config.root;

	std::string uri_path = request.getUri();
	if (uri_path.find('?') != std::string::npos)
	{
		uri_path = uri_path.substr(0, uri_path.find('?')); // Remove query string
	}
	std::string relative_path = uri_path.substr(location.path.length());
	std::string file_path = root + relative_path;

    if (isCGI(request.getUri()))
    {
		CGIExecutor cgi;
		if (!cgi.executeCGI(request, response))
		{
            response.setHeader("Connection", "close");
			response.setStatusCode(500);
		}
		return ;
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




void DeleteRequestHandler::invoke(Request& request, Response& response, const LocationConfig & location, const ServerConfig & config)
{
    (void)request;
    (void)location;
    (void)config;
    (void)response;
}


