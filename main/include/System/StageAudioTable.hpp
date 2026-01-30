#pragma once

#include "TicoKit.h"

#include "Game/Util/Array.hpp"

class JKRArchive;
class JAUSectionHeap;

struct StageBgmTableEntry {
    u32 mStageNameHash;
    s32 mChangeBgmId[6];
    u8 mChangeBgmState[6];
};

struct ScenarioBgmTableEntry {
    u32 mStageNameHash;
    s32 mScenarioNo;
    s32 mBgmId;
    bool mStartType;
};

struct MultiBgmTableEntry {
    u32 mMultiBgmId;
    u32 mBgmId;
    u32 mStreamId;
    f32 mBeatRatio;
    u32 mIntroBeat;
    u32 mLoopBeat;
    u32 mLoopStartSample;
    u32 mLoopEndSample;
};

struct WaveSetTableEntry {
    u32 mStageNameHash;
    s32 mScenarioNo;
    s8 mWaves[4];
    s8 mBgmWaves[2];
};

class StageAudioTable {
public:
    StageAudioTable();

    void initStageBgmTable(void *pResource);
    void initScenarioBgmTable(void *pResource);
    void initMultiBgmTable(void *pResource);
    void initWaveSetTable(void *pResource);

    const StageBgmTableEntry* getStageBgmEntry(const char *pGalaxyName) const;
    const ScenarioBgmTableEntry* getScenarioBgmEntry(const char *pGalaxyName, s32 scenarioNo) const;
    const MultiBgmTableEntry* getMultiBgmEntry(u32 id) const;
    const WaveSetTableEntry* getStageWaveSetEntry(const char *pGalaxyName) const;
    const WaveSetTableEntry* getScenarioWaveSetEntry(const char *pGalaxyName, s32 scenarioNo) const;

    static s32 getSoundId(const char *pLabel);

    MR::AssignableArray<StageBgmTableEntry> mStageBgmTable;
    MR::AssignableArray<ScenarioBgmTableEntry> mScenarioBgmTable;
    MR::AssignableArray<MultiBgmTableEntry> mMultiBgmTable;
    MR::AssignableArray<WaveSetTableEntry> mWaveSetTable;
};

class AudSceneMgr {
public:
    AudSceneMgr(JAUSectionHeap *);

    void loadWaveSet(const s8 *, s32);
    void setPlayerModeMario();
    void setPlayerModeLuigi();
    void loadPlayerResource();
    bool isPlayerResourceLoaded();
};

class ExtAudSceneMgr : public AudSceneMgr {
public:
    ExtAudSceneMgr(JAUSectionHeap *pSection) : AudSceneMgr(pSection) {}
    
    void loadStageWaveSet(const char *pSceneName, const char *pStageName);
    void loadScenarioWaveSet(const char *pSceneName, const char *pStageName, s32 scenarioNo);
    void eraseLastBgmWaveSet();
    void eraseLastSeWaveSet();
    void eraseLastSeScenarioWaveSet();
    void eraseWaveSet(const s8 *, s32);
    bool isLoadDoneStageWaveSet();
    bool isLoadDoneScenarioWaveSet();

    /* 0x00 */ JAUSectionHeap* mSection;
    /* 0x04 */ s32 _4;
    /* 0x08 */ s32 _8;
    /* 0x0C */ const WaveSetTableEntry* mCurrentScenarioEntry;
    /* 0x10 */ const WaveSetTableEntry* mCurrentStageEntry;
    /* 0x14 */ s32 mPlayerType;
    /* 0x18 */ s32 mPrevPlayerType;
    /* 0x1C */ bool mIsPlayerChange;
    /* 0x1D */ u8 _1D;
};