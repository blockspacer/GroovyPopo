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

/**
 * @file
 * @brief       列挙型 SampleFormat と、関連する API を定義します。
 */

namespace nn {
namespace audio {

/**
 * @brief       サンプルフォーマットを表す列挙型です。
 * @details     オーディオデバイスのサンプルフォーマットを表現する列挙型です。
 */
enum SampleFormat
{
    SampleFormat_Invalid = 0,    //!< 無効なフォーマットです
    SampleFormat_PcmInt8 = 1,    //!< 8 bit 符号付き整数 PCM フォーマットです。
    SampleFormat_PcmInt16 = 2,   //!< 16 bit 符号付き整数 PCM フォーマットです。
    SampleFormat_PcmInt24 = 3,   //!< 24 bit 符号付き整数 PCM フォーマットです。
    SampleFormat_PcmInt32 = 4,   //!< 32 bit 符号付き整数 PCM フォーマットです。
    SampleFormat_PcmFloat = 5,   //!< 単精度浮動小数点 PCM フォーマットです。
    SampleFormat_Adpcm = 6,      //!< ADPCM フォーマットです。
};

/**
 * @name        その他
 * @{
 */

/**
 * @brief       指定したサンプルフォーマットにより表現されるオーディオサンプルのバイトサイズを返します。
 * @param[in]   sampleFormat            サンプルフォーマット
 * @return      sampleFormat で表現されるサンプルのバイトサイズを返します。
 * @pre         なし。
 * @post        なし。
 */
size_t GetSampleByteSize(SampleFormat sampleFormat) NN_NOEXCEPT;

/**
 * @}
 */

}  // namespace audio
}  // namespace nn
