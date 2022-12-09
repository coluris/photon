#pragma once
#include <iostream>
#include <random>
#include <ctime>
#include "logger.h"
#include <iterator>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

inline int printLog(std::string message, Logger::LOG_TYPE logType = Logger::INFO)
{
    std::string color;
    switch (logType)
    {
    case Logger::ERROR:
        std::cerr << "\e[1;31m" << message << "\e[0m" << std::endl;
        return 0;

    case Logger::INFO:
        color = "\e[1;36m";
        break;

    case Logger::DEBUG:
        color = "\e[1;32m";
        break;

    case Logger::WARN:
        color = "\e[1;33m";
        break;
    }
    std::cout << color << message << "\e[0m" << std::endl;
    return 0;
}

inline int sendMessage(std::string message)
{
    std::cout << "@p!" << message << ";" << std::endl;
    return 0;
}

inline std::string generateUUID()
{
    static std::mt19937 rng(std::time(NULL));

    std::uniform_int_distribution<int> dist(0, 15);

    const char *v = "0123456789abcdef";
    const bool dash[] = {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0};

    std::string output;
    for (int i = 0; i < 16; i++)
    {
        if (dash[i])
            output += "-";
        output += v[dist(rng)];
        output += v[dist(rng)];
    }
    return output;
}

inline int throwFatalError(std::string message, int exitCode)
{
    std::cerr << "\e[1;31m" << Logger::formatMessage(message, Logger::FATAL) << "\e[0m" << std::endl;
    Logger::logFatalError(message);
    Logger::logFatalError("Exited with error code " + std::to_string(exitCode));
    exit(exitCode);
    return exitCode;
}

template <typename Out>
inline void split(const std::string &s, char delim, Out result)
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim))
    {
        *result++ = item;
    }
}

inline std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}