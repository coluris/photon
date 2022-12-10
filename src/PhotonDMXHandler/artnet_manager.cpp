#include "artnet_manager.h"
#include <string>
#include "logger.h"
#include "artnet/artnet.h"
#include "universe.h"
#include <map>
#include <algorithm>
#include "util.h"
ArtNetManager::ArtNetManager(std::string ip, int refresh_rate)
{
    Logger::log("Setting up Art-Net server on " + ip + " with a refresh rate of " + std::to_string(refresh_rate) + "Hz", Logger::INFO);
    this->node = artnet_new(ip.c_str(), 0);
    this->refresh_rate = refresh_rate;
    this->ip = ip;
    this->alive = false;
}
void ArtNetManager::begin()
{
    artnet_set_node_type(this->node, ARTNET_RAW);
    artnet_set_port_type(this->node, 0, ARTNET_ENABLE_INPUT, ARTNET_PORT_DMX);

    artnet_start(this->node);
    Logger::log("Starting Art-Net server on " + ip, Logger::INFO);
    this->alive = true;
}
bool ArtNetManager::setRouting(int internalUniverse, int outputUniverse)
{
    this->routing.insert(std::pair(internalUniverse, outputUniverse));
    return this->routing.at(internalUniverse) == outputUniverse;
}
int ArtNetManager::getRouting(int internalUniverse)
{
    return this->routing.at(internalUniverse);
}
void ArtNetManager::pushData()
{
    for (const auto &kv : this->routing)
    {
        artnet_raw_send_dmx(node, kv.second, 512, UniverseManager::getUniverses().at(kv.first)->getData());
    }
}
void ArtNetManager::close()
{
    Logger::log("Stopping Art-Net Server on " + this->ip, Logger::INFO);
    artnet_stop(node);
}

bool ArtNetManager::hasBegun()
{
    return this->alive;
}

std::string ArtNetManager::getIP()
{
    return this->ip;
}