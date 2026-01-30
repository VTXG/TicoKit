#include "AreaObj/ForceMorphArea.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

/*

ForceMorphArea - Forces the player to enter a specific power-up state.
  Obj_arg0: The power-up state.
    0: None
    1: Rainbow Star
    2: Fire Flower
    3: Ice Flower
    4: Bee Mushroom
    5: Spring Mushroom
    6: Boo Mushroom
    7: Red Star
  SW_A: Can enable/disable the area.

*/

ForceMorphArea::ForceMorphArea(int type, const char *pName) : AreaObj(type, pName) {

}

ForceMorphArea::~ForceMorphArea() {

}

void ForceMorphArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

bool ForceMorphArea::isInVolume(const TVec3f &rPos) const {
    if (isValidSwitchA() && !isOnSwitchA()) {
        return false;
    }

    return AreaObj::isInVolume(rPos);
}

const char* ForceMorphArea::getManagerName() const {
    return "ForceMorphArea";
}

void ForceMorphArea::makeArchiveList(NameObjArchiveListCollector *pCollector, const JMapInfoIter &rIter) {
    s32 itemStatus = -1;
    MR::getJMapInfoArg0NoInit(rIter, &itemStatus);

    const char *pPlayerName = MR::isPlayerLuigi() ? "Luigi" : "Mario";
    const char *pMorphName = nullptr;
    
    switch (itemStatus) {
        case 1:
            pCollector->addArchive("InvincibleMarioHandL");
            pCollector->addArchive("InvincibleMarioHandR");
            pMorphName = "Invincible";
            break;
        case 3:
            pCollector->addArchive("IceMarioHandL");
            pCollector->addArchive("IceMarioHandR");
            pMorphName = "Ice";
            break;
        case 4:
            pCollector->addArchive("BeeMarioWing");
            pMorphName = "Bee";
            break;
        case 5:
            pMorphName = "Hopper";
            break;
        case 6:
            pMorphName = "Teresa";
            break;
        default:
            return;
    }

    char archiveName[16];
    snprintf(archiveName, sizeof(archiveName), "%s%s", pMorphName, pPlayerName);
    pCollector->addArchive(archiveName);
}