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
#include <nn/codec/codec_HardwareOpusMultiStreamDecoderApi.h>
#include <nn/codec/codec_IOpusMultiStreamDecoder.h>

namespace nn {
namespace codec {

/**
 * @name        Opus
 * @{
 */

/**
 * @brief       ハードウェアを利用する Opus マルチストリームデコーダクラスです。
 * @details
 * モノラル、ステレオ音源に加え、3 チャンネル以上の音源の Opus データをデコードします。
 *
 * nn::codec::HardwareOpusDecoder が扱う Opus データはシングルストリームであり、本クラスでも扱うことができます。
 * ただし、本クラスでは nn::codec::HardwareOpusDecoder より大きなワークバッファサイズを必要とします。
 * 特別な理由がない限り、モノラル、ステレオ音源の Opus データのデコードには nn::codec::HardwareOpusDecoder を使うことを推奨します。
 */
class HardwareOpusMultiStreamDecoder : public IOpusMultiStreamDecoder
{
public:
    /**
     * @brief       コンストラクタです。
     * @param[in]   option              オプション
     * @details
     * オプションの詳細はプラットフォーム毎に定義されます。
     *
     * @platformbegin{NX}
     * NX では 0 を指定してください。
     * @platformend
     */
    explicit HardwareOpusMultiStreamDecoder(int32_t option) NN_NOEXCEPT;

    /**
     * @brief       デストラクタです。
     */
    virtual ~HardwareOpusMultiStreamDecoder() NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       必要なワークバッファのサイズを取得します。
     * @param[in]   sampleRate          サンプルレート
     * @param[in]   channelCount        チャンネル数
     * @param[in]   totalStreamCount    合計ストリーム数
     * @param[in]   stereoStreamCount   ステレオストリーム数
     * @return      ワークバッファのサイズを返します。
     * @details
     * 詳細は GetHardwareOpusMultiStreamDecoderWorkBufferSize() を参照してください。
     */
    virtual size_t GetWorkBufferSize(int sampleRate, int channelCount, int totalStreamCount, int stereoStreamCount) const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetHardwareOpusMultiStreamDecoderWorkBufferSize(sampleRate, channelCount, totalStreamCount, stereoStreamCount);
    }

    /**
     * @brief       初期化されているかどうかを確認します。
     * @return      初期化されていれば true を、そうでなければ false を返します。
     */
    virtual bool IsInitialized() const NN_NOEXCEPT NN_OVERRIDE
    {
        return (m_Decoder._handle != nullptr);
    }

    /**
     * @brief       マルチストリームデコーダを初期化します。
     * @param[in]   sampleRate          サンプルレート
     * @param[in]   channelCount        チャンネル数
     * @param[in]   totalStreamCount    合計ストリーム数
     * @param[in]   stereoStreamCount   ステレオストリーム数
     * @param[in]   channelMapping      チャンネルマッピング
     * @param[in]   buffer              ワークバッファ
     * @param[in]   size                ワークバッファのサイズ
     * @return      処理の結果を返します。
     * @details
     * 詳細は InitializeHardwareOpusMultiStreamDecoder() を参照してください。
     */
    virtual OpusResult Initialize(int sampleRate, int channelCount, int totalStreamCount, int stereoStreamCount, const uint8_t channelMapping[], void* buffer, size_t size) NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       終了処理を行います。
     * @details
     * 詳細は FinalizeHardwareOpusMultiStreamDecoder() を参照してください。
     */
    virtual void Finalize() NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       サンプルレートを取得します。
     * @return      サンプルレートを返します。
     * @details
     * 詳細は GetHardwareOpusMultiStreamDecoderSampleRate() を参照してください。
     */
    virtual int GetSampleRate() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetHardwareOpusMultiStreamDecoderSampleRate(&m_Decoder);
    }

    /**
     * @brief       チャンネル数を取得します。
     * @return      チャンネル数を返します。
     * @details
     * 詳細は GetHardwareOpusMultiStreamDecoderChannelCount() を参照してください。
     */
    virtual int GetChannelCount() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetHardwareOpusMultiStreamDecoderChannelCount(&m_Decoder);
    }

    /**
     * @brief       合計ストリーム数を取得します。
     * @return      合計ストリーム数を返します。
     * @details
     * 詳細は GetHardwareOpusMultiStreamDecoderTotalStreamCount() を参照してください。
     */
    virtual int GetTotalStreamCount() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetHardwareOpusMultiStreamDecoderTotalStreamCount(&m_Decoder);
    }

    /**
     * @brief       ステレオストリーム数を取得します。
     * @return      ステレオストリーム数を返します。
     * @details
     * 詳細は GetHardwareOpusMultiStreamDecoderStereoStreamCount() を参照してください。
     */
    virtual int GetStereoStreamCount() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetHardwareOpusMultiStreamDecoderStereoStreamCount(&m_Decoder);
    }

    /**
     * @brief       1 フレーム分のデータをデコードして、インターリーブされた PCM データを取得します。
     * @param[out]  pOutConsumed        デコード時に使用されたデータのサイズ
     * @param[out]  pOutSampleCount     出力サンプル数（1 チャンネルあたり）
     * @param[out]  outputBuffer        出力 PCM バッファ
     * @param[in]   outputSize          outputBuffer のサイズ
     * @param[in]   inputBuffer         入力データバッファ
     * @param[in]   inputSize           inputBuffer のサイズ
     * @return      処理の結果を返します。
     * @details
     * 詳細は DecodeOpusMultiStreamInterleavedWithHardware() を参照してください。
     */
    virtual OpusResult DecodeInterleaved(size_t* pOutConsumed, int* pOutSampleCount, int16_t* outputBuffer, size_t outputSize, const void* inputBuffer, size_t inputSize) NN_NOEXCEPT NN_OVERRIDE
    {
        return DecodeOpusMultiStreamInterleavedWithHardware(&m_Decoder, pOutConsumed, pOutSampleCount, outputBuffer, outputSize, inputBuffer, inputSize);
    }

    /**
     * @brief       Initialize() を呼び出した直後の状態にリセットします。
     * @details
     * 詳細は ResetHardwareOpusMultiStreamDecoder() を参照してください。
     */
    void Reset() NN_NOEXCEPT NN_OVERRIDE
    {
        ResetHardwareOpusMultiStreamDecoder(&m_Decoder);
    }

    /**
     * @brief       直前のデコード実行時に、ハードウェアアクセラレータ上で要した処理時間を返します。
     * @return      処理時間を返します。
     * @details
     * 詳細は GetHardwareOpusMultiStreamDecoderProcessingTime() を参照してください。
     */
    nn::TimeSpan GetProcessingTime() const NN_NOEXCEPT
    {
        return GetHardwareOpusMultiStreamDecoderProcessingTime(&m_Decoder);
    }

    /**
     * @brief       自インスタンスの HardwareOpusMultiStreamDecoderType オブジェクトへのポインタを返します。
     * @return      HardwareOpusMultiStreamDecoderType オブジェクトへのポインタ
     */
    HardwareOpusMultiStreamDecoderType* GetBase() NN_NOEXCEPT
    {
        return &m_Decoder;
    }

private:
    HardwareOpusMultiStreamDecoderType m_Decoder;
    int32_t m_Option;
};

/**
 * @}
 */

}  // namespace codec
}  // namespace nn
