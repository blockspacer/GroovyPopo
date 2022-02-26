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
#include <nn/nn_SdkAssert.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief    固定長文字列を操作、管理します。
//!
//! @details  HACK : このクラスは実装途中です。
//!                  必要になった機能を随時実装していきます。
//---------------------------------------------------------------------------
class FixedString
{
public:
    /* ctor */ FixedString() NN_NOEXCEPT;
    /* ctor */ FixedString(char* buffer, int bufferLength) NN_NOEXCEPT;

public:
    //! @brief  文字列バッファをアタッチします。
    //! @param[in] buffer 文字列バッファです。
    //! @param[in] bufferLength 文字列バッファの長さです。
    void Attach(char* buffer, int bufferLength) NN_NOEXCEPT;

    //! @brief  文字列バッファをデタッチします。
    void Detach() NN_NOEXCEPT;

    //! @brief  アタッチの有無を調べます。
    //! @return 文字列バッファがアタッチされていれば true を返します。
    bool IsAttached() const NN_NOEXCEPT { return m_Buffer != NULL; }

    //! @brief  文字列バッファの長さを取得します。
    //! @return 文字列バッファの長さを返します。
    int GetBufferLength() const NN_NOEXCEPT { return m_BufferLength; }

    //! @brief  文字列の長さを取得します。
    //! @return 文字列の長さを返します。
    int GetLength() const NN_NOEXCEPT { return m_Buffer == NULL ? 0 : static_cast<int>(std::strlen(m_Buffer)); }

    //! @brief  文字列ポインタを取得します。
    //! @return 文字列ポインタを返します。
    char* GetValue() NN_NOEXCEPT { return m_Buffer; }

    //! @brief  文字列ポインタを取得します。
    //! @return 文字列ポインタを返します。
    const char* GetValue() const NN_NOEXCEPT { return m_Buffer; }

    //! @brief  指定文字列を設定（コピー）して終端文字を付加します。
    //! @param[in] value 文字列です。
    //! @param[in] length コピーする文字列長です。
    //! @param[in] index コピー先のインデックスです。
    void CopyFrom(const char* value, int length, int index = 0) NN_NOEXCEPT;

    //! @brief  指定文字列を設定（コピー）します。
    //! @param[in] value 文字列
    //! @param[in] length コピーする文字列長
    //! @param[in] index コピー先のインデックス
    void CopyPartFrom(const char* value, int length, int index = 0) NN_NOEXCEPT;

    //! @brief  インデクサです。
    //! @param[in] index インデックスです。
    //! @return 文字への参照を返します。
    char& operator[](int index) NN_NOEXCEPT
    {
        NN_SDK_ASSERT(index < m_BufferLength, "invalid index.\n");
        return m_Buffer[index];
    }

private:
    char* m_Buffer;         //!< 文字列バッファです。
    int   m_BufferLength;   //!< バッファの長さです。
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
