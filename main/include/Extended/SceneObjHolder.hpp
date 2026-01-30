#pragma once

#include "TicoKit.h"
#include "Game/Scene/SceneObjHolder.hpp"

#define ExtSceneObj_ProductMapCreator  (0x0 + SceneObj_NumMax)
#define ExtSceneObj_PopUpAddTimeBoard  (0x1 + SceneObj_NumMax)
#define ExtSceneObj_StageHotswapHolder (0x2 + SceneObj_NumMax)
#define ExtSceneObj_COUNT 0x3

class ExtSceneObjHolder : public SceneObjHolder {
private:
    NameObj* mExtObj[ExtSceneObj_COUNT];
};