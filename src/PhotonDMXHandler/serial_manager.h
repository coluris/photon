#pragma once
#include <string>
#include <map>
#include <thread>
#include <vector>
#include <boost/asio.hpp>
#include "output_manager.h"

class SerialService
{
    boost::asio::io_service io;
    boost::asio::serial_port ser_port;

public:
    SerialService(std::string port);
    ~SerialService();
    boost::asio::serial_port *getSerialService();
};

class SerialManager : public OutputManager
{
    int refresh_rate;
    std::thread refresh_thread;
    std::string port_name;
    SerialService *ser_serv;
    bool alive;

public:
    SerialManager(std::string port, int refresh_rate);
    ~SerialManager();
    bool setRouting(int internalUniverse, int outputUniverse);
    int getRouting(int internalUniverse);
    void begin();
    void close();
    void pushData();
    bool hasBegun();

private:
    std::map<int, int> routing;
};