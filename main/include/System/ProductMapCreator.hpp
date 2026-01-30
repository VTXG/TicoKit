#pragma once

#include "TicoKit.h"
#include "Extended/NameObjFactory.hpp"

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"

struct ProductMapEntry {
    /* 0x0 */ const char* mModelName;
    /* 0x4 */ const char* mClassName;
};

typedef NameObjFactory::Name2CreateFunc ProductCreateEntry;

class ProductMapCreator : public NameObj {
public:
    ProductMapCreator(const char *pName);

    CreatorFuncPtr getCreateFunc(const char *pModelName) const;
    const ProductCreateEntry* getCreateEntry(const char *pClassName) const;
    bool isRegisteredObj(const char *pModelName) const;

    MR::AssignableArray<ProductMapEntry> mProductMapTable;
};

class ProductMapCreatorFunction {
public:
    static bool isRegisteredObj(const char *pModelName);
    static CreatorFuncPtr getCreateFunc(const char *pModelName);
};