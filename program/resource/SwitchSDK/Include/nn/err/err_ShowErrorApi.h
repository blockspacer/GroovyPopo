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
 * @brief エラー表示用APIの定義
 */

#pragma once

#include <nn/nn_Result.h>
#include <nn/err/err_ErrorResultVariant.h>
#include <nn/err/err_Types.h>

namespace nn { namespace err {

//! @name エラー表示用API
//! @{

/**
 * @brief 発生したエラーを表示します。
 * @param[in] result エラーの nn::Result 値
 * @pre
 *  - result.IsFailure() == true
 * @details
 * - エラー表示を行っている間は呼び出し元をブロックします。
 * - 本関数は「エラー表示関数」の1つです。
 */
void ShowError(nn::Result result) NN_NOEXCEPT;

/**
 * @brief 発生したエラーを表示します。
 * @param[in] errorCode 発生したエラーに対応する ErrorCode データ
 * @pre
 *  - errorCode が有効なエラーコードを表すデータである
 * @details
 * - エラー表示を行っている間は呼び出し元をブロックします。
 * - 本関数は「エラー表示関数」の1つです。
 */
void ShowError(ErrorCode errorCode) NN_NOEXCEPT;

/**
* @brief 発生したエラーを表示します。
* @param[in] errorResultVariant 発生したエラーの情報を保持したオブジェクト。
* @pre
*  - errorResultHolder.GetState() != ErrorResultVariantState_HasNone
* @details
* - エラー表示を行っている間は呼び出し元をブロックします。
* - 本関数は「エラー表示関数」の1つです。
*/
void ShowError(const ErrorResultVariant& errorResultVariant) NN_NOEXCEPT;

//! @}
//-----------------------------------------------------------------------------

}} // namespace nn::err
