#pragma once

#include "TicoKit.h"
#include "Screen/TicoCoinCounter.hpp"

#include "Game/MapObj/CollectCounter.hpp"

class PopUpAddTimeBoard : public NameObj {
public:
    PopUpAddTimeBoard(const char *pName);

    virtual ~PopUpAddTimeBoard();
    virtual void init(const JMapInfoIter &rIter);

    void popup(s32 amount);

    CollectCounter* mCounter;
};

namespace TK {
    void createPopUpAddTimeBoard();
    void requestPopUpAddTimeBoard(s32 amount);
}