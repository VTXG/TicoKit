#pragma once

#include "TicoKit.h"

#include "Game/System/FileLoader.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/FileUtil.hpp"
#include "JSystem/JKernel/JKRArchive.hpp"

class LiveActor;
class LayoutActor;

namespace TK {
    // ---------- Core Utils ---------- //
    void error(const char *pFormat, ...);
    const char* getErrorMessage();

    // ---------- Actor Utils ---------- //
    void setPanePos(LayoutActor *pLayout, const char *pPaneName, const TVec2f &rPos);
    void startBckByHash(LiveActor *pActor, u32 hash);
    void setAllEffectHostMtx(LiveActor *pActor, MtxPtr pMtx);

    // ---------- Input Utils ---------- //
    bool testCorePadTrigger1(s32 channel);

    // ---------- JMap Utils ---------- //
    void getStringFastOrNull(const JMapInfo *pInfo, int entryIndex, int itemIndex, const char **pValueOut);
    void getValueFast(const JMapInfo *pInfo, int entryIndex, int itemIndex, u16 *pValueOut);
    void getValueFast(const JMapInfo *pInfo, int entryIndex, int itemIndex, u8 *pValueOut);
    void getValueFast(const JMapInfo *pInfo, int entryIndex, int itemIndex, bool *pValueOut);
    void getCsvDataS8(s8 *pValueOut, const JMapInfo *pInfo, const char *pFieldName, s32 entryIndex);

    // ---------- File Utils ---------- //
    /// @brief Mounts an archive synchronously.
    /// @param pFile Path to the archive file.
    /// @param pHeap Memory heap to use.
    /// @return Pointer to the mounted archive.
    inline JKRArchive* mountArchive(const char *pFile, JKRHeap *pHeap) {
        return (JKRArchive *)MR::mountArchive(pFile, pHeap);
    }

    /// @brief Receives an archive after it has been asynchronously mounted.
    /// @param pFile Path to the archive file.
    /// @return Pointer to the mounted archive.
    inline JKRArchive* receiveArchive(const char *pFile) {
        return (JKRArchive *)MR::receiveArchive(pFile);
    }

    // /// @brief Deletes a file from memory.
    // /// @param pFile Path to the file.
    // inline void removeFile(const char *pFile) {
    //     return SingletonHolder<FileLoader>::get()->removeFile(pFile);
    // }

    // ---------- Memory Utils ---------- //
    inline JKRHeap* getFileCacheHeap() {
        return SingletonHolder<HeapMemoryWatcher>::get()->mFileCacheHeap;
    }

    // ---------- Bit Utils ---------- //
    /// @brief Returns the number of ON bits in the specified value.
    /// @param value The value to check.
    /// @return The number of ON bits.
    s32 getOnBitNum(u32 value);

    // ---------- Math Utils ---------- //
    /// @brief Implementation from SMG2 0x8003F6D0 (USA)
    void vecKillElement(const Vec *, const Vec *, f32);

    /// @brief Implementation from SMG2 USA at 0x800DD7A0.
    /// @param rad The angle to normalize, in radians.
    /// @return The normalized radian angle.
    f64 normalizeRadians(f64 rad);

    // ---------- String Utils ---------- //
    char* trimString(char *pString);
    bool isWhiteSpace(char c);
}