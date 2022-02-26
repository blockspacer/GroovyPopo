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

#include <cstdint>

#include <nn/audio/audio_VoiceTypes.h>

namespace nn {
namespace audio {

struct MixInfo;

/**
 * @brief       FinalMix を表す構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * (確保・解放)
 * - @ref AcquireFinalMix()
 * - @ref ReleaseFinalMix()
 *
 * (情報取得)
 * - @ref GetFinalMixBufferCount()
 * - @ref GetFinalMixNodeId()
 *
 * (音量)
 * - @ref GetFinalMixVolume()
 * - @ref SetFinalMixVolume()
 *
 * (Sink)
 * - @ref AddDeviceSink()
 * - @ref RemoveDeviceSink()
 *
 * (SubMix)
 * - @ref SetSubMixDestination(AudioRendererConfig *pOutConfig, SubMixType *pSource, FinalMixType *pDestination)
 * - @ref SetSubMixMixVolume(SubMixType *pSource, FinalMixType *pDestination, float volume, int sourceIndex, int destinationIndex)
 * - @ref GetSubMixMixVolume(const SubMixType *pSource, const FinalMixType *pDestination, int sourceIndex, int destinationIndex)
 *
 * (Voice)
 * - @ref SetVoiceDestination(AudioRendererConfig *pOutConfig, VoiceType *pSource, FinalMixType *pDestination)
 * - @ref SetVoiceMixVolume(VoiceType *pSource, FinalMixType *pDestination, float volume, int sourceIndex, int destinationIndex)
 * - @ref GetVoiceMixVolume(const VoiceType *pSource, const FinalMixType *pDestination, int sourceIndex, int destinationIndex)
 *
 * (エフェクト)
 * - @ref AddBufferMixer(AudioRendererConfig *pOutConfig, BufferMixerType *pBufferMixer, FinalMixType *pFinalMix)
 * - @ref RemoveBufferMixer(AudioRendererConfig *pOutConfig, BufferMixerType *pBufferMixer, FinalMixType *pFinalMix)
 * - @ref AddDelay(AudioRendererConfig *pOutConfig, DelayType *pDelay, void* buffer, size_t bufferSize, FinalMixType *pFinalMix, nn::TimeSpan delayTimeMax, int channelCountMax)
 * - @ref RemoveDelay(AudioRendererConfig *pOutConfig, DelayType *pDelay, FinalMixType *pFinalMix)
 * - @ref AddReverb(AudioRendererConfig *pOutConfig, ReverbType *pReverb, void* buffer, size_t bufferSize, FinalMixType *pFinalMix, int channelCount)
 * - @ref RemoveReverb(AudioRendererConfig *pOutConfig, ReverbType *pReverb, FinalMixType *pFinalMix)
 * - @ref AddAux(AudioRendererConfig *pOutConfig, AuxType *pAux, void* buffer, size_t bufferSize, FinalMixType *pFinalMix, void *pSendBuffer, void *pReturnBuffer, size_t bufferSize)
 * - @ref RemoveAux(AudioRendererConfig *pOutConfig, AuxType *pAux, FinalMixType *pFinalMix)
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct FinalMixType
{
    /**
     * @brief       設定可能なボリュームの最大値 (128.0f) を返します。
     */
    static inline float GetVolumeMax() NN_NOEXCEPT
    {
        return 128.0f;
    }

    /**
     * @brief       設定可能なボリュームの最小値 (0.0f) を返します。
     */
    static inline float GetVolumeMin() NN_NOEXCEPT
    {
        return 0.0f;
    }

    MixInfo* _pMixInfo;
};

}  // namespace audio
}  // namespace nn
