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

#include <nn/atk/atk_MultiVoice.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

/* ========================================================================
        class definition
   ======================================================================== */

class MultiVoiceManager
{
    friend class MultiVoice;

public:
    typedef util::IntrusiveList<MultiVoice, util::IntrusiveListMemberNodeTraits<MultiVoice,&MultiVoice::m_LinkNode>> VoiceList;
    typedef util::IntrusiveList<OutputBusMixVolume, util::IntrusiveListMemberNodeTraits<MultiVoice,&MultiVoice::m_LinkNode>> BusMixParamList;

public:
    static NN_NOINLINE MultiVoiceManager& GetInstance() NN_NOEXCEPT;

    size_t GetObjectSize( const detail::SoundInstanceConfig& config ) NN_NOEXCEPT;
    size_t GetRequiredMemSize( int voiceCount, const detail::SoundInstanceConfig& config ) NN_NOEXCEPT;
    void Initialize( void* mem, size_t memSize, const detail::SoundInstanceConfig& config ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // ボイス管理
    MultiVoice* AllocVoice(
        int voiceChannelCount,
        int priority,
        MultiVoice::VoiceCallback callback,
        void* callbackData
    ) NN_NOEXCEPT;
    void FreeVoice( MultiVoice* voice ) NN_NOEXCEPT;

    void StopAllVoices() NN_NOEXCEPT;

    void UpdateAllVoices() NN_NOEXCEPT;
    void UpdateAllVoiceStatus() NN_NOEXCEPT;
    void UpdateAudioFrameVoices() NN_NOEXCEPT;
    void UpdateAudioFrameVoiceStatus() NN_NOEXCEPT;
    void UpdateAllVoicesSync( uint32_t syncFlag ) NN_NOEXCEPT;

    int GetVoiceCount() const NN_NOEXCEPT;
    unsigned long GetActiveCount() const NN_NOEXCEPT;
    unsigned long GetFreeCount() const NN_NOEXCEPT;
    const VoiceList& GetVoiceList() const NN_NOEXCEPT;

private:
    MultiVoiceManager() NN_NOEXCEPT;

    void ChangeVoicePriority( MultiVoice* voice ) NN_NOEXCEPT;
    int DropLowestPriorityVoice( int priority ) NN_NOEXCEPT;
    void UpdateEachVoicePriority(
        const VoiceList::iterator& beginItr,
        const VoiceList::iterator& endItr
    ) NN_NOEXCEPT;

    void AppendVoiceList( MultiVoice* voice ) NN_NOEXCEPT;
    void RemoveVoiceList( MultiVoice* voice ) NN_NOEXCEPT;

    bool m_Initialized;
    VoiceList m_PrioVoiceList; // lower priority order
    VoiceList m_FreeVoiceList;
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

