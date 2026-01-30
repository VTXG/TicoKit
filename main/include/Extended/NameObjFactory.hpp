#pragma once

#include "TicoKit.h"
#include "Local/Game/NameObj/NameObjFactory.hpp"

class AnmModelSwitchMove;
class AnmModelGroundOnMove;
class AnmModelBindMove;
class AnmModelSwitchMoveEventCamera;
class Air;
class ProjectionMapAir;
class Sky;
class ProjectionMapSky;
class AssemblyBlock;
class HipDropMoveObj;
class ManholeCover;
class MiniatureGalaxy;
class SurprisedGalaxy;
class RevolvingWay;
class RailMoveObj;
class RotateMoveObj;
class SimpleClipPartsObj;
class SimpleFloaterObj;
class SimpleMapObj;
class SimpleEnvironmentObj;
class SimpleMapObjFarMax;
class SimpleMapObjPush;
class SimpleMapObjNoSilhouetted;
class SimpleSeesawObj;
class SimpleMirrorObj;
class SimpleMirrorReflectionObj;
class SimpleTextureSwitchChangeObj;
class SimpleTimerObj;

class ExtNameObjFactory {
public:
    static const NameObjFactory::Name2CreateFunc* getProductCreateTable();
    static const NameObjFactory::Name2CreateFunc* getProductCreateTableEnd();
};