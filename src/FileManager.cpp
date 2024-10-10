#include "FileManager.hpp"

// Überprüft, ob die Ressource existiert
bool FileManager::resourceExists(const std::string& path) const {
    std::ifstream file(path.c_str());
    return file.good();
}

// Überprüft, ob der Server die Datei lesen darf
bool FileManager::canReadFile(const std::string& path) const {
    // Überprüfen, ob die Datei existiert und ob der Server Leserechte hat
    return (access(path.c_str(), R_OK) == 0);
}

// Lädt den Inhalt der Ressource
std::string FileManager::loadResourceContent(const std::string& path) const {
    if (!canReadFile(path)) {
        throw std::runtime_error("Permission denied: " + path);
    }

    std::ifstream file(path.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::ostringstream contents;
    contents << file.rdbuf();  // Dateiinhalt in den Stream lesen
    return contents.str();
}

// Bestimmt den MIME-Typ basierend auf der Dateiendung
std::string FileManager::getMimeType(const std::string& path) const {
    std::size_t extPos = path.rfind('.');
    if (extPos == std::string::npos) {
        return "application/octet-stream";  // Standardtyp, wenn die Dateiendung unbekannt ist
    }

    std::string ext = path.substr(extPos);
    if (ext == ".html" || ext == ".htm") return "text/html";
    if (ext == ".txt") return "text/plain";
    if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg";
    if (ext == ".png") return "image/png";
    if (ext == ".css") return "text/css";
    if (ext == ".js") return "application/javascript";
    // Füge weitere MIME-Typen nach Bedarf hinzu
    return "application/octet-stream";
}
