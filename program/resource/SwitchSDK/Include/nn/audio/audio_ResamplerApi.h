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
#include <nn/audio/audio_ResamplerTypes.h>

namespace nn {
namespace audio {

/**
* @name        Resampler 関連 API
* @{
*/

/**
* @brief       リサンプラーの必要なバッファのサイズを割り出します。
* @param[in]   channelCount                 チャンネル数
* @pre
*              - channelCount > 0
*              - channelCount <= ResamplerType::ChannelCountMax
* @return      必要なバッファのサイズを返します。
*/
size_t GetRequiredBufferSizeForResampler(int channelCount) NN_NOEXCEPT;

/**
* @deprecated  この API は廃止予定です。代わりに nn::audio::GetResamplerOutputSampleCount() を使用してください。
* @brief       リサンプラーの出力バッファの必要なサイズの最大値を取得します。
* @param[in]   pResampler                     ResamplerType 構造体へのポインタ
* @param[in]   inputBufferSize                入力バッファのサイズ
* @pre
*              - pResampler != nullptr
*              - pResampler は nn::audio::InitializeResampler() により初期化済み
* @return      必要な出力バッファサイズの最大値
*/
NN_DEPRECATED size_t GetResampledOutBufferSize(const ResamplerType* pResampler, size_t inputBufferSize) NN_NOEXCEPT;

/**
* @brief       リサンプラー処理結果得られる出力サンプル数の最大値を、入力サンプル数から計算します。
* @param[in]   pResampler                     ResamplerType 構造体へのポインタ
* @param[in]   inputSampleCount               入力サンプル数
* @return      出力サンプル数の最大値を返します。
* @pre
*              - pResampler != nullptr
*              - pResampler は nn::audio::InitializeResampler() により初期化済み
* @details
* この API は、出力バッファを確保する際に必要な、出力サンプル数の最大値を返します。
* 内部では一定のマージンを確保するため、この API の戻り値と、実際の処理の結果
* 得られるサンプル数は等しくない可能性があることに注意してください。
* 実際の出力サンプル数は nn::audio::ProcessResamplerBuffer() を実行した際に得られる値を参照してください。
*/
int GetResamplerOutputSampleCount(const ResamplerType* pResampler, int inputSampleCount) NN_NOEXCEPT;

/**
* @brief       リサンプラーを初期化します。
* @param[out]  pResampler                   ResamplerType 構造体へのポインタ
* @param[out]  buffer                       ワークバッファ
* @param[in]   bufferSize                   バッファのサイズ
* @param[in]   inputSampleRate              入力サンプルレート
* @param[in]   outputSampleRate             出力サンプルレート
* @param[in]   channelCount                 チャンネル数
* @pre
*              - pResampler != nullptr
*              - buffer != nullptr
*              - inputSampleRate > 0
*              - outputSampleRate > 0
*              - bufferSize >= @ref nn::audio::GetRequiredBufferSizeForResampler "nn::audio::GetRequiredBufferSizeForResampler(channelCount)"
*              - outputSampleRate / inputSampleRate >= ResamplerType::GetConvertRatioMin()
*              - outputSampleRate / inputSampleRate <= ResamplerType::GetConvertRatioMax()
* @post
*              - pResampler が初期化される
*/
Result InitializeResampler(ResamplerType* pResampler, void* buffer, size_t bufferSize, int inputSampleRate, int outputSampleRate, int channelCount) NN_NOEXCEPT;

/**
* @brief       リサンプラーを利用して、周波数変換します。
* @param[in,out]   pResampler                     ResamplerType 構造体へのポインタ
* @param[out]  pOutputSampleCount             出力サンプル数
* @param[out]  pOutputBuffer                  出力バッファ
* @param[in]   outputBufferSize               出力バッファのサイズ
* @param[in]   pInputBuffer                   入力バッファ
* @param[in]   inputSampleCount               入力サンプル数
* @retresult
*   @handleresult{nn::audio::ResultInsufficientBuffer}
* @endretresult
* @pre
*              - pOutputBuffer != nullptr
*              - pInputBuffer != nullptr
*              - pResampler != nullptr
*              - pResampler は nn::audio::InitializeResampler() により初期化済み
* @post
*              - pOutputBuffer に周波数変換されたデータが *pOutputSampleCount 個書き込まれる
*/
Result ProcessResamplerBuffer(ResamplerType* pResampler, int* pOutputSampleCount, int16_t* pOutputBuffer, size_t outputBufferSize, const int16_t* pInputBuffer, int inputSampleCount) NN_NOEXCEPT;

/**
* @}
*/

}  // namespace audio
}  // namespace nn
