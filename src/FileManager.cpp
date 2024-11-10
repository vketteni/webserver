#include "../incl/FileManager.hpp"


FileManager::FileManager(const std::string& baseDirectory) : base_directory(baseDirectory), max_file_size(10485760) // Default to 10 MB
{
    if (!base_directory.empty() && base_directory[base_directory.length() - 1] != '/')
        base_directory += '/';
}

void FileManager::setMaxFileSize(size_t maxSize)
{
    max_file_size = maxSize;
}

bool FileManager::constructSafePath(const std::string& file_path, std::string& outsafe_path)
{
    char real_path[PATH_MAX];
    std::string full_path = base_directory + file_path;
    std::string resolved_path(real_path);

    if (resolved_path.find(base_directory) != 0)
	{
        return false;
    }

    outsafe_path = resolved_path;
    return true;
}

bool FileManager::readFile(const std::string& file_path, std::string& out_data)
{
    std::string safe_path;
    if (!constructSafePath(file_path, safe_path))
	{
        return false;
    }

    struct stat file_stat;
    if (stat(safe_path.c_str(), &file_stat) != 0)
	{
        return false;
    }

    if (!S_ISREG(file_stat.st_mode))
	{
        return false;
    }
    std::ifstream file(safe_path.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open())
	{
        return false;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    out_data = ss.str();

    file.close();
    return true;
}

bool FileManager::writeFile(const std::string& file_path, const std::string& data)
{
    if (data.size() > max_file_size)
	{
        return false;
    }

    std::string safe_path;
    if (!constructSafePath(file_path, safe_path))
	{
        return false;
    }

    size_t pos = safe_path.find_last_of('/');
    if (pos != std::string::npos)
	{
        std::string dir_path = safe_path.substr(0, pos);
        struct stat dir_stat;
        if (stat(dir_path.c_str(), &dir_stat) != 0)
		{
            if (mkdir(dir_path.c_str(), 0755) != 0 && errno != EEXIST)
			{
                return false;
            }
        }
    }

    std::ofstream file(safe_path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file.is_open())
	{
        return false;
    }

    file.write(data.c_str(), data.size());
    if (!file.good())
	{
        file.close();
        return false;
    }

    file.close();
    return true;
}

std::string extractFileName(const std::string& uri) {
    size_t pos = uri.find_last_of('/');
    if (pos != std::string::npos)
        return uri.substr(pos + 1);
    return uri;
}

bool isDirectory(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
        return false; // Error accessing path
    return (info.st_mode & S_IFDIR) != 0;
}

bool fileExists(const std::string& path) {
    struct stat info;
    return stat(path.c_str(), &info) == 0;
}

std::string readFile(const std::string& path) {
    std::ifstream file(path.c_str(), std::ios::binary);
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

bool saveFile(const std::string& path, const std::string& content) {
    std::ofstream file(path.c_str(), std::ios::binary);
    if (!file.is_open())
        return false;
    file << content;
    return true;
}

std::string generateDirectoryListing(const std::string& dirPath, const std::string& uriPath) {
    DIR* dir = opendir(dirPath.c_str());
    if (!dir)
        return "Unable to open directory.";

    std::ostringstream ss;
    ss << "<html><body><h1>Index of " << uriPath << "</h1><ul>";
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string name = entry->d_name;
        if (name == "." || name == "..")
            continue;
        ss << "<li><a href=\"" << uriPath << "/" << name << "\">" << name << "</a></li>";
    }
    ss << "</ul></body></html>";
    closedir(dir);
    return ss.str();
}


std::string extractBoundary(const std::map<std::string, std::string>& headers) {
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

bool deleteFile(const std::string& file_path) {
    if (remove(file_path.c_str()) != 0) {
        std::cerr << "Fehler: Datei konnte nicht gelöscht werden: " << file_path << std::endl;
        return false;
    }
    return true;
}
