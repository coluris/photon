#include "internal_manager.h"
#include "universe.h"
#include "util.h"
InternalManager::InternalManager(int refresh_rate)
{
    this->alive = false;
    this->refresh_rate = refresh_rate;
}

InternalManager::~InternalManager()
{
}

void InternalManager::begin()
{
    alive = true;
}
void InternalManager::close()
{
}
void InternalManager::pushData()
{
    int universe = 0;
    for (Universe *u : UniverseManager::getUniverses())
    {
        sendMessage("pkt[" + std::to_string(universe) + "]" + u->getDataAsString());
        universe++;
    }
}
bool InternalManager::hasBegun()
{
    return alive;
}

bool InternalManager::setRouting(int internalUniverse, int outputUniverse)
{
    return true;
}
int InternalManager::getRouting(int internalUniverse)
{
    return 0;
}