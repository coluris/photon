#define _USE_MATH_DEFINES

#include "serial_manager.h"
#include "logger.h"
#include <string>
#include <boost/asio/serial_port.hpp>
#include "universe.h"
#include <chrono>
#include <thread>
#include <cmath>

SerialManager::SerialManager(std::string port, int refresh_rate) : io(), ser_port(io, port)
{
    Logger::log("Initializing serial output on " + port, Logger::INFO);
    this->port_name = port;
    this->refresh_rate = refresh_rate;
    this->alive = false;
    // TODO SERIAL OUTPUT MANAGEMENT, take a page from Art-Net Manager, logic is decently airtight
}

void SerialManager::begin()
{
    Logger::log("Starting serial output on " + this->port_name, Logger::INFO);
    ser_port.set_option(boost::asio::serial_port_base::baud_rate(3000000));
    ser_port.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    ser_port.set_option(boost::asio::serial_port_base::character_size(8));
    ser_port.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));

    int counter = 0;
    for (Universe *u : UniverseManager::getUniverses())
    {
        if (u->getOutputType() == Universe::OUTPUT_TYPE::SERIAL)
        {
            this->routing.insert(std::pair(UniverseManager::getUniverseNum(u), counter));
        }
    }
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
    this->refresh_thread = std::thread([] {});
    refresh_thread.join();
    this->ser_port.close();
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
        boost::asio::write(this->ser_port, boost::asio::buffer(packet, size_t(518)));
    }
}

bool SerialManager::hasBegun()
{
    return this->alive;
}