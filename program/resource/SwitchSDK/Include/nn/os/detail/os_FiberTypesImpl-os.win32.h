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
 * @brief   OS-API 内部実装用のファイバー型の宣言（Windows 版）
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Common.h>

namespace nn { namespace os { namespace detail {

//-----------------------------------------------------------------------------

struct FiberTypeImplByWin32
{
    void*   _win32fiber;
};

typedef FiberTypeImplByWin32    FiberTypeImpl;

//-----------------------------------------------------------------------------

}}} // namespace nn::os::detail

