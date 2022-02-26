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
 * @brief   多重待ちに関するユーティリティ公開ヘッダファイル
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/os/os_MultipleWaitCommon.h>
#include <nn/os/os_MultipleWaitTypes.h>
#include <nn/os/os_MultipleWaitApi.h>
#include <nn/util/util_MacroForVariadic.h>
#include <utility>
#include <nn/nn_SdkAssert.h>
#include <nn/nn_TimeSpan.h>

namespace nn { namespace os {

// forward
struct MessageQueueType;

namespace detail {

struct MessageQueueWaitMultiWaitHelper;

}

#if defined(NN_BUILD_FOR_DOCUMENT_GENERATION)

//! @name 多重待ち関連ユーティリティ API
//! @{

/**
 * @brief   multiWait リスト内のいずれかの multiWaitHolder または指定された複数の待機可能オブジェクトを待機します。
 *
 * @tparam Args 追加待機オブジェクトの可変長型パラメータ
 *
 * @param[in] pMultiWait     multiWait オブジェクトヘッダへのポインタ
 * @param[in] args           待機可能オブジェクトの可変長パラメータ
 *
 * @return  pMultiWait にリンクされている MultiWaitHolderType がシグナルされた場合は、そのポインタを p として { p, -1 } を返し、
 *          args に指定されたオブジェクトがシグナルされた場合は、args 内での 0 から始まるインデックスを n として { nullptr, n } を返します。
 *
 * @pre
 *  - multiWait が初期化状態である
 *  - multiWait にリンクされた全ての MultiWaitHolderType が初期化状態である
 *  - multiWait が他スレッド上で使用されていない
 *  - args の各引数 x に対し InitializeMultiWaitHolder(pHolder, x) を正しく呼び出せる
 *
 * @post
 *  - 返された値に対応した待機可能オブジェクトがシグナルされている
 *
 * @details
 *  待機対象の指定方法や結果の返し方の違いを除いて、詳細な動作は WaitAny(MultiWaitType*) と同様です。
 *
 * @see WaitAny(MultiWaitType*)
 *
 */
template <typename... Args>
std::pair<MultiWaitHolderType*, int> WaitAny(MultiWaitType* pMultiWait, Args&&... args) NN_NOEXCEPT;

/**
 * @brief   指定された複数の待機可能オブジェクトを待機します。
 *
 * @tparam Args 待機オブジェクトの可変長型パラメータ
 *
 * @param[in] args           待機可能オブジェクトの可変長パラメータ
 *
 * @return  シグナルされたオブジェクトの args 内での 0 から始まるインデックス返します。
 *
 * @pre
 *  - args の各引数 x に対し InitializeMultiWaitHolder(pHolder, x) を正しく呼び出せる
 *
 * @post
 *  - 返された値に対応した待機可能オブジェクトがシグナルされている
 *
 * @details
 *  待機対象の指定方法や結果の返し方の違いを除いて、詳細な動作は WaitAny(MultiWaitType*) と同様です。
 *
 * @see WaitAny(MultiWaitType*)
 *
 */
template <typename... Args>
int WaitAny(Args&&... args) NN_NOEXCEPT;

/**
 * @brief   multiWait リスト内のいずれかの multiWaitHolder または指定された複数の待機可能オブジェクトをポーリングします。
 *
 * @tparam Args 追加待機オブジェクトの可変長型パラメータ
 *
 * @param[in] pMultiWait     multiWait オブジェクトヘッダへのポインタ
 * @param[in] args           待機可能オブジェクトの可変長パラメータ
 *
 * @return  pMultiWait にリンクされている MultiWaitHolderType がシグナルされていた場合は、そのポインタを p として { p, -1 } を返し、
 *          args に指定されたオブジェクトがシグナルされていた場合は、args 内での 0 から始まるインデックスを n として { nullptr, n } を返します。
 *          いずれもシグナルされていなかった場合には { nullptr, -1 } を返します。
 *
 * @pre
 *  - multiWait が初期化状態である
 *  - multiWait にリンクされた全ての MultiWaitHolderType が初期化状態である
 *  - multiWait が他スレッド上で使用されていない
 *  - args の各引数 x に対し InitializeMultiWaitHolder(pHolder, x) を正しく呼び出せる
 *
 * @post
 *  - 返り値 != { nullptr, -1 } のとき、返された値に対応した待機可能オブジェクトがシグナルされている
 *
 * @details
 *  待機対象の指定方法や結果の返し方の違いを除いて、詳細な動作は TryWaitAny(MultiWaitType*) と同様です。
 *
 * @see TryWaitAny(MultiWaitType*)
 *
 */
template <typename... Args>
std::pair<MultiWaitHolderType*, int> TryWaitAny(MultiWaitType* pMultiWait, Args&&... args) NN_NOEXCEPT;

/**
 * @brief   指定された複数の待機可能オブジェクトをポーリングします。
 *
 * @tparam Args 待機オブジェクトの可変長型パラメータ
 *
 * @param[in] args           待機可能オブジェクトの可変長パラメータ
 *
 * @return  シグナルされていたオブジェクトの args 内での 0 から始まるインデックス返します。
 *          いずれもシグナルされていなかった場合には -1 を返します。
 *
 * @pre
 *  - args の各引数 x に対し InitializeMultiWaitHolder(pHolder, x) を正しく呼び出せる
 *
 * @post
 *  - 返り値 >= 0 のとき、返された値に対応した待機可能オブジェクトがシグナルされている
 *
 * @details
 *  待機対象の指定方法や結果の返し方の違いを除いて、詳細な動作は TryWaitAny(MultiWaitType*) と同様です。
 *
 * @see TryWaitAny(MultiWaitType*)
 *
 */
template <typename... Args>
int TryWaitAny(Args&&... args) NN_NOEXCEPT;

/**
 * @brief   multiWait リスト内のいずれかの multiWaitHolder または指定された複数の待機可能オブジェクトをタイムアウト付きで待機します。
 *
 * @tparam Args 追加待機オブジェクトの可変長型パラメータ
 *
 * @param[in] pMultiWait     multiWait オブジェクトヘッダへのポインタ
 * @param[in] timeout        タイムアウト値
 * @param[in] args           待機可能オブジェクトの可変長パラメータ
 *
 * @return  pMultiWait にリンクされている MultiWaitHolderType がシグナルされた場合は、そのポインタを p として { p, -1 } を返し、
 *          args に指定されたオブジェクトがシグナルされた場合は、args 内での 0 から始まるインデックスを n として { nullptr, n } を返します。
 *          時間内にいずれもシグナルされていなかった場合には { nullptr, -1 } を返します。
 *
 * @pre
 *  - multiWait が初期化状態である
 *  - multiWait にリンクされた全ての MultiWaitHolderType が初期化状態である
 *  - multiWait が他スレッド上で使用されていない
 *  - timeout >= 0
 *  - args の各引数 x に対し InitializeMultiWaitHolder(pHolder, x) を正しく呼び出せる
 *
 * @post
 *  - 返り値 != { nullptr, -1 } のとき、返された値に対応した待機可能オブジェクトがシグナルされている
 *  - 返り値 == { nullptr, -1 } のとき、呼び出してから timeout 以上の時間が経過している
 *
 * @details
 *  待機対象の指定方法や結果の返し方の違いを除いて、詳細な動作は TimedWaitAny(MultiWaitType*, TimeSpan) と同様です。
 *
 * @see TimedWaitAny(MultiWaitType*, TimeSpan)
 */
template <typename... Args>
std::pair<MultiWaitHolderType*, int> TimedWaitAny(MultiWaitType* pMultiWait, TimeSpan timeout, Args&&... args) NN_NOEXCEPT;

/**
 * @brief   指定された複数の待機可能オブジェクトをタイムアウト付きで待機します。
 *
 * @tparam Args 待機オブジェクトの可変長型パラメータ
 *
 * @param[in] timeout        タイムアウト値
 * @param[in] args           待機可能オブジェクトの可変長パラメータ
 *
 * @return  シグナルされていたオブジェクトの args 内での 0 から始まるインデックス返します。
 *          時間内にいずれもシグナルされていなかった場合には -1 を返します。
 *
 * @pre
 *  - timeout >= 0
 *  - args の各引数 x に対し InitializeMultiWaitHolder(pHolder, x) を正しく呼び出せる
 *
 * @post
 *  - 返り値 >= 0 のとき、返された値に対応した待機可能オブジェクトがシグナルされている
 *  - 返り値 == -1 のとき、呼び出してから timeout 以上の時間が経過している
 *
 * @details
 *  待機対象の指定方法や結果の返し方の違いを除いて、詳細な動作は TimedWaitAny(MultiWaitType*, TimeSpan) と同様です。
 *
 * @see TimedWaitAny(MultiWaitType*, TimeSpan)
 */
template <typename... Args>
int TimedWaitAny(TimeSpan timeout, Args&&... args) NN_NOEXCEPT;

/**
 * @brief 多重待ちユーティリティでメッセージキューを指定する際のユーティリティです。
 *
 * @param[in] messageQueue      紐付ける先のメッセージキューへのポインタ
 * @param[in] waitType          メッセージキューの待ち種別
 *
 * @pre
 *  - messageQueue が初期化状態である
 *
 * @return 多重待ちユーティリティに渡すための値を返します。
 *
 * @details
 *  本関数の返り値を多重待ちユーティリティ関数に渡した際の待機対象は、
 *  messageQueue で指定されたメッセージキューに対する waitType で指定された種別での待機となります。
 */
detail::MessageQueueWaitMultiWaitHelper operator&(MessageQueueType* messageQueue, MessageQueueWaitType waitType) NN_NOEXCEPT;

//! @}

#endif

// インライン実装

#if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_CLANG) || defined(NN_BUILD_CONFIG_TOOLCHAIN_VC_VS2015) || defined(NN_BUILD_CONFIG_TOOLCHAIN_SUPPORTS_VC_VS2013)
    #define NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_USE_NATIVE_VARIADIC
#endif

// forward
void InitializeMultiWaitHolder(MultiWaitHolderType* multiWaitHolder, MessageQueueType* messageQueue, MessageQueueWaitType waitType) NN_NOEXCEPT;

namespace detail {

struct MessageQueueWaitMultiWaitHelper
{
    MessageQueueType* _messageQueue;
    MessageQueueWaitType _waitType;
};

inline void InitializeMultiWaitHolder(MultiWaitHolderType* multiWaitHolder, const detail::MessageQueueWaitMultiWaitHelper& helper) NN_NOEXCEPT
{
    InitializeMultiWaitHolder(multiWaitHolder, helper._messageQueue, helper._waitType);
}

class AutoMultiWaitHolder
{
private:

    MultiWaitHolderType m_Holder;

public:

    template <typename T>
    explicit AutoMultiWaitHolder(MultiWaitType* pMultiWait, T&& x) NN_NOEXCEPT
    {
        InitializeMultiWaitHolder(&m_Holder, std::forward<T>(x));
        LinkMultiWaitHolder(pMultiWait, &m_Holder);
    }

    ~AutoMultiWaitHolder() NN_NOEXCEPT
    {
        UnlinkMultiWaitHolder(&m_Holder);
        FinalizeMultiWaitHolder(&m_Holder);
    }

    std::pair<MultiWaitHolderType*, int> ConvertResult(const std::pair<MultiWaitHolderType*, int> result, int index) NN_NOEXCEPT
    {
        if (result.first == &m_Holder)
        {
            return std::make_pair(static_cast<MultiWaitHolderType*>(nullptr), index);
        }
        else
        {
            return result;
        }
    }

};

template <typename F>
inline std::pair<MultiWaitHolderType*, int> WaitAnyImpl(F&& waiter, MultiWaitType* pMultiWait, int) NN_NOEXCEPT
{
    auto p = waiter(pMultiWait);
    return std::pair<MultiWaitHolderType*, int>(p, -1);
}

#ifdef NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_USE_NATIVE_VARIADIC
    template <typename F, typename T, typename... Args>
    inline std::pair<MultiWaitHolderType*, int> WaitAnyImpl(F&& waiter, MultiWaitType* pMultiWait, int index, T&& x, Args&&... args) NN_NOEXCEPT
    {
        AutoMultiWaitHolder holder(pMultiWait, std::forward<T>(x));
        auto result = WaitAnyImpl(std::forward<F>(waiter), pMultiWait, index + 1, std::forward<Args>(args)...);
        return holder.ConvertResult(result, index);
    }
#else
    template <typename F, typename T>
    inline std::pair<MultiWaitHolderType*, int> WaitAnyImpl(F&& waiter, MultiWaitType* pMultiWait, int index, T&& x) NN_NOEXCEPT
    {
        AutoMultiWaitHolder holder(pMultiWait, std::forward<T>(x));
        auto result = WaitAnyImpl(std::forward<F>(waiter), pMultiWait, index + 1);
        return holder.ConvertResult(result, index);
    }

    #define NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_VARIADIC_IMPL(n) \
        template <typename F, typename T, NN_UTIL_VARIADIC_TEMPLATE_TEMPLATE_ARGUMENTS_##n (Arg)> \
        inline std::pair<MultiWaitHolderType*, int> WaitAnyImpl(F&& waiter, MultiWaitType* pMultiWait, int index, T&& x, NN_UTIL_VARIADIC_TEMPLATE_ARGUMENT_LIST_##n (Arg, arg)) NN_NOEXCEPT \
        { \
            AutoMultiWaitHolder holder(pMultiWait, std::forward<T>(x)); \
            auto result = WaitAnyImpl(std::forward<F>(waiter), pMultiWait, index + 1, NN_UTIL_VARIADIC_TEMPLATE_FORWARD_LIST_##n (Arg, arg)); \
            return holder.ConvertResult(result, index); \
        }

        NN_UTIL_VARIADIC_DEFINE_MACROS(NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_VARIADIC_IMPL)

    #undef NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_VARIADIC_IMPL
#endif

#ifdef NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_USE_NATIVE_VARIADIC
    template <typename F, typename... Args>
    inline std::pair<MultiWaitHolderType*, int> WaitAnyImpl(F&& waiter, MultiWaitType* pMultiWait, Args&&... args) NN_NOEXCEPT
    {
        return WaitAnyImpl(std::forward<F>(waiter), pMultiWait, 0, std::forward<Args>(args)...);
    }
#else
    template <typename F>
    inline std::pair<MultiWaitHolderType*, int> WaitAnyImpl(F&& waiter, MultiWaitType* pMultiWait) NN_NOEXCEPT
    {
        return WaitAnyImpl(std::forward<F>(waiter), pMultiWait, 0);
    }

    #define NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_VARIADIC_IMPL(n) \
        template <typename F, NN_UTIL_VARIADIC_TEMPLATE_TEMPLATE_ARGUMENTS_##n (Arg)> \
        inline std::pair<MultiWaitHolderType*, int> WaitAnyImpl(F&& waiter, MultiWaitType* pMultiWait, NN_UTIL_VARIADIC_TEMPLATE_ARGUMENT_LIST_##n (Arg, arg)) NN_NOEXCEPT \
        { \
            return WaitAnyImpl(std::forward<F>(waiter), pMultiWait, 0, NN_UTIL_VARIADIC_TEMPLATE_FORWARD_LIST_##n (Arg, arg)); \
        }

        NN_UTIL_VARIADIC_DEFINE_MACROS(NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_VARIADIC_IMPL)

    #undef NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_VARIADIC_IMPL
#endif

class TempMultiWait
{
private:

    MultiWaitType m_MultiWait;

public:

    TempMultiWait() NN_NOEXCEPT
    {
        nn::os::InitializeMultiWait(&m_MultiWait);
    }

    ~TempMultiWait() NN_NOEXCEPT
    {
        nn::os::FinalizeMultiWait(&m_MultiWait);
    }

    MultiWaitType* Get() NN_NOEXCEPT
    {
        return &m_MultiWait;
    }

};

#ifdef NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_USE_NATIVE_VARIADIC
    template <typename F, typename... Args>
    inline std::pair<MultiWaitHolderType*, int> WaitAnyImpl(F&& waiter, Args&&... args) NN_NOEXCEPT
    {
        TempMultiWait tempMultiWait;
        return WaitAnyImpl(std::forward<F>(waiter), tempMultiWait.Get(), 0, std::forward<Args>(args)...);
    }
#else
    template <typename F>
    inline std::pair<MultiWaitHolderType*, int> WaitAnyImpl(F&& waiter) NN_NOEXCEPT
    {
        TempMultiWait tempMultiWait;
        return WaitAnyImpl(std::forward<F>(waiter), tempMultiWait.Get(), 0);
    }

    #define NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_VARIADIC_IMPL(n) \
        template <typename F, NN_UTIL_VARIADIC_TEMPLATE_TEMPLATE_ARGUMENTS_##n (Arg)> \
        inline std::pair<MultiWaitHolderType*, int> WaitAnyImpl(F&& waiter, NN_UTIL_VARIADIC_TEMPLATE_ARGUMENT_LIST_##n (Arg, arg)) NN_NOEXCEPT \
        { \
            TempMultiWait tempMultiWait; \
            return WaitAnyImpl(std::forward<F>(waiter), tempMultiWait.Get(), 0, NN_UTIL_VARIADIC_TEMPLATE_FORWARD_LIST_##n (Arg, arg)); \
        }

        NN_UTIL_VARIADIC_DEFINE_MACROS(NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_VARIADIC_IMPL)

    #undef NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_VARIADIC_IMPL
#endif

typedef MultiWaitHolderType* (*WaitAnyFunction)(MultiWaitType*);

class TimedWaitAnyFunction
{
private:

    TimeSpan m_Timeout;

public:

    explicit TimedWaitAnyFunction(TimeSpan timeout) NN_NOEXCEPT
        : m_Timeout(timeout)
    {
    }

    MultiWaitHolderType* operator()(MultiWaitType* p) NN_NOEXCEPT
    {
        return TimedWaitAny(p, m_Timeout);
    }

};

#if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_CLANG) || defined(NN_BUILD_CONFIG_TOOLCHAIN_VC_VS2015) || defined(NN_BUILD_CONFIG_TOOLCHAIN_SUPPORTS_VC_VS2013)
class NotBoolButInt
{
public:
    NN_IMPLICIT NotBoolButInt(int n) NN_NOEXCEPT
        : m_N(n)
    {
    }
    NN_IMPLICIT operator int() const NN_NOEXCEPT
    {
        return m_N;
    }
    explicit operator bool() const NN_NOEXCEPT = delete; // NOLINT(c++11/explicit)
private:
    int m_N;
};
#else
typedef int NotBoolButInt;
#endif

}

#ifdef NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_USE_NATIVE_VARIADIC
    template <typename... Args>
    inline std::pair<MultiWaitHolderType*, int> WaitAny(MultiWaitType* pMultiWait, Args&&... args) NN_NOEXCEPT
    {
        return detail::WaitAnyImpl(static_cast<detail::WaitAnyFunction>(&os::WaitAny), pMultiWait, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline int WaitAny(Args&&... args) NN_NOEXCEPT
    {
        return detail::WaitAnyImpl(static_cast<detail::WaitAnyFunction>(&os::WaitAny), std::forward<Args>(args)...).second;
    }

    template <typename... Args>
    inline std::pair<MultiWaitHolderType*, int> TryWaitAny(MultiWaitType* pMultiWait, Args&&... args) NN_NOEXCEPT
    {
        return detail::WaitAnyImpl(static_cast<detail::WaitAnyFunction>(&os::TryWaitAny), pMultiWait, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline detail::NotBoolButInt TryWaitAny(Args&&... args) NN_NOEXCEPT
    {
        return detail::WaitAnyImpl(static_cast<detail::WaitAnyFunction>(&os::TryWaitAny), std::forward<Args>(args)...).second;
    }

    template <typename... Args>
    inline std::pair<MultiWaitHolderType*, int> TimedWaitAny(MultiWaitType* pMultiWait, TimeSpan timeout, Args&&... args) NN_NOEXCEPT
    {
        return detail::WaitAnyImpl(detail::TimedWaitAnyFunction(timeout), pMultiWait, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline detail::NotBoolButInt TimedWaitAny(TimeSpan timeout, Args&&... args) NN_NOEXCEPT
    {
        return detail::WaitAnyImpl(detail::TimedWaitAnyFunction(timeout), std::forward<Args>(args)...).second;
    }
#else
    inline int WaitAny() NN_NOEXCEPT
    {
        return detail::WaitAnyImpl(static_cast<detail::WaitAnyFunction>(&os::WaitAny)).second;
    }
    inline detail::NotBoolButInt TryWaitAny() NN_NOEXCEPT
    {
        return detail::WaitAnyImpl(static_cast<detail::WaitAnyFunction>(&os::TryWaitAny)).second;
    }
    inline detail::NotBoolButInt TimedWaitAny(TimeSpan timeout) NN_NOEXCEPT
    {
        return detail::WaitAnyImpl(detail::TimedWaitAnyFunction(timeout)).second;
    }
    #define NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_VARIADIC_IMPL(n) \
        template <NN_UTIL_VARIADIC_TEMPLATE_TEMPLATE_ARGUMENTS_##n (Arg)> \
        inline std::pair<MultiWaitHolderType*, int> WaitAny(MultiWaitType* pMultiWait, NN_UTIL_VARIADIC_TEMPLATE_ARGUMENT_LIST_##n (Arg, arg)) NN_NOEXCEPT \
        { \
            return detail::WaitAnyImpl(static_cast<detail::WaitAnyFunction>(&os::WaitAny), pMultiWait, NN_UTIL_VARIADIC_TEMPLATE_FORWARD_LIST_##n (Arg, arg)); \
        } \
        template <NN_UTIL_VARIADIC_TEMPLATE_TEMPLATE_ARGUMENTS_##n (Arg)> \
        inline int WaitAny(NN_UTIL_VARIADIC_TEMPLATE_ARGUMENT_LIST_##n (Arg, arg)) NN_NOEXCEPT \
        { \
            return detail::WaitAnyImpl(static_cast<detail::WaitAnyFunction>(&os::WaitAny), NN_UTIL_VARIADIC_TEMPLATE_FORWARD_LIST_##n (Arg, arg)).second; \
        } \
        template <NN_UTIL_VARIADIC_TEMPLATE_TEMPLATE_ARGUMENTS_##n (Arg)> \
        inline std::pair<MultiWaitHolderType*, int> TryWaitAny(MultiWaitType* pMultiWait, NN_UTIL_VARIADIC_TEMPLATE_ARGUMENT_LIST_##n (Arg, arg)) NN_NOEXCEPT \
        { \
            return detail::WaitAnyImpl(static_cast<detail::WaitAnyFunction>(&os::TryWaitAny), pMultiWait, NN_UTIL_VARIADIC_TEMPLATE_FORWARD_LIST_##n (Arg, arg)); \
        } \
        template <NN_UTIL_VARIADIC_TEMPLATE_TEMPLATE_ARGUMENTS_##n (Arg)> \
        inline detail::NotBoolButInt TryWaitAny(NN_UTIL_VARIADIC_TEMPLATE_ARGUMENT_LIST_##n (Arg, arg)) NN_NOEXCEPT \
        { \
            return detail::WaitAnyImpl(static_cast<detail::WaitAnyFunction>(&os::TryWaitAny), NN_UTIL_VARIADIC_TEMPLATE_FORWARD_LIST_##n (Arg, arg)).second; \
        } \
        template <NN_UTIL_VARIADIC_TEMPLATE_TEMPLATE_ARGUMENTS_##n (Arg)> \
        inline std::pair<MultiWaitHolderType*, int> TimedWaitAny(MultiWaitType* pMultiWait, TimeSpan timeout, NN_UTIL_VARIADIC_TEMPLATE_ARGUMENT_LIST_##n (Arg, arg)) NN_NOEXCEPT \
        { \
            return detail::WaitAnyImpl(detail::TimedWaitAnyFunction(timeout), pMultiWait, NN_UTIL_VARIADIC_TEMPLATE_FORWARD_LIST_##n (Arg, arg)); \
        } \
        template <NN_UTIL_VARIADIC_TEMPLATE_TEMPLATE_ARGUMENTS_##n (Arg)> \
        inline detail::NotBoolButInt TimedWaitAny(TimeSpan timeout, NN_UTIL_VARIADIC_TEMPLATE_ARGUMENT_LIST_##n (Arg, arg)) NN_NOEXCEPT \
        { \
            return detail::WaitAnyImpl(detail::TimedWaitAnyFunction(timeout), NN_UTIL_VARIADIC_TEMPLATE_FORWARD_LIST_##n (Arg, arg)).second; \
        }

        NN_UTIL_VARIADIC_DEFINE_MACROS(NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_VARIADIC_IMPL)

    #undef NN_OS_DETAIL_MULTIPLE_WAIT_UTILITY_VARIADIC_IMPL
#endif

inline detail::MessageQueueWaitMultiWaitHelper operator&(MessageQueueType* messageQueue, MessageQueueWaitType waitType) NN_NOEXCEPT
{
    detail::MessageQueueWaitMultiWaitHelper ret = { messageQueue, waitType };
    return ret;
}

}}
