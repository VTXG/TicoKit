#include "Extended/SceneObjHolder.hpp"

#include "Screen/PopUpAddTimeBoard.hpp"
#include "System/ProductMapCreator.hpp"
#include "System/StageHotswapHolder.hpp"

namespace {
    NameObj* newExtEachObj(SceneObjHolder *pHolder, int id) {
        if (id == ExtSceneObj_ProductMapCreator) {
            return new ProductMapCreator("ProductMapCreator");
        }

        if (id == ExtSceneObj_PopUpAddTimeBoard) {
            return new PopUpAddTimeBoard("PopUpAddTimeBoard");
        }

        if (id == ExtSceneObj_StageHotswapHolder) {
            return new StageHotswapHolder("StageHotswapHolder");
        }

        return nullptr;
    }

    void createExtSceneObj(int id) {
        MR::createSceneObj(id); // restore original call
        MR::createSceneObj(ExtSceneObj_ProductMapCreator);
    }
}

extern kmSymbol initSceneObjHolder__5SceneFv;
kmWrite16(&initSceneObjHolder__5SceneFv + 0x16, sizeof(ExtSceneObjHolder));

extern kmSymbol __ct__14SceneObjHolderFv;
kmWrite16(&__ct__14SceneObjHolderFv + 0x2, SceneObj_NumMax + ExtSceneObj_COUNT);

extern kmSymbol newEachObj__14SceneObjHolderFi;
kmCall(&newEachObj__14SceneObjHolderFi + 0xC6C, newExtEachObj);

extern kmSymbol init__9GameSceneFv;
kmCall(&init__9GameSceneFv + 0xC8, createExtSceneObj);