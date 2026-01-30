#include "Extended/NameObjFactory.hpp"
#include "AreaObj/ForceMorphArea.hpp"
#include "AreaObj/SpinSwitchArea.hpp"
#include "AreaObj/ZeroGravityArea.hpp"
#include "MapObj/AstroSyncSky.hpp"
#include "MapObj/BenefitItemDash.hpp"
#include "MapObj/SwitchingMoveBlock.hpp"
#include "MapObj/TicoCoin.hpp"
#include "MapObj/TimeAttackClock.hpp"

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/StringUtil.hpp"

#define AREA_FORM_CUBE     0 // Center origin
#define AREA_FORM_BOX      1 // Bottom origin
#define AREA_FORM_SPHERE   2
#define AREA_FORM_CYLINDER 3
#define AREA_FORM_BOWN     4

namespace {
    template<typename T> NameObj* createNameObj(const char* pName);
    template<typename T> NameObj* createCenterOriginCube(const char *pName);
    template<typename T> NameObj* createBaseOriginCube(const char *pName);
    template<typename T> NameObj* createSphere(const char *pName);
    template<typename T> NameObj* createBaseOriginCylinder(const char *pName);
    template<typename T> NameObj* createBowl(const char *pName);

    template<typename T>
    NameObj* createExtNameObj(const char* pName) { return new T(pName); }

    template<typename T, u32 Form>
    NameObj* createExtAreaObj(const char *pName) { return new T(Form, pName); }

    const NameObjFactory::Name2CreateFunc cExtCreateTable[] = {
        { "ForceMorphCube", createExtAreaObj<ForceMorphArea, AREA_FORM_CUBE>, nullptr },
        { "SpinSwitchCube", createExtAreaObj<SpinSwitchArea, AREA_FORM_CUBE>, nullptr },
        { "ZeroGravityCube", createExtAreaObj<ZeroGravityArea, AREA_FORM_CUBE>, nullptr },
        { "ZeroGravitySphere", createExtAreaObj<ZeroGravityArea, AREA_FORM_SPHERE>, nullptr },
        { "ZeroGravityCylinder", createExtAreaObj<ZeroGravityArea, AREA_FORM_CYLINDER>, nullptr },
        { "RedBlueTurnBlock", createExtNameObj<SwitchingMoveBlock>, nullptr },
        { "TicoCoin", createExtNameObj<TicoCoin>, nullptr },
        { "TimeAttackClock", createExtNameObj<TimeAttackClock>, nullptr },
        { "AstroSyncSky", createExtNameObj<AstroSyncSky>, nullptr },
        { "BenefitItemDash", createExtNameObj<BenefitItemDash>, nullptr },
    };

    const NameObjFactory::Name2MakeArchiveListFunc cExtName2MakeArchiveListFuncTable[] = {
        { "ForceMorphCube", ForceMorphArea::makeArchiveList },
    };

    const NameObjFactory::Name2CreateFunc cProductCreateTable[] = {
        { "AnmModelSwitchMove", createNameObj<AnmModelSwitchMove>, nullptr },
        { "AnmModelGroundOnMove", createNameObj<AnmModelGroundOnMove>, nullptr },
        { "AnmModelBindMove", createNameObj<AnmModelBindMove>, nullptr },
        { "AnmModelSwitchMoveEventCamera", createNameObj<AnmModelSwitchMoveEventCamera>, nullptr },
        { "Air", createNameObj<Air>, nullptr },
        { "ProjectionMapAir", createNameObj<ProjectionMapAir>, nullptr },
        { "Sky", createNameObj<Sky>, nullptr },
        { "ProjectionMapSky", createNameObj<ProjectionMapSky>, nullptr },
        { "AssemblyBlock", createNameObj<AssemblyBlock>, nullptr },
        { "HipDropMoveObj", createNameObj<HipDropMoveObj>, nullptr },
        { "ManholeCover", createNameObj<ManholeCover>, nullptr },
        { "MiniatureGalaxy", createNameObj<MiniatureGalaxy>, nullptr },
        { "SurprisedGalaxy", createNameObj<SurprisedGalaxy>, "MiniSurprisedGalaxy" },
        { "RevolvingWay", createNameObj<RevolvingWay>, nullptr },
        { "RailMoveObj", createNameObj<RailMoveObj>, nullptr },
        { "RotateMoveObj", createNameObj<RotateMoveObj>, nullptr },
        { "SimpleClipPartsObj", createNameObj<SimpleClipPartsObj>, nullptr },
        { "SimpleFloaterObj", createNameObj<SimpleFloaterObj>, nullptr },
        { "SimpleMapObj", createNameObj<SimpleMapObj>, nullptr },
        { "SimpleEnvironmentObj", createNameObj<SimpleEnvironmentObj>, nullptr },
        { "SimpleMapObjFarMax", createNameObj<SimpleMapObjFarMax>, nullptr },
        { "SimpleMapObjPush", createNameObj<SimpleMapObjPush>, nullptr },
        { "SimpleMapObjNoSilhouetted", createNameObj<SimpleMapObjNoSilhouetted>, nullptr },
        { "SimpleSeesawObj", createNameObj<SimpleSeesawObj>, nullptr },
        { "SimpleMirrorObj", createNameObj<SimpleMirrorObj>, nullptr },
        { "SimpleMirrorReflectionObj", createNameObj<SimpleMirrorReflectionObj>, nullptr },
        { "SimpleTextureSwitchChangeObj", createNameObj<SimpleTextureSwitchChangeObj>, nullptr },
        { "SimpleTimerObj", createNameObj<SimpleTimerObj>, nullptr },
    };
}

const NameObjFactory::Name2CreateFunc* ExtNameObjFactory::getProductCreateTable() {
    return cProductCreateTable;
}

const NameObjFactory::Name2CreateFunc* ExtNameObjFactory::getProductCreateTableEnd() {
    return LIST_END(cProductCreateTable);
}

namespace {
    const NameObjFactory::Name2CreateFunc* getName2CreateFunc() {
        register const char *pName;
        GET_REGISTER(pName, r29);

        for (const NameObjFactory::Name2CreateFunc *pEntry = cExtCreateTable; pEntry < LIST_END(cExtCreateTable); pEntry++) {
            if (MR::isEqualStringCase(pEntry->mName, pName)) {
                return pEntry;
            }
        }

        return nullptr;
    }

    void getName2MakeArchiveFunc(NameObjArchiveListCollector *pCollector, const char *pName, const JMapInfoIter &rIter) {
        for (const NameObjFactory::Name2MakeArchiveListFunc *pEntry = cExtName2MakeArchiveListFuncTable; pEntry < LIST_END(cExtName2MakeArchiveListFuncTable); pEntry++) {
            if (MR::isEqualString(pEntry->mName, pName)) {
                pEntry->mArchiveFunc(pCollector, rIter);
            }
        }
    }

    asm void funRegisterTime() {
        addi  r11, r1, 0x20
        mr    r3, r27
        mr    r4, r28
        mr    r5, r29
        blr
    }
}

extern kmSymbol getName2CreateFunc__14NameObjFactoryFPCcPCQ214NameObjFactory15Name2CreateFunc;
kmCall(&getName2CreateFunc__14NameObjFactoryFPCcPCQ214NameObjFactory15Name2CreateFunc + 0x6C, getName2CreateFunc);

extern kmSymbol getMountObjectArchiveList__14NameObjFactoryFP27NameObjArchiveListCollectorPCcRC12JMapInfoIter;
kmCall(&getMountObjectArchiveList__14NameObjFactoryFP27NameObjArchiveListCollectorPCcRC12JMapInfoIter + 0xEC, funRegisterTime);
kmBranch(&getMountObjectArchiveList__14NameObjFactoryFP27NameObjArchiveListCollectorPCcRC12JMapInfoIter + 0x100, getName2MakeArchiveFunc);