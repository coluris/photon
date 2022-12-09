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

Cue::Cue(TRIGGER trigger, float transitionLength, float delayLength)
{
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
            f->setAttribute(a.first, a.second);
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