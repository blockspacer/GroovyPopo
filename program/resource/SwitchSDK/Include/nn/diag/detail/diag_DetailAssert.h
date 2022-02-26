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
* @brief   Assert APIの実装に利用する関数、マクロの定義
*/

#include <nn/nn_Common.h>
#include <nn/TargetConfigs/build_Compiler.h>
#include <nn/diag/detail/diag_DetailBuildSwitch.h>
#include <nn/diag/diag_AssertionTypes.h>
#include <nn/diag/diag_MetaLog.h>

namespace nn {
    class Result;
}

namespace nn { namespace diag {

namespace detail {

    void OnAssertionFailure(
        AssertionType assertionType,
        const char* condition,
        const char* functionName,
        const char* fileName,
        int lineNumber,
        const char* format,
        ...) NN_NOEXCEPT;

    void OnAssertionFailure(
        AssertionType assertionType,
        const char* condition,
        const char* functionName,
        const char* fileName,
        int lineNumber) NN_NOEXCEPT;

    NN_NORETURN void AbortImpl(
        const char* condition,
        const char* functionName,
        const char* fileName,
        int lineNumber,
        const Result* result,
        const char* format,
        ...) NN_NOEXCEPT;

    NN_NORETURN void AbortImpl(
        const char* condition,
        const char* functionName,
        const char* fileName,
        int lineNumber,
        const char* format,
        ...) NN_NOEXCEPT;

    NN_NORETURN void AbortImpl(
        const char* condition,
        const char* functionName,
        const char* fileName,
        int lineNumber) NN_NOEXCEPT;

    NN_NORETURN void AbortImpl(
        const MetaLogContext& metaLogContext,
        const Result* result) NN_NOEXCEPT;

    NN_NORETURN void AbortImpl(
        const MetaLogContext& metaLogContext) NN_NOEXCEPT;
}

}}

#if defined(NN_DETAIL_ENABLE_ABORT_MESSAGE)

#define NN_DETAIL_ASSERT_COMMON_IMPL(assertionType, condition, conditionString, ...) \
    (static_cast<void>((condition) \
    || (::nn::diag::detail::OnAssertionFailure( \
        ::nn::diag::AssertionType_##assertionType, \
        conditionString, NN_CURRENT_FUNCTION_NAME, __FILE__, __LINE__, ##__VA_ARGS__), true)))

#define NN_DETAIL_ABORT_UNLESS_IMPL(condition, conditionString, ...) \
        (static_cast<void>((condition) \
        || (::nn::diag::detail::AbortImpl( \
            conditionString, NN_CURRENT_FUNCTION_NAME, __FILE__, __LINE__, ##__VA_ARGS__), true)))

#define NN_DETAIL_ABORT_UNLESS_RESULT_SUCCESS_IMPL(condition, result, conditionString, ...) \
        (static_cast<void>((condition) \
        || (::nn::diag::detail::AbortImpl( \
            conditionString, NN_CURRENT_FUNCTION_NAME, __FILE__, __LINE__, &result, ##__VA_ARGS__), true)))

#if !defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE)

#define NN_DETAIL_ABORT_UNLESS_IMPL_WITHOUT_AUTO_TYPE(condition, conditionString, ...) \
    NN_DETAIL_ABORT_UNLESS_IMPL(condition, conditionString)

#endif // !defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE)

#elif defined(NN_ENABLE_SDK_META_LOG)   // defined(NN_DETAIL_ENABLE_ABORT_MESSAGE)

#define NN_DETAIL_ASSERT_COMMON_IMPL(assertionType, condition, conditionString, ...) \
    (static_cast<void>((condition) \
    || (::nn::diag::detail::OnAssertionFailure( \
        ::nn::diag::AssertionType_##assertionType, \
        "", "", "", 0), true)))

#define NN_DETAIL_ABORT_UNLESS_IMPL(condition, conditionString, ...) \
        (static_cast<void>((condition) \
        || (::nn::diag::detail::AbortImpl( \
            NN_DETAIL_DIAG_META_LOG_CONTEXT("AbortUnless", "line", __LINE__, "file", __FILE__, "function", NN_CURRENT_FUNCTION_NAME, "condition", conditionString)), true)))

#define NN_DETAIL_ABORT_UNLESS_RESULT_SUCCESS_IMPL(condition, result, conditionString, format, resultStringized, ... /* result.Getters... */) \
        (static_cast<void>((condition) \
        || (::nn::diag::detail::AbortImpl( \
            NN_DETAIL_DIAG_META_LOG_CONTEXT("AbortUnlessResultSuccess", "line", __LINE__, "file", __FILE__, "function", NN_CURRENT_FUNCTION_NAME, "condition", resultStringized), &result), true)))

#if !defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE)

#define NN_DETAIL_ABORT_UNLESS_IMPL_WITHOUT_AUTO_TYPE(condition, conditionString, ...) \
    NN_DETAIL_ABORT_UNLESS_IMPL(condition, conditionString)

#endif // !defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE)

#else // defined(NN_DETAIL_ENABLE_ABORT_MESSAGE)

#define NN_DETAIL_ASSERT_COMMON_IMPL(assertionType, condition, conditionString, ...) \
    (static_cast<void>((condition) \
    || (::nn::diag::detail::OnAssertionFailure( \
        ::nn::diag::AssertionType_##assertionType, \
        "", "", "", 0), true)))

#define NN_DETAIL_ABORT_UNLESS_IMPL(condition, conditionString, ...) \
        (static_cast<void>((condition) \
        || (::nn::diag::detail::AbortImpl( \
            "", "", "", 0), true)))

#define NN_DETAIL_ABORT_UNLESS_RESULT_SUCCESS_IMPL(condition, result, conditionString, ...) \
        (static_cast<void>((condition) \
        || (::nn::diag::detail::AbortImpl( \
            "", "", "", 0, &result, ""), true)))

#if !defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE)

#define NN_DETAIL_ABORT_UNLESS_IMPL_WITHOUT_AUTO_TYPE(condition, conditionString, ...) \
    NN_DETAIL_ABORT_UNLESS_IMPL(condition, "")

#endif // !defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_AUTO_TYPE)

#endif // defined(NN_DETAIL_ENABLE_ABORT_MESSAGE)
