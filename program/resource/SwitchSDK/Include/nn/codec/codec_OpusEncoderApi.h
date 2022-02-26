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
#include <nn/codec/codec_OpusEncoderTypes.h>

namespace nn { namespace codec {

/**
 * @name        Opus
 * @{
 */

/**
 * @brief       OpusEncoder に必要なワークバッファのサイズを取得します。
 * @param[in]   sampleRate              サンプルレート
 * @param[in]   channelCount            チャンネル数
 * @return      ワークバッファのサイズを返します。
 * @pre
 * - sampleRate == 48000 || sampleRate == 24000 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 8000
 * - channelCount >= 1 && channelCount <= 2
 */
size_t GetOpusEncoderWorkBufferSize(int sampleRate, int channelCount) NN_NOEXCEPT;

/**
 * @brief       OpusEncoder インスタンスを初期化します。
 * @param[out]  pOutEncoder             OpusEncoderType 構造体
 * @param[in]   sampleRate              サンプルレート
 * @param[in]   channelCount            チャンネル数
 * @param[in]   buffer                  ワークバッファ
 * @param[in]   size                    ワークバッファのサイズ
 * @return      処理の結果を返します。
 * @retval      OpusResult_Success 成功
 * @retval      OpusResult_InvalidSampleRate 無効なサンプルレート
 * @retval      OpusResult_InvalidChannelCount 無効なチャンネル数
 * @retval      OpusResult_InvalidWorkBuffer 無効なワークバッファ
 * @retval      OpusResult_InternalError 内部エラー
 *
 * @pre
 * - pOutEncoder != nullptr
 * - sampleRate == 48000 || sampleRate == 24000 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 8000
 * - channelCount >= 1 && channelCount <= 2
 * - buffer != nullptr
 * - size >= @ref nn::codec::GetOpusEncoderWorkBufferSize "nn::codec::GetOpusEncoderWorkBufferSize(sampleRate, channelCount)"
 */
OpusResult InitializeOpusEncoder(OpusEncoderType* pOutEncoder, int sampleRate, int channelCount, void* buffer, size_t size) NN_NOEXCEPT;

/**
 * @brief       OpusEncoder インスタンスを終了します。
 * @param[in]   pEncoder                OpusEncoderType 構造体
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 */
void FinalizeOpusEncoder(OpusEncoderType* pEncoder) NN_NOEXCEPT;

/**
 * @brief       OpusEncoder インスタンスに指定されたサンプルレートを取得します。
 * @param[in]   pEncoder                OpusEncoderType 構造体
 * @return      サンプルレートを返します。
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 */
int GetOpusEncoderSampleRate(const OpusEncoderType* pEncoder) NN_NOEXCEPT;

/**
 * @brief       OpusEncoder インスタンスに指定されたチャンネル数を取得します。
 * @param[in]   pEncoder                OpusEncoderType 構造体
 * @return      チャンネル数を返します。
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 */
int GetOpusEncoderChannelCount(const OpusEncoderType* pEncoder) NN_NOEXCEPT;

/**
 * @brief       1 フレーム分のインターリーブされた PCM データをエンコードして、Opus データを取得します。
 * @param[in]   pEncoder                   OpusEncoderType 構造体
 * @param[out]  pOutputSize                出力されたエンコードデータのサイズ
 * @param[out]  outputBuffer               エンコードデータの出力先バッファ
 * @param[in]   outputBufferSize           outputBuffer のサイズ
 * @param[in]   inputBuffer                入力 PCM バッファ
 * @param[in]   inputSampleCountPerChannel inputBuffer に格納されているサンプル数 (1チャンネルあたり)
 * @return      処理の結果を返します。
 * @retval      OpusResult_Success 成功
 * @retval      OpusResult_InsufficientOpusBuffer Opus データバッファ不足
 * @retval      OpusResult_InternalError Opus エンコーダの内部エラー
 *
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 * - pOutputSize != nullptr
 * - outputBuffer != nullptr
 * - inputBuffer != nullptr
 * - inputSampleCountPerChannel * 400 == @ref nn::codec::GetOpusEncoderSampleRate() "nn::codec::GetOpusEncoderSampleRate(pEncoder)" || inputSampleCountPerChannel * 200 == @ref nn::codec::GetOpusEncoderSampleRate() "nn::codec::GetOpusEncoderSampleRate(pEncoder)" || inputSampleCountPerChannel * 100 == @ref nn::codec::GetOpusEncoderSampleRate() "nn::codec::GetOpusEncoderSampleRate(pEncoder)" || inputSampleCountPerChannel * 50 == @ref nn::codec::GetOpusEncoderSampleRate() "nn::codec::GetOpusEncoderSampleRate(pEncoder)"
 * @details
 * 入力データのチャンネル数が 2 の場合は、inputBuffer に L, R, L, R, ... の順でデータを格納して下さい。
 * 処理に失敗した場合であっても outputBuffer の内容が改変されることがあります。
 * inputSampleCountPerChannel には、OpusEncoder がサポートするフレーム (2.5, 5.0, 10.0, 20.0 ms) に相当するサンプル数を指定する必要があります。
 * サンプル数は nn::codec::CalculateOpusEncoderFrameSampleCount() とフレームから計算できます。
 */
OpusResult EncodeOpusInterleaved(OpusEncoderType* pEncoder, size_t* pOutputSize, void* outputBuffer, size_t outputBufferSize, const int16_t* inputBuffer, int inputSampleCountPerChannel) NN_NOEXCEPT;

/**
 * @brief       OpusEncoder のビットレートを設定します。
 * @param[in]   pEncoder                OpusEncoderType 構造体
 * @param[in]   bitRate                 ビットレート
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 * - bitRate >= @ref nn::codec::GetOpusBitRateMin() "nn::codec::GetOpusBitRateMin(nn::codec::GetOpusEncoderChannelCount(pEncoder))" && bitRate <= @ref nn::codec::GetOpusBitRateMax() "nn::codec::GetOpusBitRateMax(nn::codec::GetOpusEncoderChannelCount(pEncoder))"
 * @details
 * 指定可能なビットレートの最大値は、
 * nn::codec::InitializeOpusEncoder() で指定した channelCount で変わります。
 * nn::codec::SetOpusEncoderBitRate() を一度も呼び出していない状態では、
 * nn::codec::InitializeOpusEncoder() に指定したチャンネル数とサンプルレートに基づき、Opus ライブラリの初期化関数が初期値を設定します。
 */
void SetOpusEncoderBitRate(OpusEncoderType* pEncoder, int bitRate) NN_NOEXCEPT;

/**
 * @brief       OpusEncoder インスタンスに指定されたビットレートを取得します。
 * @param[in]   pEncoder                OpusEncoderType 構造体
 * @return      ビットレートを返します。
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 */
int GetOpusEncoderBitRate(const OpusEncoderType* pEncoder) NN_NOEXCEPT;

/**
 * @brief       OpusEncoder のビットレートコントロールを設定します。
 * @param[in]   pEncoder                OpusEncoderType 構造体
 * @param[in]   bitRateControl          ビットレートコントロール
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 * - bitRateControl == nn::codec::OpusBitRateControl_Vbr || bitRateControl == nn::codec::OpusBitRateControl_Cvbr || bitRateControl == nn::codec::OpusBitRateControl_Cbr
 * @details
 * nn::codec::SetOpusEncoderBitRateControl() を一度も呼び出していない状態では nn::codec::OpusBitRateControl_Vbr が設定されています。
 */
void SetOpusEncoderBitRateControl(OpusEncoderType* pEncoder, OpusBitRateControl bitRateControl) NN_NOEXCEPT;

/**
 * @brief       OpusEncoder インスタンスに指定されたビットレートコントロールを取得します。
 * @param[in]   pEncoder                OpusEncoderType 構造体
 * @return      ビットレートコントロールを返します。
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 */
OpusBitRateControl GetOpusEncoderBitRateControl(const OpusEncoderType* pEncoder) NN_NOEXCEPT;

/**
 * @brief       OpusEncoder の符号化モードを設定します。
 * @param[in]   pEncoder                OpusEncoderType 構造体
 * @param[in]   codingMode              符号化モード
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 * - codingMode == nn::codec::OpusCodingMode_Auto || codingMode == nn::codec::OpusCodingMode_Celt || codingMode == nn::codec::OpusCodingMode_Silk
 * @details
 * nn::codec::BindOpusEncoderCodingMode() を一度も呼び出していない状態では nn::codec::OpusCodingMode_Auto が設定されています。
 */
void BindOpusEncoderCodingMode(OpusEncoderType* pEncoder, OpusCodingMode codingMode) NN_NOEXCEPT;

/**
 * @brief       OpusEncoder インスタンスに指定された符号化モードを取得します。
 * @param[in]   pEncoder                OpusEncoderType 構造体
 * @return      符号化モードを返します。
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 */
OpusCodingMode GetOpusEncoderCodingMode(const OpusEncoderType* pEncoder) NN_NOEXCEPT;

/**
 * @brief       フレームから OpusEncoder に指定するべきサンプル数を計算します。
 * @param[in]   pEncoder                OpusEncoderType 構造体
 * @param[in]   frame                   フレーム (単位は [us])
 * @return      サンプル数を返します。
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 * - frame == 2500 || frame == 5000 || frame == 10000 || frame == 20000
 */
int CalculateOpusEncoderFrameSampleCount(const OpusEncoderType* pEncoder, int frame) NN_NOEXCEPT;

/**
 * @brief       デコード結果先頭の読み飛ばすべきサンプル数を返します。
 * @param[in]   pEncoder                OpusEncoderType 構造体
 * @return      サンプル数を返します。
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 * @details
 * デコード結果の先頭には読み飛ばすことが推奨されるサンプルが含まれています。
 * このサンプルの数を返します。
 * 詳細は @confluencelink{164723624,こちら} をご覧ください。
 */
int GetOpusEncoderPreSkipSampleCount(const OpusEncoderType* pEncoder) NN_NOEXCEPT;

/**
 * @}
 */

/**
 * @internal
 * @brief       OpusEncoder インスタンスを初期化します。
 * @param[out]  pOutEncoder             OpusEncoderType 構造体
 * @param[in]   sampleRate              サンプルレート
 * @param[in]   channelCount            チャンネル数
 * @param[in]   application             用途
 * @param[in]   buffer                  ワークバッファ
 * @param[in]   size                    ワークバッファのサイズ
 * @return      処理の結果を返します。
 * @retval      OpusResult_Success 成功
 * @retval      OpusResult_InvalidSampleRate 無効なサンプルレート
 * @retval      OpusResult_InvalidChannelCount 無効なチャンネル数
 * @retval      OpusResult_InvalidWorkBuffer 無効なワークバッファ
 * @retval      OpusResult_InternalError 内部エラー
 *
 * @pre
 * - pOutEncoder != nullptr
 * - sampleRate == 48000 || sampleRate == 24000 || sampleRate == 16000 || sampleRate == 12000 || sampleRate == 8000
 * - channelCount >= 1 && channelCount <= 2
 * - application == nn::codec::OpusApplication_Audio || application == nn::codec::OpusApplication_Voip || application == OpusApplication_RestrictedLowDelay
 * - buffer != nullptr
 * - size >= @ref nn::codec::GetOpusEncoderWorkBufferSize "nn::codec::GetOpusEncoderWorkBufferSize(sampleRate, channelCount)"
 */
OpusResult InitializeOpusEncoder(OpusEncoderType* pOutEncoder, int sampleRate, int channelCount, OpusApplication application, void* buffer, size_t size) NN_NOEXCEPT;

/**
 * @internal
 * @brief       OpusEncoder の波形タイプを設定します。
 * @param[in]   pEncoder                OpusEncoderType 構造体
 * @param[in]   waveformType            波形タイプ
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 * - waveformType == nn::codec::OpusWaveformType_Auto || waveformType == nn::codec::OpusWaveformType_Music || waveformType == nn::codec::OpusWaveformType_Voice
 * @details
 * nn::codec::SetOpusEncoderWaveformType() を一度も呼び出していない状態では nn::codec::OpusWaveformType_Auto が設定されています。
 */
void SetOpusEncoderWaveformType(OpusEncoderType* pEncoder, OpusWaveformType waveformType) NN_NOEXCEPT;

/**
 * @internal
 * @brief       OpusEncoder インスタンスに指定された波形タイプを取得します。
 * @param[in]   pEncoder                OpusEncoderType 構造体
 * @return      波形タイプを返します。
 * @pre
 * - pEncoder != nullptr
 * - *pEncoder は初期化済み
 */
OpusWaveformType GetOpusEncoderWaveformType(const OpusEncoderType* pEncoder) NN_NOEXCEPT;

}}  // namespace nn::codec
