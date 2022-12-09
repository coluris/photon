#pragma once
#include <fstream>
#include <string>

#undef DEBUG
#undef ERROR

class Logger
{
public:
    static std::string UUID;
    static std::string fileName;
    static std::fstream logFile;

    enum LOG_TYPE
    {
        INFO,
        WARN,
        DEBUG,
        ERROR,
        FATAL
    };
    static void init();
    static std::string formatMessage(std::string message, LOG_TYPE logType);
    static int log(std::string message, LOG_TYPE logType);
    static int logFatalError(std::string message);

    static std::string getUUID();
};
