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

#include <nn/nn_Common.h>
#include <nn/codec/codec_OpusCommonTypes.h>
#include <nn/codec/codec_OpusDecoderTypes.h>

namespace nn { namespace codec {

/**
 * @name        Opus
 * @{
 */

/**
 * @brief       OpusDecoder に必要なワークバッファのサイズを取得します。
 * @param[in]   sampleRate              サンプルレート
 * @param[in]   channelCount            チャンネル数
 * @return      ワークバッファのサイズを返します。
 * @pre
 * - sampleRate == 48000 || sampleRate == 24000 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 8000
 * - channelCount >= 1 && channelCount <= 2
 */
size_t GetOpusDecoderWorkBufferSize(int sampleRate, int channelCount) NN_NOEXCEPT;

/**
 * @brief       OpusDecoder インスタンスを初期化します。
 * @param[out]  pOutDecoder             OpusDecoder 構造体
 * @param[in]   sampleRate              サンプルレート
 * @param[in]   channelCount            チャンネル数
 * @param[in]   buffer                  ワークバッファ
 * @param[in]   size                    ワークバッファのサイズ
 * @return      処理の結果を返します。
 * @retval      OpusResult_Success 成功
 * @retval      OpusResult_InvalidSampleRate 無効なサンプルレート
 * @retval      OpusResult_InvalidChannelCount 無効なチャンネル数
 * @retval      OpusResult_InvalidWorkBuffer 無効なワークバッファ
 *
 * @pre
 * - pOutDecoder != nullptr
 * - sampleRate == 48000 || sampleRate == 24000 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 8000
 * - channelCount >= 1 && channelCount <= 2
 * - buffer != nullptr
 * - size >= GetOpusDecoderWorkBufferSize(sampleRate, channelCount)
 */
OpusResult InitializeOpusDecoder(OpusDecoderType* pOutDecoder, int sampleRate, int channelCount, void* buffer, size_t size) NN_NOEXCEPT;

/**
 * @brief       OpusDecoder インスタンスを終了します。
 * @param[in]   pDecoder                OpusDecoder 構造体
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 */
void FinalizeOpusDecoder(OpusDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @brief       OpusDecoder インスタンスに指定されたサンプルレートを取得します。
 * @param[in]   pDecoder                OpusDecoder 構造体
 * @return      サンプルレートを返します。
 * @pre
 * - pDecoder は初期化済み
 */
int GetOpusDecoderSampleRate(const OpusDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @brief       OpusDecoder インスタンスに指定されたチャンネル数を取得します。
 * @param[in]   pDecoder                OpusDecoder 構造体
 * @return      チャンネル数を返します。
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 */
int GetOpusDecoderChannelCount(const OpusDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @brief       1 フレーム分の Opus データをデコードして、インターリーブされた PCM データを取得します。
 * @param[in]   pDecoder                OpusDecoder 構造体
 * @param[out]  pOutConsumed            デコード時に使用されたデータのサイズ
 * @param[out]  pOutSampleCount         出力サンプル数（1 チャンネルあたり）
 * @param[out]  outputBuffer            出力 PCM バッファ
 * @param[in]   outputSize              outputBuffer のサイズ
 * @param[in]   inputBuffer             入力データバッファ
 * @param[in]   inputSize               inputBuffer のサイズ
 * @return      処理の結果を返します。
 * @retval      OpusResult_Success 成功
 * @retval      OpusResult_UnsupportedFormat サポートされないフォーマット
 * @retval      OpusResult_InsufficientOpusBuffer Opus データバッファ不足
 * @retval      OpusResult_InsufficientPcmBuffer PCM データバッファ不足
 * @retval      OpusResult_BrokenData データ壊れ
 * @retval      OpusResult_InternalError Opus の内部エラー
 *
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 * - pOutConsumed != nullptr
 * - pOutSampleCount != nullptr
 * - outputBuffer != nullptr
 * - inputBuffer != nullptr
 * @details
 * 入力データのチャンネル数が 2 の場合は outputBuffer に L, R, L, R, ... の順に出力データを書き込みます。
 * 処理に失敗した場合であっても outputBuffer の内容が改変されることがあります。
 */
OpusResult DecodeOpusInterleaved(OpusDecoderType* pDecoder, size_t* pOutConsumed, int* pOutSampleCount, int16_t* outputBuffer, size_t outputSize, const void* inputBuffer, size_t inputSize) NN_NOEXCEPT;

/**
 * @brief       OpusDecoder インスタンスを InitializeOpusDecoder() を呼び出した直後の状態にリセットします。
 * @param[in]   pDecoder                OpusDecoder 構造体
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 */
void ResetOpusDecoder(OpusDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @}
 */

}}  // namespace nn::codec
