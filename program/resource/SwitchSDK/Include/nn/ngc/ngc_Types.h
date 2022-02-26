/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * @file
 * @brief   NGCライブラリに関する型や定数の宣言
 */

#pragma once

#include <nn/nn_Common.h>

namespace nn {
namespace ngc {

/**
 * @brief 単語をチェックするときの、一単語あたりの最大文字数(NULL終端文字を含む)です。
 *        この値は文字数を表します。
 *        バイト数に変換するにはnn::util::Strnlen等で文字列の要素数を取得してください。
 */
const int WordLengthMax = 64;

/**
 * @brief 単語をチェックするときに、一度にチェック可能な最大の単語数です。
 */
const int WordCountMax = 16;

/**
 * @brief 文章をチェックするときの、文章の最大文字数(NULL終端文字を含む)です。
 */
const int TextLengthMax = 512;

}  // ngc
}  // nn
