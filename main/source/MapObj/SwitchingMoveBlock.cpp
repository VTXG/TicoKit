#include "MapObj/SwitchingMoveBlock.hpp"
#include "Util/GeneralUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "JSystem/JGeometry/TQuat.hpp"

MoveBlockFunction::MoveBlockFunction() : NameObj("MoveBlockFunction"),
    mPosQuat(0.0f, 0.0f, 0.0f, 1.0f),
    mMoveQuat(0.0f, 0.0f, 0.0f, 1.0f),
    _2C(0.0f),
    _38(0.0f),
    mMoveRot1(0.0f),
    mMoveRot2(0.0f) {

    mBlend = 0.0f;
    mTime = 60;
    _64 = 1;
    _68 = 0;
    mMoveState = true;
}

MoveBlockFunction::~MoveBlockFunction() {

}

void MoveBlockFunction::init(const JMapInfoIter &rIter) {
    TVec3f rot(0.0f);
    MR::getJMapInfoRotate(rIter, &rot);
    MR::makeQuatRotateDegree(&mMoveQuat, rot);

    mPosQuat.x = mMoveQuat.x;
    mPosQuat.y = mMoveQuat.y;
    mPosQuat.z = mMoveQuat.z;
    mPosQuat.w = mMoveQuat.w;

    MR::getJMapInfoTrans(rIter, &_2C);
    _2C.set(_38);
}

void MoveBlockFunction::setMoveRot1(const TVec3f &rVec) {
    mMoveRot1.set(rVec);
}

void MoveBlockFunction::setMoveRot2(const TVec3f &rVec) {
    mMoveRot2.set(rVec);
}

void MoveBlockFunction::setMoveTime(s32 a1) {
    if (a1 < 0) {
        a1 = 0;
    }

    mTime = a1;
}

void MoveBlockFunction::sub_802D4A70(s32 a1) {
    if (a1 < 1) {
        a1 = 1;
    }

    _64 = a1;
}

void MoveBlockFunction::setMoveState(bool a1) {
    mMoveState = a1;

    if (a1) {
        if (++_68 > _64) {
            _68 = _64;
        }
    }
    else {
        if (--_68 < 0) {
            _68 = 0;
        }
    }
}

bool MoveBlockFunction::updateRotate() {
    f32 v0 = 1.0f / (f32)mTime;

    if (!mMoveState) {
        v0 = -v0;
    }

    mBlend += v0;
    f32 v1 = (f32)_68 / (f32)_64;
    
    bool isMoveComplete = false;

    if (mMoveState) {
        if (mBlend > v1) {
            mBlend = v1;
            isMoveComplete = true;
        }
    }
    else if (mBlend < v1) {
        mBlend = v1;
        isMoveComplete = true;
    }

    TVec3f stack_14;
    TVec3f stack_8;

    stack_14.set(_38);
    mMoveQuat.transform(mMoveRot1, stack_8);

    TK::vecKillElement(&stack_14, &stack_8, mBlend);
    _2C.set(stack_14);

    f32 v2 = TK::normalizeRadians(PI_180 * mMoveRot2.x * mBlend);
    f32 v3 = TK::normalizeRadians(PI_180 * mMoveRot2.y * mBlend);
    f32 v4 = TK::normalizeRadians(PI_180 * mMoveRot2.z * mBlend);

    mPosQuat.setEuler(v2, v3, v4);
    PSQUATMultiply((Quaternion *)&mMoveQuat, (Quaternion *)&mPosQuat, (Quaternion *)&mPosQuat);

    return isMoveComplete;
}

namespace {
    NEW_NERVE(NrvWait, SwitchingMoveBlock, Wait);
    NEW_NERVE(NrvMove, SwitchingMoveBlock, Move);
}

SwitchingMoveBlock::SwitchingMoveBlock(const char *pName) : LiveActor(pName) {
    mMoveBlockFunc = nullptr;
}

SwitchingMoveBlock::~SwitchingMoveBlock() {

}

void SwitchingMoveBlock::init(const JMapInfoIter &rIter) {
    const char *pObjectName;
    MR::getObjectName(&pObjectName, rIter);

    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm(pObjectName, nullptr, false);

    mMoveBlockFunc = new MoveBlockFunction();
    mMoveBlockFunc->init(rIter);

    if (MR::isObjectName(rIter, "RedBlueTurnBlock")) {
        s32 moveTime = 40;
        MR::getJMapInfoArg0NoInit(rIter, &moveTime);
        mMoveBlockFunc->setMoveTime(moveTime);

        TVec3f rot1(0.0f);
        mMoveBlockFunc->setMoveRot1(rot1);

        TVec3f rot2(0.0f, 0.0f, 180.0f);
        MR::getJMapInfoArg1NoInit(rIter, &rot2.x);
        MR::getJMapInfoArg2NoInit(rIter, &rot2.y);
        MR::getJMapInfoArg3NoInit(rIter, &rot2.z);
        mMoveBlockFunc->setMoveRot2(rot2);

        MR::createPartsModelMapObjStrongLight(this, "Base", "RedBlueTurnBlockBase", nullptr);
    }    

    initHitSensor(1);
    MR::initCollisionParts(this, pObjectName, MR::addBodyMessageSensorMapObj(this), nullptr);

    initNerve(&NrvWait::sInstance);
    MR::useStageSwitchReadA(this, rIter);
    MR::listenStageSwitchOnOffA(this, MR::Functor(this, listenSwitchOnA), MR::Functor(this, listenSwitchOffA));

    MR::setClippingFar200m(this);
    MR::connectToSceneMapObj(this);
    makeActorAppeared();
}

void SwitchingMoveBlock::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mMoveBlockFunc->mPosQuat);
}

void SwitchingMoveBlock::listenSwitchOnA() {
    if (!MR::isDead(this)) {
        MR::invalidateClipping(this);
        mMoveBlockFunc->setMoveState(true);

        setNerve(&NrvMove::sInstance);
        MR::startSystemSE("SE_SY_REDBLUE_PANEL_FLIP", -1, -1);
        /*
        lis       r4, aMovestart@ha # "MoveStart"
        mr        r3, r31
        addi      r4, r4, aMovestart@l # "MoveStart"
        li        r5, -1
        li        r6, -1
        li        r7, -1
        bl        sub_800083F0
        */
    }
}

void SwitchingMoveBlock::listenSwitchOffA() {
    if (!MR::isDead(this)) {
        MR::invalidateClipping(this);
        mMoveBlockFunc->setMoveState(false);

        setNerve(&NrvMove::sInstance);
        MR::startSystemSE("SE_SY_REDBLUE_PANEL_FLIP", -1, -1);
        /*
        lis       r4, aMovestart@ha # "MoveStart"
        mr        r3, r31
        addi      r4, r4, aMovestart@l # "MoveStart"
        li        r5, -1
        li        r6, -1
        li        r7, -1
        bl        sub_800083F0
        */
    }
}

void SwitchingMoveBlock::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }
}

void SwitchingMoveBlock::exeMove() {
    if (mMoveBlockFunc->updateRotate()) {
        setNerve(&NrvWait::sInstance);
        /*
        lis       r4, aMoveend@ha # "MoveEnd"
        mr        r3, r31
        addi      r4, r4, aMoveend@l # "MoveEnd"
        li        r5, -1
        li        r6, -1
        li        r7, -1
        bl        sub_800083F0
        */
    }
    else {
        /*
        lis       r4, aMovelv@ha # "MoveLv"
        mr        r3, r31
        addi      r4, r4, aMovelv@l # "MoveLv"
        li        r5, -1
        li        r6, -1
        li        r7, -1
        bl        sub_800083F0
        */
    }
}