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

#include <nn/nn_TimeSpan.h>
#include <nn/audio/audio_EffectTypes.h>
#include <nn/audio/audio_AuxUtilityEffectTypes.h>

namespace nn { namespace audio {

/**
* @name        CPU AuxI3DL2Reverb 関連 API
* @{
*/

/**
 * @brief           nn::audio::AuxI3dl2ReverbType の利用に必要なバッファのサイズを返します。
 * @param[in]       sampleRate               nn::audio::AuxI3dl2ReverbType を利用する nn::audio::AuxType に設定されているサンプルレート
 * @param[in]       channelCountMax          nn::audio::AuxI3dl2ReverbType に指定する最大チャンネル数
 * @return          バッファサイズを返します。 [byte]
 * @pre
 * - (sampleRate == 32000) || (sampleRate == 48000)
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 */
size_t GetRequiredBufferSizeForAuxI3dl2Reverb(int sampleRate, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxI3dl2ReverbType を初期化します。
 * @param[out]      pOutReverb               初期化する nn::audio::AuxI3dl2ReverbType へのポインタ
 * @param[in]       buffer                   nn::audio::AuxI3dl2ReverbType で利用するワークバッファ
 * @param[in]       bufferSize               buffer に指定したワークバッファのサイズ [byte]
 * @param[in]       sampleRate               nn::audio::AuxI3dl2ReverbType への入力となるサンプルのサンプルレート
 * @param[in]       channelCountMax          nn::audio::AuxI3dl2ReverbType への入力となるサンプルのチャンネル数
 * @pre
 * - pOutReverb != nullptr
 * - buffer != nullptr
 * - bufferSize >= @ref nn::audio::GetRequiredBufferSizeForAuxI3dl2Reverb "nn::audio::GetRequiredBufferSizeForAuxI3dl2Reverb(sampleRate, channelCountMax)"
 * - (sampleRate == 32000) || (sampleRate == 48000)
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 * @details
 * sampleRate には pOutReverb が行うエフェクト処理の対象となる nn::audio::AuxType のサンプルレートを指定してください。
 * nn::audio::GetAuxSampleRate() で取得可能です。@n
 * channelCountMax は pOutReverb に指定することになる最大入力チャンネル数を指定してくだい。
 */
void InitializeAuxI3dl2Reverb(AuxI3dl2ReverbType* pOutReverb, void* buffer, size_t bufferSize, int sampleRate, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief       AuxI3dl2ReverbType に設定可能なチャンネル数の最大値を取得します。
 * @param[in]   pReverb                初期化された nn::audio::AuxI3dl2ReverbType へのポインタ
 * @return      設定可能なチャンネル数の最大値を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
int GetAuxI3dl2ReverbChannelCountMax(const AuxI3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxI3dl2ReverbType の入出力チャンネルインデックスを取得します。
 * @param[out]      outInput                 pReverb に設定されている入力チャンネルのインデックスの取得先となる配列
 * @param[out]      outOutput                pReverb に設定されている出力チャンネルのインデックスの取得先となる配列
 * @param[out]      pOutCount                outInput および outOutput に取得されるチャンネルのインデックスの個数
 * @param[in]       pReverb                  初期化された nn::audio::AuxI3dl2ReverbType へのポインタ
 * @param[in]       count                    outInput および outOutput の配列長
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - outInput != nullptr
 * - outOutput != nullptr
 * - pOutCount != nullptr
 * - (count == 1) || (count == 2) || (count == 4) || (count == 6)
 * @details
 * nn::audio::SetAuxI3dl2ReverbInputOutput() で指定したチャンネルインデックスの配列を取得します。
 */
void GetAuxI3dl2ReverbInputOutput(int8_t* outInput, int8_t* outOutput, int* pOutCount, const AuxI3dl2ReverbType* pReverb, int count) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxI3dl2ReverbType の入出力チャンネルインデックスを設定します。
 * @param[out]      pOutReverb               初期化された nn::audio::AuxI3dl2ReverbType へのポインタ
 * @param[in]       input                    pOutReverb に設定する入力チャンネルのインデックスの配列
 * @param[in]       output                   pOutReverb に設定する出力チャンネルのインデックスの配列
 * @param[in]       count                    input および output に格納されているチャンネルインデックスの個数
 * @pre
 * - pOutReverb != nullptr
 * - pOutReverb は初期化されている
 * - input != nullptr
 * - output != nullptr
 * - (count == 1) || (count == 2) || (count == 4) || (count == 6)
 * @details
 * nn::audio::ProcessAuxI3dl2Reverb() で inBuffer に指定する入力サンプルは、ブロックインターリーブされていることを期待しています。
 * この関数では nn::audio::ProcessAuxI3dl2Reverb() が行うエフェクト処理を、どのチャンネルに対して実施するかを指定します。
 *
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutReverb への操作を行わないでください。
 */
void SetAuxI3dl2ReverbInputOutput(AuxI3dl2ReverbType* pOutReverb, const int8_t* input, const int8_t* output, int count) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxI3dl2ReverbType のエフェクトパラメータを設定します。
 * @param[out]      pOutReverb               初期化された nn::audio::AuxI3dl2ReverbType へのポインタ
 * @param[in]       pParameterSet            nn::audio::AuxI3dl2ReverbType に設定するエフェクトパラメータ
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
void SetAuxI3dl2ReverbParameters(AuxI3dl2ReverbType* pOutReverb, const I3dl2ReverbParameterSet* pParameterSet) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxI3dl2ReverbType のエフェクトパラメータを取得します。
 * @param[in]       pReverb                  初期化された nn::audio::AuxI3dl2ReverbType へのポインタ
 * @return          outParameterSet          取得するエフェクトパラメータ
 * @pre
 * - pOutReverb != nullptr
 * - pOutReverb は初期化されている
 */
I3dl2ReverbParameterSet GetAuxI3dl2ReverbParameters(const AuxI3dl2ReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxI3dl2ReverbType のエフェクト処理を実行します。
 * @param[out]      pOutReverb               初期化された nn::audio::AuxI3dl2ReverbType へのポインタ
 * @param[in]       inBuffer                 エフェクトを施すサンプルデータを保持するバッファ
 * @param[out]      outBuffer                エフェクト処理の結果を出力するバッファ
 * @param[in]       samplesPerFrame          1 オーディオフレームのサンプル数
 * @pre
 * - pOutReverb != nullptr
 * - pOutReverb は初期化されている
 * - inBuffer != nullptr
 * - outBuffer != nullptr
 * @details
 * この関数の呼び出しにより実際にエフェクト処理を実行します。@n
 *
 * inBuffer に指定したバッファの内容は、エフェクト処理を施され、outBuffer に結果が書き出されます。@n
 * inBuffer に含まれるサンプルデータはブロックインターリーブされていることを期待しています。
 * つまり nn::audio::ReadAuxSendBuffer() によって取得したサンプルデータへのバッファを指定してください。@n
 * samplePerFrame には ブロックインターリーブの 1 チャンネル、1 フレーム分に相当するサンプル数を指定します。
 * この数は nn::audio::ReadAuxSendBuffer() で取得した場合は AudioRendererParameter.sampleCount の値に一致します。@n
 *
 * この関数は呼び出したスレッドにより実行されます。@n
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutReverb への操作を行わないでください。
 */
void ProcessAuxI3dl2Reverb(AuxI3dl2ReverbType* pOutReverb, int32_t* outBuffer, const int32_t* inBuffer, const uint32_t samplesPerFrame) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxI3dl2ReverbType のエフェクトの残響音成分を初期化します。
 * @param[in]       pOutReverb               初期化された nn::audio::AuxI3dl2ReverbType へのポインタ
 * @pre
 * - pOutReverb != nullptr
 * - pOutReverb は初期化されている
 * @details
 * この関数の呼び出しは nn::audio::AuxI3dl2ReverbType が管理するワークバッファの状態をクリアし、残響音成分を破棄した後、再初期化します。@n
 * ただし nn::audio::SetAuxI3dl2ReverbParameters() で指定したエフェクトパラメータは保持されます。@n
 * 直前までのエフェクト処理の入力を無効化し、新たにエフェクト処理を開始する場合に呼び出してください。@n
 *
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutReverb への操作を行わないでください。
 */
void ResetAuxI3dl2Reverb(AuxI3dl2ReverbType* pOutReverb) NN_NOEXCEPT;

/**
* @}
*/

/**
* @name        CPU AuxReverb 関連 API
* @{
*/


/**
 * @brief           nn::audio::AuxReverbType の利用に必要なバッファのサイズを返します。
 * @param[in]       sampleRate               nn::audio::AuxReverbType を利用する nn::audio::AuxType に設定されているサンプルレート
 * @param[in]       channelCountMax          nn::audio::AuxReverbType に指定する最大チャンネル数
 * @return          バッファサイズを返します。 [byte]
 * @pre
 * - (sampleRate == 32000) || (sampleRate == 48000)
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 */
size_t GetRequiredBufferSizeForAuxReverb(int sampleRate, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxReverbType を初期化します。
 * @param[out]      pOutReverb               初期化する nn::audio::AuxReverbType へのポインタ
 * @param[in]       buffer                   nn::audio::AuxReverbType で利用するワークバッファ
 * @param[in]       bufferSize               buffer に指定したワークバッファのサイズ [byte]
 * @param[in]       sampleRate               nn::audio::AuxReverbType への入力となるサンプルのサンプルレート
 * @param[in]       channelCountMax          nn::audio::AuxReverbType への入力となるサンプルのチャンネル数
 * @pre
 * - pOutReverb != nullptr
 * - buffer != nullptr
 * - bufferSize >= @ref nn::audio::GetRequiredBufferSizeForAuxReverb "nn::audio::GetRequiredBufferSizeForAuxReverb(sampleRate, channelCountMax)"
 * - (sampleRate == 32000) || (sampleRate == 48000)
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 * @details
 * sampleRate には pOutReverb が行うエフェクト処理の対象となる nn::audio::AuxType のサンプルレートを指定してください。
 * nn::audio::GetAuxSampleRate() で取得可能です。@n
 * channelCountMax は pOutReverb に指定することになる最大入力チャンネル数を指定してくだい。
 */
void InitializeAuxReverb(AuxReverbType* pOutReverb, void* buffer, size_t bufferSize, int sampleRate, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief       AuxReverbType に設定可能なチャンネル数の最大値を取得します。
 * @param[in]   pReverb                初期化された nn::audio::AuxReverbType へのポインタ
 * @return      設定可能なチャンネル数の最大値を返します。
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 */
int GetAuxReverbChannelCountMax(const AuxReverbType* pReverb) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxReverbType の入出力チャンネルインデックスを取得します。
 * @param[out]      outInput                 pReverb に設定されている入力チャンネルのインデックスの取得先となる配列
 * @param[out]      outOutput                pReverb に設定されている出力チャンネルのインデックスの取得先となる配列
 * @param[out]      pOutCount                outInput および outOutput に取得されるチャンネルのインデックスの個数
 * @param[in]       pReverb                  初期化された nn::audio::AuxReverbType へのポインタ
 * @param[in]       count                    outInput および outOutput の配列長
 * @pre
 * - pReverb != nullptr
 * - pReverb は初期化されている
 * - outInput != nullptr
 * - outOutput != nullptr
 * - pOutCount != nullptr
 * - (count == 1) || (count == 2) || (count == 4) || (count == 6)
 * @details
 * nn::audio::SetAuxReverbInputOutput() で指定したチャンネルインデックスの配列を取得します。
 */
void GetAuxReverbInputOutput(int8_t* outInput, int8_t* outOutput, int* pOutCount, const AuxReverbType* pReverb, int count) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxReverbType の入出力チャンネルインデックスを設定します。
 * @param[out]      pOutReverb               初期化された nn::audio::AuxReverbType へのポインタ
 * @param[in]       input                    pOutReverb に設定する入力チャンネルのインデックスの配列
 * @param[in]       output                   pOutReverb に設定する出力チャンネルのインデックスの配列
 * @param[in]       count                    input および output に格納されているチャンネルインデックスの個数
 * @pre
 * - pOutReverb != nullptr
 * - pOutReverb は初期化されている
 * - input != nullptr
 * - output != nullptr
 * - (count == 1) || (count == 2) || (count == 4) || (count == 6)
 * @details
 * nn::audio::ProcessAuxReverb() で inBuffer に指定する入力サンプルは、ブロックインターリーブされていることを期待しています。
 * この関数では nn::audio::ProcessAuxReverb() が行うエフェクト処理を、どのチャンネルに対して実施するかを指定します。
 *
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutReverb への操作を行わないでください。
 */
void SetAuxReverbInputOutput(AuxReverbType* pOutReverb, const int8_t* input, const int8_t* output, int count) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxReverbType のエフェクトパラメータを取得します。
 * @param[in]       pReverb                  初期化された nn::audio::AuxReverbType へのポインタ
 * @return          outParameterSet          取得するエフェクトパラメータ
 * @pre
 * - pOutReverb != nullptr
 * - pOutReverb は初期化されている
 */
ReverbParameterSet GetAuxReverbParameters(const AuxReverbType* pReverb);

/**
 * @brief           nn::audio::AuxReverbType のエフェクトパラメータを設定します。
 * @param[out]      pOutReverb               初期化された nn::audio::AuxReverbType へのポインタ
 * @param[in]       pParameterSet            nn::audio::AuxReverbType に設定するエフェクトパラメータ
 * @pre
 * - pOutReverb != nullptr
 * - pOutReverb は初期化されている
 * - pParameterSet != nullptr
 * - pParameterSet->earlyMode == nn::audio::ReverbType::EarlyMode_SmallRoom ||  pParameterSet->earlyMode == nn::audio::ReverbType::EarlyMode_LargeRoom || pParameterSet->earlyMode == nn::audio::ReverbType::EarlyMode_Hall || pParameterSet->earlyMode == nn::audio::ReverbType::EarlyMode_Cavern || pParameterSet->earlyMode == nn::audio::ReverbType::EarlyMode_NoEarlyReflection
 * - 0.0f <= pParameterSet->earlyGain && pParameterSet->earlyGain <= 1.0f
 * - 0.0f <= pParameterSet->predelayTimeMilliSeconds && pParameterSet->predelayTimeMilliSeconds <= 300.0f
 * - pParameterSet->lateMode == nn::audio::ReverbType::LateMode_Room || pParameterSet->lateMode == nn::audio::ReverbType::LateMode_Hall || pParameterSet->lateMode == nn::audio::ReverbType::LateMode_MetalCorridor || pParameterSet->lateMode == nn::audio::ReverbType::LateMode_Cavern || pParameterSet->lateMode == nn::audio::ReverbType::LateMode_MaximumDelay
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
void SetAuxReverbParameters(AuxReverbType* pOutReverb, const nn::audio::ReverbParameterSet* pParameterSet) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxReverbType のエフェクト処理を実行します。
 * @param[out]      pOutReverb               初期化された nn::audio::AuxReverbType へのポインタ
 * @param[in]       inBuffer                 エフェクトを施すサンプルデータを保持するバッファ
 * @param[out]      outBuffer                エフェクト処理の結果を出力するバッファ
 * @param[in]       samplesPerFrame          1 オーディオフレームのサンプル数
 * @pre
 * - pOutReverb != nullptr
 * - pOutReverb は初期化されている
 * - inBuffer != nullptr
 * - outBuffer != nullptr
 * @details
 * この関数の呼び出しにより実際にエフェクト処理を実行します。@n
 *
 * inBuffer に指定したバッファの内容は、エフェクト処理を施され、outBuffer に結果が書き出されます。@n
 * inBuffer に含まれるサンプルデータはブロックインターリーブされていることを期待しています。
 * つまり nn::audio::ReadAuxSendBuffer() によって取得したサンプルデータへのバッファを指定してください。@n
 * samplePerFrame には ブロックインターリーブの 1 チャンネル、1 フレーム分に相当するサンプル数を指定します。
 * この数は nn::audio::ReadAuxSendBuffer() で取得した場合は AudioRendererParameter.sampleCount の値に一致します。@n
 *
 * この関数は呼び出したスレッドにより実行されます。@n
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutReverb への操作を行わないでください。
 */
void ProcessAuxReverb(AuxReverbType* pOutReverb, int32_t* outBuffer, const int32_t* inBuffer, const uint32_t samplesPerFrame) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxReverbType のエフェクトの残響音成分を初期化します。
 * @param[in]       pOutReverb               初期化された nn::audio::AuxReverbType へのポインタ
 * @pre
 * - pOutReverb != nullptr
 * - pOutReverb は初期化されている
 * @details
 * この関数の呼び出しは nn::audio::AuxReverbType が管理するワークバッファの状態をクリアし、残響音成分を破棄した後、再初期化します。@n
 * ただし nn::audio::SetAuxReverbParameters() で指定したエフェクトパラメータは保持されます。@n
 * 直前までのエフェクト処理の入力を無効化し、新たにエフェクト処理を開始する場合に呼び出してください。@n
 *
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutReverb への操作を行わないでください。
 */
void ResetAuxReverb(AuxReverbType* pOutReverb) NN_NOEXCEPT;

/**
* @name        CPU AuxDelay 関連 API
* @{
*/


/**
 * @brief           nn::audio::AuxDelayType の利用に必要なバッファのサイズを返します。
 * @param[in]       delayTimeMax             nn::audio::AuxDelayType で設定しうる最大ディレイタイム
 * @param[in]       sampleRate               nn::audio::AuxDelayType を利用する nn::audio::AuxType に設定されているサンプルレート
 * @param[in]       channelCountMax          nn::audio::AuxDelayType に指定する最大チャンネル数
 * @return          バッファサイズを返します。 [byte]
 * @pre
 * - (sampleRate == 32000) || (sampleRate == 48000)
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 */
size_t GetRequiredBufferSizeForAuxDelay(nn::TimeSpan delayTimeMax, int sampleRate, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxDelayType を初期化します。
 * @param[out]      pOutDelay               初期化する nn::audio::AuxDelayType へのポインタ
 * @param[in]       buffer                   nn::audio::AuxDelayType で利用するワークバッファ
 * @param[in]       bufferSize               buffer に指定したワークバッファのサイズ [byte]
 * @param[in]       delayTimeMax             nn::audio::AuxDelayType で設定しうる最大ディレイタイム
 * @param[in]       sampleRate               nn::audio::AuxDelayType への入力となるサンプルのサンプルレート
 * @param[in]       channelCountMax          nn::audio::AuxDelayType への入力となるサンプルのチャンネル数
 * @pre
 * - pOutDelay != nullptr
 * - buffer != nullptr
 * - bufferSize >= @ref nn::audio::GetRequiredBufferSizeForAuxDelay "nn::audio::GetRequiredBufferSizeForAuxDelay(delayTimeMax, sampleRate, channelCountMax)"
 * - (sampleRate == 32000) || (sampleRate == 48000)
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 * @details
 * sampleRate には pOutDelay が行うエフェクト処理の対象となる nn::audio::AuxType のサンプルレートを指定してください。
 * nn::audio::GetAuxSampleRate() で取得可能です。@n
 * channelCountMax は pOutDelay に指定することになる最大入力チャンネル数を指定してくだい。
 */
void InitializeAuxDelay(AuxDelayType* pOutDelay, void* buffer, size_t bufferSize, nn::TimeSpan delayTimeMax, int sampleRate, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief       AuxDelayType に設定可能なチャンネル数の最大値を取得します。
 * @param[in]   pDelay                  初期化された nn::audio::AuxDelayType へのポインタ
 * @return      設定可能なチャンネル数の最大値を返します。
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 */
int GetAuxDelayChannelCountMax(const AuxDelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxDelayType の入出力チャンネルインデックスを取得します。
 * @param[out]      outInput                 pDelay に設定されている入力チャンネルのインデックスの取得先となる配列
 * @param[out]      outOutput                pDelay に設定されている出力チャンネルのインデックスの取得先となる配列
 * @param[out]      pOutCount                outInput および outOutput に取得されるチャンネルのインデックスの個数
 * @param[in]       pDelay                   初期化された nn::audio::AuxDelayType へのポインタ
 * @param[in]       count                    outInput および outOutput の配列長
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 * - outInput != nullptr
 * - outOutput != nullptr
 * - pOutCount != nullptr
 * - (count == 1) || (count == 2) || (count == 4) || (count == 6)
 * @details
 * nn::audio::SetAuxDelayInputOutput() で指定したチャンネルインデックスの配列を取得します。
 */
void GetAuxDelayInputOutput(int8_t* outInput, int8_t* outOutput, int* pOutCount, const AuxDelayType* pDelay, int count) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxDelayType の入出力チャンネルインデックスを設定します。
 * @param[out]      pOutDelay                初期化された nn::audio::AuxDelayType へのポインタ
 * @param[in]       input                    pOutDelay に設定する入力チャンネルのインデックスの配列
 * @param[in]       output                   pOutDelay に設定する出力チャンネルのインデックスの配列
 * @param[in]       count                    input および output に格納されているチャンネルインデックスの個数
 * @pre
 * - pOutDelay != nullptr
 * - pOutDelay は初期化されている
 * - input != nullptr
 * - output != nullptr
 * - (count == 1) || (count == 2) || (count == 4) || (count == 6)
 * @details
 * nn::audio::ProcessAuxDelay() で inBuffer に指定する入力サンプルは、ブロックインターリーブされていることを期待しています。
 * この関数では nn::audio::ProcessAuxDelay() が行うエフェクト処理を、どのチャンネルに対して実施するかを指定します。
 *
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutDelay への操作を行わないでください。
 */
void SetAuxDelayInputOutput(AuxDelayType* pOutDelay, const int8_t* input, const int8_t* output, int count) NN_NOEXCEPT;

/**
 * @brief       AuxDelayType に設定可能なディレイタイムの最大値を取得します。
 * @param[in]   pDelay                   初期化された nn::audio::AuxDelayType へのポインタ
 * @return      設定可能なディレイタイムの最大値を返します。
 * @pre
 * - pDelay != nullptr
 * - pDelay は初期化されている
 */
nn::TimeSpan GetAuxDelayTimeMax(const AuxDelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxDelayType のエフェクトパラメータを取得します。
 * @param[in]       pDelay                   初期化された nn::audio::AuxDelayType へのポインタ
 * @return          outParameterSet          取得するエフェクトパラメータ
 * @pre
 * - pOutDelay != nullptr
 * - pOutDelay は初期化されている
 */
DelayParameterSet GetAuxDelayParameters(const AuxDelayType* pDelay) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxDelayType のエフェクトパラメータを設定します。
 * @param[out]      pOutDelay               初期化された nn::audio::AuxDelayType へのポインタ
 * @param[in]       pParameterSet           nn::audio::AuxDelayType に設定するエフェクトパラメータ
 * @pre
 * - pOutDelay != nullptr
 * - pOutDelay は初期化されている
 * - pParameterSet != nullptr
 * - @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(0)" <= pParameterSet->delayTime && pParameterSet->delayTime <= nn::audio::GetAuxDelayTimeMax()
 * - 0.0f <= pParameterSet->inGain && pParameterSet->inGain <= 1.0f
 * - 0.0f <= pParameterSet->feedbackGain && pParameterSet->feedbackGain <= 1.0f
 * - 0.0f <= pParameterSet->dryGain && pParameterSet->dryGain <= 1.0f
 * - 0.0f <= pParameterSet->channelSpread && pParameterSet->channelSpread <= 1.0f
 * - 0.0f <= pParameterSet->lowPassAmount && pParameterSet->lowPassAmount <= 1.0f
 * @details
 * 各パラメータが持つ意味は@confluencelink{93362777, オーディオエフェクトの説明の「ディレイパラメータ」を参照してください。}
 *
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutDelay への操作を行わないでください。
 */
void SetAuxDelayParameters(AuxDelayType* pOutDelay, const DelayParameterSet* pParameterSet) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxDelayType のエフェクト処理を実行します。
 * @param[out]      pOutDelay                初期化された nn::audio::AuxDelayType へのポインタ
 * @param[in]       inBuffer                 エフェクトを施すサンプルデータを保持するバッファ
 * @param[out]      outBuffer                エフェクト処理の結果を出力するバッファ
 * @param[in]       samplesPerFrame          1 オーディオフレームのサンプル数
 * @pre
 * - pOutDelay != nullptr
 * - pOutDelay は初期化されている
 * - inBuffer != nullptr
 * - outBuffer != nullptr
 * @details
 * この関数の呼び出しにより実際にエフェクト処理を実行します。@n
 *
 * inBuffer に指定したバッファの内容は、エフェクト処理を施され、outBuffer に結果が書き出されます。@n
 * inBuffer に含まれるサンプルデータはブロックインターリーブされていることを期待しています。
 * つまり nn::audio::ReadAuxSendBuffer() によって取得したサンプルデータへのバッファを指定してください。@n
 * samplePerFrame には ブロックインターリーブの 1 チャンネル、1 フレーム分に相当するサンプル数を指定します。
 * この数は nn::audio::ReadAuxSendBuffer() で取得した場合は AudioRendererParameter.sampleCount の値に一致します。@n
 *
 * この関数は呼び出したスレッドにより実行されます。@n
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutDelay への操作を行わないでください。
 */
void ProcessAuxDelay(AuxDelayType* pOutDelay, int32_t* outBuffer, const int32_t* inBuffer, const uint32_t samplesPerFrame) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxDelayType のエフェクトの遅延音成分を初期化します。
 * @param[in]       pOutDelay               初期化された nn::audio::AuxDelayType へのポインタ
 * @pre
 * - pOutDelay != nullptr
 * - pOutDelay は初期化されている
 * @details
 * この関数の呼び出しは nn::audio::AuxDelayType が管理するワークバッファの状態をクリアし、遅延音成分を破棄した後、再初期化します。@n
 * ただし nn::audio::SetAuxDelayParameters() で指定したエフェクトパラメータは保持されます。@n
 * 直前までのエフェクト処理の入力を無効化し、新たにエフェクト処理を開始する場合に呼び出してください。@n
 *
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutDelay への操作を行わないでください。
 */
void ResetAuxDelay(AuxDelayType* pOutDelay) NN_NOEXCEPT;

/**
* @}
*/

/**
* @name        CPU AuxLimiter 関連 API
* @{
*/

/**
 * @brief           nn::audio::AuxLimiterType を初期化します。
 * @param[out]      pOutLimiter         初期化する nn::audio::AuxLimiterType へのポインタ
 * @param[in]       buffer              nn::audio::AuxLimiterType で利用するワークバッファ
 * @param[in]       bufferSize          buffer に指定したワークバッファのサイズ [byte]
 * @param[in]       attackTimeMax       nn::audio::AuxLimiterType で設定しうる最大アタックタイム
 * @param[in]       releaseTimeMax      nn::audio::AuxLimiterType で設定しうる最大リリースタイム
 * @param[in]       sampleRate          nn::audio::AuxLimiterType への入力となるサンプルのサンプルレート
 * @param[in]       channelCountMax     nn::audio::AuxLimiterType への入力となるサンプルの最大チャンネル数
 * @pre
 * - pOutLimiter != nullptr
 * - buffer != nullptr
 * - bufferSize >= @ref nn::audio::GetRequiredBufferSizeForAuxLimiter "nn::audio::GetRequiredBufferSizeForAuxLimiter(attackTime, sampleRate, channelCountMax)"
 * - (@ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(0)" < attackTimeMax) && (attackTimeMax <= @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(300)")
 * - (@ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(0)" < releaseTimeMax) && (releaseTimeMax <= @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(300)")
 * - (sampleRate == 32000) || (sampleRate == 48000)
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 * @details
 * sampleRate には pOutLimiter が行うエフェクト処理の対象となる nn::audio::AuxType のサンプルレートを指定してください。
 * nn::audio::GetAuxSampleRate() で取得可能です。@n
 * channelCountMax は pOutLimiter に指定することになる最大入力チャンネル数を指定してくだい。
 */
void InitializeAuxLimiter(AuxLimiterType* pOutLimiter, void* buffer, size_t bufferSize, nn::TimeSpan attackTimeMax, nn::TimeSpan releaseTimeMax, int sampleRate, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxLimiterType の入出力チャンネルインデックスを設定します。
 * @param[out]      pLimiter            初期化された nn::audio::AuxLimiterType へのポインタ
 * @param[in]       input               pOutLimiter に設定する入力チャンネルのインデックスの配列
 * @param[in]       output              pOutLimiter に設定する出力チャンネルのインデックスの配列
 * @param[in]       count               input および output に格納されているチャンネルインデックスの個数
 * @pre
 * - pLimiter != nullptr
 * - pLimiter は初期化されている
 * - input != nullptr
 * - output != nullptr
 * - (count == 1) || (count == 2) || (count == 4) || (count == 6)
 * @details
 * nn::audio::ProcessAuxLimiter() で inBuffer に指定する入力サンプルは、ブロックインターリーブされていることを期待しています。
 * この関数では nn::audio::ProcessAuxLimiter() が行うエフェクト処理を、どのチャンネルに対して実施するかを指定します。
 *
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pLimiter への操作を行わないでください。
 */
void SetAuxLimiterInputOutput(AuxLimiterType* pLimiter, const int8_t* input, const int8_t* output, int count) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxLimiterType の入出力チャンネルインデックスを取得します。
 * @param[out]      outInput            pLimiter に設定されている入力チャンネルのインデックスの取得先となる配列
 * @param[out]      outOutput           pLimiter に設定されている出力チャンネルのインデックスの取得先となる配列
 * @param[out]      pOutCount           outInput および outOutput に取得されるチャンネルのインデックスの個数
 * @param[in]       pLimiter            初期化された nn::audio::AuxLimiterType へのポインタ
 * @param[in]       count               outInput および outOutput の配列長
 * @pre
 * - pLimiter != nullptr
 * - pLimiter は初期化されている
 * - outInput != nullptr
 * - outOutput != nullptr
 * - pOutCount != nullptr
 * - (count == 1) || (count == 2) || (count == 4) || (count == 6)
 * @details
 * nn::audio::SetAuxLimiterInputOutput() で指定したチャンネルインデックスの配列を取得します。
 */
void GetAuxLimiterInputOutput(int8_t* outInput, int8_t* outOutput, int* pOutCount, const AuxLimiterType* pLimiter, int count) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxLimiterType のエフェクトパラメータを取得します。
 * @param[in]       pOutLimiter         初期化された nn::audio::AuxLimiterType へのポインタ
 * @param[in]       pParameterSet       nn::audio::AuxLimiterType に設定するエフェクトパラメータ
 * @pre
 * - pOutLimiter != nullptr
 * - pOutLimiter は初期化されている
 * - pParameterSet != nullptr
 * - @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(0)" < pLimiterParamSet->attackTime && pLimiterParamSet->attackTime <= @ref GetAuxLimiterAttackTimeMax "GetAuxLimiterAttackTimeMax(pOutLimiter)"
 * - @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(0)" < pLimiterParamSet->releaseTime && pLimiterParamSet->releaseTime <= @ref GetAuxLimiterReleaseTimeMax "GetAuxLimiterReleaseTimeMax(pOutLimiter)"
 * - 0 < pLimiterParamSet->threshold <= std::numeric_limits<int32_t>::max()
 */
void SetAuxLimiterParameters(AuxLimiterType* pOutLimiter, const LimiterParameterSet* pParameterSet) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxLimiterType のエフェクトパラメータを取得します。
 * @param[in]       pLimiter            初期化された nn::audio::AuxLimiterType へのポインタ
 * @return          エフェクトパラメータを返します。
 * @pre
 * - pOutLimiter != nullptr
 * - pOutLimiter は初期化されている
 */
LimiterParameterSet GetAuxLimiterParameters(const AuxLimiterType* pLimiter) NN_NOEXCEPT;

/**
 * @brief       AuxLimiterType に設定可能なアタックタイムの最大値を取得します。
 * @param[in]   pLimiter                初期化された nn::audio::AuxLimiterType へのポインタ
 * @return      設定可能なアタックタイムの最大値を返します。
 * @pre
 * - pLimiter != nullptr
 * - pLimiter は初期化されている
 */
nn::TimeSpan GetAuxLimiterAttackTimeMax(const AuxLimiterType* pLimiter) NN_NOEXCEPT;

/**
 * @brief       AuxLimiterType に設定可能なリリースタイムの最大値を取得します。
 * @param[in]   pLimiter                初期化された nn::audio::AuxLimiterType へのポインタ
 * @return      設定可能なリリースタイムの最大値を返します。
 * @pre
 * - pLimiter != nullptr
 * - pLimiter は初期化されている
 */
nn::TimeSpan GetAuxLimiterReleaseTimeMax(const AuxLimiterType* pLimiter) NN_NOEXCEPT;

/**
 * @brief       AuxLimiterType に設定可能なチャンネル数の最大値を取得します。
 * @param[in]   pLimiter                初期化された nn::audio::AuxLimiterType へのポインタ
 * @return      設定可能なチャンネル数の最大値を返します。
 * @pre
 * - pLimiter != nullptr
 * - pLimiter は初期化されている
 */
int GetAuxLimiterChannelCountMax(const AuxLimiterType* pLimiter) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxLimiterType の利用に必要なバッファのサイズを返します。
 * @param[in]       attackTimeMax       nn::audio::AuxLimiterType で設定しうる最大アタックタイム
 * @param[in]       sampleRate          nn::audio::AuxLimiterType を利用する nn::audio::AuxType に設定されているサンプルレート
 * @param[in]       channelCountMax     nn::audio::AuxLimiterType に指定する最大チャンネル数
 * @return          バッファサイズを返します。 [byte]
 * @pre
 * - (@ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(0)" < attackTimeMax) && (attackTimeMax <= @ref nn::TimeSpan::FromMilliSeconds "nn::TimeSpan::FromMilliSeconds(300)")
 * - (sampleRate == 32000) || (sampleRate == 48000)
 * - (channelCountMax == 1) || (channelCountMax == 2) || (channelCountMax == 4) || (channelCountMax == 6)
 */
size_t GetRequiredBufferSizeForAuxLimiter(nn::TimeSpan attackTimeMax, int sampleRate, int channelCountMax) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxLimiterType のエフェクト処理を実行します。
 * @param[out]      pOutLimiter         初期化された nn::audio::AuxLimiterType へのポインタ
 * @param[out]      outBuffer           エフェクト処理の結果を出力するバッファ
 * @param[in]       inBuffer            エフェクトを施すサンプルデータを保持するバッファ
 * @param[in]       samplesPerFrame     1 オーディオフレームのサンプル数
 * @pre
 * - pOutLimiter != nullptr
 * - pOutLimiter は初期化されている
 * - inBuffer != nullptr
 * - outBuffer != nullptr
 * @details
 * この関数の呼び出しにより実際にエフェクト処理を実行します。@n
 *
 * inBuffer に指定したバッファの内容は、エフェクト処理を施され、outBuffer に結果が書き出されます。@n
 * inBuffer に含まれるサンプルデータはブロックインターリーブされていることを期待しています。
 * つまり nn::audio::ReadAuxSendBuffer() によって取得したサンプルデータへのバッファを指定してください。@n
 * samplePerFrame には ブロックインターリーブの 1 チャンネル、1 フレーム分に相当するサンプル数を指定します。
 * この数は nn::audio::ReadAuxSendBuffer() で取得した場合は AudioRendererParameter.sampleCount の値に一致します。@n
 *
 * この関数は呼び出したスレッドにより実行されます。@n
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutLimiter への操作を行わないでください。
 */
void ProcessAuxLimiter(AuxLimiterType* pOutLimiter, int32_t* outBuffer, const int32_t* inBuffer, const uint32_t samplesPerFrame) NN_NOEXCEPT;

/**
 * @brief           nn::audio::AuxLimiterType のエフェクトの内部状態を初期化します。
 * @param[in]       pOutLimiter         初期化された nn::audio::AuxDelayType へのポインタ
 * @pre
 * - pOutLimiter != nullptr
 * - pOutLimiter は初期化されている
 * @details
 * この関数の呼び出しは nn::audio::AuxLimiterType が管理するワークバッファの状態をクリアし、再初期化します。@n
 * ただし nn::audio::SetAuxLimiterParameters() で指定したエフェクトパラメータは保持されます。@n
 * 直前までのエフェクト処理の入力を無効化し、新たにエフェクト処理を開始する場合に呼び出してください。@n
 *
 * この関数はスレッドセーフではありません。この関数の呼び出し中に pOutLimiter への操作を行わないでください。
 */
void ResetAuxLimiter(AuxLimiterType* pOutLimiter) NN_NOEXCEPT;

/**
* @}
*/

}}  // namespace nn::audio
