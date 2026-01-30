#pragma once

#include "TicoKit.h"

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/CounterLayoutAppearer.hpp"
#include "Game/Screen/CountUpPaneRumbler.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class TicoCoinStageCounter : public LayoutActor {
public:
    TicoCoinStageCounter(const char *pName);

    virtual ~TicoCoinStageCounter();
    virtual void appear();
    virtual void kill();
    void disappear();

    void init(u8 visibleFlag, u8 collectFlag);
    void setFollow(const TVec2f *pPosition);
    void setScale(f32 x, f32 y);
    void requestCollect(u8 flag);

    bool isDisplay() const;

    void exeWait();
    void exeAppear();
    void exeDisplay();
    void exeDisappear();

    void initPanes();
    void doCollect(bool flash);

    u8 mVisibleFlag;
    u8 mCollectFlag;
    u8 mFlashFlag;
};

class TicoCoinGlobalCounter : public LayoutActor {
public:
    TicoCoinGlobalCounter(const char *pName);

    virtual ~TicoCoinGlobalCounter();
    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void appear();
    virtual void kill();
    virtual void control();
    void disappear();

    void setForceAppear(bool v);
    bool isDisplay() const;

    void exeWait();
    void exeAppear();
    void exeDisplay();
    void exeDisappear();

    s32 mCoinCount;
    s32 mNewCoinCount;
    CounterLayoutAppearer* mAppearer;
    CountUpPaneRumbler* mRumbler;

    TicoCoinStageCounter* mStageCounter;
    TVec2f mStageCounterPos;
    TVec2f mStageCounterPosRace;
    u8 mCollectFlag;

    bool mForceAppear;
};