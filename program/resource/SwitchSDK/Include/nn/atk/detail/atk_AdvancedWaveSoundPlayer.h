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

#include <nn/atk/atk_BasicSoundPlayer.h>
#include <nn/atk/atk_Channel.h>
#include <nn/atk/atk_SoundThread.h>

#include <nn/atk/detail/atk_AdvancedWaveSoundFileReader.h>
//#include <nn/atk/atk_AdvancedWaveSoundLoader.h>

namespace nn { namespace atk { namespace detail { namespace driver {

    class AdvancedWaveSoundPlayer : public BasicSoundPlayer, public SoundThread::PlayerCallback
    {
    public:
        struct PrepareParameter
        {
            SoundArchive::AdvancedWaveSoundInfo advancedWaveSoundInfo;
            UpdateType updateType;
            const void* pAwsdFile;
            const void* pWarcFile;
        };

    public:
        AdvancedWaveSoundPlayer() NN_NOEXCEPT;
        virtual ~AdvancedWaveSoundPlayer() NN_NOEXCEPT;

        virtual void Initialize(OutputReceiver* pOutputReceiver) NN_NOEXCEPT NN_OVERRIDE;
        virtual void Finalize() NN_NOEXCEPT NN_OVERRIDE;

        virtual void Start() NN_NOEXCEPT NN_OVERRIDE;
        virtual void Stop() NN_NOEXCEPT NN_OVERRIDE;
        virtual void Pause( bool isPauseEnabled ) NN_NOEXCEPT NN_OVERRIDE;

        void Prepare(const PrepareParameter& parameter) NN_NOEXCEPT;
        bool IsPrepared() const NN_NOEXCEPT
        {
            return m_IsPrepared;
        }

        /*
        void SetLoaderManager(AdvancedWaveSoundLoaderManager* manager) NN_NOEXCEPT
        {
            m_pLoaderManager = manager;
        }
        */

        UpdateType GetUpdateType() const NN_NOEXCEPT
        {
            return m_UpdateType;
        }

    protected:
        virtual void OnUpdateFrameSoundThread(int updateFrameCount) NN_NOEXCEPT NN_OVERRIDE
        {
            NN_UNUSED(updateFrameCount);
            Update();
        }
        virtual void OnUpdateFrameSoundThreadWithAudioFrameFrequency(int updateFrameCount) NN_NOEXCEPT NN_OVERRIDE
        {
            NN_UNUSED(updateFrameCount);
            if ( m_UpdateType == UpdateType_AudioFrame )
            {
                Update();
            }
        }

    private:
        struct ClipParam
        {
            bool isPlayed;
            driver::Channel* pChannel;
        };

        struct TrackParam
        {
            static const int ClipParamCountMax = AdvancedWaveSoundTrackInfo::AdvancedWaveSoundClipInfoCountMax;

            bool isPlayed;
            ClipParam clipParam[ClipParamCountMax];
        };

        struct TrackParamSet
        {
            static const int TrackParamCountMax = AdvancedWaveSoundTrackInfoSet::AdvancedWaveSoundTrackInfoCountMax;

            bool isPlayed;

            TrackParam trackParam[TrackParamCountMax];
        };

    private:
        virtual void OnShutdownSoundThread() NN_NOEXCEPT NN_OVERRIDE
        {
            Stop();
        }

        void Update() NN_NOEXCEPT;

        void SetupPlayer() NN_NOEXCEPT;
        void TearDownPlayer() NN_NOEXCEPT;

        bool SetupTracks() NN_NOEXCEPT;
        bool UpdateTracks() NN_NOEXCEPT;
        void ReleaseTracks() NN_NOEXCEPT;

        bool StartClip(ClipParam* pClipParam, AdvancedWaveSoundClipInfo* pWaveSoundClipInfo) NN_NOEXCEPT;
        void UpdateClip(ClipParam* pClipParam, AdvancedWaveSoundClipInfo* pWaveSoundClipInfo) NN_NOEXCEPT;
        void ReleaseClip(ClipParam* pClipParam) NN_NOEXCEPT;
        void StopClip(ClipParam* pClipParam) NN_NOEXCEPT;

        void InitializeTrackParams() NN_NOEXCEPT;

    private:
        SoundArchive::AdvancedWaveSoundInfo m_AdvancedWaveSoundInfo;
        AdvancedWaveSoundTrackInfoSet m_AdvancedWaveSoundTrackInfoSet;
        TrackParamSet m_TrackParamSet;

        const void* m_pAwsdFile;
        const void* m_pWarcFile;

        UpdateType m_UpdateType;
        uint32_t m_CurrentTime;

        bool m_IsPrepared;
        bool m_IsInitialized;
        bool m_IsRegisterPlayerCallback;


    };

}}}} // namespace nn::atk::detail::driver

