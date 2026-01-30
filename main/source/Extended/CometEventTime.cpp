#include "Extended/CometEventTime.hpp"
#include "Extended/GalaxyStatusAccessor.hpp"

#include "Local/Game/NPC/CometEventKeeper.hpp"
#include "Local/Game/NPC/CometEventExecutorTimeLimit.hpp"
#include "Game/NPC/EventDirector.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

// TODO fix weird scaling up issue

void ExtTimeLimitLayout::addTime(s32 amount) {
    s32 initialTime = mTime;

    if (amount < 0) {
        amount = -amount;

        if (amount > mTime) {
            mTime = 0;
        }
        else {
            mTime -= amount;
        }
    }
    else {
        mTime += amount;

        if (mTime > mTimeLimit) {
            mTimeLimit = mTime;
        }
    }

    if (initialTime > 600 && mTime < 600) {
        MR::startAnim(this, "TimeLimit", 0);
    }
    else {
        MR::stopAnim(this, 0);
    }
}

namespace TK {
    void addCometTimer(s32 amount) {
        if (!MR::isGalaxyRedCometAppearInCurrentStage() && !MR::isGalaxyBlackCometAppearInCurrentStage()) {
            return;
        }

        ExtTimeLimitLayout *pLayout = (ExtTimeLimitLayout *)MR::getSceneObj<EventDirector>(SceneObj_EventDirector)->mCometEventKeeper->mExecutorTimeLimit->mTimeLimitLayout;
        if (pLayout) {
            pLayout->addTime(amount);
        }
    }
}

namespace {
    u32 getTimeLimitFromTable() {
        ExtGalaxyStatusAccessor accessor = MR::makeCurrentGalaxyStatusAccessor();
        return accessor.getCometLimitTimer(MR::getCurrentScenarioNo());
    }

    void addFrame(TimeLimitLayout *pLayout) {
        if (pLayout->mTime == 600) {
            MR::startAnim(pLayout, "TimeLimit", 0);
        }

        pLayout->updateTextBox();
    }

    const Timing* getCurrentTiming(TimeLimitLayout *pLayout, void *, const Timing *pTable) {
        for (s32 i = 0; i < 5; i++) {
            const Timing *pTiming = &pTable[i];

            u32 scaleStart = pTiming->mScaleStartFrame;
            u32 scaleEnd = scaleStart - pTiming->mScaleKeepFrame;

            if (pLayout->mTimeLimit <= scaleStart || pLayout->mTime <= scaleStart || pLayout->mTime > scaleEnd) {
                continue;
            }

            return pTiming;
        }

        return nullptr;
    }

    asm void resetDisplayMode() {
        li  r31, 0
        stb r31, 0x34(r30)
        blr
    }

    asm void setTimeLimit() {
        stw r4, 0x20(r3)
        stw r4, 0x24(r3)
        blr
    }
}

extern kmSymbol __ct__15TimeLimitLayoutFUl;
kmWrite32(&__ct__15TimeLimitLayoutFUl + 0x38, PPC_STW(31, 0x20, 30)); // stw r31, 0x20(r30)

extern kmSymbol resetFrame__15TimeLimitLayoutFv;
kmWrite32(&resetFrame__15TimeLimitLayoutFv + 0x10, PPC_LWZ(31, 0x24, 3)); // lwz r31, 0x24(r3)
kmCall(&resetFrame__15TimeLimitLayoutFv + 0x24, resetDisplayMode);

extern kmSymbol addFrame__15TimeLimitLayoutFv;
kmWrite32(&addFrame__15TimeLimitLayoutFv + 0x4, PPC_NOP);
kmWrite32(&addFrame__15TimeLimitLayoutFv + 0x8, PPC_CMPLWI(4, 0)); // cmplwi r4, 0
kmWrite32(&addFrame__15TimeLimitLayoutFv + 0xC, PPC_COND_BRANCH(PPC_COND_EQ, 0x10)); // beq    0x10
kmWrite32(&addFrame__15TimeLimitLayoutFv + 0x10, PPC_NOP);
kmWrite32(&addFrame__15TimeLimitLayoutFv + 0x14, PPC_ADDI(0, 4, -1)); // addi r0, r4, -1
kmBranch(&addFrame__15TimeLimitLayoutFv + 0x1C, addFrame); // addi r0, r4, -1

extern kmSymbol updateTextBox__15TimeLimitLayoutFv;
kmWrite32(&updateTextBox__15TimeLimitLayoutFv + 0x8, PPC_NOP);
kmWrite32(&updateTextBox__15TimeLimitLayoutFv + 0x10, PPC_NOP);

extern kmSymbol getCurrentTiming__15TimeLimitLayoutCFv;
kmBranch(&getCurrentTiming__15TimeLimitLayoutCFv + 0xC, getCurrentTiming);

extern kmSymbol updateNormal__15TimeLimitLayoutFv;
kmWrite32(&updateNormal__15TimeLimitLayoutFv + 0x38, PPC_LWZ(4, 0x20, 30)); // lwz r4, 0x20(r30)
kmWrite32(&updateNormal__15TimeLimitLayoutFv + 0x3C, PPC_NOP);
kmWrite32(&updateNormal__15TimeLimitLayoutFv + 0x40, PPC_NOP);
kmWrite32(&updateNormal__15TimeLimitLayoutFv + 0xD8, PPC_LWZ(0, 0x20, 30)); // lwz r0, 0x20(r30)
kmWrite32(&updateNormal__15TimeLimitLayoutFv + 0xDC, PPC_NOP);
kmWrite32(&updateNormal__15TimeLimitLayoutFv + 0xE0, PPC_NOP);
kmWrite32(&updateNormal__15TimeLimitLayoutFv + 0xF8, PPC_NOP);
kmWrite32(&updateNormal__15TimeLimitLayoutFv + 0x100, PPC_NOP);
kmWrite32(&updateNormal__15TimeLimitLayoutFv + 0x104, PPC_CMPWI(3, 0)); // cmpwi r3, 0

extern kmSymbol exeScaleKeep__15TimeLimitLayoutFv;
kmWrite32(&exeScaleKeep__15TimeLimitLayoutFv + 0x6C, PPC_B(0x10));

extern kmSymbol setTimeLimit__15TimeLimitLayoutFUl;
kmBranch(&setTimeLimit__15TimeLimitLayoutFUl, setTimeLimit);

extern kmSymbol init__16CometEventKeeperFv;
kmCall(&init__16CometEventKeeperFv + 0x68, getTimeLimitFromTable);
kmWrite32(&init__16CometEventKeeperFv + 0x6C, PPC_NOP);
kmWrite32(&init__16CometEventKeeperFv + 0x70, PPC_NOP);