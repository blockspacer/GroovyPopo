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
 * @brief       Opus データのデコードに関する API を定義します。
 */

#include <nn/nn_Macro.h>
#include <nn/codec/codec_OpusCommon.h>
#include <nn/codec/codec_OpusDecoderApi.h>
#include <nn/codec/codec_IOpusDecoder.h>

namespace nn {
namespace codec {

/**
 * @name        Opus
 * @{
 */

/**
 * @brief       Opus デコーダクラスです。
 * @details
 * モノラル、ステレオ音源の Opus データをデコードします。
 */
class OpusDecoder : public IOpusDecoder
{
public:
    /**
     * @brief       コンストラクタです。
     */
    OpusDecoder() NN_NOEXCEPT;

    /**
     * @brief       デストラクタです。
     */
    virtual ~OpusDecoder() NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       必要なワークバッファのサイズを取得します。
     * @param[in]   sampleRate          サンプルレート
     * @param[in]   channelCount        チャンネル数
     * @return      ワークバッファのサイズを返します。
     * @details
     * 詳細は GetOpusDecoderWorkBufferSize() を参照してください。
     */
    virtual size_t GetWorkBufferSize(int sampleRate, int channelCount) const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetOpusDecoderWorkBufferSize(sampleRate, channelCount);
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
     * @brief       デコーダを初期化します。
     * @param[in]   sampleRate          サンプルレート
     * @param[in]   channelCount        チャンネル数
     * @param[in]   buffer              ワークバッファ
     * @param[in]   size                ワークバッファのサイズ
     * @return      処理の結果を返します。
     * @details
     * 詳細は InitializeOpusDecoder() を参照してください。
     */
    virtual OpusResult Initialize(int sampleRate, int channelCount, void* buffer, size_t size) NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       終了処理を行います。
     * @details
     * 詳細は FinalizeOpusDecoder() を参照してください。
     */
    virtual void Finalize() NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       サンプルレートを取得します。
     * @return      サンプルレートを返します。
     * @details
     * 詳細は GetOpusDecoderSampleRate() を参照してください。
     */
    virtual int GetSampleRate() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetOpusDecoderSampleRate(&m_Decoder);
    }

    /**
     * @brief       チャンネル数を取得します。
     * @return      チャンネル数を返します。
     * @details
     * 詳細は GetOpusDecoderChannelCount() を参照してください。
     */
    virtual int GetChannelCount() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetOpusDecoderChannelCount(&m_Decoder);
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
     * 詳細は DecodeOpusInterleaved() を参照してください。
     */
    virtual OpusResult DecodeInterleaved(size_t* pOutConsumed, int* pOutSampleCount, int16_t* outputBuffer, size_t outputSize, const void* inputBuffer, size_t inputSize) NN_NOEXCEPT NN_OVERRIDE
    {
        return DecodeOpusInterleaved(&m_Decoder, pOutConsumed, pOutSampleCount, outputBuffer, outputSize, inputBuffer, inputSize);
    }

    /**
     * @brief       Initialize() を呼び出した直後の状態にリセットします。
     * @details
     * 詳細は ResetOpusDecoder() を参照してください。
     */
    void Reset() NN_NOEXCEPT NN_OVERRIDE
    {
        ResetOpusDecoder(&m_Decoder);
    }

    /**
     * @brief       自インスタンスの OpusDecoderType オブジェクトへのポインタを返します。
     * @return      OpusDecoderType オブジェクトへのポインタ
     */
    OpusDecoderType* GetBase() NN_NOEXCEPT
    {
        return &m_Decoder;
    }

private:
    OpusDecoderType m_Decoder;
};

/**
 * @}
 */

}  // namespace codec
}  // namespace nn
