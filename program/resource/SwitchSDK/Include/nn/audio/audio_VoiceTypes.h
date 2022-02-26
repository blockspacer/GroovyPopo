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

#include <nn/audio/audio_Common.h>
#include <nn/audio/audio_SampleFormat.h>
#include <nn/audio/audio_AudioRendererTypes.h>
#include <nn/audio/audio_WaveBuffer.h>

namespace nn {
namespace audio {

struct VoiceInfo;

/**
 * @brief       biquad フィルタのパラメータです。
 */
struct BiquadFilterParameter
{
    bool enable;                        //!< 有効・無効フラグ
    int16_t numerator[3];               //!< 分子係数
    int16_t denominator[2];             //!< 分母係数
};

/**
 * @brief       レンダラへの入力波形をコントロールするための構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * (確保・解放)
 * - @ref AcquireVoiceSlot()
 * - @ref ReleaseVoiceSlot()
 * - @ref IsVoiceValid()
 *
 * (情報取得)
 * - @ref GetVoiceSampleRate()
 * - @ref GetVoiceChannelCount()
 * - @ref GetVoiceSampleFormat()
 * - @ref GetVoicePriority()
 * - @ref GetVoiceNodeId()
 *
 * (出力先)
 * - @ref SetVoiceDestination(AudioRendererConfig *pOutConfig, VoiceType *pSource, SubMixType *pDestination)
 * - @ref SetVoiceDestination(AudioRendererConfig *pOutConfig, VoiceType *pSource, FinalMixType *pDestination)
 *
 * (再生・停止)
 * - @ref SetVoicePlayState()
 * - @ref GetVoicePlayState()
 *
 * (パラメータ)
 * - @ref SetVoicePitch()
 * - @ref GetVoicePitch()
 * - @ref SetVoiceVolume()
 * - @ref GetVoiceVolume()
 * - @ref SetVoiceMixVolume(VoiceType *pSource, SubMixType *pDestination, float volume, int sourceIndex, int destinationIndex)
 * - @ref SetVoiceMixVolume(VoiceType *pSource, FinalMixType *pDestination, float volume, int sourceIndex, int destinationIndex)
 * - @ref GetVoiceMixVolume(const VoiceType *pSource, const SubMixType *pDestination, int sourceIndex, int destinationIndex)
 * - @ref GetVoiceMixVolume(const VoiceType *pSource, const FinalMixType *pDestination, int sourceIndex, int destinationIndex)
 * - @ref SetVoiceBiquadFilterParameter()
 * - @ref GetVoiceBiquadFilterParameter()
 *
 * (サンプルデータの供給)
 * - @ref AppendWaveBuffer()
 * - @ref GetReleasedWaveBuffer()
 * - @ref GetVoicePlayedSampleCount()
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct VoiceType
{
    /**
     * @brief       再生状態を表す列挙型です。
     */
    enum PlayState
    {
        PlayState_Play,
        PlayState_Stop,
        PlayState_Pause
    };

    /**
     * @brief       最高優先度
     * @details     最高優先度が設定されたボイスは、ボイスドロップの対象から除外されます。
     */
    static const int PriorityHighest = 0;

    /**
    * @brief        最低優先度
    */
    static const int PriorityLowest = 255;

    /**
     * @brief       VoiceType に追加可能な WaveBuffer の最大数
     */
    static const int WaveBufferCountMax = 4;

    /**
     * @brief       VoiceType に設定可能なチャンネルの最大数
     */
    static const int ChannelCountMax = 6;

    /**
     * @brief       設定可能なピッチの最大値 (16.0f) を返します。
     */
    static inline float GetPitchMax() NN_NOEXCEPT
    {
        return 16.0f;
    }

    /**
     * @brief       設定可能なピッチの最小値 (1.0f / 16.0f) を返します。
     */
    static inline float GetPitchMin() NN_NOEXCEPT
    {
        return 1.0f / 16.0f;
    }

    /**
     * @brief       設定可能なボリュームおよびミックスボリュームの最大値 (128.0f) を返します。
     */
    static inline float GetVolumeMax() NN_NOEXCEPT
    {
        return 128.0f;
    }

    /**
     * @brief       設定可能なボリュームおよびミックスボリュームの最小値 (-128.0f) を返します。
     */
    static inline float GetVolumeMin() NN_NOEXCEPT
    {
        return -128.0f;
    }

    /**
     * @brief       BiquadFilter の数
     */
    static const int BiquadFilterCountMax = 2;


    /**
     * @brief       VoiceType の挙動を指定するオプション群です。
     */
    struct BehaviorOptions
    {
        bool isPlayedSampleCountResetAtLoopPoint; //!< nn::audio::GetVoicePlayedSampleCount() で取得されるサンプル数をループ時に 0 にクリアします。
        bool isPitchAndSrcSkipped;                //!< ピッチ変換とサンプルレート変換 (SRC) をスキップします。この設定が有効な時、 nn::audio::AcquireVoiceSlot() の引数に指定したサンプルレートとボイスの接続先に設定されたサンプルレートは同一である必要があります。
    };

    VoiceInfo* _pVoiceInfo;
};

}  // namespace audio
}  // namespace nn
