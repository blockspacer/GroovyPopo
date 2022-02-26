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
* @brief htc API の実装分岐に使用するマクロの定義
*/

#if (defined NN_ENABLE_HTC) && (defined NN_DISABLE_HTC)
#error can not define NN_ENABLE_HTC and NN_DISABLE_HTC at the same time.
#endif

#if defined(NN_SDK_BUILD_DEBUG) || defined(NN_SDK_BUILD_DEVELOP)
    #if !defined (NN_DISABLE_HTC)
        #define NN_BUILD_CONFIG_HTC_ENABLED
    #endif
#elif defined(NN_SDK_BUILD_RELEASE)
    #if defined (NN_ENABLE_HTC)
        #define NN_BUILD_CONFIG_HTC_ENABLED
    #endif
#else
    #error "ビルドタイプマクロが定義されていません。"
#endif
