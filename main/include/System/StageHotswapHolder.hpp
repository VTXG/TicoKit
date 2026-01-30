#pragma once

#include "TicoKit.h"

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"
#include "JSystem/JKernel/JKRArchive.hpp"

struct HotswapEntry {
    const char* mFileName;
    const char* mMaskName;
};

class StageHotswapHolder : public NameObj {
public:
    StageHotswapHolder(const char *pName);
    virtual ~StageHotswapHolder();

    void initCommon();
    void initScenario();

    static const char* findSwap(const char *pName);
    static const char* findSwap(const char *pName, MR::AssignableArray<HotswapEntry> *pTable);
    static MR::AssignableArray<HotswapEntry>* parseEntries(void *pResource);

    JKRArchive* mArchive;
    MR::AssignableArray<HotswapEntry>* mCommonTable;
    MR::AssignableArray<HotswapEntry>* mScenarioTable;
};