#pragma once

#include <map>
#include <string>
#include <vector>
#include "universe.h"
class Fixture
{

public:
    Fixture(int channel, int universe);
    Fixture(std::string fileName, int channel, int universe);
    ~Fixture();
    void loadFixtureFromFile();
    bool setAttribute(std::string attr, uint16_t val);
    void outputToUniverse();
    bool setFileName(std::string fileName);
    bool setUniverse(int universe);
    bool setStartChannel(int startChannel);
    int getStartChannel();
    bool setChannelMode(int channel, std::string mode);
    std::map<std::string, uint16_t> getAttributeMap();
    void detach();
    enum ATTR_TYPES
    {
        INTENSITY,
        RED,
        GREEN,
        BLUE,
        AMBER,
        WHITE,
        PAN,
        TILT,
        GOBO_INDEX,
        GOBO_SPIN,
        COLOR_INDEX,
        COLOR_SPIN,
    };

private:
    std::map<std::string, uint16_t> attributes;
    std::map<std::string, int> channels;
    static std::map<ATTR_TYPES, std::string> attr_type_names;
    std::string id;
    std::string fileName;
    int startChannel;
    Universe *u;
};