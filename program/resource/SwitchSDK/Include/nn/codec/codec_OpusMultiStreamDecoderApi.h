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

#include <nn/nn_Macro.h>
#include <nn/codec/codec_OpusCommon.h>
#include <nn/codec/codec_OpusMultiStreamDecoderTypes.h>
#include <nn/codec/codec_IOpusMultiStreamDecoder.h>

namespace nn { namespace codec {

/**
 * @name        Opus
 * @{
 */

/**
 * @brief       OpusMultiStreamDecoder に必要なワークバッファのサイズを取得します。
 * @param[in]   sampleRate          サンプルレート
 * @param[in]   channelCount        チャンネル数
 * @param[in]   totalStreamCount    合計ストリーム数
 * @param[in]   stereoStreamCount   ステレオストリーム数
 * @return      ワークバッファのサイズを返します。
 * @pre
 * - sampleRate == 48000 || sampleRate == 24000 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 8000
 * - channelCount >= 1 && channelCount <= OpusStreamCountMax
 * - totalStreamCount + stereoStreamCount <= channelCount
 * - totalStreamCount >= 1 && totalStreamCount <= OpusStreamCountMax
 * - stereoStreamCount >= 0 && stereoStreamCount <= totalStreamCount
 *
 */
size_t GetOpusMultiStreamDecoderWorkBufferSize(int sampleRate, int channelCount, int totalStreamCount, int stereoStreamCount) NN_NOEXCEPT;

/**
 * @brief       OpusMultiStreamDecoder インスタンスを初期化します。
 * @param[out]  pOutDecoder             OpusMultiStreamDecoder 構造体
 * @param[in]   sampleRate              サンプルレート
 * @param[in]   channelCount            チャンネル数
 * @param[in]   totalStreamCount        合計ストリーム数
 * @param[in]   stereoStreamCount       ステレオストリーム数
 * @param[in]   channelMapping          チャンネルマッピング
 * @param[in]   buffer                  ワークバッファ
 * @param[in]   size                    ワークバッファのサイズ
 * @return      処理の結果を返します。
 * @retval      OpusResult_Success 成功
 * @retval      OpusResult_InvalidSampleRate 無効なサンプルレート
 * @retval      OpusResult_InvalidChannelCount 無効なチャンネル数
 * @retval      OpusResult_InvalidWorkBuffer 無効なワークバッファ
 * @retval      OpusResult_InvalidStreamCount 無効なストリーム数
 * @retval      OpusResult_InvalidChannelMapping 不正なチャンネルマッピング入力
 *
 * @pre
 * - pOutDecoder != nullptr
 * - sampleRate == 48000 || sampleRate == 24000 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 8000
 * - channelCount >= 1 && channelCount <= OpusStreamCountMax
 * - totalStreamCount + stereoStreamCount <= channelCount
 * - totalStreamCount >= 1 && totalStreamCount <= OpusStreamCountMax
 * - stereoStreamCount >= 0 && stereoStreamCount <= totalStreamCount
 * - channelMapping != nullptr
 * - sizeof(channelMapping) >= channelCount
 * - buffer != nullptr
 * - size >= @ref GetOpusMultiStreamDecoderWorkBufferSize "GetOpusMultiStreamDecoderWorkBufferSize(sampleRate, channelCount, totalStreamCount, stereoStreamCount)"
 */
OpusResult InitializeOpusMultiStreamDecoder(OpusMultiStreamDecoderType* pOutDecoder, int sampleRate, int channelCount, int totalStreamCount, int stereoStreamCount, const uint8_t channelMapping[], void* buffer, size_t size) NN_NOEXCEPT;

/**
 * @brief       OpusMultiStreamDecoder インスタンスを終了します。
 * @param[in]   pDecoder                OpusMultiStreamDecoder 構造体
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 */
void FinalizeOpusMultiStreamDecoder(OpusMultiStreamDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @brief       OpusMultiStreamDecoder に指定されたサンプルレートを取得します。
 * @param[in]   pDecoder                OpusMultiStreamDecoder 構造体
 * @return      サンプルレートを返します。
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 */
int GetOpusMultiStreamDecoderSampleRate(const OpusMultiStreamDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @brief       OpusMultiStreamDecoder に指定されたチャンネル数を取得します。
 * @param[in]   pDecoder                OpusMultiStreamDecoder 構造体
 * @return      チャンネル数を返します。
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 */
int GetOpusMultiStreamDecoderChannelCount(const OpusMultiStreamDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @brief       OpusMultiStreamDecoder に指定された合計ストリーム数を取得します。
 * @param[in]   pDecoder                OpusMultiStreamDecoder 構造体
 * @return      合計ストリーム数を返します。
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 */
int GetOpusMultiStreamDecoderTotalStreamCount(const OpusMultiStreamDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @brief       OpusMultiStreamDecoder に指定されたステレオストリーム数を取得します。
 * @param[in]   pDecoder                OpusMultiStreamDecoder 構造体
 * @return      ステレオストリーム数を返します。
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 */
int GetOpusMultiStreamDecoderStereoStreamCount(const OpusMultiStreamDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @brief       1 フレーム分の OpusMultiStream データをデコードして、インターリーブされた PCM データを取得します。
 * @param[in]   pDecoder                OpusMultiStreamDecoder 構造体
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
OpusResult DecodeOpusMultiStreamInterleaved(OpusMultiStreamDecoderType* pDecoder, size_t* pOutConsumed, int* pOutSampleCount, int16_t* outputBuffer, size_t outputSize, const void* inputBuffer, size_t inputSize) NN_NOEXCEPT;

/**
 * @brief       OpusMultiStreamDecoder インスタンスを ResetOpusMultiStreamDecoder() を呼び出した直後の状態にリセットします。
 * @param[in]   pDecoder                OpusMultiStreamDecoder 構造体
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 */
void ResetOpusMultiStreamDecoder(OpusMultiStreamDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @}
 */

}}  // namespace nn::codec
