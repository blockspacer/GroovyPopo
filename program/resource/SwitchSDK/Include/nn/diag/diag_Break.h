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

#include <nn/TargetConfigs/build_Os.h>

/**
*   @file
*   @brief  ブレークポイント操作を行う API の宣言
*/

#if defined(NN_BUILD_CONFIG_OS_WIN32)
    #include <nn/diag/detail/diag_Break-os.win32.h>
#elif defined(NN_BUILD_CONFIG_OS_HORIZON)
    #include <nn/diag/detail/diag_Break-os.horizon.h>
#else
    #error "未サポートの OS 種別が指定されています。"
#endif

/**
* @brief 呼び出した箇所に、ブレークポイント命令を挿入します。
*/
#define NN_DIAG_BREAK() NN_DETAIL_DIAG_BREAK()
