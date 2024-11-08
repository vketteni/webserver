
#include "Logger.hpp"

// Default constructor
Logger::Logger(const std::string& accessLogPath, const std::string& errorLogPath, LogLevel level) : currentLog(level)
{
	struct stat info;

	if(stat("log", &info) != 0) {
		mkdir("log", 0777);
	}
	 accessLogFile.open(("log/" + accessLogPath).c_str(), std::ios::app);
	 errorLogFile.open(("log/" + errorLogPath).c_str(), std::ios::app);
}

// Default destructor
Logger::~Logger()
{
	if (accessLogFile.is_open()) accessLogFile.close();
	if (errorLogFile.is_open()) errorLogFile.close();
}

void Logger::logError(int statusCode, const std::string &errorMessage)
{
	rotateLogs(errorLogFile, errorLogPath);
	logMessage(statusCode, errorMessage, LERROR, errorLogFile);
}

void Logger::logDebug(int statusCode, const std::string &debugMessage)
{
	rotateLogs(accessLogFile, accessLogPath);
	logMessage(statusCode, debugMessage, DEBUG, accessLogFile);
}

void Logger::logWarning(int statusCode, const std::string &warningMessage)
{
	rotateLogs(errorLogFile, errorLogPath);
	logMessage(statusCode, warningMessage, WARNING, errorLogFile);
}

void Logger::logInfo(int statusCode, const std::string &infoMessage)
{
	rotateLogs(accessLogFile, accessLogPath);
	logMessage(statusCode, infoMessage, INFO, accessLogFile);
}

std::string Logger::getCurrentTime()
{
	time_t	now = time(0);
	char	buffer[80];
	struct tm *time;
	time = localtime(&now);
	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", time);
	return std::string(buffer);
}

void Logger::logMessage(int statusCode, const std::string &message, LogLevel level, std::ofstream &logFile)
{
	if (level >= currentLog) {
		logFile << "[" << getCurrentTime() << "]";
		switch (level) {
			case DEBUG: logFile << "[DEBUG] "; break;
			case INFO: logFile << "[INFO] "; break;
			case WARNING: logFile << "[WARNING] "; break;
			case LERROR: logFile << "[ERROR] "; break;
		}
		logFile << "<" << statusCode << "> " << message << std::endl;
	}
}


size_t Logger::getFileSize(const std::string &filePath)
{
	struct stat	statBuf;
	int	rc = stat(filePath.c_str(), &statBuf);
	return rc == 0 ? statBuf.st_size : 0;
}

void Logger::rotateLogs(std::ofstream &logFile, const std::string &logFilePath)
{
	maxLogFileSize = 50 * 1024 * 1024;
	std::string fullPath = "log/" + logFilePath;
	if (getFileSize(fullPath) >= maxLogFileSize) {
		logFile.close();

		std::string newLogFileName = fullPath + "." + getCurrentTime();
		std::replace(newLogFileName.begin(), newLogFileName.end(), ' ', '_');
		std::replace(newLogFileName.begin(), newLogFileName.end(), ':', '-');

		rename(fullPath.c_str(), newLogFileName.c_str());
		logFile.open(fullPath.c_str(), std::ios::app);
	}
}
