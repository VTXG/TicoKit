#include "TicoKit.h"

#include "Local/Game/AreaObj/AreaObjContainer.hpp"
#include "Game/AreaObj/AreaObj.hpp"

namespace {
    template<typename T>
    AreaObjMgr* createManager(s32 type, const char* pName);

    const Name2CreateManagerFunc cExtCreateFunc[] = {
        { "ForceMorphArea", 0x10, createManager<AreaObjMgr> },
        { "SpinSwitchArea", 0x10, createManager<AreaObjMgr> },
        { "ZeroGravityArea", 0x20, createManager<AreaObjMgr> },
    };
}

namespace {
    void initContainer(AreaObjContainer *pContainer) {
        for (const Name2CreateManagerFunc *pEntry = cExtCreateFunc; pEntry < LIST_END(cExtCreateFunc); pEntry++) {
            AreaObjMgr *pMgr = pEntry->mCreateFunc(pEntry->_4, pEntry->mName);
            pMgr->initWithoutIter();

            pContainer->mManagerArray[pContainer->mNumManagers++] = pMgr;
        }
    }

    AreaObj* findInSafe(AreaObjMgr *pMgr, const TVec3f &rPos) {
        if (pMgr) {
            return pMgr->find_in(rPos);
        }

        return false;
    }

    asm void funRegisterTime() {
        addi  r11, r1, 0x30
        mr    r3, r25
        blr
    }
}

extern kmSymbol init__16AreaObjContainerFRC12JMapInfoIter;
kmCall(&init__16AreaObjContainerFRC12JMapInfoIter + 0x90, funRegisterTime);
kmBranch(&init__16AreaObjContainerFRC12JMapInfoIter + 0xA4, initContainer);

extern kmSymbol getAreaObj__16AreaObjContainerCFPCcRCQ29JGeometry8TVec3f;
kmCall(&getAreaObj__16AreaObjContainerCFPCcRCQ29JGeometry8TVec3f + 0x1C, findInSafe);