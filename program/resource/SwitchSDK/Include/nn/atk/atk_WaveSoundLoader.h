/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <nn/atk/atk_LoaderManager.h>
#include <nn/atk/atk_PlayerHeapDataManager.h>
#include <nn/atk/atk_SoundArchiveLoader.h>
#include <nn/atk/atk_Task.h>

namespace nn { namespace atk {

    class SoundDataManager;
    class SoundPlayer;

    namespace detail {

        class PlayerHeap;

        namespace driver {
            class WaveSoundLoader;
            typedef LoaderManager<WaveSoundLoader> WaveSoundLoaderManager;
        }
    }
}}

namespace nn { namespace atk { namespace detail { namespace driver {

// プレイヤーヒープへのロードをつかさどる
class WaveSoundLoader
{
public:
    struct LoadInfo
    {
        const SoundArchive* soundArchive;
        const SoundDataManager* soundDataManager;
        const LoadItemInfo* loadInfoWsd;
        SoundPlayer* soundPlayer;

        LoadInfo(
            const SoundArchive* arc,
            const SoundDataManager* mgr,
            const LoadItemInfo* wsd,
            SoundPlayer* player) NN_NOEXCEPT:
        soundArchive(arc),
        soundDataManager(mgr),
        loadInfoWsd(wsd),
        soundPlayer(player)
        {}
    };

    struct Data
    {
        const void* wsdFile;   // メモリ上の bXwsd ファイル
        const void* waveFile;

        Data() NN_NOEXCEPT : wsdFile(NULL), waveFile(NULL) {}
        void Initialize() NN_NOEXCEPT { wsdFile = waveFile = NULL; }
    };

    struct Arg
    {
        const SoundArchive* soundArchive;
        const SoundDataManager* soundDataManager;
        SoundPlayer* soundPlayer;
        LoadItemInfo loadInfoWsd;
        int32_t index;    // .bXwsd ファイル中にいくつめのサウンドかを示す

        Arg() NN_NOEXCEPT : soundArchive(NULL), soundDataManager(NULL), soundPlayer(NULL), index(0)
        {}
    };

    class DataLoadTask : public Task
    {
    public:
        void Initialize() NN_NOEXCEPT;
        virtual void Execute(TaskProfileLogger& logger) NN_NOEXCEPT NN_OVERRIDE;
        bool TryAllocPlayerHeap() NN_NOEXCEPT;

        Arg m_Arg;
        Data m_Data;
        PlayerHeap* m_pPlayerHeap;
        PlayerHeapDataManager* m_pPlayerHeapDataManager;
        bool m_IsLoadSuccess;
        uint8_t m_Padding[3];
    };

    class FreePlayerHeapTask : public Task
    {
    public:
        void Initialize() NN_NOEXCEPT;
        virtual void Execute(TaskProfileLogger& logger) NN_NOEXCEPT NN_OVERRIDE;

        Arg m_Arg;
        PlayerHeap* m_pPlayerHeap;
        PlayerHeapDataManager* m_pPlayerHeapDataManager;
    };

    ~WaveSoundLoader() NN_NOEXCEPT;

    bool IsInUse() NN_NOEXCEPT;
    void Initialize(const Arg& arg) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;
    bool TryWait() NN_NOEXCEPT;
    bool IsLoadSuccess() const NN_NOEXCEPT { return m_Task.m_IsLoadSuccess; }

    const void* GetWsdFile() const NN_NOEXCEPT { return m_Task.m_Data.wsdFile; }
    const void* GetWaveFile() const NN_NOEXCEPT { return m_Task.m_Data.waveFile; }

private:
    DataLoadTask m_Task;
    FreePlayerHeapTask m_FreePlayerHeapTask;
    PlayerHeapDataManager m_PlayerHeapDataManager;

public:
    util::IntrusiveListNode m_LinkForLoaderManager; // for WaveSoundLoaderManager
};

}}}} // namespace nn::atk::detail::driver

