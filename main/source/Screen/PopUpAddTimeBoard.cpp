#include "Screen/PopUpAddTimeBoard.hpp"
#include "Extended/SceneObjHolder.hpp"

#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

#include "Game/Util/GamePadUtil.hpp"

PopUpAddTimeBoard::PopUpAddTimeBoard(const char *pName) : NameObj(pName) {
    mCounter = nullptr;
}

PopUpAddTimeBoard::~PopUpAddTimeBoard() {

}

void PopUpAddTimeBoard::init(const JMapInfoIter &rIter) {
    mCounter = new CollectCounter("PopUpAddTimeBoardCounter");
    mCounter->initWithoutIter();
}

void PopUpAddTimeBoard::popup(s32 amount) {
    if (MR::isDemoActive()) {
        return;
    }

    mCounter->setCount(amount);
}

namespace TK {
    void createPopUpAddTimeBoard() {
        MR::createSceneObj(ExtSceneObj_PopUpAddTimeBoard);
    }

    void requestPopUpAddTimeBoard(s32 amount) {
        MR::getSceneObj<PopUpAddTimeBoard>(ExtSceneObj_PopUpAddTimeBoard)->popup(amount);
    }
}