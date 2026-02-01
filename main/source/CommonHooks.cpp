#include "TicoKit.h"
#include "Player/MarioConstLoader.hpp"
#include "System/AstroConfigTable.hpp"
#include "System/GalaxyInfoTable.hpp"
#include "System/GameConfigTable.hpp"
#include "System/GameEventTable.hpp"

#include "Game/System/GameSystem.hpp"

typedef void (*Func)();
extern Func __ctor_loc;
extern Func __ctor_end;

namespace {
    void init(GameSystem *pGameSystem) {
        for (Func *f = &__ctor_loc; f < &__ctor_end; f++) {
            (*f)();
        }

        TK::initSingleton<AstroConfigTable>();
        TK::initSingleton<GalaxyInfoTable>();
        TK::initSingleton<GameConfigTable>();
        TK::initSingleton<GameEventTable>();
        MarioConstLoader::init();

        pGameSystem->initAfterStationedResourceLoaded();
    }
}

extern kmSymbol initAfterStationedResourceLoaded__18GameSystemFunctionFv;
kmBranch(&initAfterStationedResourceLoaded__18GameSystemFunctionFv + 0x4, init);