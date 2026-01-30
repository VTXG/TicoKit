#pragma once

#include <revolution/types.h>

class JAUSection {
public:
    s32 loadWaveArc(u32);
    void eraseWaveArc(u32);
    void eraseWaveArc(u32, u32);
};

class JAUSectionHeap : public JAUSection {
public:
    bool isWaveLoaded(u32, u32);
};