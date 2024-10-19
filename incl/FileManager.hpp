#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>

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
		FileManager& operator=(const FileManager&);
};

#endif 
