#include "MapObj/AstroSyncSky.hpp"
#include "Extended/GameData.hpp"
#include "System/AstroConfigTable.hpp"

#include "Local/Game/System/GameEventValueChecker.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util/SceneUtil.hpp"

AstroSyncSky::AstroSyncSky(const char *pName) : LiveActor(pName) {
    mMtxSetter = nullptr;
}

AstroSyncSky::~AstroSyncSky() {

}

void AstroSyncSky::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    const char *pSkyName = getSkyName();
    initModelManagerWithAnm(pSkyName, nullptr, true);
    mMtxSetter = MR::initDLMakerProjmapEffectMtxSetter(this);
    MR::newDifferedDLBuffer(this);
    mMtxSetter->updateMtxUseBaseMtx();
    MR::tryStartAllAnim(this, pSkyName);

    MR::connectToSceneSky(this);
    MR::invalidateClipping(this);
    MR::registerDemoSimpleCastAll(this);
    MR::useStageSwitchReadAppear(this, rIter);
    if (MR::isValidSwitchAppear(this)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}

void AstroSyncSky::calcAndSetBaseMtx() {
    mPosition = MR::getCamPos();
    LiveActor::calcAndSetBaseMtx();
    mMtxSetter->updateMtxUseBaseMtx();
}

const char* AstroSyncSky::getSkyName() {
    u32 entryNo = GameDataFunction::getGameEventValue("LastDomeNo");

    if (entryNo < 1 || entryNo > 6) {
        entryNo = 1;
    }

    return TK::getAstroSkyName(entryNo);
}

namespace {
    void updateLastDomeNo() {
        if (MR::isEqualStageName("AstroDome")) {
            GameEventValueChecker *pChecker = GameDataFunction::getCurrentExtGameDataHolder()->mEventValueChecker;

            int idx = pChecker->findIndex("LastDomeNo");
            if (idx != -1) {
                pChecker->mValueTable[idx] = MR::getCurrentScenarioNo();
            }
        }
    }
}

extern kmSymbol init__9GameSceneFv;
kmBranch(&init__9GameSceneFv + 0x1F4, updateLastDomeNo);