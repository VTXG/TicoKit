#pragma once

#include "TicoKit.h"
#include "Game/AreaObj/AreaObj.hpp"

class NameObjArchiveListCollector;

class ForceMorphArea : public AreaObj {
public:
    ForceMorphArea(int type, const char *pName);

    virtual ~ForceMorphArea();
    virtual void init(const JMapInfoIter &rIter);
    virtual bool isInVolume(const TVec3f &rPos) const;
    virtual const char* getManagerName() const;

    inline u16 getPlayerMode() const {
        return mObjArg0;
    }

    static void makeArchiveList(NameObjArchiveListCollector *, const JMapInfoIter &);
};