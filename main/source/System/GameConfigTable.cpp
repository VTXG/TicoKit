#include "System/GameConfigTable.hpp"
#include "Util/GeneralUtil.hpp"

#include "Local/Game/Map/FileSelector.hpp"
#include "Local/Game/Map/FileSelectSky.hpp"
#include "Local/Game/Util/LiveActorUtil.hpp"
#include "Game/MapObj/AstroDomeOrbit.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameDataHolder.hpp"
#include "Game/System/UserFile.hpp"
#include "Game/Util/SoundUtil.hpp"

GameConfigTable::GameConfigTable() {
    JKRArchive *pArchive = TK::receiveArchive("/ObjectData/GameConfigTable.arc");

    mPropertyMap.attach(pArchive->getResource("gameconfigtable.bcsv"));
    OSDebugF("GameConfigTable : Loaded table (%d)\n", mPropertyMap.mInfo.getNumEntries());
}

bool GameConfigTable::getProperty(const char *pName, GameConfigProperty *pOut) const {
    const JMapPropertyIter prop = mPropertyMap.find(pName);

    if (prop.isValid()) {
        for (s32 i = 0; i < LIST_COUNT(pOut->mParamInt); i++) {
            prop.getParamInt(i, &pOut->mParamInt[i]);
        }

        for (s32 i = 0; i < LIST_COUNT(pOut->mParamStr); i++) {
            prop.getParamStr(i, &pOut->mParamStr[i]);
        }

        return true;
    }

    return false;
}

namespace TK {
    inline GameConfigTable* getGameConfigTable() {
        return TK::getSingleton<GameConfigTable>();
    }

    bool getGameConfigProperty(const char *pName, GameConfigProperty *pOut) {
        return getGameConfigTable()->getProperty(pName, pOut);
    }
}

class ExtFileSelectSky : public FileSelectSky {
public:
    const char* mModelName;
};

namespace {
    namespace Util {
        bool isOnGameCondition(GameDataHolder *pData, s32 type, const char *pName) {
            return type == 0 ? pData->isOnGameEventFlag(pName) : pData->isPassedStoryEvent(pName);
        }
    }

    namespace Bgm {
        void startMorphInvincibleBgm(const char *pName, bool a2) {
            GameConfigProperty property;

            if (TK::getGameConfigProperty("Bgm::MorphInvincible", &property)) {
                pName = property.mParamStr[0];
            }

            if (pName) MR::startSubBGM(pName, a2);
        }

        void startMorphIceBgm(const char *pName, bool a2) {
            GameConfigProperty property;

            if (TK::getGameConfigProperty("Bgm::MorphIce", &property)) {
                pName = property.mParamStr[0];
            }

            if (pName) MR::startSubBGM(pName, a2);
        }

        void startMorphFireBgm(const char *pName, bool a2) {
            GameConfigProperty property;

            if (TK::getGameConfigProperty("Bgm::MorphFire", &property)) {
                pName = property.mParamStr[0];
            }

            if (pName) MR::startSubBGM(pName, a2);
        }

        void startMorphFlyingBgm(const char *pName, bool a2) {
            GameConfigProperty property;

            if (TK::getGameConfigProperty("Bgm::MorphFlying", &property)) {
                pName = property.mParamStr[0];
            }

            if (pName) MR::startStageBGM(pName, a2);
        }

        void startScenarioOpeningBgm(const char *pName, bool a2) {
            GameConfigProperty property;

            char name[64];
            snprintf(name, sizeof(name), "Bgm::StageIntro%s", MR::getCurrentStageName());

            if (TK::getGameConfigProperty(name, &property) || TK::getGameConfigProperty("Bgm::StageIntro", &property)) {
                pName = property.mParamStr[0];
            }

            if (pName) MR::startStageBGM(pName, a2);
        }

        void startBossBgm(s32 id, void * /* unused */, const char *pName) {
            GameConfigProperty property;

            char name[64];
            snprintf(name, sizeof(name), "Bgm::Boss%d", id);

            if (TK::getGameConfigProperty(name, &property)) {
                pName = property.mParamStr[0];
            }

            if (pName) MR::startStageBGM(pName, false);
        }
    }

    namespace File {
        bool isUserFileAppearLuigi(UserFile *pFile) {
            GameConfigProperty property;

            if (!TK::getGameConfigProperty("File::AppearLuigi", &property)) {
                return pFile->isViewCompleteEnding();
            }

            const char *pName = property.mParamStr[0];
            return pName ? Util::isOnGameCondition(pFile->mGameDataHolder, property.mParamInt[0], pName) : property.mParamInt[1];
        }
    }

    namespace Player {
        bool isPlayerSwingPermission(const char *pName) {
            GameConfigProperty property;

            if (!TK::getGameConfigProperty("Player::SwingPermission", &property)) {
                return GameDataFunction::isPassedStoryEvent(pName);
            }

            pName = property.mParamStr[0];
            return pName ? Util::isOnGameCondition(GameDataFunction::getCurrentGameDataHolder(), property.mParamInt[0], pName) : property.mParamInt[1];
        }
    }

    namespace Sky {
        void initFileSelectModel(ExtFileSelectSky *pSky, const char *pModelArcName, const char *pAnimArcName, bool a4) {
            GameConfigProperty property;

            if (TK::getGameConfigProperty("Sky::FileSelect", &property)) {
                pSky->mModelName = property.mParamStr[0];
            }
            else {
                pSky->mModelName = pModelArcName;
            }

            pSky->initModelManagerWithAnm(pSky->mModelName, pAnimArcName, a4);
        }

        void startFileSelectAnim(ExtFileSelectSky *pSky) {
            MR::startAction(pSky, pSky->mModelName);
        }
    }
}
extern kmSymbol setPlayerMode__10MarioActorFUsb;
kmCall(&setPlayerMode__10MarioActorFUsb + 0x5C, ::Bgm::startMorphInvincibleBgm);
kmCall(&setPlayerMode__10MarioActorFUsb + 0xA8, ::Bgm::startMorphIceBgm);
kmCall(&setPlayerMode__10MarioActorFUsb + 0xD4, ::Bgm::startMorphFireBgm);
kmCall(&setPlayerMode__10MarioActorFUsb + 0xF0, ::Bgm::startMorphFlyingBgm);
kmCall(&setPlayerMode__10MarioActorFUsb + 0x358, ::Bgm::startMorphInvincibleBgm);
kmCall(&setPlayerMode__10MarioActorFUsb + 0x544, ::Bgm::startMorphFireBgm);
kmCall(&setPlayerMode__10MarioActorFUsb + 0x578, ::Bgm::startMorphIceBgm);
kmCall(&setPlayerMode__10MarioActorFUsb + 0x63C, ::Bgm::startMorphFlyingBgm);

extern kmSymbol start__35GameSceneScenarioOpeningCameraStateFv;
kmCall(&start__35GameSceneScenarioOpeningCameraStateFv + 0x3C, ::Bgm::startScenarioOpeningBgm);

extern kmSymbol startBossBGM__2MRFl;
kmCall(&startBossBGM__2MRFl + 0x40, ::Bgm::startBossBgm);
kmWrite32(&startBossBGM__2MRFl + 0x44, PPC_B(0x14));
kmCall(&startBossBGM__2MRFl + 0x64, ::Bgm::startBossBgm);
kmWrite32(&startBossBGM__2MRFl + 0x68, PPC_B(0x14));
kmCall(&startBossBGM__2MRFl + 0x88, ::Bgm::startBossBgm);
kmWrite32(&startBossBGM__2MRFl + 0x8C, PPC_B(0x14));
kmCall(&startBossBGM__2MRFl + 0xAC, ::Bgm::startBossBgm);
kmWrite32(&startBossBGM__2MRFl + 0xB0, PPC_B(0x14));
kmCall(&startBossBGM__2MRFl + 0xD0, ::Bgm::startBossBgm);
kmWrite32(&startBossBGM__2MRFl + 0xD4, PPC_B(0x14));
kmCall(&startBossBGM__2MRFl + 0xF4, ::Bgm::startBossBgm);
kmWrite32(&startBossBGM__2MRFl + 0xF8, PPC_B(0x14));
kmCall(&startBossBGM__2MRFl + 0x118, ::Bgm::startBossBgm);
kmWrite32(&startBossBGM__2MRFl + 0x11C, PPC_B(0x14));
kmCall(&startBossBGM__2MRFl + 0x13C, ::Bgm::startBossBgm);
kmWrite32(&startBossBGM__2MRFl + 0x140, PPC_B(0x14));

extern kmSymbol isUserFileAppearLuigi__12FileSelectorCFl;
kmCall(&isUserFileAppearLuigi__12FileSelectorCFl + 0x30, ::File::isUserFileAppearLuigi);

extern kmSymbol startScene__20GameSequenceProgressFv;
kmCall(&startScene__20GameSequenceProgressFv + 0x10C, ::Player::isPlayerSwingPermission);

extern kmSymbol createSky__12FileSelectorFv;
kmWrite16(&createSky__12FileSelectorFv + 0x16, sizeof(ExtFileSelectSky));

extern kmSymbol init__13FileSelectSkyFRC12JMapInfoIter;
kmCall(&init__13FileSelectSkyFRC12JMapInfoIter + 0x24, ::Sky::initFileSelectModel);

extern kmSymbol exeWait__13FileSelectSkyFv;
kmWrite32(&exeWait__13FileSelectSkyFv + 0x24, PPC_MR(3, 30)); // mr r3, r30
kmCall(&exeWait__13FileSelectSkyFv + 0x28, ::Sky::startFileSelectAnim);
kmWrite32(&exeWait__13FileSelectSkyFv + 0x2C, PPC_B(0x18));