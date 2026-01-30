#pragma once

#include "TicoKit.h"

#include "Local/Game/System/GameDataGalaxyStorage.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameDataHolder.hpp"
#include "Game/System/GameDataTemporaryInGalaxy.hpp"

class ExtGalaxyStatusAccessor;
class ExtGameDataHolder;
class ExtGameDataAllGalaxyStorage;

namespace GameDataFunction {
    inline ExtGameDataHolder* getCurrentExtGameDataHolder() {
        return (ExtGameDataHolder *)GameDataFunction::getCurrentGameDataHolder();
    }
}

class ExtGameDataHolder : public GameDataHolder {
public:
    bool hasTicoCoin(const char *pStageName, s32 id) const;
    void setTicoCoinOnFlag(const char *pStageName, u8 flag);

    u8 getTicoCoinFlag(const char *pStageName) const;
    s32 getTicoCoinNumOwned(const char *pStageName) const;

    inline ExtGameDataAllGalaxyStorage* getExtAllGalaxyStorage() const {
        return (ExtGameDataAllGalaxyStorage *)mAllGalaxyStorage;
    }
};

class ExtGameDataSomeGalaxyStorage : public GameDataSomeGalaxyStorage {
public:
    s32 getTicoCoinNumOwned() const;

    s32 mTicoCoinNum;
    u8 mTicoCoinFlag;
};

class ExtGameDataAllGalaxyStorage : public GameDataAllGalaxyStorage {
public:
    ExtGameDataSomeGalaxyStorage* findStorage(const char *pStageName) const;

    s32 calcCurrentTicoCoinNum() const;
    s32 getTicoCoinNumOwned(const char *pStageName) const;
};

class ExtGameDataTemporaryInGalaxy : public GameDataTemporaryInGalaxy {
public:
    bool hasTicoCoin(s32 id);
    void setTicoCoin(s32 id);
    void resetStageResultTicoCoinParam();

    u8 mTicoCoinFlag;
};

namespace TK {
    ExtGameDataTemporaryInGalaxy* getGameDataTemporaryInGalaxy();
}