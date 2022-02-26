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

// リリースビルド時は nn::ui2d::viewer の機能は無効です。
//
// コンパイルオプションで NN_UI2D_VIEWER_{ENABLE,DISABLE} を定義することで
// デフォルトの設定を変更できます。
//
#if !defined(NN_UI2D_VIEWER_ENABLED) && \
    !defined(NN_UI2D_VIEWER_DISABLE)
#define NN_UI2D_VIEWER_ENABLED
#endif
