
#include "Logger.hpp"

// Default constructor
Logger::Logger(const std::string& accessLogPath, const std::string& errorLogPath, LogLevel level) : currentLog(level)
{
	 accessLogFile.open(accessLogPath.c_str(), std::ios::app);
	 errorLogFile.open(errorLogPath.c_str(), std::ios::app);
}

// Default destructor
Logger::~Logger()
{
	if (accessLogFile.is_open()) accessLogFile.close();
	if (errorLogFile.is_open()) errorLogFile.close();
}

void Logger::logRequest(const std::string &clientIp, const std::string &method, const std::string &path, int statusCode)
{
	(void)path;
	rotateLogs(accessLogFile, accessLogPath);
	std::stringstream ss;
	ss << "Client IP: " << clientIp << ", Method: " << method << ", Status Code: " << statusCode;
	std::string	message = ss.str();
	logMessage(message, INFO, accessLogFile);
}

void Logger::logError(const std::string &errorMessage)
{
	rotateLogs(errorLogFile, errorLogPath);
	logMessage("Error: " + errorMessage, LERROR, errorLogFile);
}

void Logger::logDebug(const std::string &debugMessage)
{
	rotateLogs(accessLogFile, accessLogPath);
	logMessage("Debug: " + debugMessage, DEBUG, accessLogFile);
}

void Logger::logWarning(const std::string &warningMessage)
{
	rotateLogs(errorLogFile, errorLogPath);
	logMessage("Warning: " + warningMessage, WARNING, errorLogFile);
}

void Logger::logInfo(const std::string &infoMessage)
{
	rotateLogs(accessLogFile, accessLogPath);
	logMessage("Info: " + infoMessage, INFO, accessLogFile);
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

void Logger::logMessage(const std::string &message, LogLevel level, std::ofstream &logFile)
{
	if (level >= currentLog) {
		logFile << "[" << getCurrentTime() << "]";
		switch (level) {
			case DEBUG: logFile << "[DEBUG] "; break;
			case INFO: logFile << "[INFO] "; break;
			case WARNING: logFile << "[WARNING] "; break;
			case LERROR: logFile << "ERROR] "; break;
		}
		logFile << message << std::endl;
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
	if (getFileSize(logFilePath) >= maxLogFileSize) {
		logFile.close();

		std::string newLogFileName = logFilePath + "." + getCurrentTime();
		std::replace(newLogFileName.begin(), newLogFileName.end(), ' ', '_');
		std::replace(newLogFileName.begin(), newLogFileName.end(), ':', '-');

		rename(logFilePath.c_str(), newLogFileName.c_str());
		logFile.open(logFilePath.c_str(), std::ios::app);
	}
}
