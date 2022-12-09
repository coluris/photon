#pragma once

#include "fixture.h"
#include <thread>
#include <string>
#include <vector>
#include <map>

class Effect
{

public:
    Effect(std::vector<Fixture *> fixList, std::string effect_type);
    ~Effect();
    void updateFixtures(std::vector<Fixture *> fixList);
    bool updateParam(std::string param, float val);
    void processEffect();
    void resolveSine(int tick);
    void resolveSquare(int tick);
    void resolveRamp(int tick);
    void resolveSawtooth(int tick);
    void resolveCustom(int tick);
    enum E_SHAPE
    {
        SQUARE,
        SINE,
        RAMP,
        SAW,
        CUSTOM
    };
    bool updateShape(E_SHAPE shape);
    static E_SHAPE getShapeFromName(std::string name);

private:
    std::string effect_type;
    std::vector<Fixture *> fixList;
    std::map<std::string, float> paramList;
    static std::map<std::string, E_SHAPE> shape_names;
    E_SHAPE effect_shape;
};