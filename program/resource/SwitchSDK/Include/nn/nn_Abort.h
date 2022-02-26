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
* @brief   条件を満たしていない時に停止するための API の宣言
*/

#include <nn/nn_Common.h>
#include <nn/diag/detail/diag_DetailAssert.h>
#include <nn/diag/detail/diag_DetailNamedAssert.h>

/**
* @brief 実行を停止します。このマクロは常に有効です。
*
* @param[in] ...          ログとして出力するメッセージの書式指定文字列と引数
*/
#define NN_ABORT(...) \
    NN_DETAIL_ABORT_UNLESS_IMPL(false, "NN_ABORT", ##__VA_ARGS__)

/**
* @brief 条件を満たしていることをテストし、満たされていない場合に停止します。このマクロは常に有効です。
*
* @param[in] condition    テストする条件
* @param[in] ...          条件を満たしていない場合にログとして出力するメッセージの書式指定文字列と引数
*/
#define NN_ABORT_UNLESS(condition, ...) \
    NN_DETAIL_ABORT_UNLESS_IMPL((condition), NN_MACRO_STRINGIZE(condition), ##__VA_ARGS__)

/**
* @brief ポインタがヌルポインタではないことをテストし、そうではない場合に停止します。
*        満たされるべき条件は pointer != nullptr です。
*        このマクロは常に有効です。
*/
#define NN_ABORT_UNLESS_NOT_NULL(pointer) \
    NN_DETAIL_NAMED_ASSERT_IMPL_1( \
        NN_DETAIL_ABORT_UNLESS_IMPL, NN_DETAIL_ASSERT_NOT_NULL_IMPL, \
        (pointer), NN_MACRO_STRINGIZE(pointer))

/**
* @brief 値が等しいことをテストし、そうではない場合に停止します。
*        満たされるべき条件は lhs == rhs です。
*        このマクロは常に有効です。
*/
#define NN_ABORT_UNLESS_EQUAL(lhs, rhs) \
    NN_DETAIL_NAMED_ASSERT_IMPL_2( \
        NN_DETAIL_ABORT_UNLESS_IMPL, NN_DETAIL_ASSERT_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 値が等しくないことをテストし、そうではない場合に停止します。
*        満たされるべき条件は lhs != rhs です。
*        このマクロは常に有効です。
*/
#define NN_ABORT_UNLESS_NOT_EQUAL(lhs, rhs) \
    NN_DETAIL_NAMED_ASSERT_IMPL_2( \
        NN_DETAIL_ABORT_UNLESS_IMPL, NN_DETAIL_ASSERT_NOT_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 左辺が右辺より小さいことをテストします。
*        満たされるべき条件は lhs < rhs です。
*/
#define NN_ABORT_UNLESS_LESS(lhs, rhs) \
    NN_DETAIL_NAMED_ASSERT_IMPL_2( \
        NN_DETAIL_ABORT_UNLESS_IMPL, NN_DETAIL_ASSERT_LESS_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 左辺が右辺より小さいか、または値が等しいことをテストします。
*        満たされるべき条件は lhs <= rhs です。
*/
#define NN_ABORT_UNLESS_LESS_EQUAL(lhs, rhs) \
    NN_DETAIL_NAMED_ASSERT_IMPL_2( \
        NN_DETAIL_ABORT_UNLESS_IMPL, NN_DETAIL_ASSERT_LESS_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 左辺が右辺より大きいことをテストします。
*        満たされるべき条件は lhs > rhs です。
*/
#define NN_ABORT_UNLESS_GREATER(lhs, rhs) \
    NN_DETAIL_NAMED_ASSERT_IMPL_2( \
        NN_DETAIL_ABORT_UNLESS_IMPL, NN_DETAIL_ASSERT_GREATER_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))
/**
* @brief 左辺が右辺より大きいか、または値が等しいことをテストします。
*        満たされるべき条件は lhs >= rhs です。
*/
#define NN_ABORT_UNLESS_GREATER_EQUAL(lhs, rhs) \
    NN_DETAIL_NAMED_ASSERT_IMPL_2( \
        NN_DETAIL_ABORT_UNLESS_IMPL, NN_DETAIL_ASSERT_GREATER_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 与えられたアドレス・ポインタのアラインメントをテストします。
*        満たされるべき条件は pointer & (align - 1) == 0 です。
*
* @pre   align は 2 のべき乗である必要があります。
*/
#define NN_ABORT_UNLESS_ALIGNED(pointer, align) \
    NN_DETAIL_NAMED_ASSERT_IMPL_2( \
        NN_DETAIL_ABORT_UNLESS_IMPL, NN_DETAIL_ASSERT_ALIGNED_IMPL, \
        (pointer), (align), NN_MACRO_STRINGIZE(pointer), NN_MACRO_STRINGIZE(align))

/**
* @brief C文字列の内容が等しいことをテストします。
*        満たされるべき条件は strcmp(lhs, rhs) == 0 です。
*
* @pre   lhs, rhs はヌルでない必要があります。
*/
#define NN_ABORT_UNLESS_STRING_EQUAL(lhs, rhs) \
    NN_DETAIL_NAMED_ASSERT_IMPL_2( \
        NN_DETAIL_ABORT_UNLESS_IMPL, NN_DETAIL_ASSERT_STRING_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief C文字列の内容が等しくないことをテストします。
*        満たされるべき条件は strcmp(lhs, rhs) != 0 です。
*
* @pre   lhs, rhs はヌルでない必要があります。
*/
#define NN_ABORT_UNLESS_STRING_NOT_EQUAL(lhs, rhs) \
    NN_DETAIL_NAMED_ASSERT_IMPL_2( \
        NN_DETAIL_ABORT_UNLESS_IMPL, NN_DETAIL_ASSERT_STRING_NOT_EQUAL_IMPL, \
        (lhs), (rhs), NN_MACRO_STRINGIZE(lhs), NN_MACRO_STRINGIZE(rhs))

/**
* @brief 値が範囲に含まれていることをテストし、そうではない場合に停止します。
*        満たされるべき条件は (begin <= value) && (value < end) です。
*        このマクロは常に有効です。
*/
#define NN_ABORT_UNLESS_RANGE(value, begin, end) \
    NN_DETAIL_NAMED_ASSERT_IMPL_3( \
        NN_DETAIL_ABORT_UNLESS_IMPL, NN_DETAIL_ASSERT_RANGE_IMPL, \
        (value), (begin), (end), NN_MACRO_STRINGIZE(value), NN_MACRO_STRINGIZE(begin), NN_MACRO_STRINGIZE(end))

/**
* @brief 値が最小値以上かつ最大値以下であることをテストし、そうではない場合に停止します。
*        満たされるべき条件は (min <= value) && (value <= max) です。
*        このマクロは常に有効です。
*/
#define NN_ABORT_UNLESS_MINMAX(value, min, max) \
    NN_DETAIL_NAMED_ASSERT_IMPL_3( \
        NN_DETAIL_ABORT_UNLESS_IMPL, NN_DETAIL_ASSERT_MINMAX_IMPL, \
        (value), (min), (max), NN_MACRO_STRINGIZE(value), NN_MACRO_STRINGIZE(min), NN_MACRO_STRINGIZE(max))

/**
* @brief 指定した nn::Result に対して成功であるかどうかをテストし、そうではない場合に停止します。
*        満たされるべき条件は result.IsSuccess() です。
*        このマクロは常に有効です。
*/
#define NN_ABORT_UNLESS_RESULT_SUCCESS(result) \
    NN_DETAIL_NAMED_ASSERT_IMPL_1( \
        NN_DETAIL_ABORT_UNLESS_RESULT_SUCCESS_IMPL, NN_DETAIL_ASSERT_RESULT_SUCCESS_IMPL, \
        (static_cast<nn::Result>(result)), NN_MACRO_STRINGIZE(result))

//NN_DETAIL_ABORT_UNLESS_IMPL, NN_DETAIL_ASSERT_RESULT_SUCCESS_IMPL,


/*
* diag_DetailAssert.h が util のインライン関数を参照しますが、
* util もアサートマクロを参照するため、マクロ定義の後にでインクルードします。
*/
#include <nn/util/util_BitUtil.h>
