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

/*!
    @file
    @brief スコープを抜ける際に実行するコードを記述するためのマクロの定義しています。

    @platformbegin{Cafe}
    本ファイルは C++11 の機能を使用しているため、Cafe では include できません。
    @platformend
*/

#include <nn/nn_Macro.h>
#include <utility>

namespace nn { namespace util { namespace detail {

template <typename F>
class ScopeExitHolder
{
    NN_DISALLOW_COPY(ScopeExitHolder);
private:

    F m_F;
    bool m_Valid;

public:

    explicit ScopeExitHolder(F&& f) NN_NOEXCEPT
        : m_F(std::move(f))
        , m_Valid(true)
    {
    }

    ScopeExitHolder(ScopeExitHolder&& other) NN_NOEXCEPT
        : m_F(std::move(other.m_F))
        , m_Valid(other.m_Valid)
    {
        other.m_Valid = false;
    }

    ~ScopeExitHolder() NN_NOEXCEPT
    {
        if (m_Valid)
        {
            m_F();
        }
    }

};

enum ScopeExitHolderMarker {};

template <typename F>
inline ScopeExitHolder<F> operator+(ScopeExitHolderMarker, F&& f) NN_NOEXCEPT
{
    return ScopeExitHolder<F>(std::forward<F>(f));
}

}}}

#if defined(NN_BUILD_FOR_DOCUMENT_GENERATION)

    /*!
        @brief スコープを抜ける際の処理を記述するためのマクロです。

        @details 本マクロに続けて、ブロックを記述すると、
                 ブロック内の処理が、スコープを抜ける際に実行されます。
                 なお、ブロックの閉じ中括弧の後にセミコロンが必要です。

                 使用例：
                 @code
                 {
                     FILE* f = std::fopen(path, "r");
                     NN_UTIL_SCOPE_EXIT
                     {
                         std::fclose(f);
                     };

                     ... std::fread などの呼び出し

                     // スコープ終了時に自動的に std::fclose(f) が呼ばれる
                 }
                 @endcode

        @platformbegin{Cafe}
        本マクロは C++11 の機能を使用しているため、Cafe では使用できません。
        @platformend
    */
    #define NN_UTIL_SCOPE_EXIT

#else

    #define NN_UTIL_SCOPE_EXIT \
        const auto NN_MAKE_TEMPORARY_NAME(_nn_util_scope_exit_) = ::nn::util::detail::ScopeExitHolderMarker() + [&] ()

#endif
