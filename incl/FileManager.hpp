#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <dirent.h>
#include <map>
#include <vector>
#include <iostream>

class FileManager {
	public:
		FileManager(const std::string& baseDirectory);
		
		bool readFile(const std::string& file_path, std::string& out_data);
		bool writeFile(const std::string& file_path, const std::string& data);
		void setMaxFileSize(size_t maxSize);

	private:
		std::string base_directory;
		size_t max_file_size;
		
		bool constructSafePath(const std::string& file_path, std::string& outsafe_path);
		FileManager(const FileManager&);
};

std::string extractFileName(const std::string& uri);
bool isDirectory(const std::string& path);
bool fileExists(const std::string& path);
std::string readFile(const std::string& path);
std::string extractFileName(const std::string& uri);
bool saveFile(const std::string& path, const std::string& content);
std::string generateDirectoryListing(const std::string& dirPath, const std::string& uriPath);
std::string extractBoundary(const std::map<std::string, std::string>& headers);
bool parseMultipartData(const std::string& body, const std::string& boundary, std::string& out_filename, std::vector<char>& out_filecontent);
bool writeFileDirectly(const std::string& directory, const std::string& filename, const std::vector<char>& file_data);


#endif 
