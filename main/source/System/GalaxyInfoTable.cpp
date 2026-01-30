#include "System/GalaxyInfoTable.hpp"
#include "Util/GeneralUtil.hpp"

#include "Local/Game/System/GameEventFlagTable.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

// todo : scrap this table and use a member var
namespace {
    static GalaxyCometTimePaper* sNewGalaxyCometTimerPaper[6];
}

GalaxyInfoTable::GalaxyInfoTable() {
    JKRArchive *pArchive = TK::receiveArchive("/ObjectData/GalaxyInfoTable.arc");

    mGalaxyTableData = pArchive->getResource("galaxytable.bcsv");
    OSDebug("GalaxyInfoTable : GalaxyTable loaded\n");

    for (s32 i = 0; i < 6; i++) {
        char name[32];
        snprintf(name, sizeof(name), "comettable%d.bcsv", i + 1);

        sNewGalaxyCometTimerPaper[i] = loadCometTable(pArchive->getResource(name));
        OSDebugF("GalaxyInfoTable : CometTable loaded (%s)\n", name);
    }
}

GalaxyCometTimePaper* GalaxyInfoTable::loadCometTable(const void *pResource) {
    JMapInfo info;
    info.attach(pResource);

    s32 entryCount = info.getNumEntries();

    s32 jmpIdxGalaxyName = info.searchItemInfo("GalaxyName");
    s32 jmpIdxCategory = info.searchItemInfo("Category");

    GalaxyCometTimePaper *pCometTable = new GalaxyCometTimePaper[entryCount + 1];

    for (s32 i = 0; i < entryCount; i++) {
        GalaxyCometTimePaper *pEntry = &pCometTable[i];

        info.getValueFast(i, jmpIdxGalaxyName, &pEntry->mGalaxyName);
        info.getValueFast(i, jmpIdxCategory, &pEntry->mCategory);
    }

    pCometTable[entryCount].mGalaxyName = nullptr;
    pCometTable[entryCount].mCategory = 0;
    return pCometTable;
}

namespace {
    void* getGalaxyTableData() {
        return TK::getSingleton<GalaxyInfoTable>()->mGalaxyTableData;
    }

    asm void getCometTable() {
        lis  r31, sNewGalaxyCometTimerPaper@ha
        addi r31, r31, sNewGalaxyCometTimerPaper@l
        blr
    }

    s32 getEncounterCometPowerStarId(s32 type, const char *pGalaxyName) {
        const char *pCometName;
        s32 id;

        if (type == 0) {
            id = 4;
            pCometName = "Normal";
        }
        else if (type == 1) {
            id = 5;
            pCometName = "Coin100";
        }
        else {
            return 0;
        }

        char name[0x100];
        snprintf(name, sizeof(name), "%sComet%s", pCometName, pGalaxyName);

        const GameEventFlag *pFlag = GameEventFlagTable::findFlag(name);

        if (pFlag && pFlag->mCondition2) {
            id = pFlag->mCondition2;
        }

        return id;
    }
}

extern kmSymbol initPaneCtrlPointing__9GalaxyMapFv;
kmCall(&initPaneCtrlPointing__9GalaxyMapFv + 0x30, getGalaxyTableData);
kmWrite32(&initPaneCtrlPointing__9GalaxyMapFv + 0x34, PPC_MR(4, 3)); // mr r4, r3
kmWrite32(&initPaneCtrlPointing__9GalaxyMapFv + 0x38, PPC_ADDI(3, 1, 0x1C)); // addi r3, r1, 0x1C

extern kmSymbol isComplete__17GalaxyMapDomeIconCFv;
kmCall(&isComplete__17GalaxyMapDomeIconCFv + 0x1C, getGalaxyTableData);
kmWrite32(&isComplete__17GalaxyMapDomeIconCFv + 0x20, PPC_MR(4, 3)); // mr r4, r3
kmWrite32(&isComplete__17GalaxyMapDomeIconCFv + 0x24, PPC_ADDI(3, 1, 0x28)); // addi r3, r1, 0x28

extern kmSymbol hasNewGalaxy__17GalaxyMapDomeIconCFv;
kmCall(&hasNewGalaxy__17GalaxyMapDomeIconCFv + 0x1C, getGalaxyTableData);
kmWrite32(&hasNewGalaxy__17GalaxyMapDomeIconCFv + 0x20, PPC_MR(4, 3)); // mr r4, r3
kmWrite32(&hasNewGalaxy__17GalaxyMapDomeIconCFv + 0x24, PPC_ADDI(3, 1, 0x28)); // addi r3, r1, 0x28

extern kmSymbol getGalaxySortIndex__19GalaxyNameSortTableFPCc;
kmCall(&getGalaxySortIndex__19GalaxyNameSortTableFPCc + 0x20, getGalaxyTableData);
kmWrite32(&getGalaxySortIndex__19GalaxyNameSortTableFPCc + 0x24, PPC_MR(4, 3)); // mr r4, r3
kmWrite32(&getGalaxySortIndex__19GalaxyNameSortTableFPCc + 0x28, PPC_ADDI(3, 1, 0x2C)); // addi r3, r1, 0x2C

extern kmSymbol getPowerStarNumToOpenGalaxy__13GameDataConstFPCc;
kmCall(&getPowerStarNumToOpenGalaxy__13GameDataConstFPCc + 0x1C, getGalaxyTableData);
kmWrite32(&getPowerStarNumToOpenGalaxy__13GameDataConstFPCc + 0x20, PPC_MR(4, 3)); // mr r4, r3
kmWrite32(&getPowerStarNumToOpenGalaxy__13GameDataConstFPCc + 0x24, PPC_ADDI(3, 1, 0x1C)); // addi r3, r1, 0x1C

extern kmSymbol getIncludedGrandGalaxyId__13GameDataConstFPCc;
kmCall(&getIncludedGrandGalaxyId__13GameDataConstFPCc + 0x1C, getGalaxyTableData);
kmWrite32(&getIncludedGrandGalaxyId__13GameDataConstFPCc + 0x20, PPC_MR(4, 3)); // mr r4, r3
kmWrite32(&getIncludedGrandGalaxyId__13GameDataConstFPCc + 0x24, PPC_ADDI(3, 1, 0x1C)); // addi r3, r1, 0x1C

extern kmSymbol getGalaxyDependedFlags__18GameEventFlagTableFPPCciPCc;
kmCall(&getGalaxyDependedFlags__18GameEventFlagTableFPPCciPCc + 0x24, getGalaxyTableData);
kmWrite32(&getGalaxyDependedFlags__18GameEventFlagTableFPPCciPCc + 0x28, PPC_MR(4, 3)); // mr r4, r3
kmWrite32(&getGalaxyDependedFlags__18GameEventFlagTableFPPCciPCc + 0x2C, PPC_ADDI(3, 1, 0x30)); // addi r3, r1, 0x30

extern kmSymbol __ct__20GalaxyCometSchedulerFv;
kmCall(&__ct__20GalaxyCometSchedulerFv + 0x38, getCometTable);
kmWrite32(&__ct__20GalaxyCometSchedulerFv + 0x40, PPC_NOP);

extern kmSymbol getEncounterCometPowerStarId__20GalaxyCometSchedulerCFPCc;
kmWrite32(&getEncounterCometPowerStarId__20GalaxyCometSchedulerCFPCc + 0x34, PPC_MR(4, 31));
kmCall(&getEncounterCometPowerStarId__20GalaxyCometSchedulerCFPCc + 0x38, getEncounterCometPowerStarId);
kmWrite32(&getEncounterCometPowerStarId__20GalaxyCometSchedulerCFPCc + 0x3C, PPC_B(0x24));

/*
#include "Game/Util/EventUtil.hpp"

namespace {
    bool canAppearNormalComet(char const *pName) {
        OSReport("Normal Comet cycle check : '%s' (0x%08X)\n", pName, pName);
        return MR::canAppearNormalComet(pName);
    }

    bool canAppearCoin100Comet(char const *pName) {
        OSReport("Purple Comet cycle check : '%s' (0x%08X)\n", pName, pName);
        return MR::canAppearCoin100Comet(pName);
    }
}

extern kmSymbol findSuitableTimePaperPos__20GalaxyCometTimeTableCFPll;
kmCall(&findSuitableTimePaperPos__20GalaxyCometTimeTableCFPll + 0x74, canAppearNormalComet);
kmCall(&findSuitableTimePaperPos__20GalaxyCometTimeTableCFPll + 0x7C, canAppearCoin100Comet);
*/