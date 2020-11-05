#pragma once

#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"

#define RRQ_DEFAULT_NOTE_STRENGTH       1.00
#define RRQ_DEFAULT_NOTE_DURATION       0.14
#define RRQ_DEFAULT_SABER_STRENGTH      0.75
#define RRQ_DEFAULT_WALL_STRENGTH       1.00
#define RRQ_DEFAULT_UI_STRENGTH         1.00

#define RRQ_RECOMMENDED_NOTE_STRENGTH   1.00
#define RRQ_RECOMMENDED_NOTE_DURATION   0.14
#define RRQ_RECOMMENDED_SABER_STRENGTH  0.25
#define RRQ_RECOMMENDED_WALL_STRENGTH   0.50
#define RRQ_RECOMMENDED_UI_STRENGTH     0.75

class ConfigClass {
public:
    bool enabled;
    float noteStrength;
    float noteDuration;
    float saberStrength;
    float wallStrength;
    float uiStrength;

    void load();
    void store();

    ConfigClass(Configuration&);

private:
    Configuration& _storage;
};

ConfigClass& getConfig();
