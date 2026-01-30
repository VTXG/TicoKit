#include "Util/GeneralUtil.hpp"

#include "Game/Animation/XanimePlayer.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/System/ResourceInfo.hpp"
#include "Game/System/WPad.hpp"
#include "Game/System/WPadButton.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "nw4r/lyt/pane.h"
#include "cmath"

namespace {
    char sErrorMsg[256];
}

namespace TK {
    void error(const char *pFormat, ...) {
        va_list list;

        va_start(list, pFormat);
        vsnprintf(sErrorMsg, sizeof(sErrorMsg), pFormat, list);
        va_end(list);

        OSReport("%s\n", sErrorMsg);
        // GXColor fg = {0xFF, 0xFF, 0xFF, 0xFF};
        // GXColor bg = {0};
        // OSFatal(fg, bg, sErrorMsg);
    }

    const char* getErrorMessage() {
        return sErrorMsg;
    }

    void setPanePos(LayoutActor *pLayout, const char *pPaneName, const TVec2f &rPos) {
        nw4r::lyt::Pane *pPane = MR::getPane(pLayout, pPaneName);
        pPane->mTranslate.x = rPos.x;
        pPane->mTranslate.y = rPos.y;
    }

    void startBckByHash(LiveActor *pActor, u32 hash) {
        ResTable ***pTmp = (ResTable ***)pActor->mModelManager->mXanimePlayer->mResourceTable;
        ResTable *pTable = pTmp[0x6C/4][4/4];

        for (ResFileInfo *pInfo = &pTable->mFileInfoTable[0]; pInfo != &pTable->mFileInfoTable[pTable->mCount]; pInfo++) {
            if (pInfo->mHashCode == hash) {
                MR::startBck(pActor, pInfo->mName, 0);
                break;
            }
        }
    }

    void setAllEffectHostMtx(LiveActor *pActor, MtxPtr pMtx) {
        EffectKeeper *pKeeper = pActor->mEffectKeeper;
        MultiEmitter **pEmitters = (MultiEmitter **)pKeeper->_C;

        for (s32 i = 0; i < pKeeper->_14; i++) {
            pEmitters[i]->setHostMtx(pMtx);
        }
    }

    bool testCorePadTrigger1(s32 channel) {
        return MR::getWPad(channel)->mButton->testTrigger1();
    }

    void getStringFastOrNull(const JMapInfo *pInfo, int entryIndex, int itemIndex, const char **pValueOut) {
        const char *tmp;
        pInfo->getValueFast(entryIndex, itemIndex, &tmp);

        if (tmp && tmp[0]) {
            *pValueOut = tmp;
        }
        else {
            *pValueOut = nullptr;
        }
    }

    void getValueFast(const JMapInfo *pInfo, int entryIndex, int itemIndex, u16 *pValueOut) {
        s32 tmp;
        pInfo->getValueFast(entryIndex, itemIndex, &tmp);
        *pValueOut = tmp;
    }

    void getValueFast(const JMapInfo *pInfo, int entryIndex, int itemIndex, u8 *pValueOut) {
        s32 tmp;
        pInfo->getValueFast(entryIndex, itemIndex, &tmp);
        *pValueOut = tmp;
    }

    void getValueFast(const JMapInfo *pInfo, int entryIndex, int itemIndex, bool *pValueOut) {
        s32 tmp;
        pInfo->getValueFast(entryIndex, itemIndex, &tmp);
        *pValueOut = tmp;
    }

    void getCsvDataS8(s8 *pValueOut, const JMapInfo *pInfo, const char *pFieldName, s32 entryIndex) {
        u8 tmp;
        MR::getCsvDataU8(&tmp, pInfo, pFieldName, entryIndex);
        *pValueOut = tmp;
    }

    s32 getOnBitNum(u32 value) {
        s32 num = 0;

        for (; value; value >>= 1) {
            if (value & 1) {
                num++;
            }
        }

        return num;
    }

    void vecKillElement(register const Vec *a1, register const Vec *a2, register f32 a3) {
        __asm {
            psq_l     f0, 0(a1), 0, 0
            psq_l     f3, 0(a2), 0, 0
            psq_l     f2, 8(a1), 1, 0
            psq_l     f4, 8(a2), 1, 0
            ps_madds0 f0, f3, a3, f0
            ps_madds0 f2, f4, a3, f2
            psq_st    f0, 0(a1), 0, 0
            psq_st    f2, 8(a1), 1, 0
            blr
        }
    }

    f64 normalizeRadians(f64 rad) {
        return fmod(rad, TWO_PI_D);
    }

    char* trimString(char *pString) {
        char c;

        while ((c = *pString) && TK::isWhiteSpace(c)) {
            pString++;
        }

        char *pEnd = pString + strlen(pString);
        while (pEnd > pString) {
            if (!TK::isWhiteSpace(*(pEnd - 1))) {
                break;
            }

            pEnd--;
        }

        *pEnd = 0;
        return pString;
    }

    bool isWhiteSpace(char c) {
        return c == ' ' || c == '\t' || c == '\r';
    }
}