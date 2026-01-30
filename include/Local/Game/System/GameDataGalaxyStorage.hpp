#pragma once

#include <revolution/types.h>
#include "Game/System/BinaryDataChunkHolder.hpp"

class GalaxyStatusAccessor;
class BinaryDataContentAccessor;
class BinaryDataContentHeaderSerializer;

class GameDataSomeGalaxyStorage {
public:
    GameDataSomeGalaxyStorage(const GalaxyStatusAccessor &);

    void resetAllData();
    s32 getPowerStarNumOwned() const;
    void deserialize(const BinaryDataContentAccessor &, const u8 *);
    void serialize(const BinaryDataContentAccessor &, u8 *);

    /* 0x00 */ const char* mName;
    /* 0x04 */ s32 mPowerStarNum;
    /* 0x08 */ u8 mPowerStarFlag;
    /* 0x09 */ u8 mFirstPlayFlag;
    /* 0x0C */ u16 mMaxCoinNum[8];
};

class GameDataAllGalaxyStorage : public BinaryDataChunkBase {
public:
    GameDataAllGalaxyStorage();

    virtual u32 makeHeaderHashCode() const;
    virtual u32 getSignature() const;
    virtual s32 serialize(u8 *, u32) const;
    virtual s32 deserialize(const u8 *, u32);
    virtual void initializeData();

    bool isExistAccessor(const char *pGalaxyName, int scenarioNo) const;
    // makeAccessor();
    s32 calcCurrentPowerStarNum() const;
    s32 getPowerStarNumOwned(const char *pGalaxyName) const;
    s32 getGalaxyNum() const;
    GameDataSomeGalaxyStorage* getGalaxyStorage(int index);
    void initHeaderSerializer();
    int findIndex(const char *pGalaxyName) const;
    int findIndex(u16 hash) const;

    /* 0x04 */ const void* mVtbl2;
    /* 0x08 */ GameDataSomeGalaxyStorage** mGalaxyStorage;
    /* 0x0C */ u32 _C; // Always 0x40?
    /* 0x10 */ u32 mGalaxyStorageNum;
    /* 0x14 */ BinaryDataContentHeaderSerializer* mHeaderSerializer;
};