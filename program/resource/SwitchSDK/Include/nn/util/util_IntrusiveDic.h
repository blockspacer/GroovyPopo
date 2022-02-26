/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
* @file
* @brief    侵入型辞書です。
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/util/util_BytePtr.h>
#include <nn/util/util_StringView.h>

namespace nn { namespace util {

namespace detail
{
    class IntrusiveDicImpl;
}

//! @brief  侵入型辞書のノードです。
class IntrusiveDicNode
{
    friend class nn::util::detail::IntrusiveDicImpl;
    NN_DISALLOW_COPY( IntrusiveDicNode );

public:

    //! @brief コンストラクタです。
    IntrusiveDicNode() NN_NOEXCEPT
        : m_Key()
    {
        Clear();
    }

    //! @brief  キーを取得します。
    //! @return 取得したキーを返します。
    const nn::util::string_view& GetKey() const NN_NOEXCEPT
    {
        return m_Key;
    }

    //! @brief      指定したインデックスの子ノードを取得します。
    //! @param[in]  childIndex  指定するインデックスです。
    //! @return     指定したインデックスの子ノードを返します。
    const IntrusiveDicNode* GetChild( int childIndex ) const NN_NOEXCEPT
    {
        return m_pChildren[ childIndex ];
    }

    //! @brief  参照ビットを取得します。
    //! @return 参照ビットを返します。
    int GetRefBit() const NN_NOEXCEPT
    {
        return m_RefBit;
    }
private:
    //! @brief  無効なインデックスです。
    static const int Npos = -1;

    //! @brief      キーを設定します。
    //! @param[in]  key 設定するキーです。
    void SetKey( const nn::util::string_view& key ) NN_NOEXCEPT
    {
        m_Key = key;
    }

    //! @brief  親ノードを取得します。
    //! @return 取得した親ノードを返します。
    IntrusiveDicNode* GetParent() NN_NOEXCEPT
    {
        return m_pParent;
    }

    //! @brief  親ノードを取得します。
    //! @return 取得した親ノードを返します。
    const IntrusiveDicNode* GetParent() const NN_NOEXCEPT
    {
        return m_pParent;
    }

    //! @brief  指定したインデックスの子ノードを取得します。
    //! @return 指定したインデックスの子ノードを返します。
    IntrusiveDicNode* GetChild( int childIndex ) NN_NOEXCEPT
    {
        return m_pChildren[ childIndex ];
    }

    //! @brief      キーに対応する子ノードを取得します。
    //! @param[in]  key 子ノードのキーです。
    //! @return     キーに対応する子ノードを返します。
    IntrusiveDicNode* GetChild( const nn::util::string_view& key ) NN_NOEXCEPT
    {
        // ビットが 1 なら右、0 なら左をたどります。
        int childIndex = ExtractRefBit( key, m_RefBit );
        return m_pChildren[ childIndex ];
    }

    //! @brief      キーに対応する子ノードを取得します。
    //! @param[in]  key 子ノードのキーです。
    //! @return     キーに対応する子ノードを返します。
    const IntrusiveDicNode* GetChild( const nn::util::string_view& key ) const NN_NOEXCEPT
    {
        // ビットが 1 なら右、0 なら左をたどります。
        int childIndex = ExtractRefBit( key, m_RefBit );
        return m_pChildren[ childIndex ];
    }

    //! @brief      キーに対応する子ノードのインデックスを取得します。
    //! @param[in]  key 子ノードのキーです。
    //! @return     キーに対応する子ノードのインデックスを返します。
    int GetChildIndex( const nn::util::string_view& key ) const NN_NOEXCEPT
    {
        return ExtractRefBit( key, m_RefBit );
    }

    //! @brief ノードを子ノードとして設定します。
    void SetChild( IntrusiveDicNode* pNode ) NN_NOEXCEPT
    {
        // ビットが 1 なら右、0 なら左の子供にします。
        int childIndex = ExtractRefBit( pNode->GetKey(), m_RefBit );
        m_pChildren[ childIndex ] = pNode;
        if ( IsOlderThan( pNode ) )
        {
            pNode->m_pParent = this;
        }
    }

    //! @brief ノードを削除します。
    IntrusiveDicNode* Erase( IntrusiveDicNode** ppOther, int* pRefBit ) NN_NOEXCEPT;

    //! @brief ノードを挿入します。
    IntrusiveDicNode* InsertChild( IntrusiveDicNode** ppNode, int* pRefBit, const nn::util::string_view& key ) NN_NOEXCEPT;

    //! @brief 参照ビットを探索します。
    bool FindRefBit( const IntrusiveDicNode* pNode ) NN_NOEXCEPT;

    //! @brief 参照ビットを比較してより自分がルートノードに近いかどうかを取得します。
    bool IsOlderThan( const IntrusiveDicNode* pNode ) const NN_NOEXCEPT
    {
        return m_RefBit < pNode->m_RefBit;
    }

    //! @brief  ノードの状態をクリアします。
    void Clear() NN_NOEXCEPT
    {
        m_Key.clear();
        m_RefBit = Npos;
        m_pParent = m_pChildren[ 0 ] = m_pChildren[ 1 ] = this;
    }

    //! @brief  文字列の末尾から数えて refBit 番目のビットを返します。
    static int ExtractRefBit( const nn::util::string_view& key, int refBit ) NN_NOEXCEPT;

    //! @brief  ノードが辞書に登録済みかどうかを取得します。
    //! @return 結果を返します。
    bool IsValid() const NN_NOEXCEPT
    {
        return !m_Key.empty() && m_RefBit != Npos;
    }

private:
    nn::util::string_view m_Key;
    int m_RefBit;
    IntrusiveDicNode* m_pParent;
    IntrusiveDicNode* m_pChildren[ 2 ];
};

namespace detail {

// 侵入型辞書内部実装
class IntrusiveDicImpl
{
    NN_DISALLOW_COPY( IntrusiveDicImpl );

public:
    class iterator;
    class const_iterator;

    typedef IntrusiveDicNode value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::reverse_iterator< iterator > reverse_iterator;
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;

    // 不変なイテレータです。
    class const_iterator
    {
    public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef nn::util::detail::AddConst< IntrusiveDicImpl::value_type >::Type value_type;
        typedef IntrusiveDicImpl::difference_type difference_type;
        typedef IntrusiveDicImpl::const_pointer pointer;
        typedef IntrusiveDicImpl::const_reference reference;

        // コンストラクタです。
        explicit const_iterator( pointer pNode ) NN_NOEXCEPT
            : m_pNode( pNode )
        {
        }

        // 要素を取得します。
        reference operator*() const NN_NOEXCEPT
        {
            return *m_pNode;
        }

        // 要素へアクセスします。
        pointer operator->() const NN_NOEXCEPT
        {
            return m_pNode;
        }

        // イテレータを進めます。
        const_iterator& operator++() NN_NOEXCEPT
        {
            m_pNode = MoveIter( m_pNode, 1 );
            return *this;
        }

        // イテレータを進めます。
        const_iterator operator++( int ) NN_NOEXCEPT
        {
            const_iterator iter( *this );
            ++( *this );
            return iter;
        }

        // イテレータを戻します。
        const_iterator& operator--() NN_NOEXCEPT
        {
            m_pNode = MoveIter( m_pNode, 0 );
            return *this;
        }

        // イテレータを戻します。
        const_iterator operator--( int ) NN_NOEXCEPT
        {
            const_iterator iter( *this );
            --( *this );
            return iter;
        }

        // イテレータが等しいかを取得します。
        bool operator==( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return Equals( rhs );
        }

        // イテレータが等しくないかを取得します。
        bool operator!=( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return !Equals( rhs );
        }

    private:
        bool Equals( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pNode == rhs.m_pNode;
        }

    private:
        pointer m_pNode;
    };

    // イテレータです。
    class iterator
    {
    public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef IntrusiveDicImpl::value_type value_type;
        typedef IntrusiveDicImpl::difference_type difference_type;
        typedef IntrusiveDicImpl::pointer pointer;
        typedef IntrusiveDicImpl::reference reference;

        // コンストラクタです。
        explicit iterator( pointer pNode ) NN_NOEXCEPT
            : m_pNode( pNode )
        {
        }

        // const_iterator キャスト演算子のオーバーロードです。
        NN_IMPLICIT operator const_iterator() const NN_NOEXCEPT
        {
            return const_iterator( m_pNode );
        }

        // 要素を取得します。
        reference operator*() const NN_NOEXCEPT
        {
            return *m_pNode;
        }

        // 要素へアクセスします。
        pointer operator->() const NN_NOEXCEPT
        {
            return m_pNode;
        }

        // イテレータを進めます。
        iterator& operator++() NN_NOEXCEPT
        {
            m_pNode = const_cast< IntrusiveDicNode* >( MoveIter( m_pNode, 1 ) );
            return *this;
        }

        // イテレータを進めます。
        iterator operator++( int ) NN_NOEXCEPT
        {
            iterator iter( *this );
            ++( *this );
            return iter;
        }

        // イテレータを戻します。
        iterator& operator--() NN_NOEXCEPT
        {
            m_pNode = const_cast< IntrusiveDicNode* >( MoveIter( m_pNode, 0 ) );
            return *this;
        }

        // イテレータを戻します。
        iterator operator--( int ) NN_NOEXCEPT
        {
            iterator iter( *this );
            --( *this );
            return iter;
        }

        // イテレータが等しいかを取得します。
        bool operator==( const iterator& rhs ) const NN_NOEXCEPT
        {
            return Equals( rhs );
        }

        // イテレータが等しくないかを取得します。
        bool operator!=( const iterator& rhs ) const NN_NOEXCEPT
        {
            return !Equals( rhs );
        }

    private:

        bool Equals( const iterator& rhs ) const NN_NOEXCEPT
        {
            return m_pNode == rhs.m_pNode;
        }

    private:
        pointer m_pNode;
    };

    // コンストラクタです。
    IntrusiveDicImpl() NN_NOEXCEPT
        : m_Root()
    {
    }

    // イテレータの先頭を返します。
    iterator begin() NN_NOEXCEPT
    {
        return iterator( const_cast< IntrusiveDicNode* >( FindIter() ) );
    }

    // イテレータの先頭を返します。
    const_iterator begin() const NN_NOEXCEPT
    {
        return const_iterator( FindIter() );
    }

    // イテレータの終端を返します。
    iterator end() NN_NOEXCEPT
    {
        return iterator( &m_Root );
    }

    // イテレータの終端を返します。
    const_iterator end() const NN_NOEXCEPT
    {
        return const_iterator( &m_Root );
    }

    // 逆イテレータの先頭を返します。
    reverse_iterator rbegin() NN_NOEXCEPT
    {
        return reverse_iterator( end() );
    }

    // 逆イテレータの先頭を返します。
    const_reverse_iterator rbegin() const NN_NOEXCEPT
    {
        return const_reverse_iterator( end() );
    }

    // 逆イテレータの終端を返します。
    reverse_iterator rend() NN_NOEXCEPT
    {
        return reverse_iterator( begin() );
    }

    // 逆イテレータの終端を返します。
    const_reverse_iterator rend() const NN_NOEXCEPT
    {
        return const_reverse_iterator( begin() );
    }

    // 不変なイテレータの先頭を返します。
    const_iterator cbegin() const NN_NOEXCEPT
    {
        return begin();
    }

    // 不変なイテレータの終端を返します。
    const_iterator cend() const NN_NOEXCEPT
    {
        return end();
    }

    // 不変な逆イテレータの先頭を返します。
    const_reverse_iterator crbegin() const NN_NOEXCEPT
    {
        return rbegin();
    }

    // 不変な逆イテレータの終端を返します。
    const_reverse_iterator crend() const NN_NOEXCEPT
    {
        return rend();
    }

    // 格納している要素数を返します。
    size_type size() const NN_NOEXCEPT
    {
        return static_cast< size_type >( std::distance( begin(), end() ) );
    }

    // 格納している要素が空かどうかを返します。
    bool empty() const NN_NOEXCEPT
    {
        return m_Root.GetChild( 0 ) == &m_Root;
    }

    // 要素に対応するイテレータを取得します。
    iterator iterator_to( reference value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value.IsValid() );
        return iterator( &value );
    }

    // 要素に対応するイテレータを取得します。
    const_iterator iterator_to( const_reference value ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( value.IsValid() );
        return const_iterator( &value );
    }

    // キーに対応する要素を探索します。
    iterator find( const nn::util::string_view& key ) NN_NOEXCEPT
    {
        IntrusiveDicNode* pNode = const_cast< IntrusiveDicNode* >( FindImpl( key ) );
        if ( pNode->GetKey() == key )
        {
            return iterator( pNode );
        }
        else
        {
            return this->end();
        }
    }

    // キーに対応する要素を探索します。
    const_iterator find( const nn::util::string_view& key ) const NN_NOEXCEPT
    {
        const IntrusiveDicNode* pNode = FindImpl( key );
        if ( pNode->GetKey() == key )
        {
            return const_iterator( pNode );
        }
        else
        {
            return this->end();
        }
    }

    // 要素を空にします。
    void clear() NN_NOEXCEPT
    {
        // end() は erase() 後も変わらないのでキャッシュします。
        iterator iter = this->begin();
        iterator end = this->end();
        while ( iter != end )
        {
            iter = erase( iter );
        }
    }

    // 要素を挿入します。
    std::pair< iterator, bool > insert( const nn::util::string_view& key, reference node ) NN_NOEXCEPT
    {
        const IntrusiveDicNode* pNode = FindImpl( key );

        // ノードが他の辞書に登録されていないことをチェックします。
        if ( !node.IsValid() )
        {
            // 処理のためノードにキーを設定する
            node.SetKey( key );
            if ( node.FindRefBit( pNode ) )
            {
                InsertImpl( &node );
                return std::make_pair( iterator( &node ), true );
            }

            // 挿入に失敗した場合、状態をクリア
            node.Clear();
        }
        return std::make_pair( iterator( const_cast< IntrusiveDicNode* >( pNode ) ), false );
    }

    // 要素を削除します。
    int erase( const nn::util::string_view& key ) NN_NOEXCEPT
    {
        IntrusiveDicNode* pNode = const_cast< IntrusiveDicNode* >( FindImpl( key ) );
        if ( !key.empty() && pNode->GetKey() == key )
        {
            EraseImpl( pNode );
            return 1;
        }
        else
        {
            return 0;
        }
    }

    // 要素を削除します。
    iterator erase( const_iterator iter ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( iter != cend() );

        IntrusiveDicNode* pNode = &const_cast< IntrusiveDicNode& >( *iter );
        ++iter;
        IntrusiveDicNode* pNext = &const_cast< IntrusiveDicNode& >( *iter );
        EraseImpl( pNode );
        return iterator( pNext );
    }

private:

    // キーに対応するノードの候補を探索します。
    // 見つかったノードに対してキーを比較する必要があります。
    const IntrusiveDicNode* FindImpl( const nn::util::string_view& key ) const NN_NOEXCEPT;

    // ノードを挿入します。
    // ノードのキーに対応する位置に挿入します。
    void InsertImpl( IntrusiveDicNode* pNode ) NN_NOEXCEPT;

    // ノードを削除します。
    void EraseImpl( IntrusiveDicNode* pNode ) NN_NOEXCEPT;

    // イテレータの先頭位置を探索します。
    const IntrusiveDicNode* FindIter() const NN_NOEXCEPT;

    // ノードが指す位置を1つ移動します。
    // 引数で指定したインデックスの子の方向へ移動します。
    static const IntrusiveDicNode* MoveIter( const IntrusiveDicNode* pNode, int fwdIndex ) NN_NOEXCEPT;

    // ノードを子に持つ親ノードを探索します。
    static const IntrusiveDicNode* FindParent( const IntrusiveDicNode* pNode ) NN_NOEXCEPT;

private:
    IntrusiveDicNode m_Root;
};

} // namespace detail

//! @brief  侵入型辞書です。
//! @tparam T           要素の型です。要素は @ref IntrusiveDicNode をメンバに持つか、
//!                     @ref IntrusiveDicBaseNode を継承した型である必要があります。
//! @tparam NodeTraits  要素の特性を示す型です。要素が @ref IntrusiveDicNode をメンバに持つ場合は
//!                     @ref IntrusiveDicMemberNodeTraits を、@ref IntrusiveDicBaseNode を継承した型である場合は
//!                     @ref IntrusiveDicBaseNodeTraits を指定する必要があります。
template< class T, class NodeTraits >
class IntrusiveDic
{
    NN_DISALLOW_COPY( IntrusiveDic );

public:
    class const_iterator;
    class iterator;

    //! @brief  要素の型です。
    typedef T value_type;

    //! @brief  要素のポインタ型です。
    typedef value_type* pointer;
    //! @brief  不変な要素のポインタ型です。
    typedef typename nn::util::detail::AddConst< value_type >::Type* const_pointer;

    //! @brief  要素の参照型です。
    typedef value_type& reference;
    //! @brief  不変な要素の参照型です。
    typedef typename nn::util::detail::AddConst< value_type >::Type& const_reference;

    //! @brief  逆イテレータです。
    typedef std::reverse_iterator< iterator > reverse_iterator;
    //! @brief  不変な逆イテレータです。
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;
    //! @brief  差分の型です。
    typedef ptrdiff_t difference_type;
    //! @brief  サイズの型です。
    typedef size_t size_type;
    //! @brief  要素の特性の型です。
    typedef NodeTraits traits_type;

    //! @brief 不変なイテレータです。
    class const_iterator
    {
        friend class IntrusiveDic<T, NodeTraits>;

    public:
        typedef std::bidirectional_iterator_tag iterator_category; //!< イテレータのカテゴリです。
        typedef typename nn::util::detail::AddConst< typename IntrusiveDic::value_type >::Type value_type; //!< イテレータが示す要素の型です。
        typedef typename IntrusiveDic::difference_type difference_type; //!< イテレータの差分の型です。
        typedef typename IntrusiveDic::const_pointer pointer; //!< 要素のポインタ型です。
        typedef typename IntrusiveDic::const_reference reference; //!< 要素の参照型です。

        //! @brief イテレータを進めます。
        const_iterator& operator++() NN_NOEXCEPT
        {
            ++m_Impl;
            return *this;
        }

        //! @brief イテレータを進めます。
        const_iterator operator++( int ) NN_NOEXCEPT
        {
            const_iterator iter( *this );
            ++m_Impl;
            return iter;
        }

        //! @brief イテレータを戻します。
        const_iterator& operator--() NN_NOEXCEPT
        {
            --m_Impl;
            return *this;
        }

        //! @brief イテレータを戻します。
        const_iterator operator--( int ) NN_NOEXCEPT
        {
            const_iterator iter( *this );
            --m_Impl;
            return iter;
        }

        //! @brief イテレータが等しいかを取得します。
        bool operator==( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return m_Impl == rhs.m_Impl;
        }

        //! @brief イテレータが等しくないかを取得します。
        bool operator!=( const const_iterator& rhs ) const NN_NOEXCEPT
        {
            return !( m_Impl == rhs.m_Impl );
        }

        //! @brief 要素を取得します。
        typename const_iterator::reference operator*() const NN_NOEXCEPT
        {
            return NodeTraits::GetItem( *m_Impl );
        }

        //! @brief 要素へアクセスします。
        typename const_iterator::pointer operator->() const NN_NOEXCEPT
        {
            return &NodeTraits::GetItem( *m_Impl );
        }
    private:

        explicit const_iterator( const nn::util::detail::IntrusiveDicImpl::const_iterator& impl ) NN_NOEXCEPT
            : m_Impl( impl )
        {
        }

    private:
        nn::util::detail::IntrusiveDicImpl::const_iterator m_Impl;
    };

    //! @brief イテレータです。
    class iterator
    {
        friend class IntrusiveDic<T, NodeTraits>;

    public:
        typedef std::bidirectional_iterator_tag iterator_category; //!< イテレータのカテゴリです。
        typedef typename IntrusiveDic::value_type value_type; //!< イテレータが示す要素の型です。
        typedef typename IntrusiveDic::difference_type difference_type; //!< イテレータの差分の型です。
        typedef typename IntrusiveDic::pointer pointer; //!< 要素のポインタ型です。
        typedef typename IntrusiveDic::reference reference; //!< 要素の参照型です。

        //! @name 型変換
        //! @{

        //  @brief  const_iterator に変換します。
        //  @return 変換された const_iterator を取得します。
        NN_IMPLICIT operator const_iterator() const NN_NOEXCEPT
        {
            return const_iterator( m_Impl );
        }

        //! @}

        //! @brief イテレータを進めます。
        iterator& operator++() NN_NOEXCEPT
        {
            ++m_Impl;
            return *this;
        }

        //! @brief イテレータを進めます。
        iterator operator++( int ) NN_NOEXCEPT
        {
            iterator iter( *this );
            ++m_Impl;
            return iter;
        }

        //! @brief イテレータを戻します。
        iterator& operator--() NN_NOEXCEPT
        {
            --m_Impl;
            return *this;
        }

        //! @brief イテレータを戻します。
        iterator operator--( int ) NN_NOEXCEPT
        {
            iterator iter( *this );
            --m_Impl;
            return iter;
        }

        //! @brief イテレータが等しいかを取得します。
        bool operator==( const iterator& rhs ) const NN_NOEXCEPT
        {
            return m_Impl == rhs.m_Impl;
        }

        //! @brief イテレータが等しくないかを取得します。
        bool operator!=( const iterator& rhs ) const NN_NOEXCEPT
        {
            return !( m_Impl == rhs.m_Impl );
        }

        //! @brief 要素を取得します。
        typename iterator::reference operator*() const NN_NOEXCEPT
        {
            return NodeTraits::GetItem( *m_Impl );
        }

        //! @brief 要素へアクセスします。
        typename iterator::pointer operator->() const NN_NOEXCEPT
        {
            return &NodeTraits::GetItem( *m_Impl );
        }
    private:

        explicit iterator( const nn::util::detail::IntrusiveDicImpl::iterator& impl ) NN_NOEXCEPT
            : m_Impl( impl )
        {
        }

    private:
        nn::util::detail::IntrusiveDicImpl::iterator m_Impl;
    };

    //! @name 構築/破棄
    //! @{

    //! @brief コンストラクタです。
    IntrusiveDic() NN_NOEXCEPT
        : m_Impl()
    {
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

    //! @brief      要素に対応するイテレータを取得します。
    //! @param[in]  value   イテレータを取得する要素です。
    //! @return     要素に対応するイテレータです。
    iterator iterator_to( reference value ) NN_NOEXCEPT
    {
        return iterator( m_Impl.iterator_to( NodeTraits::GetNode( value ) ) );
    }

    //! @brief      要素に対応するイテレータを取得します。
    //! @param[in]  value   イテレータを取得する要素です。
    //! @pre        要素が辞書に登録されている必要があります。
    //! @return     要素に対応するイテレータです。
    const_iterator iterator_to( const_reference value ) const NN_NOEXCEPT
    {
        return const_iterator( m_Impl.iterator_to( NodeTraits::GetNode( value ) ) );
    }

    //! @}

    //! @name 探索
    //! @{

    //! @brief      キーに対応する要素を探索します。
    //! @param[in]  key 探索するキーです。
    //! @return     キーに対応する要素を探索します。見つからなかった場合、@ref end() を返します。
    iterator find( const string_view& key ) NN_NOEXCEPT
    {
        return iterator( m_Impl.find( key ) );
    }

    //! @brief      キーに対応する要素を探索します。
    //! @param[in]  key 探索するキーです。
    //! @return     キーに対応する要素を探索します。見つからなかった場合、@ref end() を返します。
    const_iterator find( const string_view& key ) const NN_NOEXCEPT
    {
        return const_iterator( m_Impl.find( key ) );
    }

    //! @}

    //! @name 変更
    //! @{

    //! @brief 要素を空にします。
    void clear() NN_NOEXCEPT
    {
        m_Impl.clear();
    }

    //! @brief      要素を挿入します。
    //! @param[in]  key     挿入する要素に対応するキーです。
    //! @param[in]  element 挿入する要素です。
    //! @pre        同一の辞書ノードを使用する辞書に登録されていないことが条件です。
    //! @return     要素を挿入した位置のイテレータと挿入に成功したかどうかのペアを返します。
    std::pair< iterator, bool > insert( const nn::util::string_view& key, reference element ) NN_NOEXCEPT
    {
        std::pair< nn::util::detail::IntrusiveDicImpl::iterator, bool > result = m_Impl.insert( key, NodeTraits::GetNode( element ) );
        return std::make_pair( iterator( result.first ), result.second );
    }

    //! @brief      要素を削除します。
    //! @param[in]  key 削除する要素を探索するキーです。
    //! @return     削除された要素の数を返します。
    int erase( const string_view& key ) NN_NOEXCEPT
    {
        return m_Impl.erase( key );
    }

    //! @brief      要素を削除します。
    //! @param[in]  pos 削除する要素を示すイテレータです。
    //! @return     削除した要素の次の要素のイテレータです。
    iterator erase( const_iterator pos ) NN_NOEXCEPT
    {
        return iterator( m_Impl.erase( pos.m_Impl ) );
    }

    //! @}
private:
    nn::util::detail::IntrusiveDicImpl m_Impl;
};

//! @brief  侵入型辞書のノードへのアクセス方法を定義するクラスです。
//!         ノードをメンバとして定義した場合に使用します。
template< class HolderT, IntrusiveDicNode HolderT::*Member, class T = HolderT, size_t IsCompleteType = sizeof(HolderT)>
class IntrusiveDicMemberNodeTraits
{
public:
    //! @brief      要素を @ref IntrusiveDicNode に変換します。
    //! @param[in]  item    変換する要素です。
    //! @return     要素に対応するノードです。
    static IntrusiveDicNode& GetNode( T& item ) NN_NOEXCEPT
    {
        return item.*Member;
    }

    //! @brief      要素を @ref IntrusiveDicNode に変換します。
    //! @param[in]  item    変換する要素です。
    //! @return     要素に対応するノードです。
    static const IntrusiveDicNode& GetNode( const T& item ) NN_NOEXCEPT
    {
        return item.*Member;
    }

    //! @brief      @ref IntrusiveDicNode を要素に変換します。
    //! @param[in]  node    変換するノードです。
    //! @return     ノードに対応する要素です。
    static T& GetItem( IntrusiveDicNode& node ) NN_NOEXCEPT
    {
        return *BytePtr( &node, -GetOffset() ).Get< T >();
    }

    //! @brief      @ref IntrusiveDicNode を要素に変換します。
    //! @param[in]  node    変換するノードです。
    //! @return     ノードに対応する要素です。
    static const T& GetItem( const IntrusiveDicNode& node ) NN_NOEXCEPT
    {
        return *ConstBytePtr( &node, -GetOffset() ).Get< T >();
    }

private:
    static ptrdiff_t GetOffset() NN_NOEXCEPT
    {
        return reinterpret_cast< ptrdiff_t >( &( ( reinterpret_cast< T* >( 0 ) )->*Member ) );
    }
};

//! @brief  侵入型辞書のノードです。ノードを基底クラスとして定義するスタイルです。
//! @tparam T     侵入型辞書の要素の型です。
//! @tparam Tag   ノードを多重継承する際にノードを区別するための型です。
template < class T, class Tag = T >
class IntrusiveDicBaseNode : public IntrusiveDicNode
{
};

//! @brief  侵入型辞書のノードへのアクセス方法を定義するクラスです。
//!         ノードを基底クラスとして定義した場合に使用します。
//! @tparam T       侵入型辞書の要素の型です。
//! @tparam Tag     アクセスしたい IntrusiveDicBaseNode に指定した Tag 型です。
template < class T, class Tag = T >
class IntrusiveDicBaseNodeTraits
{
private:
    typedef IntrusiveDicBaseNode< T, Tag > BaseNodeType;

public:
    //! @brief      要素を @ref IntrusiveDicNode に変換します。
    //! @param[in]  item    変換する要素です。
    //! @return     要素に対応するノードです。
    static IntrusiveDicNode& GetNode( T& item ) NN_NOEXCEPT
    {
        return static_cast< BaseNodeType& >( item );
    }

    //! @brief      要素を @ref IntrusiveDicNode に変換します。
    //! @param[in]  item    変換する要素です。
    //! @return     要素に対応するノードです。
    static const IntrusiveDicNode& GetNode( const T& item ) NN_NOEXCEPT
    {
        return static_cast< const BaseNodeType& >( item );
    }

    //! @brief      @ref IntrusiveDicNode を要素に変換します。
    //! @param[in]  node    変換するノードです。
    //! @return     ノードに対応する要素です。
    static T& GetItem( IntrusiveDicNode& node ) NN_NOEXCEPT
    {
        return *static_cast< T* >( static_cast< BaseNodeType* >( &node ) );
    }

    //! @brief      @ref IntrusiveDicNode を要素に変換します。
    //! @param[in]  node    変換するノードです。
    //! @return     ノードに対応する要素です。
    static const T& GetItem( const IntrusiveDicNode& node ) NN_NOEXCEPT
    {
        return *static_cast< const T* >( static_cast< const BaseNodeType* >( &node ) );
    }
};

}} // namespace nn/util
