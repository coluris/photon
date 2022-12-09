#pragma once
#include "output_manager.h"

class InternalManager : public OutputManager
{
    int refresh_rate;
    bool alive;

public:
    InternalManager(int refresh_rate);
    ~InternalManager();
    void begin();
    void close();
    void pushData();
    bool hasBegun();
    bool setRouting(int internalUniverse, int outputUniverse);
    int getRouting(int internalUniverse);
};