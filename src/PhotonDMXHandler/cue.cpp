#include "cue.h"
#include "effect.h"
#include "logger.h"
#include "show.h"
#include "fixture.h"
#include <algorithm>
#include <map>
#include <string>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include "transition.h"
#include <functional>
std::map<std::string, Cue::TRIGGER> Cue::triggerMap = {{"HALT", Cue::HALT}, {"FOLLOW", Cue::FOLLOW}};
std::map<Cue::TRIGGER, std::string> Cue::triggerNameMap = {{Cue::HALT, "HALT"}, {Cue::FOLLOW, "FOLLOW"}};

Cue::Cue(TRIGGER trigger, float transitionLength, float delayLength)
{
    this->trigger = trigger;
    this->transitionLength = transitionLength;
    this->delayLength = delayLength;
    for (Fixture *fix : Show::getFixtureList())
    {
        for (auto kvp : fix->getAttributeMap())
        {
            this->setValue(fix, kvp.first, 0);
        }
    }
}

void Cue::addEffect(Effect *effect)
{
    this->effects.push_back(effect);
}

void Cue::setCueTitle(std::string title)
{
    this->cue_title = title;
}

void Cue::removeEffect(Effect *effect)
{
    this->effects.erase(std::remove(this->effects.begin(), this->effects.end(), effect), this->effects.end());
}

void Cue::setValue(Fixture *fix, std::string attr, int val)
{
    this->fixtureVals[fix][attr] = val;
    fix->setAttribute(attr, val);
}

void Cue::setValue(std::vector<Fixture *> fixtures, std::string attr, int val)
{
    for (Fixture *fix : fixtures)
    {
        this->fixtureVals[fix][attr] = val;
        fix->setAttribute(attr, val);
    }
}

void Cue::activate()
{
    for (auto val : this->fixtureVals)
    {
        Fixture *f = val.first;
        std::map<std::string, int> attrs = val.second;
        for (auto a : attrs)
        {
            if (this->effects.size() == 0)
            {
                f->setAttribute(a.first, a.second);
            }
            else
            {
                bool shouldDefaultValueRun = true;
                std::vector<Effect *> matchAttribute;
                std::copy_if(this->effects.begin(), this->effects.end(), std::back_inserter(matchAttribute), [a](Effect *effect)
                             { return effect->getEffectType() == a.first; });
                for (Effect *e : matchAttribute)
                {
                    for (Fixture *fix : e->getFixtureList())
                    {
                        if (f->getID() == fix->getID())
                        {
                            shouldDefaultValueRun = false;
                        }
                    }
                }

                if (shouldDefaultValueRun)
                {
                    f->setAttribute(a.first, a.second);
                }
            }
        }
    }
}

std::vector<Effect *> Cue::getEffects()
{
    return this->effects;
}

Cue::TRIGGER Cue::getTriggerByName(std::string name)
{
    return triggerMap[name];
}

std::string Cue::getNameByTrigger(Cue::TRIGGER trigger)
{
    return triggerNameMap[trigger];
}

bool Cue::setTransitionLength(float transitionLength)
{
    this->transitionLength = transitionLength;
    return this->transitionLength == transitionLength;
}
float Cue::getTransitionLength()
{
    return this->transitionLength;
}
bool Cue::setDelayLength(float delayLength)
{
    this->delayLength = delayLength;
    return this->delayLength == delayLength;
}
float Cue::getDelayLength()
{
    return this->delayLength;
}

std::map<Fixture *, std::map<std::string, int>> Cue::getFixtureVals()
{
    return this->fixtureVals;
}

Cue::TRIGGER Cue::getCueTrigger()
{
    return this->trigger;
}

std::string Cue::getCueTitle()
{
    return this->cue_title;
}