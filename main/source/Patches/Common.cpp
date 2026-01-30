#include "TicoKit.h"

#include "Game/MapObj/PowerStar.hpp"
#include "Game/NPC/TalkMessageInfo.hpp"
#include "Game/Screen/MoviePlayerSimple.hpp"
#include "Game/Screen/THPSimplePlayerWrapper.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/System/GameEventFlagTable.hpp"

// Missing text patch (original patch by Aurum)
namespace {
    static wchar_t sErrorMessageBuffer[128];

    asm void funRegisterTime() {
        mr r12, r5
        mr r4, r5
        blr
    }

    const bool getErrorMessage() {
        register const char *pLabel;
        register TalkMessageInfo *pTalkMsgInfo;
    	asm (
            mr pLabel, r12;
            mr pTalkMsgInfo, r31;
        );

    	s32 len = strlen(pLabel) + 1;
    	if (len > 127) {
    		len = 127;
    	}

    	mbstowcs(sErrorMessageBuffer, pLabel, len);
        pTalkMsgInfo->_0 = (u8 *)sErrorMessageBuffer;
        
    	return false;
    }
}

extern kmSymbol getMessageDirect__11MessageDataCFP15TalkMessageInfoPCc;
kmCall(&getMessageDirect__11MessageDataCFP15TalkMessageInfoPCc + 0x14, funRegisterTime);
kmCall(&getMessageDirect__11MessageDataCFP15TalkMessageInfoPCc + 0x5C, getErrorMessage);

// Skip THPs
namespace {
    bool isEndTHP(MoviePlayerSimple* pMoviePlayer) {
        if (MR::testCorePadTriggerA(0)) {
            MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL", -1, -1);
            return true;
        }

        return pMoviePlayer->getCurrentFrame() + 1 == pMoviePlayer->getTotalFrame();
    }
}

extern kmSymbol exePlaying__17MoviePlayerSimpleFv;
kmCall(&exePlaying__17MoviePlayerSimpleFv + 0x1C, isEndTHP);
kmWrite32(&exePlaying__17MoviePlayerSimpleFv + 0x20, PPC_CMPWI(3, 0)); // cmpwi r3, 0
kmWrite32(&exePlaying__17MoviePlayerSimpleFv + 0x24, PPC_COND_BRANCH(PPC_COND_EQ, 0x40)); // beq   0x40
kmWrite32(&exePlaying__17MoviePlayerSimpleFv + 0x28, PPC_B(0xC));

// 360 first-person camera
extern kmSymbol calc__16CameraSubjectiveFv;
kmWrite32(&calc__16CameraSubjectiveFv + 0x1AC, PPC_NOP);

// Instant pause menus
extern kmSymbol isPermitToPlusPause__24PauseButtonCheckerInGameCFv;
extern kmSymbol isPermitToMinusPause__24PauseButtonCheckerInGameCFv;
kmWrite16(&isPermitToPlusPause__24PauseButtonCheckerInGameCFv + 0x2E, 0x1); // cmpwi r0, <0x1>
kmWrite16(&isPermitToMinusPause__24PauseButtonCheckerInGameCFv + 0x2E, 0x1); // cmpwi r0, <0x1>

// Faster game saving
extern kmSymbol trySave__15SaveDataHandlerFPbb;
kmWrite16(&trySave__15SaveDataHandlerFPbb + 0xA4, 60);

// Remove the comet retry button
extern kmSymbol init__9GameSceneFv;
kmWrite32(&init__9GameSceneFv + 0x170, PPC_LI(3, 0)); // li r3, 0

extern kmSymbol requestGalaxyMove__20GameSequenceProgressFRC18GalaxyMoveArgument;
kmWrite32(&requestGalaxyMove__20GameSequenceProgressFRC18GalaxyMoveArgument + 0x1EC, PPC_LI(3, 0)); // li r3, 0

// Power star demo GeneralPos
namespace {
    bool doPowerStarDemoPos(PowerStar *pPowerStar) {
        if (MR::isStep(pPowerStar, 1)) {
            char buf[32];
            snprintf(buf, 32, "PowerStarDemo%d", pPowerStar->mPowerStarId);

            TMtx34f mtx;
            mtx.identity();

            if (MR::tryFindNamePos(buf, mtx.toMtxPtr())) {
                MR::extractMtxTrans(mtx, &pPowerStar->mPosition);
                JMath::gekko_ps_copy12(&pPowerStar->_B8, &mtx);
            };

            return true;
        }

        return false;
    }
}

extern kmSymbol exeStageClearDemo__9PowerStarFv;
kmCall(&exeStageClearDemo__9PowerStarFv + 0x8C, doPowerStarDemoPos);

// Korean-only patches
#ifdef KOR
namespace {
    const char cTxtGalaxyNameU[] = "TxtGalaxyNameU";
    const char cTxtGalaxyName[] = "TxtGalaxyName";

    asm void getTxtGalaxyNameU() {
        lis  r4, cTxtGalaxyNameU@ha
        addi r4, r4, cTxtGalaxyNameU@l
        blr
    }

    asm void getTxtGalaxyName() {
        lis  r4, cTxtGalaxyName@ha
        addi r4, r4, cTxtGalaxyName@l
        blr
    }
}

kmCall(0x80360810, getTxtGalaxyNameU);
kmCall(0x80360818, getTxtGalaxyName);

// Remove GalaxyMap's A button
kmWrite32(0x8035962C, PPC_B(0x5C));
kmWrite32(0x8035999C, PPC_B(0x14));
kmWrite32(0x80359B98, PPC_B(0x14));
kmWrite32(0x80359D48, PPC_B(0x14));
kmWrite32(0x80359F34, PPC_B(0x14));
kmWrite32(0x8035A8AC, PPC_B(0x14));
kmWrite32(0x8035AA18, PPC_BLR);
#endif

/*
extern kmSymbol init__15SurprisedGalaxyFRC12JMapInfoIter;
kmWrite32(&init__15SurprisedGalaxyFRC12JMapInfoIter + 0x150, PPC_LI(3, 1));
kmWrite32(&init__15SurprisedGalaxyFRC12JMapInfoIter + 0x15C, PPC_LI(3, 1));
*/