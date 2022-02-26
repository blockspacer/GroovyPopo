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
#include <nn/audio/audio_AudioRendererTypes.h>
#include <nn/audio/audio_FinalMixTypes.h>
#include <nn/audio/audio_SubMixTypes.h>

namespace nn {
namespace audio {

/**
 * @name        オーディオレンダラ SplitterType 関連 API
 * @{
 */

/**
 * @brief           SplitterType を初期化し AudioRendererConfig に追加します。
 * @param[out]      pOutConfig                 SplitterType を追加するオーディオレンダラを取得の際に利用した AudioRendererConfig
 * @param[out]      pOutSplitter               未初期化な SplitterType 構造体へのポインタ
 * @param[in]       sampleRate                 pOutSplitter に設定するサンプルレート
 * @param[in]       sourceChannelCount         入力元となる VoiceType / SubMixType から pOutSplitter へ入力するチャンネル数
 * @param[in]       destinationCount           pOutSplitter の出力先となる SubMixType / FinalMixType の総数
 * @return Splitter が利用可能であれば true を、そうでなければ false を返します。
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutSplitter != nullptr
 * - sampleRate > 0
 * - sourceChannelCount >= 1
 * - destinationCount >= 1
 * @post
 * - pOutSplitter が初期化される
 * @details
 * SplitterType は入力元からのサンプルデータを、複数の出力先へと分配する機能を提供します。
 * sampleRate には pOutSplitter が分配するサンプルデータのサンプルレートを指定します。
 * この分配処理ではサンプルレート変換が行われることはありません。
 * このため nn::audio::SetSplitterDestination() で指定する出力先のサンプルレートは sampleRate 引数で指定したサンプルレートと同一である必要があります。
 * @n
 * sourceChannelCount には pOutSplitter への入力元となる VoiceType のチャンネル数、または SubMixType のミックスバッファ数を指定します。
 * @n
 * destinationCount には pOutSplitter に接続する SubMixType / FinalMixType の総数を指定します。
 * SplitterType は destinationCount で指定した数以下の出力先を持つことができ、それぞれの出力先ごとに個別のミックスボリュームを保持します。
 * 各出力先は nn::audio::SetSplitterDestination() の destinationIndex 引数で指定するインデックスにて識別され、
 * この destinationIndex は destinationCount によって 0 <= destinationIndex < destinationCount に規定されます。
 * @n
 * 追加可能な SplitterType の総数はオーディオレンダラの初期化時に nn::audio::AudioRendererParameter.splitterCount で指定した数であり、
 * またすべての SplitterType から出力可能なチャンネルの総数は nn::audio::AudioRendererParameter.splitterSendChannelCount で指定した数となります。
 * この関数で 1 つの SplitterType を追加すると (sourceChannelCount * destinationCount) 分のチャンネルを利用します。
 * @n
 * この関数が false を返す場合  nn::audio::AudioRendererParameter.splitterCount もしくは nn::audio::AudioRendererParameter.splitterSendChannelCount の何れかが不足している可能性があります。
 * @n
 * この関数の利用中 nn::audio は pOutConfig へのアクセスを一部ロックします。@n
 * 同一の pOutConfig を利用する以下の関数はブロックされる可能性がありますので注意してください。@n
 * - AcquireSplitter(AudioRendererConfig* pOutConfig, SplitterType* pOutSplitter, int sampleRate, int sourceChannelCount, int destinationCount)
 * - ReleaseSplitter(AudioRendererConfig* pOutConfig, SplitterType* pOutSplitter)
 * - SetSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex, SubMixType* pDestination)
 * - SetSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex, FinalMixType* pDestination)
 * - ClearSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex)
 *
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pOutConfig および pOutSplitter への操作は行わないでください。@n
 * この関数と RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 */
bool AcquireSplitter(AudioRendererConfig* pOutConfig, SplitterType* pOutSplitter, int sampleRate, int sourceChannelCount, int destinationCount) NN_NOEXCEPT;

/**
 * @brief           SplitterType を AudioRendererConfig から除去します。
 * @param[out]      pOutConfig                 pOutSplitter を追加する際に利用した AudioRendererConfig
 * @param[out]      pOutSplitter               除去する SplitterType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutSplitter != nullptr
 * - pOutSplitter は初期化されている。
 *
 * @details
 * この関数の利用中 nn::audio は pOutConfig へのアクセスを一部ロックします。@n
 * 同一の pOutConfig を利用する以下の関数はブロックされる可能性がありますので注意してください。@n
 * - AcquireSplitter(AudioRendererConfig* pOutConfig, SplitterType* pOutSplitter, int sampleRate, int sourceChannelCount, int destinationCount)
 * - ReleaseSplitter(AudioRendererConfig* pOutConfig, SplitterType* pOutSplitter)
 * - SetSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex, SubMixType* pDestination)
 * - SetSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex, FinalMixType* pDestination)
 * - ClearSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex)
 *
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pOutConfig および pOutSplitter への操作は行わないでください。@n
 * この関数と RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 */
void ReleaseSplitter(AudioRendererConfig* pOutConfig, SplitterType* pOutSplitter) NN_NOEXCEPT;

/**
 * @brief           SplitterType の出力先の 1 つに SubMix を指定します。
 * @param[out]      pOutConfig                 pOutSource を追加する際に利用した AudioRendererConfig
 * @param[out]      pOutSource                 出力先を指定する SplitterType
 * @param[in]       destinationIndex           pOutSource の出力先インデックス
 * @param[in]       pDestination               指定する出力先の SubMixType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - 0 <= destinationIndex < @ref nn::audio::GetSplitterDestinationCount "nn::audio::GetSplitterDestinationCount(pOutSource)"
 * - pOutSource != nullptr
 * - pOutSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - @ref nn::audio::GetSplitterSampleRate "nn::audio::GetSplitterSampleRate(pOutSource)" == @ref nn::audio::GetSubMixSampleRate "nn::audio::GetSubMixSampleRate(pDestination)"
 * @details
 * SplitterType は nn::audio::AcquireSplitter() で destinationCount 引数に指定した個数の出力先を持つことができます。
 * この関数では pOutSource の destinationIndex 番目の出力先に pDestination を指定します。
 * SplitterType ではサンプルレート変換が行われることはありません。
 * このため pDestination に指定する出力先のサンプルレートは @ref nn::audio::GetSplitterSampleRate "nn::audio::GetSplitterSampleRate(pOutSource)" 取得されるサンプルレートと同一である必要があります。
 * @n
 * この関数の利用中 nn::audio は pOutConfig へのアクセスを一部ロックします。@n
 * 同一の pOutConfig を利用する以下の関数はブロックされる可能性がありますので注意してください。@n
 * - AcquireSplitter(AudioRendererConfig* pOutConfig, SplitterType* pOutSplitter, int sampleRate, int sourceChannelCount, int destinationCount)
 * - ReleaseSplitter(AudioRendererConfig* pOutConfig, SplitterType* pOutSplitter)
 * - SetSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex, SubMixType* pDestination)
 * - SetSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex, FinalMixType* pDestination)
 * - ClearSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex)
 *
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pOutConfig および pOutSource への操作は行わないでください。@n
 * この関数と RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 */
void SetSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex, SubMixType* pDestination) NN_NOEXCEPT;

/**
 * @brief           SplitterType の出力先の 1 つに FinalMixMix を指定します。
 * @param[out]      pOutConfig                 pOutSource を追加する際に利用した AudioRendererConfig
 * @param[out]      pOutSource                 出力先を指定する SplitterType
 * @param[in]       destinationIndex           pOutSource の出力先インデックス
 * @param[in]       pDestination               指定する出力先の FinalMixType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutSource は初期化されている
 * - pOutSource != nullptr
 * - 0 <= destinationIndex < @ref nn::audio::GetSplitterDestinationCount "nn::audio::GetSplitterDestinationCount(pOutSource)"
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - @ref nn::audio::GetSplitterSampleRate "nn::audio::GetSplitterSampleRate(pOutSource)" == @ref nn::audio::GetFinalMixSampleRate "nn::audio::GetFinalMixSampleRate(pDestination)"
 * @details
 * この関数の機能は pDestination の型が nn::audio::FinalMixType であることを除き
 * nn::audio::SetSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex, SubMixType* pDestination) と同等です。
 * 詳細はそちらをご確認ください。
 * この関数と RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 */
void SetSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex, FinalMixType* pDestination) NN_NOEXCEPT;

/**
 * @brief           SplitterType の出力先を未指定にします。
 * @param[out]      pOutConfig                 pOutSource を追加する際に利用した AudioRendererConfig
 * @param[out]      pOutSource                 SplitterType 構造体
 * @param[in]       destinationIndex           pOutSource の出力先インデックス
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutSplitter != nullptr
 * - pOutSplitter は初期化されている
 * - 0 <= destinationIndex < @ref nn::audio::GetSplitterDestinationCount "nn::audio::GetSplitterDestinationCount(pOutSource)"
 * @details
 * この関数は pOutConfig に対する操作はスレッドセーフですが、pOutSource に対する操作についてはスレッドセーフではありません。@n
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
void ClearSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex) NN_NOEXCEPT;

/**
 * @brief           SplitterType の出力先へのミックスボリュームを設定します。
 * @param[out]      pOutSource                 ボリュームを設定する SplitterType
 * @param[in]       destinationIndex           pDestination が接続されている出力先インデックス
 * @param[in]       pDestination               出力先として設定されている SubMixType
 * @param[in]       volume                     設定するボリューム
 * @param[in]       sourceChannelIndex         入力元の VoiceType チャンネル数、もしくは SubMixType のミックスバッファのインデックス
 * @param[in]       destinationChannelIndex    出力先の SubMixType のミックスバッファのインデックス
 * @pre
 * - pOutSource は初期化されている
 * - pOutSource != nullptr
 * - 0 <= destinationIndex < @ref nn::audio::GetSplitterDestinationCount "nn::audio::GetSplitterDestinationCount(pOutSource)"
 * - pOutSource の destinationIndex 番目の出力先には pDestination が接続されている。
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - nn::audio::SplitterType::GetVolumeMin() <= volume && volume <= nn::audio::SplitterType::GetVolumeMax
 * - 0 <= sourceChannelIndex && souceChannelIndex <= @ref nn::audio::GetSplitterSourceChannelCount "nn::audio::GetSplitterSourceChannelCount(pOutSouce)"
 * - 0 <= destinationChannelIndex && destinationChannelIndex <= @ref nn::audio::GetSubMixBufferCount "nn::audio::GetSubMixBufferCount(pDestination)"
 * @details
 * すべてのミックスボリュームの初期値は 0.0f です。
 * @n
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pOutSource への操作は行わないでください。@n
 * この関数と RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 * @note
 * 設定するボリューム値によっては音割れが発生する可能性があります。@n
 * 音割れが発生した場合は、ボリュームが適切な値に収まっていることを確認してください。@n
 * 負のボリューム値を設定した場合、波形の位相は反転します。
 */
void SetSplitterMixVolume(SplitterType* pOutSource, int destinationIndex, SubMixType* pDestination, float volume, int sourceChannelIndex, int destinationChannelIndex) NN_NOEXCEPT;

/**
 * @brief           SplitterType の出力先へのミックスボリュームを設定します。
 * @param[out]      pOutSource                 ボリュームを設定する SplitterType
 * @param[in]       destinationIndex           pDestination が接続されている出力先インデックス
 * @param[in]       pDestination               出力先として設定されている FinalMixType
 * @param[in]       volume                     設定するボリューム
 * @param[in]       sourceChannelIndex         入力元の VoiceType チャンネル数、もしくは SubMixType のミックスバッファのインデックス
 * @param[in]       destinationChannelIndex    出力先の FinalMixType のミックスバッファのインデックス
 * @pre
 * - pOutSource は初期化されている
 * - pOutSource != nullptr
 * - 0 <= destinationIndex < @ref nn::audio::GetSplitterDestinationCount "nn::audio::GetSplitterDestinationCount(pOutSource)"
 * - pOutSource の destinationIndex 番目の出力先には pDestination が接続されている。
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - nn::audio::SplitterType::GetVolumeMin() <= volume && volume <= nn::audio::SplitterType::GetVolumeMax()
 * - 0 <= sourceChannelIndex && souceChannelIndex <= @ref nn::audio::GetSplitterSourceChannelCount "nn::audio::GetSplitterSourceChannelCount(pOutSouce)"
 * - 0 <= destinationChannelIndex && destinationChannelIndex <= @ref nn::audio::GetFinalMixBufferCount "nn::audio::GetFinalMixBufferCount(pDestination)"
 * @details
 * この関数の機能は pDestination の型が nn::audio::FinalMixType であることを除き
 * nn::audio::SetSplitterMixVolume(SplitterType* pOutSource, int destinationIndex, SubMixType* pDestination, float volume, int sourceChannelIndex, int destinationChannelIndex) と同等です。
 * 詳細はそちらをご確認ください。
 * この関数と RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 */
void SetSplitterMixVolume(SplitterType* pOutSource, int destinationIndex, FinalMixType* pDestination, float volume, int sourceChannelIndex, int destinationChannelIndex) NN_NOEXCEPT;

/**
 * @brief           SplitterType の出力先へのミックスボリュームを取得します。
 * @param[in]       pSource                    ボリュームを取得する SplitterType
 * @param[in]       destinationIndex           pDestination が接続されている出力先インデックス
 * @param[in]       pDestination               出力先として設定されている SubMixType
 * @param[in]       sourceChannelIndex         入力元の VoiceType チャンネル数、もしくは SubMixType のミックスバッファのインデックス
 * @param[in]       destinationChannelIndex    出力先の SubMixType のミックスバッファのインデックス
 * @return          ミックスボリュームを返します。
 * @pre
 * - pSource は初期化されている
 * - pSource != nullptr
 * - 0 <= destinationIndex < @ref nn::audio::GetSplitterDestinationCount "nn::audio::GetSplitterDestinationCount(pSource)"
 * - pSource の destinationIndex 番目の出力先には pDestination が接続されている。
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - 0 <= sourceChannelIndex && souceChannelIndex <= @ref nn::audio::GetSplitterSourceChannelCount "nn::audio::GetSplitterSourceChannelCount(pSouce)"
 * - 0 <= destinationChannelIndex && destinationChannelIndex <= @ref nn::audio::GetFinalMixBufferCount "nn::audio::GetFinalMixBufferCount(pDestination)"
 * @details
 * @ref nn::audio::SetSplitterMixVolume "nn::audio::SetSplitterMixVolume(SplitterType* pSource, int destinationIndex, SubMixType* pDestination, float volume, int sourceChannelIndex, int destinationChannelIndex)" で設定した
 * ミックスボリュームを取得します。
 * @n
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pSplitter への操作は行わないでください。@n
 */
float GetSplitterMixVolume(const SplitterType* pSource, int destinationIndex, const SubMixType* pDestination, int sourceChannelIndex, int destinationChannelIndex) NN_NOEXCEPT;

/**
 * @brief           SplitterType の出力先へのミックスボリュームを取得します。
 * @param[in]       pSource                    ボリュームを取得する SplitterType
 * @param[in]       destinationIndex           pDestination が接続されている出力先インデックス
 * @param[in]       pDestination               出力先として設定されている FinalMixType
 * @param[in]       sourceChannelIndex         入力元の VoiceType チャンネル数、もしくは SubMixType のミックスバッファのインデックス
 * @param[in]       destinationChannelIndex    出力先の FinalMixType のミックスバッファのインデックス
 * @return          ミックスボリュームを返します。
 * @pre
 * - pSource は初期化されている
 * - pSource != nullptr
 * - 0 <= destinationIndex < @ref nn::audio::GetSplitterDestinationCount "nn::audio::GetSplitterDestinationCount(pSource)"
 * - pSource の destinationIndex 番目の出力先には pDestination が接続されている。
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - 0 <= sourceChannelIndex && souceChannelIndex <= @ref nn::audio::GetSplitterSourceChannelCount "nn::audio::GetSplitterSourceChannelCount(pSouce)"
 * - 0 <= destinationChannelIndex && destinationChannelIndex <= @ref nn::audio::GetFinalMixBufferCount "nn::audio::GetFinalMixBufferCount(pDestination)"
 * @details
 * この関数の機能は pDestination の型が nn::audio::FinalMixType であることを除き
 * @ref nn::audio::GetSplitterMixVolume "nn::audio::GetSplitterMixVolume(const SplitterType* pSource, int destinationIndex, const SubMixType* pDestination, int sourceChannelIndex, int destinationChannelIndex)" と同等です。
 * 詳細はそちらをご確認ください。
 */
float GetSplitterMixVolume(const SplitterType* pSource, int destinationIndex, const FinalMixType* pDestination, int sourceChannelIndex, int destinationChannelIndex) NN_NOEXCEPT;

/**
 * @brief           SplitterType への入力チャンネル数を取得します。
 * @param[in]       pSource                    チャンネル数を取得する SplitterType
 * @return          チャンネル数を返します。
 * @pre
 * - pSource != nullptr
 * - pSource は初期化されている
 * @details
 * pSource に指定されている入力元からの入力チャンネル数を取得します。
 * この値は nn::audio::AcquireSplitter() で sourceChannelCount 引数に指定した値に一致します。
 * nn::audio::SetSplitterMixVolume() および nn::audio::GetSplitterMixVolume() において、
 * sourceChannelIndex 引数に指定する値は、この関数で取得される値よりも小さい必要があります。
 */
int GetSplitterSourceChannelCount(const SplitterType* pSource) NN_NOEXCEPT;

/**
 * @brief           SplitterType からの出力先の数を取得します。
 * @param[in]       pSource                    出力先数を取得する SplitterType
 * @return          出力先の数を返します。
 * @pre
 * - pSource != nullptr
 * - pSource は初期化されている
 * @details
 * pSource からの出力先の数を返します。
 * この値は nn::audio::AcquireSplitter() で destinationCount 引数に指定した値に一致します。
 * nn::audio::SetSplitterDestination() 、 nn::audio::SetSplitterMixVolume() および nn::audio::GetSplitterMixVolume() において、
 * destinationIndex 引数に指定する値は、この関数で取得される値よりも小さい必要があります。
 */
int GetSplitterDestinationCount(const SplitterType* pSource) NN_NOEXCEPT;

/**
 * @brief           SplitterType に指定されたサンプルレートを取得します。
 * @param[in]       pSource                    サンプルレートを取得する SplitterType
 * @return          サンプルレートを返します。
 * @pre
 * - pSource != nullptr
 * - pSource は初期化されている
 * @details
 * pSource に指定されているサンプルレートを取得します。
 * この値は nn::audio::AcquireSplitter() で sampleRate 引数に指定した値に一致します。
 * SplitterType の出力先として指定される SubMixType / FinalMixType は
 * この関数で取得される値と同一のサンプルレートである必要があります。
 */
int GetSplitterSampleRate(const SplitterType* pSource) NN_NOEXCEPT;

/**
 * @}
 */

}  // namespace audio
}  // namespace nn
