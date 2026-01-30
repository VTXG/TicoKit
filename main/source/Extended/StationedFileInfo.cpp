#include "TicoKit.h"

#include "Game/System/StationedFileInfo.hpp"

#define HEAP_TYPE_NAPA 0
#define HEAP_TYPE_GDDR 1

#define LOAD_TYPE_LOAD  0
#define LOAD_TYPE_MOUNT 1
#define LOAD_TYPE_MOUNT_STATIONED 2
#define LOAD_TYPE_MOUNT_STATIONED_MARIO 3
#define LOAD_TYPE_MOUNT_STATIONED_LUIGI 4
#define LOAD_TYPE_MOUNT_LAYOUT_HOLDER 5

namespace {
    const MR::StationedFileInfo cNewStationedFileInfo[] = {
        { HEAP_TYPE_GDDR, LOAD_TYPE_MOUNT, "/ObjectData/AstroConfigTable.arc" },
        { HEAP_TYPE_GDDR, LOAD_TYPE_MOUNT, "/ObjectData/GalaxyInfoTable.arc" },
        { HEAP_TYPE_GDDR, LOAD_TYPE_MOUNT, "/ObjectData/GameConfigTable.arc" },
        { HEAP_TYPE_GDDR, LOAD_TYPE_MOUNT, "/ObjectData/GameEventTable.arc" },
        { HEAP_TYPE_GDDR, LOAD_TYPE_MOUNT, "/ObjectData/ProductMapObjDataTable.arc" },
        { HEAP_TYPE_GDDR, LOAD_TYPE_MOUNT_STATIONED, "/ObjectData/TicoCoin.arc" },
        { HEAP_TYPE_GDDR, LOAD_TYPE_MOUNT_LAYOUT_HOLDER, "/LayoutData/TicoCoinGlobalCounter.arc" },
        { HEAP_TYPE_GDDR, LOAD_TYPE_MOUNT_LAYOUT_HOLDER, "/LayoutData/TicoCoinStageCounter.arc" },
        { 0, 255, nullptr } // Table end entry
    };

    asm bool loadNewTable() {
#ifndef INTELLISENSE_FIX
        lwz       r0, 8(r31)
        cmpwi     r0, 0
        bne       loc_28
        lwz       r0, 4(r31)
        cmpwi     r0, 255
        beq       loc_28
        lis       r31, cNewStationedFileInfo@ha
        addi      r31, r31, cNewStationedFileInfo@l
    loc_28:
        lwz       r3, 8(r31)
        addic     r0, r3, -1
        subfe     r3, r0, r3
        blr
#endif
    }
}

extern kmSymbol loadResourcesFromTable__22StationedArchiveLoaderFRCQ222StationedArchiveLoader9Condition;
kmCall(&loadResourcesFromTable__22StationedArchiveLoaderFRCQ222StationedArchiveLoader9Condition + 0xB4, loadNewTable);
kmWrite32(&loadResourcesFromTable__22StationedArchiveLoaderFRCQ222StationedArchiveLoader9Condition + 0xB8, PPC_CMPWI(3, 0));

extern kmSymbol createAndAddResourcesFromTable__22StationedArchiveLoaderFRCQ222StationedArchiveLoader9Condition;
kmCall(&createAndAddResourcesFromTable__22StationedArchiveLoaderFRCQ222StationedArchiveLoader9Condition + 0x80, loadNewTable);
kmWrite32(&createAndAddResourcesFromTable__22StationedArchiveLoaderFRCQ222StationedArchiveLoader9Condition + 0x84, PPC_CMPWI(3, 0));