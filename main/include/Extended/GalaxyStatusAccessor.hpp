#pragma once

#include "TicoKit.h"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/ScenarioDataParser.hpp"

class ExtScenarioData : public ScenarioData {
public:
    s32 getTicoCoinNum() const;
    u8 getTicoCoinMask() const;
    u32 getCometLimitTimer(s32 scenario) const;
};

class ExtGalaxyStatusAccessor : public GalaxyStatusAccessor {
public:
    inline ExtGalaxyStatusAccessor(const GalaxyStatusAccessor &rBase) : GalaxyStatusAccessor(rBase) {}

    s32 getTicoCoinNum() const;
    s32 getTicoCoinNumOwned() const;
    u32 getCometLimitTimer(s32 scenario) const;

    inline ExtScenarioData* getExtScenarioData() const {
        return (ExtScenarioData *)mScenarioData;
    }
};