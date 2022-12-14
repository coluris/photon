#pragma once
#include <vector>
#include <map>
#include <string>
#include "effect.h"
#include "fixture.h"
class Cue
{

public:
    enum TRIGGER
    {
        HALT,
        FOLLOW,
    };
    Cue(TRIGGER trigger = HALT, float transitionLength = 2.0, float delayLength = 0.0);
    void addEffect(Effect *effect);
    void removeEffect(Effect *effect);
    void setValue(Fixture *fix, std::string attr, int val);
    void setValue(std::vector<Fixture *> fixList, std::string attr, int val);
    std::map<Fixture *, std::map<std::string, int>> getFixtureVals();
    bool setTransitionLength(float transitionLength);
    float getTransitionLength();
    bool setDelayLength(float delayLength);
    float getDelayLength();
    static TRIGGER getTriggerByName(std::string name);
    static std::string getNameByTrigger(TRIGGER trigger);
    TRIGGER getCueTrigger();
    void activate();
    std::vector<Effect *> getEffects();
    std::string getCueTitle();
    void setCueTitle(std::string title);

private:
    std::vector<Effect *> effects;
    std::map<Fixture *, std::map<std::string, int>> fixtureVals;
    static std::map<std::string, TRIGGER> triggerMap;
    static std::map<TRIGGER, std::string> triggerNameMap;
    float transitionLength;
    float delayLength;
    TRIGGER trigger;
    std::string cue_title;
};