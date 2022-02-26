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

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <nn/nn_Macro.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief        文字列を追加します。
//!
//! source 文字列を count 分コピーし、必ず null の終端文字を追加します。
//! dstCount が srcCount 小さい場合は切り詰めて追加されますが、
//! 必ず null 終端文字を挿入します。
//!
//! @param[in]    dst 追加先のバッファのアドレスです。
//! @param[in]    dstCount コピー先のサイズです。
//! @param[in]    src 追加元の文字列のアドレスです。
//! @param[in]    srcCount 追加する文字数です。
//!
//! @return       追加した文字数を返します。
//---------------------------------------------------------------------------
inline size_t strncat(char* dst, std::size_t dstCount, const char* src, std::size_t srcCount) NN_NOEXCEPT
{
    size_t length  = std::strlen(dst);
    if (dstCount <= length + srcCount)
    {
        srcCount = dstCount - length - 1;
    }

    std::strncat( dst, src, srcCount );

    dst[dstCount - 1] = '\0';

    return srcCount;
}

//---------------------------------------------------------------------------
//! @internal
//! @brief        文字列の先頭から文字を検索します。
//!
//! @param[in]    src      検索対象文字列です。
//! @param[in]    srcCount 検索対象文字列の最大バッファ長です。
//! @param[in]    c        検索対象の文字です。
//!
//! @return       最初に発見された文字位置のポインタを返します。
//!               見つからない場合は NULL を返します。
//---------------------------------------------------------------------------
inline char* strnchr(char* src, std::size_t srcCount, int c) NN_NOEXCEPT
{
    char* ptr = src;
    char* end_ptr = src + srcCount;

    while ( *ptr && ptr != end_ptr )
    {
        if (*ptr == c ) { return ptr; }
        ++ptr;
    }

    return nullptr;
}

//---------------------------------------------------------------------------
//! @internal
//! @brief        文字列の先頭からマッチする文字を検索します。(const 版)
//!
//! @param[in]    src      検索対象文字列です。
//! @param[in]    srcCount 検索対象文字列の最大バッファ長です。
//! @param[in]    c        検索対象の文字です。
//!
//! @return       最初に発見された文字位置のポインタを返します。
//!               見つからない場合は NULL を返します。
//---------------------------------------------------------------------------
inline const char* strnchr(const char* src, std::size_t srcCount, int c) NN_NOEXCEPT
{
    return strnchr( const_cast<char*>( src ), srcCount, c );
}

//---------------------------------------------------------------------------
//! @internal
//! @brief    文字列を操作、管理します。
//!
//! @details  HACK : このクラスは実装途中です。
//!                  必要になった機能だけを随時実装していきます。
//---------------------------------------------------------------------------
class String
{
public:
    static const int IndexNotFound = -1;


private:
    // 現状はインスタンス生成できる意味がないので禁止します。
    String() NN_NOEXCEPT { }

public:
    //! 文字列をコピーします。
    //! @param[in] dest コピー先です。
    //! @param[in] destLength コピー先に格納可能な文字数です。
    //! @param[in] source コピー元です。
    //! @param[in] sourceLength コピーする文字列長です。
    static void Copy(char* dest, uint32_t destLength, const char* source, uint32_t sourceLength) NN_NOEXCEPT;

    //! 文字列をコピーし、必要なら終端文字を設定します。
    //! @param[in] dest コピー先です。
    //! @param[in] destLength コピー先に格納可能な文字数です。
    //! @param[in] source コピー元です。
    //! @param[in] sourceLength コピーする文字列長です。
    static void SafeCopy(char* dest, uint32_t destLength, const char* source, uint32_t sourceLength) NN_NOEXCEPT;

    //! 指定した文字を文字列の先頭から検索します。
    //! @param[in] source 検索対象の文字列です。
    //! @param[in] targetChar 検索する文字です。
    //! @param[in] maxLength 検索を行う最大文字数です。
    //! @return 最初に見つかったインデックスを返します。見つからないときには IndexNotFound を返します。
    static int IndexOf(const char* source, char targetChar, uint32_t maxLength) NN_NOEXCEPT;

    //! 指定した文字を文字列の末尾から検索します。
    //! @param[in] source 検索対象の文字列です。
    //! @param[in] targetChar 検索する文字です。
    //! @param[in] maxLength 検索を行う最大文字数です。
    //! @return 最後に見つかったインデックスを返します。見つからないときには IndexNotFound を返します。
    static int LastIndexOf(const char* source, char targetChar, int maxLength) NN_NOEXCEPT;
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
