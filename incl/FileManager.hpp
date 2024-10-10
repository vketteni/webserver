#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <stdexcept>

class FileManager {
public:
    std::string root_directory;
    std::string requested_file;

    // Konstruktor

    // Überprüft, ob die angeforderte Ressource existiert
    bool resourceExists(const std::string& path) const;

    // Überprüft, ob der Server die Datei lesen darf
    bool canReadFile(const std::string& path) const;

    // Lädt den Inhalt der Ressource (z.B. HTML-Datei)
    std::string loadResourceContent(const std::string& path) const;

    // Bestimmt den MIME-Typ basierend auf der Dateiendung
    std::string getMimeType(const std::string& path) const;
};

#endif // FILEMANAGER_HPP
