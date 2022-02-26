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

namespace nn {
namespace audio {

class SplitterInfo;

/**
 * @brief       入力元となる VoiceType / SubMixType のサンプルデータを複数の出力先に分配する機能を提供する構造体です。
 *
 * @details
 * VoiceType / SubMixType の出力先をこの構造体に設定することで VoiceType / SubMixType が保持するサンプルデータを、
 * 複数の SubMixType / FinalMixType に出力することができます。
 * SplitterType ではサンプルレート変換は行われません。よって SplitterType のすべての出力先は SplitterType に指定されたサンプルレートと同一である必要があります。
 *
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * (確保・解放)
 * - @ref AcquireSplitter()
 * - @ref ReleaseSplitter()
 *
 * (入力元指定)
 * - @ref SetVoiceDestination(AudioRendererConfig* pOutConfig, VoiceType* pSource, SplitterType* pDestination)
 * - @ref SetSubMixDestination(AudioRendererConfig* pOutConfig, SubMixType* pSource, SplitterType* pDestination)
 *
 * (出力先指定)
 * - @ref SetSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pSource, int destinationIndex, SubMixType* pDestination)
 * - @ref SetSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pSource, int destinationIndex, FinalMixType* pDestination)
 *
 * (ミックスボリューム操作)
 * - @ref SetSplitterMixVolume(SplitterType* pSource, int destinationIndex, SubMixType* pDestination, float volume, int sourceChannelIndex, int destinationChannelIndex)
 * - @ref SetSplitterMixVolume(SplitterType* pSource, int destinationIndex, FinalMixType* pDestination, float volume, int sourceChannelIndex, int destinationChannelIndex)
 * - @ref GetSplitterMixVolume(const SplitterType* pSource, int destinationIndex, const SubMixType* pDestination, int sourceChannelIndex, int destinationChannelIndex)
 * - @ref GetSplitterMixVolume(const SplitterType* pSource, int destinationIndex, const FinalMixType* pDestination, int sourceChannelIndex, int destinationChannelIndex)
 *
 * (情報取得)
 * - @ref GetSplitterSourceChannelCount()
 * - @ref GetSplitterDestinationCount()
 * - @ref GetSplitterSampleRate()
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct SplitterType
{
    /**
     * @brief       設定可能なミックスボリュームの最大値 (128.0f) を返します。
     */
    static inline float GetVolumeMax() NN_NOEXCEPT
    {
        return 128.0f;
    }

    /**
     * @brief       設定可能なミックスボリュームの最小値 (-128.0f) を返します。
     */
    static inline float GetVolumeMin() NN_NOEXCEPT
    {
        return -128.0f;
    }

    SplitterInfo* _pSplitterInfo;
};

}  // namespace audio
}  // namespace nn
