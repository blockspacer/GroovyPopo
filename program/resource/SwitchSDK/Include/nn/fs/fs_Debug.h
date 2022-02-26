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
*   @brief  FS ライブラリのデバッグ機能に関する公開ヘッダファイル
*/

#pragma once

#if defined(NN_BUILD_CONFIG_SPEC_NX)
#include <nn/fs/fs_SdCardForDebug.h>
#endif
#include <nn/fs/fs_RomForDebug.h>
#include <nn/fs/fs_SaveDataForDebug.h>
