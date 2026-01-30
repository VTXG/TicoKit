#pragma once

#include "TicoKit.h"
#include "Game/LiveActor/LiveActor.hpp"

namespace MR {
    PartsModel* createPartsModelMapObjStrongLight(LiveActor *pHost, const char *pName, const char *pModelName, MtxPtr pMtx);
}

class MoveBlockFunction : public NameObj {
public:
    MoveBlockFunction();

    virtual ~MoveBlockFunction();
    virtual void init(const JMapInfoIter& rIter);

    void setMoveRot1(const TVec3f &rVec);
    void setMoveRot2(const TVec3f &rVec);
    void setMoveTime(s32 time);
    void sub_802D4A70(s32);
    void setMoveState(bool state);
    bool updateRotate();

    TQuat4f mPosQuat;    // 0x0C
    TQuat4f mMoveQuat;   // 0x1C
    TVec3f _2C;
    TVec3f _38;
    TVec3f mMoveRot1;    // 0x44
    TVec3f mMoveRot2;    // 0x50
    f32 mBlend;          // 0x5C
    s32 mTime;           // 0x60
    s32 _64;
    s32 _68;
    bool mMoveState;     // 0x6C
};

class SwitchingMoveBlock : public LiveActor {
public:
    SwitchingMoveBlock(const char *pName);

    virtual ~SwitchingMoveBlock();
    virtual void init(const JMapInfoIter &rIter);
    virtual void calcAndSetBaseMtx();
    
    void exeWait();
    void exeMove();
    
    void listenSwitchOnA();
    void listenSwitchOffA();

    MoveBlockFunction* mMoveBlockFunc;
};