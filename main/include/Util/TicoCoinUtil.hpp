#pragma once

#include "TicoKit.h"

namespace TicoCoinUtil {
    bool hasTicoCoinInStage(s32 id);
    void setTicoCoinInStage(s32 id);
    u8 getTicoCoinFlagInStage();
    s32 getTicoCoinNumInStage();

    u8 getTicoCoinMask(const char *pStageName);
    u8 getTicoCoinMaskCurrentStage();
    bool isTicoCoinExist(const char *pStageName);
    bool isTicoCoinExistCurrentStage();
    bool isValidTicoCoinIdInStage(s32 id);

    bool hasStockedTicoCoin(const char *pStageName, s32 id);
    u8 getStockedTicoCoinFlag(const char *pStageName);

    s32 getTicoCoinNumMax();
    s32 calcStockedTicoCoinNum();
    s32 calcCurrentTicoCoinNum();

    bool isNeedToUpdateStageResultTicoCoin();
    void updateStageResultTicoCoin();
}