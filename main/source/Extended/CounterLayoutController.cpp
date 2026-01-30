#include "Extended/CounterLayoutController.hpp"

#include "Game/Screen/StarPieceCounter.hpp"
#include "Game/Screen/PlayerLeft.hpp"

namespace {
    ExtCounterLayoutController* ctorCounterLayoutController(ExtCounterLayoutController *pCounterCtrl) {
        pCounterCtrl->mTicoCoinCounter = nullptr;
        return pCounterCtrl;
    }

    void initCounterLayoutController(ExtCounterLayoutController *pCounterCtrl) {
        pCounterCtrl->mTicoCoinCounter = new TicoCoinGlobalCounter("TicoCoinGlobalCounter");
        pCounterCtrl->mTicoCoinCounter->initWithoutIter();
        pCounterCtrl->appear();
    }

    bool requestedTicoEatCounterLayoutController(StarPieceCounter *pStarPieceCounter, bool mode) {
        register ExtCounterLayoutController *pCounterCtrl;
        GET_REGISTER(pCounterCtrl, r31);

        if (pStarPieceCounter->tryOnModeTicoEat(mode)) {
            pCounterCtrl->kill();
            return true;
        }

        return false;
    }

    void showAllLayout(PlayerLeft *pPlayerLeft) {
        register ExtCounterLayoutController *pCounterCtrl;
        GET_REGISTER(pCounterCtrl, r31);

        pPlayerLeft->forceAppear();
        pCounterCtrl->mTicoCoinCounter->setForceAppear(true);
    }

    void hideAllLayout(PlayerLeft *pPlayerLeft) {
        register ExtCounterLayoutController *pCounterCtrl;
        GET_REGISTER(pCounterCtrl, r31);

        pPlayerLeft->disappear();
        pCounterCtrl->mTicoCoinCounter->setForceAppear(false);
    }

    CoinCounter* killAllCounter(ExtCounterLayoutController *pController) {
        pController->mTicoCoinCounter->kill();
        return pController->mCoinCounter;
    }
}

extern kmSymbol __ct__21GameSceneLayoutHolderFv;
kmWrite16(&__ct__21GameSceneLayoutHolderFv + 0xA2, sizeof(ExtCounterLayoutController));

extern kmSymbol __ct__23CounterLayoutControllerFv;
kmBranch(&__ct__23CounterLayoutControllerFv + 0x64, ctorCounterLayoutController);

extern kmSymbol init__23CounterLayoutControllerFRC12JMapInfoIter;
kmCall(&init__23CounterLayoutControllerFRC12JMapInfoIter + 0xB4, initCounterLayoutController);
kmWrite32(&init__23CounterLayoutControllerFRC12JMapInfoIter + 0xB8, PPC_NOP);
kmWrite32(&init__23CounterLayoutControllerFRC12JMapInfoIter + 0xBC, PPC_NOP);

extern kmSymbol showAllLayout__23CounterLayoutControllerFv;
kmCall(&showAllLayout__23CounterLayoutControllerFv + 0x38, showAllLayout);

extern kmSymbol hideAllLayout__23CounterLayoutControllerFv;
kmCall(&hideAllLayout__23CounterLayoutControllerFv + 0x38, hideAllLayout);

extern kmSymbol killAllCoounter__23CounterLayoutControllerFv;
kmCall(&killAllCoounter__23CounterLayoutControllerFv + 0x14, killAllCounter);