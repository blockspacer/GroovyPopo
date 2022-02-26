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

#include <nn/audio/audio_AudioRendererTypes.h>
#include <nn/audio/audio_SinkTypes.h>
#include <nn/audio/audio_FinalMixTypes.h>

namespace nn {
namespace audio {

/**
 * @name        オーディオレンダラ Sink 関連 API
 * @{
 */

/**
 * @brief       オーディオ出力デバイス用の Sink を初期化します。
 * @param[out]  pOutConfig              AudioRendererConfig
 * @param[out]  pOutSink                Sink 構造体へのポインタ
 * @param[in]   pFinalMix               FinalMix 構造体
 * @param[in]   input                   Sink への入力となるミックスバッファのインデックスの配列
 * @param[in]   inputCount              Sink への入力となるミックスバッファのインデックス数
 * @param[in]   name                    オーディオ出力デバイス名
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutConfig と紐づく AudioRenderer が停止状態
 * - pOutConfig と紐づく AudioRenderer の動作モードが nn::audio::AudioRendererExecutionMode_AutoExecution
 * - pOutSink != nullptr
 * - pFinalMix != nullptr
 * - input != nullptr
 * - 0 < inputCount && inputCount <= nn::audio::MixBufferCountMax
 * - inputCount <= @ref nn::audio::GetFinalMixBufferCount "nn::audio::GetFinalMixBufferCount(pFinalMix)"
 * - name は有効なもの
 */
Result AddDeviceSink(AudioRendererConfig* pOutConfig, DeviceSinkType* pOutSink, FinalMixType* pFinalMix, const int8_t* input, int inputCount, const char* name) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力デバイス用の Sink の終了処理を行います。
 * @param[out]  pOutConfig              AudioRendererConfig
 * @param[out]  pSink                   Sink 構造体へのポインタ
 * @param[in]   pFinalMix               FinalMix 構造体
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutConfig と紐づく AudioRenderer が停止状態
 * - pSink != nullptr
 * - pFinalMix != nullptr
 */
void RemoveDeviceSink(AudioRendererConfig* pOutConfig, DeviceSinkType* pSink, FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief           ダウンミックス処理に用いるパラメータの設定します。
 * @param[in]       pSink               パラメータを設定するオーディオ出力デバイス
 * @param[in]       pParameter          設定するパラメータ
 * @pre
 * - pSink != nullptr
 * - pSink は初期化されている。
 * - pParameter != nullptr
 */
void SetDownMixParameter(DeviceSinkType* pSink, const DeviceSinkType::DownMixParameter* pParameter) NN_NOEXCEPT;

/**
 * @brief           ダウンミックス処理に用いるパラメータを取得します。
 * @param[out]      pOutParameter       パラメータの取得先
 * @param[in]       pSink               パラメータを設定するオーディオ出力デバイス
 * @pre
 * - pSink != nullptr
 * - pSink は初期化されている。
 * - pParameter != nullptr
 */
void GetDownMixParameter(DeviceSinkType::DownMixParameter* pOutParameter, const DeviceSinkType* pSink) NN_NOEXCEPT;

/**
 * @brief           アプリケーション独自のダウンミックス処理の有効無効を設定します。
 * @param[in]       pSink               設定するオーディオ出力デバイス
 * @param[in]       enabled             有効無効の設定 true で有効です。
 * @return
 * @pre
 * - pSink != nullptr
 * - pSink は初期化されている。
 */
void SetDownMixParameterEnabled(DeviceSinkType* pSink, bool enabled) NN_NOEXCEPT;

/**
 * @brief           アプリケーション独自のダウンミックス処理の有効無効状態を取得します。
 * @param[in]       pSink               設定するオーディオ出力デバイス
 * @return          有効であれば true を返します。
 * @pre
 * - pSink != nullptr
 * - pSink は初期化されている。
 */
bool IsDownMixParameterEnabled(const DeviceSinkType* pSink) NN_NOEXCEPT;

/**
 * @brief       循環バッファ用の Sink を初期化します。
 * @param[out]  pOutConfig              AudioRendererConfig
 * @param[out]  pOutSink                Sink 構造体へのポインタ
 * @param[in]   pFinalMix               FinalMix 構造体
 * @param[in]   input                   Sink への入力となるミックスバッファのインデックスの配列
 * @param[in]   inputCount              Sink への入力となるミックスバッファのインデックス数
 * @param[in]   buffer                  出力先となるバッファのアドレス
 * @param[in]   size                    buffer に指定したバッファのサイズ
 * @param[in]   sampleFormat            buffer に書き込まれるサンプルデータのサンプルフォーマット
 * @retresult
 *   @handleresult{nn::audio::ResultOutOfResource}
 * @endretresult
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutSink != nullptr
 * - pFinalMix != nullptr
 * - input != nullptr
 * - 0 < inputCount && inputCount <= nn::audio::MixBufferCountMax
 * - inputCount <= @ref nn::audio::GetFinalMixBufferCount "nn::audio::GetFinalMixBufferCount(pFinalMix)"
 * - buffer != nullptr
 * - buffer は nn::audio::BufferAlignSize にアラインしている
 * - sampleFormat == SampleFormat_PcmInt16
 */
Result AddCircularBufferSink(AudioRendererConfig* pOutConfig, CircularBufferSinkType* pOutSink, FinalMixType* pFinalMix, const int8_t* input, int inputCount, void* buffer, size_t size, SampleFormat sampleFormat) NN_NOEXCEPT;

/**
 * @brief       循環バッファ用の Sink の終了処理を行います。
 * @param[out]  pOutConfig              AudioRendererConfig
 * @param[out]  pSink                   Sink 構造体へのポインタ
 * @param[in]   pFinalMix               FinalMix 構造体
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pSink != nullptr
 * - pFinalMix != nullptr
 */
void RemoveCircularBufferSink(AudioRendererConfig* pOutConfig, CircularBufferSinkType* pSink, FinalMixType* pFinalMix) NN_NOEXCEPT;

/**
 * @brief       循環バッファ用 Sink に必要なバッファサイズを取得します。
 * @param[in]   pParameter    循環バッファ用 Sink を追加するオーディオレンダラを取得する際に指定した AudioRendererParameter
 * @param[in]   inputCount    循環バッファ用 Sink への入力となるミックスバッファのインデックス数
 * @param[in]   frameCount    循環バッファ用 Sink で保持するオーディオフレーム数
 * @param[in]   sampleFormat  循環バッファ用 Sink が出力するサンプルフォーマット
 * @return      指定した設定の循環バッファ用 Sink を利用するのに必要なバッファサイズを返します。
 * @pre
 * - pParameter != nullptr
 * - 0 < inputCount && inputCount <= nn::audio::MixBufferCountMax
 * - 0 < frameCount
 * - sampleFormat == SampleFormat_PcmInt16
 */
size_t GetRequiredBufferSizeForCircularBufferSink(const AudioRendererParameter* pParameter, int inputCount, int frameCount, SampleFormat sampleFormat) NN_NOEXCEPT;

/**
 * @brief       循環バッファ用 Sink からサンプルデータを読み込みます。
 * @param[in]   pSink        オーディオレンダラに追加されている pSink
 * @param[out]  pOutBuffer   読み込み先となるバッファ
 * @param[in]   bufferSize   pOutBuffer に指定したバッファのサイズ
 * @return      pOutBuffer へ読み込まれたサンプルデータのサイズを返します。
 * @pre
 * - pSink != nullptr
 * - pSink は初期化されている
 * - pOutBuffer != nullptr
 */
size_t ReadCircularBufferSink(CircularBufferSinkType* pSink, void* pOutBuffer, size_t bufferSize) NN_NOEXCEPT;

/**
 * @brief       DeviceSinkType のオーディオレンダラ上での ID を取得します。
 * @param[in]   pSink                  DeviceSinkType 構造体へのポインタ
 * @return      NodeId を返します。
 * @pre
 * - pSink != nullptr
 * - pSink は初期化されている
 */
NodeId GetSinkNodeId(const DeviceSinkType* pSink) NN_NOEXCEPT;

/**
 * @brief       CircularBufferSinkType のオーディオレンダラ上での ID を取得します。
 * @param[in]   pSink                  CircularBufferSinkType 構造体へのポインタ
 * @return      NodeId を返します。
 * @pre
 * - pSink != nullptr
 * - pSink は初期化されている
 */
NodeId GetSinkNodeId(const CircularBufferSinkType* pSink) NN_NOEXCEPT;

/**
 * @}
 */

}  // namespace audio
}  // namespace nn
