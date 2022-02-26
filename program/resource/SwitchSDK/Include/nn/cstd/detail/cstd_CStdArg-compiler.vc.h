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

// <cstdarg> 関連のワークアラウンド（VC++ 向け）

#include <nn/cstd/cstd_Config.h>
#if !defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_VC)
    #error  "VC++ コンパイラーが利用出来るビルド条件ではありません。"
#endif

#include <cstdarg>

// VC++ 2012 には va_copy が存在しないため、独自に定義します。
#if !defined(va_copy)
#define NN_CSTD_VA_COPY(dst, src) ((dst) = (src))
#else
#define NN_CSTD_VA_COPY(dst, src) va_copy(dst, src)
#endif
