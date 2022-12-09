#pragma once
#include <map>
class OutputManager
{
    int refresh_rate;
    std::map<int, int> routing;
    bool alive;

public:
    virtual void begin() = 0;
    virtual void close() = 0;
    virtual void pushData() = 0;
    virtual bool setRouting(int internalUniverse, int outputUniverse) = 0;
    virtual int getRouting(int internalUniverse) = 0;
    virtual bool hasBegun() = 0;
};