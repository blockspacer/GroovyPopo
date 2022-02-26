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

#include <string> // std::char_traits のみ使用します。
#include <algorithm>
#include <nn/nn_Common.h>
#include <nn/nn_Abort.h>
#include <nn/nn_SdkAssert.h>

namespace nn { namespace util {

//! @brief      文字列参照です。
template< class charT, class traits = std::char_traits< charT > >
class basic_string_view
{
    //! @brief 自身の型です。
    typedef basic_string_view< charT, traits > self_type;

public:
    //! @brief 要素の型です。
    typedef charT value_type;

    //! @brief 要素のポインタ型です。
    typedef charT* pointer;

    //! @brief 不変な要素のポインタ型です。
    typedef const charT* const_pointer;

    //! @brief 要素の参照型です。
    typedef charT& reference;

    //! @brief 不変な要素の参照型です。
    typedef const charT& const_reference;

    //! @brief 不変なイテレータです。
    typedef const_pointer const_iterator;

    //! @brief イテレータです。
    typedef const_iterator iterator;

    //! @brief 不変な逆イテレータです。
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    //! @brief 逆イテレータです。
    typedef const_reverse_iterator reverse_iterator;

    //! @brief サイズの型です。
    typedef size_t size_type;

    //! @brief ポインタの差分です。
    typedef ptrdiff_t difference_type;

    //! @brief char_traits の型です。
    typedef traits traits_type;

    //! @brief 無効なインデックスです。
    static const size_type npos = size_type( -1 );

    //! @name コンストラクタ
    //! @{

    //! @brief コンストラクタです。
    basic_string_view() NN_NOEXCEPT
        : m_Str ( NULL )
        , m_Len ( 0 )
    {
    }

    /*!
    @brief      コンストラクタです。
    @param[in]  str   参照する文字列の先頭です。
    @param[in]  len   参照する文字列の長さです。
    */
    basic_string_view( const charT* str, size_type len ) NN_NOEXCEPT
        : m_Str ( str )
        , m_Len ( len )
    {
    }

    //! @brief      コンストラクタです。
    //! @param[in]  str   参照する文字列の先頭です。
    NN_IMPLICIT basic_string_view( const charT* str ) NN_NOEXCEPT
        : m_Str( str )
        , m_Len( str ? traits::length( str ) : 0 )
    {
    }

    //! @}

    //! @name イテレータ
    //! @{

    //! @brief  イテレータの先頭を返します。
    //! @return イテレータの先頭を返します。
    const_iterator begin() const NN_NOEXCEPT
    {
        return m_Str;
    }

    //! @brief  イテレータの終端を返します。
    //! @return イテレータの終端を返します。
    const_iterator end() const NN_NOEXCEPT
    {
        return m_Str + m_Len;
    }

    //! @brief  不変なイテレータの先頭を返します。
    //! @return 不変なイテレータの先頭を返します。
    const_iterator cbegin() const NN_NOEXCEPT
    {
        return m_Str;
    }

    //! @brief  不変なイテレータの終端を返します。
    //! @return 不変なイテレータの終端を返します。
    const_iterator cend() const NN_NOEXCEPT
    {
        return m_Str + m_Len;
    }

    //! @brief  不変なイテレータの終端を返します。
    //! @return 不変なイテレータの終端を返します。
    const_reverse_iterator rbegin() const NN_NOEXCEPT
    {
        return reverse_iterator( end() );
    }

    //! @brief  逆イテレータの終端を返します。
    //! @return 逆イテレータの終端を返します。
    const_reverse_iterator rend() const NN_NOEXCEPT
    {
        return reverse_iterator( begin() );
    }

    //! @brief  不変な逆イテレータの終端を返します。
    //! @return 不変な逆イテレータの終端を返します。
    const_reverse_iterator crbegin() const NN_NOEXCEPT
    {
        return const_reverse_iterator( cend() );
    }

    //! @brief  不変な逆イテレータの終端を返します。
    //! @return 不変な逆イテレータの終端を返します。
    const_reverse_iterator crend() const NN_NOEXCEPT
    {
        return const_reverse_iterator( cbegin() );
    }

    //! @}

    //! @name 取得
    //! @{

    //! @brief  文字列のサイズを返します。
    //! @return 文字列のサイズを返します。
    size_type size() const NN_NOEXCEPT
    {
        return m_Len;
    }

    //! @brief  文字列の長さを返します。
    //! @return 文字列の長さを返します。
    size_type length() const NN_NOEXCEPT
    {
        return m_Len;
    }

    //! @brief  参照中の文字列の先頭を返します。
    //! @return 参照中の文字列の先頭を返します。
    const_pointer data() const NN_NOEXCEPT
    {
        return m_Str;
    }

    /*!
    @brief  参照中の文字列が空であるかどうかを返します。
    @return 参照中の文字列が空であれば true を返します。
    */
    bool empty() const NN_NOEXCEPT
    {
        return m_Len == 0;
    }

    /*!
    @brief      部分文字列を取得します。
    @param[in]  pos     取得を開始する位置です。
    @param[in]  n       部分文字列の長さです。
    @pre        pos <= size()
    */
    self_type substr( size_type pos = 0, size_type n = npos ) const
    {
        NN_SDK_REQUIRES( pos <= size() );
        size_type rlen = std::min NN_PREVENT_MACRO_FUNC ( n, size() - pos );
        return self_type( m_Str + pos, rlen );
    }

    //! @}

    //! @name 要素アクセス
    //! @{

    /*!
    @brief      指定したインデックスの文字を返します。
    @param[in]  pos     取得する文字の位置です。
    @return     取得した文字を返します。
    @pre        pos < size()
    */
    const_reference operator[]( size_type pos ) const
    {
        NN_SDK_REQUIRES_RANGE( pos, size_type( 0 ), size() );
        return m_Str[ pos ];
    }

    /*!
    @brief      指定したインデックスの文字を返します。
    @param[in]  pos     取得する文字の位置です。
    @return     取得した文字を返します。
    @pre        pos < size()
    @details    範囲外のインデックスを指定した場合はアサートにかかります。@n
                アサートが無効の際に範囲外のインデックスを指定した場合はアボートします。
    */
    const_reference at( size_type pos ) const
    {
        NN_SDK_REQUIRES_RANGE( pos, size_type( 0 ), size() );
        NN_ABORT_UNLESS_RANGE( pos, size_type( 0 ), size() );
        return m_Str[ pos ];
    }

    /*!
    @brief  参照中の文字列の先頭の文字を取得します。
    @return 取得した文字を返します。
    @pre    !empty()
    */
    const_reference front() const
    {
        NN_SDK_REQUIRES( !empty() );
        return m_Str[ 0 ];
    }

    /*!
    @brief  参照中の文字列の末尾の文字を取得します。
    @return 取得した文字を返します。
    @pre    !empty()
    */
    const_reference back() const
    {
        NN_SDK_REQUIRES( !empty() );
        return m_Str[ size() - 1 ];
    }

    //! @}

    //! @name 削除
    //! @{

    //! @brief  文字列への参照を取り外します。
    void clear() NN_NOEXCEPT
    {
        m_Str = NULL;
        m_Len = 0;
    }

    /*!
    @brief      参照中の文字列から先頭の n 文字の参照を取り外します。
    @param[in]  n   参照を取り外す文字数です。
    @pre        n <= size()
    */
    void remove_prefix( size_type n )
    {
        NN_SDK_REQUIRES( n <= size() );
        m_Str += n;
        m_Len -= n;
    }

    /*!
    @brief      参照中の文字列から末尾の n 文字の参照を取り外します。
    @param[in]  n   参照を取り外す文字数です。
    @pre        n < size()
    */
    void remove_suffix( size_type n )
    {
        NN_SDK_REQUIRES( n <= size() );
        m_Len -= n;
    }

    //! @}

    //! @name 比較
    //! @{

    /*!
    @brief      指定した文字列と比較します。
    @param[in]  str     比較する文字列です。
    @return     指定した文字列より小さい場合は負の値を返します。@n
                指定した文字列と等しい場合はゼロを返します。@n
                指定した文字列より大きい場合は正の値を返します。
    */
    int compare( const self_type& str ) const NN_NOEXCEPT
    {
        size_type rlen = std::min NN_PREVENT_MACRO_FUNC ( size(), str.size() );
        int result = traits::compare( data(), str.data(), rlen );
        if (result != 0)
        {
            return result;
        }
        else if ( str.size() < size() )
        {
            return 1;
        }
        else if ( size() < str.size() )
        {
            return -1;
        }

        return 0;
    }

    /*!
    @brief      指定した文字列と比較します。
    @param[in]  pos     比較を開始する位置です。
    @param[in]  count   比較する文字数です。
    @param[in]  str     比較する文字列です。
    @return     指定した文字列より小さい場合は負の値を返します。@n
                指定した文字列と等しい場合はゼロを返します。@n
                指定した文字列より大きい場合は正の値を返します。
    */
    int compare(size_type pos, size_type count, const self_type& str) const
    {
        return substr(pos, count).compare(str);
    }

    /*!
    @brief      指定した文字列と比較します。
    @param[in]  pos1     比較を開始する位置です。
    @param[in]  count1   比較する文字数です。
    @param[in]  str      比較する文字列です。
    @param[in]  pos2     比較する文字列の比較を開始する位置です。
    @param[in]  count2   比較する文字列の比較する文字数です。
    @return     指定した文字列より小さい場合は負の値を返します。@n
                指定した文字列と等しい場合はゼロを返します。@n
                指定した文字列より大きい場合は正の値を返します。
    */
    int compare(size_type pos1, size_type count1, const self_type& str, size_type pos2, size_type count2) const
    {
        return substr(pos1, count1).compare(str.substr(pos2, count2));
    }

    /*!
    @brief      指定した文字列と比較します。
    @param[in]  str     比較する文字列です。
    @return     指定した文字列より小さい場合は負の値を返します。@n
                指定した文字列と等しい場合はゼロを返します。@n
                指定した文字列より大きい場合は正の値を返します。
    */
    int compare( const charT* str ) const
    {
        return compare( self_type( str ) );
    }

    /*!
    @brief      指定した文字列と比較します。
    @param[in]  pos     比較を開始する位置です。
    @param[in]  count   比較する文字数です。
    @param[in]  str     比較する文字列です。
    @return     指定した文字列より小さい場合は負の値を返します。@n
                指定した文字列と等しい場合はゼロを返します。@n
                指定した文字列より大きい場合は正の値を返します。
    */
    int compare(size_type pos, size_type count, const charT* str) const
    {
        return substr(pos, count).compare(self_type(str));
    }

    /*!
    @brief      指定した文字列と比較します。
    @param[in]  pos1     比較を開始する位置です。
    @param[in]  count1   比較する文字数です。
    @param[in]  str      比較する文字列です。
    @param[in]  count2   比較する文字列の比較する文字数です。
    @return     指定した文字列より小さい場合は負の値を返します。@n
                指定した文字列と等しい場合はゼロを返します。@n
                指定した文字列より大きい場合は正の値を返します。
    */
    int compare(size_type pos1, size_type count1, const charT* str, size_type count2) const
    {
        return substr(pos1, count1).compare(self_type(str, count2));
    }

    /*!
    @brief      指定された接頭辞で始まるかどうかを返します。
    @param[in]  str     比較する文字列です。
    @return     指定された接頭辞で始まるならば true、そうでなければ false を返します
    */
    bool starts_with(const self_type& str) const NN_NOEXCEPT
    {
        return size() >= str.size() && compare(0, str.size(), str) == 0;
    }

    /*!
    @brief      指定された接頭辞で始まるかどうかを返します。
    @param[in]  c       比較する文字です。
    @return     指定された接頭辞で始まるならば true、そうでなければ false を返します
    */
    bool starts_with(charT c) const NN_NOEXCEPT
    {
        return starts_with(self_type(&c, 1));
    }

    /*!
    @brief      指定された接頭辞で始まるかどうかを返します。
    @param[in]  str     比較する文字列です。
    @return     指定された接頭辞で始まるならば true、そうでなければ false を返します
    */
    bool starts_with(const charT* str) const
    {
        return starts_with(self_type(str));
    }

    /*!
    @brief      指定された接尾辞で終わるかどうかを返します。
    @param[in]  str     比較する文字列です。
    @return     指定された接尾辞で終わるならば true、そうでなければ false を返します
    */
    bool ends_with(const self_type& str) const NN_NOEXCEPT
    {
        return size() >= str.size() && compare(size() - str.size(), npos, str) == 0;
    }

    /*!
    @brief      指定された接尾辞で終わるかどうかを返します。
    @param[in]  c       比較する文字です。
    @return     指定された接尾辞で終わるならば true、そうでなければ false を返します
    */
    bool ends_with(charT c) const NN_NOEXCEPT
    {
        return ends_with(self_type(&c, 1));
    }

    /*!
    @brief      指定された接尾辞で終わるかどうかを返します。
    @param[in]  str     比較する文字列です。
    @return     指定された接尾辞で終わるならば true、そうでなければ false を返します
    */
    bool ends_with(const charT* str) const
    {
        return ends_with(self_type(str));
    }

    //! @}

    //! @name 探索
    //! @{

    /*!
    @brief      指定した文字列を探索して位置を返します。
    @param[in]  str     指定する文字列です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の先頭から探索を行います。
    @return     指定した文字列が見つかった位置です。
                文字列が見つからなかった場合、npos を返します。
    */
    size_type find( const self_type& str, size_type pos = 0 ) const NN_NOEXCEPT
    {
        const_iterator result = std::search( begin() + pos, end(), str.begin(), str.end() );
        return result == end() ? npos : static_cast< size_type >( std::distance( begin(), result ) );
    }

    /*!
    @brief      指定した文字を探索して位置を返します。
    @param[in]  c       指定する文字です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の先頭から探索を行います。
    @return     指定した文字が見つかった位置です。
                文字が見つからなかった場合、npos を返します。
    */
    size_type find( charT c, size_type pos = 0 ) const NN_NOEXCEPT
    {
        return find( self_type( &c, 1 ), pos );
    }

    /*!
    @brief      指定した文字列を探索して位置を返します。
    @param[in]  str     指定する文字列です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の先頭から探索を行います。
    @return     指定した文字列が見つかった位置です。
                文字列が見つからなかった場合、npos を返します。
    */
    size_type find( const charT* str, size_type pos = 0 ) const
    {
        return find( self_type( str ), pos );
    }

    /*!
    @brief      指定した文字列を逆順に探索して位置を返します。
    @param[in]  str     指定する文字列です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の末尾から探索を行います。
    @return     指定した文字列が見つかった位置です。
                文字列が見つからなかった場合、npos を返します。
    */
    size_type rfind( const self_type& str, size_type pos = npos ) const NN_NOEXCEPT
    {
        pos = std::min NN_PREVENT_MACRO_FUNC ( size() - 1, pos );
        int rpos = ( size() < str.size() + pos ) ? 0 : static_cast< int >( size() - ( str.size() + pos ) );

        const_reverse_iterator result = std::search( rbegin() + rpos, rend(), str.rbegin(), str.rend() );
        return result == rend() ? npos :
            size() - str.size() - static_cast< size_type >( std::distance( rbegin(), result ) );
    }

    /*!
    @brief      指定した文字を逆順に探索して位置を返します。
    @param[in]  c       探索する文字です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の末尾から探索を行います。
    @return     指定した文字が見つかった位置です。
                文字が見つからなかった場合、npos を返します。
    */
    size_type rfind( charT c, size_type pos = npos ) const NN_NOEXCEPT
    {
        return rfind( self_type( &c, 1 ), pos );
    }

    /*!
    @brief      指定した文字列を逆順に探索して位置を返します。
    @param[in]  str     指定する文字列です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の末尾から探索を行います。
    @return     指定した文字列が見つかった位置です。
                見つからなかった場合、npos を返します。
    */
    size_type rfind( const charT* str, size_type pos = npos ) const
    {
        return rfind( self_type( str ), pos );
    }

    /*!
    @brief      指定した文字列中の文字のいずれかと一致する文字を探索して位置を返します。
    @param[in]  str     指定する文字列です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の先頭から探索を行います。
    @return     指定した文字列中の文字が見つかった位置です。
                見つからなかった場合、npos を返します。
    */
    size_type find_first_of( const self_type& str, size_type pos = 0 ) const NN_NOEXCEPT
    {
        for ( const_iterator iter = begin() + pos, iterEnd = end(); iter != iterEnd; ++iter )
        {
            if ( npos != str.find( *iter ) )
            {
                return static_cast< size_type >( std::distance( begin(), iter ) );
            }
        }
        return npos;
    }

    /*!
    @brief      指定した文字列中の文字のいずれかと一致する文字を探索して位置を返します。
    @param[in]  str     指定する文字列です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の先頭から探索を行います。
    @return     指定した文字列中の文字が見つかった位置です。
                見つからなかった場合、npos を返します。
    */
    size_type find_first_of( const charT* str, size_type pos = 0 ) const
    {
        return find_first_of( self_type( str ), pos );
    }

    /*!
    @brief      指定した文字列中の文字のいずれかと一致する文字を逆順に探索して位置を返します。
    @param[in]  str     指定する文字列です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の末尾から探索を行います。
    @return     指定した文字列中の文字が見つかった位置です。
                見つからなかった場合、npos を返します。
    */
    size_type find_last_of( const self_type& str, size_type pos = npos ) const NN_NOEXCEPT
    {
        int rpos = static_cast< int >( ( size() - 1 ) - std::min NN_PREVENT_MACRO_FUNC ( size() - 1, pos ) );
        for ( const_reverse_iterator iter = rbegin() + rpos, iterEnd = rend(); iter != iterEnd; ++iter )
        {
            if ( npos != str.find( *iter ) )
            {
                return size() - 1 - static_cast< size_type >( std::distance( rbegin(), iter ) );
            }
        }
        return npos;
    }

    /*!
    @brief      指定した文字列中の文字のいずれかと一致する文字を逆順に探索して位置を返します。
    @param[in]  str     指定する文字列です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の末尾から探索を行います。
    @return     指定した文字列中の文字が見つかった位置です。
                見つからなかった場合、npos を返します。
    */
    size_type find_last_of( const charT* str, size_type pos = npos ) const
    {
        return find_last_of( self_type( str ), pos );
    }

    /*!
    @brief      指定した文字列中の文字と一致しない文字を探索して位置を返します。
    @param[in]  str     指定する文字列です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の先頭から探索を行います。
    @return     指定した文字列中にない文字が見つかった位置です。
                見つからなかった場合、npos を返します。
    */
    size_type find_first_not_of( const self_type& str, size_type pos = 0 ) const NN_NOEXCEPT
    {
        for ( const_iterator iter = begin() + pos, iterEnd = end(); iter != iterEnd; ++iter )
        {
            if ( npos == str.find( *iter ) )
            {
                return static_cast< size_type >( std::distance( begin(), iter ) );
            }
        }
        return npos;
    }

    /*!
    @brief      指定した文字と一致しない文字を探索して位置を返します。
    @param[in]  c       指定する文字です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の先頭から探索を行います。
    @return     指定した文字でない文字が見つかった位置です。
                見つからなかった場合、npos を返します。
    */
    size_type find_first_not_of( charT c, size_type pos = 0 ) const NN_NOEXCEPT
    {
        return find_first_not_of( self_type( &c, 1 ), pos );
    }

    /*!
    @brief      指定した文字列中の文字と一致しない文字を探索して位置を返します。
    @param[in]  str     指定する文字列です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の先頭から探索を行います。
    @return     指定した文字列中にない文字が見つかった位置です。
                見つからなかった場合、npos を返します。
    */
    size_type find_first_not_of( const charT* str, size_type pos = 0 ) const
    {
        return find_first_not_of( self_type( str ), pos );
    }

    /*!
    @brief      指定した文字列中の文字と一致しない文字を逆順に探索して位置を返します。
    @param[in]  str     指定する文字列です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の末尾から探索を行います。
    @return     指定した文字列中にない文字が見つかった位置です。
                見つからなかった場合、npos を返します。
    */
    size_type find_last_not_of( const self_type& str, size_type pos = npos ) const NN_NOEXCEPT
    {
        int rpos = static_cast< int >( ( size() - 1 ) - std::min NN_PREVENT_MACRO_FUNC ( size() - 1, pos ) );
        for ( const_reverse_iterator iter = rbegin() + rpos, iterEnd = rend(); iter != iterEnd; ++iter )
        {
            if ( npos == str.find( *iter ) )
            {
                return size() - 1 - static_cast< size_type >( std::distance( rbegin(), iter ) );
            }
        }
        return npos;
    }

    /*!
    @brief      指定した文字と一致しない文字を逆順に探索して位置を返します。
    @param[in]  c       指定する文字です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の末尾から探索を行います。
    @return     指定した文字でない文字が見つかった位置です。
                見つからなかった場合、npos を返します。
    */
    size_type find_last_not_of( charT c, size_type pos = npos ) const NN_NOEXCEPT
    {
        return find_last_not_of( self_type( &c, 1 ), pos );
    }

    /*!
    @brief      指定した文字列中の文字と一致しない文字を逆順に探索して位置を返します。
    @param[in]  str     指定する文字列です。
    @param[in]  pos     探索を開始する位置です。省略した場合、文字列の末尾から探索を行います。
    @return     指定した文字列中にない文字が見つかった位置です。
                見つからなかった場合、npos を返します。
    */
    size_type find_last_not_of( const charT* str, size_type pos = npos ) const
    {
        return find_last_not_of( self_type( str ), pos );
    }

    //! @}

    //! @brief 左辺と右辺が等しいかどうかを返します。
    friend bool operator==( const self_type& lhs, const self_type& rhs ) NN_NOEXCEPT
    {
        return lhs.compare( rhs ) == 0;
    }

    //! @brief 左辺と右辺が等しくないかどうかを返します。
    friend bool operator!=( const self_type& lhs, const self_type& rhs ) NN_NOEXCEPT
    {
        return lhs.compare( rhs ) != 0;
    }

    //! @brief 左辺が右辺よりも小さいかどうかを返します。
    friend bool operator< ( const self_type& lhs, const self_type& rhs ) NN_NOEXCEPT
    {
        return lhs.compare( rhs ) < 0;
    }

    //! @brief 左辺が右辺よりも大きいかどうかを返します。
    friend bool operator> ( const self_type& lhs, const self_type& rhs ) NN_NOEXCEPT
    {
        return lhs.compare( rhs ) > 0;
    }

    //! @brief 左辺が右辺よりも小さいもしくは等しいどうかを返します。
    friend bool operator<=( const self_type& lhs, const self_type& rhs ) NN_NOEXCEPT
    {
        return lhs.compare( rhs ) <= 0;
    }

    //! @brief 左辺が右辺よりも大きいもしくは等しいかどうかを返します。
    friend bool operator>=( const self_type& lhs, const self_type& rhs ) NN_NOEXCEPT
    {
        return lhs.compare( rhs ) >= 0;
    }

private:
    const_pointer m_Str;
    size_type     m_Len;
};

// クラスメンバの定義
//! @cond

template<class charT, class traits>
const size_t basic_string_view< charT, traits >::npos;

//! @endcond

//! @brief 文字の型が char である文字列参照です。
typedef basic_string_view< char > string_view;

}}  // namespace nn::util
