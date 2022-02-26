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

#include <nn/atk/atk_BasicSound.h>
#include <nn/atk/atk_SoundInstanceManager.h>
#include <nn/atk/atk_Debug.h>
#include <nn/atk/atk_Task.h>

#include <nn/atk/detail/atk_AdvancedWaveSoundPlayer.h>

namespace nn { namespace atk {
    class AdvancedWaveSoundHandle;
}}

namespace nn { namespace atk { namespace detail {

    class AdvancedWaveSound;

    typedef SoundInstanceManager<AdvancedWaveSound> AdvancedWaveSoundInstanceManager;

    class AdvancedWaveSound : public BasicSound
    {
        friend class nn::atk::AdvancedWaveSoundHandle;

    public:
        explicit AdvancedWaveSound( AdvancedWaveSoundInstanceManager& manager ) NN_NOEXCEPT;
        virtual ~AdvancedWaveSound() NN_NOEXCEPT;

        virtual bool Initialize(OutputReceiver* pOutputReceiver) NN_NOEXCEPT NN_OVERRIDE;
        virtual void Finalize() NN_NOEXCEPT NN_OVERRIDE;

        virtual bool IsPrepared() const NN_NOEXCEPT NN_OVERRIDE
        {
            if ( !IsPlayerAvailable() )
            {
                return false;
            }

            return m_PlayerInstance.IsPrepared();
        }

        /*
        void SetLoaderManager(driver::AdvancedWaveSoundLoaderManager& manager) NN_NOEXCEPT
        {
            m_PlayerInstance.SetLoaderManager(&manager);
        }
        */

        void Prepare(const driver::AdvancedWaveSoundPlayer::PrepareParameter& parameter) NN_NOEXCEPT;

        // デバッグ関数
        DebugSoundType GetSoundType() const NN_NOEXCEPT
        {
            return DebugSoundType_Wavesound;
        }

    private:
        virtual bool IsAttachedTempSpecialHandle() NN_NOEXCEPT NN_OVERRIDE;
        virtual void DetachTempSpecialHandle() NN_NOEXCEPT NN_OVERRIDE;
        virtual void OnUpdatePlayerPriority() NN_NOEXCEPT NN_OVERRIDE;

        virtual driver::BasicSoundPlayer* GetBasicSoundPlayerHandle() NN_NOEXCEPT NN_OVERRIDE
        {
            return &m_PlayerInstance;
        }

    public:
        util::IntrusiveListNode m_PriorityLink; // for SoundInstanceManager

    private:
        AdvancedWaveSoundHandle* m_pTempSpecialHandle;
        AdvancedWaveSoundInstanceManager& m_InstanceManager;

        driver::AdvancedWaveSoundPlayer m_PlayerInstance;

        bool m_IsInitialized;
        uint8_t m_Padding[3];
    };

}}} // namespace nn::atk::detail
