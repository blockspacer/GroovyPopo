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

#include <nn/nn_Macro.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SoundStartable.h>

namespace nn { namespace atk { namespace detail {

    class StartInfoReader
    {
    public:
        explicit StartInfoReader(const SoundArchive::SoundInfo& soundInfo) NN_NOEXCEPT;

        void Read(const SoundStartable::StartInfo* startInfo) NN_NOEXCEPT;

        SoundStartable::StartInfo::StartOffsetType GetStartOffsetType() const NN_NOEXCEPT
        {
            return m_StartOffsetType;
        }

        int GetStartOffset() const NN_NOEXCEPT
        {
            return m_StartOffset;
        }


        int GetDelayTime() const NN_NOEXCEPT
        {
            return m_DelayTime;
        }

        int GetDelayCount() const NN_NOEXCEPT
        {
            return m_DelayCount;
        }

        UpdateType GetUpdateType() const NN_NOEXCEPT
        {
            return m_UpdateType;
        }

        int GetPlayerPriority() const NN_NOEXCEPT
        {
            return m_PlayerPriority;
        }

        SoundArchive::ItemId GetPlayerId() const NN_NOEXCEPT
        {
            return m_PlayerId;
        }

        int GetActorPlayerId() const NN_NOEXCEPT
        {
            return m_ActorPlayerId;
        }

        const SoundStartable::StartInfo::SequenceSoundInfo* GetSeqInfo() const NN_NOEXCEPT
        {
            return m_pSeqInfo;
        }

        const SoundStartable::StartInfo::StreamSoundInfo* GetStrmInfo() const NN_NOEXCEPT
        {
            return m_pStrmInfo;
        }

        const SoundArchive::StreamSoundInfo* GetStrmMetaInfo() const NN_NOEXCEPT
        {
            return m_pStrmMetaInfo;
        }

        const SoundArchive::StreamSoundInfo2* GetStrmMetaInfo2() const NN_NOEXCEPT
        {
            return m_pStrmMetaInfo2;
        }

        const SoundStartable::StartInfo::LoopInfo* GetLoopInfo() const NN_NOEXCEPT
        {
            return m_pLoopInfo;
        }

        const SoundStartable::StartInfo::WaveSoundInfo* GetWsdInfo() const NN_NOEXCEPT
        {
            return m_pWsdInfo;
        }

        int GetSubMixIndex() const NN_NOEXCEPT
        {
            return m_SubMixIndex;
        }

        OutputReceiver* GetOutputReceiver() const NN_NOEXCEPT
        {
            return m_pOutputReceiver;
        }

        bool IsAdditionalDecodingOnLoopEnabled() const NN_NOEXCEPT
        {
            return m_IsAdditionalDecodingOnLoopEnabled;
        }

    private:
        SoundStartable::StartInfo::StartOffsetType m_StartOffsetType;
        int m_StartOffset;
        int m_DelayTime;
        int m_DelayCount;
        UpdateType m_UpdateType;
        int m_PlayerPriority;
        SoundArchive::ItemId m_PlayerId;
        int m_ActorPlayerId;
        const SoundStartable::StartInfo::SequenceSoundInfo* m_pSeqInfo;
        const SoundStartable::StartInfo::StreamSoundInfo* m_pStrmInfo;
        const SoundArchive::StreamSoundInfo* m_pStrmMetaInfo;
        const SoundArchive::StreamSoundInfo2* m_pStrmMetaInfo2;
        const SoundStartable::StartInfo::WaveSoundInfo* m_pWsdInfo;
        const SoundStartable::StartInfo::LoopInfo* m_pLoopInfo;
        int m_SubMixIndex;
        OutputReceiver* m_pOutputReceiver;
        bool m_IsAdditionalDecodingOnLoopEnabled;

    };

}}} // namespace nn::atk::detail
