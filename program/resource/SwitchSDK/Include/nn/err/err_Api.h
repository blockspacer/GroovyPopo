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
 * @brief エラーAPIの定義
 */

#pragma once

#include <nn/err/err_ShowErrorApi.h>
#include <nn/err/err_Types.h>

namespace nn { namespace err {

//! @name エラーAPI
//! @{

/**
 * @brief エラーコードカテゴリの種類とエラーコード番号から ErrorCode を作成します。
 * @param[in] errorCodeCategoryType エラーコードカテゴリの種類。
 * @param[in] errorCodeNumber エラーコード番号。
 * @details
 *  ライブラリの API からエラーコード番号のみが取得でき、エラーコードカテゴリを手動で設定する必要がある場合などに使用します。
 */
ErrorCode MakeErrorCode(ErrorCodeCategoryType errorCodeCategoryType, ErrorCodeNumber errorCodeNumber) NN_NOEXCEPT;

//! @}
//-----------------------------------------------------------------------------

}} // namespace nn::err
