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

#include <algorithm>
#include <nn/util/util_BitUtil.h>
#include <nn/util/util_BytePtr.h>
#include <nn/util/util_BinTypes.h>

namespace nn { namespace util {

//! @brief メモリ分配計算
class MemorySplitter
{
private:
    static const size_t DefaultAlignment = sizeof( BinPtr );

public:
    class MemoryBlock;

    //! @brief コンストラクタです。
    MemorySplitter() NN_NOEXCEPT
    {
        Initialize();
    }

    //! @brief 状態を初期化します。
    void Initialize() NN_NOEXCEPT
    {
        m_Size = 0;
        m_Alignment = DefaultAlignment;
    }

    //! @brief 状態を無効化します。
    void Invalidate() NN_NOEXCEPT
    {
        m_Size = 0;
        m_Alignment = 0;
    }

    //! @brief  状態が有効かどうかを取得します。
    //! @return 状態が有効であれば true を返します。
    bool IsValid() const NN_NOEXCEPT
    {
        return m_Alignment > 0;
    }

    //! @brief      メモリブロックを追加します。
    //! @param[in]  pBlock   追加するメモリブロックです。
    //! @return     *this を返します。
    MemorySplitter& Append( MemoryBlock* pBlock ) NN_NOEXCEPT;

    //! @brief  メモリの合計サイズを返します。
    //! @return 結果を返します。
    size_t GetSize() const NN_NOEXCEPT
    {
        return m_Size;
    }

    //! @brief  メモリの最大アライメントを返します。
    //! @return 結果を返します。
    size_t GetAlignment() const NN_NOEXCEPT
    {
        return m_Alignment;
    }

private:
    size_t m_Size;
    size_t m_Alignment;
};

//! @brief メモリブロック
class MemorySplitter::MemoryBlock
{
    friend class MemorySplitter;

public:
    static const ptrdiff_t Npos = -1;

    //! @brief  コンストラクタです。
    MemoryBlock() NN_NOEXCEPT
    {
        Initialize();
    }

    //! @brief  状態を初期化します。
    void Initialize() NN_NOEXCEPT
    {
        m_Size = 0;
        m_Alignment = DefaultAlignment;
        m_Bias = 0;
        m_Position = Npos;
    }

    //! @brief      ブロックのサイズを設定します。
    //! @param[in]  size    ブロックに設定するサイズです。
    void SetSize( size_t size ) NN_NOEXCEPT
    {
        m_Size = size;
    }

    //! @brief  型を元にブロックのサイズを設定します。
    //! @tparam T   サイズ計算の元になる型です。
    template< typename T >
    void SetSizeBy() NN_NOEXCEPT
    {
        SetSizeBy<T>( DefaultAlignment, 1 );
    }

    //! @brief      型を元にブロックのサイズを設定します。指定のアライメントで切り上げて設定します。
    //! @tparam     T   ブロックのサイズ計算の元になる型です。
    //! @param[in]  alignment   ブロックのサイズを切り上げるアライメントです。
    template< typename T >
    void SetSizeBy( size_t alignment ) NN_NOEXCEPT
    {
        SetSizeBy<T>( alignment, 1 );
    }

    //! @brief      型を元にブロックのサイズを設定します。配列長分のサイズを指定のアライメントで切り上げて設定します。
    //! @tparam     T   ブロックのサイズ計算の元になる型です。
    //! @param[in]  alignment   ブロックのサイズを切り上げるアライメントです。
    //! @param[in]  length      配列の長さです。
    template< typename T >
    void SetSizeBy( size_t alignment, int length ) NN_NOEXCEPT
    {
        m_Size = nn::util::align_up( sizeof( T ) * length, alignment );
    }

    //! @brief      ブロックを配置するアライメントとアライメントに先行するバイアスを設定します。
    //! @param[in]  alignment   ブロックを配置するアライメントです。
    void SetAlignment( size_t alignment ) NN_NOEXCEPT
    {
        SetAlignment( alignment, 0 );
    }

    //! @brief      ブロックを配置するアライメントとアライメントに先行するバイアスを設定します。
    //! @param[in]  alignment   ブロックを配置するアライメントです。
    //! @param[in]  bias        アライメントに先行する領域のサイズです。
    void SetAlignment( size_t alignment, size_t bias ) NN_NOEXCEPT
    {
        m_Alignment = alignment;
        m_Bias = bias;
    }

    //! @brief ブロックのサイズを取得します。
    size_t GetSize() const NN_NOEXCEPT
    {
        return m_Size;
    }

    //! @brief ブロックを配置するアライメントを取得します。
    size_t GetAlignment() const NN_NOEXCEPT
    {
        return m_Alignment;
    }

    //! @brief 指定したメモリにおけるブロックのポインタを返します。
    void* Get( void* pHead ) const NN_NOEXCEPT
    {
        return Get< void >( pHead );
    }

    //! @brief 指定したメモリにおけるブロックのポインタを返します。
    template< typename T >
    T* Get( void* pHead ) const NN_NOEXCEPT
    {
        if ( pHead == NULL || m_Size == 0 )
        {
            return NULL;
        }

        NN_SDK_ASSERT( m_Position != Npos ); // MemorySplitter::Append() を呼ぶ必要があります。

        return nn::util::BytePtr( pHead, m_Position ).Get< T >();
    }

    //! @brief  MemorySplitter への登録後に MemorySplitter 内の自身の位置を取得します。@n
    //!         MemorySplitter に登録されていない状態では MemoryBlock::NPos を返します。
    ptrdiff_t GetPosition() const NN_NOEXCEPT
    {
        return m_Position;
    }

private:
    size_t m_Size;
    size_t m_Alignment;
    size_t m_Bias;
    ptrdiff_t m_Position;
};

inline
MemorySplitter& MemorySplitter::Append( MemoryBlock* pBlock ) NN_NOEXCEPT
{
    NN_SDK_ASSERT_NOT_NULL( pBlock );
    NN_SDK_ASSERT( pBlock->m_Position == MemoryBlock::Npos, "Append must not be called twice." );
    NN_SDK_ASSERT( IsValid(), "Initialize() has not called after Invalidate()." );

    if ( pBlock->m_Size == 0 )
    {
        return *this; // サイズが 0 の場合はアライメントも考慮しません。
    }

    // アライメントの最大値を計算します。
    m_Alignment = std::max NN_PREVENT_MACRO_FUNC ( m_Alignment, pBlock->m_Alignment );

    // バイアス分の領域をアライメントの前に配置します。
    m_Size += pBlock->m_Bias;
    m_Size = nn::util::align_up( m_Size, pBlock->m_Alignment );
    pBlock->m_Position = static_cast< ptrdiff_t >( m_Size - pBlock->m_Bias );

    // ブロックのサイズを加算します。
    m_Size = pBlock->m_Position + pBlock->m_Size;

    return *this;
}

}} // namespace nn::util
