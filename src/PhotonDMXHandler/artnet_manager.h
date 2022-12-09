#pragma once

#include "artnet/artnet.h"
#include <string>
#include <map>
#include "output_manager.h"

class ArtNetManager : public OutputManager
{
    artnet_node node;
    int refresh_rate;
    std::string ip;
    bool alive;

public:
    ArtNetManager(std::string ip, int refresh_rate);
    bool setRouting(int internalUniverse, int outputUniverse);
    int getRouting(int internalUniverse);
    void begin();
    void close();
    void pushData();
    bool hasBegun();
    std::string getIP();

private:
    std::map<int, int> routing;
};