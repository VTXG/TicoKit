#include "System/StageAudioTable.hpp"
#include "Util/GeneralUtil.hpp"

#include "Local/Game/GameAudio/AudStageBgmWrap.hpp"
#include "Local/JSystem/JAudio2/JAUSectionHeap.hpp"
#include "Game/AudioLib/AudSoundNameConverter.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "JSystem/JKernel/JKRExpHeap.hpp"

StageAudioTable::StageAudioTable() {
    JKRArchive *pArchive = TK::mountArchive("/AudioRes/Info/StageAudioTable.arc", nullptr);

    initStageBgmTable(pArchive->getResource("stagebgmtable.bcsv"));
    initScenarioBgmTable(pArchive->getResource("scenariobgmtable.bcsv"));
    initMultiBgmTable(pArchive->getResource("multibgmtable.bcsv"));
    initWaveSetTable(pArchive->getResource("wavesettable.bcsv"));
}

void StageAudioTable::initStageBgmTable(void *pResource) {
    JMapInfo info;
    info.attach(pResource);

    mStageBgmTable.init(info.getNumEntries());

    s32 jmpIdxStageName = info.searchItemInfo("StageName");

    for (s32 i = 0; i < mStageBgmTable.mMaxSize; i++) {
        StageBgmTableEntry *pEntry = &mStageBgmTable[i];
        const char *pName;

        TK::getStringFastOrNull(&info, i, jmpIdxStageName, &pName);
        pEntry->mStageNameHash = MR::getHashCode(pName);

        for (s32 j = 0; j < 6; j++) {
            char jmpNameChangeBgmIdName[32];
            char jmpNameChangeBgmState[32];
            const char *pBgmIdName;

            snprintf(jmpNameChangeBgmIdName, sizeof(jmpNameChangeBgmIdName), "ChangeBgmIdName%d", j);
            snprintf(jmpNameChangeBgmState, sizeof(jmpNameChangeBgmState), "ChangeBgmState%d", j);

            MR::getCsvDataStrOrNULL(&pBgmIdName, &info, jmpNameChangeBgmIdName, i);
            MR::getCsvDataU8(&pEntry->mChangeBgmState[j], &info, jmpNameChangeBgmState, i);

            pEntry->mChangeBgmId[j] = getSoundId(pBgmIdName);
        }
    }

    OSDebugF("StageAudioTable : Loaded all stage entries (%d)\n", mStageBgmTable.mMaxSize);
}

void StageAudioTable::initScenarioBgmTable(void *pResource) {
    JMapInfo info;
    info.attach(pResource);

    mScenarioBgmTable.init(info.getNumEntries());

    s32 jmpIdxStageName = info.searchItemInfo("StageName");
    s32 jmpIdxScenarioNo = info.searchItemInfo("ScenarioNo");
    s32 jmpIdxBgmIdName = info.searchItemInfo("BgmIdName");
    s32 jmpIdxStartType = info.searchItemInfo("StartType");

    for (s32 i = 0; i < mScenarioBgmTable.mMaxSize; i++) {
        ScenarioBgmTableEntry *pEntry = &mScenarioBgmTable[i];
        const char *pName;
        const char *pBgmIdName;

        TK::getStringFastOrNull(&info, i, jmpIdxStageName, &pName);
        info.getValueFast(i, jmpIdxScenarioNo, &pEntry->mScenarioNo);
        TK::getStringFastOrNull(&info, i, jmpIdxBgmIdName, &pBgmIdName);
        TK::getValueFast(&info, i, jmpIdxStartType, &pEntry->mStartType);

        pEntry->mStageNameHash = MR::getHashCode(pName);
        pEntry->mBgmId = getSoundId(pBgmIdName);
    }

    OSDebugF("StageAudioTable : Loaded all scenario entries (%d)\n", mScenarioBgmTable.mMaxSize);
}

void StageAudioTable::initMultiBgmTable(void *pResource) {
    JMapInfo info;
    info.attach(pResource);

    mMultiBgmTable.init(info.getNumEntries());

    s32 jmpIdxMultiBgmName = info.searchItemInfo("MultiBgmName");
    s32 jmpIdxBgmName = info.searchItemInfo("BgmName");
    s32 jmpIdxStreamName = info.searchItemInfo("StreamName");
    s32 jmpIdxBeatRatio = info.searchItemInfo("BeatRatio");
    s32 jmpIdxIntroBeat = info.searchItemInfo("IntroBeat");
    s32 jmpIdxLoopBeat = info.searchItemInfo("LoopBeat");
    s32 jmpIdxLoopStartSample = info.searchItemInfo("LoopStartSample");
    s32 jmpIdxLoopEndSample = info.searchItemInfo("LoopEndSample");

    for (s32 i = 0; i < mMultiBgmTable.mMaxSize; i++) {
        MultiBgmTableEntry *pEntry = &mMultiBgmTable[i];
        const char *pMultiBgmName;
        const char *pBgmName;
        const char *pStreamName;

        TK::getStringFastOrNull(&info, i, jmpIdxMultiBgmName, &pMultiBgmName);
        TK::getStringFastOrNull(&info, i, jmpIdxBgmName, &pBgmName);
        TK::getStringFastOrNull(&info, i, jmpIdxStreamName, &pStreamName);
        info.getValueFast(i, jmpIdxBeatRatio, &pEntry->mBeatRatio);
        info.getValueFast(i, jmpIdxIntroBeat, &pEntry->mIntroBeat);
        info.getValueFast(i, jmpIdxLoopBeat, &pEntry->mLoopBeat);
        info.getValueFast(i, jmpIdxLoopStartSample, &pEntry->mLoopStartSample);
        info.getValueFast(i, jmpIdxLoopEndSample, &pEntry->mLoopEndSample);

        pEntry->mMultiBgmId = getSoundId(pMultiBgmName);
        pEntry->mBgmId = getSoundId(pBgmName);
        pEntry->mStreamId = getSoundId(pStreamName);
    }

    OSDebugF("StageAudioTable : Loaded all multi bgm entries (%d)\n", mMultiBgmTable.mMaxSize);
}

void StageAudioTable::initWaveSetTable(void *pResource) {
    JMapInfo info;
    info.attach(pResource);

    mWaveSetTable.init(info.getNumEntries());

    s32 jmpIdxStageName = info.searchItemInfo("StageName");
    s32 jmpIdxScenarioNo = info.searchItemInfo("ScenarioNo");

    for (s32 i = 0; i < mWaveSetTable.mMaxSize; i++) {
        WaveSetTableEntry *pEntry = &mWaveSetTable[i];

        const char *pName;
        TK::getStringFastOrNull(&info, i, jmpIdxStageName, &pName);
        info.getValueFast(i, jmpIdxScenarioNo, &pEntry->mScenarioNo);

        pEntry->mStageNameHash = MR::getHashCode(pName);

        for (s32 j = 0; j < 4; j++) {
            char jmpNameBgmWaveId[32];
            snprintf(jmpNameBgmWaveId, sizeof(jmpNameBgmWaveId), "BgmWaveId%d", j);

            if (j < 2) {
                TK::getCsvDataS8(&pEntry->mBgmWaves[j], &info, jmpNameBgmWaveId, i);
            }

            TK::getCsvDataS8(&pEntry->mWaves[j], &info, jmpNameBgmWaveId + 3 /* "Bgm" */, i);
        }
    }

    OSDebugF("StageAudioTable : Loaded all wave sets (%d)\n", mWaveSetTable.mMaxSize);
}

const StageBgmTableEntry* StageAudioTable::getStageBgmEntry(const char *pGalaxyName) const {
    u32 hash = MR::getHashCode(pGalaxyName);

    for (const StageBgmTableEntry *pEntry = mStageBgmTable.begin(); pEntry != mStageBgmTable.end(); pEntry++) {
        if (pEntry->mStageNameHash == hash) {
            return pEntry;
        }
    }

    return nullptr;
}

const ScenarioBgmTableEntry* StageAudioTable::getScenarioBgmEntry(const char *pGalaxyName, s32 scenarioNo) const {
    u32 hash = MR::getHashCode(pGalaxyName);
    const ScenarioBgmTableEntry *pDefaultEntry = nullptr;

    for (const ScenarioBgmTableEntry *pEntry = mScenarioBgmTable.begin(); pEntry != mScenarioBgmTable.end(); pEntry++) {
        if (pEntry->mStageNameHash == hash) {
            if (pEntry->mScenarioNo == scenarioNo) {
                return pEntry;
            }

            if (pEntry->mScenarioNo == 0) {
                pDefaultEntry = pEntry;
            }
        }
    }

    return pDefaultEntry;
}

const MultiBgmTableEntry* StageAudioTable::getMultiBgmEntry(u32 id) const {
    for (const MultiBgmTableEntry *pEntry = mMultiBgmTable.begin(); pEntry != mMultiBgmTable.end(); pEntry++) {
        if ((pEntry->mMultiBgmId & 0xFEFEFFFF) == id) {
            return pEntry;
        }
    }

    return nullptr;
}

const WaveSetTableEntry* StageAudioTable::getStageWaveSetEntry(const char *pGalaxyName) const {
    return getScenarioWaveSetEntry(pGalaxyName, 0);
}

const WaveSetTableEntry* StageAudioTable::getScenarioWaveSetEntry(const char *pGalaxyName, s32 scenarioNo) const {
    u32 hash = MR::getHashCode(pGalaxyName);

    for (const WaveSetTableEntry *pEntry = mWaveSetTable.begin(); pEntry != mWaveSetTable.end(); pEntry++) {
        if (pEntry->mStageNameHash == hash && pEntry->mScenarioNo == scenarioNo) {
            return pEntry;
        }
    }

    return nullptr;
}

s32 StageAudioTable::getSoundId(const char *pLabel) {
    if (!pLabel) {
        return -1;
    }

    return AudSingletonHolder<AudSoundNameConverter>::get()->getSoundID(pLabel).mID;
}

void ExtAudSceneMgr::loadStageWaveSet(const char *pSceneName, const char *pStageName) {
    StageAudioTable *pStageAudioTable = TK::getSingleton<StageAudioTable>();
    const WaveSetTableEntry *pEntry = pStageAudioTable->getStageWaveSetEntry(pStageName);

    eraseLastSeScenarioWaveSet();

    if (mPlayerType != mPrevPlayerType) {
        mSection->eraseWaveArc(0x22, 2);
        mSection->eraseWaveArc(0x22, 4);
        loadPlayerResource();

        mPrevPlayerType = mPlayerType;
    }

    if (pEntry != mCurrentStageEntry) {
        eraseLastBgmWaveSet();
        eraseLastSeWaveSet();

        if (pEntry) {
            loadWaveSet(pEntry->mWaves, 4);
            loadWaveSet(pEntry->mBgmWaves, 2);
        }
    }

    mCurrentStageEntry = pEntry;
}

void ExtAudSceneMgr::loadScenarioWaveSet(const char *pSceneName, const char *pStageName, s32 scenarioNo) {
    StageAudioTable *pStageAudioTable = TK::getSingleton<StageAudioTable>();
    const WaveSetTableEntry *pEntry = pStageAudioTable->getScenarioWaveSetEntry(pStageName, scenarioNo);

    if (pEntry) {
        loadWaveSet(pEntry->mWaves, 4);
    }

    mCurrentScenarioEntry = pEntry;
}

void ExtAudSceneMgr::eraseLastBgmWaveSet() {
    if (mCurrentStageEntry) {
        eraseWaveSet(mCurrentStageEntry->mBgmWaves, 2);
    }
}

void ExtAudSceneMgr::eraseLastSeWaveSet() {
    if (mCurrentStageEntry) {
        eraseWaveSet(mCurrentStageEntry->mWaves, 4);
    }
}

void ExtAudSceneMgr::eraseLastSeScenarioWaveSet() {
    if (mCurrentScenarioEntry) {
        eraseWaveSet(mCurrentScenarioEntry->mWaves, 4);
    }
}

void ExtAudSceneMgr::eraseWaveSet(const s8 *pWaves, s32 num) {
    for (s32 i = 0; i < num; i++) {
        s8 wave = pWaves[i];

        if (wave >= 0) {
            mSection->eraseWaveArc(wave);
        }
    }
}

bool ExtAudSceneMgr::isLoadDoneStageWaveSet() {
    if (mCurrentStageEntry) {
        for (s32 i = 0; i < 4; i++) {
            s8 wave = mCurrentStageEntry->mWaves[i];

            if (wave >= 0 && !mSection->isWaveLoaded(wave, 0)) {
                return false;
            }
        }

        for (s32 i = 0; i < 2; i++) {
            s8 wave = mCurrentStageEntry->mBgmWaves[i];

            if (wave >= 0 && !mSection->isWaveLoaded(wave, 0)) {
                return false;
            }
        }
    }

    return isPlayerResourceLoaded();
}

bool ExtAudSceneMgr::isLoadDoneScenarioWaveSet() {
    if (mCurrentScenarioEntry) {
        for (s32 i = 0; i < 4; i++) {
            s8 wave = mCurrentScenarioEntry->mWaves[i];

            if (wave >= 0 && !mSection->isWaveLoaded(wave, 0)) {
                return false;
            }
        }
    }

    return true;
}

namespace {
    void initAudio() {
        TK::initSingleton<StageAudioTable>();
    }

    void getStageBgmId() {
        register AudStageBgmWrap *pAudWrap;
        register const char *pGalaxyName;
        register s32 scenario;

        GET_REGISTER(pAudWrap, r30);
        GET_REGISTER(pGalaxyName, r25);
        GET_REGISTER(scenario, r31);

        pAudWrap->getCometEventBgm(pGalaxyName);
        const ScenarioBgmTableEntry *pEntry = TK::getSingleton<StageAudioTable>()->getScenarioBgmEntry(pGalaxyName, scenario);

        if (pEntry && (pAudWrap->mSoundId == -1 || pEntry->mScenarioNo != 0)) {
            pAudWrap->mSoundId = pEntry->mBgmId;
        }
    }

    s32 getStageBgmChangeId(char const *pGalaxyName) {
        register s32 idx;
        GET_REGISTER(idx, r31);

        const StageBgmTableEntry *pEntry = TK::getSingleton<StageAudioTable>()->getStageBgmEntry(pGalaxyName);
        return pEntry ? pEntry->mChangeBgmId[idx] : 0;
    }

    s32 getStageBgmChangeState(char const *pGalaxyName) {
        register s32 idx;
        GET_REGISTER(idx, r31);

        const StageBgmTableEntry *pEntry = TK::getSingleton<StageAudioTable>()->getStageBgmEntry(pGalaxyName);
        return pEntry ? pEntry->mChangeBgmState[idx] : 0;
    }

    bool isStartBgmOnWelcome() {
        const ScenarioBgmTableEntry *pEntry = TK::getSingleton<StageAudioTable>()->getScenarioBgmEntry(MR::getCurrentStageName(), MR::getCurrentScenarioNo());
        return pEntry ? pEntry->mStartType : false;
    }

    u32 getSeqIdForMBGM(u32 id) {
        const MultiBgmTableEntry *pEntry = TK::getSingleton<StageAudioTable>()->getMultiBgmEntry(id);
        return pEntry ? pEntry->mBgmId : 0;
    }

    u32 getStreamIdForMBGM(u32 id) {
        const MultiBgmTableEntry *pEntry = TK::getSingleton<StageAudioTable>()->getMultiBgmEntry(id);
        return pEntry ? pEntry->mStreamId : 0;
    }

    f32 getBeatMulForMBGM(u32 id) {
        const MultiBgmTableEntry *pEntry = TK::getSingleton<StageAudioTable>()->getMultiBgmEntry(id);
        return pEntry ? pEntry->mBeatRatio : 0.0f;
    }

    u32 getIntroBeatsForMBGM(u32 id) {
        const MultiBgmTableEntry *pEntry = TK::getSingleton<StageAudioTable>()->getMultiBgmEntry(id);
        return pEntry ? pEntry->mIntroBeat : 0;
    }

    u32 getLoopBeatsForMBGM(u32 id) {
        const MultiBgmTableEntry *pEntry = TK::getSingleton<StageAudioTable>()->getMultiBgmEntry(id);
        return pEntry ? pEntry->mLoopBeat : 0;
    }

    u32 getLoopStartSamplesForMBGM(u32 id) {
        const MultiBgmTableEntry *pEntry = TK::getSingleton<StageAudioTable>()->getMultiBgmEntry(id);
        return pEntry ? pEntry->mLoopEndSample : 0;
    }

    u32 getLoopEndSamplesForMBGM(u32 id) {
        const MultiBgmTableEntry *pEntry = TK::getSingleton<StageAudioTable>()->getMultiBgmEntry(id);
        return pEntry ? pEntry->mLoopEndSample : 0;
    }

    void loadStageWaveSet(ExtAudSceneMgr *pSceneMgr, const char *pSceneName, const char *pStageName) {
        pSceneMgr->loadStageWaveSet(pSceneName, pStageName);
    }

    void loadScenarioWaveSet(ExtAudSceneMgr *pSceneMgr, const char *pSceneName, const char *pStageName, s32 scenarioNo) {
        pSceneMgr->loadScenarioWaveSet(pSceneName, pStageName, scenarioNo);
    }

    bool isLoadDoneStageWaveSet(ExtAudSceneMgr *pSceneMgr) {
        return pSceneMgr->isLoadDoneStageWaveSet();
    }

    bool isLoadDoneScenarioWaveSet(ExtAudSceneMgr *pSceneMgr) {
        return pSceneMgr->isLoadDoneScenarioWaveSet();
    }
}

extern kmSymbol createAudioSystem__16AudSystemWrapperFv;
kmBranch(&createAudioSystem__16AudSystemWrapperFv + 0x9C, initAudio);

extern kmSymbol changeStageNameToSoundID__15AudStageBgmWrapFPCcPCcl;
kmWrite32(&changeStageNameToSoundID__15AudStageBgmWrapFPCcPCcl + 0x50, PPC_B(0x28));
kmCall(&changeStageNameToSoundID__15AudStageBgmWrapFPCcPCcl + 0xB4, getStageBgmId);
kmWrite32(&changeStageNameToSoundID__15AudStageBgmWrapFPCcPCcl + 0xB8, PPC_B(0x8C));

extern kmSymbol getBgmId__16AudStageBgmTableFPCcl;
kmCall(&getBgmId__16AudStageBgmTableFPCcl + 0x44, getStageBgmChangeId);
kmWrite32(&getBgmId__16AudStageBgmTableFPCcl + 0x48, PPC_B(0x1C));

extern kmSymbol getBgmState__16AudStageBgmTableFPCcl;
kmCall(&getBgmState__16AudStageBgmTableFPCcl + 0x14, getStageBgmChangeState);
kmWrite32(&getBgmState__16AudStageBgmTableFPCcl + 0x18, PPC_B(0x1C));

extern kmSymbol isStartBgmOnWelcome__15ScenarioStarterFv;
kmCall(&isStartBgmOnWelcome__15ScenarioStarterFv + 0x38, isStartBgmOnWelcome);
kmWrite32(&isStartBgmOnWelcome__15ScenarioStarterFv + 0x3C, PPC_B(0x6C));

extern kmSymbol getSeqIdForMultiBgm__13AudBgmSettingFUl;
kmBranch(&getSeqIdForMultiBgm__13AudBgmSettingFUl, getSeqIdForMBGM);

extern kmSymbol getStreamIdForMultiBgm__13AudBgmSettingFUl;
kmBranch(&getStreamIdForMultiBgm__13AudBgmSettingFUl, getStreamIdForMBGM);

extern kmSymbol getBeatMulForMultiBgm__13AudBgmSettingFUl;
kmBranch(&getBeatMulForMultiBgm__13AudBgmSettingFUl, getBeatMulForMBGM);

extern kmSymbol getIntroBeatsForMultiBgm__13AudBgmSettingFUl;
kmBranch(&getIntroBeatsForMultiBgm__13AudBgmSettingFUl, getIntroBeatsForMBGM);

extern kmSymbol getLoopBeatsForMultiBgm__13AudBgmSettingFUl;
kmBranch(&getLoopBeatsForMultiBgm__13AudBgmSettingFUl, getLoopBeatsForMBGM);

extern kmSymbol getLoopStartSamplesForMultiBgm__13AudBgmSettingFUl;
kmBranch(&getLoopStartSamplesForMultiBgm__13AudBgmSettingFUl, getLoopStartSamplesForMBGM);

extern kmSymbol getLoopEndSamplesForMultiBgm__13AudBgmSettingFUl;
kmBranch(&getLoopEndSamplesForMultiBgm__13AudBgmSettingFUl, getLoopEndSamplesForMBGM);

extern kmSymbol __ct__11AudSceneMgrFP14JAUSectionHeap;
kmWrite32(&__ct__11AudSceneMgrFP14JAUSectionHeap + 0x10, PPC_STW(5, 0xC, 3)); // stw r5, 0xC(r3)

extern kmSymbol loadStageResource__11AudSceneMgrFPCcPCc;
kmBranch(&loadStageResource__11AudSceneMgrFPCcPCc, loadStageWaveSet);

extern kmSymbol loadScenarioResource__11AudSceneMgrFPCcPCcl;
kmBranch(&loadScenarioResource__11AudSceneMgrFPCcPCcl, loadScenarioWaveSet);

extern kmSymbol isLoadDoneStageResource__11AudSceneMgrFv;
kmBranch(&isLoadDoneStageResource__11AudSceneMgrFv, isLoadDoneStageWaveSet);

extern kmSymbol isLoadDoneScenarioResource__11AudSceneMgrFv;
kmBranch(&isLoadDoneScenarioResource__11AudSceneMgrFv, isLoadDoneScenarioWaveSet);