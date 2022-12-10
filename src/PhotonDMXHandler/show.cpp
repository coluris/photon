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
std::map<std::string, std::vector<std::map<std::string, std::pair<float, float>>>> Show::layouts_map;
std::string Show::showName;
float Show::liveTime = 2.0;

void Show::loadCuelists(json cuelists)
{
    for (auto entry : cuelists.items())
    {
        std::string id = entry.value()["id"].get<std::string>();
        std::string cuelist_name = entry.key();
        Cuelist *cuelist = new Cuelist(cuelist_name);
        Show::addCuelist(cuelist, id);
        for (auto cue_iter : entry.value()["cuelist"].items())
        {
            float transition = cue_iter.value()["transition"].get<float>();
            std::string title = cue_iter.value()["title"].get<std::string>();
            float trigger_delay = cue_iter.value()["trigger_delay"].get<float>();
            Cue::TRIGGER trigger = Cue::getTriggerByName(cue_iter.value()["trigger"].get<std::string>());
            Cue *cue = new Cue(trigger, transition, trigger_delay);
            cue->setCueTitle(title);
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
                int multiplier = effects.value()["multiplier"].get<float>();
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
        Fixture *f = new Fixture(id, channel, universe);
        fix_list.push_back(f);
        if (fixType != "")
        {
            f->setFileName(fixType);
        }
        fixMap[id] = f;
    }
}
void Show::loadLayouts(json layouts)
{
    for (auto entry : layouts.items())
    {
        std::string layout_name = entry.key();
        std::vector<std::map<std::string, std::pair<float, float>>> layout;
        for (auto fix : entry.value().items())
        {
            std::string fix_name = fix.key();
            float xPos = fix.value()["x"].get<float>();
            float yPos = fix.value()["y"].get<float>();
            std::pair<float, float> position(xPos, yPos);
            std::map<std::string, std::pair<float, float>> fix_pos;
            fix_pos.insert(std::pair(fix_name, position));
            layout.push_back(fix_pos);
        }
        Show::layouts_map.insert(std::pair(layout_name, layout));
    }
}
void Show::loadShowFromFile(std::string fileName)
{
    Logger::log("Loading show from " + fileName, Logger::INFO);
    std::fstream showFile((fileName).c_str());
    json showData = json::parse(showFile);
    loadFixtures(showData["fixtures"]);
    loadCuelists(showData["cuelists"]);
    loadLayouts(showData["layouts"]);
    Show::showName = showData["name"].get<std::string>();
    Show::fileName = fileName;
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

json Show::serializeCuelists()
{
    json out_cuelists;
    for (auto cuelist : Show::cuelists)
    {
        std::string cuelist_id = cuelist.first;
        Cuelist *current_cuelist = cuelist.second;
        std::string cuelist_name = current_cuelist->getName();
        std::vector<Cue *> cue_vec = current_cuelist->getCuelist();

        json cuelists_cuelist;
        cuelists_cuelist["id"] = cuelist_id;
        std::vector<json> cuelistJSON_vec;
        for (auto cue : cue_vec)
        {

            std::string trigger = Cue::getNameByTrigger(cue->getCueTrigger());
            std::string title = cue->getCueTitle();
            float transition = cue->getTransitionLength();
            float trigger_delay = cue->getDelayLength();
            std::map<Fixture *, std::map<std::string, int>> values = cue->getFixtureVals();
            json cuelistJSON;
            cuelistJSON["title"] = title;
            cuelistJSON["trigger"] = trigger;
            cuelistJSON["transition"] = transition;
            cuelistJSON["trigger_delay"] = trigger_delay;
            json cuelists_cuelist_values;
            for (auto value : values)
            {
                std::string fix_id = value.first->getID();
                std::map<std::string, int> all_fix_vals = value.second;

                json cuelists_cuelist_values_fixtures;
                for (auto fix_val : all_fix_vals)
                {
                    cuelists_cuelist_values_fixtures[fix_val.first] = fix_val.second;
                }
                cuelists_cuelist_values[fix_id] = cuelists_cuelist_values_fixtures;
            }
            cuelistJSON["values"] = cuelists_cuelist_values;
            std::vector<Effect *> effects = cue->getEffects();
            std::vector<json> cuelists_cuelist_effects;

            for (auto effect : effects)
            {
                json effectsJSON;
                effectsJSON["type"] = effect->getEffectType();
                effectsJSON["tempo"] = effect->getParameter("bpm");
                effectsJSON["shape"] = Effect::getNameFromShape(effect->getEffectShape());
                effectsJSON["phase"] = effect->getParameter("phase");
                effectsJSON["size"] = effect->getParameter("size");
                effectsJSON["multiplier"] = effect->getParameter("multiplier");
                effectsJSON["offset"] = effect->getParameter("offset");
                effectsJSON["direction"] = effect->getParameter("direction");

                std::vector<Fixture *> effect_fixtures = effect->getFixtureList();
                std::vector<std::string> effect_fix_names;
                for (auto fix : effect_fixtures)
                {
                    effect_fix_names.push_back(fix->getID());
                }
                effectsJSON["fixtures"] = effect_fix_names;
                cuelists_cuelist_effects.push_back(effectsJSON);
            }
            cuelistJSON["effects"] = cuelists_cuelist_effects;
            cuelistJSON_vec.push_back(cuelistJSON);
        }
        cuelists_cuelist["cuelist"] = cuelistJSON_vec;
        out_cuelists[cuelist_name] = cuelists_cuelist;
    }
    return out_cuelists;
}

json Show::serializeFixtures()
{
    json out_fixtures;

    std::vector<Fixture *> show_fix_list = Show::getFixtureList();
    for (auto fixture : show_fix_list)
    {
        std::string id = fixture->getID();
        std::string type = fixture->getFileName();
        int channel = fixture->getStartChannel();
        int universe = UniverseManager::getUniverseNum(fixture->getUniverse());

        json fix_def;
        fix_def["type"] = type;
        fix_def["channel"] = channel + 1;
        fix_def["universe"] = universe + 1;
        out_fixtures[id] = fix_def;
    }
    return out_fixtures;
}

json Show::serializeLayouts()
{
    json out_layouts;
    for (auto layout : Show::layouts_map)
    {
        json sub_layout;
        for (auto fix : layout.second)
        {
            for (auto key : fix)
            {
                json pos_vals;
                pos_vals["x"] = key.second.first;
                pos_vals["y"] = key.second.second;
                sub_layout[key.first] = pos_vals;
            }
        }
        out_layouts[layout.first] = sub_layout;
    }
    return out_layouts;
}

void Show::saveShowToFile(std::string saveFile)
{
    if (saveFile == "")
    {
        saveFile = Show::fileName;
    }
    json show;
    show["name"] = Show::showName;
    show["fixtures"] = serializeFixtures();
    show["cuelists"] = serializeCuelists();
    show["layouts"] = serializeLayouts();
    std::ofstream showFile((saveFile).c_str(), std::ios::trunc);
    showFile << show;
    showFile.close();
}