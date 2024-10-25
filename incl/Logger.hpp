#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <sys/stat.h>
#include <algorithm>

enum LogLevel {
	DEBUG,
	INFO,
	WARNING,
	LERROR
} ;


#ifndef LOGGER_HPP_
#define LOGGER_HPP_

// Class declaration
class Logger {

	private:

		LogLevel		currentLog;
		std::string		accessLogPath;
		std::string		errorLogPath;
		size_t			maxLogFileSize; //in bytes

		void			logMessage(int statusCode, const std::string &message, LogLevel level, std::ofstream &logFile);
		std::string		getCurrentTime();
		size_t			getFileSize(const std::string& filePath);

	public:
		std::ofstream	accessLogFile;
		std::ofstream	errorLogFile;
		Logger(const std::string& accessLogPath, const std::string& errorLogPath, LogLevel level);
		~Logger();

		void			rotateLogs(std::ofstream &logFile, const std::string &logFilePath);
		void			logRequest(const std::string& clientIp, const std::string& method, const std::string& path, int statusCode);
		void			logError(int statusCode, const std::string& errorMessage);
		void			logDebug(int statusCode, const std::string& debugMessage);
		void			logWarning(int statusCode, const std::string& warningMessage);
		void			logInfo(int statusCode, const std::string& infoMessage);

};


#endif // LOGGER_HPP_

