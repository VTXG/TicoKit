#pragma once

#include "TicoKit.h"
#include "Game/Util/Color.hpp"

struct AstroOrbitSetup {
    f32 mRadius;
    TVec3f mRotation;
};

struct AstroConfigEntry {
    u32 mKoopaFortressBgmId;
    u8 mOrbitColor[3];
    u8 mOrbitColorBloom[3];
    AstroOrbitSetup mOrbitSetup[5];
    f32 mOrbitSpeed;
    const char* mSkyName;
};

class AstroConfigTable {
public:
    AstroConfigTable();

    const AstroConfigEntry* getCurrentEntry();
    const AstroConfigEntry* getEntry(s32 scenarioNo);

    static void loadEntry(void *pResource, AstroConfigEntry *pEntry);

private:
    AstroConfigEntry mEntries[6];
};