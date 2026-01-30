#pragma once

#include "TicoKit.h"
#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class PopUpAddTimeAttackTime;

class TimeAttackClock : public LiveActor {
public:
    TimeAttackClock(const char *pName);

    virtual ~TimeAttackClock();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeCollect();
    void addTimer();

    /* 0x8C */ TMtx34f mBaseMtx;
    /* 0xBC */ TMtx34f mPositionMtx;
    /* 0xEC */ s32 mTimeAdd;
};