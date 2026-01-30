#include "System/ProductMapCreator.hpp"
#include "Extended/SceneObjHolder.hpp"
#include "Util/GeneralUtil.hpp"

#include "Game/Map/PlanetMapCreator.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"

ProductMapCreator::ProductMapCreator(const char *pName) : NameObj(pName) {
    JKRArchive *pArchive = TK::receiveArchive("/ObjectData/ProductMapObjDataTable.arc");

    JMapInfo info;
    info.attach(pArchive->getResource("productmapobjdatatable.bcsv"));

    mProductMapTable.init(info.getNumEntries());
 
    s32 modelNameInfoIdx = info.searchItemInfo("ModelName");
    s32 classNameInfoIdx = info.searchItemInfo("ClassName");

    for (s32 i = 0; i < mProductMapTable.mMaxSize; i++) {
        ProductMapEntry *pEntry = &mProductMapTable[i];

        info.getValueFast(i, modelNameInfoIdx, &pEntry->mModelName);
        info.getValueFast(i, classNameInfoIdx, &pEntry->mClassName);
    }

    OSDebugF("ProductMapCreator : Loaded all entries (%d)\n", mProductMapTable.mMaxSize);
}

CreatorFuncPtr ProductMapCreator::getCreateFunc(const char *pModelName) const {
    for (s32 i = 0; i < mProductMapTable.mMaxSize; i++) {
        if (MR::isEqualString(mProductMapTable[i].mModelName, pModelName)) {
            return getCreateEntry(mProductMapTable[i].mClassName)->mCreateFunc;
        }
    }

    return nullptr;
}

const ProductCreateEntry* ProductMapCreator::getCreateEntry(const char *pClassName) const {
    for (const ProductCreateEntry *pEntry = ExtNameObjFactory::getProductCreateTable(); pEntry < ExtNameObjFactory::getProductCreateTableEnd(); pEntry++) {
        if (MR::isEqualString(pEntry->mName, pClassName)) {
            return pEntry;
        }
    }

    return nullptr;
}

bool ProductMapCreator::isRegisteredObj(const char *pModelName) const {
    for (s32 i = 0; i < mProductMapTable.mMaxSize; i++) {
        if (MR::isEqualString(mProductMapTable[i].mModelName, pModelName)) {
            return true;
        }
    }

    return false;
}

bool ProductMapCreatorFunction::isRegisteredObj(const char *pModelName) {
    return MR::getSceneObj<ProductMapCreator>(ExtSceneObj_ProductMapCreator)->isRegisteredObj(pModelName);
}

CreatorFuncPtr ProductMapCreatorFunction::getCreateFunc(const char *pModelName) {
    return MR::getSceneObj<ProductMapCreator>(ExtSceneObj_ProductMapCreator)->getCreateFunc(pModelName);
}

namespace {
    CreatorFuncPtr getProductMapCreator(const char *pName) {
        if (PlanetMapCreatorFunction::isRegisteredObj(pName)) {
            return PlanetMapCreatorFunction::getPlanetMapCreator(pName);
        }

        return ProductMapCreatorFunction::getCreateFunc(pName);
    }
}

extern kmSymbol getCreator__14NameObjFactoryFPCc;
kmCall(&getCreator__14NameObjFactoryFPCc + 0x14, getProductMapCreator);
kmWrite32(&getCreator__14NameObjFactoryFPCc + 0x20, PPC_B(0x2C));