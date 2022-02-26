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

#include <nn/nn_Result.h>
#include <nn/nn_TimeSpan.h>
#include <nn/audio/audio_AudioRendererTypes.h>
#include <nn/audio/audio_FinalMixTypes.h>
#include <nn/audio/audio_SubMixTypes.h>
#include <nn/audio/audio_EffectTypes.h>

namespace nn {
namespace audio {

/**
 * @name        オーディオレンダラ エフェクト BufferMixer 関連 API
 * @{
 */

/**
 * @brief       BufferMixer を FinalMixType に追加します。
 * @param[out]  pOutConfig              pFinalMix を nn::audio::AcquireFinalMix() で初期化する際に使用した AudioRendererConfig
 * @param[out]  pBufferMixer            未初期化の BufferMixer
 * @param[in]   pFinalMix               pBufferMixer を追加する FinalMixType
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pBufferMixer != nullptr
 * - pBufferMixer は未初期化である
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * @post
 * - pBufferMixer は初期化されている
 * - pBufferMixer は有効化されている
 * - pBufferMixer への入出力バッファは未設定である
 */
Result AddBufferMixer(AudioRendererConfig* pOutConfig, BufferMixerType* pBufferMixer, FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief       BufferMixer を SubMixType に追加します。
 * @param[out]  pOutConfig              pSubMix を nn::audio::AcquireSubMix() で初期化する際に使用した AudioRendererConfig
 * @param[out]  pBufferMixer            未初期化の BufferMixer
 * @param[in]   pSubMix                 エフェクトを追加する SubMixType
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pBufferMixer != nullptr
 * - pBufferMixer は未初期化である
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * @post
 * - pBufferMixer は初期化されている
 * - pBufferMixer は有効化されている
 * - pBufferMixer への入出力バッファは未設定である
 */
Result AddBufferMixer(AudioRendererConfig* pOutConfig, BufferMixerType* pBufferMixer, SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @brief       BufferMixer を FinalMixType から削除します。
 * @param[out]  pOutConfig              pFinalMix を nn::audio::AcquireFinalMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]   pBufferMixer            pFinalMix に追加されている BufferMixer
 * @param[in]   pFinalMix               pBufferMixer を追加した FinalMixType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pBufferMixer != nullptr
 * - pBufferMixer は初期化されている
 * - @ref nn::audio::IsBufferMixerRemovable "nn::audio::IsBufferMixerRemovable(pBufferMixer)" == true
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * @post
 * - pBufferMixer は未初期化である
 */
void RemoveBufferMixer(AudioRendererConfig* pOutConfig, BufferMixerType* pBufferMixer, FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief       BufferMixer を SubMixType から削除します。
 * @param[out]  pOutConfig              pSubMix を nn::audio::AcquireSubMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]   pBufferMixer            pSubMix に追加されている BufferMixer
 * @param[in]   pSubMix                 pBufferMixer を追加した SubMixType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pBufferMixer != nullptr
 * - pBufferMixer は初期化されている
 * - @ref nn::audio::IsBufferMixerRemovable "nn::audio::IsBufferMixerRemovable(pBufferMixer)" == true
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * - pSubMix に pBufferMixer が追加されている
 * @post
 * - pBufferMixer は未初期化である
 */
void RemoveBufferMixer(AudioRendererConfig* pOutConfig, BufferMixerType* pBufferMixer, SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @brief       BufferMixer に設定されたミックス処理の入出力関係を取得します。
 * @param[in]   pBufferMixer            SubMixTyp / FinalMixType  に追加され、初期化済みの BufferMixer
 * @param[out]  outInput                pBufferMixer に設定されている入力ミックスバッファのインデックスの取得先となる配列
 * @param[out]  outOutput               pBufferMixer に設定されている出力ミックスバッファのインデックスの取得先となる配列
 * @param[out]  pOutCount               outInput および outOutput に取得されるミックスバッファのインデックスの個数
 * @param[in]   count                   outInput および outOutput の配列長
 * @pre
 * - pBufferMixer != nullptr
 * - pBufferMixer は初期化されている
 * - outInput != nullptr
 * - outOutput != nullptr
 * - pOutCount != nullptr
 * - 0 < count && count <= nn::audio::MixBufferCountMax
 */
void GetBufferMixerInputOutput(const BufferMixerType* pBufferMixer, int8_t* outInput, int8_t* outOutput, int* pOutCount, int count) NN_NOEXCEPT;

/**
 * @brief       BufferMixer で行うミックス処理の入出力関係を設定します。
 * @param[out]  pBufferMixer            SubMixType / FinalMixType に追加され、初期化済みの BufferMixer
 * @param[in]   input                   pBufferMixer に設定する入力ミックスバッファのインデックスの配列
 * @param[in]   output                  pBufferMixer に設定する出力ミックスバッファのインデックスの配列
 * @param[in]   count                   input および output に格納されているミックスバッファインデックスの個数
 * @pre
 * - pBufferMixer != nullptr
 * - pBufferMixer は初期化されている
 * - input != nullptr
 * - output != nullptr
 * - 0 < count && count <= nn::audio::MixBufferCountMax
 */
void SetBufferMixerInputOutput(BufferMixerType* pBufferMixer, const int8_t* input, const int8_t* output, int count) NN_NOEXCEPT;

/**
 * @brief       BufferMixer の各入出力関係に置けるボリューム設定を取得します。
 * @param[in]   pBufferMixer            SubMixType / FinalMixType に追加され、初期化済みの BufferMixer
 * @param[in]   index                   入出力関係のインデックス
 * @return      ボリューム値を返します。
 * @pre
 * - pBufferMixer != nullptr
 * - pBufferMixer は初期化されている
 * - 0 <= index && index <= nn::audio::MixBufferCountMax
 */
float GetBufferMixerVolume(const BufferMixerType* pBufferMixer, int index) NN_NOEXCEPT;

/**
 * @brief       BufferMixer の各入出力関係におけるボリュームを設定します。
 * @param[in]   pBufferMixer            SubMixType / FinalMixType に追加され、初期化済みの BufferMixer
 * @param[in]   index                   入出力関係のインデックス
 * @param[in]   volume                  index で指定するミックス処理において施すボリューム値
 * @pre
 * - pBufferMixer != nullptr
 * - pBufferMixer は初期化されている
 * - 0 <= index && index <= nn::audio::MixBufferCountMax
 * - 0.0f <= volume && volume <= 2.0f
 * @post
 * - @ref nn::audio::GetBufferMixerVolume "nn::audio::GetBufferMixerVolume(pBufferMixer, index)" == volume
 */
void SetBufferMixerVolume(BufferMixerType* pBufferMixer, int index, float volume) NN_NOEXCEPT;

/**
 * @brief       BufferMixer が有効かどうかを取得します。
 * @param[in]   pBufferMixer            BufferMixer
 * @return      有効であれば true を、そうでなければ false を返します。
 * @pre
 * - pBufferMixer != nullptr
 * - pBufferMixer は初期化されている
 */
bool IsBufferMixerEnabled(const BufferMixerType* pBufferMixer) NN_NOEXCEPT;

/**
 * @brief       BufferMixer の有効・無効を設定します。
 * @param[in]   pBufferMixer            BufferMixer
 * @param[in]   enable                  有効にするなら true, 無効にするなら false
 * @pre
 * - pBufferMixer != nullptr
 * - pBufferMixer は初期化されている
 * @post
 * - @ref nn::audio::IsBufferMixerEnabled "nn::audio::IsBufferMixerEnabled(pBufferMixer)" == enable
 */
void SetBufferMixerEnabled(BufferMixerType* pBufferMixer, bool enable) NN_NOEXCEPT;

/**
 * @brief       BufferMixer が安全に削除可能かどうかを取得します。
 * @param[in]   pBufferMixer            BufferMixer
 * @return      可能であれば true を返します。
 * @pre
 * - pBufferMixer != nullptr
 * - pBufferMixer は初期化されている
 */
bool IsBufferMixerRemovable(const BufferMixerType* pBufferMixer) NN_NOEXCEPT;

/**
 * @}
 */

/**
 * @name        オーディオレンダラ エフェクト Delay 関連 API
 * @{
 */

/**
 * @brief       DelayType を利用するためにレンダラが必要とするワークバッファサイズを取得します。
 * @param[in]   delayTimeMax            DealyType で設定しうる最大ディレイタイム
 * @param[in]   sampleRate              DelayType を追加する SubMixType / FinalMixType に設定されているサンプルレート
 * @param[in]   channelCountMax         利用する最大チャンネル数
 * @return      DelayType の利用に必要なバッファサイズを返します。
 * @pre
 * - (sampleRate == 32000) || (sampleRate == 48000)
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 */
size_t GetRequiredBufferSizeForDelay(nn::TimeSpan delayTimeMax, int sampleRate, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief       DelayType を FinalMixType に追加します。
 * @param[out]  pOutConfig              pFinalMix を  nn::audio::AcquireFinalMix() で初期化する際に使用した AudioRendererConfig
 * @param[out]  pDelay                  未初期化の DelayType
 * @param[in]   buffer                  DelayType で利用するメモリへのポインタ
 * @param[in]   bufferSize              buffer で指定するメモリのサイズ
 * @param[in]   pFinalMix               エフェクトを追加する FinalMix
 * @param[in]   delayTimeMax            設定可能なディレイタイムの最大値
 * @param[in]   channelCountMax         設定可能なチャンネル数の最大値
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pDelay != nullptr
 * - pDelay は未初期化である
 * - buffer != nullptr
 * - buffer は nn::audio::BufferAlignSize にアラインしている
 * - bufferSize >= @ref nn::audio::GetRequiredBufferSizeForDelay "nn::audio::GetRequiredBufferSizeForDelay(delayTimeMax, sampleRate, channelCountMax)"
 *   (sampleRate は pFinalMix に指定されているサンプルレート。 nn::audio::GetAudioRendererSampleRate() にて取得可能)
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 * @post
 * - pDelay は初期化されている
 * - pDelay は有効化されている
 * - pDelay への入出力バッファは未設定である
 */
Result AddDelay(AudioRendererConfig* pOutConfig, DelayType* pDelay, void* buffer, size_t bufferSize, FinalMixType* pFinalMix, nn::TimeSpan delayTimeMax, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief       DelayType を SubMixType に追加します。
 * @param[out]  pOutConfig              pSubMix を nn::audio::AcquireSubMix() で初期化する際に使用した AudioRendererConfig
 * @param[out]  pDelay                  未初期化の DelayType
 * @param[in]   buffer                  DelayType で利用するメモリへのポインタ
 * @param[in]   bufferSize              buffer で指定するメモリのサイズ
 * @param[in]   pSubMix                 エフェクトを追加する SubMix
 * @param[in]   delayTimeMax            設定可能なディレイタイムの最大値
 * @param[in]   channelCountMax         設定可能なチャンネル数の最大値
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pDelay != nullptr
 * - pDelay は未初期化である
 * - buffer != nullptr
 * - buffer は nn::audio::BufferAlignSize にアラインしている
 * - bufferSize >= @ref nn::audio::GetRequiredBufferSizeForDelay "nn::audio::GetRequiredBufferSizeForDelay" (delayTimeMax, @ref nn::audio::GetSubMixSampleRate "GetSubMixSampleRate(pSubMix)", channelCountMax)
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 * @post
 * - pDelay は初期化されている
 * - pDelay は有効化されている
 * - pDelay への入出力バッファは未設定である
 */
Result AddDelay(AudioRendererConfig* pOutConfig, DelayType* pDelay, void* buffer, size_t bufferSize, SubMixType* pSubMix, nn::TimeSpan delayTimeMax, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief       DelayType を FinalMixType から削除します。
 * @param[out]  pOutConfig              pFinalMix を  nn::audio::AcquireFinalMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]   pDelay                  pFinalMix に追加された DelayType
 * @param[in]   pFinalMix               pDelay を削除する FinalMixType
 * @return DelayType で利用されていたメモリ( nn::audio::AddDelay() での初期化時に buffer に渡したメモリ)を返します。
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pDelay != nullptr
 * - pDelay は初期化されている
 * - @ref nn::audio::IsDelayRemovable "nn::audio::IsDelayRemovable(pDelay)" == true
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * @post
 * - pDelay は未初期化である
 */
void* RemoveDelay(AudioRendererConfig* pOutConfig, DelayType* pDelay, FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief       DelayType を SubMixType から削除します。
 * @param[out]  pOutConfig              pSubMix を nn::audio::AcquireSubMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]   pDelay                  pSubMix に追加された DelayType
 * @param[in]   pSubMix                 pDelay を削除する SubMixType
 * @return DelayType で利用されていたメモリ( nn::audio::AddDelay() での初期化時に buffer に渡したメモリ)を返します。
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pDelay != nullptr
 * - pDelay は初期化されている
 * - @ref nn::audio::IsDelayRemovable "nn::audio::IsDelayRemovable(pDelay)" == true
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * @post
 * - pDelay は未初期化である
 */
void* RemoveDelay(AudioRendererConfig* pOutConfig, DelayType* pDelay, SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @brief       DelayType の入出力ミックスバッファのインデックスを取得します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @param[out]  outInput                pDelay に設定されている入力ミックスバッファのインデックスの取得先となる配列
 * @param[out]  outOutput               pDelay に設定されている出力ミックスバッファのインデックスの取得先となる配列
 * @param[out]  pOutCount               outInput および outOutput に取得されるミックスバッファのインデックスの個数
 * @param[in]   count                   outInput および outOutput の配列長
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 * - outInput != nullptr
 * - outOutput != nullptr
 * - pOutCount != nullptr
 * - (count == 1) || (count == 2) || (count == 4) || (count == 6)
 * @post
 * - outInput および outOutput に pOutCount 個のインデックス情報が書き込まれている
 */
void GetDelayInputOutput(const DelayType* pDelay, int8_t* outInput, int8_t* outOutput, int* pOutCount, int count) NN_NOEXCEPT;

/**
 * @brief       DelayType の入出力ミックスバッファインデックスを設定します。
 * @param[out]  pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @param[in]   input                   pDelay に設定する入力ミックスバッファのインデックスの配列
 * @param[in]   output                  pDelay に設定する出力ミックスバッファのインデックスの配列
 * @param[in]   count                   input および output に格納されているミックスバッファインデックスの個数
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 * - input != nullptr
 * - output != nullptr
 * - count <= @ref nn::audio::GetDelayChannelCountMax "nn::audio::GetDelayChannelCountMax(pDelay)"
 * - (count == 1) || (count == 2) || (count == 4) || (count == 6)
 */
void SetDelayInputOutput(DelayType* pDelay, const int8_t* input, const int8_t* output, int count) NN_NOEXCEPT;

/**
 * @brief       DelayType に設定可能なディレイタイムの最大値を取得します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @return      設定可能なディレイタイムの最大値を返します。
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 */
nn::TimeSpan GetDelayTimeMax(const DelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief       DelayType に設定可能なチャンネル数の最大値を取得します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @return      設定可能なチャンネル数の最大値を返します。
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 */
int GetDelayChannelCountMax(const DelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief       DelayType のディレイタイムを取得します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @return      ディレイタイムを返します。
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 */
nn::TimeSpan GetDelayTime(const DelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief       DelayType のディレイタイムを設定します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @param[in]   time                    ディレイタイム
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 * - time <= @ref GetDelayTimeMax "GetDelayTimeMax(pDelay)"
 * @post
 * - @ref nn::audio::GetDelayTimeMax "nn::audio::GetDelayTimeMax(pDelay)" == time
 */
void SetDelayTime(DelayType* pDelay, nn::TimeSpan time) NN_NOEXCEPT;

/**
 * @brief       DelayType のフィードバックゲインを取得します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @return      フィードバックゲインを返します。
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 */
float GetDelayFeedbackGain(const DelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief       DelayType のフィードバックゲインを設定します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @param[in]   feedbackGain            フィードバックゲイン
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 * - 0.0f <= feedbackGain && feedbackGain <= 1.0f
 * @post
 * - @ref nn::audio::GetDelayFeedbackGain "nn::audio::GetDelayFeedbackGain(pDelay)" == feedbackGain
 */
void SetDelayFeedbackGain(DelayType* pDelay, float feedbackGain) NN_NOEXCEPT;

/**
 * @brief       DelayType のドライゲインを取得します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @return      ドライゲインを返します。
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 */
float GetDelayDryGain(const DelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief       DelayType のドライゲインを設定します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @param[in]   dryGain                 ドライゲイン
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 * - 0.0f <= dryGain && dryGain <= 1.0f
 * @post
 * - @ref nn::audio::GetDelayDryGain "nn::audio::GetDelayDryGain(pDelay)" == dryGain
 */
void SetDelayDryGain(DelayType* pDelay, float dryGain) NN_NOEXCEPT;

/**
 * @brief       DelayType のインゲインを取得します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @return      インゲインを返します。
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 */
float GetDelayInGain(const DelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief       DelayType のインゲインを設定します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @param[in]   inGain                  インゲイン
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 * - 0.0f <= inGain && inGain <= 1.0f
 * @post
 * - @ref nn::audio::GetDelayInGain "nn::audio::GetDelayInGain(pDelay)" == inGain
 */
void SetDelayInGain(DelayType* pDelay, float inGain) NN_NOEXCEPT;

/**
 * @brief       DelayType のチャンネルスプレッドを取得します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @return      チャンネルスプレッドを返します。
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 */
float GetDelayChannelSpread(const DelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief       DelayType のチャンネルスプレッドを設定します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @param[in]   channelSpread           チャンネルスプレッド
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 * - 0.0f <= channelSpread && channelSpread <= 1.0f
 * @post
 * - @ref nn::audio::GetDelayChannelSpread "nn::audio::GetDelayChannelSpread(pDelay)" == channelSpread
 */
void SetDelayChannelSpread(DelayType* pDelay, float channelSpread) NN_NOEXCEPT;

/**
 * @brief       DelayType のローパスフィルタのパラメータを取得します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @return      ローパスフィルタのパラメータを返します。
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 */
float GetDelayLowPassAmount(const DelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief       DelayType のローパスフィルタのパラメータを設定します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @param[in]   lowPassAmount           ローパスフィルタのパラメータ
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 * - 0.0f <= lowPassAmount && lowPassAmount <= 1.0f
 * @post
 * - @ref nn::audio::GetDelayLowPassAmount "nn::audio::GetDelayLowPassAmount(pDelay)" == lowPassAmount
 */
void SetDelayLowPassAmount(DelayType* pDelay, float lowPassAmount) NN_NOEXCEPT;

/**
 * @brief       DelayType が有効かどうかを取得します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @return      有効であれば true を、そうでなければ false を返します。
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 */
bool IsDelayEnabled(const DelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief       DelayType の有効・無効を設定します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @param[in]   enable                  有効にするなら true, 無効にするなら false
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 * @post
 * - @ref nn::audio::IsDelayEnabled "nn::audio::IsDelayEnabled(pDelay)" == enable
 */
void SetDelayEnabled(DelayType* pDelay, bool enable) NN_NOEXCEPT;

/**
 * @brief       DelayType が安全に削除可能かどうかを取得します。
 * @param[in]   pDelay                  SubMixType / FinalMixType に追加され初期化された DelayType
 * @return      可能であれば true を返します。
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 */
bool IsDelayRemovable(const DelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief           nn::audio::DelayType のエフェクトパラメータを取得します。
 * @param[in]       pDelay                  初期化された nn::audio::DelayType へのポインタ
 * @return          エフェクトパラメータを返します。
 * @pre
 * - pOutDelay != nullptr
 * - pOutDelay は初期化されている
 */
DelayParameterSet GetDelayParameters(const DelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief           nn::audio::DelayType のエフェクトパラメータを設定します。
 * @param[out]      pOutDelay               初期化された nn::audio::DelayType へのポインタ
 * @param[in]       pParameterSet            nn::audio::DelayType に設定するエフェクトパラメータ
 * @pre
 * - pOutDelay != nullptr
 * - pOutDelay は初期化されている
 * - pParameterSet != nullptr
 * - @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(0)" <= pParameterSet->delayTime && pParameterSet->delayTime <= nn::audio::GetDelayTimeMax()
 * - 0.0f <= pParameterSet->inGain && pParameterSet->inGain <= 1.0f
 * - 0.0f <= pParameterSet->feedbackGain && pParameterSet->feedbackGain <= 1.0f
 * - 0.0f <= pParameterSet->dryGain && pParameterSet->dryGain <= 1.0f
 * - 0.0f <= pParameterSet->channelSpread && pParameterSet->channelSpread <= 1.0f
 * - 0.0f <= pParameterSet->lowPassAmount && pParameterSet->lowPassAmount <= 1.0f
 * @details
 * 各パラメータが持つ意味は@confluencelink{93362777, オーディオエフェクトの説明の「 ディレイパラメータ」を参照してください。}
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutDelay への操作を行わないでください。
 */
void SetDelayParameters(DelayType* pOutDelay, const DelayParameterSet* pParameterSet) NN_NOEXCEPT;

/**
 * @}
 */

/**
 * @name        オーディオレンダラ エフェクト Reverb 関連 API
 * @{
 */

/**
 * @brief           ReverbType を利用するためにレンダラが必要とするワークバッファサイズを取得します。
 * @param[in]       sampleRate          ReverbType を追加する SubMixType / FinalMixType に設定されているサンプルレート
 * @param[in]       channelCountMax     利用する最大チャンネル数
 * @return          ReverbType の利用に必要なバッファサイズを返します。
 * @pre
 * - (sampleRate == 32000) || (sampleRate == 48000)
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 */
size_t GetRequiredBufferSizeForReverb(int sampleRate, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief           ReverbType を FinalMixType に追加します。
 * @param[out]      pOutConfig         pFinalMix を nn::audio::AcquireFinalMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]       pReverb            未初期化の ReverbType
 * @param[in]       buffer             ReverbType で利用するメモリへのポインタ
 * @param[in]       bufferSize         buffer で指定するメモリのサイズ
 * @param[in]       pFinalMix          エフェクトを追加する FinalMixType
 * @param[in]       channelCountMax    pReverb に設定可能なチャンネル数の最大値
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pReverb != nullptr
 * - pReverb は未初期化である
 * - buffer != nullptr
 * - buffer は nn::audio::BufferAlignSize にアラインしている
 * - bufferSize >= @ref nn::audio::GetRequiredBufferSizeForReverb "nn::audio::GetRequiredBufferSizeForReverb(sampleRate, channelCountMax)"
 *   (sampleRate は pFinalMix に指定されているサンプルレート。 nn::audio::GetAudioRendererSampleRate() にて取得可能)
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 */
Result AddReverb(AudioRendererConfig* pOutConfig, ReverbType* pReverb, void* buffer, size_t bufferSize, FinalMixType* pFinalMix, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief           ReverbType を SubMixType に追加します。
 * @param[out]      pOutConfig         pSubMix を nn::audio::AcquireSubMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]       pReverb            未初期化の ReverbType
 * @param[in]       buffer             ReverbType で利用するメモリへのポインタ
 * @param[in]       bufferSize         buffer で指定するメモリのサイズ
 * @param[in]       pSubMix            エフェクトを追加する SubMixType
 * @param[in]       channelCountMax    pReverb に設定可能なチャンネル数の最大値
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pReverb != nullptr
 * - pReverb は未初期化である
 * - buffer != nullptr
 * - buffer は nn::audio::BufferAlignSize にアラインしている
 * - bufferSize >= @ref nn::audio::GetRequiredBufferSizeForReverb "nn::audio::GetRequiredBufferSizeForReverb(GetSubMixSampleRate(pSubMix)", channelCountMax)
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 */
Result AddReverb(AudioRendererConfig* pOutConfig, ReverbType* pReverb, void* buffer, size_t bufferSize, SubMixType* pSubMix, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief           ReverbType を FianlMix から削除します。
 * @param[out]      pOutConfig         pFinalMix を  nn::audio::AcquireFinalMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]       pReverb            pFinalMix に追加された ReverbType
 * @param[in]       pFinalMix          pReverb を削除する FinalMixType
 * @return ReverbType で利用されていたメモリ( nn::audio::AddReverb() での初期化時に buffer に渡したメモリ)を返します。
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - @ref nn::audio::IsReverbRemovable "nn::audio::IsReverbRemovable(pReverb)" == true
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * @post
 * - pReverb は未初期化である
 */
void* RemoveReverb(AudioRendererConfig* pOutConfig, ReverbType* pReverb, FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief           ReverbType を SubMixType から削除します。
 * @param[out]      pOutConfig         pSubMix を nn::audio::AcquireSubMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]       pReverb            pSubMix に追加された ReverbType
 * @param[in]       pSubMix            pReverb を削除する SubMixType
 * @return ReverbType で利用されていたメモリ( nn::audio::AddReverb() での初期化時に buffer に渡したメモリ)を返します。
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - @ref nn::audio::IsReverbRemovable "nn::audio::IsReverbRemovable(pReverb)" == true
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * @post
 * - pReverb は未初期化である
 */
void* RemoveReverb(AudioRendererConfig* pOutConfig, ReverbType* pReverb, SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @brief       ReverbType に設定可能なチャンネル数の最大値を取得します。
 * @param[in]   pReverb                  SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return      設定可能なチャンネル数の最大値を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
int GetReverbChannelCountMax(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           ReverbType の入出力バッファインデックスを設定します。
 * @param[in]       pReverb            SubMixType / FinalMixType に追加され初期化された ReverbType
 * @param[in]       input              pReverb に設定する入力ミックスバッファのインデックスの配列
 * @param[in]       output             pReverb に設定する出力ミックスバッファのインデックスの配列
 * @param[in]       count              input および output に格納されているミックスバッファインデックスの個数
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - input != nullptr
 * - output != nullptr
 * - (count == 1) || (count == 2) || (count == 4)
 */
void SetReverbInputOutput(ReverbType* pReverb, const int8_t* input, const int8_t* output, int count) NN_NOEXCEPT;

/**
 * @brief           ReverbType の入出力バッファインデックスを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された DelayType
 * @param[in]       outInput             pReverb に設定されている入力ミックスバッファのインデックスの取得先となる配列
 * @param[in]       outOutput            pReverb に設定されている出力ミックスバッファのインデックスの取得先となる配列
 * @param[in]       pOutCount            outInput および outOutput に取得されるミックスバッファのインデックスの個数
 * @param[in]       count                outInput および outOutput の配列帳
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - outInput != nullptr
 * - outOutput != nullptr
 * - pOutCount != nullptr
 * - (count == 1) || (count == 2) || (count == 4)
 */
void GetReverbInputOutput(const ReverbType* pReverb, int8_t* outInput, int8_t* outOutput, int* pOutCount, int count) NN_NOEXCEPT;

/**
 * @brief           ReverbType の初期反射音モードを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された DelayType
 * @return          初期反射音モードを返します。
 * @pre
 * - ReverbType * pReverb
 * - pReverb は初期化されている
 */
ReverbType::EarlyMode GetReverbEarlyMode(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           ReverbType の初期反射音モードを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @param[in]       mode                 初期反射音モード
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * @post
 * - @ref nn::audio::GetReverbEarlyMode "nn::audio::GetReverbEarlyMode(pReverb)" == mode
 */
void SetReverbEarlyMode(ReverbType* pReverb, ReverbType::EarlyMode mode) NN_NOEXCEPT;

/**
 * @brief           ReverbType の初期反射音のゲインを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return          初期反射音のゲインを返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetReverbEarlyGain(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           ReverbType の初期反射音のゲインを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @param[in]       gain                 初期反射音のゲイン
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - 0.0f <= gain <= 1.0f
 * @post
 * - @ref nn::audio::GetReverbEarlyGain "nn::audio::GetReverbEarlyGain(pReverb)" == gain
 */
void SetReverbEarlyGain(ReverbType* pReverb, float gain) NN_NOEXCEPT;

/**
 * @brief           ReverbType のプレディレイの時間を取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return          プレディレイの時間を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
nn::TimeSpan GetReverbPredelayTime(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           ReverbType のプレディレイの時間を設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @param[in]       time                 プレディレイの時間
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(0)" <= time <= @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(300)"
 * @post
 * - @ref nn::audio::GetReverbPredelayTime "nn::audio::GetReverbPredelayTime(pReverb)" == time
 */
void SetReverbPredelayTime(ReverbType* pReverb, nn::TimeSpan time) NN_NOEXCEPT;

/**
 * @brief           ReverbType の後期残響音モードを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return          後期残響音モードを返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
ReverbType::LateMode GetReverbLateMode(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           ReverbType の後期残響音モードを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @param[in]       mode                 後期残響音モード
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * @post
 * - @ref nn::audio::GetReverbLateMode "nn::audio::GetReverbLateMode(pReverb)" == mode
 */
void SetReverbLateMode(ReverbType* pReverb, ReverbType::LateMode mode) NN_NOEXCEPT;

/**
 * @brief           ReverbType の後期残響音のゲインを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return          後期残響音のゲインを返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetReverbLateGain(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           ReverbType の後期残響音のゲインを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @param[in]       gain                 後期残響音のゲイン
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - 0.0f <= gain <= 1.0f
 * @post
 * - @ref nn::audio::GetReverbLateGain "nn::audio::GetReverbLateGain(pReverb)" == gain
 */
void SetReverbLateGain(ReverbType* pReverb, float gain) NN_NOEXCEPT;

/**
 * @brief           ReverbType の残響時間を取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return          残響時間を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
nn::TimeSpan GetReverbDecayTime(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           ReverbType の残響時間を設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @param[in]       time                 残響時間
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(100)" <= time <= @ref nn::TimeSpan::FromSeconds "nn::TimeSpan::FromSeconds(20)"
 * @post
 * - @ref nn::audio::GetReverbDecayTime "nn::audio::GetReverbDecayTime(pReverb)" == time
 */
void SetReverbDecayTime(ReverbType* pReverb, nn::TimeSpan time) NN_NOEXCEPT;

/**
 * @brief           ReverbType の高周波成分の減衰率を取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return          高周数成分の減衰率を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetReverbHighFrequencyDecayRatio(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           ReverbType の高周波数成分の減衰率を設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @param[in]       ratio                高周波成分の減衰率
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - 0.1f <= ratio <= 1.0f
 * @post
 * - @ref nn::audio::GetReverbHighFrequencyDecayRatio "nn::audio::GetReverbHighFrequencyDecayRatio(pReverb)"== ratio
 */
void SetReverbHighFrequencyDecayRatio(ReverbType* pReverb, float ratio) NN_NOEXCEPT;

/**
 * @brief           ReverbType の残響音の音色パラメータを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return          残響音の音色パラメータを返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetReverbColoration(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           ReverbType の残響音の音色パラメータを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @param[in]       value                残響音の音色
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - 0.0f <= value <= 1.0f
 * @post
 * - @ref nn::audio::GetReverbColoration "nn::audio::GetReverbColoration(pReverb)" == value
 */
void SetReverbColoration(ReverbType* pReverb, float value) NN_NOEXCEPT;

/**
 * @brief           ReverbType のリバーブゲインを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return          リバーブゲインを返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetReverbReverbGain(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           ReverbType のリバーブゲインを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @param[in]       gain                 リバーブゲイン
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * @post
 * - @ref nn::audio::GetReverbReverbGain "nn::audio::GetReverbReverbGain(pReverb)" == gain
 */
void SetReverbReverbGain(ReverbType* pReverb, float gain) NN_NOEXCEPT;

/**
 * @brief           ReverbType の出力ゲインを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return          出力ゲインを返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetReverbOutGain(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           ReverbType の出力ゲインを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @param[in]       gain                 出力ゲイン
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - 0.0f <= gain <= 1.0f
 */
void SetReverbOutGain(ReverbType* pReverb, float gain) NN_NOEXCEPT;

/**
 * @brief           ReverbType のドライゲインを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return          ドライゲインを返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetReverbDryGain(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           ReverbType のドライゲインを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @param[in]       gain                 ドライゲイン
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - 0.0f <= gain <= 1.0f
 * @post
 * - @ref nn::audio::GetReverbDryGain "nn::audio::GetReverbDryGain(pReverb)" == gain
 */
void SetReverbDryGain(ReverbType* pReverb, float gain) NN_NOEXCEPT;

/**
 * @brief           nn::audio::ReverbType のエフェクトパラメータを取得します。
 * @param[in]       pReverb                  初期化された nn::audio::ReverbType へのポインタ
 * @return          エフェクトパラメータを返します。
 * @pre
 * - pOutReverb != nullptr
 * - pOutReverb は初期化されている
 */
ReverbParameterSet GetReverbParameters(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           nn::audio::ReverbType のエフェクトパラメータを設定します。
 * @param[out]      pOutReverb               初期化された nn::audio::ReverbType へのポインタ
 * @param[in]       pParameterSet            nn::audio::ReverbType に設定するエフェクトパラメータ
 * @pre
 * - pOutReverb != nullptr
 * - pOutReverb は初期化されている
 * - pParameterSet != nullptr
 * - pParameterSet->earlyMode == nn::audio::ReverbType::EarlyMode_SmallRoom ||  pParameterSet->earlyMode == nn::audio::ReverbType::EarlyMode_LargeRoom || pParameterSet->earlyMode == nn::audio::ReverbType::EarlyMode_Hall || pParameterSet->earlyMode == nn::audio::ReverbType::EarlyMode_Cavern || pParameterSet->earlyMode == nn::audio::ReverbType::EarlyMode_NoEarlyReflection
 * - 0.0f <= pParameterSet->earlyGain && pParameterSet->earlyGain <= 1.0f
 * - 0.0f <= pParameterSet->predelayTimeMilliSeconds && pParameterSet->predelayTimeMilliSeconds <= 300.0f
 * - pParameterSet->lateMode == nn::audio::ReverbType::LateMode_Hall || pParameterSet->lateMode == nn::audio::ReverbType::LateMode_MetalCorridor || pParameterSet->lateMode == nn::audio::ReverbType::LateMode_Cavern || pParameterSet->lateMode == nn::audio::ReverbType::LateMode_MaximumDelay
 * - 0.0f <= pParameterSet->lateGain && pParameterSet->lateGain <= 1.0f
 * - 0.1f <= pParameterSet->decayTimeSeconds && pParameterSet->decayTimeSeconds <= 20.0f
 * - 0.1f <= pParameterSet->highFreqDecayRatio && pParameterSet->highFreqDecayRatio <= 1.0f
 * - 0.0f <= pParameterSet->coloration && pParameterSet->coloration <= 1.0f
 * - 0.0f <= pParameterSet->reverbGain && pParameterSet->reverbGain <= 1.0f
 * - 0.0f <= pParameterSet->outGain && pParameterSet->outGain <= 1.0f
 * - 0.0f <= pParameterSet->dryGain && pParameterSet->dryGain <= 1.0f
 * @details
 * 各パラメータが持つ意味は@confluencelink{93362777, オーディオエフェクトの説明の「 リバーブパラメータ」を参照してください。}
 *
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutReverb への操作を行わないでください。
 */
void SetReverbParameters(ReverbType* pOutReverb, const ReverbParameterSet* pParameterSet) NN_NOEXCEPT;

/**
 * @brief       ReverbType が有効かどうかを取得します。
 * @param[in]   pReverb                  SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return      有効であれば true を、そうでなければ false を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
bool IsReverbEnabled(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief       ReverbType の有効・無効を設定します。
 * @param[in]   pReverb                 SubMixType / FinalMixType に追加され初期化された ReverbType
 * @param[in]   enable                  有効にするなら true, 無効にするなら false
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * @post
 * - @ref nn::audio::IsReverbEnabled "nn::audio::IsReverbEnabled(pReverb)" == enable
 */
void SetReverbEnabled(ReverbType* pReverb, bool enable) NN_NOEXCEPT;

/**
 * @brief       ReverbType が安全に削除可能かどうかを取得します。
 * @param[in]   pReverb                  SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return      可能であれば true を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
bool IsReverbRemovable(const ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @}
 */

/**
 * @name        オーディオレンダラ エフェクト Aux 関連 API
 * @{
 */

/**
 * @brief       AuxType を利用するためにレンダラが必要とするワークバッファサイズを取得します。
 * @param[in]   pParameter               AuxType を追加する SubMixType / FinalMixType が属するオーディオレンダラを nn::audio::OpenAudioRenderer() で取得する際に利用したパラメータ
 * @param[in]   mixBufferFrameCount      AuxBuffer に持たせたいオーディオフレーム数
 * @param[in]   channelCountMax          AuxBuffer に格納するチャンネル数
 * @return      バッファサイズ [byte] を返します。
 * @pre
 * - pParameter != nullptr
 * - pParameter は初期化されている
 * - mixBufferFrameCount > 1
 * - channelCountMax > 0
 */
size_t GetRequiredBufferSizeForAuxSendReturnBuffer(const AudioRendererParameter* pParameter, int mixBufferFrameCount, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief       AuxType を FinalMixType へ追加します。
 * @param[out]  pOutConfig               pFinalMix を nn::audio::AcquireFinalMix() で初期化する際に使用した AudioRendererConfig
 * @param[out]  pAux                     未初期化の AuxType
 * @param[in]   pFinalMix                エフェクトを追加する先の FinalMixType
 * @param[in]   pSendBuffer              オーディオレンダラからのサンプルを格納するのに用いるバッファ
 * @param[in]   pReturnBuffer            オーディオレンダラへ返すサンプルを格納するのに用いるバッファ
 * @param[in]   sendReturnBufferSize     pSendBuffer, pReturnBuffer のサイズ [byte]
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pAux != nullptr
 * - pAux は未初期化である
 * - pFinalMix != nullptr
 * - pSendBuffer != nullptr
 * - pSendBuffer は nn::audio::BufferAlignSize にアラインしている
 * - pReturnBuffer != nullptr
 * - pReturnBuffer は nn::audio::BufferAlignSize にアラインしている
 * - pReturnBuffer != pSendBuffer
 * - sendReturnBufferSize > 0
 * @post
 * - pAux は初期化されている
 * - pAux は有効化されている
 * - pAux への入出力バッファは未設定である
 */
Result AddAux(AudioRendererConfig* pOutConfig, AuxType* pAux, FinalMixType* pFinalMix, void* pSendBuffer, void* pReturnBuffer, size_t sendReturnBufferSize) NN_NOEXCEPT;

/**
 * @brief       AuxType を SubMixType へ追加します。
 * @param[out]  pOutConfig               pSubMix を nn::audio::AcquireSubMix() で初期化する際に使用した AudioRendererConfig
 * @param[out]  pAux                     未初期化の AuxType
 * @param[in]   pSubMix                  エフェクトを追加する SubMixType
 * @param[in]   pSendBuffer              オーディオレンダラからのサンプルを格納するのに用いるバッファ
 * @param[in]   pReturnBuffer            オーディオレンダラへ返すサンプルを格納するのに用いるバッファ
 * @param[in]   sendReturnBufferSize     pSendBuffer, pReturnBuffer のサイズ [byte]
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pAux != nullptr
 * - pAux は未初期化である
 * - pSubMix != nullptr
 * - pSendBuffer != nullptr
 * - pSendBuffer は nn::audio::BufferAlignSize にアラインしている
 * - pReturnBuffer != nullptr
 * - pReturnBuffer は nn::audio::BufferAlignSize にアラインしている
 * - pReturnBuffer != pSendBuffer
 * - sendReturnBufferSize > 0
 * @post
 * - pAux は初期化されている
 * - pAux は有効化されている
 * - pAux への入出力バッファは未設定である
 */
Result AddAux(AudioRendererConfig* pOutConfig, AuxType* pAux, SubMixType* pSubMix, void* pSendBuffer, void* pReturnBuffer, size_t sendReturnBufferSize) NN_NOEXCEPT;

/**
 * @brief       AuxType を FinalMixType から削除します。
 * @param[out]  pOutConfig              pFinalMix を  nn::audio::AcquireFinalMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]   pAux                    pFinalMix に追加された AuxType
 * @param[in]   pFinalMix               pAux を削除する FinalMixType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pAux != nullptr
 * - pAux は初期化されている
 * - @ref nn::audio::IsAuxRemovable "nn::audio::IsAuxRemovable(pAux)" == true
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * @post
 * - pAux は未初期化である
 */
void RemoveAux(AudioRendererConfig* pOutConfig, AuxType* pAux, FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief       AuxType を SubMixType から削除します。
 * @param[out]  pOutConfig              pSubMix を nn::audio::AcquireSubMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]   pAux                    pSubMix に追加された AuxType
 * @param[in]   pSubMix                 pAux を削除する SubMixType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pAux != nullptr
 * - pAux は初期化されている
 * - @ref nn::audio::IsAuxRemovable "nn::audio::IsAuxRemovable(pAux)" == true
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * @post
 * - pAux は未初期化である
 */
void RemoveAux(AudioRendererConfig* pOutConfig, AuxType* pAux, SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @brief       AuxType に設定可能なチャンネル数の最大値を取得します。
 * @param[in]   pAux                  SubMixType / FinalMixType に追加され初期化された AuxType
 * @return      設定可能なチャンネル数の最大値を返します。
 * @pre
 * - pAux != nullptr
 * - pAux は初期化されている
 */
int GetAuxChannelCountMax(const AuxType* pAux) NN_NOEXCEPT;

/**
 * @brief       AuxType の入出力バッファインデックスを取得します。
 * @param[in]   pAux                    SubMixType / FinalMixType に追加され初期化された AuxType
 * @param[out]  pOutInput               pAux に設定されている入力ミックスバッファのインデックスの取得先となる配列
 * @param[out]  pOutOutput              pAux に設定されている出力ミックスバッファのインデックスの取得先となる配列
 * @param[out]  pOutCount               pOutInput および pOutOutput に取得されるミックスバッファのインデックスの個数
 * @param[in]   count                   pOutInput および pOutOutput の配列長
 * @pre
 * - pAux != nullptr
 * - pAux は初期化されている
 * - pOutInput != nullptr
 * - pOutOutput != nullptr
 * - pOutCount != nullptr
 * - 0 < count && count <= nn::audio::MixBufferCountMax
 * @post
 * - pOutInput および pOutOutput に pOutCount 個のインデックス情報が書き込まれている
 */
void GetAuxInputOutput(const AuxType* pAux, int8_t* pOutInput, int8_t* pOutOutput, int* pOutCount, int count) NN_NOEXCEPT;

/**
 * @brief       ExternEffect の入出力バッファインデックスを設定します。
 * @param[out]  pAux                    SubMixType / FinalMixType に追加され初期化された AuxType
 * @param[in]   input                   pAux に設定する入力ミックスバッファのインデックスの配列
 * @param[in]   output                  pAux に設定する出力ミックスバッファのインデックスの配列
 * @param[in]   count                   input および output に格納されているミックスバッファインデックスの個数
 * @pre
 * - pAux != nullptr
 * - pAux は初期化されている
 * - input != nullptr
 * - output != nullptr
 * - 0 < count
 * - count <= (pAux の初期化時に接続した SubMixType / FinalMixType の持つミックスバッファ数)
 * - count <= (pAux の初期化時に pSendBuffer / pReturnBuffer として渡したバッファサイズから許容されるチャンネル数)
 */
void SetAuxInputOutput(AuxType* pAux, const int8_t* input, const int8_t* output, const int count) NN_NOEXCEPT;

/**
 * @brief       AuxType から count 数のサンプルを読み込みます。
 * @param[in]   pAux                    SubMixType / FinalMixType に追加され初期化された AuxType
 * @param[out]  pOutData                指定数のサンプル列が読み込まれるバッファ
 * @param[in]   count                   読み込みサンプル数を指定
 * @return      実際に読み込まれたサンプル数を返します。
 * @pre
 * - pAux != nullptr
 * - pAux は初期化されている
 * - pOutData != nullptr
 * - count >= 0
 */
int ReadAuxSendBuffer( AuxType* pAux, int32_t* pOutData, int count ) NN_NOEXCEPT;

/**
 * @brief       AuxType へ count 数のサンプルを書き込みます。
 * @param[out]  pAux                    SubMixType / FinalMixType に追加され初期化された AuxType
 * @param[in]   pData                   指定数のサンプル列が読み込まれるバッファ
 * @param[in]   count                   書き出すサンプル数を指定
 * @return      実際に書き込まれたサンプル数を返します。
 * @pre
 * - pAux != nullptr
 * - pAux は初期化されている
 * - pData != nullptr
 * - count >= 0
 */
int WriteAuxReturnBuffer( AuxType* pAux, const int32_t* pData, int count ) NN_NOEXCEPT;

/**
 * @brief       AuxType が管理する SendBuffer および ReturnBuffer のサンプルレートを取得します。
 * @param[in]   pAux                    SubMixType / FinalMixType に追加され初期化された AuxType
 * @return      サンプルレートを返します。
 * @pre
 * - pAux は初期化されている
 * - pAux != nullptr
 */
int GetAuxSampleRate( const AuxType* pAux ) NN_NOEXCEPT;

/**
 * @brief       AuxType が管理する SendBuffer および ReturnBuffer で保持可能な最大サンプル数を取得します。
 * @param[in]   pAux                    SubMixType / FinalMixType に追加され初期化された AuxType
 * @return      サンプル数を返します。
 * @pre
 * - pAux は初期化されている
 * - pAux != nullptr
 */
int GetAuxSampleCount( const AuxType* pAux ) NN_NOEXCEPT;

/**
 * @brief       AuxType が有効かどうかを取得します。
 * @param[in]   pAux                    SubMixType / FinalMixType に追加され初期化された AuxType
 * @return      有効であれば true を、そうでなければ false を返します。
 * @pre
 * - pAux != nullptr
 * - pAux は初期化されている
 */
bool IsAuxEnabled(const AuxType* pAux ) NN_NOEXCEPT;

/**
 * @brief       AuxType の有効・無効を設定します。
 * @param[out]  pAux                    SubMixType / FinalMixType に追加され初期化された AuxType
 * @param[in]   enable                  true ならば有効化、false なら無効化します。
 * @return 処理結果を返します。
 * @pre
 * - pAux != nullptr
 * - pAux は初期化されている
 * @post
 * - @ref nn::audio::IsAuxEnabled "nn::audio::IsAuxEnabled(pAux)" == enable
 */
void SetAuxEnabled(AuxType* pAux, bool enable) NN_NOEXCEPT;

/**
 * @brief       Aux が安全に削除可能かどうかを取得します。
 * @param[in]   pAux                    SubMixType / FinalMixType に追加され初期化された AuxType
 * @return      可能であれば true を返します。
 * @pre
 * - pAux != nullptr
 * - pAux は初期化されている
 */
bool IsAuxRemovable(const AuxType* pAux) NN_NOEXCEPT;


/**
 * @}
 */

/**
 * @name        オーディオレンダラ エフェクト I3DL2Reverb 関連 API
 * @{
 */

/**
 * @brief           I3dl2ReverbType を利用するためにレンダラが必要とするワークバッファサイズを取得します。
 * @param[in]       sampleRate           I3dl2ReverbType を追加する SubMixType / FinalMixType に設定されているサンプルレート
 * @param[in]       channelCountMax      利用する最大チャンネル数
 * @return          I3dl2ReverbType の利用に必要なバッファサイズを返します。
 * @pre
 * - (sampleRate == 32000) || (sampleRate == 48000)
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 */
size_t GetRequiredBufferSizeForI3dl2Reverb(int sampleRate, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType を FinalMixType に追加します。
 * @param[out]      pOutConfig           pFinalMix を nn::audio::AcquireFinalMix() で初期化する際に使用した AudioRendererConfig
 * @param[out]      pOutReverb           未初期化の I3dl2ReverbType
 * @param[in]       buffer               I3dl2ReverbType で利用するメモリへのポインタ
 * @param[in]       bufferSize           buffer で指定するメモリのサイズ
 * @param[in]       pFinalMix            エフェクトを追加する FinalMixType
 * @param[in]       channelCountMax      設定可能なチャンネル数の最大値
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutReverb != nullptr
 * - pOutReverb は未初期化である
 * - buffer != nullptr
 * - buffer は nn::audio::BufferAlignSize にアラインしている
 * - bufferSize => @ref nn::audio::GetRequiredBufferSizeForI3dl2Reverb "nn::audio::GetRequiredBufferSizeForI3dl2Reverb(sampleRate, channelCountMax)"
 *   (sampleRate は pFinalMix に指定されているサンプルレート。 nn::audio::GetAudioRendererSampleRate() にて取得可能)
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 * @post
 * - pOutReverb は初期化されている
 * - pOutReverb は有効化されている
 * - pOutReverb への入出力バッファは未設定である
 */
Result AddI3dl2Reverb(AudioRendererConfig* pOutConfig, I3dl2ReverbType* pOutReverb, void* buffer, size_t bufferSize, FinalMixType* pFinalMix, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType を SubMixType に追加します。
 * @param[out]      pOutConfig           pSubMix を nn::audio::AcquireSubMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]       pOutReverb           未初期化の I3dl2ReverbType
 * @param[in]       buffer               I3dl2ReverbType で利用するメモリへのポインタ
 * @param[in]       bufferSize           buffer で指定するメモリのサイズ
 * @param[in]       pSubMix              エフェクトを追加する SubMixType
 * @param[in]       channelCountMax      pOutReverb に設定可能なチャンネルの最大値
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutReverb != nullptr
 * - pOutReverb は未初期化である
 * - buffer != nullptr
 * - buffer は nn::audio::BufferAlignSize にアラインしている
 * - bufferSize => @ref nn::audio::GetRequiredBufferSizeForI3dl2Reverb "nn::audio::GetRequiredBufferSizeForI3dl2Reverb(sampleRate, channelCountMax)"
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 * @post
 * - pOutReverb は初期化されている
 * - pOutReverb は有効化されている
 * - pOutReverb への入出力バッファは未設定である
 */
Result AddI3dl2Reverb(AudioRendererConfig* pOutConfig, I3dl2ReverbType* pOutReverb, void* buffer, size_t bufferSize, SubMixType* pSubMix, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief           ReverbType を FianlMix から削除します。
 * @param[out]      pOutConfig           pFinalMix を  nn::audio::AcquireFinalMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]       pReverb              pFinalMix に追加された I3dl2ReverbType
 * @param[in]       pFinalMix            pRevreb を削除する FinalMixType
 * @return          I3dl2ReverbType で利用されていたメモリ( nn::audio::AddI3dl2Reverb() での初期化時に buffer に渡したメモリ)を返します。
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - @ref nn::audio::IsI3dl2ReverbRemovable "nn::audio::IsI3dl2ReverbRemovable(pReverb)" == true
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * @post
 * - pReverb は未初期化である
 */
void* RemoveI3dl2Reverb(AudioRendererConfig* pOutConfig, I3dl2ReverbType* pReverb, FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief           ReverbType を SubMixType から削除します。
 * @param[out]      pOutConfig           pSubMix を nn::audio::AcquireSubMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]       pReverb              pSubMix に追加された I3dl2ReverbType
 * @param[in]       pSubMix              pReverb を削除する SubMixType
 * @return I2DL2ReverbType で利用されていたメモリ( nn::audio::AddI3dl2Reverb() での初期化時に buffer に渡したメモリ)を返します。
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - @ref nn::audio::IsReverbRemovable "nn::audio::IsReverbRemovable(pReverb)" == true
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * @post
 * - pReverb は未初期化である
 */
void* RemoveI3dl2Reverb(AudioRendererConfig* pOutConfig, I3dl2ReverbType* pReverb, SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType が安全に削除可能かどうかを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された ReverbType
 * @return          可能であれば true を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
bool IsI3dl2ReverbRemovable(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief       I3dl2ReverbType に設定可能なチャンネル数の最大値を取得します。
 * @param[in]   pReverb                  SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return      設定可能なチャンネル数の最大値を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
int GetI3dl2ReverbChannelCountMax(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType の入出力バッファインデックスを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[out]      outInput             pReverb に設定されている入力ミックスバッファのインデックスの取得先となる配列
 * @param[out]      outOutput            pReverb に設定されている出力ミックスバッファのインデックスの取得先となる配列
 * @param[out]      pOutCount            outInput および outOutput に取得されるミックスバッファのインデックスの個数
 * @param[in]       count                outInput および outOutput の配列長
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - outInput != nullptr
 * - outOutput != nullptr
 * - pOutCount != nullptr
 * - (count == 1) || (count == 2) || (count == 4) || (count == 6)
 */
void GetI3dl2ReverbInputOutput(const I3dl2ReverbType* pReverb, int8_t* outInput, int8_t* outOutput, int* pOutCount, int count) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType の入出力バッファインデックスを設定します。
 * @param[in]       pReverb            SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       input              pReverb に設定する入力ミックスバッファのインデックスの配列
 * @param[in]       output             pReverb に設定する出力ミックスバッファのインデックスの配列
 * @param[in]       count              input および output に格納されているミックスバッファインデックスの個数
 * @return
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - input != nullptr
 * - output != nullptr
 * - (count == 1) || (count == 2) || (count == 4) || (count == 6)
 */
void SetI3dl2ReverbInputOutput(I3dl2ReverbType* pReverb, const int8_t* input, const int8_t* output, int count) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType にルームゲインを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       room                 設定するルームゲイン値 [mB]
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - -10000.0f <= room && room <= 0.0f
 */
void SetI3dl2ReverbRoomGain(I3dl2ReverbType* pReverb, float room) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType のルームゲインを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return          ルームゲインを返します。単位は [mB] です。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetI3dl2ReverbRoomGain(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType に低周波成分を基準とする高周波成分の減衰量を設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       roomHf               低周波成分を基準とする高周波成分の減衰量 [mB]
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - -10000.0f <= roomHf && roomHf <= 0.0f
 */
void SetI3dl2ReverbRoomHfGain(I3dl2ReverbType* pReverb, float roomHf) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType の低周波成分を基準とする高周波成分の減衰量を取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return          低周波成分を基準とする高周波成分の減衰量を返します。単位は [mB] です。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetI3dl2ReverbRoomHfGain(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType に残響時間を設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       time
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(100)" <= time && time <= @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromSeconds(20)"
 */
void SetI3dl2ReverbLateReverbDecayTime(I3dl2ReverbType* pReverb, nn::TimeSpan time) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType の残響時間を取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return          残響時間を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
nn::TimeSpan GetI3dl2ReverbLateReverbDecayTime(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType に低周波の残響時間に対する高周波の残響時間の比を設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       hfDecayRatio         低周波の残響時間に対する高周波の残響時間の比
 * @return
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - 0.1f <= hfDecayRatio && hfDecayRatio <= 2.0f
 */
void SetI3dl2ReverbLateReverbHfDecayRatio(I3dl2ReverbType* pReverb, float hfDecayRatio) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType の低周波の残響時間に対する高周波の残響時間の比を取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return          低周波の残響時間に対する高周波の残響時間の比を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetI3dl2ReverbLateReverbHfDecayRatio(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType にルームゲインを基準とした初期反射音のレベルを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       reflections          初期反射音のレベル [mB]
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - -10000.0f <= reflections && reflections <= 1000.0f
 */
void SetI3dl2ReverbReflectionsGain(I3dl2ReverbType* pReverb, float reflections) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType のルームゲインを基準とした初期反射音のレベルを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return          初期反射音のレベルを返します。単位は [mB] です。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetI3dl2ReverbReflectionsGain(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType に直接音を基準とする初期反射音の遅延時間を設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       time                 初期反射音の遅延時間
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(0)" <= time && time <= @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(300)"
 */
void SetI3dl2ReverbReflectionDelayTime(I3dl2ReverbType* pReverb, nn::TimeSpan time) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType の直接音を基準とする初期反射音の遅延時間を取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return          初期反射音の遅延時間を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
nn::TimeSpan GetI3dl2ReverbReflectionDelayTime(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType にルームゲインを基準とした後期残響音のレベルを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       reverb               後期残響音のレベル [mB]
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - -10000.0f <= reverb && reverb <= 2000.0f
 */
void SetI3dl2ReverbReverbGain(I3dl2ReverbType* pReverb, float reverb) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType のルームゲインを基準とした後期残響音のレベルを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return          後期残響音のレベルを返します。単位は [mB] です。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetI3dl2ReverbReverbGain(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType に初期反射音を基準とする後期残響音の遅延時間を設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       time                 後期残響音の遅延時間
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(0)" <= time && time <= @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(100)"
 */
void SetI3dl2ReverbLateReverbDelayTime(I3dl2ReverbType* pReverb, nn::TimeSpan time) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType の初期反射音を基準とする後期残響音の遅延時間を取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return          後期残響音の遅延時間を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
nn::TimeSpan GetI3dl2ReverbLateReverbDelayTime(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType に後期残響音のエコー密度を設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       diffusion            後期残響音のエコー密度
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - 0.f <= diffusion && diffusion <= 100.f
 */
void SetI3dl2ReverbLateReverbDiffusion(I3dl2ReverbType* pReverb, float diffusion) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType の後期残響音のエコー密度を取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return          後期残響音のエコー密度を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetI3dl2ReverbLateReverbDiffusion(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType に後期残響音のモーダル密度を設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       density              後期残響音のモーダル密度
 * @return
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - 0.f <= density && density <= 100.f
 */
void SetI3dl2ReverbLateReverbDensity(I3dl2ReverbType* pReverb, float density) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType の後期残響音のモーダル密度を取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return          後期残響音のモーダル密度を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetI3dl2ReverbLateReverbDensity(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType に高周波の参照周波数を設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       hfReference          高周波の参照周波数 [Hz]
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - 20.f <= hfReference && hfReference <= 20000.f
 */
void SetI3dl2ReverbHfReference(I3dl2ReverbType* pReverb, float hfReference) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType の高周波の参照周波数を取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return          高周波の参照周波数を返します。単位は [Hz] です。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetI3dl2ReverbHfReference(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType にI3dl2ReverbType のドライゲインを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       dryGain             ドライゲイン
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - 0.f <= dryGain && dryGain <= 1.f
 */
void SetI3dl2ReverbDryGain(I3dl2ReverbType* pReverb, float dryGain) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType のドライゲインを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return          ドライゲインを返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
float GetI3dl2ReverbDryGain(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @deprecated      本 API は非推奨となりました。同等の機能を提供している void LoadI3dl2ReverbPreset(I3dl2ReverbParameterSet* pOutParameter, I3dl2ReverbType::Preset preset) の利用に切り替えてください。
 * @brief           I3dl2ReverbType にプリセットを設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       preset               プリセットの種類
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - nn::audio::I3dl2ReverbType::Preset::Preset_None <= preset && preset < nn::audio::I3dl2ReverbType::Preset::Preset_Count
 */
NN_DEPRECATED void LoadI3dl2ReverbPreset(I3dl2ReverbType* pReverb, I3dl2ReverbType::Preset preset) NN_NOEXCEPT;

/**
* @brief           nn::audio::I3dl2ReverbType のエフェクトパラメータを設定します。
* @param[out]      pOutReverb               初期化された nn::audio::I3dl2ReverbType へのポインタ
* @param[in]       pParameterSet            nn::audio::I3dl2ReverbParameterSet に設定するエフェクトパラメータ
* @pre
* - pOutReverb != nullptr
* - pOutReverb は初期化されている
* - pParameterSet != nullptr
* - -10000.0f <= pParameterSet->roomGain && pParameterSet->roomGain <= 0.0f
* - -10000.0f <= pParameterSet->roomHfGain && pParameterSet->roomHfGain <= 0.0f;
* - 0.1f <= pParameterSet->lateReverbDecayTime && pParameterSet->lateReverbDecayTime <= 20.0f
* - 0.1f <= pParameterSet->lateReverbHfDecayRatio && pParameterSet->lateReverbHfDecayRatio <= 2.0f
* - -10000.0f <= pParameterSet->reflectionsGain && pParameterSet->reflectionsGain <= 1000.0f
* - 0.0f <= pParameterSet->reflectionsDelayTime && pParameterSet->reflectionsDelayTime <= 0.3f
* - -10000.0f <= pParameterSet->reverbGain && pParameterSet->reverbGain <= 2000.0f
* - 0.0f <= pParameterSet->reverbDelayTime && pParameterSet->reverbDelayTime <= 0.1f
* - 0.1f <= pParameterSet->reverbDiffusion && pParameterSet->reverbDiffusion <= 100.0f
* - 0.1f <= pParameterSet->reverbDensity && pParameterSet->reverbDensity <= 100.0f
* - 20.0f <= pParameterSet->hfReference && pParameterSet->hfReference <= 20000.0f
* - 0.0f <= pParameterSet->dryGain && pParameterSet->dryGain <= 1.0f
* @details
* 各パラメータが持つ意味は@confluencelink{93362777, オーディオエフェクトの説明の「I3dl2 リバーブパラメータ」を参照してください。}
*
* この関数はスレッドセーフではありません。この関数の呼び出し中に pOutReverb への操作を行わないでください。
*/
void SetI3dl2ReverbParameters(I3dl2ReverbType* pOutReverb, const I3dl2ReverbParameterSet* pParameterSet) NN_NOEXCEPT;

/**
* @brief           nn::audio::I3dl2ReverbType のエフェクトパラメータを取得します。
* @param[in]       pReverb                  初期化された nn::audio::I3dl2ReverbType へのポインタ
* @return          エフェクトパラメータを返します。
* @pre
* - pOutReverb != nullptr
* - pOutReverb は初期化されている
*/
I3dl2ReverbParameterSet GetI3dl2ReverbParameters(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
* @brief           nn::audio::I3dl2ReverbType のプリセットに相当するエフェクトパラメータを取得します。
* @param[out]      pOutParameter            取得するエフェクトパラメータ
* @param[in]       preset                   nn::audio::I3dl2ReverbType::Preset に定義されているプリセットの種類
* @pre
* - pOutParameter != nullptr
* @details
* nn::audio::I3dl2ReverbType::Preset に準備されているプリセットに相当するエフェクトパラメータを取得します。
* この関数で取得したパラメータを nn::audio::SetI3dl2ReverbParameters() に指定することで、
* nn::audio::I3dl2ReverbType::Preset と同等のエフェクト処理を施すことが可能です。
*
* この関数はスレッドセーフではありません。この関数の呼び出し中に pOutReverb への操作を行わないでください。
*/
void LoadI3dl2ReverbPreset(I3dl2ReverbParameterSet* pOutParameter, I3dl2ReverbType::Preset preset) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType の有効・無効を設定します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @param[in]       enable               true ならば有効化、false なら無効化します。
 * @return
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
void SetI3dl2ReverbEnabled(I3dl2ReverbType* pReverb, bool enable) NN_NOEXCEPT;

/**
 * @brief           I3dl2ReverbType が有効かどうかを取得します。
 * @param[in]       pReverb              SubMixType / FinalMixType に追加され初期化された I3dl2ReverbType
 * @return          有効ならば true を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
bool IsI3dl2ReverbEnabled(const I3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @}
 */

/**
 * @name        オーディオレンダラ エフェクト BiquadFilter 関連 API
 * @{
 */

/**
 * @brief       BiquadFilterType を FinalMixType に追加します。
 * @param[out]  pOutConfig              pFinalMix を nn::audio::AcquireFinalMix() で初期化する際に使用した AudioRendererConfig
 * @param[out]  pBiquadFilter           未初期化の BiquadFilterType
 * @param[in]   pFinalMix               pBiquadFilter を追加する FinalMixType
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pBiquadFilter != nullptr
 * - pBiquadFilter は未初期化である
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * @post
 * - pBiquadFilter は初期化されている
 * - pBiquadFilter は有効化されている
 * - pBiquadFilter への入出力バッファは未設定である
 */
Result AddBiquadFilter(AudioRendererConfig* pOutConfig, BiquadFilterType* pBiquadFilter, FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief       BiquadFilterType を SubMixType に追加します。
 * @param[out]  pOutConfig              pSubMix を nn::audio::AcquireSubMix() で初期化する際に使用した AudioRendererConfig
 * @param[out]  pBiquadFilter           未初期化の BiquadFilterType
 * @param[in]   pSubMix                 エフェクトを追加する SubMixType
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pBiquadFilter != nullptr
 * - pBiquadFilter は未初期化である
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * @post
 * - pBiquadFilter は初期化されている
 * - pBiquadFilter は有効化されている
 * - pBiquadFilter への入出力バッファは未設定である
 */
Result AddBiquadFilter(AudioRendererConfig* pOutConfig, BiquadFilterType* pBiquadFilter, SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @brief       BiquadFilterType を FinalMixType から削除します。
 * @param[out]  pOutConfig              pFinalMix を nn::audio::AcquireFinalMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]   pBiquadFilter           pFinalMix に追加されている BiquadFilterType
 * @param[in]   pFinalMix               pBiquadFilter を追加した FinalMixType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pBiquadFilter != nullptr
 * - pBiquadFilter は初期化されている
 * - @ref nn::audio::IsBiquadFilterRemovable "nn::audio::IsBiquadFilterRemovable(pBiquadFilter)" == true
 * - pFinalMix != nullptr
 * - pFinalMix は初期化されている
 * @post
 * - pBiquadFilter は未初期化である
 */
void RemoveBiquadFilter(AudioRendererConfig* pOutConfig, BiquadFilterType* pBiquadFilter, FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief       BiquadFilterType を SubMixType から削除します。
 * @param[out]  pOutConfig              pSubMix を nn::audio::AcquireSubMix() で初期化する際に使用した AudioRendererConfig
 * @param[in]   pBiquadFilter           pSubMix に追加されている BiquadFilterType
 * @param[in]   pSubMix                 pBiquadFilter を追加した SubMixType
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pBiquadFilter != nullptr
 * - pBiquadFilter は初期化されている
 * - @ref nn::audio::IsBiquadFilterRemovable "nn::audio::IsBiquadFilterRemovable(pBiquadFilter)" == true
 * - pSubMix != nullptr
 * - pSubMix は初期化されている
 * - pSubMix に pBiquadFilter が追加されている
 * @post
 * - pBiquadFilter は未初期化である
 */
void RemoveBiquadFilter(AudioRendererConfig* pOutConfig, BiquadFilterType* pBiquadFilter, SubMixType* pSubMix) NN_NOEXCEPT;

/**
 * @brief       BiquadFilterType に設定されたミックス処理の入出力関係を取得します。
 * @param[in]   pBiquadFilter           SubMixType / FinalMixType  に追加され、初期化済みの BiquadFilterType
 * @param[out]  outInput                pBiquadFilter に設定されている入力ミックスバッファのインデックスの取得先となる配列
 * @param[out]  outOutput               pBiquadFilter に設定されている出力ミックスバッファのインデックスの取得先となる配列
 * @param[out]  pOutCount               outInput および outOutput に取得されるミックスバッファのインデックスの個数
 * @param[in]   count                   outInput および outOutput の配列長
 * @pre
 * - pBiquadFilter != nullptr
 * - pBiquadFilter は初期化されている
 * - outInput != nullptr
 * - outOutput != nullptr
 * - pOutCount != nullptr
 * - 0 < count && count <= nn::audio::BiquadFilterType::ChannelCountMax
 */
void GetBiquadFilterInputOutput(const BiquadFilterType* pBiquadFilter, int8_t* outInput, int8_t* outOutput, int* pOutCount, int count) NN_NOEXCEPT;

/**
 * @brief       BiquadFilterType で行うミックス処理の入出力関係を設定します。
 * @param[out]  pBiquadFilter            SubMixType / FinalMixType に追加され、初期化済みの BiquadFilterType
 * @param[in]   input                   pBiquadFilter に設定する入力ミックスバッファのインデックスの配列
 * @param[in]   output                  pBiquadFilter に設定する出力ミックスバッファのインデックスの配列
 * @param[in]   count                   input および output に格納されているミックスバッファインデックスの個数
 * @pre
 * - pBiquadFilter != nullptr
 * - pBiquadFilter は初期化されている
 * - input != nullptr
 * - output != nullptr
 * - 0 < count && count <= nn::audio::BiquadFilterType::ChannelCountMax
 */
void SetBiquadFilterInputOutput(BiquadFilterType* pBiquadFilter, const int8_t* input, const int8_t* output, int count) NN_NOEXCEPT;

/**
 * @brief       BiquadFilterType のパラメータを取得します。
 * @param[in]   pBiquadFilter            SubMixType / FinalMixType に追加され、初期化済みの BiquadFilterType
 * @return      パラメータを返します。
 * @pre
 * - pBiquadFilter != nullptr
 * - pBiquadFilter は初期化されている
 */
BiquadFilterParameter GetBiquadFilterParameter(const BiquadFilterType* pBiquadFilter) NN_NOEXCEPT;

/**
 * @brief       BiquadFilterType のパラメータを設定します。
 * @param[in]   pBiquadFilter           SubMixType / FinalMixType に追加され、初期化済みの BiquadFilterType
 * @param[in]   parameter               パラメータ
 * @pre
 * - pBiquadFilter != nullptr
 * - pBiquadFilter は初期化されている
 * @post
 * - GetBiquadFilterParameter(pBiquadFilter) == parameter
 * - IsBiquadFilterEnabled(pBiquadFilter) == parameter.enable
 */
void SetBiquadFilterParameter(BiquadFilterType* pBiquadFilter, const BiquadFilterParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       BiquadFilter が有効かどうかを取得します。
 * @param[in]   pBiquadFilter            BiquadFilter
 * @return      有効であれば true を、そうでなければ false を返します。
 * @pre
 * - pBiquadFilter != nullptr
 * - pBiquadFilter は初期化されている
 */
bool IsBiquadFilterEnabled(const BiquadFilterType* pBiquadFilter) NN_NOEXCEPT;

/**
 * @brief       BiquadFilterType の有効・無効を設定します。
 * @param[in]   pBiquadFilter            BiquadFilter
 * @param[in]   enable                  有効にするなら true, 無効にするなら false
 * @pre
 * - pBiquadFilter != nullptr
 * - pBiquadFilter は初期化されている
 * @post
 * - IsBiquadFilterEnabled(pBiquadFilter) == enable
 */
void SetBiquadFilterEnabled(BiquadFilterType* pBiquadFilter, bool enable) NN_NOEXCEPT;

/**
 * @brief       BiquadFilterType が安全に削除可能かどうかを取得します。
 * @param[in]   pBiquadFilter            BiquadFilter
 * @return      可能であれば true を返します。
 * @pre
 * - pBiquadFilter != nullptr
 * - pBiquadFilter は初期化されている
 */
bool IsBiquadFilterRemovable(const BiquadFilterType* pBiquadFilter) NN_NOEXCEPT;

/**
 * @}
 */

}  // namespace audio
}  // namespace nn
