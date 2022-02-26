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
* @brief   SDK 用の、条件を満たしていることをテストする API の宣言
*/

#include <nn/nn_Common.h>
#include <nn/TargetConfigs/build_Compiler.h>
#include <nn/diag/detail/diag_DetailBuildSwitch.h>
#include <nn/diag/detail/diag_DetailAssert.h>
#include <nn/diag/detail/diag_DetailNamedAssert.h>

#if defined(NN_DETAIL_ENABLE_SDK_ASSERT)

#define NN_DETAIL_SDK_ASSERT_IMPL(condition, conditionString, ...) \
    NN_DETAIL_ASSERT_COMMON_IMPL(SdkAssert, condition, conditionString, ##__VA_ARGS__)

#define NN_DETAIL_SDK_REQUIRES_IMPL(condition, conditionString, ...) \
    NN_DETAIL_ASSERT_COMMON_IMPL(SdkRequires, condition, conditionString, ##__VA_ARGS__)

#if !defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE)

#define NN_DETAIL_SDK_ASSERT_IMPL_WITHOUT_AUTO_TYPE(condition, conditionString, ...) \
    NN_DETAIL_ASSERT_COMMON_IMPL(SdkAssert, condition, conditionString)

#define NN_DETAIL_SDK_REQUIRES_IMPL_WITHOUT_AUTO_TYPE(condition, conditionString, ...) \
    NN_DETAIL_ASSERT_COMMON_IMPL(SdkRequires, condition, conditionString)

#endif // !defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE)

#define NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_1( \
    assertImplMacroName, \
    namedAssertImplMacroName, \
    arg1, \
    arg1Stringized) \
    NN_DETAIL_NAMED_ASSERT_IMPL_1( \
        assertImplMacroName, \
        namedAssertImplMacroName, \
        arg1, \
        arg1Stringized)

#define NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
    assertImplMacroName, \
    namedAssertImplMacroName, \
    arg1, \
    arg2, \
    arg1Stringized, \
    arg2Stringized) \
    NN_DETAIL_NAMED_ASSERT_IMPL_2( \
        assertImplMacroName, \
        namedAssertImplMacroName, \
        arg1, \
        arg2, \
        arg1Stringized, \
        arg2Stringized)

#define NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_3( \
    assertImplMacroName, \
    namedAssertImplMacroName, \
    arg1, \
    arg2, \
    arg3, \
    arg1Stringized, \
    arg2Stringized, \
    arg3Stringized) \
    NN_DETAIL_NAMED_ASSERT_IMPL_3( \
        assertImplMacroName, \
        namedAssertImplMacroName, \
        arg1, \
        arg2, \
        arg3, \
        arg1Stringized, \
        arg2Stringized, \
        arg3Stringized)

#else // #if defined(NN_DETAIL_ENABLE_SDK_ASSERT)

#define NN_DETAIL_SDK_ASSERT_IMPL(condition, conditionString, ...) static_cast<void>(0)

#define NN_DETAIL_SDK_REQUIRES_IMPL(condition, conditionString, ...) static_cast<void>(0)

#define NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_1( \
    assertImplMacroName, \
    namedAssertImplMacroName, \
    arg1, \
    arg1Stringized) \
    static_cast<void>(0)

#define NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
    assertImplMacroName, \
    namedAssertImplMacroName, \
    arg1, \
    arg2, \
    arg1Stringized, \
    arg2Stringized) \
    static_cast<void>(0)

#define NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_3( \
    assertImplMacroName, \
    namedAssertImplMacroName, \
    arg1, \
    arg2, \
    arg3, \
    arg1Stringized, \
    arg2Stringized, \
    arg3Stringized) \
    static_cast<void>(0)

#endif // #if defined(NN_DETAIL_ENABLE_SDK_ASSERT)

/**
* @brief 条件を満たしていることをテストします。
*
* @param[in] condition    テストする条件
* @param[in] ...          条件を満たしていない場合にログとして出力するメッセージの書式指定文字列と引数
*/
#define NN_SDK_ASSERT(condition, ...) \
    NN_DETAIL_SDK_ASSERT_IMPL((condition), NN_MACRO_STRINGIZE(condition), ##__VA_ARGS__)

/**
* @brief 事前条件を満たしていることをテストします。
*
* @param[in] condition    テストする条件
* @param[in] ...          条件を満たしていない場合にログとして出力するメッセージの書式指定文字列と引数
*/
#define NN_SDK_REQUIRES(condition, ...) \
    NN_DETAIL_SDK_REQUIRES_IMPL((condition), NN_MACRO_STRINGIZE(condition), ##__VA_ARGS__)


/**
* @brief ポインタがヌルポインタではないことをテストします。
*        満たされるべき条件は pointer != nullptr です。
*/
#define NN_SDK_ASSERT_NOT_NULL(pointer) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_1( \
        NN_DETAIL_SDK_ASSERT_IMPL, NN_DETAIL_ASSERT_NOT_NULL_IMPL, \
        (pointer), NN_MACRO_STRINGIZE(pointer))

/**
* @brief 値が等しいことをテストします。
*        満たされるべき条件は lhs == rhs です。
*/
#define NN_SDK_ASSERT_EQUAL(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_ASSERT_IMPL, NN_DETAIL_ASSERT_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 値が等しくないことをテストします。
*        満たされるべき条件は lhs != rhs です。
*/
#define NN_SDK_ASSERT_NOT_EQUAL(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_ASSERT_IMPL, NN_DETAIL_ASSERT_NOT_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 左辺が右辺より小さいことをテストします。
*        満たされるべき条件は lhs < rhs です。
*/
#define NN_SDK_ASSERT_LESS(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_ASSERT_IMPL, NN_DETAIL_ASSERT_LESS_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 左辺が右辺より小さいか、または値が等しいことをテストします。
*        満たされるべき条件は lhs <= rhs です。
*/
#define NN_SDK_ASSERT_LESS_EQUAL(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_ASSERT_IMPL, NN_DETAIL_ASSERT_LESS_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 左辺が右辺より大きいことをテストします。
*        満たされるべき条件は lhs > rhs です。
*/
#define NN_SDK_ASSERT_GREATER(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_ASSERT_IMPL, NN_DETAIL_ASSERT_GREATER_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 左辺が右辺より大きいか、または値が等しいことをテストします。
*        満たされるべき条件は lhs >= rhs です。
*/
#define NN_SDK_ASSERT_GREATER_EQUAL(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_ASSERT_IMPL, NN_DETAIL_ASSERT_GREATER_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 与えられたアドレス・ポインタのアラインメントをテストします。
*        満たされるべき条件は pointer & (align - 1) == 0 です。
*
* @pre   align は 2 のべき乗である必要があります。
*/
#define NN_SDK_ASSERT_ALIGNED(pointer, align) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_ASSERT_IMPL, NN_DETAIL_ASSERT_ALIGNED_IMPL, \
        (pointer), (align), NN_MACRO_STRINGIZE(pointer), NN_MACRO_STRINGIZE(align))

/**
* @brief C文字列の内容が等しいことをテストします。
*        満たされるべき条件は strcmp(lhs, rhs) == 0 です。
*
* @pre   lhs, rhs はヌルでない必要があります。
*/
#define NN_SDK_ASSERT_STRING_EQUAL(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_ASSERT_IMPL, NN_DETAIL_ASSERT_STRING_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief C文字列の内容が等しくないことをテストします。
*        満たされるべき条件は strcmp(lhs, rhs) != 0 です。
*
* @pre   lhs, rhs はヌルでない必要があります。
*/
#define NN_SDK_ASSERT_STRING_NOT_EQUAL(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_ASSERT_IMPL, NN_DETAIL_ASSERT_STRING_NOT_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 値が範囲に含まれていることをテストします。
*        満たされるべき条件は (begin <= value) && (value < end) です。
*/
#define NN_SDK_ASSERT_RANGE(value, begin, end) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_3( \
        NN_DETAIL_SDK_ASSERT_IMPL, NN_DETAIL_ASSERT_RANGE_IMPL, \
        (value), (begin), (end), NN_MACRO_STRINGIZE(value), NN_MACRO_STRINGIZE(begin), NN_MACRO_STRINGIZE(end))

/**
* @brief 値が最小値以上かつ最大値以下であることをテストします。
*        満たされるべき条件は (min <= value) && (value <= max) です。
*/
#define NN_SDK_ASSERT_MINMAX(value, min, max) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_3( \
        NN_DETAIL_SDK_ASSERT_IMPL, NN_DETAIL_ASSERT_MINMAX_IMPL, \
        (value), (min), (max), NN_MACRO_STRINGIZE(value), NN_MACRO_STRINGIZE(min), NN_MACRO_STRINGIZE(max))

/**
* @brief ポインタがヌルポインタではないことをテストします。
*        満たされるべき条件は pointer != nullptr です。
*/
#define NN_SDK_REQUIRES_NOT_NULL(pointer) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_1( \
        NN_DETAIL_SDK_REQUIRES_IMPL, NN_DETAIL_ASSERT_NOT_NULL_IMPL, \
        (pointer), NN_MACRO_STRINGIZE(pointer))

/**
* @brief 値が等しいことをテストします。
*        満たされるべき条件は lhs == rhs です。
*/
#define NN_SDK_REQUIRES_EQUAL(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_REQUIRES_IMPL, NN_DETAIL_ASSERT_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 値が等しくないことをテストします。
*        満たされるべき条件は lhs != rhs です。
*/
#define NN_SDK_REQUIRES_NOT_EQUAL(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_REQUIRES_IMPL, NN_DETAIL_ASSERT_NOT_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 左辺が右辺より小さいことをテストします。
*        満たされるべき条件は lhs < rhs です。
*/
#define NN_SDK_REQUIRES_LESS(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_REQUIRES_IMPL, NN_DETAIL_ASSERT_LESS_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 左辺が右辺より小さいか、または値が等しいことをテストします。
*        満たされるべき条件は lhs <= rhs です。
*/
#define NN_SDK_REQUIRES_LESS_EQUAL(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_REQUIRES_IMPL, NN_DETAIL_ASSERT_LESS_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 左辺が右辺より大きいことをテストします。
*        満たされるべき条件は lhs > rhs です。
*/
#define NN_SDK_REQUIRES_GREATER(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_REQUIRES_IMPL, NN_DETAIL_ASSERT_GREATER_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 左辺が右辺より大きいか、または値が等しいことをテストします。
*        満たされるべき条件は lhs >= rhs です。
*/
#define NN_SDK_REQUIRES_GREATER_EQUAL(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_REQUIRES_IMPL, NN_DETAIL_ASSERT_GREATER_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 与えられたアドレス・ポインタのアラインメントをテストします。
*        満たされるべき条件は pointer & (align - 1) == 0 です。
*
* @pre   align は 2 のべき乗である必要があります。
*/
#define NN_SDK_REQUIRES_ALIGNED(pointer, align) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_REQUIRES_IMPL, NN_DETAIL_ASSERT_ALIGNED_IMPL, \
        (pointer), (align), NN_MACRO_STRINGIZE(pointer), NN_MACRO_STRINGIZE(align))

/**
* @brief C文字列の内容が等しいことをテストします。
*        満たされるべき条件は strcmp(lhs, rhs) == 0 です。
*
* @pre   lhs, rhs はヌルでない必要があります。
*/
#define NN_SDK_REQUIRES_STRING_EQUAL(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_REQUIRES_IMPL, NN_DETAIL_ASSERT_STRING_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief C文字列の内容が等しくないことをテストします。
*        満たされるべき条件は strcmp(lhs, rhs) != 0 です。
*
* @pre   lhs, rhs はヌルでない必要があります。
*/
#define NN_SDK_REQUIRES_STRING_NOT_EQUAL(lhs, rhs) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_2( \
        NN_DETAIL_SDK_REQUIRES_IMPL, NN_DETAIL_ASSERT_STRING_NOT_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 値が範囲に含まれていることをテストします。
*        満たされるべき条件は (begin <= value) && (value < end) です。
*/
#define NN_SDK_REQUIRES_RANGE(value, begin, end) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_3( \
        NN_DETAIL_SDK_REQUIRES_IMPL, NN_DETAIL_ASSERT_RANGE_IMPL, \
        (value), (begin), (end), NN_MACRO_STRINGIZE(value), NN_MACRO_STRINGIZE(begin), NN_MACRO_STRINGIZE(end))

/**
* @brief 値が最小値以上かつ最大値以下であることをテストします。
*        満たされるべき条件は (min <= value) && (value <= max) です。
*/
#define NN_SDK_REQUIRES_MINMAX(value, min, max) \
    NN_DETAIL_SDK_INVOKE_NAMED_ASSERT_3( \
        NN_DETAIL_SDK_REQUIRES_IMPL, NN_DETAIL_ASSERT_MINMAX_IMPL, \
        (value), (min), (max), NN_MACRO_STRINGIZE(value), NN_MACRO_STRINGIZE(min), NN_MACRO_STRINGIZE(max))

/*
* diag_DetailAssert.h が util のインライン関数を参照しますが、
* util もアサートマクロを参照するため、マクロ定義の後にでインクルードします。
*/
#include <nn/util/util_BitUtil.h>
