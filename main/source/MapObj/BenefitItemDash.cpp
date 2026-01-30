#include "MapObj/BenefitItemDash.hpp"

#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioHolder.hpp"

BenefitItemDash::BenefitItemDash(const char *pName) : BenefitItemObj(pName, "Kinoko") {
    mDashTime = 180;
    mDashForward = 5.0f;
}

BenefitItemDash::~BenefitItemDash() {

}

void BenefitItemDash::initModelAndEfx() {
    initModelManagerWithAnm(_120, nullptr, false);
    initEffectKeeper(5, "KinokoOneUp", 0);
    initBinder(50.0f, 50.0f, 0);
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 4u, 50.0, TVec3f(0.0f, 50.0f, 0.0f));
}

void BenefitItemDash::exeCatch() {
    Mario *pMario = MR::getMarioHolder()->mActor->mMario;
    pMario->_436 = 0;
    pMario->_278 = mDashForward;
    pMario->_434 = mDashTime;
    pMario->playSound("スペシャルダッシュ強", -1);
    pMario->playSound("ダッシュ加速強成功", -1);
    pMario->playSound("声物ジャンプ", -1);
    MR::startSystemSE("SE_SY_GET_DASH_RING", -1, -1);
    MR::startSystemSE("SE_PV_JUMP_JOY", -1, -1);

    kill();
}

extern kmSymbol calcFogLighting__10MarioActorFv;
kmWrite8(&calcFogLighting__10MarioActorFv + 0x1D8 + 0x3, 0xFF); // Player Dash R
kmWrite8(&calcFogLighting__10MarioActorFv + 0x1DC + 0x3, 0xFF); // Player Dash G
kmWrite8(&calcFogLighting__10MarioActorFv + 0x1E0 + 0x3, 0xFF); // Player Dash B