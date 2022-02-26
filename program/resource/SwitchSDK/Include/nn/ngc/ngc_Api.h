/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/** @file
    @brief      NGCライブラリ に関する API の宣言
*/

#pragma once

namespace nn {
namespace ngc {

/**
 * @name    文字列解析処理
 * @{
 */

/**
 * @brief   文字列に含まれる、数を表す文字の個数を返します。
 *
 * @param[in]  pString     調べる対象の NULL 終端文字列を指定します。文字コードは UTF-8 で格納してください。
 *
 * @return  指定された文字列に含まれる数を表す文字の個数が返ります。失敗した場合に負の数が返ります。
 *
 * @details
 * 多くの数値を含む文字列は、電話番号などの個人情報を示す場合があります。
 * ユーザーが入力した文字列に含まれる数字の数から個人情報がどれくらい含まれているかを
 * 参考にするためにご使用ください。
 */
int CountNumbers( const char* pString ) NN_NOEXCEPT;

/**
 * @brief   文字列に含まれる、数を表す文字の個数を返します。
 *
 * @param[in]  pString     調べる対象の NULL 終端文字列を指定します。文字コードは UTF-16 で格納してください。
 *
 * @return  指定された文字列に含まれる数を表す文字の個数が返ります。失敗した場合に負の数が返ります。
 *
 * @details
 * 多くの数値を含む文字列は、電話番号などの個人情報を示す場合があります。
 * ユーザーが入力した文字列に含まれる数字の数から個人情報がどれくらい含まれているかを
 * 参考にするためにご使用ください。
 */
int CountNumbers(const uint16_t* pString) NN_NOEXCEPT;

/**
 * @brief   文字列に含まれる、数を表す文字の個数を返します。
 *
 * @param[in]  pString     調べる対象の NULL 終端文字列を指定します。文字コードは UTF-16 で格納してください。
 *
 * @return  指定された文字列に含まれる数を表す文字の個数が返ります。失敗した場合に負の数が返ります。
 *
 * @details
 * 多くの数値を含む文字列は、電話番号などの個人情報を示す場合があります。
 * ユーザーが入力した文字列に含まれる数字の数から個人情報がどれくらい含まれているかを
 * 参考にするためにご使用ください。
 */
int CountNumbers(const char16_t* pString) NN_NOEXCEPT;

/**
 * @}
 */

}   // ngc
}   // nn

