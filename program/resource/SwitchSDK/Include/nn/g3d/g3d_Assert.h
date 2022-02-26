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

#include <nn/nn_SdkLog.h>
#include <nn/nn_SdkAssert.h>
#include <nn/g3d/detail/g3d_Log.h>

// リソースクラスのポインターからリソース名を取得するマクロです。
#define NN_G3D_RES_GET_NAME(pRes, get_func) \
    (pRes)==NULL?"":(pRes)->get_func

// 前提条件をチェックします。リソースに名前があるなら、name に指定します。
#define NN_G3D_REQUIRES(condition, name) \
    NN_SDK_REQUIRES(condition, "Resource name : %s\n", name);

// 条件をチェックします。リソースに名前があるなら、name に指定します。
#define NN_G3D_ASSERT(condition, name) \
    NN_SDK_ASSERT(condition, "Resource name : %s\n", name);

// 範囲をチェックします。リソースに名前があるなら、name に指定します。
#define NN_G3D_REQUIRES_RANGE(value, min, max, name) \
    NN_SDK_REQUIRES(min <= value && value < max, "Resource name : %s\n", name);

// 範囲をチェックします。任意のメッセージを指定できます。
#define NN_G3D_REQUIRES_RANGE_DETAIL(value, min, max, ...) \
    NN_SDK_REQUIRES(min <= value && value < max, __VA_ARGS__);

//--------------------------------------------------------------------------------------------------

#define NN_G3D_NOT_IMPLEMENTED() \
    NN_SDK_ASSERT(0, "Not implemented yet.\n")

//--------------------------------------------------------------------------------------------------

#define NN_G3D_NOT_SUPPORTED() \
    NN_SDK_ASSERT(0, "Not supported.\n")

//--------------------------------------------------------------------------------------------------

#if defined(_DEBUG) || defined(NN_SDK_BUILD_DEBUG) || defined(NN_SDK_BUILD_DEVELOP)
#define NN_G3D_WARNING(exp, fmt, ...) NN_G3D_WARNING_IMPL(exp, "G3D WARNING : " fmt " (in %s at line %d)\n", ##__VA_ARGS__, NN_CURRENT_FUNCTION_NAME, __LINE__)
#define NN_G3D_WARNING_IMPL(exp, ...)         \
    do                                        \
    {                                         \
        if (!NN_STATIC_CONDITION(exp))        \
        {                                     \
            NN_DETAIL_G3D_WARN(__VA_ARGS__);  \
        }                                     \
    } while(NN_STATIC_CONDITION(0))
#else
#define NN_G3D_WARNING(exp, ... ) ((void)0)
#endif


