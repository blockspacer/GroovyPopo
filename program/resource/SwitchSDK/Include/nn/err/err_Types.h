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
 * @brief エラーAPIで利用される型の定義
 */

#pragma once

#include <cstdint>
#include <nn/settings/settings_Language.h>

namespace nn { namespace err {

//! @name エラーAPIで利用される型
//! @{

/**
 * @brief エラーコードのカテゴリを示す数値です。
 * @details
 * - 値の内容はライブラリやモジュールの種類によって決められています。
 * - この値をエラーハンドリングに使用しないでください。
 */
typedef uint32_t ErrorCodeCategory;

/**
 * @brief エラーコードの番号を示す数値です。
 * @details
 * - 値の内容は各ライブラリ・モジュールによって決められており、それぞれ異なった意味を持ちます。
 * - この値をエラーハンドリングに使用しないでください。
 */
typedef uint32_t ErrorCodeNumber;

/**
 * @brief エラーコードを表す構造体です。
 * @details
 * - この値をエラーハンドリングに使用しないでください。
 */
struct ErrorCode
{
    static const int StringLengthMax = 15; //!< エラーコードを文字列形式で表現した際に最大で必要となる配列の長さです。NULL終端を含めています。

    /**
    * @brief    カテゴリ。
    */
    ErrorCodeCategory category;
    /**
    * @brief    番号。
    */
    ErrorCodeNumber number;

    /**
    * @brief    有効なエラーコードかどうかを返します。
    * @return   有効なエラーコードならば true。そうでなければ false。
    */
    bool IsValid() const NN_NOEXCEPT;

    /**
    * @brief    無効なエラーコードを返します。
    * @return   無効なエラーコード。
    */
    static ErrorCode GetInvalidErrorCode() NN_NOEXCEPT;
};

/**
* @brief アプリケーション独自のエラーコードの番号を示す数値です。
*/
typedef ErrorCodeNumber ApplicationErrorCodeNumber;

/**
 * @brief エラーコードのカテゴリの種類です。
 */
enum class ErrorCodeCategoryType : uint32_t
{
    SubscriptionService,        //!< 期間型サービスアイテム(期間券)
    ConsumableService,          //!< 消費型サービスアイテム

    ErrorCodeCategoryTypeCount, // 要素数カウント用(カテゴリの種類ではありません)
};

//! @}
//-----------------------------------------------------------------------------

}}
