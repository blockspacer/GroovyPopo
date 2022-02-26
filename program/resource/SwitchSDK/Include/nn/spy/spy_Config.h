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

#include <nn/nn_Common.h>
#include <nn/htc.h>

#if defined(NN_ENABLE_SPY)
#error "NN_ENABLE_SPY マクロは spy_Config.h で定義しなければなりません。"
#endif

#if defined(NN_SDK_BUILD_LIBRARY) \
    || defined(NN_SDK_BUILD_DEBUG) \
    || defined(NN_SDK_BUILD_DEVELOP) \
    // || defined(NN_SDK_BUILD_RELEASE) /* リリースビルドで有効化 */
#define NN_ENABLE_SPY
#endif

#if defined(NN_ENABLE_SPY)
#if !defined(NN_BUILD_CONFIG_HTC_ENABLED)
#error "spy は htc に依存します。NN_ENABLE_HTC マクロを定義して htc を有効化してください。"
#endif
#define NN_BUILD_CONFIG_SPY_ENABLED
#endif

#ifdef NN_BUILD_CONFIG_SPY_ENABLED

// Spy デバッグスイッチ
//#define STATE_DEBUG_ENABLED
//#define BUFFER_DEBUG_ENABLED

#endif // NN_BUILD_CONFIG_SPY_ENABLED
