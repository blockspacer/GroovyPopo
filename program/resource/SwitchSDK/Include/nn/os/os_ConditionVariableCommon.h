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
 * @brief   条件変数に関する列挙型の定義
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/os/os_Macro.h>
#include <nn/os/detail/os_MacroImpl.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   時限付き条件変数待ち機能の返値を表す列挙型です。
 */
enum ConditionVariableStatus
{
    ConditionVariableStatus_Timeout     = 0,    //!< タイムアウトしました
    ConditionVariableStatus_NoTimeout   = 1,    //!< タイムアウト以外で起床しました
};

//--------------------------------------------------------------------------

}} // namespace nn::os

