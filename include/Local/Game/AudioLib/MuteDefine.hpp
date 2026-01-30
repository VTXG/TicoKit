#pragma once

#include <revolution/types.h>

struct MuteDefineEntry {
public:
    /* 0x00 */ u8 mSeqTracks[8];
    /* 0x08 */ u8 mStmTracks[2];
};