#pragma once

#include <revolution.h>

class JAUSection;
class JAUSectionHeap;

struct AudSceneSoundInfo {
    /* 0x00 */ const char* mSceneName;
    /* 0x04 */ const char* mStageName;
    s8 mStageEntryIdx;
    s8 mScenarioEntryIdx;
};

class AudSceneMgr {
public:
    AudSceneMgr(JAUSectionHeap *);

    bool isLoadDoneSystemInit();
    bool loadStaticResource();
    bool isLoadDoneStaticResource();
    void loadStageResource(const char *, const char *);
    void loadStageResource(const AudSceneSoundInfo *);
    bool isLoadDoneStageResource();
    void loadScenarioResource(const char *, const char *, s32);
    void loadScenarioResource(const AudSceneSoundInfo *, s32);
    bool isLoadDoneScenarioResource();
    void startScene();
    const AudSceneSoundInfo* findSceneSoundInfo(const char *, const char *);
    void eraseLastBgmWaveSet();
    void eraseLastSeWaveSet();
    void eraseLastSeScenarioWaveSet();
    void loadWaveSet(const s8 *, s32);
    void setPlayerModeMario();
    void setPlayerModeLuigi();
    void loadPlayerResource();
    bool isPlayerResourceLoaded();

    /* 0x00 */ JAUSectionHeap* mSection;
    /* 0x04 */ s32 _4;
    /* 0x08 */ s32 _8;
    /* 0x0C */ s32 _C;
    /* 0x10 */ s32 _10;
    /* 0x14 */ s32 mPlayerType;
    /* 0x18 */ s32 mPrevPlayerType;
    /* 0x1C */ bool mIsPlayerChange;
    /* 0x1D */ u8 _1D;
};