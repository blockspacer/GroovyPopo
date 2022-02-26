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

#include <nn/codec/codec_AacCommonTypes.h>
#include <nn/codec/codec_AacDecoderTypes.h>

namespace nn { namespace codec {

/**
 * @name        Aac
 * @{
 */

/**
 * @brief       指定したパラメータに対応する AacDecoder 1 インスタンスに必要なワークバッファのサイズを取得します。
 * @param[in]   sampleRate              サンプルレート
 * @param[in]   channelCount            チャンネル数
 * @param[in]   type                    Mpeg-4 Audio Object Type
 * @return      ワークバッファのサイズを返します。
 * @pre
 * - sampleRate == 96000 || sampleRate == 88200 || sampleRate == 64000 || sampleRate == 48000 || sampleRate == 44100 || sampleRate == 32000 || sampleRate == 24000 || sampleRate == 22050 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 11025 || sampleRate == 8000
 * - channelCount >= 1 && channelCount <= 6
 */
size_t GetAacDecoderWorkBufferSize(int sampleRate, int channelCount, Mpeg4AudioObjectType type) NN_NOEXCEPT;

/**
 * @brief       AacDecoder インスタンスを取得、初期化します。
 * @param[out]  pOutDecoder             AacDecoderType 構造体
 * @param[in]   sampleRate              サンプルレート
 * @param[in]   channelCount            チャンネル数
 * @param[in]   type                    Mpeg-4 Audio Object Type
 * @param[in]   buffer                  ワークバッファ
 * @param[in]   size                    ワークバッファのサイズ
 * @return      処理の結果を返します。
 * @pre
 * - pOutDecoder != nullptr
 * - sampleRate == 96000 || sampleRate == 88200 || sampleRate == 64000 || sampleRate == 48000 || sampleRate == 44100 || sampleRate == 32000 || sampleRate == 24000 || sampleRate == 22050 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 11025 || sampleRate == 8000
 * - channelCount >= 1 && channelCount <= 6
 * - buffer != nullptr
 * - size >= @ref GetAacDecoderWorkBufferSize "GetAacDecoderWorkBufferSize(sampleRate, channelCount, type)"
 */
AacDecoderResult InitializeAacDecoder(AacDecoderType* pOutDecoder, int sampleRate, int channelCount, Mpeg4AudioObjectType type, void* buffer, size_t size) NN_NOEXCEPT;

/**
 * @brief       AacDecoder インスタンスを終了、破棄します。
 * @param[in]   pDecoder                AacDecoderType 構造体
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 */
void FinalizeAacDecoder(AacDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @brief       AacDecoder インスタンスに指定されたサンプルレートを取得します。
 * @param[in]   pDecoder                AacDecoderType 構造体
 * @return      サンプルレートを返します。
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 */
int GetAacDecoderSampleRate(const AacDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @brief       AacDecoder インスタンスに指定されたチャンネル数を取得します。
 * @param[in]   pDecoder                AacDecoderType 構造体
 * @return      チャンネル数を返します。
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 */
int GetAacDecoderChannelCount(const AacDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @brief       AacDecoder インスタンスに指定された Mpeg-4 Audio Object Type を取得します。
 * @param[in]   pDecoder                AacDecoderType 構造体
 * @return      Mpeg-4 Audio Object Type を返します。
 * @pre
 * - pDecoder != nullptr
 * - *pDecoder は初期化済み
 */
Mpeg4AudioObjectType GetAacDecoderMpeg4AudioObjectType(const AacDecoderType* pDecoder) NN_NOEXCEPT;

/**
 * @brief       AacDecoder インスタンスによりデコード処理を行います。
 * @param[in]   pDecoder                AacDecoderType 構造体
 * @param[out]  pOutConsumed            デコード時に使用されたデータのサイズ
 * @param[out]  pOutSampleCount         出力サンプル数（1 チャンネルあたり）
 * @param[out]  outputBuffer            出力 PCM バッファ
 * @param[in]   outputSize              outputBuffer のサイズ
 * @param[in]   inputBuffer             入力データバッファ
 * @param[in]   inputSize               inputBuffer のサイズ
 * @return      処理の結果を返します。
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
AacDecoderResult DecodeAacInterleaved(AacDecoderType* pDecoder, size_t* pOutConsumed, int* pOutSampleCount, int16_t* outputBuffer, size_t outputSize, const void* inputBuffer, size_t inputSize) NN_NOEXCEPT;

/**
 * @}
 */

}}  // namespace nn::codec
