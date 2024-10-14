#include "../incl/FileManager.hpp"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

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

    if (realpath(full_path.c_str(), real_path) == NULL)
	{
        return false;
    }

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

    if (file_stat.st_size > max_file_size)
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
