#pragma once

#include "TicoKit.h"

#include "Local/Game/System/GameEventFlag.hpp"
#include "Local/Game/System/GameEventValue.hpp"
#include "Game/Util/Array.hpp"

class JKRArchive;

class GameEventTable {
public:
    GameEventTable();

    void initFlagTable(void *pResource);
    void initValueTable(void *pResource);
    void initStoryEventTable(void *pResource);

    /// @brief Gets a pointer to a GameEventFlag based on its index.
    /// @param idx The flag index.
    /// @return A pointer to the flag, or nullptr if the flag index is invalid.
    const GameEventFlag* getFlag(int idx) const;

    /// @brief Gets the index of a GameEventFlag pointer.
    /// @param pFlag The flag to get the index of.
    /// @return The index of the flag.
    int getFlagIndex(const GameEventFlag *pFlag) const;

    MR::AssignableArray<GameEventFlag> mFlagTable;
    MR::AssignableArray<GameEventValue> mValueTable;
    void* mStoryEventTableData;
};

class ExtGameEventFlagTable {
public:
    ExtGameEventFlagTable();

    /// @brief Gets a pointer to a GameEventFlag based on its index.
    /// @param idx The flag index.
    /// @return A pointer to the flag, or nullptr if the flag index is invalid.
    const GameEventFlag* getFlag(int idx) const;

    /// @brief Gets the index of a GameEventFlag pointer.
    /// @param pFlag The flag to get the index of.
    /// @return The index of the flag.
    int getIndex(const GameEventFlag *pFlag) const;

    GameEventFlag* mFlagTable;
    s32 mFlagNum;
};