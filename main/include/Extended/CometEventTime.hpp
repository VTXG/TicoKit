#pragma once

#include "TicoKit.h"
#include "Local/Game/Screen/TimeLimitLayout.hpp"

class ExtTimeLimitLayout : public TimeLimitLayout {
public:
    void addTime(s32 amount);
};

namespace TK {
    void addCometTimer(s32 amount);
}