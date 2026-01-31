#include "System/GameEventTable.hpp"
#include "Extended/GameData.hpp"
#include "Util/GeneralUtil.hpp"

#include "Local/Game/System/GameEventFlagChecker.hpp"
#include "Local/Game/System/GameEventFlagStorage.hpp"
#include "Local/Game/System/GameEventFlagTable.hpp"
#include "Local/Game/System/GameEventValueChecker.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"

/*

GameEventFlagTable - Unhardcoded sGameEventFlagStatic

GameEventFlag are now defined in ObjectData/GameEventFlagTable.arc/gameeventflagtable.bcsv:
  "Name"       | The name of this entry.
  "Type"       | The entry type.
  "SaveFlag"   | If this entry should NOT be saved to the save file.
  "Condition1" | 1st parameter (LONG).
  "Condition2" | 2st parameter (LONG).
  "Condition3" | 3st parameter (STRINGOFF).
  "Condition4" | 4st parameter (STRINGOFF).

  Types:
  - "None"           (0) | This flag can be set to true or false regardless of parameters.
  - "StarMin"        (1) | True if the current power star count is greater or equal to Condition1.
  - "StarMinStage"   (2) | True if the current power star count is greater or equal to the amount of stars required to unlock a galaxy (Condition3).
  - "HasStar"        (3) | True if a star (of ID Condition1) inside a galaxy (Condition3) is collected.
  - "FlagAndFlag"    (4) | True if the GameEventFlags defined in Condition3 and Condition4 are true.
  - "StoryEvent"     (5) | True if the player has passed a story event (Condition3). Never used in the vanilla game.
  - "StageUnlock"    (6) | True if the player can unlock a galaxy (Condition3).
  - "CometUnlock"    (7) | TODO - Research.
  - "TicoGalaxy"     (8) | True if the player has fed a TicoGalaxy (ID = Condition1) a minumum of X starbits (X = Condition2 * 10) in a galaxy (Condition3).
  - "FlagAndValue"   (9) | True if the GameEventFlag defined in Condition3 is true and the GameEventValue defined in Condition4 is equal to 0.
  - "CompleteAll"   (10) | True if the player has seen both game endings. Only used for ViewCompleteMarioAndLuigi.
  - "EventValue"    (11) | True if the specified GameEventValue (Condition3) is non-zero.
  - "TicoCoin"      (12) | True if a comet medal (of ID Condition1) inside a galaxy (Condition3) is collected.

*/

namespace {
    const char* cTypeNameTable[] = {
        "None",         // 0
        "StarMin",      // 1
        "StarMinStage", // 2
        "HasStar",      // 3
        "FlagAndFlag",  // 4
        "StoryEvent",   // 5
        "StageUnlock",  // 6
        "CometUnlock",  // 7
        "TicoGalaxy",   // 8
        "FlagAndValue", // 9
        "CompleteAll",  // 10
        "EventValue",   // 11
        "TicoCoin",     // 12 (Custom)
    };
}

GameEventTable::GameEventTable() {
    JKRArchive *pArchive = TK::receiveArchive("/ObjectData/GameEventTable.arc");

    initFlagTable(pArchive->getResource("gameeventflagtable.bcsv"));
    initValueTable(pArchive->getResource("gameeventvaluetable.bcsv"));
    initStoryEventTable(pArchive->getResource("gamestoryeventtable.bcsv"));
}

void GameEventTable::initFlagTable(void *pResource) {
    JMapInfo info;
    info.attach(pResource);

    mFlagTable.init(info.getNumEntries());

    s32 nameInfoIdx = info.searchItemInfo("Name");
    s32 typeInfoIdx = info.searchItemInfo("Type");
    s32 saveFlagInfoIdx = info.searchItemInfo("SaveFlag");
    s32 condition1InfoIdx = info.searchItemInfo("Condition1");
    s32 condition2InfoIdx = info.searchItemInfo("Condition2");
    s32 condition3InfoIdx = info.searchItemInfo("Condition3");
    s32 condition4InfoIdx = info.searchItemInfo("Condition4");

    for (s32 i = 0; i < mFlagTable.mMaxSize; i++) {
        GameEventFlag *pFlag = &mFlagTable[i];
        
        u8 type = -1;
        const char *pTypeName;

        info.getValueFast(i, nameInfoIdx, &pFlag->mName);
        info.getValueFast(i, typeInfoIdx, &pTypeName);
        TK::getValueFast(&info, i, saveFlagInfoIdx, &pFlag->mSaveFlag);
        TK::getValueFast(&info, i, condition1InfoIdx, &pFlag->mCondition1);
        TK::getValueFast(&info, i, condition2InfoIdx, &pFlag->mCondition2);
        TK::getStringFastOrNull(&info, i, condition3InfoIdx, &pFlag->mCondition3);
        TK::getStringFastOrNull(&info, i, condition4InfoIdx, &pFlag->mCondition4);

        for (s32 i = 0; i < LIST_COUNT(cTypeNameTable); i++) {
            if (MR::isEqualString(pTypeName, cTypeNameTable[i])) {
                type = i;
                break;
            }
        }

        if (type == -1) {
            if (MR::isEqualString(pTypeName, "StarPiece")) {
                type = 8;
            }
            else if (MR::isEqualString(pTypeName, "EventValueIsZero")) {
                type = 9;
            }
            else if (MR::isEqualString(pTypeName, "FlagAndStoryEvent")) {
                type = 11;
            }
            else {
                OSReport("GameEventTable : Invalid event flag type '%s'\n", pTypeName);
                type = 0;
            }
        }

        pFlag->mType = type;
    }

    OSDebugF("GameEventTable : Loaded all flag entries (%d)\n", mFlagTable.mMaxSize);
}

void GameEventTable::initValueTable(void *pResource) {
    JMapInfo info;
    info.attach(pResource);

    mValueTable.init(info.getNumEntries());

    s32 nameInfoIdx = info.searchItemInfo("Name");
    s32 valueInfoIdx = info.searchItemInfo("Value");

    for (s32 i = 0; i < mValueTable.mMaxSize; i++) {
        GameEventValue *pValue = &mValueTable[i];

        TK::getStringFastOrNull(&info, i, nameInfoIdx, &pValue->mName);
        TK::getValueFast(&info, i, valueInfoIdx, &pValue->mDefaultValue);
    }

    OSDebugF("GameEventTable : Loaded all value entries (%d)\n", mValueTable.mMaxSize);
}

void GameEventTable::initStoryEventTable(void *pResource) {
    mStoryEventTableData = pResource;
    OSDebug("GameEventTable : Loaded story event entries\n");
}

const GameEventFlag* GameEventTable::getFlag(int idx) const {
    if (idx < 0 || idx >= mFlagTable.mMaxSize) {
        return nullptr;
    }

    return &mFlagTable[idx];
}

int GameEventTable::getFlagIndex(const GameEventFlag *pFlag) const {
    return pFlag - mFlagTable.mArr;
}

namespace {
    void initFlagSortTable(GameEventFlagTableInstance *pTableInstance) {
        pTableInstance->mFlagNum = TK::getSingleton<GameEventTable>()->mFlagTable.mMaxSize;
        pTableInstance->initSortTable();
    }

    int getFlagTableSize() {
        return TK::getSingleton<GameEventTable>()->mFlagTable.mMaxSize;
    }

    int getFlagSortTableSize() {
        return TK::getSingleton<GameEventTable>()->mFlagTable.mMaxSize * sizeof(GameEventFlagTableInstance::Key);
    }

    const GameEventFlag* getFlag(int idx) {
        return TK::getSingleton<GameEventTable>()->getFlag(idx);
    }

    int getFlagIndex(const GameEventFlag *pFlag) {
        return TK::getSingleton<GameEventTable>()->getFlagIndex(pFlag);
    }

    bool extFlagCanOn() {
        register GameEventFlagChecker *pChecker;
        register GameEventFlag *pFlag;
        register u8 type;

        GET_REGISTER(pChecker, r29);
        GET_REGISTER(pFlag, r30);
        GET_REGISTER(type, r0);

        ExtGameDataHolder *pHolder = (ExtGameDataHolder *)pChecker->mDataHolder;

        if (type == 12) {
            return pHolder->hasTicoCoin(pFlag->mCondition3, pFlag->mCondition1);
        }

        return false;
    }

    bool extFlagTryOn(GameEventFlagStorage *pStorage, const GameEventFlag *pFlag, bool value) {
        if (pFlag->mType == 12) {
            return false;
        }

        pStorage->set(pFlag, value);
        return true;
    }

    GameEventValue* getValueTable() {
        return TK::getSingleton<GameEventTable>()->mValueTable.mArr;
    }

    u32 getValueBufferSize(GameEventValueChecker *pChecker) {
        GameEventTable *pTable = TK::getSingleton<GameEventTable>();
        return (pChecker->mValueNum = pTable->mValueTable.mMaxSize) * sizeof(u16);
    }

    void initializeValueData(GameEventValueChecker *pChecker) {
        GameEventValue *pTable = TK::getSingleton<GameEventTable>()->mValueTable.mArr;

        for (s32 i = 0; i < pChecker->mValueNum; i++) {
            pChecker->mValueTable[i] = pTable[i].mDefaultValue;
        }
    }

    void attachStoryTable(JMapInfo *pInfo) {
        pInfo->attach(TK::getSingleton<GameEventTable>()->mStoryEventTableData);
    }
}

extern kmSymbol __ct__26GameEventFlagTableInstanceFv;
kmCall(&__ct__26GameEventFlagTableInstanceFv + 0x20, initFlagSortTable);

extern kmSymbol getTableSize__18GameEventFlagTableFv;
kmBranch(&getTableSize__18GameEventFlagTableFv, getFlagTableSize);

extern kmSymbol initSortTable__26GameEventFlagTableInstanceFv;
kmCall(&initSortTable__26GameEventFlagTableInstanceFv + 0x18, getFlagSortTableSize);
kmWrite32(&initSortTable__26GameEventFlagTableInstanceFv + 0x20, PPC_NOP);
kmWrite32(&initSortTable__26GameEventFlagTableInstanceFv + 0x30, PPC_NOP);

extern kmSymbol getFlag__18GameEventFlagTableFi;
kmBranch(&getFlag__18GameEventFlagTableFi, getFlag);

extern kmSymbol getIndex__18GameEventFlagTableFPC13GameEventFlag;
kmBranch(&getIndex__18GameEventFlagTableFPC13GameEventFlag, getFlagIndex);

extern kmSymbol getIndexFromHashCode__18GameEventFlagTableFUs;
kmCall(&getIndexFromHashCode__18GameEventFlagTableFUs + 0x68, getFlagIndex);
kmWrite32(&getIndexFromHashCode__18GameEventFlagTableFUs + 0x6C, PPC_B(0x44));

extern kmSymbol canOn__20GameEventFlagCheckerCFPCc;
kmCall(&canOn__20GameEventFlagCheckerCFPCc + 0x1EC, extFlagCanOn);

extern kmSymbol tryOn__20GameEventFlagCheckerFPCc;
kmCall(&tryOn__20GameEventFlagCheckerFPCc + 0x84, extFlagTryOn);
kmWrite32(&tryOn__20GameEventFlagCheckerFPCc + 0x88, PPC_NOP);

extern kmSymbol __ct__21GameEventValueCheckerFv;
kmCall(&__ct__21GameEventValueCheckerFv + 0x2C, getValueBufferSize);
kmWrite32(&__ct__21GameEventValueCheckerFv + 0x40, PPC_NOP);

extern kmSymbol serialize__21GameEventValueCheckerCFPUcUl;
kmCall(&serialize__21GameEventValueCheckerCFPUcUl + 0x34, getValueTable);
kmWrite32(&serialize__21GameEventValueCheckerCFPUcUl + 0x3C, PPC_MR(29, 3)); // mr r29, r3

extern kmSymbol initializeData__21GameEventValueCheckerFv;
kmBranch(&initializeData__21GameEventValueCheckerFv, initializeValueData);

extern kmSymbol findIndex__21GameEventValueCheckerCFPCc;
kmWrite32(&findIndex__21GameEventValueCheckerCFPCc + 0x14, PPC_MR(27, 3)); // mr r27, r3
kmWrite32(&findIndex__21GameEventValueCheckerCFPCc + 0x18, PPC_MR(28, 4)); // mr r28, r4
kmWrite32(&findIndex__21GameEventValueCheckerCFPCc + 0x1C, PPC_LI(29, 0)); // li r29, 0
kmCall(&findIndex__21GameEventValueCheckerCFPCc + 0x20, getValueTable);
kmWrite32(&findIndex__21GameEventValueCheckerCFPCc + 0x24, PPC_MR(30, 3)); // mr r30, r3

extern kmSymbol findIndexFromHashCode__21GameEventValueCheckerCFUs;
kmWrite32(&findIndexFromHashCode__21GameEventValueCheckerCFUs + 0x14, PPC_MR(27, 3)); // mr r27, r3
kmWrite32(&findIndexFromHashCode__21GameEventValueCheckerCFUs + 0x18, PPC_MR(28, 4)); // mr r28, r4
kmWrite32(&findIndexFromHashCode__21GameEventValueCheckerCFUs + 0x1C, PPC_LI(29, 0)); // li r29, 0
kmCall(&findIndexFromHashCode__21GameEventValueCheckerCFUs + 0x20, getValueTable);
kmWrite32(&findIndexFromHashCode__21GameEventValueCheckerCFUs + 0x24, PPC_MR(30, 3)); // mr r30, r3

extern kmSymbol __ct__14GameDataHolderFPC8UserFile;
kmCall(&__ct__14GameDataHolderFPC8UserFile + 0x110, attachStoryTable);