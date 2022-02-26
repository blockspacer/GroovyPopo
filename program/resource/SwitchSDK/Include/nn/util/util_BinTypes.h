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
#include <nn/util/util_BytePtr.h>
#include <nn/util/util_Endian.h>
#include <nn/util/util_StringView.h>
#include <nn/util/detail/util_TypeTraits.h>

namespace nn { namespace util {

//! @brief 4 つの 1 バイト値からなるシグネチャを定義します。
#define NN_UTIL_CREATE_SIGNATURE_4( s0, s1, s2, s3 )\
    ( static_cast< int32_t >( ( s0 & 0xFF ) | ( ( s1 & 0xFF ) << 8 ) | ( ( s2 & 0xFF ) << 16 ) | ( ( s3 & 0xFF ) << 24 ) ) )

//! @brief 8 つの 1 バイト値からなるシグネチャを定義します。
#define NN_UTIL_CREATE_SIGNATURE_8( s0, s1, s2, s3, s4, s5, s6, s7 )\
    ( static_cast< int64_t >( NN_UTIL_CREATE_SIGNATURE_4( s0, s1, s2, s3 ) ) |\
    ( static_cast< int64_t >( NN_UTIL_CREATE_SIGNATURE_4( s4, s5, s6, s7 ) ) << 32 ) )

#if defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_LITTLE )
    //! @brief BinVersion に相当するバージョン値を定義します。
    #define NN_UTIL_CREATE_BINVERSION( major, minor, micro )\
        ( static_cast< uint32_t >( ( major & 0xFFFF ) << 16 | ( minor & 0xFF ) << 8  | ( micro & 0xFF ) ) )
#elif defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_BIG )
    //! @brief BinVersion に相当するバージョン値を定義します。
    #define NN_UTIL_CREATE_BINVERSION( major, minor, micro )\
        ( static_cast< uint32_t >( ::nn::util::reverse_bytes( static_cast< uint16_t >( major & 0xFFFF ) ) << 16 |\
        ( minor & 0xFF ) << 8 | ( micro & 0xFF ) ) )
#else
    #error Unknown NN_BUILD_TARGET_PLATFORM_ENDIAN
#endif

//! @brief  読み取り専用の機能です。構築しないでください。@n
//!         バイナリファイルヘッダのシリアライズ用シグネチャです。
struct BinFileSignature
{
    //! @brief      シグネチャが一致しているか判定します。
    //! @param[in]  signature   シグネチャに相当する 8 つの 1 バイト値の配列です。
    //! @return     シグネチャが一致していれば true を返します。
    bool IsValid( const char* signature ) const NN_NOEXCEPT
    {
        int length = static_cast< int >( sizeof( _str ) );
        for ( int i = 0; i < length; ++i )
        {
            if ( _str[ i ] != signature[ i ] )
            {
                return false;
            }
        }
        return true;
    }

    //! @brief      シグネチャが一致しているか判定します。
    //! @param[in]  signature   シグネチャに相当する値です。
    //! @return     シグネチャが一致していれば true を返します。
    bool IsValid( int64_t signature ) const NN_NOEXCEPT
    {
        return signature == util::LoadLittleEndian( &this->_packed );
    }

    //! @brief      シグネチャを設定します。
    //! @param[in]  signature   設定するシグネチャです。
    void Set( const char* signature ) NN_NOEXCEPT
    {
        std::memcpy( _str, signature, sizeof( _str ) );
    }

    //! @brief  シグネチャを取得します。
    //! @return シグネチャに相当する 8 つの 1 バイト値の配列を返します。
    const char* Get() const NN_NOEXCEPT
    {
        return _str;
    }

    //! @brief      シグネチャを設定します。
    //! @param[in]  signature   設定するシグネチャです。
    void SetPacked( int64_t signature ) NN_NOEXCEPT
    {
        this->_packed = signature;
    }

    //! @brief  シグネチャを取得します。
    //! @return シグネチャに相当する値を返します。
    int64_t GetPacked() const NN_NOEXCEPT
    {
        return util::LoadLittleEndian( &this->_packed );
    }

public:
    union
    {
        char _str[ 8 ];
        int64_t _packed;
    };
};

//! @brief  読み取り専用の機能です。構築しないでください。@n
//!         バイナリブロックヘッダのシリアライズ用シグネチャです。
struct BinBlockSignature
{
    //! @brief      シグネチャが一致しているか判定します。
    //! @param[in]  signature   判定するシグネチャに相当する 4 つの 1 バイト値の配列です。
    //! @return     シグネチャが一致していれば true を返します。
    bool IsValid( const char* signature ) const NN_NOEXCEPT
    {
        int length = static_cast< int >( sizeof( _str ) );
        for ( int i = 0; i < length; ++i )
        {
            if ( _str[ i ] != signature[ i ] )
            {
                return false;
            }
        }
        return true;
    }

    //! @brief      シグネチャが一致しているか判定します。
    //! @param[in]  signature   判定するシグネチャに相当する値です。
    //! @return     シグネチャが一致していれば true を返します。
    bool IsValid( int32_t signature ) const NN_NOEXCEPT
    {
        return signature == util::LoadLittleEndian( &this->_packed );
    }

    //! @brief      シグネチャを設定します。
    //! @param[in]  signature   設定するシグネチャです。
    void Set( const char* signature ) NN_NOEXCEPT
    {
        std::memcpy( _str, signature, sizeof( _str ) );
    }

    //! @brief  シグネチャを取得します。
    //! @return シグネチャに相当する 8 つの 1 バイト値の配列を返します。
    const char* Get() const NN_NOEXCEPT
    {
        return this->_str;
    }

    //! @brief      シグネチャを設定します。
    //! @param[in]  signature   設定するシグネチャです。
    void SetPacked( int32_t signature ) NN_NOEXCEPT
    {
        this->_packed = signature;
    }

    //! @brief  シグネチャを取得します。
    //! @return シグネチャに相当する値を返します。
    int32_t GetPacked() const NN_NOEXCEPT
    {
        return util::LoadLittleEndian( &this->_packed );
    }

public:
    union
    {
        char _str[ 4 ];
        int32_t _packed;
    };
};

//! @brief  読み取り専用の機能です。構築しないでください。@n
//!         バイナリファイルヘッダのシリアライズ用バージョンです。
struct BinVersion
{
    //! @brief  マイクロバージョンです。
    uint8_t micro;

    //! @brief  マイナーバージョンです。
    uint8_t minor;

    //! @brief  メジャーバージョンです。
    uint16_t major;

    //! @brief      バージョンが有効かどうかを取得します。
    //! @param[in]  majorVersion       メジャーバージョンです。
    //! @param[in]  minorVersion       マイナーバージョンです。
    //! @param[in]  microVersion       マイクロバージョンです。
    //! @return     バージョンが有効であれば true を返します。
    bool IsValid( int majorVersion, int minorVersion, int microVersion) const NN_NOEXCEPT
    {
        NN_UNUSED( microVersion);
        return util::LoadLittleEndian( &this->major ) == majorVersion && this->minor <= minorVersion;
    }

    //! @brief      バージョンを設定します。
    //! @param[in]  packedVersion   バージョンを示す値です。
    void SetPacked( uint32_t packedVersion )
    {
        util::StoreLittleEndian( &this->major, static_cast< uint16_t >( packedVersion >> 16 ) );
        this->minor = static_cast< uint8_t >( ( packedVersion >> 8 ) & 0xFF );
        this->micro = static_cast< uint8_t >( packedVersion & 0xFF );
    }

    //! @brief  バージョンに相当する値を取得します。
    //! @return バージョンに相当する値を返します。
    uint32_t GetPacked() const
    {
        return static_cast< uint32_t >( util::LoadLittleEndian( &this->major ) << 16 | minor << 8 | micro );
    }
};

//! @brief  読み取り専用の機能です。構築しないでください。@n
//!         バイナリリソースのシリアライズ用ポインタです。
template< typename T >
struct BinTPtr
{
public:
    typedef T* pointer;
    typedef typename util::detail::AddConst< T >::Type* const_pointer;
    typedef int64_t difference_type;

    //! @brief  オフセットを 0 で初期化します。
    void Clear() NN_NOEXCEPT
    {
        _offset = 0;
    }

    //! @brief      ポインタを設定します。
    //! @param[in]  ptr     設定するポインタです。
    void Set( pointer ptr ) NN_NOEXCEPT
    {
        _ptr = ptr;
    }

    //! @brief  ポインタを取得します。
    //! @return ポインタを返します。
    //! @pre    @ref SetOffset() でオフセットを設定した場合、
    //!         @ref Relocate() でポインタに変換する必要があります。
    pointer Get() NN_NOEXCEPT
    {
        return _ptr;
    }

    //! @brief  ポインタを取得します。
    //! @return ポインタを返します。
    //! @pre    @ref SetOffset() でオフセットを設定した場合、
    //!         @ref Relocate() でポインタに変換する必要があります。
    const_pointer Get() const NN_NOEXCEPT
    {
        return _ptr;
    }

    //! @briefprivate   ポインタから計算したオフセットを設定します。
    //! @param[in]      pBase   オフセットの基準となる先頭のポインタです。
    //! @param[in]      ptr     設定するポインタです。
    void SetOffset( void* pBase, void* ptr ) NN_NOEXCEPT
    {
        if ( ptr )
        {
            difference_type offset = static_cast< difference_type >( util::BytePtr( pBase ).Distance( ptr ) );
            util::StoreLittleEndian( &_offset, offset );
        }
        else
        {
            _offset = 0;
        }
    }

    //! @briefprivate   オフセットを設定します。
    //! @param[in]      offset  オフセットの値です。
    void SetOffset( difference_type offset ) NN_NOEXCEPT
    {
        util::StoreLittleEndian( &_offset, offset );
    }

    //! @briefprivate   オフセットを取得します。
    //! @return         オフセットを返します。
    //! @pre            @ref Set() でポインタを設定した場合、
    //!                 @ref Unrelocate() でオフセットに変換する必要があります。
    difference_type GetOffset() const NN_NOEXCEPT
    {
        return util::LoadLittleEndian( &_offset );
    }

    //! @briefprivate   オフセットから計算したポインタを取得します。
    //! @param[in]      pBase   オフセットの基準となる先頭のポインタです。
    //! @pre            @ref Set() でポインタを設定した場合、
    //!                 @ref Unrelocate() でオフセットに変換する必要があります。
    //! @return オフセットから計算したポインタを返します。
    pointer ToPtr( void* pBase ) const NN_NOEXCEPT
    {
        ptrdiff_t offset = static_cast< ptrdiff_t >( GetOffset() );
        if ( offset )
        {
            return util::BytePtr( pBase, offset ).Get< T >();
        }
        else
        {
            return NULL;
        }
    }

    //! @briefprivate   オフセットをポインタに変換します。
    //! @param[in]      pBase   オフセットの基準となる先頭のポインタです。
    void Relocate( void* pBase ) NN_NOEXCEPT
    {
        _ptr = ToPtr( pBase );
    }

    //! @briefprivate   ポインタをオフセットに変換します。
    //! @param[in]      pBase   オフセットの基準となる先頭のポインタです。
    void Unrelocate( void* pBase ) NN_NOEXCEPT
    {
        SetOffset( pBase, _ptr );
    }

public:
    union
    {
        difference_type _offset;
        pointer _ptr;
    };
};

typedef BinTPtr< void > BinPtr;

//--------------------------------------------------------------------------------------------------

//! @brief  読み取り専用の機能です。構築しないでください。@n
//!         バイナリリソースのシリアライズ用文字列です。
template< typename CharT >
struct BinTString
{
    //! @brief 文字の型です。
    typedef CharT char_type;

    //! @brief 文字列参照の型です。
    typedef util::basic_string_view< char_type > StringViewType;

    //! @brief  文字列の長さを取得します。
    //! @return 文字列の長さを返します。
    int GetLength() const NN_NOEXCEPT
    {
        return static_cast< int >( _length );
    }

    //! @brief  文字列の先頭へのポインタを取得します。
    //! @return 文字列の先頭へのポインタを返します。
    const char_type* GetData() const NN_NOEXCEPT
    {
        return _data;
    }

    //! @brief  文字列参照を取得します。
    //! @return 文字列参照を返します。
    StringViewType Get() const NN_NOEXCEPT
    {
        return StringViewType( _data, _length );
    }

    //! @brief  文字列参照へ変換します。
    //! @return 変換した文字列参照を返します。
    NN_IMPLICIT operator StringViewType() const NN_NOEXCEPT
    {
        return Get();
    }

    //! @brief      指定した文字列を格納するのに必要なバッファのバイトサイズを返します。
    //! @param[in]  length  文字列の長さを指定します。
    //! @return     バッファのバイトサイズを返します。
    static size_t CalculateSize( int length ) NN_NOEXCEPT
    {
        size_t dataSize = ( length + 1 ) * sizeof( char_type ); // NUL 終端分を追加。
        return sizeof( uint16_t ) + util::align_up( dataSize, sizeof( uint16_t ) );
    }

    //! @brief      指定した文字列でバッファを初期化します。
    //! @param[in]  str バッファを初期化する文字列です。
    void Initialize( const StringViewType& str ) NN_NOEXCEPT
    {
        size_t size = CalculateSize( static_cast< int >( str.length() ) );

        // 文字列長。
        uint16_t length = static_cast< uint16_t >( str.length() );
        _length = length;

        // 文字列。
        std::copy( str.cbegin(), str.cend(), &_data[ 0 ] );

        // NUL 終端とパディング。
        int padding = static_cast< int >( ( size - sizeof( uint16_t ) ) / sizeof( char_type ) );
        padding -= length;
        std::fill_n( _data + length, padding, char_type( 0 ) );
    }

    //! @brief  文字列プール内に存在する次の文字列を取得します。
    //! @return 次の文字列を返します。
    BinTString< char_type >* GetNext() NN_NOEXCEPT
    {
        ptrdiff_t offset = static_cast< ptrdiff_t >( CalculateSize( GetLength() ) );
        return util::BytePtr( this, offset ).Get< BinTString< char_type > >();
    }

    //! @brief  文字列プール内に存在する次の文字列を取得します。
    //! @return 次の文字列を返します。
    const BinTString< char_type >* GetNext() const NN_NOEXCEPT
    {
        ptrdiff_t offset = static_cast< ptrdiff_t >( CalculateSize( GetLength() ) );
        return util::ConstBytePtr( this, offset ).Get< BinTString< char_type > >();
    }

public:
    uint16_t _length;
    char_type _data[ sizeof( uint16_t ) / sizeof( char_type ) ];
};

//! @brief 文字の型が char である BinTString です。
typedef BinTString< char > BinString;

//! @brief BinString 型を扱う BinTPtr です。
typedef BinTPtr< BinString > BinPtrToString;

}} // namespace nn/util
