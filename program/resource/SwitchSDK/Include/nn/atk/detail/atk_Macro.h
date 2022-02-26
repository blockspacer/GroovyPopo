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

#include <nn/atk/fnd/string/atkfnd_Path.h>
#include <nn/atk/detail/atk_Log.h>

// atk 開発者向けの詳細な計測を有効化するマクロです。
//#define NN_ATK_ENABLE_DETAIL_PROFILE

#define NN_ATK_FILENAME (nn::atk::detail::fnd::TrimDirPath(nn::atk::detail::fnd::TrimDirPath(__FILE__, '\\'), '/'))
#define NN_ATK_WARNING(...)  { NN_DETAIL_ATK_WARN("%s:%d Warning: ", NN_ATK_FILENAME, __LINE__); NN_DETAIL_ATK_WARN(__VA_ARGS__); NN_DETAIL_ATK_WARN(" (%s)\n", __FUNCTION__); }

