#include "../incl/MethodHandler.hpp"

std::string extractSessionId(const std::string &cookie){
    std::string session_id;
    std::string::size_type start = cookie.find("session_id=");
    if (start != std::string::npos) {
        start += 10; // Length of "session_id="
        std::string::size_type end = cookie.find(";", start);
        if (end == std::string::npos) {
            end = cookie.length();
        }
        session_id = cookie.substr(start, end - start);
    }
    return session_id;
};
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
	std::string relative_path = Utils::trim(uri_path.substr(location.path.length()), "/");
	relative_path = relative_path.size() == 0 && !location.index.empty() ? location.index : relative_path;
	std::string absolute_path = root + relative_path;

    pretty_debug("File Path: " + absolute_path);
    if (isCGI(absolute_path))
    {
		CGIExecutor cgi;
		if (!cgi.executeCGI(request, response))
			response.setStatusCode(500);
		return ;
    }

    // if cookie is set in header do something else proceed
    if (request.getHeaders().find("Cookie") != request.getHeaders().end())
    {
        std::string cookie = request.getHeaders().at("Cookie");
        std::string session_id = extractSessionId(cookie);
        if (!session_id.empty())
        {
            response.setBody("Session ID: " + session_id);
            response.setStatusCode(200);
            return;
        }
    }

	if (isDirectory(absolute_path))
	{
        std::string index_file = location.index;
        std::string index_path = Utils::trim(absolute_path, "/") + "/" + index_file;
        if (fileExists(index_path))
            absolute_path = index_path;
		else if (location.autoindex == "on")
		{
            // Generate directory listing
            response.setStatusCode(200);
            response.setBody(generateDirectoryListing(absolute_path, uri_path));
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

	std::ifstream file(absolute_path.c_str());
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
    // this is for the bonus, do uncomment and recompile 
   // response.setHeader("Set-Cookie", "session_id=1234; Max-Age=12; Path=/; HttpOnly");
}

void PostRequestHandler::invoke(Request& request, Response& response, const LocationConfig & location, const ServerConfig & config)
{
	std::string root = !location.root.empty() ? location.root : config.root;

	std::string uri_path = request.getUri();
	if (uri_path.find('?') != std::string::npos)
	{
		uri_path = uri_path.substr(0, uri_path.find('?')); // Remove query string
	}
	// std::string relative_path = uri_path.substr(location.path.length());
	std::string relative_path = Utils::build_relative_path_from_location_match(request.getUri(), location.path);

	std::string absolute_path = root + relative_path;
    request.setUri(absolute_path);
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
        if (writeFileDirectly("www/uploads", filename, file_content)) {
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
    std::string file_name = extractFileName(request.getUri());
    std::string upload_folder = "www/uploads/";
    std::string absolute_path = upload_folder + file_name;
   (void) location;
    (void) config;
   
    if (deleteFile(absolute_path ))
    {
       response.setStatusCode(200);
    }
    else
    {
       response.setStatusCode(404);
    }
}


