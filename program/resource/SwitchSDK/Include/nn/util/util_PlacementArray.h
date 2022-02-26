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

#include <utility>
#include <algorithm>
#include <nn/nn_Common.h>
#include <nn/nn_SdkAssert.h>
#include <nn/nn_Abort.h>
#include <nn/util/detail/util_TypeTraits.h>

namespace nn { namespace util {

//! @brief 固定長配列です。
template< typename T >
class PlacementArray
{
    NN_DISALLOW_COPY( PlacementArray );
public:
    //! @brief 要素の型です。
    typedef T value_type;

    //! @brief 要素のポインタ型です。
    typedef value_type* pointer;

    //! @brief 不変な要素のポインタ型です。
    typedef typename nn::util::detail::AddConst< value_type >::Type* const_pointer;

    //! @brief 要素の参照型です。
    typedef value_type& reference;

    //! @brief 不変な要素の参照型です。
    typedef typename nn::util::detail::AddConst< value_type >::Type& const_reference;

    //! @brief イテレータです。
    typedef pointer iterator;

    //! @brief 不変なイテレータです。
    typedef const_pointer const_iterator;

    //! @brief 逆イテレータです。
    typedef std::reverse_iterator<iterator> reverse_iterator;

    //! @brief 不変な逆イテレータです。
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    //! @brief イテレータのカテゴリです。
    typedef std::random_access_iterator_tag iterator_category;

    //! @brief サイズの型です。
    typedef int size_type;

    //! @brief 差分の型です。
    typedef int difference_type;

    //! @name 構築/破棄
    //! @{

    //! @brief      コンストラクタです。
    //! @details    @ref ResetWorkMemory( void*, size_t, size_type ) でメモリ領域を割り当てるまで使用することはできません。
    PlacementArray() NN_NOEXCEPT
        : m_pBuf( NULL )
        , m_Len( 0 )
    {
    }

    //! @brief      コンストラクタです。
    //! @param[in]  workMemory      割り当てるメモリ領域の先頭のポインタです。
    //! @param[in]  workMemorySize  割り当てるメモリ領域のサイズです。
    //! @param[in]  length          配列の要素数です。
    //! @pre        workMemory が T 型のアライメント要求を満たしている。
    PlacementArray( void* workMemory, size_t workMemorySize, size_type length ) NN_NOEXCEPT
        : m_pBuf( NULL )
        , m_Len( 0 )
    {
        ResetWorkMemory( workMemory, workMemorySize, length );
    }

    //! @brief      ムーブコンストラクタです。
    //! @param[in]  other   移動する対象の PlacementArray
    PlacementArray( PlacementArray&& other ) NN_NOEXCEPT
        : m_pBuf( other.m_pBuf )
        , m_Len( other.m_Len )
    {
        other.m_pBuf = NULL;
        other.m_Len = 0;
    }

    //! @brief デストラクタです。
    ~PlacementArray() NN_NOEXCEPT
    {
        DestructElements();
    }

    //! @brief      指定した数の要素を格納するのに必要なメモリ領域のバイトサイズを返します。
    //! @param[in]  length  配列の要素数です。
    //! @return     結果を返します。
    static size_t CalculateWorkMemorySize( size_type length ) NN_NOEXCEPT
    {
        return sizeof( T ) * length;
    }

    //! @brief  割り当て中のメモリ領域を取り外し、0 要素に再初期化します。
    //! @return 直前まで割り当てられていたメモリ領域の先頭のポインタを返します。
    void* ResetWorkMemory() NN_NOEXCEPT
    {
        DestructElements();

        void* ptr = m_pBuf;
        m_pBuf = NULL;
        m_Len = 0;

        return ptr;
    }

    //! @brief      新たにメモリ領域を割り当てて再初期化します。
    //! @param[in]  workMemory      割り当てるメモリ領域の先頭のポインタです。
    //! @param[in]  workMemorySize  割り当てるメモリ領域のサイズです。
    //! @param[in]  length      配列の要素数です。
    //! @return     直前まで割り当てられていたメモリ領域の先頭のポインタを返します。
    //! @pre        CalculateWorkMemorySize( length ) <= workMemorySize
    //! @pre        workMemory が T 型のアライメント要求を満たしている。
    void* ResetWorkMemory( void* workMemory, size_t workMemorySize, size_type length ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( CalculateWorkMemorySize( length ) <= workMemorySize );
        NN_SDK_REQUIRES( is_aligned( reinterpret_cast< uintptr_t >( workMemory ), NN_ALIGNOF( T ) ) );
        NN_UNUSED( workMemorySize );

        DestructElements();

        void* ptr = m_pBuf;
        m_pBuf = static_cast< pointer >( workMemory );
        m_Len = length;

        ConstructElements();

        return ptr;
    }

    //! @brief      ムーブ代入演算子です。
    //! @param[in]  other   移動する対象の PlacementArray
    //! @pre        this == &other || data() == NULL
    PlacementArray& operator=( PlacementArray&& other ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( this == &other || m_pBuf == NULL );

        if ( this != &other )
        {
            m_pBuf = other.m_pBuf;
            m_Len = other.m_Len;
            other.m_pBuf = NULL;
            other.m_Len = 0;
        }

        return *this;
    }

    //! @}

    //! @name イテレータ
    //! @{

    //! @brief  イテレータの先頭を返します。
    //! @return イテレータの先頭を返します。
    iterator begin() NN_NOEXCEPT
    {
        return m_pBuf;
    }

    //! @brief  イテレータの先頭を返します。
    //! @return イテレータの先頭を返します。
    const_iterator begin() const NN_NOEXCEPT
    {
        return m_pBuf;
    }

    //! @brief  イテレータの終端を返します。
    //! @return イテレータの終端を返します。
    iterator end() NN_NOEXCEPT
    {
        return m_pBuf + m_Len;
    }

    //! @brief  イテレータの終端を返します。
    //! @return イテレータの終端を返します。
    const_iterator end() const NN_NOEXCEPT
    {
        return m_pBuf + m_Len;
    }

    //! @brief  逆イテレータの先頭を返します。
    //! @return 逆イテレータの先頭を返します。
    reverse_iterator rbegin() NN_NOEXCEPT
    {
        return reverse_iterator( end() );
    }

    //! @brief  逆イテレータの先頭を返します。
    //! @return 逆イテレータの先頭を返します。
    const_reverse_iterator rbegin() const NN_NOEXCEPT
    {
        return const_reverse_iterator( end() );
    }

    //! @brief  逆イテレータの終端を返します。
    //! @return 逆イテレータの終端を返します。
    reverse_iterator rend() NN_NOEXCEPT
    {
        return reverse_iterator( begin() );
    }

    //! @brief  逆イテレータの終端を返します。
    //! @return 逆イテレータの終端を返します。
    const_reverse_iterator rend() const NN_NOEXCEPT
    {
        return const_reverse_iterator( begin() );
    }

    //! @brief  不変なイテレータの先頭を返します。
    //! @return 不変なイテレータの先頭を返します。
    const_iterator cbegin() const NN_NOEXCEPT
    {
        return begin();
    }

    //! @brief  不変なイテレータの終端を返します。
    //! @return 不変なイテレータの終端を返します。
    const_iterator cend() const NN_NOEXCEPT
    {
        return end();
    }

    //! @brief  不変な逆イテレータの先頭を返します。
    //! @return 不変な逆イテレータの先頭を返します。
    const_reverse_iterator crbegin() const NN_NOEXCEPT
    {
        return rbegin();
    }

    //! @brief  不変な逆イテレータの終端を返します。
    //! @return 不変な逆イテレータの終端を返します。
    const_reverse_iterator crend() const NN_NOEXCEPT
    {
        return rend();
    }

    //! @}

    //! @name 取得
    //! @{

    //! @brief  格納している要素数を返します。
    //! @return 格納している要素数を返します。
    size_type size() const NN_NOEXCEPT
    {
        return m_Len;
    }

    //! @brief  格納している要素が空かどうかを返します。
    //! @return 格納している要素が空かどうかを返します。
    bool empty() const NN_NOEXCEPT
    {
        return m_Len == 0;
    }

    //! @brief  格納している要素配列の先頭へのポインタを返します。
    //! @return 格納している要素配列の先頭へのポインタを返します。
    pointer data() NN_NOEXCEPT
    {
        return m_pBuf;
    }

    //! @brief  格納している要素配列の先頭へのポインタを返します。
    //! @return 格納している要素配列の先頭へのポインタを返します。
    const_pointer data() const NN_NOEXCEPT
    {
        return m_pBuf;
    }

    //! @}

    //! @name 要素アクセス
    //! @{

    //! @brief      指定したインデックスの要素を返します。
    //! @param[in]  index   取得する要素の位置です。
    //! @return     取得した要素を返します。
    //! @pre        0 <= index < @ref size()
    reference operator[]( size_type index ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE( index, 0, m_Len );
        return m_pBuf[ index ];
    }

    //! @brief      指定したインデックスの要素を返します。
    //! @param[in]  index   取得する要素の位置です。
    //! @return     取得した要素を返します。
    //! @pre        0 <= index < @ref size()
    const_reference operator[]( size_type index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE( index, 0, m_Len );
        return m_pBuf[ index ];
    }

    //! @brief      指定したインデックスの要素を返します。
    //! @param[in]  index   取得する要素の位置です。
    //! @return     取得した要素を返します。
    //! @pre        0 <= index < @ref size()
    //! @details    範囲外のインデックスを指定した場合はアサートにかかります。@n
    //!             アサートが無効の際に範囲外のインデックスを指定した場合はアボートします。
    reference at( size_type index ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE( index, 0, m_Len );
        NN_ABORT_UNLESS_RANGE( index, 0, m_Len );
        return m_pBuf[ index ];
    }

    //! @brief      指定したインデックスの要素を返します。
    //! @param[in]  index   取得する要素の位置です。
    //! @return     取得した要素を返します。
    //! @pre        0 <= index < @ref size()
    //! @details    範囲外のインデックスを指定した場合はアサートにかかります。@n
    //!             アサートが無効の際に範囲外のインデックスを指定した場合はアボートします。
    const_reference at( size_type index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE( index, 0, m_Len );
        NN_ABORT_UNLESS_RANGE( index, 0, m_Len );
        return m_pBuf[ index ];
    }

    //! @brief  最初の要素を返します。
    //! @return 最初の要素を返します。
    reference front() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( !empty() );
        return m_pBuf[ 0 ];
    }

    //! @brief  最初の要素を返します。
    //! @return 最初の要素を返します。
    const_reference front() const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( !empty() );
        return m_pBuf[ 0 ];
    }

    //! @brief  最後の要素を返します。
    //! @return 最後の要素を返します。
    reference back() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( !empty() );
        return m_pBuf[ m_Len - 1 ];
    }

    //! @brief  最後の要素を返します。
    //! @return 最後の要素を返します。
    const_reference back() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT( !empty() );
        return m_pBuf[ m_Len - 1 ];
    }

    //! @}

private:
    static void Destruct( T& x )
    {
        NN_UNUSED( x ); // VC で未使用変数として警告が出るのを回避します。
        x.~T();
    }

    static void DefaultConstruct( T& x )
    {
        new( &x ) T;
    }

    void ConstructElements()
    {
        std::for_each( begin(), end(), DefaultConstruct );
    }

    void DestructElements()
    {
        std::for_each( begin(), end(), Destruct );
    }

private:
    pointer m_pBuf;
    size_type m_Len;
};

//--------------------------------------------------------------------------------------------------

//! @brief 固定長ポインタ配列です。
template< typename T >
class PtrPlacementArray
{
    NN_DISALLOW_COPY( PtrPlacementArray );

public:
    class iterator;
    class const_iterator;

    //! @brief 要素の型です。
    typedef T value_type;

    //! @brief 要素のポインタ型です。
    typedef value_type* pointer;

    //! @brief 不変な要素のポインタ型です。
    typedef typename nn::util::detail::AddConst< value_type >::Type* const_pointer;

    //! @brief 要素の参照型です。
    typedef value_type& reference;

    //! @brief 不変な要素の参照型です。
    typedef typename nn::util::detail::AddConst< value_type >::Type& const_reference;

    //! @brief 逆イテレータです。
    typedef std::reverse_iterator< iterator > reverse_iterator;

    //! @brief 不変な逆イテレータです。
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;

    //! @brief サイズの型です。
    typedef int size_type;

    //! @brief 差分の型です。
    typedef int difference_type;

    //! @brief イテレータです。
    class iterator
    {
        friend class PtrPlacementArray;
    public:
        typedef typename PtrPlacementArray::value_type value_type;           //!< イテレータが示す要素の型です。
        typedef typename PtrPlacementArray::difference_type difference_type; //!< イテレータの差分の型です。
        typedef typename PtrPlacementArray::pointer pointer;                 //!< 要素のポインタ型です。
        typedef typename PtrPlacementArray::reference reference;             //!< 要素の参照型です。
        typedef std::random_access_iterator_tag iterator_category;  //!< イテレータのカテゴリです。

        //! @name 型変換
        //! @{

        //! @brief  const_iterator キャスト演算子のオーバーロードです。
        NN_IMPLICIT operator const_iterator() const NN_NOEXCEPT
        {
            return const_iterator( m_pImpl );
        }

        //! @}

        //! @brief イテレータを進めます。
        iterator& operator++() NN_NOEXCEPT
        {
            ++m_pImpl;
            return *this;
        }

        //! @brief イテレータを進めます。
        iterator operator++( int ) NN_NOEXCEPT
        {
            iterator iter( *this );
            ++m_pImpl;
            return iter;
        }

        //! @brief イテレータを戻します。
        iterator& operator--() NN_NOEXCEPT
        {
            --m_pImpl;
            return *this;
        }

        //! @brief イテレータを戻します。
        iterator operator--( int ) NN_NOEXCEPT
        {
            iterator iter( *this );
            --m_pImpl;
            return iter;
        }

        //! @brief イテレータを進めます。
        iterator& operator+=( difference_type rhs ) NN_NOEXCEPT
        {
            m_pImpl += rhs;
            return *this;
        }

        //! @brief イテレータを進めます。
        iterator operator+( difference_type rhs ) const NN_NOEXCEPT
        {
            return iterator( *this ) += rhs;
        }

        //! @brief イテレータを戻します。
        iterator& operator-=( difference_type rhs ) NN_NOEXCEPT
        {
            m_pImpl -= rhs;
            return *this;
        }

        //! @brief イテレータを戻します。
        iterator operator-( difference_type rhs ) const NN_NOEXCEPT
        {
            return iterator( *this ) -= rhs;
        }

        //! @brief イテレータ間の距離を取得します。
        difference_type operator-( const iterator& rhs ) const NN_NOEXCEPT
        {
            return static_cast< difference_type >( m_pImpl - rhs.m_pImpl );
        }

        //! @brief イテレータが等しいかを取得します。
        bool operator==( const iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pImpl == rhs.m_pImpl;
        }

        //! @brief イテレータが等しくないかを取得します。
        bool operator!=( const iterator& rhs ) const NN_NOEXCEPT
        {
            return !( m_pImpl == rhs.m_pImpl );
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator<( const iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pImpl - rhs.m_pImpl < 0;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator>( const iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pImpl - rhs.m_pImpl > 0;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator<=( const iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pImpl - rhs.m_pImpl <= 0;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator>=( const iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pImpl - rhs.m_pImpl >= 0;
        }

        //! @brief 要素のポインタを取得します。
        pointer ToPtr() const NN_NOEXCEPT
        {
            return reinterpret_cast< pointer >( *m_pImpl );
        }

        //! @brief 要素を取得します。
        reference operator*() const NN_NOEXCEPT
        {
            return *ToPtr();
        }

        //! @brief 要素へアクセスします。
        pointer operator->() const NN_NOEXCEPT
        {
            return ToPtr();
        }

        //! @brief 要素を取得します。
        reference operator[]( difference_type offset ) const NN_NOEXCEPT
        {
            return *( *this + offset );
        }

    private:
        //! @brief      コンストラクタです。
        //! @param[in]  ptr イテレータの元になるポインタです。
        explicit iterator( uintptr_t* ptr ) NN_NOEXCEPT
            : m_pImpl( ptr )
        {
        }

    private:
        uintptr_t* m_pImpl;
    };

    //! @brief 不変なイテレータです。
    class const_iterator
    {
        friend class PtrPlacementArray;
    public:
        typedef typename nn::util::detail::AddConst< typename PtrPlacementArray::value_type >::Type value_type;    //!< イテレータが示す要素の型です。
        typedef typename PtrPlacementArray::difference_type difference_type; //!< イテレータの差分の型です。
        typedef typename PtrPlacementArray::const_pointer pointer;           //!< 要素のポインタ型です。
        typedef typename PtrPlacementArray::const_reference reference;       //!< 要素の参照型です。
        typedef std::random_access_iterator_tag iterator_category;  //!< イテレータのカテゴリです。

        //! @brief イテレータを進めます。
        const_iterator& operator++() NN_NOEXCEPT
        {
            ++m_pImpl;
            return *this;
        }

        //! @brief イテレータを進めます。
        const_iterator operator++( int ) NN_NOEXCEPT
        {
            const_iterator iter( *this );
            ++m_pImpl;
            return iter;
        }

        //! @brief イテレータを戻します。
        const_iterator& operator--() NN_NOEXCEPT
        {
            --m_pImpl;
            return *this;
        }

        //! @brief イテレータを戻します。
        const_iterator operator--( int ) NN_NOEXCEPT
        {
            const_iterator iter( *this );
            --m_pImpl;
            return iter;
        }

        //! @brief イテレータを進めます。
        const_iterator& operator+=( difference_type rhs ) NN_NOEXCEPT
        {
            m_pImpl += rhs;
            return *this;
        }

        //! @brief イテレータを進めます。
        const_iterator operator+( difference_type rhs ) const NN_NOEXCEPT
        {
            return const_iterator( *this ) += rhs;
        }

        //! @brief イテレータを戻します。
        const_iterator& operator-=( difference_type rhs ) NN_NOEXCEPT
        {
            m_pImpl -= rhs;
            return *this;
        }

        //! @brief イテレータを戻します。
        const_iterator operator-( difference_type rhs ) const NN_NOEXCEPT
        {
            return const_iterator( *this ) -= rhs;
        }

        //! @brief イテレータ間の距離を取得します。
        difference_type operator-( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return static_cast< difference_type >( m_pImpl - rhs.m_pImpl );
        }

        //! @brief イテレータが等しいかを取得します。
        bool operator==( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pImpl == rhs.m_pImpl;
        }

        //! @brief イテレータが等しくないかを取得します。
        bool operator!=( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return !( m_pImpl == rhs.m_pImpl );
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator<( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pImpl - rhs.m_pImpl < 0;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator>( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pImpl - rhs.m_pImpl > 0;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator<=( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pImpl - rhs.m_pImpl <= 0;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator>=( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pImpl - rhs.m_pImpl >= 0;
        }

        //! @brief 要素のポインタを取得します。
        pointer ToPtr() const NN_NOEXCEPT
        {
            return reinterpret_cast< pointer >( *m_pImpl );
        }

        //! @brief 要素を取得します。
        reference operator*() const NN_NOEXCEPT
        {
            return *ToPtr();
        }

        //! @brief 要素へアクセスします。
        pointer operator->() const NN_NOEXCEPT
        {
            return ToPtr();
        }

        //! @brief 要素を取得します。
        reference operator[]( difference_type offset ) const NN_NOEXCEPT
        {
            return *( *this + offset );
        }

    private:
        explicit const_iterator( const uintptr_t* ptr ) NN_NOEXCEPT
            : m_pImpl( ptr )
        {
        }

    private:
        const uintptr_t* m_pImpl;
    };

    //! @name 構築/破棄
    //! @{

    //! @brief  コンストラクタです。
    //! @pre    @ref ResetWorkMemory( void*, size_t, size_type ) でメモリ領域を割り当てるまで使用することはできません。
    PtrPlacementArray() NN_NOEXCEPT
        : m_Impl()
    {
    }

    //! @brief      コンストラクタです。
    //! @param[in]  workMemory      割り当てるメモリ領域の先頭のポインタです。
    //! @param[in]  workMemorySize  割り当てるメモリ領域のサイズです。
    //! @param[in]  length          配列の要素数です。
    //! @pre        workMemory がポインタ型のアライメント要求を満たしている。
    PtrPlacementArray( void* workMemory, size_t workMemorySize, size_type length ) NN_NOEXCEPT
        : m_Impl( workMemory, workMemorySize, length )
    {
    }

    //! @brief      ムーブコンストラクタです。
    //! @param[in]  other   移動する対象の PtrPlacementArray
    PtrPlacementArray( PtrPlacementArray&& other ) NN_NOEXCEPT
        : m_Impl( std::move( other.m_Impl ) )
    {
    }

    //! @brief      指定した数の要素を格納するのに必要なメモリ領域のバイトサイズを返します。
    //! @param[in]  length  配列の要素数です。
    //! @return     結果を返します。
    static size_t CalculateWorkMemorySize( size_type length ) NN_NOEXCEPT
    {
        return PlacementArray< uintptr_t >::CalculateWorkMemorySize( length );
    }

    //! @brief  割り当て中のメモリ領域を取り外し、0 要素に再初期化します。
    //! @return 直前まで割り当てられていたメモリ領域の先頭のポインタを返します。
    void* ResetWorkMemory() NN_NOEXCEPT
    {
        return m_Impl.ResetWorkMemory();
    }

    //! @brief      新たにメモリ領域を割り当てて再初期化します。
    //! @param[in]  workMemory      割り当てるメモリ領域の先頭のポインタです。
    //! @param[in]  workMemorySize  割り当てるメモリ領域のサイズです。
    //! @param[in]  length      配列の要素数です。
    //! @return     直前まで割り当てられていたメモリ領域の先頭のポインタを返します。
    //! @pre        CalculateWorkMemorySize( length ) <= workMemorySize
    //! @pre        workMemory がポインタ型のアライメント要求を満たしている。
    void* ResetWorkMemory( void* workMemory, size_t workMemorySize, size_type length ) NN_NOEXCEPT
    {
        return m_Impl.ResetWorkMemory( workMemory, workMemorySize, length );
    }

    //! @brief      ムーブ代入演算子です。
    //! @param[in]  other   移動する対象の PtrPlacementArray
    //! @pre        this == &other || data() == NULL
    PtrPlacementArray& operator=( PtrPlacementArray&& other ) NN_NOEXCEPT
    {
        if ( this != &other )
        {
            m_Impl = std::move( other.m_Impl );
        }

        return *this;
    }

    //! @}

    //! @name イテレータ
    //! @{

    //! @brief  イテレータの先頭を返します。
    //! @return イテレータの先頭を返します。
    iterator begin() NN_NOEXCEPT
    {
        return iterator( m_Impl.begin() );
    }

    //! @brief  イテレータの先頭を返します。
    //! @return イテレータの先頭を返します。
    const_iterator begin() const NN_NOEXCEPT
    {
        return const_iterator( m_Impl.begin() );
    }

    //! @brief  イテレータの終端を返します。
    //! @return イテレータの終端を返します。
    iterator end() NN_NOEXCEPT
    {
        return iterator( m_Impl.end() );
    }

    //! @brief  イテレータの終端を返します。
    //! @return イテレータの終端を返します。
    const_iterator end() const NN_NOEXCEPT
    {
        return const_iterator( m_Impl.end() );
    }

    //! @brief  逆イテレータの先頭を返します。
    //! @return 逆イテレータの先頭を返します。
    reverse_iterator rbegin() NN_NOEXCEPT
    {
        return reverse_iterator( end() );
    }

    //! @brief  逆イテレータの先頭を返します。
    //! @return 逆イテレータの先頭を返します。
    const_reverse_iterator rbegin() const NN_NOEXCEPT
    {
        return const_reverse_iterator( end() );
    }

    //! @brief  逆イテレータの終端を返します。
    //! @return 逆イテレータの終端を返します。
    reverse_iterator rend() NN_NOEXCEPT
    {
        return reverse_iterator( begin() );
    }

    //! @brief  逆イテレータの終端を返します。
    //! @return 逆イテレータの終端を返します。
    const_reverse_iterator rend() const NN_NOEXCEPT
    {
        return const_reverse_iterator( begin() );
    }

    //! @brief  不変なイテレータの先頭を返します。
    //! @return 不変なイテレータの先頭を返します。
    const_iterator cbegin() const NN_NOEXCEPT
    {
        return begin();
    }

    //! @brief  不変なイテレータの終端を返します。
    //! @return 不変なイテレータの終端を返します。
    const_iterator cend() const NN_NOEXCEPT
    {
        return end();
    }

    //! @brief  不変な逆イテレータの先頭を返します。
    //! @return 不変な逆イテレータの先頭を返します。
    const_reverse_iterator crbegin() const NN_NOEXCEPT
    {
        return rbegin();
    }

    //! @brief  不変な逆イテレータの終端を返します。
    //! @return 不変な逆イテレータの終端を返します。
    const_reverse_iterator crend() const NN_NOEXCEPT
    {
        return rend();
    }

    //! @}

    //! @name 取得
    //! @{

    //! @brief  格納している要素数を返します。
    //! @return 格納している要素数を返します。
    size_type size() const NN_NOEXCEPT
    {
        return m_Impl.size();
    }

    //! @brief  格納している要素が空かどうかを返します。
    //! @return 格納している要素が空かどうかを返します。
    bool empty() const NN_NOEXCEPT
    {
        return m_Impl.empty();
    }

    //! @brief  格納している要素配列の先頭へのポインタを返します。
    //! @return 格納している要素配列の先頭へのポインタを返します。
    pointer* data() NN_NOEXCEPT
    {
        return reinterpret_cast< pointer* >( m_Impl.data() );
    }

    //! @brief  格納している要素配列の先頭へのポインタを返します。
    //! @return 格納している要素配列の先頭へのポインタを返します。
    const const_pointer* data() const NN_NOEXCEPT
    {
        return reinterpret_cast< const const_pointer* >( m_Impl.data() );
    }

    //! @}

    //! @name 要素アクセス
    //! @{

    //! @brief      指定したインデックスの要素を返します。
    //! @param[in]  index   取得する要素の位置です。
    //! @return     取得した要素を返します。
    //! @pre        0 <= index < @ref size()
    reference operator[]( size_type index ) NN_NOEXCEPT
    {
        return ToRef( m_Impl[ index ] );
    }

    //! @brief      指定したインデックスの要素を返します。
    //! @param[in]  index   取得する要素の位置です。
    //! @return     取得した要素を返します。
    //! @pre        0 <= index < @ref size()
    const_reference operator[]( size_type index ) const NN_NOEXCEPT
    {
        return ToRef( m_Impl[ index ] );
    }

    //! @brief      指定したインデックスの要素を返します。
    //! @param[in]  index   取得する要素の位置です。
    //! @return     取得した要素を返します。
    //! @pre        0 <= index < @ref size()
    //! @details    範囲外のインデックスを指定した場合はアサートにかかります。@n
    //!             アサートが無効の際に範囲外のインデックスを指定した場合は先頭の要素を返します。
    reference at( size_type index ) NN_NOEXCEPT
    {
        return ToRef( m_Impl.at( index ) );
    }

    //! @brief      指定したインデックスの要素を返します。
    //! @param[in]  index   取得する要素の位置です。
    //! @return     取得した要素を返します。
    //! @pre        0 <= index < @ref size()
    //! @details    範囲外のインデックスを指定した場合はアサートにかかります。@n
    //!             アサートが無効の際に範囲外のインデックスを指定した場合は先頭の要素を返します。
    const_reference at( size_type index ) const NN_NOEXCEPT
    {
        return ToRef( m_Impl.at( index ) );
    }

    //! @brief      指定したインデックスの要素としてポインタを設定します。
    //! @param[in]  index   設定する要素のインデックスです。
    //! @param[in]  ptr     要素として設定するポインタです。
    //! @pre        0 <= index < @ref size()
    void SetPtr( size_type index, pointer ptr ) NN_NOEXCEPT
    {
        m_Impl[ index ] = reinterpret_cast< uintptr_t >( ptr );
    }

    //! @brief      指定したインデックスの要素のポインタを返します。
    //! @param[in]  index   取得する要素のインデックスです。
    //! @return     取得した要素のポインタを返します。
    //! @pre        0 <= index < @ref size()
    pointer GetPtr( size_type index ) NN_NOEXCEPT
    {
        return ToPtr( m_Impl[ index ] );
    }

    //! @brief      指定したインデックスの要素のポインタを返します。
    //! @param[in]  index   取得する要素のインデックスです。
    //! @return     取得した要素のポインタを返します。
    //! @pre        0 <= index < @ref size()
    const_pointer GetPtr( size_type index ) const NN_NOEXCEPT
    {
        return ToPtr( m_Impl[ index ] );
    }

    //! @brief  最初の要素を返します。
    //! @return 最初の要素を返します。
    reference front() NN_NOEXCEPT
    {
        return ToRef( m_Impl.front() );
    }

    //! @brief  最初の要素を返します。
    //! @return 最初の要素を返します。
    const_reference front() const NN_NOEXCEPT
    {
        return ToRef( m_Impl.front() );
    }

    //! @brief  最後の要素を返します。
    //! @return 最後の要素を返します。
    reference back() NN_NOEXCEPT
    {
        return ToRef( m_Impl.back() );
    }

    //! @brief  最後の要素を返します。
    //! @return 最後の要素を返します。
    const_reference back() const NN_NOEXCEPT
    {
        return ToRef( m_Impl.back() );
    }

    //! @}

    //! @name 変更
    //! @{

    //! @brief      述語に従って要素をソートします。
    //! @tparam     PredT       述語の型です。
    //! @param[in]  predicator  ソートを判定する述語です。
    template< typename PredicatorT >
    void Sort( PredicatorT predicator ) NN_NOEXCEPT
    {
        std::sort( m_Impl.begin(), m_Impl.end(), PtrPredicatorAdaptor< PredicatorT >( predicator ) );
    }

    //! @}

private:
    template< typename PredicatorT >
    class PtrPredicatorAdaptor
    {
    public:
        explicit PtrPredicatorAdaptor( PredicatorT& pred )
            : m_Predicator( pred )
        {
        }

        bool operator()( uintptr_t& x )
        {
            return m_Predicator( ToRef( x ) );
        }

        bool operator()( uintptr_t lhs, uintptr_t rhs )
        {
            return m_Predicator( ToRef( lhs ), ToRef( rhs ) );
        }

    private:
        PredicatorT m_Predicator;
    };

    static reference ToRef( uintptr_t& x )
    {
        return *reinterpret_cast< pointer >( x );
    }

    static const_reference ToRef( const uintptr_t& x )
    {
        return *reinterpret_cast< const_pointer >( x );
    }

    static pointer& ToPtr( uintptr_t& x )
    {
        return reinterpret_cast< pointer& >( x );
    }

    static const_pointer ToPtr( const uintptr_t& x )
    {
        return reinterpret_cast< const_pointer >( x );
    }

private:
    PlacementArray< uintptr_t > m_Impl;
};

}} // namespace nn::util
