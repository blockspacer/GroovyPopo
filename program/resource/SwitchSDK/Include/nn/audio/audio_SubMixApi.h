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
#include <nn/audio/audio_SplitterTypes.h>
#include <nn/audio/audio_SubMixTypes.h>

namespace nn {
namespace audio {

/**
 * @name        オーディオレンダラ SubMix 関連 API
 * @{
 */

/**
 * @brief       SubMixType を初期化し AudioRendererConfig に追加します。
 * @param[out]  pOutConfig              SubMixType を追加するオーディオレンダラの取得時に利用した AudioRendererConfig
 * @param[out]  pOutSubMix              SubMixType 構造体
 * @param[in]   sampleRate              SubMixType のサンプルレート
 * @param[in]   bufferCount             SubMixType のミックスバッファ数
 * @return      SubMixType が利用可能であれば true を、そうでなければ false を返します。
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutSubMix != nullptr
 * - sampleRate == 32000 || sampleRate == 48000
 * - 0 < bufferCount && bufferCount <= nn::audio::MixBufferCountMax
 */
bool AcquireSubMix(AudioRendererConfig* pOutConfig, SubMixType* pOutSubMix, int sampleRate, int bufferCount) NN_NOEXCEPT;

/**
 * @brief       SubMixType を AudioRendererConfig から除きます。
 * @param[out]  pOutConfig              nn::audio::AcquireSubMix() で pSubMix を初期化する際に利用した AudioRendererConfig
 * @param[in]   pSubMix                 SubMixType 構造体
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutSubMix != nullptr
 */
void ReleaseSubMix(AudioRendererConfig* pOutConfig, SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @brief       SubMixType の出力先となる FinalMixType を指定します。
 * @param[out]  pOutConfig              nn::audio::AcquireSubMix() で pSubMix を初期化する際に利用した AudioRendererConfig
 * @param[in]   pSource                 入力元となる SubMixType
 * @param[in]   pDestination            出力先となる FinalMixType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 */
void SetSubMixDestination(AudioRendererConfig* pOutConfig, SubMixType* pSource, FinalMixType* pDestination) NN_NOEXCEPT;

/**
 * @brief       SubMixType の出力先となる SubMixType を指定します。
 * @param[out]  pOutConfig              nn::audio::AcquireSubMix() で pSubMix を初期化する際に利用した AudioRendererConfig
 * @param[in]   pSource                 入力元となる SubMixType
 * @param[in]   pDestination            出力先となる SubMixType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 */
void SetSubMixDestination(AudioRendererConfig* pOutConfig, SubMixType* pSource, SubMixType* pDestination) NN_NOEXCEPT;

/**
 * @brief       SubMixType の出力先となる SplitterType を指定します。
 * @param[out]  pOutConfig              nn::audio::AcquireSubMix() で pSubMix を初期化する際に利用した AudioRendererConfig
 * @param[in]   pSource                 入力元となる SubMixType
 * @param[in]   pDestination            出力先となる SplitterType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * @details
 * SplitterType を出力先に指定することで pSource が保持するサンプルデータを複数の SubMixType / FinalMixType に出力することができます。
 * SplitterType ではサンプルレート変換は行われないため、同一のサンプルレートを持つ SubMixType / FinalMixType にのみ出力可能であることに注意してください。
 */
void SetSubMixDestination(AudioRendererConfig* pOutConfig, SubMixType* pSource, SplitterType* pDestination) NN_NOEXCEPT;

/**
 * @brief       SubMixType の出力先を未指定にします。
 * @param[out]  pOutConfig              nn::audio::AcquireSubMix() で pSubMix を初期化する際に利用した AudioRendererConfig
 * @param[in]   pSource              SubMixType 構造体
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pSource != nullptr
 * - pSource は初期化されている
 * @details
 * この関数は pOutConfig に対する操作はスレッドセーフですが、pSource に対する操作についてはスレッドセーフではありません。@n
 * この関数の呼び出し中に pSource への操作は行わないでください。@n
 * @n
 * この関数の利用中 nn::audio は pOutConfig へのアクセスを一部ロックします。@n
 * 同一の pOutConfig を利用する以下の関数はブロックされる可能性がありますので注意してください。@n
 * - AcquireFinalMix(AudioRendererConfig* pOutConfig, FinalMixType* pOutFinalMix, int bufferCount)
 * - ReleaseFinalMix(AudioRendererConfig* pOutConfig, FinalMixType* pFinalMix)
 * - AcquireSubMix(AudioRendererConfig* pOutConfig, SubMixType* pOutSubMix, int sampleRate, int bufferCount)
 * - ReleaseSubMix(AudioRendererConfig* pOutConfig, SubMixType* pSubMix)
 * - SetSubMixDestination(AudioRendererConfig* pOutConfig, SubMixType* pSource, FinalMixType* pDestination)
 * - SetSubMixDestination(AudioRendererConfig* pOutConfig, SubMixType* pSource, SubMixType* pDestination)
 * - ClearSubMixDestination(AudioRendererConfig* pOutConfig, SubMixType* pSource)
 *
 * この関数と RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。詳細は RequestUpdateAudioRenderer() の説明をご確認ください。
 */
void ClearSubMixDestination(AudioRendererConfig* pOutConfig, SubMixType* pSource) NN_NOEXCEPT;

/**
 * @brief       SubMixType のサンプルレートを取得します。
 * @param[in]   pSubMix                 SubMixType 構造体
 * @return       サンプルレートを返します。
 * @pre
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 */
int GetSubMixSampleRate(const SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @brief       SubMixType のミックスバッファ数を取得します。
 * @param[in]   pSubMix                 SubMixType 構造体
 * @return      ミックスバッファ数を返します。
 * @pre
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 */
int GetSubMixBufferCount(const SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @brief       SubMixType のボリュームを取得します。
 * @param[in]   pSubMix                 SubMixType 構造体
 * @return      ボリュームを返します。
 * @pre
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 */
float GetSubMixVolume(const SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @brief       SubMixType のボリュームを設定します。
 * @param[in]   pSubMix                 SubMixType 構造体
 * @param[in]   volume                  ボリューム
 * @pre
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * - nn::audio::SubMixType::GetVolumeMin() <= volume && volume <= nn::audio::SubMixType::GetVolumeMax()
 */
void SetSubMixVolume(SubMixType* pSubMix, float volume) NN_NOEXCEPT;

/**
 * @brief       SubMixType のミックスボリュームを設定します。
 * @param[in]   pSource                 入力元となる SubMixType
 * @param[in]   pDestination            出力先となる FinalMixType
 * @param[in]   volume                  ミックスボリューム
 * @param[in]   sourceIndex             入力側のミックスバッファのインデックス
 * @param[in]   destinationIndex        出力側のミックスバッファのインデックス
 * @pre
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - pSource, pDestination に対して @ref nn::audio::SetSubMixDestination "nn::audio::SetSubMixDestination(pSource, pDestination)" を実行済である
 * - nn::audio::SubMixType::GetVolumeMin() <= volume && volume <= nn::audio::SubMixType::GetVolumeMax()
 * - 0 <= sourceIndex && sourceIndex < @ref nn::audio::GetSubMixBufferCount "nn::audio::GetSubMixBufferCount(pSource)"
 * - 0 <= destinationIndex && destinationIndex < @ref nn::audio::GetFinalMixBufferCount "nn::audio::GetFinalMixBufferCount(pDestination)"
 * @post
 * - @ref nn::audio::GetSubMixMixVolume "nn::audio::GetSubMixMixVolume(pSource, pDestination, sourceIndex, destinationIndex)" == volume
 */
void SetSubMixMixVolume(SubMixType* pSource, FinalMixType* pDestination, float volume, int sourceIndex, int destinationIndex) NN_NOEXCEPT;

/**
 * @brief       SubMixType のミックスボリュームを取得します。
 * @param[in]   pSource                 入力元となる SubMixType
 * @param[in]   pDestination            出力先となる FinalMixType
 * @param[in]   sourceIndex             入力側のミックスバッファのインデックス
 * @param[in]   destinationIndex        出力側のミックスバッファのインデックス
 * @return      (sourceIndex, destinationIndex) に対するミックスボリュームを返します。
 * @pre
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - pSource, pDestination に対して @ref nn::audio::SetSubMixDestination "nn::audio::SetSubMixDestination(pSource, pDestination)" を実行済である
 * - 0 <= sourceIndex && sourceIndex < @ref nn::audio::GetSubMixBufferCount "nn::audio::GetSubMixBufferCount(pSource)"
 * - 0 <= destinationIndex && destinationIndex < @ref nn::audio::GetFinalMixBufferCount "nn::audio::GetFinalMixBufferCount(pDestination)"
 */
float GetSubMixMixVolume(const SubMixType* pSource, const FinalMixType* pDestination, int sourceIndex, int destinationIndex) NN_NOEXCEPT;

/**
 * @brief       SubMixType のミックスボリュームを設定します。
 * @param[in]   pSource                 入力元となる SubMixType
 * @param[in]   pDestination            出力先となる SubMixType
 * @param[in]   volume                  ミックスボリューム
 * @param[in]   sourceIndex             入力側のミックスバッファのインデックス
 * @param[in]   destinationIndex        出力側のミックスバッファのインデックス
 * @pre
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - pSource, pDestination に対して @ref nn::audio::SetSubMixDestination "nn::audio::SetSubMixDestination(pSource, pDestination)" を実行済である
 * - nn::audio::SubMixType::GetVolumeMin() <= volume && volume <= @ref nn::audio::SubMixType::GetVolumeMax "nn::audio::SubMixType::GetVolumeMax()"
 * - 0 <= sourceIndex && sourceIndex < @ref nn::audio::GetSubMixBufferCount "nn::audio::GetSubMixBufferCount(pSource)"
 * - 0 <= destinationIndex && destinationIndex < @ref nn::audio::GetSubMixBufferCount "nn::audio::GetSubMixBufferCount(pDestination)"
 * @post
 * - @ref nn::audio::GetSubMixMixVolume "nn::audio::GetSubMixMixVolume(pSource, pDestination, sourceIndex, destinationIndex)" == volume
 */
void SetSubMixMixVolume(SubMixType* pSource, SubMixType* pDestination, float volume, int sourceIndex, int destinationIndex) NN_NOEXCEPT;

/**
 * @brief       SubMixType のミックスボリュームを取得します。
 * @param[in]   pSource                 入力元となる SubMixType
 * @param[in]   pDestination            出力先となる SubMixType
 * @param[in]   sourceIndex             入力側のミックスバッファのインデックス
 * @param[in]   destinationIndex        出力側のミックスバッファのインデックス
 * @return      (sourceIndex, destinationIndex) に対するミックスボリュームを返します。
 * @pre
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - pSource, pDestination に対して @ref nn::audio::SetSubMixDestination "nn::audio::SetSubMixDestination(pSource, pDestination)" を実行済である
 * - 0 <= sourceIndex && sourceIndex < @ref nn::audio::GetSubMixBufferCount "nn::audio::GetSubMixBufferCount(pSource)"
 * - 0 <= destinationIndex && destinationIndex < @ref nn::audio::GetSubMixBufferCount "nn::audio::GetSubMixBufferCount(pDestination)"
 */
float GetSubMixMixVolume(const SubMixType* pSource, const SubMixType* pDestination, int sourceIndex, int destinationIndex) NN_NOEXCEPT;

/**
 * @brief       SubMixType のオーディオレンダラ上での ID を取得します。
 * @param[in]   pSubMix                 SubMixType
 * @return      NodeId を返します。
 * @pre
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * @details
 * NodeId はオーディオレンダラに登録されている VoiceType / SubMixType / FinalMixType / DeviceSinkType / CircularBufferSinkType を一意に特定する数値です。
 * 各 NodeId は単独のオーディオレンダラ内で重複することはありません。
 * NodeId は VoiceType / SubMixType / FinalMixType / DeviceSinkType / CircularBufferSinkType が追加されるたびに、それぞれのインスタンスに割り振られます。
 */
NodeId GetSubMixNodeId(const SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @}
 */
}  // namespace audio
}  // namespace nn
