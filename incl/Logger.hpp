#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <sys/stat.h>
#include <algorithm>
#include "Response.hpp"

enum LogLevel {
	DEBUG,
	INFO,
	WARNING,
	LERROR
} ;



// Class declaration
class Logger {

	private:

		LogLevel		currentLog;
		std::string		accessLogPath;
		std::string		errorLogPath;
		size_t			maxLogFileSize;

		void			logMessage(int statusCode, const std::string &message, LogLevel level, std::ofstream &logFile);
		std::string		getCurrentTime();
		size_t			getFileSize(const std::string& filePath);

	public:
		std::ofstream	accessLogFile;
		std::ofstream	errorLogFile;
		Logger(const std::string& accessLogPath, const std::string& errorLogPath, LogLevel level);
		~Logger();

		void			rotateLogs(std::ofstream &logFile, const std::string &logFilePath);
		void			logError(int statusCode, const std::string& errorMessage);
		void			logDebug(int statusCode, const std::string& debugMessage);
		void			logWarning(int statusCode, const std::string& warningMessage);
		void			logInfo(int statusCode, const std::string& infoMessage);

};


#endif // LOGGER_HPP_

