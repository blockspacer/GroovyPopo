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
#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/nn_SdkAssert.h>
#include <nn/nn_StaticAssert.h>
#include <nn/util/util_BitUtil.h>

namespace nn { namespace util {

//! @brief 固定長ビット配列
class BitArray
{
    NN_DISALLOW_COPY( BitArray );

public:
    class reference;
    class iterator;
    class const_iterator;

    //! @brief 要素の型です。
    typedef bool value_type;
    //! @brief 要素のポインタ型です。
    typedef value_type* pointer;
    //! @brief 不変な要素のポインタ型です。
    typedef const value_type* const_pointer;
    //! @brief 不変な要素の参照型です。
    typedef const bool const_reference;
    //! @brief 逆イテレータです。
    typedef std::reverse_iterator< iterator > reverse_iterator;
    //! @brief 不変な逆イテレータです。
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;
    //! @brief サイズの型です。
    typedef int size_type;
    //! @brief 差分の型です。
    typedef int difference_type;

    //! @brief ビット配列の個々の要素の参照です。
    class reference
    {
        friend class BitArray;
        friend class BitArray::iterator;

    public:
        //! @name 変更
        //! @{

        //! @brief  要素を反転します。
        //! @return *this を返します。
        reference& flip() NN_NOEXCEPT
        {
            m_pParent->flip( m_Pos );
            return *this;
        }

        //! @}

        //! @name オペレータ
        //! @{

        //! @brief  要素が 1 である場合に true を返します。
        //! @return 結果を返します。
        NN_IMPLICIT operator bool() const NN_NOEXCEPT
        {
            return m_pParent->test( m_Pos );
        }

        //! @brief  要素に値を設定します。
        //! @return *this を返します。
        reference& operator= ( bool value ) NN_NOEXCEPT
        {
            m_pParent->set( m_Pos, value );
            return *this;
        }

        //! @brief  要素に値を設定します。
        //! @return *this を返します。
        reference& operator= ( const reference& value ) NN_NOEXCEPT
        {
            m_pParent->set( m_Pos, value );
            return *this;
        }

        //! @brief  要素を反転した値を返します。
        //! @return 結果を返します。
        bool operator~() const NN_NOEXCEPT
        {
            return !( m_pParent->test( m_Pos ) );
        }

        //! @}

    private:
        reference( BitArray* parent, size_type pos ) NN_NOEXCEPT
            : m_pParent( parent )
            , m_Pos( pos )
        {
        }

    private:
        BitArray* m_pParent;
        size_type m_Pos;
    };

    class iterator
    {
        friend class BitArray;
    public:
        typedef BitArray::value_type value_type;                    //!< イテレータが示す要素の型です。
        typedef BitArray::difference_type difference_type;          //!< イテレータの差分の型です。
        typedef BitArray::pointer pointer;                          //!< 要素のポインタ型です。
        typedef BitArray::reference reference;                      //!< イテレータが示す参照の型です。
        typedef std::random_access_iterator_tag iterator_category;  //!< イテレータのカテゴリです。

        //! @name 型変換
        //! @{

        //! @brief  const_iterator キャスト演算子のオーバーロードです。
        NN_IMPLICIT operator const_iterator() const NN_NOEXCEPT
        {
            return const_iterator( m_pParent, m_Pos );
        }

        //! @}

        //! @brief イテレータを進めます。
        iterator& operator++() NN_NOEXCEPT
        {
            ++m_Pos;
            return *this;
        }

        //! @brief イテレータを進めます。
        iterator operator++( int ) NN_NOEXCEPT
        {
            iterator iter( *this );
            ++m_Pos;
            return iter;
        }

        //! @brief イテレータを戻します。
        iterator& operator--() NN_NOEXCEPT
        {
            --m_Pos;
            return *this;
        }

        //! @brief イテレータを戻します。
        iterator operator--( int ) NN_NOEXCEPT
        {
            iterator iter( *this );
            --m_Pos;
            return iter;
        }

        //! @brief イテレータを進めます。
        iterator& operator+=( difference_type rhs ) NN_NOEXCEPT
        {
            m_Pos += rhs;
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
            m_Pos -= rhs;
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
            return static_cast< difference_type >( m_Pos ) - static_cast< difference_type >( rhs.m_Pos );
        }

        //! @brief イテレータが等しいかを取得します。
        bool operator==( const iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pParent == rhs.m_pParent && m_Pos == rhs.m_Pos;
        }

        //! @brief イテレータが等しくないかを取得します。
        bool operator!=( const iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pParent != rhs.m_pParent || m_Pos != rhs.m_Pos;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator<( const iterator& rhs ) const NN_NOEXCEPT
        {
            return m_Pos < rhs.m_Pos;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator>( const iterator& rhs ) const NN_NOEXCEPT
        {
            return m_Pos > rhs.m_Pos;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator<=( const iterator& rhs ) const NN_NOEXCEPT
        {
            return m_Pos <= rhs.m_Pos;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator>=( const iterator& rhs ) const NN_NOEXCEPT
        {
            return m_Pos >= rhs.m_Pos;
        }

        //! @brief 要素を取得します。
        reference operator*() const NN_NOEXCEPT
        {
            return BitArray::reference( m_pParent, m_Pos );
        }

        //! @brief 要素を取得します。
        reference operator[]( difference_type offset ) const NN_NOEXCEPT
        {
            return BitArray::reference( m_pParent, m_Pos + offset );
        }

    private:
        iterator( BitArray* parent, size_type pos ) NN_NOEXCEPT
            : m_pParent( parent )
            , m_Pos( pos )
        {
        }

    private:
        BitArray* m_pParent;
        size_type m_Pos;
    };

    class const_iterator
    {
        friend class BitArray;
        friend class BitArray::iterator;
    public:
        typedef BitArray::value_type value_type;                    //!< イテレータが示す要素の型です。
        typedef BitArray::difference_type difference_type;          //!< イテレータの差分の型です。
        typedef BitArray::const_pointer pointer;                    //!< 要素のポインタ型です。
        typedef BitArray::const_reference reference;                //!< イテレータが示す参照の型です。
        typedef std::random_access_iterator_tag iterator_category;  //!< イテレータのカテゴリです。

        //! @brief イテレータを進めます。
        const_iterator& operator++() NN_NOEXCEPT
        {
            ++m_Pos;
            return *this;
        }

        //! @brief イテレータを進めます。
        const_iterator operator++( int ) NN_NOEXCEPT
        {
            const_iterator iter( *this );
            ++m_Pos;
            return iter;
        }

        //! @brief イテレータを戻します。
        const_iterator& operator--() NN_NOEXCEPT
        {
            --m_Pos;
            return *this;
        }

        //! @brief イテレータを戻します。
        const_iterator operator--( int ) NN_NOEXCEPT
        {
            const_iterator iter( *this );
            --m_Pos;
            return iter;
        }

        //! @brief イテレータを進めます。
        const_iterator& operator+=( difference_type rhs ) NN_NOEXCEPT
        {
            m_Pos += rhs;
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
            m_Pos -= rhs;
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
            return static_cast< difference_type >( m_Pos ) - static_cast< difference_type >( rhs.m_Pos );
        }

        //! @brief イテレータが等しいかを取得します。
        bool operator==( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pParent == rhs.m_pParent && m_Pos == rhs.m_Pos;
        }

        //! @brief イテレータが等しくないかを取得します。
        bool operator!=( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pParent != rhs.m_pParent || m_Pos != rhs.m_Pos;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator<( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return m_Pos < rhs.m_Pos;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator>( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return m_Pos > rhs.m_Pos;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator<=( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return m_Pos <= rhs.m_Pos;
        }

        //! @brief イテレータの前後関係を取得します。
        bool operator>=( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return m_Pos >= rhs.m_Pos;
        }

        //! @brief 要素を取得します。
        reference operator*() const NN_NOEXCEPT
        {
            return m_pParent->test( m_Pos );
        }

        //! @brief 要素を取得します。
        reference operator[]( difference_type offset ) const NN_NOEXCEPT
        {
            return m_pParent->test( m_Pos + offset );
        }

    private:
        const_iterator( const BitArray* parent, size_type pos ) NN_NOEXCEPT
            : m_pParent( parent )
            , m_Pos( pos )
        {
        }

    private:
        const BitArray* m_pParent;
        size_type m_Pos;
    };

    //! @name 構築/破棄
    //! @{

    //! @brief      要素数が 0 のビット配列を構築します。
    //! @details    @ref ResetWorkMemory( void*, size_t, size_type ) でメモリ領域を割り当てるまで使用することはできません。
    BitArray() NN_NOEXCEPT
        : m_pBuf( NULL )
        , m_Len( 0 )
    {
    }

    //! @brief      メモリ領域を割り当ててビット配列を構築します。全要素を 0 に初期化します。
    //! @param[in]  workMemory      割り当てるメモリ領域の先頭のポインタです。
    //! @param[in]  workMemorySize  割り当てるメモリ領域のサイズです。
    //! @param[in]  length          ビット配列の要素数です。
    //! @details    割り当てたメモリ領域の内容は不定になります。永続化して利用することはできません。
    BitArray( void* workMemory, size_t workMemorySize, size_type length ) NN_NOEXCEPT
        : m_pBuf( NULL )
        , m_Len( 0 )
    {
        ResetWorkMemory( workMemory, workMemorySize, length );
    }

    //! @brief  割り当て中のメモリ領域を取り外し、0 要素に再初期化します。
    //! @return 直前まで割り当てられていたメモリ領域の先頭のポインタを返します。
    void* ResetWorkMemory() NN_NOEXCEPT
    {
        void* ptr = m_pBuf;
        m_pBuf = NULL;
        m_Len = 0;

        return ptr;
    }

    //! @brief      新たにメモリ領域を割り当て、全要素を 0 に初期化します。
    //! @param[in]  workMemory      割り当てるメモリ領域の先頭のポインタです。
    //! @param[in]  workMemorySize  割り当てるメモリ領域のサイズです。
    //! @param[in]  length          配列の要素数です。
    //! @return     直前まで割り当てられていたメモリ領域の先頭のポインタを返します。
    //! @pre        CalculateWorkMemorySize( length ) <= workMemorySize
    //! @details    割り当てたメモリ領域の内容は不定になります。永続化して利用することはできません。
    void* ResetWorkMemory( void* workMemory, size_t workMemorySize, size_type length ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( CalculateWorkMemorySize( length ) <= workMemorySize );
        NN_UNUSED( workMemorySize );

        void* ptr = m_pBuf;
        m_pBuf = static_cast< RawType* >( workMemory );
        m_Len = length;

        reset();

        return ptr;
    }

    //! @}

    //! @name 取得
    //! @{

    //! @brief  格納している要素数を返します。
    //! @return 結果を返します。
    size_type size() const NN_NOEXCEPT
    {
        return m_Len;
    }

    //! @brief  格納している要素のうち、1 であるビット数を返します。
    //! @return 結果を返します。
    size_type count() const NN_NOEXCEPT;

    //! @brief  全ての要素が 1 である場合に true を返します。
    //! @return 結果を返します。
    bool all() const NN_NOEXCEPT;

    //! @brief  全ての要素が 0 である場合に true を返します。
    //! @return 結果を返します。
    bool none( ) const NN_NOEXCEPT;

    //! @brief  1 である要素が 1 つ以上存在する場合に true を返します。
    //! @return 結果を返します。
    bool any() const NN_NOEXCEPT
    {
        return !none();
    }

    //! @brief      指定位置の要素が 1 である場合に true を返します。
    //! @param[in]  pos 指定する位置です。
    //! @return     結果を返します。
    //! @pre        pos < @ref size()
    bool test( size_type pos ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( pos < m_Len );
        return !!( Block( pos ) & MakeBlockMask( pos ) );
    }

    //! @}

    //! @name 変更
    //! @{

    //! @brief      全ての要素を反転します。
    //! @return     *this を返します。
    BitArray& flip() NN_NOEXCEPT;

    //! @brief      指定位置の要素を反転します。
    //! @param[in]  pos 指定する位置です。
    //! @return     *this を返します。
    //! @pre        pos < @ref size()
    BitArray& flip( size_type pos ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( pos < m_Len );
        Block( pos ) ^= MakeBlockMask( pos );
        return *this;
    }

    //! @brief      全ての要素を 1 に設定します。
    //! @return     *this を返します。
    BitArray& set() NN_NOEXCEPT;

    //! @brief      指定位置の要素を 1 に設定します。
    //! @param[in]  pos 指定する位置です。
    //! @return     *this を返します。
    //! @pre        pos < @ref size()
    BitArray& set( size_type pos ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( pos < m_Len );

        Block( pos ) |= MakeBlockMask( pos );

        return *this;
    }

    //! @brief      指定位置の要素を入力値に設定します。
    //! @param[in]  pos     指定する位置です。
    //! @param[in]  value   設定する入力値です。
    //! @return     *this を返します。
    //! @pre        pos < @ref size()
    BitArray& set( size_type pos, bool value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( pos < m_Len );

        if ( value )
        {
            set( pos );
        }
        else
        {
            reset( pos );
        }

        return *this;
    }

    //! @brief      全ての要素を 0 に設定します。
    //! @return     *this を返します。
    BitArray& reset() NN_NOEXCEPT;

    //! @brief      指定位置の要素を 0 に設定します。
    //! @param[in]  pos 指定する位置です。
    //! @return     *this を返します。
    //! @pre        pos < @ref size()
    BitArray& reset( size_type pos ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( pos < m_Len );
        Block( pos ) &= ~MakeBlockMask( pos );
        return *this;
    }

    //! @}

    //! @name イテレータ
    //! @{

    //! @brief  イテレータの先頭を返します。
    //! @return イテレータの先頭を返します。
    iterator begin() NN_NOEXCEPT
    {
        return iterator( this, 0 );
    }

    //! @brief  イテレータの先頭を返します。
    //! @return イテレータの先頭を返します。
    const_iterator begin() const NN_NOEXCEPT
    {
        return const_iterator( this, 0 );
    }

    //! @brief  イテレータの終端を返します。
    //! @return イテレータの終端を返します。
    iterator end() NN_NOEXCEPT
    {
        return iterator( this, m_Len );
    }

    //! @brief  イテレータの終端を返します。
    //! @return イテレータの終端を返します。
    const_iterator end() const NN_NOEXCEPT
    {
        return const_iterator( this, m_Len );
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

    //! @name オペレータ
    //! @{

    //! @brief      指定したビット配列と一致する場合に true を返します。
    //! @param[in]  rhs 指定するビット配列です。
    //! @return     結果を返します。
    //! @pre        指定するビット配列はこの配列と要素数が等しい必要があります。
    bool operator== ( const BitArray& rhs ) const NN_NOEXCEPT;

    //! @brief      指定したビット配列と一致しない場合に true を返します。
    //! @param[in]  rhs 指定するビット配列です。
    //! @return     結果を返します。
    //! @pre        指定するビット配列はこの配列と要素数が等しい必要があります。
    bool operator!= ( const BitArray& rhs ) const NN_NOEXCEPT
    {
        return !( this->operator==( rhs ) );
    }

    //! @brief      指定位置の要素が 1 である場合に true を返します。
    //! @param[in]  pos 指定する位置です。
    //! @return     結果を返します。
    //! @pre        pos < @ref size()
    bool operator[] ( size_type pos ) const NN_NOEXCEPT
    {
        return test( pos );
    }

    //! @brief      指定位置の参照を返します。
    //! @param[in]  pos 指定する位置です。
    //! @return     結果を返します。
    //! @pre        pos < @ref size()
    reference operator[] ( size_type pos ) NN_NOEXCEPT
    {
        return reference( this, pos );
    }

    //! @}

    //! @name 計算
    //! @{

    //! @brief      ビット配列の論理積を計算します。
    //! @param[in]  pResult 結果を設定するビット配列です。
    //! @param[in]  lhs     入力するビット配列です。
    //! @param[in]  rhs     入力するビット配列です。
    //! @pre        pResult にバッファが割り当て済みである必要があります。
    //! @pre        全てのビット配列の要素数が等しい必要があります。
    static void And( BitArray* pResult, const BitArray& lhs, const BitArray& rhs ) NN_NOEXCEPT;

    //! @brief      ビット配列の論理和を計算します。
    //! @param[in]  pResult 結果を設定するビット配列です。
    //! @param[in]  lhs     入力するビット配列です。
    //! @param[in]  rhs     入力するビット配列です。
    //! @pre        pResult にバッファが割り当て済みである必要があります。
    //! @pre        全てのビット配列の要素数が等しい必要があります。
    static void Or( BitArray* pResult, const BitArray& lhs, const BitArray& rhs ) NN_NOEXCEPT;

    //! @brief      ビット配列の排他的論理和を計算します。
    //! @param[in]  pResult 結果を設定するビット配列です。
    //! @param[in]  lhs     入力するビット配列です。
    //! @param[in]  rhs     入力するビット配列です。
    //! @pre        pResult にバッファが割り当て済みである必要があります。
    //! @pre        全てのビット配列の要素数が等しい必要があります。
    static void Xor( BitArray* pResult, const BitArray& lhs, const BitArray& rhs ) NN_NOEXCEPT;

    //! @brief      反転したビット配列を計算します。
    //! @param[in]  pResult 結果を設定するビット配列です。
    //! @param[in]  lhs     入力するビット配列です。
    //! @pre        pResult にバッファが割り当て済みである必要があります。
    //! @pre        全てのビット配列の要素数が等しい必要があります。
    static void Not( BitArray* pResult, const BitArray& lhs ) NN_NOEXCEPT;

    //! @brief      ビット配列の右シフトを計算します。
    //! @param[in]  pResult 結果を設定するビット配列です。
    //! @param[in]  lhs     入力するビット配列です。
    //! @param[in]  pos     右シフトする桁数です。
    //! @pre        pResult にバッファが割り当て済みである必要があります。
    //! @pre        全てのビット配列の要素数が等しい必要があります。
    static void RightShift( BitArray* pResult, const BitArray& lhs, size_type pos ) NN_NOEXCEPT;

    //! @brief      ビット配列の左シフトを計算します。
    //! @param[in]  pResult 結果を設定するビット配列です。
    //! @param[in]  lhs     入力するビット配列です。
    //! @param[in]  pos     左シフトする桁数です。
    //! @pre        pResult にバッファが割り当て済みである必要があります。
    //! @pre        全てのビット配列の要素数が等しい必要があります。
    static void LeftShift( BitArray* pResult, const BitArray& lhs, size_type pos ) NN_NOEXCEPT;

    //! @brief      要素数を元に必要なバイトサイズを計算します。
    //! @param[in]  length  ビット配列の要素数です。
    //! @return     結果を返します。
    static size_t CalculateWorkMemorySize( size_type length ) NN_NOEXCEPT
    {
        return static_cast< size_t >( align_up( length, BitsPerBlock ) / CHAR_BIT );
    }

    //! @}

private:

#if defined(NN_BUILD_CONFIG_ADDRESS_64)
    typedef uint64_t RawType;
#else
    typedef uint32_t RawType;
#endif

    static const int BitsPerBlock = NN_BITSIZEOF( RawType );
    static const int BlockShift = nn::util::detail::Log2< BitsPerBlock >::Value;
    static const RawType BlockBitMask = RawType( BitsPerBlock - 1 );

    RawType& Block( size_type pos ) NN_NOEXCEPT
    {
        return m_pBuf[ pos >> BlockShift ];
    }

    const RawType& Block( size_type pos ) const NN_NOEXCEPT
    {
        return m_pBuf[ pos >> BlockShift ];
    }

    //! @brief pos ビットだけが立っているマスク
    static RawType MakeBlockMask( size_type pos ) NN_NOEXCEPT
    {
        return RawType( 1 ) << ( pos & BlockBitMask );
    }

    size_type GetBlockCount() const NN_NOEXCEPT
    {
        return align_up( m_Len, BitsPerBlock ) >> BlockShift;
    }

    RawType MakeLastBlockBitMask() const NN_NOEXCEPT
    {
        if ( unsigned int bits = ( m_Len & BlockBitMask ) )
        {
            return ( RawType( 1 ) << bits ) - 1;
        }
        else
        {
            return RawType( -1 );
        }
    }

private:
    RawType* m_pBuf;
    size_type m_Len;
};

}} // namespace nn::util
