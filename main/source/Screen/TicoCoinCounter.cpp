#include "Screen/TicoCoinCounter.hpp"
#include "Util/GeneralUtil.hpp"
#include "Util/TicoCoinUtil.hpp"

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/RaceManager.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/CounterLayoutController.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

namespace {
    NEW_NERVE(NrvStageCounterWait, TicoCoinStageCounter, Wait);
    NEW_NERVE(NrvStageCounterAppear, TicoCoinStageCounter, Appear);
    NEW_NERVE(NrvStageCounterDisplay, TicoCoinStageCounter, Display);
    NEW_NERVE(NrvStageCounterDisappear, TicoCoinStageCounter, Disappear);
}

TicoCoinStageCounter::TicoCoinStageCounter(const char *pName) : LayoutActor(pName, false) {
    mVisibleFlag = 0;
    mCollectFlag = 0;
    mFlashFlag = 0;
}

TicoCoinStageCounter::~TicoCoinStageCounter() {

}

void TicoCoinStageCounter::init(u8 visibleFlag, u8 collectFlag) {
    mVisibleFlag = visibleFlag;
    mCollectFlag = collectFlag;

    initLayoutManager("TicoCoinStageCounter", 1);
    initNerve(&NrvStageCounterWait::sInstance);
    initPanes();

    MR::connectToSceneLayout(this);
    MR::registerDemoSimpleCastAll(this);

    LayoutActor::appear();
}

void TicoCoinStageCounter::appear() {
    if (!isNerve(&NrvStageCounterAppear::sInstance) && !isNerve(&NrvStageCounterDisplay::sInstance)) {
        setNerve(&NrvStageCounterAppear::sInstance);
    }
}

void TicoCoinStageCounter::kill() {
    MR::hideLayout(this);
    setNerve(&NrvStageCounterWait::sInstance);
}

void TicoCoinStageCounter::disappear() {
    if (isNerve(&NrvStageCounterAppear::sInstance) || isNerve(&NrvStageCounterDisplay::sInstance)) {
        setNerve(&NrvStageCounterDisappear::sInstance);
    }
}

void TicoCoinStageCounter::setFollow(const TVec2f *pPosition) {
    MR::setFollowPos(pPosition, this, nullptr);
}

void TicoCoinStageCounter::setScale(f32 x, f32 y) {
    MR::setPaneScale(this, x, y, nullptr);
}

void TicoCoinStageCounter::requestCollect(u8 flag) {
    flag &= ~mCollectFlag;

    if (flag) {
        mFlashFlag |= flag;
        appear();
    }
}

bool TicoCoinStageCounter::isDisplay() const {
    return isNerve(&NrvStageCounterDisplay::sInstance);
}

void TicoCoinStageCounter::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::hideLayout(this);
    }
}

void TicoCoinStageCounter::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        MR::startAnim(this, "Appear", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&NrvStageCounterDisplay::sInstance);
    }
}

void TicoCoinStageCounter::exeDisplay() {
    if (mFlashFlag) {
        doCollect(true);
    }
}

void TicoCoinStageCounter::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&NrvStageCounterWait::sInstance);
    }
}

void TicoCoinStageCounter::initPanes() {
    u8 visibleFlag = mVisibleFlag;
    u8 collectFlag = mCollectFlag;

    TVec2f offsetPosition;
    offsetPosition.zero();

    for (s32 i = 0; i < 8; i++) {
        char paneName[32];
        snprintf(paneName, sizeof(paneName), "Medal%d", i);

        if (visibleFlag & 1) {
            MR::showPaneRecursive(this, paneName);
            MR::createAndAddPaneCtrl(this, paneName, 2);

            MR::startPaneAnim(this, paneName, "Icon", 0);
            MR::setPaneAnimFrameAndStop(this, paneName, (f32)(collectFlag & 1), 0);
        }
        else {
            MR::hidePaneRecursive(this, paneName);

            offsetPosition.x += 25.0f;
            TK::setPanePos(this, "CounterOffset", offsetPosition);
        }

        visibleFlag >>= 1;
        collectFlag >>= 1;
    }
}

void TicoCoinStageCounter::doCollect(bool flash) {
    u8 flag = mFlashFlag;

    for (s32 i = 0; i < 8; i++) {
        char paneName[32];
        snprintf(paneName, sizeof(paneName), "Medal%d", i);

        if ((flag & 1) && !((mCollectFlag >> i) & 1)) {
            if (flash) {
                MR::startPaneAnim(this, paneName, "Flash", 1);
            }

            MR::startPaneAnim(this, paneName, "Icon", 0);
            MR::setPaneAnimFrameAndStop(this, paneName, 1.0f, 0);
        }

        flag >>= 1;
    }

    mCollectFlag |= mFlashFlag;
    mFlashFlag = 0;
}

namespace {
    NEW_NERVE(NrvGlobalCounterWait, TicoCoinGlobalCounter, Wait);
    NEW_NERVE(NrvGlobalCounterAppear, TicoCoinGlobalCounter, Appear);
    NEW_NERVE(NrvGlobalCounterDisplay, TicoCoinGlobalCounter, Display);
    NEW_NERVE(NrvGlobalCounterDisappear, TicoCoinGlobalCounter, Disappear);
}

TicoCoinGlobalCounter::TicoCoinGlobalCounter(const char *pName) : LayoutActor(pName, false) {
    mCoinCount = 0;
    mNewCoinCount = 0;
    mAppearer = nullptr;
    mRumbler = nullptr;
    mStageCounter = nullptr;
    mStageCounterPos.zero();
    mCollectFlag = 0;
    mForceAppear = false;
}

TicoCoinGlobalCounter::~TicoCoinGlobalCounter() {

}

void TicoCoinGlobalCounter::init(const JMapInfoIter& rIter) {
    mCoinCount = TicoCoinUtil::calcCurrentTicoCoinNum();
    mNewCoinCount = mCoinCount;

    initLayoutManager("TicoCoinGlobalCounter", 2);
    initNerve(&NrvGlobalCounterWait::sInstance);

    MR::createAndAddPaneCtrl(this, "Counter", 1);

    mAppearer = new CounterLayoutAppearer(this, TVec2f(-50.0f, 0.0f));
    mRumbler = new CountUpPaneRumbler(this, "Counter");

    if (!MR::isStageAstroLocation() && TicoCoinUtil::isTicoCoinExistCurrentStage()) {
        const char *pStageName = MR::getCurrentStageName();

        mCollectFlag = TicoCoinUtil::getStockedTicoCoinFlag(pStageName) | TicoCoinUtil::getTicoCoinFlagInStage();
        mStageCounter = new TicoCoinStageCounter("TicoCoinStageCounterForStage");
        mStageCounter->init(TicoCoinUtil::getTicoCoinMask(pStageName), mCollectFlag);
        mStageCounter->setScale(0.8f, 0.8f);

        MR::copyPaneTrans(&mStageCounterPos, this, "StageCounter");
        MR::copyPaneTrans(&mStageCounterPosRace, this, "StageCounterR");
        MR::connectToSceneLayout(mStageCounter);
    }

    MR::setTextBoxNumberRecursive(this, "Counter", mCoinCount);
    MR::connectToSceneLayout(this);

    LayoutActor::appear();
}

void TicoCoinGlobalCounter::movement() {
    if (mStageCounter) {
        RaceManager *pRaceManager = MR::getSceneObj<RaceManager>(SceneObj_RaceManager);

        if (pRaceManager && pRaceManager->mLayout && !MR::isDead(pRaceManager->mLayout)) {
            mStageCounter->setFollow(&mStageCounterPosRace);
        }
        else {
            mStageCounter->setFollow(&mStageCounterPos);
        }
    }

    if (TicoCoinUtil::isTicoCoinExistCurrentStage()) {
        u8 flag = TicoCoinUtil::getTicoCoinFlagInStage() & ~mCollectFlag;

        if (flag) {
            mCollectFlag |= flag;
            mStageCounter->requestCollect(flag);
            mNewCoinCount = TicoCoinUtil::calcCurrentTicoCoinNum();
            appear();
        }
    }

    LayoutActor::movement();
}

void TicoCoinGlobalCounter::appear() {
    if (!isNerve(&NrvGlobalCounterAppear::sInstance) && !isNerve(&NrvGlobalCounterDisplay::sInstance)) {
        setNerve(&NrvGlobalCounterAppear::sInstance);

        if (mStageCounter) {
            mStageCounter->appear();
        }
    }
}

void TicoCoinGlobalCounter::kill() {
    MR::hideLayout(this);
    setNerve(&NrvGlobalCounterWait::sInstance);

    if (mStageCounter) {
        mStageCounter->kill();
    }
}

void TicoCoinGlobalCounter::control() {


    mAppearer->update();
    mRumbler->update();
}

void TicoCoinGlobalCounter::disappear() {
    if (isNerve(&NrvGlobalCounterAppear::sInstance) || isNerve(&NrvGlobalCounterDisplay::sInstance)) {
        setNerve(&NrvGlobalCounterDisappear::sInstance);

        if (mStageCounter) {
            mStageCounter->disappear();
        }
    }
}

void TicoCoinGlobalCounter::setForceAppear(bool v) {
    mForceAppear = v;
    appear();
}

bool TicoCoinGlobalCounter::isDisplay() const {
    return isNerve(&NrvGlobalCounterDisplay::sInstance);
}

void TicoCoinGlobalCounter::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::hideLayout(this);
    }
}

void TicoCoinGlobalCounter::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);

        mRumbler->reset();
        mAppearer->appear(TVec2f(0.0f, 0.0f));

        MR::startAnim(this, "Wait", 1);
    }

    if (mAppearer->isAppeared()) {
        setNerve(&NrvGlobalCounterDisplay::sInstance);
    }
}

void TicoCoinGlobalCounter::exeDisplay() {
    if (mNewCoinCount != mCoinCount) {
        mCoinCount = mNewCoinCount;

        MR::setTextBoxNumberRecursive(this, "Counter", mCoinCount);
        MR::startAnim(this, "Flash", 0);

        mRumbler->start();
        mSpine->mStep = 0;
    }

    if (!mForceAppear && MR::isGreaterEqualStep(this, 180)) {
        disappear();
    }
}

void TicoCoinGlobalCounter::exeDisappear() {
    if (MR::isFirstStep(this)) {
        mAppearer->disappear();
    }

    if (mAppearer->isDisappeared()) {
        setNerve(&NrvGlobalCounterWait::sInstance);
    }
}