#include "AreaObj/SpinSwitchArea.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/StringUtil.hpp"

SpinSwitchArea::SpinSwitchArea(int type, const char *pName) : AreaObj(type, pName) {
    mIsSwing = false;
}

SpinSwitchArea::~SpinSwitchArea() {

}

void SpinSwitchArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);    
    MR::connectToSceneAreaObj(this);
}

void SpinSwitchArea::movement() {
    if (!isInVolume(*MR::getPlayerPos())) {
        return;
    }

    bool isSwing = MR::isPlayerSwingAction();

    if (isSwing && !mIsSwing) {
        isOnSwitchA() ? offSwitchA() : onSwitchA();
    }

    mIsSwing = isSwing;
}

const char* SpinSwitchArea::getManagerName() const {
    return "SpinSwitchArea";
}