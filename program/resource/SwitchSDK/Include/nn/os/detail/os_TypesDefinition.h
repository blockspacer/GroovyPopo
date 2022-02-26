/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <nn/os/os_Config.h>

//  このマクロが定義されたら、正しいビルド条件
#undef  NN_OS_SELECT_TYPES_DEFINITION

//  各ターゲットの ABI 条件でオブジェクトのサイズを定義する
#if defined(NN_BUILD_CONFIG_ABI_ILP32)
    #include "os_TypesDefinition-abi.ilp32.h"
#elif defined(NN_BUILD_CONFIG_ABI_LP64)
    #include "os_TypesDefinition-abi.lp64.h"
#elif defined(NN_BUILD_CONFIG_OS_WIN32) && defined(NN_BUILD_CONFIG_ADDRESS_32)
    #include "os_TypesDefinition-os.win32.h"
#elif defined(NN_BUILD_CONFIG_OS_WIN32) && defined(NN_BUILD_CONFIG_ADDRESS_64)
    #include "os_TypesDefinition-os.win64.h"
#endif

#if !defined(NN_OS_SELECT_TYPES_DEFINITION)
    #error  "未サポートのビルド条件が指定されています。"
#endif

