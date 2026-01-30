#include "System/StageHotswapHolder.hpp"
#include "Extended/SceneObjHolder.hpp"
#include "Util/GeneralUtil.hpp"

#include "Game/Scene/SceneFunction.hpp"
#include "Game/System/FileRipper.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/JKernel/JKRSolidHeap.hpp"

StageHotswapHolder::StageHotswapHolder(const char *pName) : NameObj(pName) {
    mArchive = nullptr;
    mCommonTable = nullptr;
    mScenarioTable = nullptr;
}

StageHotswapHolder::~StageHotswapHolder() {
    mArchive = nullptr;

    if (mCommonTable) {
        delete mCommonTable;
        mCommonTable = nullptr;
    }

    if (mScenarioTable) {
        delete mScenarioTable;
        mScenarioTable = nullptr;
    }
}

void StageHotswapHolder::initCommon() {
    const char *pStageName = MR::getCurrentStageName();

    char name[0x200];
    snprintf(name, sizeof(name), "/StageData/%s/%sSwap.arc", pStageName, pStageName);

    if (MR::isFileExist(name, false)) {
        mArchive = TK::mountArchive(name, TK::getFileCacheHeap());
        mCommonTable = parseEntries(mArchive->getResource("common.bcsv"));
    }
}

void StageHotswapHolder::initScenario() {
    if (!mArchive) {
        return;
    }

    char name[0x40];
    snprintf(name, sizeof(name), "scenario%d.bcsv", MR::getCurrentSelectedScenarioNo());
    mScenarioTable = parseEntries(mArchive->getResource(name));
}

const char* StageHotswapHolder::findSwap(const char *pName) {
    if (!MR::isExistSceneObj(ExtSceneObj_StageHotswapHolder)) {
        return pName;
    }

    StageHotswapHolder *pHolder = MR::getSceneObj<StageHotswapHolder>(ExtSceneObj_StageHotswapHolder);
    const char *pSwapName;

    if (pSwapName = findSwap(pName, pHolder->mScenarioTable)) {
        return pSwapName;
    }

    if (pSwapName = findSwap(pName, pHolder->mCommonTable)) {
        return pSwapName;
    }

    return pName;
}

const char* StageHotswapHolder::findSwap(const char *pName, MR::AssignableArray<HotswapEntry> *pTable) {
    if (!pTable) {
        return nullptr;
    }

    bool startWithSlash = pName[0] == '/';

    for (HotswapEntry *pEntry = pTable->begin(); pEntry != pTable->end(); pEntry++) {
        const char *pFile = pEntry->mFileName;
        const char *pMask = pEntry->mMaskName;

        if (pFile[0] == '/' && !startWithSlash) {
            pFile++;
        }

        if (MR::isEqualString(pName, pFile)) {
            return pMask;
        }
    }

    return nullptr;
}

MR::AssignableArray<HotswapEntry>* StageHotswapHolder::parseEntries(void *pResource) {
    JMapInfo info;

    if (pResource && info.attach(pResource)) {
        MR::AssignableArray<HotswapEntry> *pArray = new MR::AssignableArray<HotswapEntry>(info.getNumEntries());

        for (s32 i = 0; i < pArray->mMaxSize; i++) {
            HotswapEntry *pEntry = &pArray->mArr[i];
            MR::getCsvDataStrOrNULL(&pEntry->mFileName, &info, "FileName", i);
            MR::getCsvDataStrOrNULL(&pEntry->mMaskName, &info, "MaskName", i);
        }

        return pArray;
    }

    return nullptr;
}

namespace {
    void initHotswapCommon() {
        ((StageHotswapHolder *)MR::createSceneObj(ExtSceneObj_StageHotswapHolder))->initCommon();
        SceneFunction::startStageFileLoad();
    }

    void initHotswapScenario() {
        MR::getSceneObj<StageHotswapHolder>(ExtSceneObj_StageHotswapHolder)->initScenario();
        SceneFunction::initAfterScenarioSelected();
    }

    void* loadToMainRamHotswap(const char *pName, u8 *a2, bool a3, JKRHeap *pHeap, FileRipper::AllocDirection dir) {
        return FileRipper::loadToMainRAM(StageHotswapHolder::findSwap(pName), a2, a3, pHeap, dir);
    }
}

extern kmSymbol init__9GameSceneFv;
kmCall(&init__9GameSceneFv + 0x18, initHotswapCommon);
kmCall(&init__9GameSceneFv + 0x128, initHotswapScenario);

extern kmSymbol loadFileUsingRipper__30_unnamed_FileLoaderThread_cpp_FP15RequestFileInfo;
kmBranch(&loadFileUsingRipper__30_unnamed_FileLoaderThread_cpp_FP15RequestFileInfo + 0x28, loadToMainRamHotswap);
