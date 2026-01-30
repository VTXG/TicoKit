#include "System/AstroConfigTable.hpp"
#include "Util/GeneralUtil.hpp"
#include "Util/JMapProperty.hpp"

#include "Game/AudioLib/AudSoundNameConverter.hpp"
#include "Game/MapObj/AstroDomeOrbit.hpp"
#include "Game/Util/SceneUtil.hpp"

AstroConfigTable::AstroConfigTable() {
    JKRArchive *pArchive = TK::receiveArchive("/ObjectData/AstroConfigTable.arc");

    for (s32 i = 0; i < 6; i++) {
        char name[32];
        snprintf(name, sizeof(name), "astroconfig%d.bcsv", i + 1);
        loadEntry(pArchive->getResource(name), &mEntries[i]);
    }

    OSDebug("AstroConfigTable : Loaded all entries\n");
}

const AstroConfigEntry* AstroConfigTable::getCurrentEntry() {
    return &mEntries[MR::getCurrentScenarioNo() - 1];
}

const AstroConfigEntry* AstroConfigTable::getEntry(s32 scenarioNo) {
    return &mEntries[scenarioNo - 1];
}

void AstroConfigTable::loadEntry(void *pResource, AstroConfigEntry *pEntry) {
    JMapPropertyInfo info;
    info.attach(pResource);

    JMapPropertyIter iter = info.find("OrbitColor");
    for (s32 i = 0; i < LIST_COUNT(pEntry->mOrbitColor); i++) {
        s32 tmp;

        iter.getParamInt(i, &tmp);
        pEntry->mOrbitColor[i] = tmp;
    }

    iter = info.find("OrbitColorBloom");
    for (s32 i = 0; i < LIST_COUNT(pEntry->mOrbitColor); i++) {
        s32 tmp;

        iter.getParamInt(i, &tmp);
        pEntry->mOrbitColorBloom[i] = tmp;
    }

    for (s32 i = 0; i < 5; i++) {
        char name[64];
        snprintf(name, sizeof(name), "OrbitSetup%d", i);

        iter = info.find(name);
        if (!iter.isValid()) {
            break;
        }

        s32 tmp;
        iter.getParamInt(0, &tmp);
        pEntry->mOrbitSetup[i].mRadius = tmp;

        iter.getParamF32(0, &pEntry->mOrbitSetup[i].mRotation.x);
        iter.getParamF32(1, &pEntry->mOrbitSetup[i].mRotation.y);
        iter.getParamF32(2, &pEntry->mOrbitSetup[i].mRotation.z);
    }

    iter = info.find("OrbitSpeed");
    iter.getParamF32(0, &pEntry->mOrbitSpeed);

    iter = info.find("SkyName");
    iter.getParamStr(0, &pEntry->mSkyName);

    const char *pBgmName;
    iter = info.find("KoopaFortressBgm");
    iter.getParamStr(0, &pBgmName);

    pEntry->mKoopaFortressBgmId = AudSingletonHolder<AudSoundNameConverter>::get()->getSoundID(pBgmName).mID;
}

namespace {
    u32 getKoopaFortressBgmId() {
        return TK::getSingleton<AstroConfigTable>()->getCurrentEntry()->mKoopaFortressBgmId;
    }

    void drawOrbit(AstroDomeOrbit *pOrbit) {
        const AstroConfigEntry *pEntry = TK::getSingleton<AstroConfigTable>()->getCurrentEntry();

        Color8 color;
        color.r = pEntry->mOrbitColor[0];
        color.g = pEntry->mOrbitColor[1];
        color.b = pEntry->mOrbitColor[2];
        color.a = 0xFF;
        pOrbit->initDraw(color);
    }

    void drawOrbitBloom(AstroDomeOrbit *pOrbit) {
        const AstroConfigEntry *pEntry = TK::getSingleton<AstroConfigTable>()->getCurrentEntry();

        Color8 color;
        color.r = pEntry->mOrbitColorBloom[0];
        color.g = pEntry->mOrbitColorBloom[1];
        color.b = pEntry->mOrbitColorBloom[2];
        color.a = 0xFF;
        pOrbit->initDraw(color);
    }

    void setupOrbit(AstroDomeOrbit *pOrbit, s32 idx) {
        const AstroConfigEntry *pEntry = TK::getSingleton<AstroConfigTable>()->getCurrentEntry();

        pOrbit->_8C = pEntry->mOrbitSetup[idx].mRadius;
        pOrbit->_90 = 230.0f * idx;
        pOrbit->mRotation.set(pEntry->mOrbitSetup[idx].mRotation);
    }

    void moveCoord(AstroDomeOrbit *pOrbit) {
        const AstroConfigEntry *pEntry = TK::getSingleton<AstroConfigTable>()->getCurrentEntry();
        pOrbit->_90 = pOrbit->calcRepeatedRotateCoord(pOrbit->_90 + pEntry->mOrbitSpeed);
    }

    const char* getSkyName(s32 scenarioNo) {
        return TK::getSingleton<AstroConfigTable>()->getEntry(scenarioNo)->mSkyName;
    }

    const char* getSkyNameForAstroList() {
        register s32 scenarioNo;
#ifndef INTELLISENSE_FIX
        __asm {
            mr scenarioNo, r29
        }
#endif
        return getSkyName(scenarioNo);
    }
}

extern kmSymbol getKoopaFortressAppearBgm__15AudStageBgmWrapFPCc;
kmCall(&getKoopaFortressAppearBgm__15AudStageBgmWrapFPCc + 0x38, getKoopaFortressBgmId);
kmWrite32(&getKoopaFortressAppearBgm__15AudStageBgmWrapFPCc + 0x3C, PPC_MR(0, 3)); // mr r0, r3

extern kmSymbol draw__14AstroDomeOrbitCFv;
kmCall(&draw__14AstroDomeOrbitCFv + 0x28, drawOrbit);

extern kmSymbol drawBloom__14AstroDomeOrbitCFv;
kmCall(&drawBloom__14AstroDomeOrbitCFv + 0x28, drawOrbitBloom);

extern kmSymbol setup__14AstroDomeOrbitFl;
kmBranch(&setup__14AstroDomeOrbitFl, setupOrbit);

extern kmSymbol moveCoord__14AstroDomeOrbitFv;
kmBranch(&moveCoord__14AstroDomeOrbitFv, moveCoord);

extern kmSymbol getModelName__19AstroMapObjFunctionFPCcl;
kmCall(&getModelName__19AstroMapObjFunctionFPCcl + 0x54, getSkyNameForAstroList);
kmWrite32(&getModelName__19AstroMapObjFunctionFPCcl + 0x58, PPC_B(0x54));

extern kmSymbol init__12AstroDomeSkyFRC12JMapInfoIter;
kmWrite32(&init__12AstroDomeSkyFRC12JMapInfoIter + 0x38, PPC_LWZ(3, 8, 1)); // lwz r3, 8(r1)
kmCall(&init__12AstroDomeSkyFRC12JMapInfoIter + 0x3C, getSkyName);
kmWrite32(&init__12AstroDomeSkyFRC12JMapInfoIter + 0x40, PPC_MR(4, 3)); // mr r4, r3
kmWrite32(&init__12AstroDomeSkyFRC12JMapInfoIter + 0x48, PPC_MR(3, 28)); // mr r3, r28
kmWrite32(&init__12AstroDomeSkyFRC12JMapInfoIter + 0x50, PPC_MR(30, 4)); // mr r30, r4
kmWrite32(&init__12AstroDomeSkyFRC12JMapInfoIter + 0x54, PPC_B(0x8));