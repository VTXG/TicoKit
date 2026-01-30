#include "MapObj/TimeAttackClock.hpp"
#include "Extended/CometEventTime.hpp"
#include "Screen/PopUpAddTimeBoard.hpp"
#include "Util/GeneralUtil.hpp"

#include "Game/MapObj/CoinRotater.hpp"
#include "Game/Util/ActorMovementUtil.hpp"

namespace {
    NEW_NERVE(NrvWait, TimeAttackClock, Wait);
    NEW_NERVE(NrvCollect, TimeAttackClock, Collect);
}

TimeAttackClock::TimeAttackClock(const char *pName) : LiveActor(pName) {
    mBaseMtx.identity();
    mPositionMtx.identity();
    mTimeAdd = 10;

    MR::createCoinRotater();
    TK::createPopUpAddTimeBoard();
}

TimeAttackClock::~TimeAttackClock() {

}

void TimeAttackClock::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("TimeAttackClock", nullptr, false);
    initHitSensor(1);
    initEffectKeeper(0, nullptr, false);
    initNerve(&NrvWait::sInstance);

    bool calcShadow = true;
    bool removeTime = false;
    f32 colorFrame  = 0.0f;
    MR::getJMapInfoArg0NoInit(rIter, &mTimeAdd);
    MR::getJMapInfoArg1NoInit(rIter, &removeTime);
    MR::getJMapInfoArg2NoInit(rIter, &calcShadow);
    MR::getJMapInfoArg3NoInit(rIter, &colorFrame);
    MR::useStageSwitchReadAppear(this, rIter);

    if (removeTime) {
        mTimeAdd = -mTimeAdd;
    }

    if (calcShadow) {
        MR::initShadowFromCSV(this, "Shadow");
        MR::onCalcShadowAll(this);
    }

    MR::startBrkAndSetFrameAndStop(this, "ColorChange", colorFrame);
    TK::setAllEffectHostMtx(this, mPositionMtx.toMtxPtr());
    MR::makeMtxTR(mBaseMtx.toMtxPtr(), this);

    MR::addHitSensorMapObj(this, "Body", 8, 120.0f, TVec3f(0.0f, 100.0f, 0.0f));
    MR::validateClipping(this);
    MR::connectToSceneNoShadowedMapObj(this);

    if (MR::isValidSwitchAppear(this)) {
        MR::listenStageSwitchOnAppear(this, MR::Functor(this, &appear));
        kill();
    }
    else {
        makeActorAppeared();
    }
}

void TimeAttackClock::appear() {
    LiveActor::appear();
}

void TimeAttackClock::calcAndSetBaseMtx() {
    if (isNerve(&NrvWait::sInstance)) {
        mPositionMtx.concat(mBaseMtx, MR::getCoinInWaterRotateYMatrix());
    }

    MR::setBaseTRMtx(this, mPositionMtx.toMtxPtr());
}

void TimeAttackClock::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvWait::sInstance) && MR::isSensorPlayer(pReceiver)) {
        addTimer();
    }
}

bool TimeAttackClock::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::isMsgStarPieceReflect(msg);
}

void TimeAttackClock::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
        MR::emitEffect(this, "Wait");
    }
}

void TimeAttackClock::exeCollect() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Get");
        MR::emitEffect(this, "Get");

        if (mTimeAdd < 0) {
            MR::shakeCameraVeryWeak();
            MR::startSystemSE("SE_SY_E3_TIMER_COUNT_1", -1, -1);
        }

        MR::startSystemSE("SE_SY_T_ATCK_CLOCK_TAKEN", -1, -1);
    }
    else {
        kill();
    }
}

void TimeAttackClock::addTimer() {
    TK::addCometTimer(mTimeAdd * 60);
    TK::requestPopUpAddTimeBoard(mTimeAdd);
    setNerve(&NrvCollect::sInstance);
}