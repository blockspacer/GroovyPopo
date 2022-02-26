﻿/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
*   @file
*   @brief  ユーザ入力に関する API の宣言
*/

#pragma once

#include <nn/nn_Macro.h>

namespace nn { namespace hws {

/*!
*  @brief マウスホイールの回転値を取得します。
*
*  @return     ユーザが入力したマウスホイールの回転値を返します。値は 120 (WHEEL_DELTA) の倍数または約数で表現されます。
*
*  @pre        なし
*
*  @post
*         - マウスホイールの回転値を返す
*/
int GetMouseWheel() NN_NOEXCEPT;

}
} // namespace wnd
