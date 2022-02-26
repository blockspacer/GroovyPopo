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
#include <nn/audio/audio_FinalMixTypes.h>

namespace nn {
namespace audio {

struct MixInfo;

/**
 * @brief       SubMix を表す構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * (確保・解放)
 * - @ref AcquireSubMix()
 * - @ref ReleaseSubMix()
 *
 * (情報取得)
 * - @ref GetSubMixSampleRate()
 * - @ref GetSubMixBufferCount()
 * - @ref GetSubMixNodeId()
 *
 * (音量・ミックスボリューム)
 * - @ref GetSubMixVolume()
 * - @ref SetSubMixVolume()
 * - @ref SetSubMixMixVolume(SubMixType *pSource, SubMixType *pDestination, float volume, int sourceIndex, int destinationIndex)
 * - @ref SetSubMixMixVolume(SubMixType *pSource, FinalMixType *pDestination, float volume, int sourceIndex, int destinationIndex)
 * - @ref GetSubMixMixVolume(const SubMixType *pSource, const SubMixType *pDestination, int sourceIndex, int destinationIndex)
 * - @ref GetSubMixMixVolume(const SubMixType *pSource, const FinalMixType *pDestination, int sourceIndex, int destinationIndex)
 *
 * (出力先)
 * - @ref SetSubMixDestination(AudioRendererConfig *pOutConfig, SubMixType *pSource, FinalMixType *pDestination)
 * - @ref SetSubMixDestination(AudioRendererConfig *pOutConfig, SubMixType *pSource, SubMixType *pDestination)
 *
 * (Voice)
 * - @ref SetVoiceDestination(AudioRendererConfig *pOutConfig, VoiceType *pSource, SubMixType *pDestination)
 * - @ref SetVoiceMixVolume(VoiceType *pSource, SubMixType *pDestination, float volume, int sourceIndex, int destinationIndex)
 * - @ref GetVoiceMixVolume(const VoiceType *pSource, const SubMixType *pDestination, int sourceIndex, int destinationIndex)
 *
 * (エフェクト)
 * - @ref AddBufferMixer(AudioRendererConfig *pOutConfig, BufferMixerType *pBufferMixer, SubMixType *pSubMix)
 * - @ref RemoveBufferMixer(AudioRendererConfig *pOutConfig, BufferMixerType *pBufferMixer, SubMixType *pSubMix)
 * - @ref AddDelay(AudioRendererConfig *pOutConfig, DelayType *pDelay, void* buffer, size_t bufferSize, SubMixType *pSubMix, nn::TimeSpan delayTimeMax, int channelCountMax)
 * - @ref RemoveDelay(AudioRendererConfig *pOutConfig, DelayType *pDelay, SubMixType *pSubMix)
 * - @ref AddReverb(AudioRendererConfig *pOutConfig, ReverbType *pReverb, void* buffer, size_t bufferSize, SubMixType *pSubMix, int channelCount)
 * - @ref RemoveReverb(AudioRendererConfig *pOutConfig, ReverbType *pReverb, SubMixType *pSubMix)
 * - @ref AddAux(AudioRendererConfig *pOutConfig, AuxType *pAux, void* buffer, size_t bufferSize, SubMixType *pSubMix, void *pSendBuffer, void *pReturnBuffer, size_t bufferSize)
 * - @ref RemoveAux(AudioRendererConfig *pOutConfig, AuxType *pAux, SubMixType *pSubMix)
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct SubMixType
{
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

    MixInfo* _pMixInfo;
};

}  // namespace audio
}  // namespace nn
