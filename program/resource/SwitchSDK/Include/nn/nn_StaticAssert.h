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
* @brief   コンパイル時に条件式が成立することをテストする API です。
*/

#include <nn/nn_Common.h>
#include <nn/TargetConfigs/build_Compiler.h>

//! @cond
#if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_STATIC_ASSERT)

// コンパイル時に条件式が成立することをテストします。
#define NN_STATIC_ASSERT(condition) \
    static_assert(condition, "Static assertion failure: '" #condition "'")

#else /* NN_BUILD_CONFIG_COMPILER_SUPPORTS_STATIC_ASSERT */

namespace nn {
    template <bool> struct StaticAssertionFailure;
    template <> struct StaticAssertionFailure<true>
    {
    };

    namespace detail {
        template<int x> struct StaticAssertionTest
        {
        };
    }
}

// コンパイル時に条件式が成立することをテストします。
#define NN_STATIC_ASSERT(condition) \
    typedef ::nn::detail::StaticAssertionTest< \
        sizeof(::nn::StaticAssertionFailure<(condition) != 0>) > \
            NN_MAKE_TEMPORARY_NAME(NN_MACRO_CONCATENATE(nndetailStaticAssertionTypedef_, __LINE__))

#endif /* NN_BUILD_CONFIG_COMPILER_SUPPORTS_STATIC_ASSERT */
//! @endcond

#if defined(NN_BUILD_FOR_DOCUMENT_GENERATION)

/**
* @brief コンパイル時に条件式が成立することをテストします。
*
* @param[in] condition    コンパイル時定数である条件式
*
* @details
*   条件式 condition が true の場合、何も起こりません。
*   条件式 condition が false の場合、コンパイルエラーになります。
*/
#define NN_STATIC_ASSERT(condition)

#endif /* NN_BUILD_FOR_DOCUMENT_GENERATION */
