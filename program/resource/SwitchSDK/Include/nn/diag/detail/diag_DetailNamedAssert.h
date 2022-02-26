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
* @brief   意味づけされた Assert の実装マクロです。
*/

#include <cstddef>
#include <cstring>

#include <nn/nn_Common.h>
#include <nn/diag/detail/diag_DetailAssert.h>

/*
* util のインライン関数の前方宣言です。
* このヘッダファイルをインクルードする際は、util_BitUtil.h もインクルードする必要があります。
*/
namespace nn { namespace util {
    template< typename T >
    inline bool is_aligned( T x, size_t align ) NN_NOEXCEPT;
}}

#if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE)

#define NN_DETAIL_NAMED_ASSERT_IMPL_1( \
    assertImplMacroName, \
    namedAssertImplMacroName, \
    arg1, \
    arg1Stringized) \
    do \
    { \
        const auto& nnDetailAssertArgValue_arg1(arg1); \
        namedAssertImplMacroName( \
            assertImplMacroName, \
            nnDetailAssertArgValue_arg1, \
            arg1Stringized); \
    } while (NN_STATIC_CONDITION(0))

#define NN_DETAIL_NAMED_ASSERT_IMPL_2( \
    assertImplMacroName, \
    namedAssertImplMacroName, \
    arg1, \
    arg2, \
    arg1Stringized, \
    arg2Stringized) \
    do \
    { \
        const auto& nnDetailAssertArgValue_arg1(arg1); \
        const auto& nnDetailAssertArgValue_arg2(arg2); \
        namedAssertImplMacroName( \
            assertImplMacroName, \
            nnDetailAssertArgValue_arg1, \
            nnDetailAssertArgValue_arg2, \
            arg1Stringized, \
            arg2Stringized); \
    } while (NN_STATIC_CONDITION(0))

#define NN_DETAIL_NAMED_ASSERT_IMPL_3( \
    assertImplMacroName, \
    namedAssertImplMacroName, \
    arg1, \
    arg2, \
    arg3, \
    arg1Stringized, \
    arg2Stringized, \
    arg3Stringized) \
    do \
    { \
        const auto& nnDetailAssertArgValue_arg1(arg1); \
        const auto& nnDetailAssertArgValue_arg2(arg2); \
        const auto& nnDetailAssertArgValue_arg3(arg3); \
        namedAssertImplMacroName( \
            assertImplMacroName, \
            nnDetailAssertArgValue_arg1, \
            nnDetailAssertArgValue_arg2, \
            nnDetailAssertArgValue_arg3, \
            arg1Stringized, \
            arg2Stringized, \
            arg3Stringized); \
    } while (NN_STATIC_CONDITION(0))

#else // defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE)

#define NN_DETAIL_NAMED_ASSERT_IMPL_1( \
    assertImplMacroName, \
    namedAssertImplMacroName, \
    arg1, \
    arg1Stringized) \
    do \
    { \
        namedAssertImplMacroName( \
            assertImplMacroName##_WITHOUT_AUTO_TYPE, \
            (arg1), \
            arg1Stringized); \
    } while (NN_STATIC_CONDITION(0))

#define NN_DETAIL_NAMED_ASSERT_IMPL_2( \
    assertImplMacroName, \
    namedAssertImplMacroName, \
    arg1, \
    arg2, \
    arg1Stringized, \
    arg2Stringized) \
    do \
    { \
        namedAssertImplMacroName( \
            assertImplMacroName##_WITHOUT_AUTO_TYPE, \
            (arg1), \
            (arg2), \
            arg1Stringized, \
            arg2Stringized); \
    } while (NN_STATIC_CONDITION(0))

#define NN_DETAIL_NAMED_ASSERT_IMPL_3( \
    assertImplMacroName, \
    namedAssertImplMacroName, \
    arg1, \
    arg2, \
    arg3, \
    arg1Stringized, \
    arg2Stringized, \
    arg3Stringized) \
    do \
    { \
        namedAssertImplMacroName( \
            assertImplMacroName##_WITHOUT_AUTO_TYPE, \
            (arg1), \
            (arg2), \
            (arg3), \
            arg1Stringized, \
            arg2Stringized, \
            arg3Stringized); \
    } while (NN_STATIC_CONDITION(0))

#endif // !defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE)

namespace nn { namespace diag { namespace detail {

    template <typename T>
    inline bool NotNull(const T& pointer) NN_NOEXCEPT
    {
#if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_NULLPTR)
        return pointer != nullptr;
#else
        return pointer != NULL;
#endif
    }

    bool NotNullImpl(uintptr_t address) NN_NOEXCEPT;

    template <typename T>
    inline bool NotNull(T* pointer) NN_NOEXCEPT
    {
        return NotNullImpl(reinterpret_cast<uintptr_t>(pointer));
    }

    template <typename T1, typename T2>
    inline bool Equal(const T1& lhs, const T2& rhs) NN_NOEXCEPT
    {
        return lhs == rhs;
    }

    template <typename T1, typename T2>
    inline bool NotEqual(const T1& lhs, const T2& rhs) NN_NOEXCEPT
    {
        return lhs != rhs;
    }

    template <typename T1, typename T2>
    inline bool Less(const T1& lhs, const T2& rhs) NN_NOEXCEPT
    {
        return lhs < rhs;
    }

    template <typename T1, typename T2>
    inline bool LessEqual(const T1& lhs, const T2& rhs) NN_NOEXCEPT
    {
        return lhs <= rhs;
    }

    template <typename T1, typename T2>
    inline bool Greater(const T1& lhs, const T2& rhs) NN_NOEXCEPT
    {
        return lhs > rhs;
    }

    template <typename T1, typename T2>
    inline bool GreaterEqual(const T1& lhs, const T2& rhs) NN_NOEXCEPT
    {
        return lhs >= rhs;
    }

#if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_NULLPTR)
    inline bool IsAligned(std::nullptr_t, size_t)
    {
        return true;
    }
#endif // #if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_NULLPTR)

    template <typename T>
    inline bool IsAligned(T* ptr, size_t align)
    {
        return nn::util::is_aligned<uintptr_t>(reinterpret_cast<uintptr_t>(ptr), align);
    }

    template <typename T>
    inline bool IsAligned(T address, size_t align)
    {
        return nn::util::is_aligned<T>(address, align);
    }

#if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_NULLPTR)
    inline bool StringEqual(std::nullptr_t, std::nullptr_t) NN_NOEXCEPT
    {
        return false;
    }

    inline bool StringEqual(std::nullptr_t, const char*) NN_NOEXCEPT
    {
        return false;
    }

    inline bool StringEqual(const char*, std::nullptr_t) NN_NOEXCEPT
    {
        return false;
    }
#endif // #if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_NULLPTR)

    inline bool StringEqual(const char* lhs, const char* rhs) NN_NOEXCEPT
    {
        return std::strcmp(lhs, rhs) == 0;
    }

#if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_NULLPTR)
    inline bool StringNotEqual(std::nullptr_t, std::nullptr_t) NN_NOEXCEPT
    {
        return false;
    }

    inline bool StringNotEqual(std::nullptr_t, const char*) NN_NOEXCEPT
    {
        return false;
    }

    inline bool StringNotEqual(const char*, std::nullptr_t) NN_NOEXCEPT
    {
        return false;
    }
#endif // #if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_NULLPTR)

    inline bool StringNotEqual(const char* lhs, const char* rhs) NN_NOEXCEPT
    {
        return std::strcmp(lhs, rhs) != 0;
    }

    template <typename T1, typename T2, typename T3>
    inline bool WithinRange(const T1& value, const T2& begin, const T3& end) NN_NOEXCEPT
    {
        return (begin <= value) && (value < end);
    }

    template <typename T1, typename T2, typename T3>
    inline bool WithinMinMax(const T1& value, const T2& min, const T3& max) NN_NOEXCEPT
    {
        return (min <= value) && (value <= max);
    }
}}}

// ヌルポインタではないことのテストです。
#define NN_DETAIL_ASSERT_NOT_NULL_IMPL(assertImplMacroName, pointer, pointerStringized) \
    assertImplMacroName( \
        ::nn::diag::detail::NotNull(pointer), \
        "NotNull", \
        "%s must not be nullptr.", \
        pointerStringized)

// 等しいことのテストです。
// 現状では、タイプセーフなフォーマットの仕組みがないため、実際の値を表示できません。
#define NN_DETAIL_ASSERT_EQUAL_IMPL(assertImplMacroName, lhs, rhs, lhsStringized, rhsStringized) \
    assertImplMacroName( \
        ::nn::diag::detail::Equal(lhs, rhs), \
        "Equal", \
        "%s must be equal to %s.", \
        lhsStringized, \
        rhsStringized)

// 等しくないことのテストです。
// 現状では、タイプセーフなフォーマットの仕組みがないため、実際の値を表示できません。
#define NN_DETAIL_ASSERT_NOT_EQUAL_IMPL(assertImplMacroName, lhs, rhs, lhsStringized, rhsStringized) \
    assertImplMacroName( \
        ::nn::diag::detail::NotEqual(lhs, rhs), \
        "NotEqual", \
        "%s must not be equal to %s.", \
        lhsStringized, \
        rhsStringized)

// 左辺が右辺より小さいことのテストです。
// 現状では、タイプセーフなフォーマットの仕組みがないため、実際の値を表示できません。
#define NN_DETAIL_ASSERT_LESS_IMPL(assertImplMacroName, lhs, rhs, lhsStringized, rhsStringized) \
    assertImplMacroName( \
        ::nn::diag::detail::Less(lhs, rhs), \
        "Less", \
        "%s must be less than %s.", \
        lhsStringized, \
        rhsStringized)

// 左辺が右辺より小さいか、または等しいことのテストです。
// 現状では、タイプセーフなフォーマットの仕組みがないため、実際の値を表示できません。
#define NN_DETAIL_ASSERT_LESS_EQUAL_IMPL(assertImplMacroName, lhs, rhs, lhsStringized, rhsStringized) \
    assertImplMacroName( \
        ::nn::diag::detail::LessEqual(lhs, rhs), \
        "LessEqual", \
        "%s must be less than or equal to %s.", \
        lhsStringized, \
        rhsStringized)

// 左辺が右辺より大きいことのテストです。
// 現状では、タイプセーフなフォーマットの仕組みがないため、実際の値を表示できません。
#define NN_DETAIL_ASSERT_GREATER_IMPL(assertImplMacroName, lhs, rhs, lhsStringized, rhsStringized) \
    assertImplMacroName( \
        ::nn::diag::detail::Greater(lhs, rhs), \
        "Greater", \
        "%s must be greater than %s.", \
        lhsStringized, \
        rhsStringized)

// 左辺が右辺より大きいか、または等しいことのテストです。
// 現状では、タイプセーフなフォーマットの仕組みがないため、実際の値を表示できません。
#define NN_DETAIL_ASSERT_GREATER_EQUAL_IMPL(assertImplMacroName, lhs, rhs, lhsStringized, rhsStringized) \
    assertImplMacroName( \
        ::nn::diag::detail::GreaterEqual(lhs, rhs), \
        "GreaterEqual", \
        "%s must be greater than or equal to %s.", \
        lhsStringized, \
        rhsStringized)

// ポインタ・アドレスがアラインしていることのテストです。
// 現状では、タイプセーフなフォーマットの仕組みがないため、実際の値を表示できません。
#define NN_DETAIL_ASSERT_ALIGNED_IMPL(assertImplMacroName, lhs, rhs, lhsStringized, rhsStringized) \
    assertImplMacroName( \
        ::nn::diag::detail::IsAligned(lhs, rhs), \
        "Aligned", \
        "%s must be %s-bytes aligned.", \
        lhsStringized, \
        rhsStringized)

/**
*   NN_DETAIL_INVOKE_NAMED_ASSERT_* の実装の都合で実装分岐します。
*   auto に対応している場合は、NN_DETAIL_ASSERT_***_IMPL の引数は1度しか評価されないことが保証されます。
*   auto に対応していない場合は、引数の使用回数だけ評価が行われるため、1度の使用に抑えないといけません。
*/
#if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE)

// C文字列が等しい内容であることのテストです。
#define NN_DETAIL_ASSERT_STRING_EQUAL_IMPL(assertImplMacroName, lhs, rhs, lhsStringized, rhsStringized) \
    do \
    { \
        NN_DETAIL_ASSERT_NOT_NULL_IMPL(assertImplMacroName, lhs, lhsStringized); \
        NN_DETAIL_ASSERT_NOT_NULL_IMPL(assertImplMacroName, rhs, rhsStringized); \
        assertImplMacroName( \
            ::nn::diag::detail::StringEqual(lhs, rhs), \
            "StringEqual", \
            "%s must be equal to %s.\n" "Left: %s\n" "Right: %s\n", \
            lhsStringized, \
            rhsStringized, \
            lhs, \
            rhs); \
    } while (NN_STATIC_CONDITION(0))

// C文字列が等しい内容でないことのテストです。
#define NN_DETAIL_ASSERT_STRING_NOT_EQUAL_IMPL(assertImplMacroName, lhs, rhs, lhsStringized, rhsStringized) \
    do \
    { \
        NN_DETAIL_ASSERT_NOT_NULL_IMPL(assertImplMacroName, lhs, lhsStringized); \
        NN_DETAIL_ASSERT_NOT_NULL_IMPL(assertImplMacroName, rhs, rhsStringized); \
        assertImplMacroName( \
            ::nn::diag::detail::StringNotEqual(lhs, rhs), \
            "StringNotEqual", \
            "%s must not be equal to %s.\n" "Left: %s\n" "Right: %s\n", \
            lhsStringized, \
            rhsStringized, \
            lhs, \
            rhs); \
    } while (NN_STATIC_CONDITION(0))

#else // NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE

// C文字列が等しい内容であることのテストです。
#define NN_DETAIL_ASSERT_STRING_EQUAL_IMPL(assertImplMacroName, lhs, rhs, lhsStringized, rhsStringized) \
    assertImplMacroName( \
        ::nn::diag::detail::StringEqual(lhs, rhs), \
        "StringEqual", \
        "%s must be equal to %s.\n", \
        lhsStringized, \
        rhsStringized)

// C文字列が等しい内容でないことのテストです。
#define NN_DETAIL_ASSERT_STRING_NOT_EQUAL_IMPL(assertImplMacroName, lhs, rhs, lhsStringized, rhsStringized) \
    assertImplMacroName( \
        ::nn::diag::detail::StringNotEqual(lhs, rhs), \
        "StringNotEqual", \
        "%s must not be equal to %s.\n", \
        lhsStringized, \
        rhsStringized)

#endif  // NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE

// 値が範囲内にあることのテストです。
// 現状では、タイプセーフなフォーマットの仕組みがないため、引数が整数であると仮定して long long にキャストしています。
#define NN_DETAIL_ASSERT_RANGE_IMPL(assertImplMacroName, value, begin, end, valueStringized, beginStringized, endStringized) \
    assertImplMacroName( \
        ::nn::diag::detail::WithinRange(value, begin, end), \
        "RangeCheck", \
        "%s must be within the range [%lld, %lld) (actual: %lld)", \
        valueStringized, \
        static_cast<long long>(begin), \
        static_cast<long long>(end), \
        static_cast<long long>(value))

// 値が最小値以上かつ最大値以下であることのテストです。
// 現状では、タイプセーフなフォーマットの仕組みがないため、実際の値を表示できません。
#define NN_DETAIL_ASSERT_MINMAX_IMPL(assertImplMacroName, value, min, max, valueStringized, minStringized, maxStringized) \
    assertImplMacroName( \
        ::nn::diag::detail::WithinMinMax(value, min, max), \
        "MinMaxCheck", \
        "%s must satisfy the condition min:%s and max:%s", \
        valueStringized, \
        minStringized, \
        maxStringized)

// result.IsSuccess() であることのテストです。
#define NN_DETAIL_ASSERT_RESULT_SUCCESS_IMPL(assertImplMacroName, result, resultStringized) \
    assertImplMacroName( \
        result.IsSuccess(), \
        result, \
        "nn::Result::IsSuccess()", \
        "Failed: %s\n" \
        "  Module: %d\n" \
        "  Description: %d\n" \
        "  InnerValue: 0x%08x", \
        resultStringized, result.GetModule(), result.GetDescription(), result.GetInnerValueForDebug())
