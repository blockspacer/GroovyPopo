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

/**
* @file
* @brief   diag API の内部実装の分岐に使用するマクロの定義
*/

#if defined(NN_SDK_BUILD_DEBUG) || defined(NN_SDK_BUILD_DEVELOP)
    #define NN_DETAIL_ENABLE_SDK_ASSERT
    #define NN_DETAIL_ENABLE_SDK_LOG
    #define NN_DETAIL_DIAG_ENABLE_TRANSLATION
#elif defined(NN_SDK_BUILD_RELEASE)

#else
    #error "ビルドタイプマクロが定義されていません。"
#endif

#if defined(NN_ENABLE_ABORT_MESSAGE) && defined(NN_DISABLE_ABORT_MESSAGE)
    #error "NN_ENABLE_ABORT_MESSAGE と NN_DISABLE_ABORT_MESSAGE を同時に定義することはできません。"
#elif defined(NN_ENABLE_ABORT_MESSAGE)
    #define NN_DETAIL_ENABLE_ABORT_MESSAGE
#elif defined(NN_DISABLE_ABORT_MESSAGE)

#else
    #if defined(NN_SDK_BUILD_DEBUG) || defined(NN_SDK_BUILD_DEVELOP)
        #define NN_DETAIL_ENABLE_ABORT_MESSAGE
    #elif defined(NN_SDK_BUILD_RELEASE)

    #else
        #error "ビルドタイプマクロが定義されていません。"
    #endif
#endif

#if defined(NN_ENABLE_ASSERT) && defined(NN_DISABLE_ASSERT)
    #error "NN_ENABLE_ASSERT と NN_DISABLE_ASSERT を同時に定義することはできません。"
#elif defined(NN_ENABLE_ASSERT)
    #define NN_DETAIL_ENABLE_ASSERT
#elif defined(NN_DISABLE_ASSERT)

#else
    #define NN_DETAIL_ENABLE_ASSERT
#endif

#if defined(NN_ENABLE_LOG) && defined(NN_DISABLE_LOG)
    #error "NN_ENABLE_LOG と NN_DISABLE_LOG を同時に定義することはできません。"
#elif defined(NN_ENABLE_LOG)
    #define NN_DETAIL_ENABLE_LOG
#elif defined(NN_DISABLE_LOG)

#else
    #define NN_DETAIL_ENABLE_LOG
#endif
