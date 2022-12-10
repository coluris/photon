#define _USE_MATH_DEFINES

#include "serial_manager.h"
#include "logger.h"
#include <string>
#include <boost/asio/serial_port.hpp>
#include "universe.h"
#include <chrono>
#include <cmath>

SerialManager::SerialManager(std::string port, int refresh_rate)
{
    this->ser_serv = new SerialService(port);
    this->port_name = port;
    this->refresh_rate = refresh_rate;
    this->alive = false;
}

void SerialManager::begin()
{
    Logger::log("Starting serial output on " + this->port_name, Logger::INFO);
    this->ser_serv->getSerialService()->set_option(boost::asio::serial_port_base::baud_rate(3000000));
    this->ser_serv->getSerialService()->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    this->ser_serv->getSerialService()->set_option(boost::asio::serial_port_base::character_size(8));
    this->ser_serv->getSerialService()->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));

    this->alive = true;
}

SerialManager::~SerialManager()
{
}

bool SerialManager::setRouting(int internalUniverse, int outputUniverse)
{
    this->routing.insert(std::pair<int, int>(internalUniverse, outputUniverse));
    return this->routing.at(internalUniverse) == outputUniverse;
}

int SerialManager::getRouting(int internalUniverse)
{
    return this->routing.at(internalUniverse);
}

void SerialManager::close()
{
    this->ser_serv->getSerialService()->close();
    Logger::log("Stopping serial output port on " + this->port_name, Logger::INFO);
}

void SerialManager::pushData()
{

    for (auto kv : this->routing)
    {
        Universe *u = UniverseManager::getUniverses()[kv.first];
        uint8_t packet[518];
        packet[0] = 0x7E;
        packet[2] = (uint8_t)((513) & 0xFF);
        packet[3] = (uint8_t)((512 >> 8) & 0xFF);
        packet[4] = 0x00;
        packet[517] = 0xE7;

        packet[1] = (uint8_t)(100 + kv.second);

        uint8_t *data = u->getData();
        for (int i = 0; i < 512; i++)
        {
            packet[5 + i] = data[i];
        }
        boost::asio::write(*this->ser_serv->getSerialService(), boost::asio::buffer(packet, size_t(518)));
    }
}

bool SerialManager::hasBegun()
{
    return this->alive;
}

SerialService::SerialService(std::string port) : io(), ser_port(io, port)
{
    Logger::log("Initializing serial output on " + port, Logger::INFO);
}

SerialService::~SerialService()
{
}

boost::asio::serial_port *SerialService::getSerialService()
{
    return &this->ser_port;
}