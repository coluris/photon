#include "fixture.h"
#include <fstream>
#include <json/json.hpp>
#include "logger.h"
#include "universe.h"
#include <filesystem>
using json = nlohmann::json;

std::map<Fixture::ATTR_TYPES, std::string> Fixture::attr_type_names =
    {{Fixture::INTENSITY, "in"},
     {Fixture::RED, "r"},
     {Fixture::GREEN, "g"},
     {Fixture::BLUE, "b"},
     {Fixture::AMBER, "a"},
     {Fixture::WHITE, "w"},
     {Fixture::PAN, "pan"},
     {Fixture::TILT, "tilt"},
     {Fixture::GOBO_INDEX, "gob-i"},
     {Fixture::GOBO_SPIN, "gob-s"},
     {Fixture::COLOR_INDEX, "col-i"},
     {Fixture::COLOR_SPIN, "col-s"}};

Fixture::Fixture(std::string id, int channel, int universe)
{
    this->id = id;
    this->startChannel = channel - 1;
    this->fileName = "";
    this->u = UniverseManager::getUniverses()[universe - 1];
}

Fixture::Fixture(std::string fileName, std::string id, int channel, int universe)
{
    this->id = id;
    this->startChannel = channel - 1;
    this->fileName = fileName;
    this->u = UniverseManager::getUniverses()[universe - 1];
    loadFixtureFromFile();
}

Fixture::~Fixture()
{
    delete u;
}

void Fixture::loadFixtureFromFile()
{
    auto path = std::filesystem::canonical("./Fixtures/");
    std::fstream fixFile((path.string() + "/" + fileName + ".phix").c_str());
    json fixData = json::parse(fixFile);
    for (auto entry : fixData["channels"].items())
    {
        std::string attr = entry.key();
        std::string val = entry.value().dump();
        this->setChannelMode(std::stoi(val), entry.key());
        this->attributes[entry.key()] = 0;
    }

    outputToUniverse();
}

std::map<std::string, uint16_t> Fixture::getAttributeMap()
{
    return this->attributes;
}

void Fixture::detach()
{
    this->fileName = "";
    this->channels.clear();
}

bool Fixture::setChannelMode(int channel, std::string mode)
{
    this->channels[mode] = channel;
    return this->channels[mode] == channel;
}

bool Fixture::setStartChannel(int startChannel)
{
    if (startChannel < 1)
    {
        Logger::log("Must specify a channel between 1-512!", Logger::ERROR);
        this->startChannel = 0;
        return this->startChannel == 0;
    }
    if (startChannel > 512)
    {
        Logger::log("Must specify a channel between 1-512!", Logger::ERROR);
        this->startChannel = 511;
        return this->startChannel == 511;
    }
    this->startChannel = startChannel - 1;
    return this->startChannel == startChannel - 1;
}

bool Fixture::setUniverse(int universe)
{
    this->u = UniverseManager::getUniverses()[universe - 1];
    return this->u == UniverseManager::getUniverses()[universe - 1];
}

bool Fixture::setAttribute(std::string attr, uint16_t val)
{
    this->attributes[attr] = val;
    outputToUniverse();
    return this->attributes[attr] == val;
}

bool Fixture::setFileName(std::string fileName)
{
    this->fileName = fileName;
    loadFixtureFromFile();
    return this->fileName == fileName;
}

int Fixture::getStartChannel()
{
    return this->startChannel;
}

void Fixture::outputToUniverse()
{
    if (this->fileName != "")
    {
        for (auto kvp : this->attributes)
        {
            std::string attr = kvp.first;
            uint16_t val = kvp.second;

            if (attr.find("_") == 0)
            {
                uint8_t LSB = val & 0xFF;
                uint8_t MSB = (val >> 8) & 0xFF;
                this->u->setChannelValue(this->channels[attr] - 1 + startChannel, MSB);
                this->u->setChannelValue(this->channels[attr] + startChannel, LSB);
            }
            else
            {
                this->u->setChannelValue(this->channels[attr] - 1 + startChannel, val);
            }
        }
    }
}

std::string Fixture::getID()
{
    return this->id;
}

std::string Fixture::getFileName()
{
    return this->fileName;
}

Universe *Fixture::getUniverse()
{
    return this->u;
}