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
#include <nn/atk/atk_StartInfoReader.h>
#include <nn/atk/atk_StreamSound.h>

namespace nn { namespace atk {

class SoundDataManager;

namespace detail {

class StreamSoundRuntime
{
public:
    StreamSoundRuntime() NN_NOEXCEPT;
    ~StreamSoundRuntime() NN_NOEXCEPT;

public:
    bool Initialize( int soundCount, void** pOutAllocatedAddr, const void* endAddr, void* streamInstanceBuffer, size_t streamInstanceBufferSize ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    bool SetupStreamBuffer(
        const SoundArchive* pSoundArchive,
        void* strmBuffer,
        size_t strmBufferSize
        ) NN_NOEXCEPT;

    bool SetupStreamBuffer(
        const SoundArchive* pSoundArchive,
        void* strmBuffer,
        size_t strmBufferSize,
        detail::driver::StreamBufferPool* pStreamBufferPool
    ) NN_NOEXCEPT;

    bool SetupStreamCacheBuffer(
        const SoundArchive* pSoundArchive,
        void* streamCacheBuffer,
        size_t streamCacheSize
    ) NN_NOEXCEPT;


    static size_t GetRequiredMemorySize(const SoundArchive::SoundArchivePlayerInfo& soundArchivePlayerInfo, int alignment) NN_NOEXCEPT;
    size_t GetRequiredStreamBufferSize( const SoundArchive* arc ) const NN_NOEXCEPT;
    static int GetRequiredStreamBufferTimes( const SoundArchive* arc ) NN_NOEXCEPT;
    static size_t GetRequiredStreamCacheSize( const SoundArchive* arc, size_t cacheSizePerSound ) NN_NOEXCEPT;
    static size_t GetRequiredStreamInstanceSize(int soundCount) NN_NOEXCEPT;

    void SetSoundArchiveFilesHook(detail::SoundArchiveFilesHook* filesHook)
    {
        m_pSoundArchiveFilesHook = filesHook;
    }

    int GetActiveCount() const NN_NOEXCEPT;
    int GetActiveChannelCount() const NN_NOEXCEPT;
    int GetActiveTrackCount() const NN_NOEXCEPT;
    int GetFreeCount() const NN_NOEXCEPT;

    void SetupUserParam(void** pOutAllocatedAddr, size_t adjustSize) NN_NOEXCEPT;

    void Update() NN_NOEXCEPT;

    StreamSound* AllocSound(
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
        detail::StreamSound* sound,
        const SoundArchive::SoundInfo* commonInfo,
        const StartInfoReader& startInfoReader
    ) NN_NOEXCEPT;

    void DumpMemory(const SoundArchive* pSoundArchive) const NN_NOEXCEPT;
    bool SetStreamBlockCount(int count) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_MINMAX(count, StreamBlockCountMin, DefaultStreamBlockCount);
        if (count >= StreamBlockCountMin && count <= DefaultStreamBlockCount)
        {
            m_StreamBlockCount = count;
            return true;
        }
        return false;
    }
    int GetStreamBlockCount() NN_NOEXCEPT
    {
        return m_StreamBlockCount;
    }

private:
    static const int DefaultStreamBlockCount = 5;

private:
    detail::StreamSoundInstanceManager m_StreamSoundInstanceManager;
    detail::driver::StreamSoundLoaderManager m_StreamSoundLoaderManager;

    detail::driver::StreamBufferPool m_StreamBufferPool;

    // フック
    detail::SoundArchiveFilesHook* m_pSoundArchiveFilesHook;
    int m_StreamBlockCount;
};

}}} // namespace nn::atk::detail
