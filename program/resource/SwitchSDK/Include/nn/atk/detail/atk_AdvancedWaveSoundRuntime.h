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
#include <nn/atk/atk_SoundArchive.h>

#include <nn/atk/detail/atk_AdvancedWaveSound.h>

namespace nn { namespace atk {

    class SoundDataManager;

    namespace detail {

        class AdvancedWaveSound;
        class StartInfoReader;

    }
}}

namespace nn { namespace atk { namespace detail {

    class AdvancedWaveSoundRuntime
    {
    public:
        AdvancedWaveSoundRuntime() NN_NOEXCEPT;
        ~AdvancedWaveSoundRuntime() NN_NOEXCEPT;

    public:
        bool Initialize( int soundCount, void** pOutAllocatedAddr, const void* endAddr ) NN_NOEXCEPT;
        void Finalize() NN_NOEXCEPT;

        static size_t GetRequiredMemorySize(const SoundArchive::SoundArchivePlayerInfo& soundArchivePlayerInfo, size_t alignmentSize) NN_NOEXCEPT;

        int GetFreeAdvancedWaveSoundCount() const NN_NOEXCEPT;

        void SetupUserParam(void** startAddr, size_t adjustSize) NN_NOEXCEPT;

        void Update() NN_NOEXCEPT;

        AdvancedWaveSound* AllocSound(
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
            detail::AdvancedWaveSound* sound,
            const SoundArchive::SoundInfo* commonInfo,
            const StartInfoReader& startInfoReader
        ) NN_NOEXCEPT;

        void DumpMemory(const SoundArchive* pSoundArchive) const NN_NOEXCEPT;

    private:
        detail::AdvancedWaveSoundInstanceManager m_InstanceManager;
        //detail::driver::AdvancedWaveSoundLoaderManager m_LoaderManager;
    };

}}} // namespace nn::atk::detail
