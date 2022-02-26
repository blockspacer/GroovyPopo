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
 * @brief   ユーザ例外ハンドラに関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/os/os_UserExceptionHandlerTypes.h>


//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

//! @name ユーザ例外ハンドラ関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   ユーザ例外ハンドラを設定します。
 *
 * @param[in] pHandler        例外発生時に呼び出される例外ハンドラの関数
 * @param[in] stack           例外ハンドラで使用するスタック領域の先頭アドレス
 * @param[in] stackSize       例外ハンドラで使用するスタック領域のサイズ
 * @param[in] pExceptionInfo  例外情報を格納するバッファ領域へのポインタ
 *
 * @pre
 *  - stack     % nn::os::HandlerStackAlignment == 0
 *  - stackSize % nn::os::HandlerStackAlignment == 0
 *
 */
void SetUserExceptionHandler(UserExceptionHandler pHandler, void* stack, size_t stackSize, UserExceptionInfo* pExceptionInfo) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   デバッガ接続時にもユーザ例外ハンドラを起動するか否かを設定します。
 *
 * @param[in] isEnabled   デバッガ接続時にもユーザ例外ハンドラを起動するか否か
 *
 * @details
 *  通常、ユーザ例外ハンドラはデバッガが接続されている場合には起動しませんが、
 *  本 API はそのような状態でもユーザ例外ハンドラが起動するようになります。
 *
 *  isEnabled == true を指定した場合、デバッガの接続中にユーザ例外が発生すると、
 *  ユーザ例外ハンドラを起動してからデバッガがプログラムを停止させます。
 *
 *  そのため、デバッガで停止時に表示される位置やレジスタ等のコンテキストは
 *  実際に例外が発生したときのものですが、
 *  ユーザ例外ハンドラの処理を行なった後であるため、
 *  ユーザ例外ハンドラが書き換えたメモリは変化した後であることに注意して下さい。
 *
 *  本 API はユーザ例外ハンドラをデバッグするために使用するためのものです。
 *
 *  デフォルトでは isEnabled = false となっています。
 *
 */
void EnableUserExceptionHandlerOnDebugging(bool isEnabled) NN_NOEXCEPT;

//! @}


}}  // namespace nn::os

