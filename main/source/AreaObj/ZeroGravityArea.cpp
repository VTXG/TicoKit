#include "AreaObj/ZeroGravityArea.hpp"

#include "Game/Gravity/PlanetGravityManager.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

ZeroGravityArea::ZeroGravityArea(int type, const char *pName) : AreaObj(type, "ZeroGravityArea") {

}

ZeroGravityArea::~ZeroGravityArea() {

}

void ZeroGravityArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

const char* ZeroGravityArea::getManagerName() const {
    return "ZeroGravityArea";
}

namespace {
    bool calcTotalGravityVector(PlanetGravityManager *pManager, TVec3f *pGravity, GravityInfo *pInfo, const TVec3f &rPosition, u32 gravityType, u32 host) {
        if (MR::isInAreaObj("ZeroGravityArea", rPosition)) {
            if (pInfo) {
                pInfo->init();
            }

            if (pGravity) {
                pGravity->zero();
            }

            return false;
        }

        return pManager->calcTotalGravityVector(pGravity, pInfo, rPosition, gravityType, host);
    }
}

extern kmSymbol calcGravityVectorOrZero__25unnamedGravityUtil_cppFPC7NameObjRCQ29JGeometry8TVec3fUlPQ29JGeometry8TVec3fP11GravityInfoUl;
kmCall(&calcGravityVectorOrZero__25unnamedGravityUtil_cppFPC7NameObjRCQ29JGeometry8TVec3fUlPQ29JGeometry8TVec3fP11GravityInfoUl + 0x54, calcTotalGravityVector);