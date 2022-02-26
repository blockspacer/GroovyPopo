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

#include <nn/diag/detail/diag_DetailTranslation.h>

/**
* @file
* @brief   SDK 用の、文字列リテラルの文字コードを変換する API の宣言
*/

/**
* @brief 引数に指定された文字列リテラルの文字コードを UTF-8 に変換します
*
* @param[in] string    文字列リテラル
* @details
*  文字列リテラルを直接指定して使用します。
*
*  文字列リテラルに L 等のエンコーディングプレフィックスを付加してはいけません。
*
*  使用例:
*  @code
   NN_SDK_ASSERT(NN_TEXT("この文章が置き換わります。"));
*  @endcode
*/
#define NN_TEXT(string) NN_DETAIL_DIAG_SDK_TEXT_ENCODE(string)
