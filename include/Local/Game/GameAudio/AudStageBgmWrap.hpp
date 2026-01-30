#pragma once

#include <revolution.h>

class AudStageBgmWrap {
public:
    s32 changeStageNameToSoundID(const char *, const char *, long);
    void getCometEventBgm(const char *);
    void getKoopaFortressAppearBgm(const char *);

    s32 mSoundId;
};