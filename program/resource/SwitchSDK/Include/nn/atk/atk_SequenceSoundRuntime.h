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

#include <nn/atk/atk_MmlParser.h>
#include <nn/atk/atk_MmlSequenceTrackAllocator.h>
#include <nn/atk/atk_SoundStartable.h>
#include <nn/atk/atk_SoundInstanceManager.h>
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SequenceSound.h>
#include <nn/atk/atk_SequenceSoundPlayer.h>
#include <nn/atk/detail/atk_SoundArchiveManager.h>

namespace nn { namespace atk {

    class SoundArchive;
    class SoundDataManager;

    namespace detail {

        class SequenceSound;
        class StartInfoReader;
        struct SequenceSoundFile;

    }
}}

namespace nn { namespace atk { namespace detail {

class SequenceSoundRuntime
{
public:
    SequenceSoundRuntime() NN_NOEXCEPT;
    ~SequenceSoundRuntime() NN_NOEXCEPT;

    bool Initialize( int soundCount, void** pOutAllocatedAddr, const void* endAddr ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    bool SetupSequenceTrack(
        int trackCount,
        void** pOutAllocatedAddr,
        const void* endAddr
    ) NN_NOEXCEPT;

    void SetupUserParam(void** pOutAllocatedAddr, size_t adjustSize) NN_NOEXCEPT;

    static size_t GetRequiredMemorySize(const SoundArchive::SoundArchivePlayerInfo& soundArchivePlayerInfo, int alignment) NN_NOEXCEPT;
    static size_t GetRequiredSequenceTrackMemorySize(const SoundArchive::SoundArchivePlayerInfo& soundArchivePlayerInfo, int alignment) NN_NOEXCEPT;

    void SetSoundArchiveManager(const SoundArchiveManager* pSoundArchiveManager) NN_NOEXCEPT
    {
        m_pSoundArchiveManager = pSoundArchiveManager;
    }

    void SetSoundArchiveFilesHook(detail::SoundArchiveFilesHook* filesHook)
    {
        m_pSoundArchiveFilesHook = filesHook;
    }

    void SetMmlParser( detail::driver::MmlParser* parser ) NN_NOEXCEPT
    {
        if ( parser == NULL )
        {
            m_MmlSequenceTrackAllocator.SetMmlParser( &m_MmlParser );
        }
        else
        {
            m_MmlSequenceTrackAllocator.SetMmlParser( parser );
        }
    }

    void SetSequenceUserProcCallback(
            SequenceUserProcCallback callback,
            void* callbackArg
    ) NN_NOEXCEPT
    {
        m_SequenceUserProcCallback = callback;
        m_pSequenceUserProcCallbackArg = callbackArg;
    }

    bool IsSoundArchiveAvailable() const NN_NOEXCEPT;

    int GetActiveCount() const NN_NOEXCEPT;
    int GetFreeCount() const NN_NOEXCEPT;

    static void SetSequenceSkipIntervalTick( int intervalTick ) NN_NOEXCEPT;
    static int GetSequenceSkipIntervalTick() NN_NOEXCEPT;

    void Update() NN_NOEXCEPT;

    SequenceSound* AllocSound(
        SoundArchive::ItemId soundId,
        int priority,
        int ambientPriority,
        detail::BasicSound::AmbientInfo* ambientArgInfo,
        OutputReceiver* pOutputReceiver
    ) NN_NOEXCEPT;

    SoundStartable::StartResult PrepareImpl(
        const SoundArchiveManager::SnapShot& snapShot,
        SoundArchive::ItemId soundId,
        detail::SequenceSound* sound,
        const SoundArchive::SoundInfo* commonInfo,
        const StartInfoReader& startInfoReader
    ) NN_NOEXCEPT;

    void DumpMemory(const SoundArchive* pSoundArchive) const NN_NOEXCEPT;

private:
    class SequenceNoteOnCallback : public detail::driver::NoteOnCallback
    {
        NN_DISALLOW_COPY(SequenceNoteOnCallback);

    public:
        SequenceNoteOnCallback() NN_NOEXCEPT : m_pSequenceSoundRuntime( NULL ) { }
        void Initialize( const SequenceSoundRuntime& owner ) NN_NOEXCEPT
        {
            m_pSequenceSoundRuntime = &owner;
        }

        virtual detail::driver::Channel* NoteOn(
            detail::driver::SequenceSoundPlayer* seqPlayer,
            uint8_t bankIndex,
            const detail::driver::NoteOnInfo& noteOnInfo
        ) NN_NOEXCEPT;

    private:
        const SequenceSoundRuntime* m_pSequenceSoundRuntime;
    };
    friend class SequenceSoundRuntime::SequenceNoteOnCallback;

    struct PrepareContext
    {
        const detail::SequenceSoundFile* pSequenceSoundFile;
        uint32_t sequenceOffset;
        uint32_t allocateTrackFlags;
        detail::LoadItemInfo loadTargetSequenceInfo;
        detail::LoadItemInfo loadTargetBankInfos[SoundArchive::SequenceBankMax];
        detail::LoadItemInfo loadTargetWaveArchiveInfos[SoundArchive::SequenceBankMax]; // 現状は、バンクの数と同数
        bool isLoadIndividuals[SoundArchive::SequenceBankMax];
        bool canUsePlayerHeap;
        bool isRegisterDataLoadTaskNeeded;

        void Initialize() NN_NOEXCEPT
        {
            pSequenceSoundFile = nullptr;
            sequenceOffset = 0;
            allocateTrackFlags = 0;

            for ( auto i = 0; i < SoundArchive::SequenceBankMax; ++i )
            {
                isLoadIndividuals[i] = false;
            }

            canUsePlayerHeap = false;
            isRegisterDataLoadTaskNeeded = false;
        }
    };

private:
    bool SetupSequenceSoundInfo(
        SoundArchive::SequenceSoundInfo* pOutInfo,
        SoundArchive::ItemId soundId,
        const SoundArchive& soundArchive,
        const SoundStartable::StartInfo::SequenceSoundInfo* pExternalSequenceSoundInfo) NN_NOEXCEPT;

    SoundStartable::StartResult SetupSequenceSoundFile(
        PrepareContext* pOutContext,
        const detail::SequenceSound& sound,
        const SoundArchive& soundArchive,
        const SoundDataManager& soundDataManager,
        const SoundArchive::SoundInfo& soundInfo,
        const SoundStartable::StartInfo::SequenceSoundInfo* pExternalSequenceSoundInfo) NN_NOEXCEPT;

    SoundStartable::StartResult SetupBankFileAndWaveArchiveFile(
        PrepareContext* pOutContext,
        const detail::SequenceSound& sound,
        const SoundArchive::SequenceSoundInfo& sequenceSoundInfo,
        const SoundArchiveManager::SnapShot& snapShot,
        const SoundStartable::StartInfo::SequenceSoundInfo* pExternalSequenceSoundInfo) NN_NOEXCEPT;

    void SetupBankFileAndWaveArchiveFileFromHook(
        PrepareContext* pOutContext,
        const detail::SequenceSound& sound,
        const SoundArchive& soundArchive) NN_NOEXCEPT;

    void SetupSequenceSoundPlayerStartInfo(
        detail::driver::SequenceSoundPlayer::StartInfo* pOutStartInfo,
        uint32_t sequenceOffset,
        const StartInfoReader& startInfoReader) NN_NOEXCEPT;

private:
    detail::SequenceSoundInstanceManager m_SequenceSoundInstanceManager;
    detail::driver::SequenceSoundLoaderManager m_SequenceSoundLoaderManager;

    detail::driver::SequenceTrackAllocator* m_pSequenceTrackAllocator;
    detail::driver::MmlSequenceTrackAllocator m_MmlSequenceTrackAllocator; // デフォルトのシーケンストラックアロケータ

    // デフォルトのシーケンスパーサ
    detail::driver::MmlParser m_MmlParser;

    // コールバック
    SequenceNoteOnCallback m_SequenceCallback;
    SequenceUserProcCallback m_SequenceUserProcCallback;
    void* m_pSequenceUserProcCallbackArg;

    const SoundArchiveManager* m_pSoundArchiveManager;

    // フック
    detail::SoundArchiveFilesHook* m_pSoundArchiveFilesHook;
};

}}} // namespace nn::atk::detail
