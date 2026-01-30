#pragma once

#include "TicoKit.h"
#include "Game/MapObj/BenefitItemObj.hpp"

class BenefitItemDash : public BenefitItemObj {
public:
    BenefitItemDash(const char *pName);

    virtual ~BenefitItemDash();
    virtual void initModelAndEfx();
    virtual void exeCatch();

    u32 mDashTime;
    f32 mDashForward;
};