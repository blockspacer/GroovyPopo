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

#include <cstddef>

namespace nn { namespace codec {

/**
 * @name        Opus
 * @{
 */

/**
 * @brief       Opus パケットサイズの最大値です。
 */
const std::size_t OpusPacketSizeMaximum = 1508;

/**
 * @brief       Opus でサポートしているストリーム数最大値です。
 */
const int OpusStreamCountMax = 255;

/**
 * @brief       Opus パケット関連の関数が返すエラーコードです。
 * @deprecated  エラーハンドリングを改善した API の公開により削除される予定です。@n
 *
 *   @li nn::codec::GetOpusPacketSampleCountPerFrame(int* pOutSampleCount, const uint8_t pPacket[], size_t packetSize, int sampleRate)
 *   @li nn::codec::GetOpusPacketChannelCount(int* pOutChannelCount, const uint8_t pPacket[], size_t packetSize)
 *   @li nn::codec::GetOpusPacketSampleCountInPacket(int* pOutSampleCount, const uint8_t pPacket[], size_t packetSize, int sampleRate)
 *   @li nn::codec::GetOpusPacketFrameCount(int* pOutFrameCount, const uint8_t pPacket[], size_t packetSize)
 *
 * を使用するように切り替え、本定数は参照しないようにして下さい。
 */
const int OpusPacketErrorCode = -1;

/**
 * @brief       Opus に関する処理結果を表す列挙型です。
 */
enum OpusResult
{
    OpusResult_Success = 0,                 //!< 成功
    OpusResult_InvalidSampleRate = 1,       //!< 無効なサンプルレート
    OpusResult_InvalidChannelCount = 2,     //!< 無効なチャンネル数
    OpusResult_InvalidWorkBuffer = 3,       //!< 無効なワークバッファ
    OpusResult_BrokenData = 4,              //!< データ壊れ
    OpusResult_UnsupportedFormat = 5,       //!< サポートされないフォーマット
    OpusResult_InsufficientOpusBuffer = 6,  //!< Opus データバッファ不足
    OpusResult_InsufficientPcmBuffer = 7,   //!< PCM データバッファ不足
    OpusResult_OutOfResource = 8,           //!< リソース不足
    OpusResult_InternalError = 9,           //!< 内部エラー
    OpusResult_UnsupportedSampleCount = 10, //!< サポートされないサンプル数
    OpusResult_InvalidChannelMapping = 11   //!< 無効なチャンネルマッピング
};

/**
 * @}
 */

}}  // namespace nn::codec
