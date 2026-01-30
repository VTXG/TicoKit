#include "Extended/GalaxyStatusAccessor.hpp"
#include "Extended/GameData.hpp"
#include "Util/GeneralUtil.hpp"

#include "Game/Util/SceneUtil.hpp"

s32 ExtScenarioData::getTicoCoinNum() const {
    return TK::getOnBitNum(getTicoCoinMask());
}

u8 ExtScenarioData::getTicoCoinMask() const {
    u8 mask = 0u;

    for (s32 i = 1; i <= mScenarioData->getNumEntries(); i++) {
        u32 id = 0;
        getValueU32("TicoCoinId", i, &id);

        if (id > 0) {
            mask |= id;
        }
    }

    return mask;
}

u32 ExtScenarioData::getCometLimitTimer(s32 scenario) const {
    u32 timer = 0;
    getValueU32("CometLimitTimer", scenario, &timer);
    return timer;
}

s32 ExtGalaxyStatusAccessor::getTicoCoinNum() const {
    return getExtScenarioData()->getTicoCoinNum();
}

s32 ExtGalaxyStatusAccessor::getTicoCoinNumOwned() const {
    ExtGameDataHolder *pHolder = GameDataFunction::getCurrentExtGameDataHolder();
    return pHolder->getExtAllGalaxyStorage()->getTicoCoinNumOwned(mScenarioData->mGalaxyName);
}

u32 ExtGalaxyStatusAccessor::getCometLimitTimer(s32 scenario) const {
    return getExtScenarioData()->getCometLimitTimer(scenario);
}

namespace {
    bool isCompletedAccessor(const ExtGalaxyStatusAccessor &rAccessor) {
        return rAccessor.getPowerStarNum() == rAccessor.getPowerStarNumOwned()
            && rAccessor.getTicoCoinNum() == rAccessor.getTicoCoinNumOwned();
    }

    bool isCompleted(const char *pGalaxyName) {
        return isCompletedAccessor(MR::makeGalaxyStatusAccessor(pGalaxyName));
    }
}

extern kmSymbol isCompleted__20GalaxyStatusAccessorCFv;
kmBranch(&isCompleted__20GalaxyStatusAccessorCFv, isCompletedAccessor);

extern kmSymbol isStarComplete__2MRFPCc;
kmBranch(&isStarComplete__2MRFPCc, isCompleted);

extern kmSymbol execute__Q231unnamedGalaxyMapDomeIcon_cpp17CheckerIsCompleteFPCc;
kmCall(&execute__Q231unnamedGalaxyMapDomeIcon_cpp17CheckerIsCompleteFPCc + 0x1C, isCompleted);
kmWrite32(&execute__Q231unnamedGalaxyMapDomeIcon_cpp17CheckerIsCompleteFPCc + 0x20, PPC_CMPWI(3, 0)); // cmpwi r3, 0
kmWrite32(&execute__Q231unnamedGalaxyMapDomeIcon_cpp17CheckerIsCompleteFPCc + 0x24, PPC_COND_BRANCH(PPC_COND_EQ, 0x2C)); // beq   0x2C
kmWrite32(&execute__Q231unnamedGalaxyMapDomeIcon_cpp17CheckerIsCompleteFPCc + 0x28, PPC_B(0x30));