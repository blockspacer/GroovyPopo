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

#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_SoundInstanceManager.h>
#include <nn/atk/atk_BasicSound.h>
#include <nn/atk/atk_StreamSoundPlayer.h>
#include <nn/atk/atk_StreamBufferPool.h>
#include <nn/atk/atk_Debug.h>

namespace nn {
namespace atk {

class StreamSoundHandle;

namespace detail {

class StreamSound;
typedef SoundInstanceManager<StreamSound> StreamSoundInstanceManager;

class StreamSound : public BasicSound
{
    friend class nn::atk::StreamSoundHandle;

public:
    NN_ATK_DETAIL_RUNTIME_TYPEINFO(BasicSound)

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    explicit StreamSound( StreamSoundInstanceManager& manager ) NN_NOEXCEPT;

    void SetCacheBuffer(void* cache, size_t cacheSize) NN_NOEXCEPT
    {
        m_pCacheBuffer = cache;
        m_CacheSize = cacheSize;
    }
    bool IsCacheAvailable() const NN_NOEXCEPT
    {
        if (m_pCacheBuffer != NULL && m_CacheSize != 0)
        {
            return true;
        }
        return false;
    }
    void* GetCacheBuffer() NN_NOEXCEPT { return m_pCacheBuffer; }
    size_t GetCacheSize() NN_NOEXCEPT { return m_CacheSize; }

    void Setup(const driver::StreamSoundPlayer::SetupArg& arg) NN_NOEXCEPT;
    void Prepare(const driver::StreamSoundPlayer::PrepareBaseArg& arg) NN_NOEXCEPT;
    void PreparePrefetch(const void* strmPrefetchFile, const driver::StreamSoundPlayer::PrepareBaseArg& arg) NN_NOEXCEPT;

    virtual bool Initialize(OutputReceiver* pOutputReceiver) NN_NOEXCEPT NN_OVERRIDE;
    virtual void Finalize() NN_NOEXCEPT NN_OVERRIDE;
    virtual bool IsPrepared() const NN_NOEXCEPT NN_OVERRIDE;
    bool IsSuspendByLoadingDelay() const NN_NOEXCEPT;
    bool IsLoadingDelayState() const NN_NOEXCEPT;

    // トラックパラメータ
    void SetTrackVolume( uint32_t trackBitFlag, float volume, int frames = 0 ) NN_NOEXCEPT;
    void SetTrackInitialVolume( uint32_t trackBitFlag, uint32_t volume ) NN_NOEXCEPT;
    void SetTrackOutputLine( uint32_t trackBitFlag, uint32_t lineFlag ) NN_NOEXCEPT;
    void ResetTrackOutputLine( uint32_t trackBitFlag ) NN_NOEXCEPT;

    void SetTrackChannelMixParameter( uint32_t trackBitFlag, uint32_t srcChNo, const MixParameter& mixParam ) NN_NOEXCEPT;
    void SetTrackMainOutVolume( uint32_t trackBitFlag, float volume ) NN_NOEXCEPT;
    void SetTrackPan( uint32_t trackBitFlag, float pan ) NN_NOEXCEPT;
    void SetTrackSurroundPan( uint32_t trackBitFlag, float span ) NN_NOEXCEPT;
    void SetTrackMainSend( uint32_t trackBitFlag, float send ) NN_NOEXCEPT;
    void SetTrackFxSend( uint32_t trackBitFlag, AuxBus bus, float send ) NN_NOEXCEPT;

    // 情報取得
    bool ReadStreamSoundDataInfo( StreamSoundDataInfo* info ) const NN_NOEXCEPT;
    long GetPlayLoopCount() const NN_NOEXCEPT;
    position_t GetPlaySamplePosition(bool isOriginalSamplePosition) const NN_NOEXCEPT;
    inline uint32_t GetAvailableTrackBitFlag( uint32_t srcChNo ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT( srcChNo < WaveChannelMax );
        return m_AvailableTrackBitFlag[ srcChNo ];
    }
    float GetFilledBufferPercentage() const NN_NOEXCEPT;
    int GetBufferBlockCount(nn::atk::WaveBuffer::Status status) const NN_NOEXCEPT;
    int GetTotalBufferBlockCount() const NN_NOEXCEPT;
    int GetActiveChannelCount() const NN_NOEXCEPT
    {
        return m_PlayerInstance.GetActiveChannelCount();
    }
    int GetActiveTrackCount() const NN_NOEXCEPT
    {
        return m_PlayerInstance.GetActiveTrackCount();
    }

    // デバッグ関数
    DebugSoundType GetSoundType() const NN_NOEXCEPT { return DebugSoundType_Strmsound; }

    void SetLoaderManager(driver::StreamSoundLoaderManager& manager) NN_NOEXCEPT
    {
        m_PlayerInstance.SetLoaderManager(&manager);
    }

    nn::os::Tick GetProcessTick(const SoundProfile& profile) NN_NOEXCEPT
    {
        return m_PlayerInstance.GetProcessTick(profile);
    }

    void* detail_SetFsAccessLog( fnd::FsAccessLog* pFsAccessLog ) NN_NOEXCEPT
    {
        return m_PlayerInstance.detail_SetFsAccessLog(pFsAccessLog);
    }

    util::IntrusiveListNode m_PriorityLink; // for SoundInstanceManager

protected:
    bool IsAttachedTempSpecialHandle() NN_NOEXCEPT NN_OVERRIDE;
    void DetachTempSpecialHandle() NN_NOEXCEPT NN_OVERRIDE;

    void UpdateMoveValue() NN_NOEXCEPT NN_OVERRIDE;

    driver::BasicSoundPlayer* GetBasicSoundPlayerHandle() NN_NOEXCEPT NN_OVERRIDE
    {
        return &m_PlayerInstance;
    }

    virtual void OnUpdatePlayerPriority() NN_NOEXCEPT NN_OVERRIDE;

private:
    void OnUpdateParam() NN_NOEXCEPT NN_OVERRIDE;
    StreamSoundHandle* m_pTempSpecialHandle;
    StreamSoundInstanceManager& m_Manager;

    MoveValue<float, int> m_TrackVolume[ StreamTrackCount ];

    uint16_t m_AllocTrackFlag;
    bool m_InitializeFlag;
    uint8_t m_Padding[1];

    uint32_t m_AvailableTrackBitFlag[ WaveChannelMax ];

    void* m_pCacheBuffer;
    size_t m_CacheSize;
    NN_AUDIO_ALIGNAS_BUFFER_ALIGN driver::StreamSoundPlayer m_PlayerInstance;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

