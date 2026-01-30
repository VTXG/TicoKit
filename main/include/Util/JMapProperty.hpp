#pragma once

#include "TicoKit.h"
#include "Game/Util/JMapInfo.hpp"

class JMapPropertyIter;

class JMapPropertyInfo {
public:
    inline bool attach(const void *pData) {
        return mInfo.attach(pData);
    }

    const JMapPropertyIter find(const char *pName) const;
    bool getParamInt(int entryIdx, int paramIdx, s32 *pOut) const;
    bool getParamF32(int entryIdx, int paramIdx, f32 *pOut) const;
    bool getParamStr(int entryIdx, int paramIdx, const char **pOut) const;

    JMapInfo mInfo;
};

class JMapPropertyIter {
public:
    inline JMapPropertyIter(const JMapPropertyInfo* pMap, s32 index) {
        mInfo = pMap;
        mIndex = index;
    }

    inline bool getParamInt(int paramIdx, s32 *pOut) const {
        return mInfo->getParamInt(mIndex, paramIdx, pOut);
    }

    inline bool getParamF32(int paramIdx, f32 *pOut) const {
        return mInfo->getParamF32(mIndex, paramIdx, pOut);
    }

    inline bool getParamStr(int paramIdx, const char **pOut) const {
        return mInfo->getParamStr(mIndex, paramIdx, pOut);
    }

    inline bool isValid() const {
        return mInfo != nullptr;
    }

    const JMapPropertyInfo* mInfo;
    s32 mIndex;
};