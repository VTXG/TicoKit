#include "Player/MarioConstLoader.hpp"
#include "Util/GeneralUtil.hpp"

#include "Game/System/FileRipper.hpp"

extern MarioConstTable gMarioConstDefault;
extern MarioConstTable gMarioConstDefault2;

namespace MarioConstLoader {
    void init() {
        loadTable("/PlayerData/MarioConst.bin", &gMarioConstDefault);
        loadTable("/PlayerData/LuigiConst.bin", &gMarioConstDefault2);
    }

    void loadTable(const char *pName, MarioConstTable *pTable) {
        if (MR::isFileExist(pName, false) && MR::getFileSize(pName, false) == sizeof(MarioConstTable)) {
            void *pBuffer = FileRipper::loadToMainRAM(pName, nullptr, true, nullptr, FileRipper::UNK_0);
            memcpy(pTable, pBuffer, sizeof(*pTable));
            delete pBuffer;

            OSReport("MarioConstLoader : Loaded table (%s)\n", pName);
        }
        else {
            OSReport("MarioConstLoader : Failed to load table (%s)\n", pName);
        }
    }
}