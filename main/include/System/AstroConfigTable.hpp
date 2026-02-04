#pragma once

#include "TicoKit.h"
#include "Util/JMapProperty.hpp"

#include "Game/MapObj/AstroDomeOrbit.hpp"
#include "Game/Util/Color.hpp"

class ExtAstroDomeOrbit : public AstroDomeOrbit {
public:
    Color8 mColor;
    Color8 mColorBloom;
    f32 mOrbitSpeed;
};

class AstroConfigTable {
public:
    AstroConfigTable();

    bool getOrbitColor(s32 scenarioNo, Color8 *pColor);
    bool getOrbitColorBloom(s32 scenarioNo, Color8 *pColor);
    f32 getOrbitRadius(s32 scenarioNo, s32 idx);
    bool getOrbitRotation(s32 scenarioNo, s32 idx, TVec3f *pVec);
    f32 getOrbitSpeed(s32 scenarioNo);
    const char* getSkyName(s32 scenarioNo);
    u32 getKoopaFortressBgmId(s32 scenarioNo);

    JMapPropertyInfo mTables[6];
};

namespace TK {
    const char* getAstroSkyName(s32 scenarioNo);
}