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
*   @file
*   @brief  ブレークポイント操作を行う API の内部実装の定義
*/

#pragma once

#include <nn/TargetConfigs/build_Compiler.h>

#if defined(NN_BUILD_CONFIG_COMPILER_CLANG)
#define NN_DETAIL_DIAG_BREAK()  __builtin_debugtrap()
#elif defined(NN_BUILD_CONFIG_COMPILER_GCC)
#define NN_DETAIL_DIAG_BREAK()  __builtin_trap()
#else
#error  "未サポートのコンパイラー種別が指定されています。"
#endif
