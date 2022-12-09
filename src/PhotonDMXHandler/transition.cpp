#include "transition.h"
#include "cue.h"
#include <vector>
#include <map>
#include <string>
#include "universe.h"
#include <functional>
#include "logger.h"
#include <cmath>

Transition::Transition(Cue *before, Cue *after, std::function<void()> callback)
{
    this->beforeData = before->getFixtureVals();
    this->afterData = after->getFixtureVals();
    this->length = (int)(after->getTransitionLength() * (float)UniverseManager::getRefreshRate());
    this->callback = callback;
    int delayFrames = (float)UniverseManager::getRefreshRate() * after->getDelayLength();
    this->currentFrame = -delayFrames;
    for (int i = 0; i < length + 1; i++)
    {
        std::map<Fixture *, std::map<std::string, int>> allTransFixVals;
        for (auto fix : this->beforeData)
        {
            Fixture *fixture = fix.first;
            std::map<std::string, int> beforeFixVals = fix.second;
            std::map<std::string, int> afterFixVals = this->afterData[fixture];
            std::map<std::string, int> transFixVals = beforeFixVals;
            for (auto pair : beforeFixVals)
            {
                std::string attr = pair.first;
                float beforeVal = pair.second;
                float afterVal = afterFixVals[attr];
                int transVal = (((afterVal - beforeVal) / (float)length) * i) + beforeVal;
                transFixVals[attr] = transVal;
            }
            allTransFixVals[fixture] = transFixVals;
        }
        this->transitionData.push_back(allTransFixVals);
    }
    this->transitionData.push_back(afterData);
}

Transition::~Transition()
{
}

std::map<Fixture *, std::map<std::string, int>> Transition::getTransitionDataAtFrame(int frame)
{
    if (frame < 0)
        return this->transitionData[0];
    if (frame >= length)
        return this->transitionData[length - 1];
    return this->transitionData[frame];
}

void Transition::nextFrame()
{
    this->currentFrame++;
    if (currentFrame != length)
    {
        for (auto val : this->getTransitionDataAtFrame(currentFrame))
        {
            Fixture *f = val.first;
            std::map<std::string, int> attrs = val.second;
            for (auto a : attrs)
            {
                f->setAttribute(a.first, a.second);
            }
        }
        return;
    }
    this->callback();
}

int Transition::getLength()
{
    return this->length;
}