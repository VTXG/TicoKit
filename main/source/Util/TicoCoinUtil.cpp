#include "Util/TicoCoinUtil.hpp"
#include "Extended/GameData.hpp"
#include "Extended/GalaxyStatusAccessor.hpp"
#include "Util/GeneralUtil.hpp"

#include "Game/Util/SceneUtil.hpp"

namespace TicoCoinUtil {
    bool hasTicoCoinInStage(s32 id) {
        return TicoCoinUtil::hasStockedTicoCoin(MR::getCurrentStageName(), id)
            || TK::getGameDataTemporaryInGalaxy()->hasTicoCoin(id);
    }

    void setTicoCoinInStage(s32 id) {
        if (!TicoCoinUtil::hasStockedTicoCoin(MR::getCurrentStageName(), id)) {
            TK::getGameDataTemporaryInGalaxy()->setTicoCoin(id);
        }
    }

    u8 getTicoCoinFlagInStage() {
        return TK::getGameDataTemporaryInGalaxy()->mTicoCoinFlag;
    }

    s32 getTicoCoinNumInStage() {
        return TK::getOnBitNum(TicoCoinUtil::getTicoCoinFlagInStage());
    }

    u8 getTicoCoinMask(const char *pStageName) {
        ExtGalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(pStageName);
        ExtScenarioData *pScenarioData = accessor.getExtScenarioData();

        if (pScenarioData) {
            return pScenarioData->getTicoCoinMask();
        }

        return 0;
    }

    u8 getTicoCoinMaskCurrentStage() {
        ExtGalaxyStatusAccessor accessor = MR::makeCurrentGalaxyStatusAccessor();
        ExtScenarioData *pScenarioData = accessor.getExtScenarioData();

        if (pScenarioData) {
            return pScenarioData->getTicoCoinMask();
        }

        return 0;
    }

    bool isTicoCoinExist(const char *pStageName) {
        return TicoCoinUtil::getTicoCoinMask(pStageName) != 0;
    }

    bool isTicoCoinExistCurrentStage() {
        return TicoCoinUtil::getTicoCoinMaskCurrentStage() != 0;
    }

    bool isValidTicoCoinIdInStage(s32 id) {
        return (TicoCoinUtil::getTicoCoinMaskCurrentStage() >> id) & 1;
    }

    bool hasStockedTicoCoin(const char *pStageName, s32 id) {
        return GameDataFunction::getCurrentExtGameDataHolder()->hasTicoCoin(pStageName, id);
    }

    u8 getStockedTicoCoinFlag(const char *pStageName) {
        return GameDataFunction::getCurrentExtGameDataHolder()->getTicoCoinFlag(pStageName);
    }

    s32 getTicoCoinNumMax() {
        s32 num = 0;
        ScenarioDataIter iter = MR::makeBeginScenarioDataIter();

        while (!iter.isEnd()) {
            ExtGalaxyStatusAccessor accessor = iter.makeAccessor();
            num += accessor.getTicoCoinNum();
            iter.goNext();
        }

        return num;
    }

    s32 calcStockedTicoCoinNum() {
        return GameDataFunction::getCurrentExtGameDataHolder()->getExtAllGalaxyStorage()->calcCurrentTicoCoinNum();
    }

    s32 calcCurrentTicoCoinNum() {
        return TicoCoinUtil::calcStockedTicoCoinNum() + TicoCoinUtil::getTicoCoinNumInStage();
    }

    bool isNeedToUpdateStageResultTicoCoin() {
        return TK::getGameDataTemporaryInGalaxy()->mTicoCoinFlag != 0;
    }
    
    void updateStageResultTicoCoin() {
        ExtGameDataHolder *pHolder = GameDataFunction::getCurrentExtGameDataHolder();
        ExtGameDataTemporaryInGalaxy *pTemporary = TK::getGameDataTemporaryInGalaxy();

        pHolder->setTicoCoinOnFlag(pTemporary->mStageName, pTemporary->mTicoCoinFlag);
    }
}