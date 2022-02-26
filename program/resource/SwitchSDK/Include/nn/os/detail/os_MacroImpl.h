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
#include <nn/nn_Macro.h>

//--------------------------------------------------------------------------

#if defined(NN_BUILD_CONFIG_COMPILER_CLANG) || \
    defined(NN_BUILD_CONFIG_COMPILER_GCC)   || \
    defined(NN_BUILD_CONFIG_COMPILER_VC)

#include <type_traits>
#undef  NN_BUILD_CONFIG_COMPILER_SUPPORTS_STD_IS_TRIVIAL
#define NN_BUILD_CONFIG_COMPILER_SUPPORTS_STD_IS_TRIVIAL    1

#endif  // CLANG or GCC or VC++


//  name で指定された型が trivial かどうかをコンパイル時に判定する
#if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_STD_IS_TRIVIAL)
    #define NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(name) \
            static_assert(std::is_trivial<name>::value, #name" is non-trivial.")
    #define NN_OS_DETAIL_STATIC_ASSERT_TRIVIALLY_DESTRUCTIBLE(name) \
            static_assert(std::is_trivially_destructible<name>::value, #name" is non-trivially-destructible.")
#else
    #define NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(name)
    #define NN_OS_DETAIL_STATIC_ASSERT_TRIVIALLY_DESTRUCTIBLE(name)
#endif

//  cppname 同等のサイズとアライメントを持つオブジェクトを cname で定義する
#define NN_OS_DETAIL_DEFINE_ALIGNED_STORAGE(cname, cppname, size, alignment_type)  \
        typedef struct cname                                                \
        {                                                                   \
            union                                                           \
            {                                                               \
                uint8_t         buf[size];                                  \
                alignment_type  alignment_holder;                           \
            };                                                              \
        } cname

//  構造体のサイズを ILP32/LP64 それぞれのビルド条件に合わせて算出するマクロ。
//  baseSize は ILP32/LP64 で変化しない部分のサイズ、
//  pointerNum はポインタ変数の数を指定する。
#define NN_OS_DETAIL_CALC_OBJECT_SIZE(baseSize, pointerNum)    \
                                ((baseSize) + (pointerNum) * sizeof(void*))

#if defined(NN_BUILD_CONFIG_ADDRESS_32)
    #define NN_OS_DETAIL_PADDING_POINTER    NN_PADDING4
#elif defined(NN_BUILD_CONFIG_ADDRESS_64)
    #define NN_OS_DETAIL_PADDING_POINTER    NN_PADDING8
#endif
