#pragma once

#include "TicoKit.h"
#include "Game/LiveActor/LiveActor.hpp"

class TicoCoin : public LiveActor {
public:
    TicoCoin(const char *);

    virtual ~TicoCoin();
    virtual void init(const JMapInfoIter& rIter);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeRotate();
    void exeCollect();
    
    bool tryCollect();
    bool isCollectedInStage() const;

    s32 mId;
};