#pragma once

#include "TicoKit.h"

#include "Game/AreaObj/AreaObj.hpp"

class ZeroGravityArea : public AreaObj {
public:
    ZeroGravityArea(int type, const char *pName);

    virtual ~ZeroGravityArea();
    virtual void init(const JMapInfoIter &rIter);
    virtual const char* getManagerName() const;
};