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

#include <nn/codec/codec_AacDecoderApi.h>
#include <nn/codec/codec_AacDecoderTypes.h>
#include <nn/codec/codec_IAacDecoder.h>

namespace nn { namespace codec {

/**
 * @name        Aac
 * @{
 */

/**
 * @brief       Aac デコーダクラスです。
 */
class AacDecoder : public IAacDecoder
{
public:
    /**
     * @brief       コンストラクタです。
     */
    AacDecoder() NN_NOEXCEPT;

    /**
     * @brief       デストラクタです。
     */
    virtual ~AacDecoder() NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       AacDecoder に必要なワークバッファのサイズを取得します。
     * @param[in]   sampleRate              サンプルレート
     * @param[in]   channelCount            チャンネル数
     * @param[in]   type                    Mpeg-4 Audio Object Type
     * @return      ワークバッファのサイズを返します。
     * @details
     * 詳細は GetAacDecoderWorkBufferSize() を参照してください。
     */
    virtual size_t GetWorkBufferSize(int sampleRate, int channelCount, Mpeg4AudioObjectType type) const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetAacDecoderWorkBufferSize(sampleRate, channelCount, type);
    }

    /**
     * @brief       インスタンスが利用可能かどうかを取得します。
     * @return      インスタンスが利用可能かどうかを返します。
     */
    virtual bool IsInitialized() const NN_NOEXCEPT NN_OVERRIDE
    {
        return (m_Decoder._buffer != nullptr);
    }

    /**
     * @brief       初期化します。
     * @param[in]   sampleRate              サンプルレート
     * @param[in]   channelCount            チャンネル数
     * @param[in]   type                    Mpeg-4 Audio Object Type
     * @param[in]   buffer                  ワークバッファ
     * @param[in]   size                    ワークバッファのサイズ
     * @return      処理の結果を返します。
     * @details
     * 詳細は InitializeAacDecoder() を参照してください。
     */
    virtual AacDecoderResult Initialize(int sampleRate, int channelCount, Mpeg4AudioObjectType type, void* buffer, size_t size) NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       終了、破棄します。
     * @details
     * 詳細は FinalizeAacDecoder() を参照してください。
     */
    virtual void Finalize() NN_NOEXCEPT NN_OVERRIDE
    {
        return FinalizeAacDecoder(&m_Decoder);
    }

    /**
     * @brief       サンプルレートを取得します。
     * @return      サンプルレートを返します。
     * @details
     * 詳細は GetAacDecoderSampleRate() を参照してください。
     */
    virtual int GetSampleRate() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetAacDecoderSampleRate(&m_Decoder);
    }

    /**
     * @brief       チャンネル数を取得します。
     * @return      チャンネル数を返します。
     * @details
     * 詳細は GetAacDecoderChannelCount() を参照してください。
     */
    virtual int GetChannelCount() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetAacDecoderChannelCount(&m_Decoder);
    }

    /**
     * @brief       Mpeg-4 Audio Object Type を取得します。
     * @return      Mpeg-4 Audio Object Type を返します。
     * @details
     * 詳細は GetAacDecoderMpeg4AudioObjectType() を参照してください。
     */
    virtual Mpeg4AudioObjectType GetMpeg4AudioObjectType() const NN_NOEXCEPT NN_OVERRIDE
    {
        return GetAacDecoderMpeg4AudioObjectType(&m_Decoder);
    }

    /**
     * @brief       AacDecoder インスタンスによりデコード処理を行います。
     * @param[out]  pOutConsumed            デコード時に使用されたデータのサイズ
     * @param[out]  pOutSampleCount         出力サンプル数（1 チャンネルあたり）
     * @param[out]  outputBuffer            出力 PCM バッファ
     * @param[in]   outputSize              outputBuffer のサイズ
     * @param[in]   inputBuffer             入力データバッファ
     * @param[in]   inputSize               inputBuffer のサイズ
     * @return      処理の結果を返します。
     * @details
     * 詳細は DecodeAacInterleaved() を参照してください。
     */
    virtual AacDecoderResult DecodeInterleaved(size_t* pOutConsumed, int* pOutSampleCount, int16_t* outputBuffer, size_t outputSize, const void* inputBuffer, size_t inputSize) NN_NOEXCEPT NN_OVERRIDE
    {
        return DecodeAacInterleaved(&m_Decoder, pOutConsumed, pOutSampleCount, outputBuffer, outputSize, inputBuffer, inputSize);
    }

    /**
     * @brief       自インスタンスの AacDecoderType オブジェクトへのポインタを返します。
     * @return      AacDecoderType オブジェクトへのポインタ
     */
    AacDecoderType* GetBase() NN_NOEXCEPT
    {
        return &m_Decoder;
    }

private:
    AacDecoderType m_Decoder;
};

/**
 * @}
 */

}}  // namespace nn::codec
