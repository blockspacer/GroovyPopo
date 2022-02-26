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
 * @brief   OS-API 内部実装用のスレッドハンドルの型宣言（Windows 版）
 */

#pragma once

#include <nn/os/os_Config.h>

#if !defined(NN_BUILD_CONFIG_OS_SUPPORTS_WIN32)
    #error  "OS 種別として Win32 が指定されていません。"
#endif

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

//--------------------------------------------------------------------------

namespace nn { namespace os {
namespace detail {

//--------------------------------------------------------------------------
//  ThreadHandle の Windows 実装
//

struct InternalThreadHandle
{
    void*       _win32Handle;
    int         _idealCore;
    nn::Bit64   _affinityMask;
};

}   // namespace detail
}}  // namespace nn::os

