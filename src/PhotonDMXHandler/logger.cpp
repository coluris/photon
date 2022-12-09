#include "logger.h"
#include <fstream>
#include <string.h>
#include <chrono>
#include <ctime>
#include "util.h"

std::string Logger::UUID = generateUUID();
std::string Logger::fileName;
std::fstream Logger::logFile;

void Logger::init()
{
    std::system("mkdir logs");
    std::time_t logTime = std::time(nullptr);
    char timeString[sizeof("Jan011970-000000") / sizeof(char)];
    std::strftime(timeString, sizeof(timeString), "%b%d%Y-%H%M%S", std::localtime(&logTime));
    fileName = std::string(timeString) + "-" + UUID;
    log("Logger initialized with UUID " + UUID, LOG_TYPE::INFO);
}
std::string Logger::formatMessage(std::string message, LOG_TYPE logType)
{
    std::time_t logTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string status = "";

    switch (logType)
    {
    case LOG_TYPE::INFO:
        status = "INFO";
        break;
    case LOG_TYPE::ERROR:
        status = "ERROR";
        break;
    case LOG_TYPE::DEBUG:
        status = "DEBUG";
        break;
    case LOG_TYPE::WARN:
        status = "WARN";
        break;
    case LOG_TYPE::FATAL:
        status = "FATAL";
        break;
    }
    std::string f_message = std::string(strtok(std::ctime(&logTime), "\n")) + " [" + status + "]  " + std::string(message);
    return f_message;
}

int Logger::logFatalError(std::string message)
{
    logFile.open("logs/" + fileName + ".log", std::fstream::app);
    std::string f_message = formatMessage(message, LOG_TYPE::FATAL);
    logFile << f_message + "\n";
    logFile.close();
    return 0;
}

int Logger::log(std::string message, Logger::LOG_TYPE logType)
{

    logFile.open("logs/" + fileName + ".log", std::fstream::app);
    std::string f_message = formatMessage(message, logType);
    logFile << f_message + "\n";
    logFile.close();
    printLog(f_message.c_str(), logType);
    return 0;
}

std::string Logger::getUUID()
{
    return UUID;
}