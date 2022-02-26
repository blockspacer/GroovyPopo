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
* @name        Polyphase Resampler 関連 API
* @{
*
* @brief       Polyphase リサンプラーは、入出力サンプルレートの比の有理数表現の分母および分子が 255 以下の
*              正の整数とすることで効率的に動作します。もし inputSampleRate, outputSampleRate がこの条件を
*              満たさない場合、Polyphase リサンプラーは出力サンプルレートを条件を満たす最も近い値に変換して
*              処理を行います。
*              実際の出力サンプルレートは nn::audio::CalculatePolyphaseResamplerActualOutputSampleRate() を用いて、
*              （PolyphaseResamplerType の初期化前、初期化後問わず）指定した値と実際の値の誤差の有無や、
*              誤差が許容できるかどうかの確認を行うことができます。
*/

/**
* @brief       Polyphase リサンプラーの必要なバッファのサイズを割り出します。
* @param[in]   inputSampleRate              入力サンプルレート
* @param[in]   outputSampleRate             出力サンプルレート
* @param[in]   channelCount                 チャンネル数
* @param[in]   isHighQuality                音質フラグ（true の場合は 32 タップのフィルタ、false の場合は 16 タップのフィルタを内部処理で利用）
* @pre
*              - channelCount > 0
*              - channelCount <= PolyphaseResamplerType::ChannelCountMax
*              - inputSampleRate > 0
*              - outputSampleRate > 0
*              - outputSampleRate / inputSampleRate >= PolyphaseResamplerType::GetConversionRatioMin()
*              - outputSampleRate / inputSampleRate <= PolyphaseResamplerType::GetConversionRatioMax()
* @return      必要なバッファのサイズを返します。
*/
size_t GetRequiredBufferSizeForPolyphaseResampler(int inputSampleRate, int outputSampleRate, int channelCount, bool isHighQuality) NN_NOEXCEPT;

/**
* @brief       Polyphase リサンプラーの実際の出力サンプルレートを計算します。
* @param[in]   inputSampleRate              入力サンプルレート
* @param[in]   outputSampleRate             出力サンプルレート
* @pre
*              - inputSampleRate > 0
*              - outputSampleRate > 0
*              - outputSampleRate / inputSampleRate >= PolyphaseResamplerType::GetConversionRatioMin()
*              - outputSampleRate / inputSampleRate <= PolyphaseResamplerType::GetConversionRatioMax()
* @return      実際の出力サンプルレートを返します。結果は整数値でない可能性があります。
* @details     Polyphase リサンプラーは、入出力サンプルレートの比の有理数表現の分母および分子が 255 以下の
*              正の整数とすることで効率的に動作します。もし inputSampleRate, outputSampleRate がこの条件を
*              満たさない場合、Polyphase リサンプラーは出力サンプルレートを条件を満たす最も近い値に変換して
*              処理を行います。この API は、入出力サンプルレートから、実際の出力サンプルレートを計算します。
*/
double CalculatePolyphaseResamplerActualOutputSampleRate(int inputSampleRate, int outputSampleRate) NN_NOEXCEPT;

/**
* @deprecated  この API は廃止予定です。代わりに nn::audio::GetPolyphaseResamplerOutputSampleCount() を使用してください。
* @brief       Polyphase リサンプラーの出力バッファの必要なサイズの最大値を取得します。
* @param[in]   pResampler                     PolyphaseResamplerType 構造体へのポインタ
* @param[in]   inputBufferSize                入力バッファのサイズ
* @pre
*              - pResampler != nullptr
*              - pResampler は nn::audio::InitializePolyphaseResampler() により初期化済み
* @return      必要な出力バッファサイズの最大値
*/
NN_DEPRECATED size_t GetPolyphaseResampledOutBufferSize(const PolyphaseResamplerType* pResampler, size_t inputBufferSize) NN_NOEXCEPT;

/**
* @brief       Polyphase リサンプラー処理結果得られる出力サンプル数の最大値を、入力サンプル数から計算します。
* @param[in]   pResampler                     PolyphaseResamplerType 構造体へのポインタ
* @param[in]   inputSampleCount               入力サンプル数
* @return      出力サンプル数の最大値を返します。
* @pre
*              - pResampler != nullptr
*              - pResampler は nn::audio::InitializePolyphaseResampler() により初期化済み
* @details
* この API は、出力バッファを確保する際に必要な、出力サンプル数の最大値を返します。
* 内部では一定のマージンを確保するため、この API の戻り値と、実際の処理の結果
* 得られるサンプル数は等しくない可能性があることに注意してください。
* 実際の出力サンプル数は nn::audio::ProcessPolyphaseResamplerBuffer() を実行した際に得られる値を参照してください。
*/
int GetPolyphaseResamplerOutputSampleCount(const PolyphaseResamplerType* pResampler, int inputSampleCount) NN_NOEXCEPT;

/**
* @brief       Polyphase リサンプラーを初期化します。
* @param[out]  pResampler                   PolyphaseResamplerType 構造体へのポインタ
* @param[out]  buffer                       ワークバッファ
* @param[in]   bufferSize                   バッファのサイズ
* @param[in]   inputSampleRate              入力サンプルレート
* @param[in]   outputSampleRate             出力サンプルレート
* @param[in]   channelCount                 チャンネル数
* @param[in]   isHighQuality                音質フラグ（true の場合は 32 タップのフィルタ、false の場合は 16 タップのフィルタを内部処理で利用します）
* @param[in]   gain                         ゲイン（0.0f ~ 1.0f の値、この範囲を超える値を指定した場合は、内部でクランプします）
* @pre
*              - pResampler != nullptr
*              - buffer != nullptr
*              - inputSampleRate > 0
*              - outputSampleRate > 0
*              - bufferSize >= GetRequiredBufferSizeForPolyphaseResampler()
*              - outputSampleRate / inputSampleRate >= PolyphaseResamplerType::GetConversionRatioMin()
*              - outputSampleRate / inputSampleRate <= PolyphaseResamplerType::GetConversionRatioMax()
*              - channelCount > 0
*              - channelCount <= PolyphaseResamplerType::ChannelCountMax
* @post
*              - pResampler が初期化される
*/
Result InitializePolyphaseResampler(PolyphaseResamplerType* pResampler, void* buffer, size_t bufferSize, int inputSampleRate, int outputSampleRate, int channelCount, bool isHighQuality, float gain) NN_NOEXCEPT;

/**
* @brief       Polyphase リサンプラーを利用して、周波数変換します。
* @param[in,out]   pResampler                 PolyphaseResamplerType 構造体へのポインタ
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
*              - pResampler は nn::audio::InitializePolyphaseResampler() により初期化済み
* @post
*              - pOutputBuffer に周波数変換されたデータが *pOutputSampleCount 個書き込まれる
*/
Result ProcessPolyphaseResamplerBuffer(PolyphaseResamplerType* pResampler, int* pOutputSampleCount, int16_t* pOutputBuffer, size_t outputBufferSize, const int16_t* pInputBuffer, int inputSampleCount) NN_NOEXCEPT;

/**
* @}
*/

}  // namespace audio
}  // namespace nn
