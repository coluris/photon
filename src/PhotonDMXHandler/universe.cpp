#define _USE_MATH_DEFINES

#include "universe.h"
#include "util.h"
#include <cstring>
#include "logger.h"
#include <thread>
#include <chrono>
#include <ctime>
#include <string>
#include "artnet/artnet.h"
#include <cmath>
#include <cstdlib>
#include <map>
#include "artnet_manager.h"
#include "output_manager.h"
#include "effect.h"
#include "show.h"
#include "cue.h"
#include "transition.h"
#include "cuelist.h"
#undef ERROR

Universe::Universe()
{
    this->data = new uint8_t[512];
    this->UUID = generateUUID();
    memset(this->data, 0, 512);
}
Universe::~Universe()
{
    delete this->data;
}
uint8_t *Universe::getData()
{
    return this->data;
}
uint8_t Universe::getChannelValue(short channel)
{
    if (channel < 0 || channel > 511)
    {
        Logger::log("Trying to use channel " + std::to_string(channel), Logger::ERROR);
        Logger::log("Must specify a channel between 0-511! Using channel 0.", Logger::ERROR);
        return this->data[0];
    }
    return this->data[channel];
}
bool Universe::setChannelValue(short channel, uint8_t value)
{
    if (channel < 0 || channel > 511)
    {
        Logger::log("Trying to use channel " + std::to_string(channel), Logger::ERROR);
        Logger::log("Must specify a channel between 0-511! Using channel 0.", Logger::ERROR);
        return false;
    }
    this->data[channel] = value;
    return this->data[channel] == value;
}
bool Universe::setUniverse(uint8_t *values)
{
    this->data = values;
    return this->data == values;
}

std::string Universe::getDataAsString()
{
    std::string output = "";
    for (int i = 0; i < 512; i++)
    {
        output += std::to_string(this->getData()[i]) + ',';
    }
    return output;
}

// #########################################    UNIVERSE MANAGER    ####################################################
std::vector<Universe *> UniverseManager::universes;
std::map<Universe *, int> UniverseManager::internalUniv;
std::thread UniverseManager::tickThread;
int UniverseManager::tick;
int UniverseManager::refresh_rate;
std::vector<OutputManager *> UniverseManager::outputs_vec;
std::map<std::string, OutputManager *> UniverseManager::output_ids;
void UniverseManager::init(int refresh, std::vector<OutputManager *> outputs)
{
    outputs_vec = outputs;
    refresh_rate = refresh;
    for (auto out : outputs_vec)
    {
        if (!out->hasBegun())
            out->begin();
    }
    Logger::log("Initializing universe manager at a rate of " + std::to_string(refresh) + "Hz", Logger::INFO);
    tickThread = std::thread(&updateTick);
    tickThread.detach();
}

std::vector<Universe *> UniverseManager::getUniverses()
{
    return universes;
}

int UniverseManager::addUniverse(Universe *universe)
{

    universes.push_back(universe);
    internalUniv.insert(std::pair(universe, universes.size() - 1));
    for (auto out : outputs_vec)
    {
        if (!out->hasBegun())
            out->begin();
    }

    return sizeof(universes) / sizeof(universe);
}

bool UniverseManager::removeUniverse(Universe *universe)
{
    for (Universe *u : universes)
    {
        auto i = &u - &universes[0];
        if (universes[i] == universe)
        {
            universes.erase(universes.begin() + i);
            return true;
        }
    }
    return false;
}
int UniverseManager::getUniverseNum(Universe *universe)
{
    return internalUniv.at(universe);
}

void UniverseManager::updateTick()
{
    int refresh_ms = 1000 / refresh_rate;
    std::chrono::high_resolution_clock::time_point target_time = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(refresh_ms);
    while (true)
    {
        for (auto out : outputs_vec)
        {
            out->pushData();
        }
        processNextFrame();
        std::this_thread::sleep_until(target_time);
        target_time += std::chrono::milliseconds(refresh_ms);
    }
}

void UniverseManager::processEffects()
{
    for (Cuelist *cuelist : Show::getCuelists())
    {
        if (cuelist->isActive())
        {
            for (Effect *effect : cuelist->getActiveCue()->getEffects())
            {
                std::thread effectThread([effect]()
                                         { effect->processEffect(); });
                effectThread.detach();
            }
        }
    }
}

void UniverseManager::processTransitions()
{
    for (Cuelist *cuelist : Show::getCuelists())
    {
        if (cuelist->isActive())
        {
            Transition *transition = cuelist->getActiveTransition();
            if (transition != nullptr)
            {
                std::thread transitionThread([transition]()
                                             { transition->nextFrame(); });
                transitionThread.detach();
            }
        }
    }
}

void UniverseManager::processNextFrame()
{
    tick++;
    std::thread proc_effect(&UniverseManager::processEffects);
    std::thread proc_transitions(&UniverseManager::processTransitions);
    proc_effect.detach();
    proc_transitions.detach();
};

int UniverseManager::getTick()
{
    return tick;
}

int UniverseManager::getRefreshRate()
{
    return refresh_rate;
}

void UniverseManager::attachOutput(OutputManager *out, std::string id)
{
    outputs_vec.push_back(out);
    output_ids[id] = out;
    for (auto output : outputs_vec)
    {
        if (!output->hasBegun())
            output->begin();
    }
}

OutputManager *UniverseManager::getOutputManager(std::string id)
{
    return output_ids[id];
}