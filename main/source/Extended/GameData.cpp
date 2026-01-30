#include "Extended/GameData.hpp"
#include "Extended/GalaxyStatusAccessor.hpp"
#include "Util/GeneralUtil.hpp"
#include "Util/TicoCoinUtil.hpp"

#include "Local/Game/System/GameEventFlagChecker.hpp"
#include "Game/System/BinaryDataContentAccessor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameSequenceDirector.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/GameSystem.hpp"

// --------- ExtGameDataHolder --------- //
bool ExtGameDataHolder::hasTicoCoin(const char *pStageName, s32 id) const {
    ExtGameDataSomeGalaxyStorage *pStorage = getExtAllGalaxyStorage()->findStorage(pStageName);

    if (pStorage) {
        return (pStorage->mTicoCoinFlag >> id) & 1;
    }

    return false;
}

void ExtGameDataHolder::setTicoCoinOnFlag(const char *pStageName, u8 flag) {
    ExtGameDataSomeGalaxyStorage *pStorage = getExtAllGalaxyStorage()->findStorage(pStageName);

    if (pStorage) {
        pStorage->mTicoCoinFlag |= flag;
    }
}

u8 ExtGameDataHolder::getTicoCoinFlag(const char *pStageName) const {
    ExtGameDataSomeGalaxyStorage *pStorage = getExtAllGalaxyStorage()->findStorage(pStageName);

    if (pStorage) {
        return pStorage->mTicoCoinFlag;
    }

    return 0;
}

s32 ExtGameDataHolder::getTicoCoinNumOwned(const char *pStageName) const {
    return getExtAllGalaxyStorage()->getTicoCoinNumOwned(pStageName);
}

// --------- ExtGameDataSomeGalaxyStorage --------- //
s32 ExtGameDataSomeGalaxyStorage::getTicoCoinNumOwned() const {
    return TK::getOnBitNum(mTicoCoinFlag);
}

// --------- ExtGameDataAllGalaxyStorage --------- //
ExtGameDataSomeGalaxyStorage* ExtGameDataAllGalaxyStorage::findStorage(const char *pStageName) const {
    int i = findIndex(pStageName);

    if (i >= 0) {
        return (ExtGameDataSomeGalaxyStorage *)mGalaxyStorage[i];
    }

    return nullptr;
}

s32 ExtGameDataAllGalaxyStorage::calcCurrentTicoCoinNum() const {
    s32 num = 0;

    for (s32 i = 0; i < mGalaxyStorageNum; i++) {
        ExtGameDataSomeGalaxyStorage *pStorage = (ExtGameDataSomeGalaxyStorage *)mGalaxyStorage[i];
        num += pStorage->getTicoCoinNumOwned();
    }

    return num;
}

s32 ExtGameDataAllGalaxyStorage::getTicoCoinNumOwned(const char *pStageName) const {
    ExtGameDataSomeGalaxyStorage *pStorage = findStorage(pStageName);

    if (pStorage) {
        return pStorage->getTicoCoinNumOwned();
    }

    return 0;
}

// --------- ExtGameDataTemporaryInGalaxy --------- //
bool ExtGameDataTemporaryInGalaxy::hasTicoCoin(s32 id) {
    return (mTicoCoinFlag >> id) & 1;
}

void ExtGameDataTemporaryInGalaxy::setTicoCoin(s32 id) {
    mTicoCoinFlag |= (1 << id);
}

void ExtGameDataTemporaryInGalaxy::resetStageResultTicoCoinParam() {
    mTicoCoinFlag = 0;
}

// --------- Utilities --------- //
namespace TK {
    ExtGameDataTemporaryInGalaxy* getGameDataTemporaryInGalaxy() {
        GameSystem *pSystem = SingletonHolder<GameSystem>::get();
        return (ExtGameDataTemporaryInGalaxy *)pSystem->mSequenceDirector->mGameDataTemporaryInGalaxy;
    }
}

// --------- Hooks --------- //
namespace {
    void ctorGalaxyStorage(ExtGameDataSomeGalaxyStorage *pStorage) {
        register ExtGalaxyStatusAccessor *pAccessor;
        GET_REGISTER(pAccessor, r31);

        pStorage->mTicoCoinNum = pAccessor->getTicoCoinNum();
        pStorage->resetAllData();
    }

    void initGalaxyStorageHeaderSerializer(BinaryDataContentHeaderSerializer *pSerializer) {
        pSerializer->addAttribute("mTicoCoinFlag", 1);
        pSerializer->flush();
    }

    void resetAllGalaxyStorageData(ExtGameDataSomeGalaxyStorage *pStorage) {
        pStorage->mTicoCoinFlag = 0;
    }

    void* serializeGalaxyStorage(BinaryDataContentAccessor *pAccessor, const char *pAttributeName, u8 *pOffset) {
        register ExtGameDataSomeGalaxyStorage *pStorage;
        GET_REGISTER(pStorage, r28);

        *(u8 *)pAccessor->getPointer("mTicoCoinFlag", pOffset) = pStorage->mTicoCoinFlag;
        return pAccessor->getPointer(pAttributeName, pOffset);
    }

    void* deserializeGalaxyStorage(BinaryDataContentAccessor *pAccessor, const char *pAttributeName, u8 *pOffset) {
        register ExtGameDataSomeGalaxyStorage *pStorage;
        GET_REGISTER(pStorage, r26);

        u8 *pTicoCoinFlag = (u8 *)pAccessor->getPointer("mTicoCoinFlag", pOffset);

        if (pTicoCoinFlag) {
            pStorage->mTicoCoinFlag = *pTicoCoinFlag;
        }

        return pAccessor->getPointer(pAttributeName, pOffset);
    }

    void resetGameDataGoToGalaxyFirst(ExtGameDataTemporaryInGalaxy *pGameDataTemporary) {
        pGameDataTemporary->resetStageResultCoinParam();
        pGameDataTemporary->resetStageResultTicoCoinParam();
    }

    void resetStageResultParam(ExtGameDataTemporaryInGalaxy *pGameDataTemporary) {
        pGameDataTemporary->resetStageResultPowerStarParam();
        pGameDataTemporary->resetStageResultTicoCoinParam();
    }

    void receiveStageResultParam(ExtGameDataTemporaryInGalaxy *pGameDataTemporary, const char *pStageName, s32 scenarioNo, int starPieceNum, int coinNum) {
        u32 ticoCoinFlag = pGameDataTemporary->mTicoCoinFlag;
        pGameDataTemporary->receiveStageResultParam(pStageName, scenarioNo, starPieceNum, coinNum);
        pGameDataTemporary->mTicoCoinFlag = ticoCoinFlag;
    }

    void updateGameDataBeforeChangeScene() {
        GameSequenceFunction::reflectStageResultSequenceCoin();
        TicoCoinUtil::updateStageResultTicoCoin();
    }
}

extern kmSymbol __ct__24GameDataAllGalaxyStorageFv;
kmWrite16(&__ct__24GameDataAllGalaxyStorageFv + 0xBE, sizeof(ExtGameDataSomeGalaxyStorage));

extern kmSymbol __ct__25GameDataSomeGalaxyStorageFRC20GalaxyStatusAccessor;
kmCall(&__ct__25GameDataSomeGalaxyStorageFRC20GalaxyStatusAccessor + 0x38, ctorGalaxyStorage);

extern kmSymbol initHeaderSerializer__24GameDataAllGalaxyStorageFv;
kmCall(&initHeaderSerializer__24GameDataAllGalaxyStorageFv + 0x64, initGalaxyStorageHeaderSerializer);

extern kmSymbol resetAllData__25GameDataSomeGalaxyStorageFv;
kmBranch(&resetAllData__25GameDataSomeGalaxyStorageFv + 0x30, resetAllGalaxyStorageData);

extern kmSymbol serialize__25GameDataSomeGalaxyStorageCFRC25BinaryDataContentAccessorPUc;
kmCall(&serialize__25GameDataSomeGalaxyStorageCFRC25BinaryDataContentAccessorPUc + 0x30, serializeGalaxyStorage);

extern kmSymbol deserialize__25GameDataSomeGalaxyStorageFRC25BinaryDataContentAccessorPCUc;
kmCall(&deserialize__25GameDataSomeGalaxyStorageFRC25BinaryDataContentAccessorPCUc + 0x30, deserializeGalaxyStorage);

extern kmSymbol __ct__20GameSequenceDirectorFv;
kmWrite16(&__ct__20GameSequenceDirectorFv + 0xD6, sizeof(ExtGameDataTemporaryInGalaxy));

extern kmSymbol resetGameDataGoToGalaxyFirst__16GameDataFunctionFv;
kmCall(&resetGameDataGoToGalaxyFirst__16GameDataFunctionFv + 0x6C, resetGameDataGoToGalaxyFirst);

extern kmSymbol resetStageResultParam__25GameDataTemporaryInGalaxyFv;
kmCall(&resetStageResultParam__25GameDataTemporaryInGalaxyFv + 0x14, resetStageResultParam);

extern kmSymbol sendStageResultSequenceParam__20GameSequenceFunctionFPCclii;
kmBranch(&sendStageResultSequenceParam__20GameSequenceFunctionFPCclii + 0x24, receiveStageResultParam);

extern kmSymbol updateGameDataBeforeChangeScene__20GameSequenceProgressFv;
kmCall(&updateGameDataBeforeChangeScene__20GameSequenceProgressFv + 0x30, updateGameDataBeforeChangeScene);
