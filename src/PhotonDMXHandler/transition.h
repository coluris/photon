#pragma once
#include <vector>
#include <map>
#include <string>
#include "fixture.h"
#include "cue.h"
#include <functional>

class Transition
{

public:
    Transition(Cue *before, Cue *after, std::function<void()> callback);
    ~Transition();
    int getLength();
    std::map<Fixture *, std::map<std::string, int>> getTransitionDataAtFrame(int frame);
    void nextFrame();

private:
    std::map<Fixture *, std::map<std::string, int>> beforeData;
    std::map<Fixture *, std::map<std::string, int>> afterData;
    std::vector<std::map<Fixture *, std::map<std::string, int>>> transitionData;
    int length;
    int currentFrame;
    std::function<void()> callback;
};