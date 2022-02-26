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
#include <nn/codec/codec_AacCommonTypes.h>

namespace nn { namespace codec {

/**
 * @name        Aac
 * @{
 */

/**
 * @brief       AacDecoder を扱うための構造体です。
 */
struct AacDecoderType
{
    void* _handle;
    void* _buffer;
    size_t _size;
    int _sampleRate;
    int _channelCount;
    Mpeg4AudioObjectType _type;
};

/**
 * @brief       AacDecoder の処理結果です。
 */
enum AacDecoderResult
{
    AacDecoderResult_Success = 0,                           //!< 成功

    AacDecoderResult_OutOfMemory = 0x0002,                  //!< メモリ不足

    AacDecoderResult_NotEnoughBits = 0x1002,                //!< 入力データが不足しています

    AacDecoderResult_InvalidHandle = 0x2001,                //!< 不正なハンドル
    AacDecoderResult_UnsupportedAudioObjectType = 0x2002,   //!< サポートされていない Audio Object Type
    AacDecoderResult_UnsupportedFormat = 0x2003,            //!< サポートされていないフォーマット
    AacDecoderResult_UnsupportedChannelConfig = 0x2007,     //!< サポートされていないチャンネルコンフィグ
    AacDecoderResult_UnsupportedSampleRate = 0x2008,        //!< サポートされていないサンプルレート
    AacDecoderResult_InvalidParameter = 0x200a,             //!< 無効なパラメータ
    AacDecoderResult_ReinitializationRequired = 0x200b,     //!< デコーダの再初期化が必要
    AacDecoderResult_OutputBufferTooSmall = 0x200c,         //!< 出力バッファのサイズ不足

    AacDecoderResult_CrcError = 0x4005,                     //!< CRC エラー

    AacDecoderResult_InputBitstreamCorrupted = 0xfffe,      //!< 入力ビットストリーム壊れ
    AacDecoderResult_InternalError = 0xffff,                //!< 内部エラー
};

/**
 * @}
 */

}}  // namespace nn::codec
