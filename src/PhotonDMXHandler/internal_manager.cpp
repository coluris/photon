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
    this->alive = true;
}
void InternalManager::close()
{
    this->alive = false;
}
void InternalManager::pushData()
{
    if(this->alive)
    {
        int universe = 0;
        for (Universe *u : UniverseManager::getUniverses())
        {
            sendMessage("pkt[" + std::to_string(universe) + "]" + u->getDataAsString());
            universe++;
        }
    }
}
bool InternalManager::hasBegun()
{
    return this->alive;
}

bool InternalManager::setRouting(int internalUniverse, int outputUniverse)
{
    return true;
}
int InternalManager::getRouting(int internalUniverse)
{
    return 0;
}