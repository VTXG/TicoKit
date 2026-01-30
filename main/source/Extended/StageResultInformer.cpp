#include "Extended/StageResultInformer.hpp"
#include "Extended/GalaxyStatusAccessor.hpp"
#include "Extended/GameData.hpp"
#include "Util/TicoCoinUtil.hpp"

#include "Local/Game/System/StageResultSequenceChecker.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/Util/LayoutUtil.hpp"

namespace {
    NEW_NERVE(NrvAppearGetTicoCoin, ExtStageResultInformer, AppearGetTicoCoin);
    NEW_NERVE(NrvDisplayGetTicoCoin, ExtStageResultInformer, DisplayGetTicoCoin);
    NEW_NERVE(NrvDisappearGetTicoCoin, ExtStageResultInformer, DisappearGetTicoCoin);
}

void ExtStageResultInformer::initTicoCoinCounter() {
    if (!TicoCoinUtil::isNeedToUpdateStageResultTicoCoin()) {
        mTicoCoinCounter = nullptr;
        return;
    }

    const char *pClearedStage = GameSequenceFunction::getClearedStageName();
    mTicoCoinCounter = new TicoCoinStageCounter("TicoCoinResultCounter");
    mTicoCoinCounter->init(
        TicoCoinUtil::getTicoCoinMask(pClearedStage),
        TicoCoinUtil::getStockedTicoCoinFlag(pClearedStage)
    );
    mTicoCoinCounter->setFollow(&mCounterPos);
    TicoCoinUtil::updateStageResultTicoCoin();
}

void ExtStageResultInformer::exeAppearGetTicoCoin() {
    if (MR::isFirstStep(this)) {
        setCounterPaneShowHide(false, false, false);

        MR::startAnim(this, "Appear", 0);
        MR::setTextBoxGameMessageRecursive(this, "Result", "System_Result010");
        MR::startAnim(this, "Line", 1);
        MR::setAnimFrameAndStopAdjustTextHeight(this, "Result", 1);

        MR::copyPaneTrans(&mCounterPos, this, "Arrow");
        mTicoCoinCounter->appear();
    }

    if (mTicoCoinCounter->isDisplay()) {
        setNerve(&NrvDisplayGetTicoCoin::sInstance);
    }
}

void ExtStageResultInformer::exeDisplayGetTicoCoin() {
    if (!MR::isGreaterEqualStep(this, 10)) {
        return;
    }

    if (MR::isStep(this, 10)) {
        mTicoCoinCounter->requestCollect(TicoCoinUtil::getTicoCoinFlagInStage());
        mIconAButton->openWithoutMessage();
    }

    tryWaitSystemPadTriggerDecide(&NrvDisappearGetTicoCoin::sInstance);
}

void ExtStageResultInformer::exeDisappearGetTicoCoin() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);

        mTicoCoinCounter->disappear();
        mIconAButton->term();
    }

    if (MR::isAnimStopped(this, 0)) {
        mTicoCoinCounter->kill();
        decideNextNerve();
    }
}

namespace {
    void initStageResultInformer(ExtStageResultInformer *pInformer, const Nerve *pNerve) {
        pInformer->initNerve(pNerve);
        pInformer->initTicoCoinCounter();
    }

    void decideNextNerve(ExtStageResultInformer *pInformer) {
        if (!pInformer->mTicoCoinCounter || pInformer->isNerve(&NrvDisappearGetTicoCoin::sInstance)) {
            pInformer->kill();
            return;
        }

        pInformer->setNerve(&NrvAppearGetTicoCoin::sInstance);
    }

    void fillCheckListIfComplete(StageResultSequenceCheckList *pList, const ExtGameDataHolder *pGameDataHolder, const char *pGalaxyName) {
        ExtGalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(pGalaxyName);

        if (accessor.getPowerStarNum() == pGameDataHolder->getPowerStarNumOwned(pGalaxyName) &&
            accessor.getTicoCoinNum() == pGameDataHolder->getTicoCoinNumOwned(pGalaxyName)) {
            pList->mIsCompleteGalaxy = true;
        }
        else {
            pList->mIsCompleteGalaxy = false;
        }
    }
}

extern kmSymbol init__21StarReturnDemoStarterFRC12JMapInfoIter;
kmWrite16(&init__21StarReturnDemoStarterFRC12JMapInfoIter + 0x3A, sizeof(ExtStageResultInformer));

extern kmSymbol init__26GrandStarReturnDemoStarterFRC12JMapInfoIter;
kmWrite16(&init__26GrandStarReturnDemoStarterFRC12JMapInfoIter + 0x3A, sizeof(ExtStageResultInformer));

extern kmSymbol init__19StageResultInformerFRC12JMapInfoIter;
kmCall(&init__19StageResultInformerFRC12JMapInfoIter + 0x48, initStageResultInformer);

extern kmSymbol decideNextNerve__19StageResultInformerFv;
kmWrite32(&decideNextNerve__19StageResultInformerFv + 0x1C0, PPC_MR(3, 25)); // mr r3, r25
kmCall(&decideNextNerve__19StageResultInformerFv + 0x1C4, decideNextNerve);
kmWrite32(&decideNextNerve__19StageResultInformerFv + 0x1C8, PPC_B(0xC));

extern kmSymbol fillCheckListIfComplete__26StageResultSequenceCheckerFP28StageResultSequenceCheckListPC14GameDataHolderPCc;
kmBranch(&fillCheckListIfComplete__26StageResultSequenceCheckerFP28StageResultSequenceCheckListPC14GameDataHolderPCc, fillCheckListIfComplete);