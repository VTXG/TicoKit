#pragma once

#include "TicoKit.h"

#include "Game/AreaObj/AreaObj.hpp"

class SpinSwitchArea : public AreaObj {
public:
    SpinSwitchArea(int type, const char *pName);

    virtual ~SpinSwitchArea();
    virtual void init(const JMapInfoIter &rIter);
    virtual void movement();
    virtual const char* getManagerName() const;

    bool mIsSwing;
};