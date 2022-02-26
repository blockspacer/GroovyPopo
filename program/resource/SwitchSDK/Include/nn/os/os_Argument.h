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
 * @brief   コマンドライン引数に関する公開ヘッダファイル
 */

#pragma once

#include <nn/nn_Macro.h>

namespace nn { namespace os {

//! @name コマンドライン引数関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   ホスト指定のコマンドライン引数の数 (argc) を取得します。
 *
 * @return  ホスト指定のコマンドライン引数の数（常に 1 以上）
 *
 * @details
 *  ホスト指定のコマンドライン引数の数 (argc) を取得します。
 *  引数情報がない場合、返り値は 1 となり、 nn::os::GetHostArgv()[0][0] == '\0'
 *  となります。
 *
 *  本 API は、TargetManager や VSI、RunOnTarget などからプログラムを起動する際に指定したコマンドライン引数を取得するためのものです。
 *
 */
int    GetHostArgc() NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   ホスト指定のコマンドライン引数の文字列を指すポインタの配列 (argv) を取得します。
 *
 * @return  ホスト指定のコマンドライン引数の文字列を指すポインタの配列へのポインタ
 *
 * @details
 *  ホスト指定のコマンドライン引数の文字列を指すポインタの配列 (argv) を取得します。
 *  引数情報がない場合、返り値は { "", NULL } という配列へのポインタを返します。
 *
 *  本 API は、TargetManager や VSI、RunOnTarget などからプログラムを起動する際に指定したコマンドライン引数を取得するためのものです。
 *
 */
char** GetHostArgv() NN_NOEXCEPT;

//! @}

}}   // namespace nn::os
