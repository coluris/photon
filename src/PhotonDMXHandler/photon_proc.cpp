#include "universe.h"
#include "logger.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "artnet_manager.h"
#include "internal_manager.h"
#include "serial_manager.h"
#include "output_manager.h"
#include "show.h"
#include "util.h"
std::vector<OutputManager *> outputs;

constexpr unsigned int hash(const char *s, int off = 0)
{
    return !s[off] ? 5381 : (hash(s, off + 1) * 33) ^ s[off];
}
constexpr inline unsigned int operator"" _(char const *p, size_t) { return hash(p); }

void executeMessage(std::string command, std::string data)
{
    switch (hash(command.c_str()))
    {
    case "frate"_:
    {
        InternalManager *internal = new InternalManager(UniverseManager::getRefreshRate());
        UniverseManager::init(std::stoi(data), outputs);
        UniverseManager::attachOutput(internal, "internal");
        break;
    }
    case "uadd"_:
        for (int i = 0; i < std::stoi(data); i++)
        {
            Universe *uni = new Universe();
            UniverseManager::addUniverse(uni);
        }
        break;
    case "serial"_:
    {
        SerialManager *serial = new SerialManager(data, UniverseManager::getRefreshRate());
        UniverseManager::attachOutput(serial, data);
        break;
    }
    case "artnet"_:
    {
        ArtNetManager *artnet = new ArtNetManager(data, UniverseManager::getRefreshRate());
        UniverseManager::attachOutput(artnet, artnet->getIP());
        break;
    }
    case "uedit"_:
    {
        std::vector<std::string> tokens = split(data, '|');
        int univ_num = std::stoi(tokens.at(0));
        std::string out_type = tokens.at(1);
        break;
    }
    case "fedit"_:
    {
        std::vector<std::string> tokens = split(data, '|');
        std::vector<std::string> fixtures = split(tokens.at(0), ',');
        std::vector<Fixture *> fix_list;
        std::string attr = tokens.at(1);
        int val = std::stoi(tokens.at(2));
        for (auto fix : fixtures)
        {
            fix_list.push_back(Show::getFixtureById(fix));
        }
        Show::getCuelists()[0]->getActiveCue()->setValue(fix_list, attr, val);
        break;
    }
    case "route"_:
    {
        std::vector<std::string> tokens = split(data, '|');
        std::string output_id = tokens.at(0);
        int extern_univ = std::stoi(tokens.at(1));
        int intern_univ = std::stoi(tokens.at(2));
        UniverseManager::getOutputManager(output_id)->setRouting(extern_univ, intern_univ);
        break;
    }
    case "layedit"_:
    {
        std::vector<std::string> tokens = split(data, '|');
        std::string layout_name = tokens.at(0);
        std::string fix_id = tokens.at(1);
        float x = std::stof(tokens.at(2));
        float y = std::stof(tokens.at(3));
        Show::setFixturePosition(layout_name, fix_id, x, y);
        break;
    }
    case "ldshow"_:
        Show::loadShowFromFile(data);
        Show::getCuelists()[0]->goToFirst();
        break;
    case "fxedit"_:
    {
        std::vector<std::string> tokens = split(data, '|');
        std::string effect_uuid = tokens.at(0);
        std::string parameter = tokens.at(1);
        float param_value = std::stof(tokens.at(2));
        Effect *e = Effect::getEffectByUUID(effect_uuid);
        Logger::log(e->getUUID(), Logger::DEBUG);
        e->updateParam(parameter, param_value);
        break;
    }
    case "nextcue"_:
        Show::getCuelists()[0]->nextCue();
        break;
    case "save"_:
        Show::saveShowToFile();
        break;
    }
}

int main(int argc, char *argv[])
{
    using namespace std::chrono_literals;
    std::system("mkdir Fixtures");
    std::system("mkdir Shows");
    std::system("mkdir Effects");
    Logger::init();

    std::string msg;
    while (true)
    {
        std::getline(std::cin, msg);
        if (msg.substr(0, 3) == "@p!")
        {
            std::string str_parse = msg.substr(0, msg.length());
            size_t splitter = str_parse.find("~");
            std::string command = msg.substr(3, splitter - 3);
            std::string data = msg.substr(splitter + 1, msg.length() - splitter + 1);
            executeMessage(command, data);
        }
    }
    return 0;
}