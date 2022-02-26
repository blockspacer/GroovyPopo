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

#include <nn/atk/atk_SoundStartable.h>
#include <nn/atk/atk_SoundInstanceManager.h>
#include <nn/atk/atk_WaveSound.h>
#include <nn/atk/atk_SoundArchive.h>

namespace nn { namespace atk {

    class SoundDataManager;

    namespace detail {

        class WaveSound;
        class StartInfoReader;

    }
}}

namespace nn { namespace atk { namespace detail {

class WaveSoundRuntime
{
public:
    WaveSoundRuntime() NN_NOEXCEPT;
    ~WaveSoundRuntime() NN_NOEXCEPT;

public:
    bool Initialize( int soundCount, void** pOutAllocatedAddr, const void* endAddr ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    static size_t GetRequiredMemorySize(const SoundArchive::SoundArchivePlayerInfo& soundArchivePlayerInfo, int alignmentSize) NN_NOEXCEPT;

    void SetSoundArchiveFilesHook(detail::SoundArchiveFilesHook* filesHook)
    {
        m_pSoundArchiveFilesHook = filesHook;
    }

    int GetActiveCount() const NN_NOEXCEPT;
    int GetFreeWaveSoundCount() const NN_NOEXCEPT;

    void SetupUserParam(void** startAddr, size_t adjustSize) NN_NOEXCEPT;

    void Update() NN_NOEXCEPT;

    WaveSound* AllocSound(
        SoundArchive::ItemId soundId,
        int priority,
        int ambientPriority,
        detail::BasicSound::AmbientInfo* ambientArgInfo,
        OutputReceiver* pOutputReceiver
    ) NN_NOEXCEPT;

    SoundStartable::StartResult PrepareImpl(
        const SoundArchive* pSoundArchive,
        const SoundDataManager* pSoundDataManager,
        SoundArchive::ItemId soundId,
        detail::WaveSound* sound,
        const SoundArchive::SoundInfo* commonInfo,
        const StartInfoReader& startInfoReader
    ) NN_NOEXCEPT;

    void DumpMemory(const SoundArchive* pSoundArchive) const NN_NOEXCEPT;

private:
    detail::WaveSoundInstanceManager m_WaveSoundInstanceManager;
    detail::driver::WaveSoundLoaderManager m_WaveSoundLoaderManager;

    // フック
    detail::SoundArchiveFilesHook* m_pSoundArchiveFilesHook;
};

}}} // namespace nn::atk::detail
