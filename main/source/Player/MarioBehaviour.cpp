#include "TicoKit.h"
#include "AreaObj/ForceMorphArea.hpp"

#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
    
namespace {
    void updateForceMorphArea(Mario *pMario) {
        if (pMario->mActor->mHealth > 0) {
            ForceMorphArea *pMorphArea = (ForceMorphArea *)MR::getAreaObj("ForceMorphArea", pMario->getTrans());

            if (pMorphArea) {
                u16 mode = pMorphArea->getPlayerMode();

                if (pMario->getPlayerMode() != mode) {
                    MR::startSystemSE("SE_SY_MORPH", -1, -1);
                    pMario->mActor->setPlayerMode(mode, true);
                }
            }
        }

        pMario->updateBinderInfo(); // restore original call
    }
}

extern kmSymbol update__5MarioFv;
kmCall(&update__5MarioFv + 0x7C, updateForceMorphArea);