#pragma once

#include "fixture.h"
#include <thread>
#include <string>
#include <vector>
#include <map>

class Effect
{

public:
    Effect(std::vector<Fixture *> fixList, std::string effect_type, std::string uuid);
    ~Effect();
    void updateFixtures(std::vector<Fixture *> fixList);
    bool updateParam(std::string param, float val);
    void processEffect();
    void resolveSine(int tick);
    void resolveSquare(int tick);
    void resolveRamp(int tick);
    void resolveSawtooth(int tick);
    void resolveCustom(int tick);
    static Effect* getEffectByUUID(std::string uuid);
    std::string getUUID();
    std::string getEffectType();
    std::vector<Fixture *> getFixtureList();
    float getParameter(std::string param);
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
    static std::string getNameFromShape(E_SHAPE shape);
    E_SHAPE getEffectShape();

private:
    std::string effect_type;
    std::vector<Fixture *> fixList;
    std::vector<Fixture *> rev_fixList;
    std::vector<Fixture *> rand_fixList;
    std::map<std::string, float> paramList;
    static std::map<std::string, E_SHAPE> shape_names;
    static std::map<E_SHAPE, std::string> names_of_shapes;
    static std::map<std::string, Effect *> effect_pool;
    E_SHAPE effect_shape;
    int currentExtension;
    int getNumExtensions();
    std::string uuid;
};