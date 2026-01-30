#pragma once

#include <revolution/types.h>
#include <RFLi_Types.h>
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/FileSelectIconID.hpp"

class FileSelectItem;

class FileSelector : public LiveActor {
public:
    FileSelector(const char *);

    virtual ~FileSelector();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void callbackStart();
    void callbackCopy();
    void callbackMii();
    void callbackDelete();
    void callbackManual();
    void notifyItem(FileSelectItem *, s32);
    void initUserFileArray();
    void createCameraController();
    void createSky();
    void createFileItems();
    void createOperationButton();
    void createBackButton();
    void createBrosButton();
    void createInfoMessage();
    void createSysInfoWindow();
    void createFileInfo();
    void createTitle();
    void createMiiSelect();
    void createMiiConfirmIcon();
    void createMiiFont();
    void createManual();
    void createSelectEffect();
    void appearAllItems();
    void disappearAllLayout();
    bool isHiddenAllLayout() const;
    void updateFileInfo();
    void appearAllIndex();
    void disappearAllIndex();
    void invalidateSelectAll();
    void validateSelectAll();
    void initUserFile();
    void restoreUserFile();
    void checkAllComplete();
    void onPoint(FileSelectItem *);
    void onSelect(FileSelectItem *);
    void clearPointing();
    void setFileInfo(s32);
    void checkSelectedBackButton();
    void goToNearPoint();
    void calcBasePos(f32);
    void initAllItems();
    void validateRotateAllItems();
    void getUserFileFellowID(s32) const;
    bool isUserFileMiiIdValid(s32) const;
    void getUserFileMiiIndex(s32) const;
    bool isUserFileCorrupted(s32) const;
    bool isUserFileAppearLuigi(s32) const;
    bool isUserFileLuigi(s32) const;
    void setUserFileMario(s32, bool);
    void storeSetMiiIdUserFile(s32, const FileSelectIconID &);
    void getMiiId(RFLCreateID, const FileSelectIconID &);
    void getIconId(FileSelectIconID *, s32) const;
    void getMissCount(s32) const;
    void playSelectedME();
    void updateBgm();

    void exeTitle();
    void exeTitleEnd();
    void exeRFLError();
    void exeRFLWait();
    void exeRFLWaitEnd();
    void exeFileSelectStart();
    void exeFileSelect();
    void exeFileConfirmStart();
    void exeFileConfirmMiiDeleteWarningStart();
    void exeFileConfirmMiiDeleteWarning();
    void exeFileConfirmMiiDeleteSave();
    void exeFileConfirm();
    void exeDemoStartWait();
    void exeDemo();
    void exeCreateConfirmStart();
    void exeCreateConfirm();
    void exeCreate();
    void exeCopyWait();
    void exeCopySelect();
    void exeCopyConfirmStart();
    void exeCopyConfirm();
    void exeCopySave();
    void exeCopySaveMii();
    void exeCopyDemo();
    void exeCopyRejectStart();
    void exeCopyReject();
    void exeMiiWait();
    void exeMiiTip();
    void exeMiiSelectStart();
    void exeMiiSelect();
    void exeMiiCancel();
    void exeMiiConfirmWait();
    void exeMiiConfirm();
    void exeMiiCreateWait();
    void exeMiiCreateDemo();
    void exeMiiCaution();
    void exeMiiInfoStart();
    void exeMiiInfo();
    void exeDeleteConfirmStart();
    void exeDeleteConfirm();
    void exeDelete();
    void exeDeleteDemo();
    void exeFileBroken();
    void exeManualStart();
    void exeManual();

    u8 _90[0x68];
};