#pragma once

#include <revolution/types.h>

struct GameEventFlag {
    /* 0x00 */ const char* mName;
    /* 0x04 */ u8 mType;
    /* 0x05 */ u8 mSaveFlag;
    /* 0x06 */ u8 mCondition1;
    /* 0x07 */ u8 mCondition2;
    u32 _8;
    /* 0x0C */ const char* mCondition3;
    /* 0x10 */ const char* mCondition4;
};