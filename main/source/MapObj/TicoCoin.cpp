#include "MapObj/TicoCoin.hpp"
#include "Util/TicoCoinUtil.hpp"

namespace {
    NEW_NERVE(NrvRotate, TicoCoin, Rotate);
    NEW_NERVE(NrvCollect, TicoCoin, Collect);
}

TicoCoin::TicoCoin(const char *pName) : LiveActor(pName) {
    mId = -1;
}

TicoCoin::~TicoCoin() {

}

void TicoCoin::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    
    initModelManagerWithAnm("TicoCoin", nullptr, false);
    initEffectKeeper(0, nullptr, false);
    initHitSensor(1);
    initNerve(&NrvRotate::sInstance);

    MR::getJMapInfoArg0NoInit(rIter, &mId);
    MR::useStageSwitchWriteA(this, rIter);

    if (TicoCoinUtil::isValidTicoCoinIdInStage(mId)) {
        MR::startBva(this, TicoCoinUtil::hasTicoCoinInStage(mId) ? "Empty" : "Normal");
    }
    else {
        MR::startBva(this, "Normal");
        mId = -1;
    }

    MR::addHitSensorMapObj(this, "Body", 4, 120.0f, TVec3f(0.0f, 215.0f, 0.0f));
    MR::calcGravity(this);
    MR::validateClipping(this);
    MR::setClippingFarMax(this);
    MR::connectToSceneNoSilhouettedMapObj(this);
    makeActorAppeared();
}

bool TicoCoin::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::isMsgStarPieceReflect(msg);
}

bool TicoCoin::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgItemGet(msg)) {
        return tryCollect();
    }

    return false;
}

void TicoCoin::exeRotate() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Rotate");
    }
}

void TicoCoin::exeCollect() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Get");
        MR::startSystemSE("SE_SY_QUESTION_COIN", -1, -1);
        MR::hideModel(this);

        TicoCoinUtil::setTicoCoinInStage(mId);

        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }
    }

    if (MR::isGreaterStep(this, 2)) {
        makeActorDead();
    }
}

bool TicoCoin::tryCollect() {
    if (isNerve(&NrvRotate::sInstance)) {
        setNerve(&NrvCollect::sInstance);
        return true;
    }

    return false;
}