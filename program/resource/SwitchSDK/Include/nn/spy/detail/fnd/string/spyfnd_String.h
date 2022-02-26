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

#include <cstring>
#include <cstdarg>
#include <cstdio>

#include <nn/util/util_FormatString.h>

namespace nn {
namespace spy {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @brief    文字列を操作、管理します。
//!
//! @details  HACK : このクラスは実装途中です。
//!                  必要になった機能だけを随時実装していきます。
//---------------------------------------------------------------------------
class String
{
private:
    // 現状はインスタンス生成できる意味がないので禁止します。
    String() NN_NOEXCEPT { }

public:
    //! @brief 文字列をコピーします。
    //! コピー先の文字列は必ずヌル文字で終端されます。
    //! @param[in] buffer コピー先です。
    //! @param[in] bufferLength コピー先に格納可能な文字数です。終端のヌル文字を含みます。
    //! @param[in] src コピー元です。
    static void Copy(char* buffer, int bufferLength, const char* src) NN_NOEXCEPT;

    //! @brief 文字列をコピーします。
    //! コピー先の文字列は必ずヌル文字で終端されます。
    //! @tparam BufferLength コピー先に格納可能な文字数です。終端のヌル文字を含みます。
    //! @param[in] buffer コピー先です。
    //! @param[in] src コピー元です。
    template <int BufferLength>
    static inline void Copy(char (&buffer)[BufferLength], const char* src) NN_NOEXCEPT
    {
        Copy(buffer, BufferLength, src);
    }

    //---------------------------------------------------------------------------
    static inline int
    Vsnprintf(char* buffer, std::size_t bufferLength, const char* format, std::va_list vlist) NN_NOEXCEPT
    {
        return nn::util::VSNPrintf(buffer, bufferLength, format, vlist);
    }
};

} // namespace nn::spy::detail::fnd
} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn
