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
 * @brief   OS 関連オブジェクトの定義で使用するマクロ定義
 */

#pragma once

#include <nn/os/os_Config.h>

//--------------------------------------------------------------------------

// 外部リンケージを C で宣言する
// 本マクロも将来的には NN_OS_DETAIL_EXTERN_C に修正すべきだが、
// os 以外での使用箇所が多いため、暫定的に残しておく。
#define NN_OS_EXTERN_C  extern "C"
