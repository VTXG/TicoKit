#pragma once

#include "TicoKit.h"
#include "Screen/TicoCoinCounter.hpp"

#include "Game/Screen/CounterLayoutController.hpp"

class ExtCounterLayoutController : public CounterLayoutController {
public:
    TicoCoinGlobalCounter* mTicoCoinCounter;
};