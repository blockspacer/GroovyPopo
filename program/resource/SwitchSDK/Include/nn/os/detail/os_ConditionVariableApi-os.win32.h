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
 * @brief   条件変数に関する Windows 依存のマクロの定義
 */

#pragma once

#if 1
//--------------------------------------------------------------------------
// 静的に初期化された ConditionVariableType オブジェクトを Windows 用に配置します。
// ::InitializeConditionVariable() 使用時相当の静的初期化版はこちら。
//
#define NN_OS_CONDITION_VARIABLE_INITIALIZER()                      \
        {                                                           \
            ::nn::os::ConditionVariableType::State_Initialized,     \
            0, /* means false for _needLazyInitialize */            \
            { NULL, NULL },                                         \
        }
//--------------------------------------------------------------------------
#else
//--------------------------------------------------------------------------
// 静的に初期化された ConditionVariableType オブジェクトを Windows 用に配置します。
// std::condition_variable_any 使用時の LazyInitialize 版はこちら。
//
#define NN_OS_CONDITION_VARIABLE_INITIALIZER()                      \
        {                                                           \
            ::nn::os::ConditionVariableType::State_NotInitialized,  \
            1, /* means true for _needLazyInitialize */             \
            { NULL, NULL },                                         \
        }
//--------------------------------------------------------------------------
#endif

