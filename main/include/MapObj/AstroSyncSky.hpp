#pragma once

#include "TicoKit.h"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"

class AstroSyncSky : public LiveActor {
public:
    AstroSyncSky(const char *pName);

    virtual ~AstroSyncSky();
    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAndSetBaseMtx();

    static const char* getSkyName();

    ProjmapEffectMtxSetter* mMtxSetter;
};