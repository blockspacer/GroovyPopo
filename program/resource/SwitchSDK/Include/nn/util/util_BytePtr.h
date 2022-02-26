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
#include <nn/util/util_BitUtil.h>

namespace nn { namespace util {

//! @brief  バイト単位アクセス用ポインタです。@n
//!         void* にバイト単位の移動機能を追加してポインタを模しています。
class BytePtr
{
public:

    //! @name 構築/破棄
    //! @{

    //! @brief      コンストラクタです。
    //! @param[in]  ptr     参照するポインタです。
    explicit BytePtr( void* ptr ) NN_NOEXCEPT
        : m_Ptr( ptr )
    {
    }

    //! @brief      指定したバイト単位のオフセット分ポインタを進めるコンストラクタです。
    //! @param[in]  ptr     参照するポインタです。
    //! @param[in]  offset  バイト単位のオフセット値です。
    BytePtr( void* ptr, ptrdiff_t offset ) NN_NOEXCEPT
        : m_Ptr( ptr )
    {
        Advance( offset );
    }

    //! @brief      参照しているポインタを差し替えます。
    //! @param[in]  ptr     差し替えるポインタです。
    void Reset( void* ptr ) NN_NOEXCEPT
    {
        m_Ptr = ptr;
    }

    //! @}

    //! @{
    //! @name ポインタの取得

    //! @brief  参照しているポインタを返します。
    //! @return 参照しているポインタです。
    void* Get() const NN_NOEXCEPT
    {
        return m_Ptr;
    }

    //! @brief  参照しているポインタを指定の型で返します。
    //! @return 参照しているポインタです。
    template< typename T >
    T* Get() const NN_NOEXCEPT
    {
        return static_cast< T* >( m_Ptr );
    }

    //! @}

    //! @name ポインタの移動操作
    //! @{

    //! @brief      指定したバイト単位のオフセット分ポインタを進めます。
    //! @param[in]  offset  バイト単位のオフセット値です。
    //! @return     *this を返します。
    BytePtr& Advance( ptrdiff_t offset ) NN_NOEXCEPT
    {
        m_Ptr = Get< char >() + offset;
        return *this;
    }

    //! @brief      指定したポインタとの差分をバイト単位で返します。
    //! @param[in]  ptr     差分を取得するポインタです。
    //! @return     バイト単位の差分を示す数値を返します。
    ptrdiff_t Distance( const void* ptr ) const NN_NOEXCEPT
    {
        return static_cast< const char* >( ptr ) - Get< char >();
    }

    //! @brief      参照中のポインタが指定したアライメントに揃っているかどうかを返します。
    //! @param[in]  alignment   指定するアライメントです。
    //! @return     指定のアライメントに揃っていれば true を返します。
    bool IsAligned( size_t alignment ) const NN_NOEXCEPT
    {
        uintptr_t ptr = reinterpret_cast< uintptr_t >( m_Ptr );
        return is_aligned( ptr, alignment );
    }

    //! @brief      指定したアライメントまで切り上げます。
    //! @param[in]  alignment   指定するアライメントです。
    //! @return     *this を返します。
    BytePtr& AlignUp( size_t alignment ) NN_NOEXCEPT
    {
        uintptr_t& ptr = reinterpret_cast< uintptr_t& >( m_Ptr );
        ptr = align_up( ptr, alignment );
        return *this;
    }

    //! @brief      指定したアライメントまで切り下げます。
    //! @param[in]  alignment   指定するアライメントです。
    //! @return     *this を返します。
    BytePtr& AlignDown( size_t alignment ) NN_NOEXCEPT
    {
        uintptr_t& ptr = reinterpret_cast< uintptr_t& >( m_Ptr );
        ptr = align_down( ptr, alignment );
        return *this;
    }

    //! @brief      指定したバイト単位のオフセット分ポインタを進めます。
    //! @param[in]  offset  バイト単位のオフセット値です。
    //! @return     *this を返します。
    BytePtr& operator+=( ptrdiff_t offset ) NN_NOEXCEPT
    {
        return Advance( offset );
    }

    //! @brief      指定したバイト単位のオフセット分ポインタを戻します。
    //! @param[in]  offset  バイト単位のオフセット値です。
    //! @return     *this を返します。
    BytePtr& operator-=( ptrdiff_t offset ) NN_NOEXCEPT
    {
        return Advance( -offset );
    }

    //! @brief      指定したバイト単位のオフセット分進んだポインタを返します。
    //! @param[in]  offset  バイト単位のオフセット値です。
    //! @return     オフセット分進んだポインタを返します。
    BytePtr operator+( ptrdiff_t offset ) const NN_NOEXCEPT
    {
        return BytePtr( *this ).Advance( offset );
    }

    //! @brief      指定したバイト単位のオフセット分戻ったポインタを返します。
    //! @param[in]  offset  バイト単位のオフセット値です。
    //! @return     オフセット分戻ったポインタを返します。
    BytePtr operator-( ptrdiff_t offset ) const NN_NOEXCEPT
    {
        return BytePtr( *this ).Advance( -offset );
    }

    //! @brief  ポインタを 1 バイト進めます。
    //! @return 参照中のポインタを返し、1 バイト進めます。
    BytePtr operator++( int ) NN_NOEXCEPT
    {
        BytePtr self( *this );
        Advance( 1 );
        return self;
    }

    //! @brief  ポインタを 1 バイト進めます。
    //! @return ポインタを 1 バイト進めた後、*this を返します。
    BytePtr& operator++() NN_NOEXCEPT
    {
        return Advance( 1 );
    }

    //! @brief  ポインタを 1 バイト戻します。
    //! @return 参照中のポインタを返し、1 バイト戻します。
    BytePtr operator--( int ) NN_NOEXCEPT
    {
        BytePtr self( *this );
        Advance( -1 );
        return self;
    }

    //! @brief  ポインタを 1 バイト戻します。
    //! @return ポインタを 1 バイト戻した後、*this を返します。
    BytePtr& operator--() NN_NOEXCEPT
    {
        return Advance( -1 );
    }

    //! @}

private:
    void* m_Ptr;
};

//! @brief  不変なバイト単位アクセス用ポインタです。@n
//!         const void* にバイト単位の移動機能を追加してポインタを模しています。
class ConstBytePtr
{
public:

    //! @name 構築/破棄
    //! @{

    //! @brief      コンストラクタです。
    //! @param[in]  ptr     参照するポインタです。
    explicit ConstBytePtr( const void* ptr ) NN_NOEXCEPT
        : m_Ptr( ptr )
    {
    }

    //! @brief      コンストラクタです。
    //! @param[in]  ptr     参照するバイト単位アクセス用のポインタです。
    NN_IMPLICIT ConstBytePtr( const BytePtr& ptr ) NN_NOEXCEPT
        : m_Ptr( ptr.Get() )
    {
    }

    //! @brief      指定したバイト単位のオフセット分ポインタを進めるコンストラクタです。
    //! @param[in]  ptr     参照するポインタです。
    //! @param[in]  offset  バイト単位のオフセット値です。
    ConstBytePtr( const void* ptr, ptrdiff_t offset ) NN_NOEXCEPT
        : m_Ptr( ptr )
    {
        Advance( offset );
    }

    //! @brief      参照しているポインタを差し替えます。
    //! @param[in]  ptr     差し替えるポインタです。
    void Reset( const void* ptr ) NN_NOEXCEPT
    {
        m_Ptr = ptr;
    }

    //! @}

    //! @{
    //! @name ポインタの取得

    //! @brief  参照しているポインタを返します。
    //! @return 参照しているポインタです。
    const void* Get() const NN_NOEXCEPT
    {
        return m_Ptr;
    }

    //! @brief  参照しているポインタを指定の型で返します。
    //! @return 参照しているポインタです。
    template< typename T >
    const T* Get() const NN_NOEXCEPT
    {
        return static_cast< const T* >( m_Ptr );
    }

    //! @}

    //! @name ポインタの移動操作
    //! @{

    //! @brief      指定したバイト単位のオフセット分ポインタを進めます。
    //! @param[in]  offset  バイト単位のオフセット値です。
    ConstBytePtr& Advance( ptrdiff_t offset ) NN_NOEXCEPT
    {
        m_Ptr = Get< char >() + offset;
        return *this;
    }

    //! @brief      指定したポインタとの差分をバイト単位で返します。
    //! @param[in]  ptr     差分を取得するポインタです。
    //! @return     バイト単位の差分を示す数値を返します。
    ptrdiff_t Distance( const void* ptr ) const NN_NOEXCEPT
    {
        return static_cast< const char* >( ptr ) - Get< char >();
    }

    //! @brief      参照中のポインタが指定したアライメントに揃っているかどうかを返します。
    //! @param[in]  alignment   指定するアライメントです。
    //! @return     指定のアライメントに揃っていれば true を返します。
    bool IsAligned( size_t alignment ) const NN_NOEXCEPT
    {
        uintptr_t ptr = reinterpret_cast< uintptr_t >( m_Ptr );
        return is_aligned( ptr, alignment );
    }

    //! @brief      指定したアライメントまで切り上げます。
    //! @param[in]  alignment   指定するアライメントです。
    //! @return     *this を返します。
    ConstBytePtr& AlignUp( size_t alignment ) NN_NOEXCEPT
    {
        uintptr_t& ptr = reinterpret_cast< uintptr_t& >( m_Ptr );
        ptr = align_up( ptr, alignment );
        return *this;
    }

    //! @brief      指定したアライメントまで切り下げます。
    //! @param[in]  alignment   指定するアライメントです。
    //! @return     *this を返します。
    ConstBytePtr& AlignDown( size_t alignment ) NN_NOEXCEPT
    {
        uintptr_t& ptr = reinterpret_cast< uintptr_t& >( m_Ptr );
        ptr = align_down( ptr, alignment );
        return *this;
    }

    //! @brief      指定したバイト単位のオフセット分ポインタを進めます。
    //! @param[in]  offset  バイト単位のオフセット値です。
    //! @return     *this を返します。
    ConstBytePtr& operator+=( ptrdiff_t offset ) NN_NOEXCEPT
    {
        return Advance( offset );
    }

    //! @brief      指定したバイト単位のオフセット分ポインタを戻します。
    //! @param[in]  offset  バイト単位のオフセット値です。
    //! @return     *this を返します。
    ConstBytePtr& operator-=( ptrdiff_t offset ) NN_NOEXCEPT
    {
        return Advance( -offset );
    }

    //! @brief      指定したバイト単位のオフセット分進んだポインタを返します。
    //! @param[in]  offset  バイト単位のオフセット値です。
    //! @return     オフセット分進んだポインタを返します。
    ConstBytePtr operator+( ptrdiff_t offset ) const NN_NOEXCEPT
    {
        return ConstBytePtr( *this ).Advance( offset );
    }

    //! @brief      指定したバイト単位のオフセット分戻ったポインタを返します。
    //! @param[in]  offset  バイト単位のオフセット値です。
    //! @return     オフセット分戻ったポインタを返します。
    ConstBytePtr operator-( ptrdiff_t offset ) const NN_NOEXCEPT
    {
        return ConstBytePtr( *this ).Advance( -offset );
    }

    //! @brief  ポインタを 1 バイト進めます。
    //! @return 参照中のポインタを返し、1 バイト進めます。
    ConstBytePtr operator++( int ) NN_NOEXCEPT
    {
        ConstBytePtr self( *this );
        Advance( 1 );
        return self;
    }

    //! @brief  ポインタを 1 バイト進めます。
    //! @return ポインタを 1 バイト進めた後、*this を返します。
    ConstBytePtr& operator++() NN_NOEXCEPT
    {
        return Advance( 1 );
    }

    //! @brief  ポインタを 1 バイト戻します。
    //! @return 参照中のポインタを返し、1 バイト戻します。
    ConstBytePtr operator--( int ) NN_NOEXCEPT
    {
        ConstBytePtr self( *this );
        Advance( -1 );
        return self;
    }

    //! @brief  ポインタを 1 バイト戻します。
    //! @return ポインタを 1 バイト戻した後、*this を返します。
    ConstBytePtr& operator--() NN_NOEXCEPT
    {
        return Advance( -1 );
    }

    //! @}

private:
    const void* m_Ptr;
};

}} // namespace nn::util
