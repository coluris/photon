#define _USE_MATH_DEFINES

#include "fixture.h"
#include <thread>
#include "effect.h"
#include "logger.h"
#include <cstdlib>
#include <cmath>
#include <thread>
#include <math.h>
#include <random>
#include <algorithm>

std::map<std::string, Effect::E_SHAPE> Effect::shape_names{{"square", Effect::SQUARE}, {"sine", Effect::SINE}, {"ramp", Effect::RAMP}, {"sawtooth", Effect::SAW}, {"custom", Effect::CUSTOM}};
std::map<Effect::E_SHAPE, std::string> Effect::names_of_shapes{{Effect::SQUARE, "square"}, {Effect::SINE, "sine"}, {Effect::RAMP, "ramp"}, {Effect::SAW, "sawtooth"}, {Effect::CUSTOM, "custom"}};
Effect::Effect(std::vector<Fixture *> fixList, std::string effect_type)
{
    this->fixList = fixList;
    this->effect_type = effect_type;
    this->effect_shape = SINE;

    this->paramList["bpm"] = 120.0;
    this->paramList["stagger"] = 100.0;
    this->paramList["size"] = 100.0;
    this->paramList["multiplier"] = 1.0;
    this->paramList["offset"] = 100.0;
    this->paramList["phase"] = 0.0;
    this->paramList["direction"] = 1.0;
    this->rand_fixList = fixList;

    std::vector<Fixture *> rev_fix(fixList.rbegin(), fixList.rend());
    this->rev_fixList = rev_fix;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(this->rand_fixList.begin(), this->rand_fixList.end(), g);
}

Effect::~Effect()
{
}

void Effect::processEffect()
{
    int tick = UniverseManager::getTick();
    switch (this->effect_shape)
    {
    case SINE:
        resolveSine(tick);
        break;
    case SQUARE:
        resolveSquare(tick);
        break;
    case RAMP:
        resolveRamp(tick);
        break;
    case SAW:
        resolveSawtooth(tick);
        break;
    case CUSTOM:
        resolveCustom(tick);
        break;
    }
}

void Effect::resolveSine(int tick)
{

    float bpm = this->paramList["bpm"] * this->paramList["multiplier"];
    int f_rate = UniverseManager::getRefreshRate();
    float size = this->paramList["size"];
    float siz_adjst = 2.55 * size;
    int num_fix = this->fixList.size();
    int t_per_cycle = (float)f_rate / (bpm / 60.0);
    int index = 0;
    float phaseOffsetCoeff = (this->paramList["phase"] * t_per_cycle) / 360.0;
    std::vector<Fixture *> fix_vec =
        this->paramList["direction"] < 0
            ? this->rev_fixList
        : this->paramList["direction"] > 0 ? this->fixList
                                           : this->rand_fixList;
    for (Fixture *f : fix_vec)
    {
        int val = ((127.5 * size) / 100.0) * -cos((1 / ((t_per_cycle / 2) / M_PI)) * ((tick + phaseOffsetCoeff) + ((index * t_per_cycle) / num_fix))) + 255 - (127.5 * (size / 100));
        f->setAttribute(this->effect_type, val);
        ++index;
    }
}

void Effect::resolveSquare(int tick)
{
    float bpm = this->paramList["bpm"] * this->paramList["multiplier"];
    int f_rate = UniverseManager::getRefreshRate();
    float size = this->paramList["size"];
    float siz_adjst = 2.55 * size;
    int num_fix = this->fixList.size();
    int t_per_cycle = (float)f_rate / (bpm / 60.0);
    int index = 1;
    float phaseOffsetCoeff = (this->paramList["phase"] * t_per_cycle) / 360.0;
    std::vector<Fixture *> rev_fix_vec(this->fixList.rbegin(), this->fixList.rend());
    std::vector<Fixture *> fix_vec =
        this->paramList["direction"] < 0
            ? this->rev_fixList
        : this->paramList["direction"] > 0 ? this->fixList
                                           : this->rand_fixList;

    for (Fixture *f : fix_vec)
    {
        float val = -siz_adjst * (2.0 * std::floor(((float)tick + phaseOffsetCoeff + (((float)index * (float)t_per_cycle) / (float)num_fix)) / (float)t_per_cycle) - std::floor(2.0 * ((float)tick + phaseOffsetCoeff + (((float)index * (float)t_per_cycle) / (float)num_fix)) / (float)t_per_cycle)) + (255.0 - siz_adjst);
        f->setAttribute(this->effect_type, (int)val);
        ++index;
    }
}

void Effect::resolveRamp(int tick)
{
    float bpm = this->paramList["bpm"] * this->paramList["multiplier"];
    int f_rate = UniverseManager::getRefreshRate();
    float size = this->paramList["size"];
    float siz_adjst = 2.55 * size;
    int num_fix = this->fixList.size();
    int t_per_cycle = (float)f_rate / (bpm / 60.0);
    int index = 1;
    float phaseOffsetCoeff = (this->paramList["phase"] * t_per_cycle) / 360.0;
    std::vector<Fixture *> rev_fix_vec(this->fixList.rbegin(), this->fixList.rend());
    std::vector<Fixture *> fix_vec =
        this->paramList["direction"] < 0
            ? this->rev_fixList
        : this->paramList["direction"] > 0 ? this->fixList
                                           : this->rand_fixList;

    for (Fixture *f : fix_vec)
    {
        int val = (2 * siz_adjst) * std::abs((((float)tick + phaseOffsetCoeff + (((float)index * t_per_cycle) / (float)num_fix)) / t_per_cycle) - std::floor(((tick + phaseOffsetCoeff + (((float)index * t_per_cycle) / (float)num_fix)) / t_per_cycle) + (1.0 / 2.0))) + (255 - siz_adjst);
        f->setAttribute(this->effect_type, val);
        ++index;
    }
}

void Effect::resolveSawtooth(int tick)
{
    float bpm = this->paramList["bpm"] * this->paramList["multiplier"];
    int f_rate = UniverseManager::getRefreshRate();
    float size = this->paramList["size"];
    float siz_adjst = 2.55 * size;
    int num_fix = this->fixList.size();
    int t_per_cycle = f_rate / (bpm / 60.0);
    int index = 1;
    int phaseOffsetCoeff = (this->paramList["phase"] * t_per_cycle) / 360;
    std::vector<Fixture *> rev_fix_vec(this->fixList.rbegin(), this->fixList.rend());
    std::vector<Fixture *> fix_vec =
        this->paramList["direction"] < 0
            ? this->rev_fixList
        : this->paramList["direction"] > 0 ? this->fixList
                                           : this->rand_fixList;

    for (Fixture *f : fix_vec)
    {
        int val = (int)((siz_adjst * (tick + phaseOffsetCoeff + ((index * t_per_cycle) / num_fix))) / t_per_cycle) % (int)siz_adjst;
        f->setAttribute(this->effect_type, val);
        ++index;
    }
}

void Effect::resolveCustom(int tick)
{
}

void Effect::updateFixtures(std::vector<Fixture *> fixList)
{
    this->fixList = fixList;
}

bool Effect::updateParam(std::string param, float val)
{
    this->paramList[param] = val;
    return paramList[param] == val;
}

bool Effect::updateShape(E_SHAPE shape)
{
    this->effect_shape = shape;
    return this->effect_shape == shape;
}

Effect::E_SHAPE Effect::getShapeFromName(std::string name)
{
    return Effect::shape_names[name];
}

std::string Effect::getEffectType()
{
    return this->effect_type;
}
std::vector<Fixture *> Effect::getFixtureList()
{
    return this->fixList;
}
float Effect::getParameter(std::string param)
{
    return this->paramList[param];
}

std::string Effect::getNameFromShape(E_SHAPE shape)
{
    return Effect::names_of_shapes[shape];
}
Effect::E_SHAPE Effect::getEffectShape()
{
    return this->effect_shape;
}