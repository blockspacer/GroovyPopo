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
#include <nn/nn_Macro.h>

namespace nn { namespace atk { namespace detail { namespace fnd {

//! @internal
class WorkBufferAllocator
{
public:
    //! @internal
    //! @brief        コンストラクタです
    //! @param[in]    buffer     ワークバッファのアドレスです
    //! @param[in]    size       ワークバッファのサイズです
    //! @pre
    //!  - buffer != nullptr
    WorkBufferAllocator(void* buffer, size_t size) NN_NOEXCEPT;

    //! @internal
    //! @brief        コンストラクタです
    //! @param[in]    buffer            ワークバッファのアドレスです
    //! @param[in]    size              ワークバッファのサイズです
    //! @param[in]    requiredAlignment ワークバッファが要求するアライメントです
    //! @pre
    //!  - buffer != nullptr
    //!  - buffer は requiredAlignment のアラインに沿っている
    //!  - requiredAlignment は 2 のべき乗である
    WorkBufferAllocator(void* buffer, size_t size, size_t requiredAlignment) NN_NOEXCEPT;

    //! @internal
    //! @brief        アライメント調整を行わずバッファを確保します。
    //!               確保するバッファのサイズが 0 の場合は nullptr を返します。
    //! @param[in]    size      確保するバッファのサイズです
    //! @pre
    //!  - size <= GetFreeSize()
    //! @return       確保したバッファのアドレスです
    void* Allocate(size_t size) NN_NOEXCEPT;

    //! @internal
    //! @brief        アライメント調整を行ったうえで、バッファを確保します。
    //!               確保するバッファのサイズが 0 の場合は nullptr を返します。
    //! @param[in]    size      確保するバッファのサイズです
    //! @param[in]    alignment 確保するバッファのアライメントです
    //! @pre
    //!  - alignment は 2 のべき乗である
    //!  - GetFreeSize() で取得できる値が、指定した size, alignment により決まるサイズ以上である
    //! @return       確保したバッファのアドレスです
    void* Allocate(size_t size, size_t alignment) NN_NOEXCEPT;

    //! @internal
    //! @brief        アライメント調整を行ったうえで、指定サイズのバッファを個数分確保します。
    //!               確保するバッファのサイズが 0 の場合は nullptr を返します。
    //! @param[in]    objectSize オブジェクト 1 個あたり確保するバッファのサイズです
    //! @param[in]    alignment  オブジェクトが要求するアライメントです
    //! @param[in]    count      確保するオブジェクトの個数です
    //! @pre
    //!  - alignment は 2 のべき乗である
    //!  - GetFreeSize() で取得できる値が、指定した size, alignment により決まるサイズ以上である
    //!  - count > 0
    //! @return       確保したバッファのアドレスです
    void* Allocate(size_t objectSize, size_t alignment, int count) NN_NOEXCEPT;

    //! @internal
    //! @brief        クラスインスタンス生成のためのバッファを確保します。
    //! @tparam       T バッファを確保するクラスです
    //! @pre
    //!  - GetFreeSize() で取得できる値が、指定した size, alignment により決まるサイズ以上である
    //! @return       確保したバッファのアドレスです
    template<class T>
    T* Allocate() NN_NOEXCEPT
    {
        return reinterpret_cast<T*>(Allocate(sizeof(T), NN_ALIGNOF(T)));
    }

    //! @internal
    //! @brief        指定個数分のクラスインスタンス生成のためのバッファを確保します。
    //! @tparam       T     バッファを確保するクラスです
    //! @param[in]    count 確保するクラスインスタンスの個数です
    //! @pre
    //!  - GetFreeSize() で取得できる値が、指定した size, alignment により決まるサイズ以上である
    //!  - count > 0
    //! @return       確保したバッファの先頭アドレスです
    template<class T>
    T* Allocate(int count) NN_NOEXCEPT
    {
        return reinterpret_cast<T*>(Allocate(sizeof(T), NN_ALIGNOF(T), count));
    }

    //! @internal
    //! @brief        WorkBufferAllocator が持つワークバッファのサイズを取得します
    //! @return       WorkBufferAllocator が持つワークバッファのサイズです
    size_t GetSize() const NN_NOEXCEPT
    {
        return m_Size;
    }

    //! @internal
    //! @brief        WorkBufferAllocator が今までに確保したサイズを取得します
    //! @return       WorkBufferAllocator が今までに確保したサイズです
    size_t GetAllocSize() const NN_NOEXCEPT
    {
        return m_Offset;
    }

    //! @internal
    //! @brief        WorkBufferAllocator が確保できる残りのサイズを取得します
    //! @return       WorkBufferAllocator が確保できる残りのサイズです
    size_t GetFreeSize() const NN_NOEXCEPT
    {
        return m_Size - m_Offset;
    }

private:
    uintptr_t m_Buffer;
    size_t m_Offset;
    size_t m_Size;
};

}}}}
