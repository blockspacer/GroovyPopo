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
 * @brief   ユーザ例外ハンドラに関する定数の宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Common.h>


//-----------------------------------------------------------------------------
//  C++ 向けの宣言
//-----------------------------------------------------------------------------

namespace nn { namespace os {

struct UserExceptionInfo;

const uintptr_t HandlerStackUsesThreadStackValue = 0;
const int UserExceptionInfoUsesHandlerStackValue = 0;
const int UserExceptionInfoUsesThreadStackValue  = 1;

//--------------------------------------------------------------------------
/**
 * @brief   ユーザ例外ハンドラのスタックアライメント制約を表す定数です。
 */
const size_t HandlerStackAlignment = 16;

//-----------------------------------------------------------------------------
/**
 * @brief   ユーザ例外ハンドラのスタックとしてスレッドスタックを指定する定数です。
 */
void* const HandlerStackUsesThreadStack = reinterpret_cast<void*>(HandlerStackUsesThreadStackValue);

//-----------------------------------------------------------------------------
/**
 * @brief   例外情報格納領域として例外ハンドラのスタックを指定する定数です。
 */
UserExceptionInfo* const UserExceptionInfoUsesHandlerStack = reinterpret_cast<UserExceptionInfo*>(static_cast<const intptr_t>(UserExceptionInfoUsesHandlerStackValue));

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 * @brief   例外情報格納領域としてスレッドスタックを指定する定数です。
 */
UserExceptionInfo* const UserExceptionInfoUsesThreadStack  = reinterpret_cast<UserExceptionInfo*>(static_cast<const intptr_t>(UserExceptionInfoUsesThreadStackValue));

}} // namespace nn::os

