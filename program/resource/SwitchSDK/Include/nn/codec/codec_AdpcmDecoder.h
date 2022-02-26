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

/**
 * @file
 * @brief       Adpcm データのデコードに関する API を定義します。
 */

#include <nn/nn_Common.h>
#include <nn/codec/codec_AdpcmCommon.h>

namespace nn {
namespace codec {

/**
 * @name        Adpcm
 * @{
 */

/**
 * @brief           ADPCM データサイズからサンプル数を取得します。
 * @param[in]       dataSize               ADPCM データサイズ
 * @return          サンプル数
 * @pre
 * - dataSize >= 0
 */
int GetAdpcmSampleCount(std::size_t dataSize) NN_NOEXCEPT;

/**
 * @brief           サンプル数から ADPCM データサイズを取得します。
 * @param[in]       sampleCount            サンプル数
 * @return          ADPCM データサイズ
 * @pre
 * - sampleCount >= 0
 */
std::size_t GetAdpcmDataSize(int sampleCount) NN_NOEXCEPT;

/**
 * @brief           Adpcm データをデコードします。
 * @param[out]      output                 出力 PCM バッファ
 * @param[in,out]   pContext               コンテキスト情報
 * @param[in]       pParameter             係数パラメータ情報
 * @param[in]       input                  入力 Adpcm バッファ
 * @param[in]       inputSize              入力 Adpcm バッファサイズ
 * @param[in]       sampleCount            出力するサンプル数
 * @pre
 * - pContext != nullptr
 * - pParameter != nullptr
 * - input != nullptr
 * - inputSize >= 0
 * - sampleCount >= 0 && sampleCount <= @ref GetAdpcmSampleCount "GetAdpcmSampleCount(inputSize)"
 * @post
 * - output に sampleCount 個のデコード結果が出力される
 * - pContext が更新される
 */
void DecodeAdpcm(int16_t* output, AdpcmContext* pContext, const AdpcmParameter* pParameter, const void* input, std::size_t inputSize, int sampleCount) NN_NOEXCEPT;

/**
 * @brief           1 フレーム分の Adpcm データの sampleCount 目のデコード開始時のコンテキストを取得します。
 * @param[in,out]   pContext               コンテキスト情報
 * @param[in]       pParameter             係数パラメータ情報
 * @param[in]       input                  入力 Adpcm バッファ
 * @param[in]       inputSize              入力 Adpcm バッファサイズ
 * @param[in]       sampleCount            出力するサンプル数
 * @pre
 * - pContext != nullptr
 * - pParameter != nullptr
 * - input != nullptr
 * - inputSize == nn::codec::AdpcmFrameSize
 * - sampleCount >= 0 && sampleCount <= nn::codec::AdpcmFrameSampleCount
 * @post
 * - pContext が更新される
 */
void GetAdpcmLoopContext(AdpcmContext* pContext, const AdpcmParameter* pParameter, const void* input, std::size_t inputSize, int sampleCount) NN_NOEXCEPT;

/**
 * @}
 */

}  // namespace codec
}  // namespace nn
