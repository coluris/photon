#pragma once
#include "transition.h"
#include <vector>
#include "cue.h"

class Cuelist
{
    std::vector<Cue *> cuelist;
    Transition *currentTransition = nullptr;
    int currentCue;

public:
    Cuelist();
    void appendCue(Cue *cue);
    void appendCopy(Cue *cue);
    void insertCue(Cue *cue, int index);
    void insertCopy(Cue *cue, int index);
    bool release();
    bool goToLast();
    bool goToFirst();
    bool selectCue(int cue);
    bool nextCue();
    bool previousCue();
    bool isActive();
    Cue *getActiveCue();
    Transition *getActiveTransition();
    bool setActiveTransition(Transition *activeTransition);
};