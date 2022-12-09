#include "cuelist.h"
#include "transition.h"
#include <vector>
#include "logger.h"

Cuelist::Cuelist()
{
    this->currentCue = 0;
}

void Cuelist::appendCue(Cue *cue)
{
    this->cuelist.push_back(cue);
}

void Cuelist::appendCopy(Cue *cue)
{
    Cue *copyCue = new Cue(*cue);
    this->cuelist.push_back(copyCue);
}

void Cuelist::insertCue(Cue *cue, int index)
{
    this->cuelist.insert(this->cuelist.begin() + index, cue);
}

void Cuelist::insertCopy(Cue *cue, int index)
{
    Cue *copyCue = new Cue(*cue);
    this->cuelist.insert(this->cuelist.begin() + index, copyCue);
}

bool Cuelist::isActive()
{
    return this->currentCue != 0;
}

bool Cuelist::release()
{
    this->currentCue = 0;
    return this->currentCue == 0;
}

bool Cuelist::goToFirst()
{
    this->currentCue = 1;
    this->cuelist.at(this->currentCue - 1)->activate();
    return this->currentCue == 1;
}

bool Cuelist::goToLast()
{
    this->currentCue = this->cuelist.size();
    this->cuelist.at(this->currentCue - 1)->activate();
    return this->currentCue == this->cuelist.size();
}

bool Cuelist::selectCue(int cue)
{
    this->currentCue = cue;
    this->cuelist.at(this->currentCue - 1)->activate();
    return this->currentCue == cue;
}

bool Cuelist::nextCue()
{
    int currentIndex = this->currentCue;
    this->currentCue = (this->currentCue % this->cuelist.size()) + 1;
    int nextIndex = this->currentCue;
    if (this->cuelist.at(this->currentCue - 1)->getTransitionLength() != 0)
    {
        std::function<void()> callback = [this]()
        { this->cuelist.at(this->currentCue - 1)->activate();
    this->currentTransition = nullptr; };
        Transition *transition = new Transition(this->cuelist.at(currentIndex - 1), this->cuelist.at(nextIndex - 1), callback);
        this->currentTransition = transition;
    }
    else
    {
        this->cuelist.at(this->currentCue - 1)->activate();
        this->currentTransition = nullptr;
    }
    return this->currentCue == (this->currentCue % this->cuelist.size()) + 1;
}

bool Cuelist::previousCue()
{
    if (this->currentCue == 1)
        return goToLast();
    this->currentCue = ((this->currentCue - 2) % (int)this->cuelist.size()) + 1;
    this->cuelist.at(this->currentCue - 1)->activate();
    return this->currentCue == ((this->currentCue - 2) % this->cuelist.size()) + 1;
}

Cue *Cuelist::getActiveCue()
{
    return this->cuelist.at(this->currentCue - 1);
}

Transition *Cuelist::getActiveTransition()
{
    return this->currentTransition;
}