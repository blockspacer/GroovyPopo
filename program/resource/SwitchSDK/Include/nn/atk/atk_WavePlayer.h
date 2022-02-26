/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * :include nn/atk/atk_WavePlayer.h
 *
 * @file atk_WavePlayer.h
 */

#pragma once

#include <nn/atk/atk_MultiVoice.h>

namespace nn {
namespace atk {

//! @internal
class WavePlayer
{
public:
    typedef void (*VoiceCallback)( void* arg );

    // ------------------------------------------------------------------------
    // 確保と解放
    bool Initialize(
        int channelCount,
        /* int voiceOutCount, */
        int priority,
        VoiceCallback callback, // このコールバックが呼ばれたあとでしか、
                                // WaveBuffer::bufferAddress 上のデータの解放および
                                // Voice インスタンスを使い回しをしてはいけない
        void* callbackArg ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // 再生制御
    void Start() NN_NOEXCEPT;
    void Stop() NN_NOEXCEPT;
    void Pause( bool isPauseOn ) NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // 再生状態
    bool IsActive() const NN_NOEXCEPT;
    bool IsRun() const NN_NOEXCEPT;
    bool IsPause() const NN_NOEXCEPT;
    detail::position_t GetCurrentPlayingSample() const NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // 波形データ設定
    void SetWaveInfo( SampleFormat format, int sampleRate, int interpolationType ) NN_NOEXCEPT;
    void SetAdpcmParam( int channel, /*int voiceOut,*/const AdpcmParam& param ) NN_NOEXCEPT;
    void AppendWaveBuffer( int channel, /*int voiceOut,*/ WaveBuffer* pBuffer, bool lastFlag ) NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // パラメータ設定
    void SetPriority( int priority ) NN_NOEXCEPT;
    void SetVolume( float volume ) NN_NOEXCEPT;
    void SetPitch( float pitch ) NN_NOEXCEPT;
    void SetPanMode( PanMode mode ) NN_NOEXCEPT;
    void SetPanCurve( PanCurve curve ) NN_NOEXCEPT;
    void SetPan( float pan ) NN_NOEXCEPT;
    void SetSurroundPan( float span ) NN_NOEXCEPT;
    void SetLpfFreq( float lpfFreq ) NN_NOEXCEPT;
    void SetBiquadFilter( int type, float value ) NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // 出力パラメータ
    void SetOutputLine( uint32_t lineFlag ) NN_NOEXCEPT;
    void SetMainOutVolume( float volume ) NN_NOEXCEPT;
    void SetRemoteOutVolume( int remoteIndex, float volume ) NN_NOEXCEPT;
    void detail_SetDrcOutVolume( int drcIndex, float volume ) NN_NOEXCEPT; // SDK 対応まで非公開
    void SetDrcOutVolume( float volume ) NN_NOEXCEPT
    {
        detail_SetDrcOutVolume( 0, volume );
    }
    void SetMainSend( float send ) NN_NOEXCEPT;
    void SetFxSend( AuxBus bus, float send ) NN_NOEXCEPT;


private:
    static void MultiVoiceCallbackFunc(
        detail::driver::MultiVoice* voice,
        detail::driver::MultiVoice::VoiceCallbackStatus status,
        void* arg ) NN_NOEXCEPT;

    detail::driver::MultiVoice* m_pMultiVoice;
    VoiceCallback m_Callback;
    void* m_pCallbackArg;
};

} // namespace nn::atk
} // namespace nn

