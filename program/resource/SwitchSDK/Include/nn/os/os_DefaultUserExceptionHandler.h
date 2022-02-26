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
 * @brief   デフォルトユーザー例外ハンドラの宣言
 */

#pragma once

#include <nn/TargetConfigs/build_Compiler.h>
#include <nn/os/os_UserExceptionHandler.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

//! @name デフォルトユーザー例外ハンドラ
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   デフォルトユーザー例外ハンドラです。
 *
 * @param[in] info 発生した例外の情報
 *
 * @details
 *  プログラム起動時に標準で設定されるユーザー例外ハンドラです。
 *  例外発生時に以下の情報をログに出力します。
 *
 *  - スレッド、もしくはファイバーの情報
 *  - レジスタ一覧
 *  - スタックトレース
 *  - スタックダンプ
 *  - メモリ状態一覧
 *
 *  このデフォルトユーザ例外ハンドラは、以下の設定相当で登録されています。
 *
 *  nn::os::SetUserExceptionHandler(nn::os::DefaultUserExceptionHandler,
 *                                  nn::os::HandlerStackUsesThreadStack, 0,
 *                                  nn::os::UserExceptionInfoUsesHandlerStack);
 *
 *  独自のユーザー例外ハンドラを利用したい場合には、
 *  SetUserExceptionHandler() によってユーザー例外ハンドラを上書きすることができます。@n
 *  独自のユーザー例外ハンドラから本 API を呼び出すこともできます。
 *
 */
void DefaultUserExceptionHandler(nn::os::UserExceptionInfo* info) NN_NOEXCEPT;

//! @}


}} // namespace nn::os

