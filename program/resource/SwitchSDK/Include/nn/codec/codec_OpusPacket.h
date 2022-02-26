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
 * @brief       Opus パケットの情報を取得する API を定義します。
 */

#include <nn/nn_Common.h>
#include <nn/codec/codec_OpusCommon.h>

namespace nn {
namespace codec {

/**
 * @name        Opus
 * @{
 */

/**
 * @brief       Opus パケットのサイズを取得します。
 * @param[in]   pPacket       Opus パケット
 * @param[in]   packetSize    pPacketの有効サイズ
 * @return      Opus パケットのサイズをバイト単位で返します。
 * @retval      1以上の値 サイズ
 * @retval      0 エラー値
 *
 * @pre
 * - pPacket != nullptr
 * - packetSize >= 8
 * @details
 * pPacket は Opus パケットの先頭を指している必要があります。
 * 本関数から 0 が返る場合は、
 * pPacket が Opus パケットの先頭を指していないか、
 * 引数が事前条件を満たしていない可能性があります。
 */
size_t GetOpusPacketSize(const uint8_t pPacket[], size_t packetSize) NN_NOEXCEPT;

/**
 * @brief       この関数は非推奨です。Opus パケットに含まれるフレーム当たりのサンプル数を取得します。
 * @deprecated  エラーハンドリングを改善した API を公開しました。 nn::codec::GetOpusPacketSampleCountPerFrame(int* pOutSampleCount, const uint8_t pPacket[], size_t packetSize, int sampleRate) を使用するように切り替えて下さい。
 *
 * @param[in]   pPacket       Opus パケット
 * @param[in]   packetSize    Opus パケットのサイズ
 * @param[in]   sampleRate    サンプルレート
 * @return      フレーム当たりのサンプル数を返します。
 * @retval      0以上の値 フレーム当たりのサンプル数
 * @retval      nn::codec::OpusPacketErrorCode エラー値
 *
 * @pre
 * - pPacket != nullptr
 * - packetSize >= 9
 * - sampleRate == 48000 || sampleRate == 24000 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 8000
 * @details
 * pPacket は Opus パケットの先頭を指している必要があります。
 * 本関数から nn::codec::OpusPacketErrorCode が返る場合は、
 * pPacket が Opus パケットの先頭を指していないか、
 * 引数が事前条件を満たしていない可能性があります。
 */
NN_DEPRECATED int GetOpusPacketSampleCountPerFrame(const uint8_t pPacket[], size_t packetSize, int sampleRate) NN_NOEXCEPT;

/**
 * @brief       Opus パケットに含まれるフレーム当たりのサンプル数を取得します。
 * @param[out]  pOutSampleCount  サンプル数格納先
 * @param[in]   pPacket       Opus パケット
 * @param[in]   packetSize    Opus パケットのサイズ
 * @param[in]   sampleRate    サンプルレート
 * @return      処理の結果を返します。
 * @retval      nn::codec::OpusResult_Success 成功
 *
 * @pre
 * - pOutSampleCount != nullptr
 * - pPacket != nullptr
 * - pPacket が Opus パケットの先頭を指している
 * - packetSize >= 9
 * - sampleRate == 48000 || sampleRate == 24000 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 8000
 */
OpusResult GetOpusPacketSampleCountPerFrame(int* pOutSampleCount, const uint8_t pPacket[], size_t packetSize, int sampleRate) NN_NOEXCEPT;

/**
 * @brief       この関数は非推奨です。Opus パケットに含まれるフレームのチャンネル数を取得します。
 * @deprecated  エラーハンドリングを改善した API を公開しました。 nn::codec::GetOpusPacketChannelCount(int* pOutChannelCount, const uint8_t pPacket[], size_t packetSize) を使用するように切り替えて下さい。
 *
 * @param[in]   pPacket       Opus パケット
 * @param[in]   packetSize    Opus パケットのサイズ
 * @return      フレームのチャンネル数を返します。
 * @retval      0以上の値 フレーム当たりのサンプル数
 * @retval      nn::codec::OpusPacketErrorCode エラー値
 *
 * @pre
 * - pPacket != nullptr
 * - packetSize >= 9
 * @details
 * pPacket は Opus パケットの先頭を指している必要があります。
 * 本関数から nn::codec::OpusPacketErrorCode が返る場合は、
 * pPacket が Opus パケットの先頭を指していないか、
 * 引数が事前条件を満たしていない可能性があります。
 */
NN_DEPRECATED int GetOpusPacketChannelCount(const uint8_t pPacket[], size_t packetSize) NN_NOEXCEPT;

/**
 * @brief       Opus パケットに含まれるフレームのチャンネル数を取得します。
 * @param[out]  pOutChannelCount チャンネル数格納先
 * @param[in]   pPacket       Opus パケット
 * @param[in]   packetSize    Opus パケットのサイズ
 * @return      処理の結果を返します。
 * @retval      nn::codec::OpusResult_Success 成功
 *
 * @pre
 * - pOutChannelCount != nullptr
 * - pPacket != nullptr
 * - pPacket が Opus パケットの先頭を指している
 * - packetSize >= 9
 */
OpusResult GetOpusPacketChannelCount(int* pOutChannelCount, const uint8_t pPacket[], size_t packetSize) NN_NOEXCEPT;

/**
 * @brief       この関数は非推奨です。Opus パケットに含まれるサンプル数を取得します。
 * @deprecated  エラーハンドリングを改善した API を公開しました。 nn::codec::GetOpusPacketSampleCountInPacket(int* pOutSampleCount, const uint8_t pPacket[], size_t packetSize, int sampleRate) を使用するように切り替えて下さい。
 *
 * @param[in]   pPacket       Opus パケット
 * @param[in]   packetSize    Opus パケットのサイズ
 * @param[in]   sampleRate    サンプルレート
 * @return      パケットに含まれるサンプル数を返します。
 * @retval      0以上の値 パケットに含まれるサンプル数
 * @retval      nn::codec::OpusPacketErrorCode エラー値
 *
 * @pre
 * - pPacket != nullptr
 * - packetSize >= 9
 * - sampleRate == 48000 || sampleRate == 24000 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 8000
 * @details
 * pPacket は Opus パケットの先頭を指している必要があります。
 * 本関数が返す値は、
 * nn::codec::GetOpusPacketSampleCountPerFrame() と nn::codec::GetOpusPacketFrameCount() の積と等しくなります。@n
 * 本関数から nn::codec::OpusPacketErrorCode が返る場合は、
 * pPacket が Opus パケットの先頭を指していないか、
 * 引数が事前条件を満たしていない可能性があります。
 */
NN_DEPRECATED int GetOpusPacketSampleCountInPacket(const uint8_t pPacket[], size_t packetSize, int sampleRate) NN_NOEXCEPT;

/**
 * @brief       Opus パケットに含まれるサンプル数を取得します。
 * @param[out]  pOutSampleCount  サンプル数格納先
 * @param[in]   pPacket       Opus パケット
 * @param[in]   packetSize    Opus パケットのサイズ
 * @param[in]   sampleRate    サンプルレート
 * @retval      nn::codec::OpusResult_Success 成功
 * @retval      nn::codec::OpusResult_BrokenData データ壊れ
 *
 * @pre
 * - pOutSampleCount != nullptr
 * - pPacket != nullptr
 * - pPacket が Opus パケットの先頭を指している
 * - packetSize >= 9
 * - sampleRate == 48000 || sampleRate == 24000 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 8000
 * @details
 * 本関数が返す値は、
 * nn::codec::GetOpusPacketSampleCountPerFrame(int* pOutSampleCount, const uint8_t pPacket[], size_t packetSize, int sampleRate) と
 * nn::codec::GetOpusPacketFrameCount(int* pOutFrameCount, const uint8_t pPacket[], size_t packetSize) から取得できる値の積と等しくなります。@n
 * 本関数から nn::codec::OpusResult_BrokenData が返る場合、pPacket が Opus パケットの先頭を指していない可能性があります。
 */
OpusResult GetOpusPacketSampleCountInPacket(int* pOutSampleCount, const uint8_t pPacket[], size_t packetSize, int sampleRate) NN_NOEXCEPT;

/**
 * @brief       この関数は非推奨です。Opus パケットに含まれるフレーム数を取得します。
 * @deprecated  エラーハンドリングを改善した API を公開しました。 nn::codec::GetOpusPacketFrameCount(int* pOutFrameCount, const uint8_t pPacket[], size_t packetSize) を使用するように切り替えて下さい。
 *
 * @param[in]   pPacket       Opus パケット
 * @param[in]   packetSize    Opus パケットのサイズ
 * @return      パケットに含まれるフレーム数を返します。
 * @retval      0以上の値 フレーム数
 * @retval      nn::codec::OpusPacketErrorCode エラー値
 *
 * @pre
 * - pPacket != nullptr
 * - packetSize >= 9
 * @details
 * pPacket は Opus パケットの先頭を指している必要があります。
 * 本関数から nn::codec::OpusPacketErrorCode が返る場合は、
 * pPacket が Opus パケットの先頭を指していないか、
 * 引数が事前条件を満たしていない可能性があります。
 */
NN_DEPRECATED int GetOpusPacketFrameCount(const uint8_t pPacket[], size_t packetSize) NN_NOEXCEPT;

/**
 * @brief       Opus パケットに含まれるフレーム数を取得します。
 * @param[out]  pOutFrameCount   フレーム数格納先
 * @param[in]   pPacket       Opus パケット
 * @param[in]   packetSize    Opus パケットのサイズ
 * @return      処理の結果を返します。
 * @retval      nn::codec::OpusResult_Success 成功
 * @retval      nn::codec::OpusResult_BrokenData データ壊れ
 *
 * @pre
 * - pOutFrameCount != nullptr
 * - pPacket != nullptr
 * - pPacket が Opus パケットの先頭を指している
 * - packetSize >= 9
 * @details
 * 本関数から nn::codec::OpusResult_BrokenData が返る場合、pPacket が Opus パケットの先頭を指していない可能性があります。
 */
OpusResult GetOpusPacketFrameCount(int* pOutFrameCount, const uint8_t pPacket[], size_t packetSize) NN_NOEXCEPT;

/**
 * @}
 */


}}  // nn::codec
