#include "Util/JMapProperty.hpp"
#include "Util/GeneralUtil.hpp"

#include "Game/Util/StringUtil.hpp"

const JMapPropertyIter JMapPropertyInfo::find(const char *pName) const {
    s32 jmpIdxName = mInfo.searchItemInfo("Name");
    if (jmpIdxName < 0) {
        return JMapPropertyIter(nullptr, 0);
    }

    for (int i = 0; i < mInfo.getNumEntries(); i++) {
        const char *pCurr;
        TK::getStringFastOrNull(&mInfo, i, jmpIdxName, &pCurr);

        if (pCurr && MR::isEqualString(pCurr, pName)) {
            return JMapPropertyIter(this, i);
        }
    }

    return JMapPropertyIter(nullptr, -1);
}

bool JMapPropertyInfo::getParamInt(int entryIdx, int paramIdx, s32 *pOut) const {
    char name[0x20];
    snprintf(name, sizeof(name), "Param%02dInt", paramIdx);

    s32 jmpIdxParamInt = mInfo.searchItemInfo(name);
    if (jmpIdxParamInt < 0) {
        *pOut = 0;
        return false;
    }

    return mInfo.getValueFast(entryIdx, jmpIdxParamInt, pOut);
}

bool JMapPropertyInfo::getParamF32(int entryIdx, int paramIdx, f32 *pOut) const {
    char name[0x20];
    snprintf(name, sizeof(name), "Param%02dF32", paramIdx);

    s32 jmpIdxParamF32 = mInfo.searchItemInfo(name);
    if (jmpIdxParamF32 < 0) {
        *pOut = 0;
        return false;
    }

    return mInfo.getValueFast(entryIdx, jmpIdxParamF32, pOut);
}

bool JMapPropertyInfo::getParamStr(int entryIdx, int paramIdx, const char **pOut) const {
    char name[0x20];
    snprintf(name, sizeof(name), "Param%02dStr", paramIdx);

    s32 jmpIdxParamStr = mInfo.searchItemInfo(name);
    if (jmpIdxParamStr < 0) {
        *pOut = nullptr;
        return false;
    }

    TK::getStringFastOrNull(&mInfo, entryIdx, jmpIdxParamStr, pOut);
    return true;
}