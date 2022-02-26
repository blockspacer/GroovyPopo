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

namespace nn { namespace codec {

/**
 * @brief       Opus データ内の情報タイプです。
 */
enum OpusInfoType
{
    OpusInfoType_BasicInfo       = 0x80000001,      //!< 基本情報
    OpusInfoType_OffsetInfo      = 0x80000002,      //!< （未使用）
    OpusInfoType_ContextInfo     = 0x80000003,      //!< （未使用）
    OpusInfoType_DataInfo        = 0x80000004,      //!< データ本体
    OpusInfoType_MultiStreamInfo = 0x80000005,      //!< マルチストリーム情報
};

/**
 * @brief       Opus データ内の情報ヘッダです。
 */
struct OpusInfoHeader
{
    int32_t type;                       //!< タイプ (OpusInfoType)
    int32_t size;                       //!< サイズ
};

/**
 * @brief       Opus ヘッダ内の基本情報です。
 */
struct OpusBasicInfo
{
    OpusInfoHeader header;              //!< 情報ヘッダ
    int8_t version;                     //!< （未使用）
    int8_t channelCount;                //!< チャンネル数
    int16_t frameDataSize;              //!< 1 フレームのバイトサイズ（可変ビットレートの場合は 0）
    int32_t sampleRate;                 //!< サンプルレート
    int32_t dataInfoOffset;             //!< Opus データ本体の、ファイル先頭位置からのオフセット
    int32_t frameDataOffsetInfoOffset;  //!< （未使用）
    int32_t contextInfoOffset;          //!< （未使用）
    int16_t preSkipSampleCount;         //!< エンコーダの内部遅延サンプル数
    uint8_t reserved[2];                //!< （未使用）
};

/**
 * @brief       Opus ヘッダ内のマルチストリーム情報です。
 */
struct OpusMultiStreamInfo
{
    OpusInfoHeader header;              //!< 情報ヘッダ
    uint8_t totalStreamCount;           //!< 合計ストリーム数
    uint8_t stereoStreamCount;          //!< ステレオストリーム数
    uint8_t reserved[2];                //!< （未使用）
    uint8_t channelMapping[OpusStreamCountMax]; //!< チャンネルマッピング
};

}}  // namespace nn::codec
