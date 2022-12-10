#pragma once
#include <string>
#include <cstdint>
#include <vector>
#include <thread>
#include "artnet/artnet.h"
#include <map>
#include "output_manager.h"
class Universe
{

public:
    Universe();
    ~Universe();
    uint8_t *getData();
    std::string getDataAsString();
    uint8_t getChannelValue(short channel);
    bool setChannelValue(short channel, uint8_t value);
    bool setUniverse(uint8_t *values);
    std::string getUUID();

private:
    uint8_t *data;
    std::string UUID;
};

class UniverseManager
{
    static std::vector<Universe *> universes;
    static std::map<Universe *, int> internalUniv;
    static std::thread tickThread;
    static int refresh_rate;
    static int tick;
    static void updateTick();
    static void processNextFrame();
    static std::vector<OutputManager *> outputs_vec;
    static std::map<std::string, OutputManager *> output_ids;

public:
    static void init(int refresh_rate, std::vector<OutputManager *> outputs);
    static std::vector<Universe *> getUniverses();
    static int addUniverse(Universe *universe);
    static bool removeUniverse(Universe *universe);
    static int getUniverseNum(Universe *);
    static int getTick();
    static int getRefreshRate();
    static void processEffects();
    static void processTransitions();
    static void attachOutput(OutputManager *out, std::string id);
    static OutputManager *getOutputManager(std::string id);
};