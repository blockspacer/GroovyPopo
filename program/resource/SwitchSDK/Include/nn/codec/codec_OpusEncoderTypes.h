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

#include <nn/os/os_MutexTypes.h>

/**
 * @namespace   nn::codec
 * @brief       オーディオおよびビデオに関する圧縮コーデックライブラリの名前空間です。
 */

/**
 * @file
 * @brief       Opus エンコーダに関するデータ型を定義します。
 */

namespace nn {
namespace codec {

/**
 * @internal
 * @brief       Opus エンコーダが扱うエンコード用途を表す列挙型です。
 */
enum OpusApplication
{
    OpusApplication_Audio = 1,              //!< @internal 音楽用途
    OpusApplication_Voip,                   //!< @internal 音声用途
    OpusApplication_RestrictedLowDelay,     //!< @internal 低遅延
    OpusApplication_Invalid,                //!< @internal 無効値
};

/**
 * @internal
 * @brief       Opus エンコーダが扱う波形タイプを表す列挙型です。
 */
enum OpusWaveformType
{
    OpusWaveformType_Auto = 1,             //!< @internal 自動判別
    OpusWaveformType_Music,                //!< @internal 音楽音源
    OpusWaveformType_Voice,                //!< @internal 音声音源
    OpusWaveformType_Invalid,              //!< @internal 無効値
};

/**
 * @name        Opus
 * @{
 */

/**
 * @brief       Opus エンコーダが扱うビットレートコントロールを表す列挙型です。
 */
enum OpusBitRateControl
{
    OpusBitRateControl_Vbr = 1,             //!< 可変ビットレート
    OpusBitRateControl_Cvbr = 2,            //!< 制限付き可変ビットレート
    OpusBitRateControl_Cbr = 3,             //!< 固定ビットレート
    OpusBitRateControl_Invalid = 4          //!< 無効値
};

/**
 * @brief       Opus エンコーダが扱う符号化モードを表す列挙型です。
 */
enum OpusCodingMode
{
    OpusCodingMode_Auto = 1,            //!< 自動判別
    OpusCodingMode_Celt,                //!< Celt
    OpusCodingMode_Silk,                //!< Silk
    OpusCodingMode_Hybrid,              //!< @internal Hybrid
    OpusCodingMode_Invalid,             //!< 無効値
};

/**
 * @brief       OpusEncoder を扱うための構造体です。
 */
struct OpusEncoderType
{
    void* _handle;
    void* _buffer;
    size_t _size;
    int _sampleRate;
    int _channelCount;
    int _bitRate;
    OpusBitRateControl _bitRateControl;
    OpusCodingMode _codingMode;
    OpusWaveformType _waveformType;

    nn::os::MutexType _mutex;
};

/**
 * @}
 */

}  // namespace codec
}  // namespace nn
