#pragma once

#include "TicoKit.h"

struct IniEntry {
    const char* mKey;
    char* mValue;
    int mValueSize;
};

namespace TK {
    bool parseIni(const char *pFile, IniEntry *pEntries, s32 entryNum);
}