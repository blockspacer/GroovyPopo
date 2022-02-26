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
 * @brief       Opus データのエンコードに関する API を定義します。
 */

#include <nn/nn_Macro.h>
#include <nn/os/os_Mutex.h>
#include <nn/codec/codec_OpusCommon.h>
#include <nn/codec/codec_OpusEncoderApi.h>
#include <nn/codec/codec_OpusEncoderTypes.h>
#include <nn/codec/codec_IOpusEncoder.h>

namespace nn {
namespace codec {

/**
 * @name        Opus
 * @{
 */

/**
 * @brief       Opus エンコーダクラスです。
 */
class OpusEncoder : public IOpusEncoder
{
public:

    /**
     * @brief       コンストラクタです。
     */
    OpusEncoder() NN_NOEXCEPT;

    /**
     * @brief       デストラクタです。
     */
    virtual ~OpusEncoder() NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       必要なワークバッファのサイズを取得します。
     * @param[in]   sampleRate          サンプルレート
     * @param[in]   channelCount        チャンネル数
     * @return      ワークバッファのサイズを返します。
     * @details
     * 詳細は nn::codec::GetOpusEncoderWorkBufferSize() を参照してください。
     */
    size_t GetWorkBufferSize(int sampleRate, int channelCount) const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetOpusEncoderWorkBufferSize(sampleRate, channelCount);
    }

    /**
     * @brief       初期化されているかどうかを確認します。
     * @return      初期化されていれば true を、そうでなければ false を返します。
     */
    virtual bool IsInitialized() const NN_NOEXCEPT NN_OVERRIDE
    {
        return (m_Encoder._handle != nullptr);
    }

    /**
     * @brief       エンコーダを初期化します。
     * @param[in]   sampleRate          サンプルレート
     * @param[in]   channelCount        チャンネル数
     * @param[in]   buffer              ワークバッファ
     * @param[in]   size                ワークバッファのサイズ
     * @return      処理の結果を返します。
     * @details
     * 詳細は InitializeOpusEncoder() を参照してください。
     */
    virtual OpusResult Initialize(int sampleRate, int channelCount, void* buffer, size_t size) NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @internal
     * @brief       エンコーダを初期化します。
     * @param[in]   sampleRate          サンプルレート
     * @param[in]   channelCount        チャンネル数
     * @param[in]   application         用途
     * @param[in]   buffer              ワークバッファ
     * @param[in]   size                ワークバッファのサイズ
     * @return      処理の結果を返します。
     * @details
     * 詳細は nn::codec::InitializeOpusEncoder() を参照してください。
     */
    virtual OpusResult Initialize(int sampleRate, int channelCount, OpusApplication application, void* buffer, size_t size) NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       終了処理を行います。
     * @details
     * 詳細は nn::codec::FinalizeOpusEncoder() を参照してください。
     */
    virtual void Finalize() NN_NOEXCEPT NN_OVERRIDE
    {
        return FinalizeOpusEncoder(&m_Encoder);
    }

    /**
     * @brief       サンプルレートを取得します。
     * @return      サンプルレートを返します。
     * @details
     * 詳細は nn::codec::GetOpusEncoderSampleRate() を参照してください。
     */
    virtual int GetSampleRate() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetOpusEncoderSampleRate(&m_Encoder);
    }

    /**
     * @brief       チャンネル数を取得します。
     * @return      チャンネル数を返します。
     * @details
     * 詳細は nn::codec::GetOpusEncoderChannelCount() を参照してください。
     */
    virtual int GetChannelCount() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetOpusEncoderChannelCount(&m_Encoder);
    }

    /**
     * @brief       1 フレーム分のインターリーブされた PCM データをエンコードして、Opus データを取得します。
     * @param[out]  pOutputSize                出力されたエンコードデータのサイズ
     * @param[out]  outputBuffer               エンコードデータの出力先バッファ
     * @param[in]   outputBufferSize           outputBuffer のサイズ
     * @param[in]   inputBuffer                入力 PCM バッファ
     * @param[in]   inputSampleCountPerChannel inputBuffer に格納されているサンプル数 (1チャンネルあたり)
     * @return      処理の結果を返します。
     * @details
     * 詳細は nn::codec::EncodeOpusInterleaved() を参照してください。
     */
    virtual OpusResult EncodeInterleaved(size_t* pOutputSize, void* outputBuffer, size_t outputBufferSize, const int16_t* inputBuffer, int inputSampleCountPerChannel) NN_NOEXCEPT NN_OVERRIDE
    {
        return EncodeOpusInterleaved(&m_Encoder, pOutputSize, outputBuffer, outputBufferSize, inputBuffer, inputSampleCountPerChannel);
    }

    /**
     * @brief       ビットレートを設定します。
     * @param[in]   bitRate    ビットレート
     * @details
     * 詳細は nn::codec::SetOpusEncoderBitRate() を参照してください。
     */
    virtual void SetBitRate(int bitRate) NN_NOEXCEPT NN_OVERRIDE
    {
        return SetOpusEncoderBitRate(&m_Encoder, bitRate);
    }

    /**
     * @brief       ビットレートを取得します。
     * @return      ビットレートを返します。
     * @details
     * 詳細は nn::codec::GetOpusEncoderBitRate() を参照してください。
     */
    virtual int GetBitRate() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetOpusEncoderBitRate(&m_Encoder);
    }

    /**
     * @brief       ビットレートコントロールを設定します。
     * @param[in]   bitRateControl    ビットレートコントロール
     * @details
     * 詳細は nn::codec::SetOpusEncoderBitRateControl() を参照してください。
     */
    virtual void SetBitRateControl(OpusBitRateControl bitRateControl) NN_NOEXCEPT NN_OVERRIDE
    {
        return SetOpusEncoderBitRateControl(&m_Encoder, bitRateControl);
    }

    /**
     * @brief       ビットレートコントロールを取得します。
     * @return      ビットレートコントロールを返します。
     * @details
     * 詳細は nn::codec::GetOpusEncoderBitRateControl() を参照してください。
     */
    virtual OpusBitRateControl GetBitRateControl() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetOpusEncoderBitRateControl(&m_Encoder);
    }

    /**
     * @internal
     * @brief       波形タイプを設定します。
     * @param[in]   waveformType  波形タイプ
     * @details
     * 詳細は nn::codec::SetOpusEncoderWaveformType() を参照してください。
     */
    virtual void SetWaveformType(OpusWaveformType waveformType) NN_NOEXCEPT NN_OVERRIDE
    {
        return SetOpusEncoderWaveformType(&m_Encoder, waveformType);
    }

    /**
     * @internal
     * @brief       波形タイプを取得します。
     * @return      波形タイプを返します。
     * @details
     * 詳細は nn::codec::GetOpusEncoderWaveformType() を参照してください。
     */
    virtual OpusWaveformType GetWaveformType() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetOpusEncoderWaveformType(&m_Encoder);
    }

    /**
     * @brief       符号化モードを設定します。
     * @param[in]   codingMode    符号化モード
     * @details
     * 詳細は nn::codec::BindOpusEncoderCodingMode() を参照してください。
     */
    virtual void BindCodingMode(OpusCodingMode codingMode) NN_NOEXCEPT NN_OVERRIDE
    {
        return BindOpusEncoderCodingMode(&m_Encoder, codingMode);
    }

    /**
     * @brief       符号化モードを取得します。
     * @return      符号化モードを返します。
     * @details
     * 詳細は nn::codec::GetOpusEncoderCodingMode() を参照してください。
     */
    virtual OpusCodingMode GetCodingMode() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetOpusEncoderCodingMode(&m_Encoder);
    }

    /**
     * @brief       フレームからサンプル数を計算します。
     * @param[in]   frame       フレーム (単位は [us])
     * @return      サンプル数を返します。
     * @details
     * 詳細は nn::codec::CalculateOpusEncoderFrameSampleCount() を参照してください。
     */
    virtual int CalculateFrameSampleCount(int frame) const NN_NOEXCEPT NN_OVERRIDE
    {
        return CalculateOpusEncoderFrameSampleCount(&m_Encoder, frame);
    }

    /**
     * @brief       デコード結果先頭の読み飛ばすべきサンプル数を返します。
     * @return      サンプル数を返します。
     * @details
     * 詳細は nn::codec::GetOpusEncoderPreSkipSampleCount() を参照してください。
     */
    virtual int GetPreSkipSampleCount() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetOpusEncoderPreSkipSampleCount(&m_Encoder);
    }

    /**
     * @brief       自インスタンスの OpusEncoderType オブジェクトへのポインタを返します。
     * @return      OpusEncoderType オブジェクトへのポインタ
     */
    OpusEncoderType* GetBase() NN_NOEXCEPT
    {
        return &m_Encoder;
    }

private:
    OpusEncoderType m_Encoder;
};

/**
 * @}
 */

}  // namespace codec
}  // namespace nn
