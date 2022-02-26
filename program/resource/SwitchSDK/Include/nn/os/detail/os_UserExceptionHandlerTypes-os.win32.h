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
 * @brief   ユーザ例外ハンドラに関する Windows 用の型の宣言
 */

#pragma once

#include <nn/os/os_Config.h>

#include <nn/nn_Common.h>
#include <nn/os/os_Macro.h>
#include <nn/os/detail/os_MacroImpl.h>

//-----------------------------------------------------------------------------
//  C++ 向けの宣言
//-----------------------------------------------------------------------------

namespace nn { namespace os {

//-----------------------------------------------------------------------------
/**
 * @brief   Windows 環境において例外情報の詳細を格納する構造体です。
 *
 * @details
 *  この構造体はダミーであり、ビルドエラーを回避するために定義されています。@n
 *  Windows 環境ではユーザ例外ハンドラ機能はサポートされておりません。
 */
struct UserExceptionInfoDetailWin32
{
    Bit32   dummy;                              //!< ダミーメンバ変数
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(UserExceptionInfoDetailWin32);

typedef UserExceptionInfoDetailWin32    UserExceptionInfoDetail;

//-----------------------------------------------------------------------------

}} // namespace nn::os

