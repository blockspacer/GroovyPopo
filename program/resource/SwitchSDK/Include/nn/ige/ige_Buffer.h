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
#include <nn/nn_SdkAssert.h>

namespace nn { namespace ige {

//! @briefprivate   書き込み可能なバッファのポインタとサイズを保持して各種インタフェースで使用するクラスです。
//! @details バッファのポインタとサイズを保持して各種インタフェースで使用します。
//! イテレータと同様にクラス自身が const であっても非 const ポインタを返します。
//! インタフェースで使用する際はバッファが不変であるという誤解を避けるため値渡しで使います。
//! 処理速度がクリティカルでない箇所で使われることを想定しています。
class BufferReference
{
public:

    //! @name 構築/破棄
    //! @{

    //! @briefprivate コンストラクタです。
    BufferReference() NN_NOEXCEPT
        : m_Ptr(NULL)
        , m_Size(0)
    {
    }

    //! @briefprivate       コンストラクタです。
    //! @param[in]  ptr     参照先のバッファの先頭のポインタです。
    //! @param[in]  size    参照先のバッファのサイズです。
    BufferReference(void* ptr, size_t size) NN_NOEXCEPT
    {
        Set(ptr, size);
    }

    //! @briefprivate       コンストラクタです。
    //! @param[in]  buffer  参照先のバッファです。
    template<typename T, int N>
    NN_IMPLICIT BufferReference(T(&buffer)[N]) NN_NOEXCEPT
    {
        Set(buffer);
    }

    //! @briefprivate       参照先のバッファを指定して構築します。
    //! @param[in]  ptr     参照先のバッファの先頭のポインタです。
    //! @param[in]  size    参照先のバッファのサイズです。
    void Set(void* ptr, size_t size) NN_NOEXCEPT
    {
        NN_SDK_ASSERT(ptr != NULL || size == 0, "ptr: 0x%x, size: %d", ptr, size);
        m_Ptr = ptr;
        m_Size = size;
    }

    //! @briefprivate       参照先のバッファを指定して構築します。
    //! @param[in]  buffer  参照先のバッファです。
    template<typename T, int N>
    void Set(T(&buffer)[N]) NN_NOEXCEPT
    {
        m_Ptr = buffer;
        m_Size = sizeof(buffer);
    }

    //! @}

    //! @name 取得
    //! @{

    //! @briefprivate  バッファの先頭のポインタを返します。
    //! @return バッファの先頭のポインタです。
    void* GetPtr() const NN_NOEXCEPT
    {
        return m_Ptr;
    }

    //! @briefprivate  バッファのサイズを返します。
    //! @return バッファのサイズです。
    size_t GetSize() const NN_NOEXCEPT
    {
        return m_Size;
    }

    //! @}

private:
    void* m_Ptr;
    size_t m_Size;
};

//! @briefprivate 読み取り専用のバッファのポインタとサイズを保持して各種インタフェースで使用するクラスです。
//! @details バッファのポインタとサイズを保持して各種インタフェースで使用します。
//! イテレータと同様にクラス自身が非 const であっても const ポインタを返します。
//! 処理速度がクリティカルでない箇所で使われることを想定しています。
class BufferView
{
public:

    //! @name 構築/破棄
    //! @{

    //! @briefprivate コンストラクタです。
    BufferView() NN_NOEXCEPT
        : m_Ptr(NULL)
        , m_Size(0)
    {
    }

    //! @briefprivate       コンストラクタです。
    //! @param[in]  ptr     参照先のバッファの先頭のポインタです。
    //! @param[in]  size    参照先のバッファのサイズです。
    BufferView(const void* ptr, size_t size) NN_NOEXCEPT
    {
        Set(ptr, size);
    }

    //! @briefprivate       コンストラクタです。
    //! @param[in]  buffer  参照先のバッファです。
    template<typename T, int N>
    NN_IMPLICIT BufferView(T(&buffer)[N]) NN_NOEXCEPT
    {
        Set(buffer);
    }

    //! @briefprivate           コンストラクタです。
    //! @param[in]  reference   バッファの参照です。
    NN_IMPLICIT BufferView(const BufferReference& reference) NN_NOEXCEPT
    {
        Set(reference.GetPtr(), reference.GetSize());
    }

    //! @briefprivate       参照先のバッファを指定して構築します。
    //! @param[in]  ptr     参照先のバッファの先頭のポインタです。
    //! @param[in]  size    参照先のバッファのサイズです。
    void Set(const void* ptr, size_t size) NN_NOEXCEPT
    {
        NN_SDK_ASSERT(ptr != NULL || size == 0, "ptr: 0x%x, size: %d", ptr, size);
        m_Ptr = ptr;
        m_Size = size;
    }

    //! @briefprivate       参照先のバッファを指定して構築します。
    //! @param[in]  buffer  参照先のバッファです。
    template<typename T, int N>
    void Set(T(&buffer)[N]) NN_NOEXCEPT
    {
        m_Ptr = buffer;
        m_Size = sizeof(buffer);
    }

    //! @}

    //! @name 取得
    //! @{

    //! @briefprivate  バッファのポインタを返します。
    //! @return バッファの先頭のポインタです。
    const void* GetPtr() const NN_NOEXCEPT
    {
        return m_Ptr;
    }

    //! @briefprivate  バッファのサイズを返します。
    //! @return バッファのサイズです。
    size_t GetSize() const NN_NOEXCEPT
    {
        return m_Size;
    }

    //! @}

private:
    const void* m_Ptr;
    size_t m_Size;
};

}} // namespace nn::ige
