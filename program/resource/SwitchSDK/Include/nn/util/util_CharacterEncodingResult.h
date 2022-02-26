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

namespace nn { namespace util {

/*!
    @brief     文字エンコーディング変換の結果(エラーコード)です。
*/
enum CharacterEncodingResult {
    CharacterEncodingResult_Success,            //!< 正常に変換されました。
    CharacterEncodingResult_InsufficientLength, //!< 変換先の配列の長さが不足しました。
    CharacterEncodingResult_InvalidFormat,      //!< 変換できない文字が検出されました。
};

}}
