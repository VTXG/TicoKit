#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ProjmapEffectMtxSetter;

class FileSelectSky : public LiveActor {
public:
    FileSelectSky(const char *);

    virtual ~FileSelectSky();
    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAnim();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();

    /* 0x8C */ f32 _8C;
    /* 0x90 */ f32 _90;
    /* 0x94 */ TPos3f mBaseMtx;
    /* 0xC4 */ ProjmapEffectMtxSetter* mMtxSetter;
};