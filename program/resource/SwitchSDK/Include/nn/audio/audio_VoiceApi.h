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
#include <nn/audio/audio_Result.h>
#include <nn/audio/audio_SampleFormat.h>
#include <nn/audio/audio_AudioRendererTypes.h>
#include <nn/audio/audio_WaveBuffer.h>
#include <nn/audio/audio_FinalMixTypes.h>
#include <nn/audio/audio_SplitterTypes.h>
#include <nn/audio/audio_SubMixTypes.h>

namespace nn {
namespace audio {

/**
 * @name        オーディオレンダラ Voice 関連 API
 * @{
 */

/**
 * @brief       挙動指定のオプション付きで VoiceType を初期化し AudioRendererConfig に追加します。
 * @param[out]  pOutConfig              VoiceType を追加する対象のオーディオレンダラを取得際に利用した AudioRendererConfig
 * @param[out]  pOutVoice               未初期化な VoiceType 構造体へのポインタ
 * @param[in]   sampleRate              サンプルレート
 * @param[in]   channelCount            チャンネル数
 * @param[in]   sampleFormat            サンプルフォーマット
 * @param[in]   priority                優先度
 * @param[in]   pParameter              追加パラメータ
 * @param[in]   size                    追加パラメータのサイズ
 * @param[in]   pBehaviorOptions        Voice の挙動を指定するオプション
 * @return      VoiceType が利用可能であれば true を、そうでなければ false を返します。
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutVoice != nullptr
 * - sampleRate > 0
 * - channelCount >= 1 && channelCount <= nn::audio::VoiceType::ChannelCountMax
 * - sampleFormat == nn::audio::SampleFormat_PcmInt16 || sampleFormat == nn::audio::SampleFormat_Adpcm
 * - nn::audio::VoiceType::PriorityHighest <= priority && priority <= nn::audio::VoiceType::PriorityLowest
 * - pParameter != nullptr の時、pParameter は nn::audio::BufferAlignSize にアラインしている
 * - pParameter != nullptr の時、size > 0
 * - pBehaviorOptions != nullptr
 * - pBehaviorOptions->isPitchAndSrcSkipped == true の時、sampleRate == @ref nn::audio::GetAudioRendererSampleRate "nn::audio::GetAudioRendererSampleRate(handle)"
 * @post
 * - pOutVoice が初期化される
 * @details
 * この関数の呼び出し以後 pOutVoice への操作は、 VoiceType の説明に列挙された各種関数で行います。@n
 * nn::audio::SetVoiceDestination() でボイスの出力先を設定の上、ミックスボリュームを nn::audio::SetVoiceMixVolume() で設定してください。@n
 * @n
 * VoiceType 初期化時に以下のパラメータを指定する必要があります。@n
 * これらはこの関数の呼び出し後に変更することができません。@n
 * - チャンネル数
 * - サンプルレート
 * - サンプルフォーマット
 * - 追加パラメータとそのサイズ
 *
 * チャンネル数、サンプルレートおよびサンプルフォーマットは nn::audio::AppendWaveBuffer() で pOutVoice に与えられる入力波形が必要とするサンプルレートを指定してください。@n
 * ボイスの接続先に設定されたサンプルレートとボイスに設定されたサンプルレートが異なる場合は、レンダリング処理中に自動的にサンプルレート変換処理が行われます。
 * @n
 * この関数はチャンネル数に応じたボイスを消費します。
 * 例えば、6 チャンネルのボイスを確保した場合、6 ボイス分のボイスを消費します。
 * @n
 * 優先度は VoiceType::PriorityHighest から VoiceType::PriorityLowest の間の値を指定します。@n
 * 小さな値ほど高優先度です。@n
 * @n
 * 追加パラメータは sampleFormat に SampleFormat_Adpcm を指定する際に必要となります。@n
 * その他のフォーマットを指定する場合は nullptr および 0 を指定してください。@n
 * @n
 * pParameter に指定するメモリ領域は nn::audio::MemoryPoolType の保持する領域に含まれている必要があります。@n
 * 詳しくは nn::audio::MemoryPoolType および nn::audio::AcquireMemoryPool() の説明を参照してください。@n
 * @n
 * pBehaviorOptions は初期化される VoiceType の挙動を指定するオプションです。
 * VoiceType::BehaviorOptions::isPlayedSampleCountResetAtLoopPoint を true に指定すると、 nn::audio::GetVoicePlayedSampleCount() で取得されるサンプル数がループ時に 0 にクリアされるようになります。@n
 * VoiceType::BehaviorOptions::isPitchAndSrcSkipped を true に指定すると、ピッチ変換とサンプルレート変換 (SRC) をスキップします。この時 nn::audio::SetVoicePitch() によるピッチ設定は禁止されていることに注意してください。@n
 * サンプルレート変換は行われないため、引数に指定したサンプルレートとボイスの接続先に設定されたサンプルレートは同一である必要があります。
 * @n
 * この関数の利用中 nn::audio は pOutConfig へのアクセスを一部ロックします。@n
 * 同一の pOutConfig を利用する以下の関数はブロックされる可能性がありますので注意してください。@n
 * - AcquireVoiceSlot(AudioRendererConfig* pOutConfig, VoiceType* pOutVoice, int sampleRate, int channelCount, SampleFormat sampleFormat, int priority, const void* pParameter, size_t size)
 * - ReleaseVoiceSlot(AudioRendererConfig* pOutConfig, VoiceType* pVoice)
 * - SetVoiceDestination(AudioRendererConfig* pOutConfig, VoiceType* pSource, FinalMixType* pDestination)
 * - SetVoiceDestination(AudioRendererConfig* pOutConfig, VoiceType* pSource, SubMixType* pDestination)
 * - SetVoiceDestination(AudioRendererConfig* pOutConfig, VoiceType* pSource, SplitterType* pDestination)
 * - ClearVoiceDestination(AudioRendererConfig* pOutConfig, VoiceType* pSource)
 *
 * この関数と nn::audio::RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 */
bool AcquireVoiceSlot(AudioRendererConfig* pOutConfig, VoiceType* pOutVoice, int sampleRate, int channelCount, SampleFormat sampleFormat, int priority, const void* pParameter, size_t size, const VoiceType::BehaviorOptions* pBehaviorOptions ) NN_NOEXCEPT;

/**
 * @brief       VoiceType を初期化し AudioRendererConfig に追加します。
 * @param[out]  pOutConfig              VoiceType を追加する対象のオーディオレンダラを取得際に利用した AudioRendererConfig
 * @param[out]  pOutVoice               未初期化な VoiceType 構造体へのポインタ
 * @param[in]   sampleRate              サンプルレート
 * @param[in]   channelCount            チャンネル数
 * @param[in]   sampleFormat            サンプルフォーマット
 * @param[in]   priority                優先度
 * @param[in]   pParameter              追加パラメータ
 * @param[in]   size                    追加パラメータのサイズ
 * @return      VoiceType が利用可能であれば true を、そうでなければ false を返します。
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutVoice != nullptr
 * - sampleRate > 0
 * - channelCount >= 1 && channelCount <= nn::audio::VoiceType::ChannelCountMax
 * - sampleFormat == nn::audio::SampleFormat_PcmInt16 || sampleFormat == nn::audio::SampleFormat_Adpcm
 * - nn::audio::VoiceType::PriorityHighest <= priority && priority <= nn::audio::VoiceType::PriorityLowest
 * - pParameter != nullptr の時、pParameter は nn::audio::BufferAlignSize にアラインしている
 * - pParameter != nullptr の時、size > 0
 * @post
 * - pOutVoice が初期化される
 * @details
 * この関数はデフォルト挙動で動作する VoiceType を初期化します。
 * それ以外は nn::audio::AcquireVoiceSlot(AudioRendererConfig* pOutConfig, VoiceType* pOutVoice, int sampleRate, int channelCount, SampleFormat sampleFormat, int priority, const void* pParameter, size_t size, const VoiceType::BehaviorOptions* pBehaviorOptions) と同様に振る舞います。@n
 * ここでデフォルト挙動とは、上記関数の pBehaviorOptions の各オプションに対して、すべて false を指定する挙動に相当します。@n
 * @n
 * この関数と nn::audio::RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 */
bool AcquireVoiceSlot(AudioRendererConfig* pOutConfig, VoiceType* pOutVoice, int sampleRate, int channelCount, SampleFormat sampleFormat, int priority, const void* pParameter, size_t size) NN_NOEXCEPT;

/**
 * @brief       VoiceType を AudioRendererConfig から除去します。
 * @param[out]  pOutConfig              VoiceType を除去する対象のオーディオレンダラを取得する際に利用した AudioRendererConfig
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
void ReleaseVoiceSlot(AudioRendererConfig* pOutConfig, VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType が有効かどうかを返します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      有効なら true を、そうでないなら false を返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
bool IsVoiceValid(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType のボイスドロップフラグを返します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      ボイスドロップフラグがオンなら true を、そうでないなら false を返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
bool IsVoiceDroppedFlagOn(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType のボイスドロップフラグをオフにします。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 * @post
 * - IsVoiceDroppedFlagOn(pVoice) == false
 */
void ResetVoiceDroppedFlag(VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType の出力先となる FinalMix を指定します。
 * @param[out]  pOutConfig              nn::audio::AcquireVoiceSlot() で VoiceType を初期化する際に利用した AudioRendererConfig
 * @param[in]   pSource                 入力元となる VoiceType
 * @param[in]   pDestination            出力先となる FinalMixType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - @ref nn::audio::GetVoiceBehaviorOptions "nn::audio::GetVoiceBehaviorOptions(pVoice)".isPitchAndSrcSkipped == true の時、 @ref nn::audio::GetVoiceSampleRate "GetVoiceSampleRate(pSource)"  == @ref nn::audio::GetFinalMixSampleRate "GetFinalMixSampleRate(pDestination)"
 */
void SetVoiceDestination(AudioRendererConfig* pOutConfig, VoiceType* pSource, FinalMixType* pDestination) NN_NOEXCEPT;

/**
 * @brief       VoiceType の出力先となる SubMix を指定します。
 * @param[out]  pOutConfig              nn::audio::AcquireVoiceSlot() で VoiceType を初期化する際に利用した AudioRendererConfig
 * @param[in]   pSource                 入力元となる VoiceType
 * @param[in]   pDestination            出力先となる SubMixType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - @ref nn::audio::GetVoiceBehaviorOptions "nn::audio::GetVoiceBehaviorOptions(pVoice)".isPitchAndSrcSkipped == true の時、 @ref nn::audio::GetVoiceSampleRate "GetVoiceSampleRate(pSource)"  == @ref nn::audio::GetSubMixSampleRate "GetSubMixSampleRate(pDestination)"
 */
void SetVoiceDestination(AudioRendererConfig* pOutConfig, VoiceType* pSource, SubMixType* pDestination) NN_NOEXCEPT;

/**
 * @brief       VoiceType の出力先となる SplitterType を指定します。
 * @param[out]  pOutConfig              nn::audio::AcquireVoiceSlot() で VoiceType を初期化する際に利用した AudioRendererConfig
 * @param[in]   pSource                 入力元となる VoiceType
 * @param[in]   pDestination            出力先となる SplitterType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - AcquireVoiceSlot() で VoiceType::BehaviorOptions::isPitchAndSrcSkipped に true を設定した場合、 @ref nn::audio::GetVoiceSampleRate "GetVoiceSampleRate(pSource)"  == @ref nn::audio::GetSplitterSampleRate "GetSplitterSampleRate(pDestination)"
 */
void SetVoiceDestination(AudioRendererConfig* pOutConfig, VoiceType* pSource, SplitterType* pDestination) NN_NOEXCEPT;

/**
 * @brief       VoiceType の出力先を未指定にします。
 * @param[out]  pOutConfig              nn::audio::AcquireVoiceSlot() で VoiceType を初期化する際に利用した AudioRendererConfig
 * @param[in]   pSource                 VoiceType 構造体へのポインタ
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutVoice != nullptr
 * - pOutVoice は初期化されている
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
void ClearVoiceDestination(AudioRendererConfig* pOutConfig, VoiceType* pSource) NN_NOEXCEPT;

/**
 * @brief       VoiceType のサンプルレートを取得します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      サンプルレートを返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
int GetVoiceSampleRate(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType のチャンネル数を取得します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      チャンネル数を返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
int GetVoiceChannelCount(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType のサンプルフォーマットを取得します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      サンプルフォーマットを返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
SampleFormat GetVoiceSampleFormat(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType の優先度を設定します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @param[in]   priority                優先度
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 * - (nn::audio::VoiceType::PriorityHighest <= priority && priority <= nn::audio::VoiceType::PriorityLowest)
 * @post
 * - @ref GetVoicePriority "GetVoicePriority(pVoice)" == priority
 */
void SetVoicePriority(VoiceType* pVoice, int priority) NN_NOEXCEPT;

/**
 * @brief       VoiceType の優先度を取得します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      優先度を返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
int GetVoicePriority(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType の再生状態を設定します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @param[in]   playState               再生状態
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 * @post
 * - @ref GetVoicePlayState "GetVoicePlayState(pVoice)" == state
 */
void SetVoicePlayState(VoiceType* pVoice, VoiceType::PlayState playState) NN_NOEXCEPT;

/**
 * @brief       VoiceType の再生状態を取得します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      再生状態を返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
VoiceType::PlayState GetVoicePlayState(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType のピッチを設定します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @param[in]   pitch                   ピッチ
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 * - nn::audio::VoiceType::GetPitchMin() <= pitch && pitch <= nn::audio::VoiceType::GetPitchMax()
 * - @ref nn::audio::GetVoiceBehaviorOptions "nn::audio::GetVoiceBehaviorOptions(pVoice)".isPitchAndSrcSkipped == false
 * @post
 * - @ref GetVoicePitch "GetVoicePitch(pVoice)" == pitch
 */
void SetVoicePitch(VoiceType* pVoice, float pitch) NN_NOEXCEPT;

/**
 * @brief       VoiceType のピッチを取得します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      ピッチを返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
float GetVoicePitch(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType のボリュームを設定します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @param[in]   volume                  ボリューム
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 * - nn::audio::VoiceType::GetVolumeMin() <= volume && volume <= nn::audio::VoiceType::GetVolumeMax()
 * @post
 * - @ref GetVoiceVolume "GetVoiceVolume(pVoice)" == volume
 */
void SetVoiceVolume(VoiceType* pVoice, float volume) NN_NOEXCEPT;

/**
 * @brief       VoiceType のボリュームを取得します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      ボリュームを返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
float GetVoiceVolume(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType のミックスボリュームを設定します。
 * @param[in]   pSource                 入力元となる VoiceType
 * @param[in]   pDestination            出力先となる FinalMix
 * @param[in]   volume                  ミックスボリューム
 * @param[in]   sourceIndex             入力側のチャンネル番号
 * @param[in]   destinationIndex        出力側のミックスバッファのインデックス
 * @pre
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - nn::audio::VoiceType::GetVolumeMin() <= volume && volume <= nn::audio::VoiceType::GetVolumeMax()
 * - pSource, pDestination に対して @ref nn::audio::SetVoiceDestination "nn::audio::SetVoiceDestination(pSource, pDestination)" を実行済である
 * - 0 <= sourceIndex && sourceIndex < @ref nn::audio::GetVoiceChannelCount "nn::audio::GetVoiceChannelCount(pSource)"
 * - 0 <= destinationIndex && destinationIndex < @ref nn::audio::GetFinalMixBufferCount "nn::audio::GetFinalMixBufferCount(pDestination)"
 * @post
 * - @ref nn::audio::GetVoiceMixVolume "nn::audio::GetVoiceMixVolume(pSource, pDestination, sourceIndex, destinationIndex)" == volume
 */
void SetVoiceMixVolume(VoiceType* pSource, FinalMixType* pDestination, float volume, int sourceIndex, int destinationIndex) NN_NOEXCEPT;

/**
 * @brief       VoiceType のミックスボリュームを取得します。
 * @param[in]   pSource                 入力元となる VoiceType
 * @param[in]   pDestination            出力先となる FinalMix
 * @param[in]   sourceIndex             入力側のチャンネル番号
 * @param[in]   destinationIndex        出力側のミックスバッファのインデックス
 * @return      (sourceIndex, destinationIndex) に対するミックスボリュームを返します。
 * @pre
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - pSource, pDestination に対して @ref nn::audio::SetVoiceDestination "nn::audio::SetVoiceDestination(pSource, pDestination)" を実行済である
 * - 0 <= sourceIndex && sourceIndex < @ref nn::audio::GetVoiceChannelCount "nn::audio::GetVoiceChannelCount(pSource)"
 * - 0 <= destinationIndex && destinationIndex < @ref nn::audio::GetFinalMixBufferCount "nn::audio::GetFinalMixBufferCount(pDestination)"
 */
float GetVoiceMixVolume(const VoiceType* pSource, const FinalMixType* pDestination, int sourceIndex, int destinationIndex) NN_NOEXCEPT;

/**
 * @brief       VoiceType のミックスボリュームを設定します。
 * @param[in]   pSource                 入力元となる VoiceType
 * @param[in]   pDestination            出力先となる SubMix
 * @param[in]   volume                  ミックスボリューム
 * @param[in]   sourceIndex             入力側のチャンネル番号
 * @param[in]   destinationIndex        出力側のミックスバッファのインデックス
 * @pre
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - pSource, pDestination に対して @ref nn::audio::SetSubMixDestination "nn::audio::SetSubMixDestination(pSource, pDestination)" を実行済である
 * - nn::audio::VoiceType::GetVolumeMin() <= volume && volume <= nn::audio::VoiceType::GetVolumeMax()
 * - 0 <= sourceIndex && sourceIndex < @ref nn::audio::GetVoiceChannelCount "nn::audio::GetVoiceChannelCount(pSource)"
 * - 0 <= destinationIndex && destinationIndex < @ref nn::audio::GetSubMixBufferCount "nn::audio::GetSubMixBufferCount(pDestination)"
 * @post
 * - @ref nn::audio::GetVoiceMixVolume "nn::audio::GetVoiceMixVolume(pSource, pDestination, sourceIndex, destinationIndex)" == volume
 */
void SetVoiceMixVolume(VoiceType* pSource, SubMixType* pDestination, float volume, int sourceIndex, int destinationIndex) NN_NOEXCEPT;

/**
 * @brief       VoiceType のミックスボリュームを取得します。
 * @param[in]   pSource                 入力元となる VoiceType
 * @param[in]   pDestination            出力先となる SubMix
 * @param[in]   sourceIndex             入力側のチャンネル番号
 * @param[in]   destinationIndex        出力側のミックスバッファのインデックス
 * @return      (sourceIndex, destinationIndex) に対するミックスボリュームを返します。
 * @pre
 * - pSource != nullptr
 * - pSource は初期化されている
 * - pDestination != nullptr
 * - pDestination は初期化されている
 * - pSource, pDestination に対して @ref nn::audio::SetSubMixDestination "nn::audio::SetSubMixDestination(pSource, pDestination)" を実行済である
 * - 0 <= sourceIndex && sourceIndex < @ref nn::audio::GetVoiceChannelCount "nn::audio::GetVoiceChannelCount(pSource)"
 * - 0 <= destinationIndex && destinationIndex < @ref nn::audio::GetSubMixBufferCount "nn::audio::GetSubMixBufferCount(pDestination)"
 */
float GetVoiceMixVolume(const VoiceType* pSource, const SubMixType* pDestination, int sourceIndex, int destinationIndex) NN_NOEXCEPT;

/**
 * @brief       この関数は非推奨です。VoiceType の biquad フィルタのパラメータを設定します。
 * @deprecated  biquad フィルタの段数が変更になり nn::audio::SetVoiceBiquadFilterParameter(VoiceType* pVoice, int filterIndex, const BiquadFilterParameter& parameter) が準備されました。そちらの利用に切り替えてください。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @param[in]   parameter               パラメータ
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 * @details
 * 本関数は互換性の目的で残されている関数です。 filterIndex = 0 の biquad フィルタのパラメータのみ設定できます。
 * 利用は控え @ref nn::audio::SetVoiceBiquadFilterParameter "nn::audio::SetVoiceBiquadFilterParameter(VoiceType* pVoice, int filterIndex, const BiquadFilterParameter& parameter)" に切り替えてください。
 */
NN_DEPRECATED void SetVoiceBiquadFilterParameter(VoiceType* pVoice, const BiquadFilterParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       VoiceType の biquad フィルタのパラメータを設定します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @param[in]   filterIndex             パラメータを変更する biquad フィルタのインデックス
 * @param[in]   parameter               パラメータ
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 * - (filterIndex == 0) || (filterIndex == 1)
 * @details
 * 各 VoiceType には 2 段の biquad フィルタが組み込みで準備されており、 filterIndex でどちらを設定するか指定することができます。
 * フィルタ処理は filterIndex が 0, 1 の順番に施されます。
 */
void SetVoiceBiquadFilterParameter(VoiceType* pVoice, int filterIndex, const BiquadFilterParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       この関数は非推奨です。VoiceType の biquad フィルタのパラメータを取得します。
 * @deprecated  biquad フィルタの段数が変更になり @ref nn::audio::GetVoiceBiquadFilterParameter "nn::audio::GetVoiceBiquadFilterParameter(const VoiceType* pVoice, int filterIndex)" が準備されました。そちらの利用に切り替えてください。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      パラメータを返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 * @details
 * 本関数は互換性の目的で残されている関数です。 filterIndex = 0 の biquad フィルタのパラメータのみ取得できます。
 * 利用は控え @ref nn::audio::GetVoiceBiquadFilterParameter "nn::audio::GetVoiceBiquadFilterParameter(const VoiceType* pVoice, int filterIndex)" に切り替えてください。
 */
NN_DEPRECATED const BiquadFilterParameter& GetVoiceBiquadFilterParameter(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType の biquad フィルタのパラメータを取得します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @param[in]   filterIndex             パラメータを取得する biquad フィルタのインデックス
 * @return      パラメータを返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 * - (filterIndex == 0) || (filterIndex == 1)
 */
BiquadFilterParameter GetVoiceBiquadFilterParameter(const VoiceType* pVoice, int filterIndex) NN_NOEXCEPT;

/**
 * @brief       VoiceType に WaveBuffer を追加します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @param[in]   pWaveBuffer             WaveBuffer 構造体へのポインタ
 * @return      追加に成功したら true を、そうでなければ false を返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 * - pWaveBuffer != nullptr
 * - pWaveBuffer は初期化されている
 * - pWaveBuffer->buffer は nn::audio::BufferAlignSize にアラインしている
 * - pWaveBuffer->startSampleOffset >= 0
 * - pWaveBuffer->endSampleOffset >= 0
 * - pWaveBuffer->endSampleOffset >= pWaveBuffer->startSampleOffset
 * - pWaveBuffer->startSampleOffset と pWaveBuffer->endSampleOffset が指すサンプル位置は、pWaveBuffer->buffer と pWaveBuffer->size で指定したメモリ領域の範囲内
 * - @ref nn::audio::GetVoiceSampleFormat "nn::audio::GetVoiceSampleFormat(pVoice)" == nn::audio::SampleFormat_Adpcm の時、pWaveBuffer->buffer は ADPCM フレームの先頭を指している
 * - @ref nn::audio::GetVoiceSampleFormat "nn::audio::GetVoiceSampleFormat(pVoice)" == nn::audio::SampleFormat_Adpcm の時、pWaveBuffer->pContext は nullptr、または nn::audio::BufferAlignSize にアラインしている
 * - @ref nn::audio::GetVoiceSampleFormat "nn::audio::GetVoiceSampleFormat(pVoice)" == nn::audio::SampleFormat_Adpcm かつ pWaveBuffer->pContext != nullptr の時、pWaveBuffer->contextSize > 0
 */
bool AppendWaveBuffer(VoiceType* pVoice, const WaveBuffer* pWaveBuffer) NN_NOEXCEPT;

/**
 * @brief       VoiceType による再生が完了した WaveBuffer を取得します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      再生が完了した WaveBuffer のポインタを返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
const WaveBuffer* GetReleasedWaveBuffer(VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType に登録されている再生可能な WaveBuffer の個数を取得します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      再生可能な WaveBuffer の個数を返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
int GetWaveBufferCount(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType により再生したサンプル数を取得します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      再生したサンプル数を取得します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
int64_t GetVoicePlayedSampleCount(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType のオーディオレンダラ上での ID を取得します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      NodeId を返します。
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
NodeId GetVoiceNodeId(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType に登録されている WaveBuffer を全て再生済みにします。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      なし
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
void FlushWaveBuffers(VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @brief       VoiceType に設定された挙動を指定するオプション群を返します。
 * @param[in]   pVoice                  VoiceType 構造体へのポインタ
 * @return      挙動を指定するオプション群
 * @pre
 * - pVoice != nullptr
 * - pVoice は初期化されている
 */
VoiceType::BehaviorOptions GetVoiceBehaviorOptions(const VoiceType* pVoice) NN_NOEXCEPT;

/**
 * @}
 */
}  // namespace audio
}  // namespace nn
