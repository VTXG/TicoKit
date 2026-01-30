#pragma once

#include "TicoKit.h"
#include "Game/System/GalaxyCometScheduler.hpp"

class JKRArchive;
class JMapInfo;

class GalaxyInfoTable {
public:
    GalaxyInfoTable();

    static GalaxyCometTimePaper* loadCometTable(const void *pResource);

    void* mGalaxyTableData;
};