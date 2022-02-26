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

#include <nn/nn_Common.h>
#include <nn/result/result_ResultBase.h>

namespace nn {

namespace result { namespace detail {

class ResultInternalAccessor;

}}

class ResultSuccess;

/**
    @brief 処理の結果を汎用的に表す型です。
*/
class Result : public result::detail::ResultBase<Result>
{
    friend ::nn::result::detail::ResultInternalAccessor;
private:
    typedef result::detail::ResultBase<Result> Base;
public:

    /**
        @brief 有効な Result 値を持たないインスタンスを生成するデフォルトコンストラクタです。
    */
    Result() NN_NOEXCEPT {}

    /**
        @brief 成功であるかどうかを bool 値で返します。

        @return 成功を表す Result 値を持っているときには true、そうでないときには false が返ります。

        @pre *this が有効な Result 値を持っている。
    */
    bool IsSuccess() const NN_NOEXCEPT
    {
        return m_Value == InnerSuccessValue;
    }

    /**
        @brief 内部表現の値を返します。(デバッグ用)

        @return 内部表現の値を返します。

        @pre *this が有効な Result 値を持っている

        @details 得られる内部表現の値は @ref ResultFinder に入力することで、詳細情報を確認することができます。
    */
    InnerType GetInnerValueForDebug() const NN_NOEXCEPT
    {
        return m_Value;
    }

    /**
        @brief ResultSuccess 型への変換関数です。

        @return ResultSuccess 型のインスタンスを返します。

        @pre *this が有効な Result 値を持っている。

        @details
        持っている Result 値が成功を表すものでなかった場合(!this->IsSuccess() の場合)、
        変換は失敗し、UnhandledResult ハンドラが呼ばれ、処理は返りません。
    */
    NN_IMPLICIT operator ResultSuccess() const NN_NOEXCEPT;

    /**
        @brief 引数にとった Result 値をこの型の変数に格納できるかどうかを返します。常に true を返します。(内部実装用)

        @param[in] result この型の変数に格納できるかどうかを判定する対象の Result 値を指定します。
        @return 常に true を返します。

        @pre *this が有効な Result 値を持っている。
    */
    static bool CanAccept(Result result) NN_NOEXCEPT;

    /**
        @brief ! IsSuccess() を返します。

        @return ! IsSuccess() を返します。

        @pre *this が有効な Result 値を持っている。
    */
    bool IsFailure() const NN_NOEXCEPT
    {
        return !IsSuccess();
    }

    /**
        @brief モジュール値を返します。(デバッグ用)

        @return モジュール値を返します。

        @pre *this が有効な Result 値を持っている。
    */
    int GetModule() const NN_NOEXCEPT
    {
        return Base::GetModule();
    }

    /**
        @brief 詳細値を返します。(デバッグ用)

        @return 詳細値を返します。

        @pre *this が有効な Result 値を持っている。
    */
    int GetDescription() const NN_NOEXCEPT
    {
        return Base::GetDescription();
    }

private:
    InnerType m_Value;
    explicit Result(InnerType innerValue) NN_NOEXCEPT : m_Value(innerValue) {}
};

inline bool Result::CanAccept(Result result) NN_NOEXCEPT
{
    NN_UNUSED(result);
    return true;
}

namespace result { namespace detail {

class ResultInternalAccessor
{
public:
    static Result ConstructResult(ResultTraits::InnerType innerValue) NN_NOEXCEPT
    {
        return Result(innerValue);
    }
};

inline Result ConstructResult(ResultTraits::InnerType innerValue) NN_NOEXCEPT
{
    return ResultInternalAccessor::ConstructResult(innerValue);
}

}} // result::detail

/**
    @brief Result の成功を表す型です。
*/
class ResultSuccess : public result::detail::ResultBase<ResultSuccess>
{
private:
    typedef result::detail::ResultBase<ResultSuccess> Base;
public:

    /**
        @brief Result 型への変換関数です。

        @return Result::IsSuccess() が true であるような Result 型のインスタンスを返します。
    */
    NN_IMPLICIT operator Result() const NN_NOEXCEPT
    {
        return result::detail::ConstructResult(InnerSuccessValue);
    }

    /**
        @brief 成功であるかどうかを bool 値で返します。常に true を返します。

        @return 常に true を返します。
    */
    bool IsSuccess() const NN_NOEXCEPT
    {
        return true;
    }

    /**
        @brief 引数にとった Result 値をこの型の変数に格納できるかどうかを返します。(内部実装用)

        @param[in] result この型の変数に格納できるかどうかを判定する対象の Result 値を指定します。
        @return result.IsSuccess() を返します。

        @pre result が有効な Result 値を持っている
    */
    static bool CanAccept(Result result) NN_NOEXCEPT
    {
        return result.IsSuccess();
    }

    /**
        @brief 内部表現の値を返します。(デバッグ用)

        @return 内部表現の値を返します。
    */
    InnerType GetInnerValueForDebug() const NN_NOEXCEPT
    {
        return InnerSuccessValue;
    }

    /**
        @brief モジュール値を返します。(デバッグ用)

        @return モジュール値を返します。
    */
    int GetModule() const NN_NOEXCEPT
    {
        return Base::GetModule();
    }

    /**
        @brief 詳細値を返します。(デバッグ用)

        @return 詳細値を返します。
    */
    int GetDescription() const NN_NOEXCEPT
    {
        return Base::GetDescription();
    }
};

namespace result { namespace detail {

NN_NORETURN void OnUnhandledResult(Result result) NN_NOEXCEPT;

}}

inline Result::operator ResultSuccess() const NN_NOEXCEPT
{
    if (!ResultSuccess::CanAccept(*this))
    {
        result::detail::OnUnhandledResult(*this);
    }
    return ResultSuccess();
}

}
