#pragma once

#include "TicoKit.h"

#include "Game/Player/MarioConst.hpp"

namespace MarioConstLoader {
    void init();
    void loadTable(const char *pName, MarioConstTable *pTable);
}