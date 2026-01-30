#pragma once

#include "TicoKit.h"
#include "Screen/TicoCoinCounter.hpp"

#include "Local/Game/Screen/StageResultInformer.hpp"

class ExtStageResultInformer : public StageResultInformer {
public:
    void initTicoCoinCounter();
    void exeAppearGetTicoCoin();
    void exeDisplayGetTicoCoin();
    void exeDisappearGetTicoCoin();

    TicoCoinStageCounter* mTicoCoinCounter;
    TVec2f mCounterPos;
};