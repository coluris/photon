#include <string>
#include <vector>
#include <map>
#include "fixture.h"
#include "cue.h"
#include "cuelist.h"
#include "show.h"
#include <fstream>
#include <json/json.hpp>
#include "logger.h"
using json = nlohmann::json;

std::string Show::fileName;
std::map<std::string, Fixture *> Show::fixMap;
std::map<std::string, Cuelist *> Show::cuelists;
std::vector<Fixture *> Show::fix_list;
float Show::liveTime = 2.0;

void Show::loadCuelists(json cuelists)
{
    for (auto entry : cuelists.items())
    {
        std::string id = entry.value()["id"].get<std::string>();
        Cuelist *cuelist = new Cuelist();
        Show::addCuelist(cuelist, id);
        for (auto cue_iter : entry.value()["cuelist"].items())
        {
            float transition = cue_iter.value()["transition"].get<float>();
            float trigger_delay = cue_iter.value()["trigger_delay"].get<float>();
            Cue::TRIGGER trigger = Cue::getTriggerByName(cue_iter.value()["trigger"].get<std::string>());
            Cue *cue = new Cue(trigger, transition, trigger_delay);
            for (auto values : cue_iter.value()["values"].items())
            {
                std::string fix_name = values.key();
                Fixture *fix = Show::getFixtureById(fix_name);
                for (auto param : values.value().items())
                {
                    cue->setValue(fix, param.key(), param.value().get<int>());
                }
            }
            for (auto effects : cue_iter.value()["effects"].items())
            {
                std::string type = effects.value()["type"].get<std::string>();
                float tempo = effects.value()["tempo"].get<float>();
                std::string shape = effects.value()["shape"].get<std::string>();
                if (shape == "custom")
                {
                    // DO custom shape logic
                }
                float size = effects.value()["size"].get<float>();
                int multiplier = effects.value()["multiplier"].get<int>();
                float offset = effects.value()["offset"].get<float>();
                float phase = effects.value()["phase"].get<float>();
                float direction = effects.value()["direction"].get<float>();
                std::vector<Fixture *> fix_list;
                for (auto sub_fix : effects.value()["fixtures"].items())
                {
                    std::string fix_id = sub_fix.value().get<std::string>();
                    Fixture *f = Show::getFixtureById(fix_id);
                    fix_list.push_back(f);
                }
                Effect *e = new Effect(fix_list, type);
                e->updateParam("size", size);
                e->updateParam("offset", offset);
                e->updateParam("phase", phase);
                e->updateParam("direction", direction);
                e->updateParam("bpm", tempo);
                e->updateShape(Effect::getShapeFromName(shape));
                cue->addEffect(e);
            }
            cuelist->appendCopy(cue);
        }
    }
}
void Show::loadFixtures(json fixtures)
{
    for (auto entry : fixtures.items())
    {
        int channel = entry.value()["channel"].get<int>();
        int universe = entry.value()["universe"].get<int>();
        std::string fixType = entry.value()["type"].get<std::string>();
        std::string id = entry.key();
        Fixture *f = new Fixture(channel, universe);
        fix_list.push_back(f);
        f->setFileName(fixType);
        fixMap[id] = f;
    }
}
void Show::loadShowFromFile(std::string fileName)
{
    Logger::log("Loading show from " + fileName, Logger::INFO);
    std::fstream showFile((fileName).c_str());
    json showData = json::parse(showFile);
    loadFixtures(showData["fixtures"]);
    loadCuelists(showData["cuelists"]);
    showFile.close();
}

std::vector<Fixture *> Show::getFixtureList()
{
    return fix_list;
}

Cuelist *Show::getCuelist(std::string id)
{
    return cuelists[id];
}

std::vector<Cuelist *> Show::getCuelists()
{
    std::vector<Cuelist *> vec_cuelist;
    for (auto kvp : cuelists)
    {
        vec_cuelist.push_back(kvp.second);
    }
    return vec_cuelist;
}

void Show::addCuelist(Cuelist *cuelist, std::string id)
{
    cuelists[id] = cuelist;
}

Fixture *Show::getFixtureById(std::string id)
{
    return fixMap[id];
}

void Show::saveShowToFile(std::string showfile = fileName)
{
    std::fstream saveFile((showfile).c_str());
    saveFile.close();
}