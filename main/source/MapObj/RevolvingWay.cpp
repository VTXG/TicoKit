#include "TicoKit.h"
#include "Game/MapObj/RevolvingWay.hpp"
#include "Game/Util/JMapUtil.hpp"

namespace {
    const char* getModelName() {
        register const JMapInfoIter* pIter;
        GET_REGISTER(pIter, r30);

        const char *pName;
        MR::getObjectName(&pName, *pIter);
        return pName;
    }

    asm void funRegisterTime() {
        mr r29, r3
        mr r30, r4
        blr
    }

    asm void funRegisterTimePart2() {
        mr r4, r3
        mr r30, r4
        blr
    }
}

extern kmSymbol init__12RevolvingWayFRC12JMapInfoIter;
kmCall(&init__12RevolvingWayFRC12JMapInfoIter + 0x14, funRegisterTime);
kmCall(&init__12RevolvingWayFRC12JMapInfoIter + 0x1C, getModelName);
kmCall(&init__12RevolvingWayFRC12JMapInfoIter + 0x20, funRegisterTimePart2);
kmWrite32(&init__12RevolvingWayFRC12JMapInfoIter + 0x24, PPC_MR(3, 29)); // mr r3, r29
kmWrite32(&init__12RevolvingWayFRC12JMapInfoIter + 0x98, PPC_MR(4, 30)); // mr r4, r30