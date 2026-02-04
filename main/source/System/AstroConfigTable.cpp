#include "System/AstroConfigTable.hpp"
#include "Util/GeneralUtil.hpp"

#include "Game/AudioLib/AudSoundNameConverter.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/Util/SceneUtil.hpp"

AstroConfigTable::AstroConfigTable() {
    JKRArchive *pArchive = TK::receiveArchive("/ObjectData/AstroConfigTable.arc");

    for (s32 i = 0; i < 6; i++) {
        char name[32];
        snprintf(name, sizeof(name), "astroconfig%d.bcsv", i + 1);
        mTables[i].attach(pArchive->getResource(name));
    }

    OSDebug("AstroConfigTable : Loaded all entries\n");
}

bool AstroConfigTable::getOrbitColor(s32 scenarioNo, Color8 *pColor) {
    JMapPropertyIter iter = mTables[scenarioNo - 1].find("OrbitColor");

    if (iter.isValid()) {
        s32 r, g, b;
        iter.getParamInt(0, &r);
        iter.getParamInt(1, &g);
        iter.getParamInt(2, &b);
        pColor->r = r; pColor->g = g; pColor->b = b;
        pColor->a = 0xFF;
        return true;
    }

    return false;
}

bool AstroConfigTable::getOrbitColorBloom(s32 scenarioNo, Color8 *pColor) {
    JMapPropertyIter iter = mTables[scenarioNo - 1].find("OrbitColorBloom");

    if (iter.isValid()) {
        s32 r, g, b;
        iter.getParamInt(0, &r);
        iter.getParamInt(1, &g);
        iter.getParamInt(2, &b);
        pColor->r = r; pColor->g = g; pColor->b = b;
        pColor->a = 0xFF;
        return true;
    }

    return false;
}

f32 AstroConfigTable::getOrbitRadius(s32 scenarioNo, s32 idx) {
    char name[32];
    snprintf(name, sizeof(name), "OrbitSetup%d", idx);
    JMapPropertyIter iter = mTables[scenarioNo - 1].find(name);

    if (iter.isValid()) {
        s32 out;
        iter.getParamInt(0, &out);
        return (f32)out;
    }

    return 0.0f;
}

bool AstroConfigTable::getOrbitRotation(s32 scenarioNo, s32 idx, TVec3f *pVec) {
    char name[32];
    snprintf(name, sizeof(name), "OrbitSetup%d", idx);
    JMapPropertyIter iter = mTables[scenarioNo - 1].find(name);

    if (iter.isValid()) {
        iter.getParamF32(0, &pVec->x);
        iter.getParamF32(1, &pVec->y);
        iter.getParamF32(2, &pVec->z);
        return true;
    }

    return false;
}

f32 AstroConfigTable::getOrbitSpeed(s32 scenarioNo) {
    JMapPropertyIter iter = mTables[scenarioNo - 1].find("OrbitSpeed");

    if (iter.isValid()) {
        f32 out;
        iter.getParamF32(0, &out);
        return out;
    }

    return nullptr;
}

const char* AstroConfigTable::getSkyName(s32 scenarioNo) {
    JMapPropertyIter iter = mTables[scenarioNo - 1].find("SkyName");

    if (iter.isValid()) {
        const char *pOut;
        iter.getParamStr(0, &pOut);
        return pOut;
    }

    return nullptr;
}

u32 AstroConfigTable::getKoopaFortressBgmId(s32 scenarioNo) {
    JMapPropertyIter iter = mTables[scenarioNo - 1].find("KoopaFortressBgm");

    if (iter.isValid()) {
        const char *pName;
        iter.getParamStr(0, &pName);
        return AudSingletonHolder<AudSoundNameConverter>::get()->getSoundID(pName).mID;
    }

    return nullptr;
}

namespace TK {
    const char* getAstroSkyName(s32 scenarioNo) {
        return TK::getSingleton<AstroConfigTable>()->getSkyName(scenarioNo);
    }
}

namespace {
    u32 getKoopaFortressBgmId() {
        return TK::getSingleton<AstroConfigTable>()->getKoopaFortressBgmId(MR::getCurrentScenarioNo());
    }

    void drawOrbit(ExtAstroDomeOrbit *pOrbit) {
        pOrbit->initDraw(pOrbit->mColor);
    }

    void drawOrbitBloom(ExtAstroDomeOrbit *pOrbit) {
        pOrbit->initDraw(pOrbit->mColorBloom);
    }

    void setupOrbit(ExtAstroDomeOrbit *pOrbit, s32 idx) {
        AstroConfigTable *pTable = TK::getSingleton<AstroConfigTable>();
        s32 scenarioNo = MR::getCurrentScenarioNo();

        pTable->getOrbitColor(scenarioNo, &pOrbit->mColor);
        pTable->getOrbitColorBloom(scenarioNo, &pOrbit->mColorBloom);
        pTable->getOrbitRotation(scenarioNo, idx, &pOrbit->mRotation);
        pOrbit->_8C = pTable->getOrbitRadius(scenarioNo, idx);
        pOrbit->_90 = 230.0f * idx;
        pOrbit->mOrbitSpeed = pTable->getOrbitSpeed(scenarioNo);
    }

    void moveCoord(ExtAstroDomeOrbit *pOrbit) {
        pOrbit->_90 = pOrbit->calcRepeatedRotateCoord(pOrbit->_90 + pOrbit->mOrbitSpeed);
    }

    const char* getSkyNameForAstroList() {
        register s32 scenarioNo;
#ifndef INTELLISENSE_FIX
        __asm {
            mr scenarioNo, r29
        }
#endif
        return TK::getAstroSkyName(scenarioNo);
    }
}

extern kmSymbol init__15MiniatureGalaxyFRC12JMapInfoIter;
kmWrite16(&init__15MiniatureGalaxyFRC12JMapInfoIter + 0x176, sizeof(ExtAstroDomeOrbit));

extern kmSymbol getKoopaFortressAppearBgm__15AudStageBgmWrapFPCc;
kmCall(&getKoopaFortressAppearBgm__15AudStageBgmWrapFPCc + 0x38, getKoopaFortressBgmId);
kmWrite32(&getKoopaFortressAppearBgm__15AudStageBgmWrapFPCc + 0x3C, PPC_MR(0, 3)); // mr r0, r3

extern kmSymbol exeOpen__15MiniatureGalaxyFv;
kmCall(&exeOpen__15MiniatureGalaxyFv + 0x1B0, getKoopaFortressBgmId);
kmWrite32(&exeOpen__15MiniatureGalaxyFv + 0x1B4, PPC_LI(4, 0));
kmCall(&exeOpen__15MiniatureGalaxyFv + 0x1B8, AudWrap::startStageBgm);

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
kmCall(&init__12AstroDomeSkyFRC12JMapInfoIter + 0x3C, TK::getAstroSkyName);
kmWrite32(&init__12AstroDomeSkyFRC12JMapInfoIter + 0x40, PPC_MR(4, 3)); // mr r4, r3
kmWrite32(&init__12AstroDomeSkyFRC12JMapInfoIter + 0x48, PPC_MR(3, 28)); // mr r3, r28
kmWrite32(&init__12AstroDomeSkyFRC12JMapInfoIter + 0x50, PPC_MR(30, 4)); // mr r30, r4
kmWrite32(&init__12AstroDomeSkyFRC12JMapInfoIter + 0x54, PPC_B(0x8));