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

/**
* @file
* @brief    ビット演算を行う API の宣言です。
*/

#include <cstring>
#include <climits>
#include <nn/nn_Common.h>
#include <nn/nn_SdkAssert.h>
#include <nn/nn_StaticAssert.h>
#include <nn/util/detail/util_TypeTraits.h>

namespace nn { namespace util {

//! @name ビット演算関数
//! @{

/*!
@brief      値をビット列に見立て、ビットの並び順を反転します。
@tparam     T               数値の型です。
@param[in]  x               指定する値です。
@param[in]  subword_bits    ビットの並び順を反転する単位を指定します。デフォルト値は 1 です。
@param[in]  num_swar_words  ビット列を分割するグループ数を指定します。デフォルト値は 1 です。
@return     結果を返します。
@pre        - 0 < num_swar_words <= @ref NN_BITSIZEOF( T )
            - @ref NN_BITSIZEOF( T ) % num_swar_words == 0
            - 0 < subword_bits <= @ref NN_BITSIZEOF( T )
            - ( @ref NN_BITSIZEOF( T ) / num_swar_words ) % subword_bits == 0
@details    ビット列を num_swar_words 個に分割した各グループ内で、subword_bits 個のビットの組を作り、その並び順を反転します。
            グループ自体の並び順は変化せず、組内のビットの並び順も変化しません。@n

            例）8 ビットのビット列を ABCDEFGH と表記した場合、結果は以下のようになります。 @n
            reverse_bits(ABCDEFGH) -> HGFEDCBA@n
            reverse_bits(ABCDEFGH, 1, 2) -> DCBAHGFE@n
            reverse_bits(ABCDEFGH, 1, 4) -> BADCFEHG@n
            reverse_bits(ABCDEFGH, 2, 1) -> GHEFCDAB@n
            reverse_bits(ABCDEFGH, 2, 2) -> CDABGHEF@n
            reverse_bits(ABCDEFGH, 4, 1) -> EFGHABCD
*/
template< typename T >
T reverse_bits( T x, int subword_bits=1, int num_swar_words=1 ) NN_NOEXCEPT;

/*!
@brief      値をバイト列に見立て、バイトの並び順を反転します。
@tparam     T               数値の型です。
@param[in]  x               指定する値です。
@param[in]  subword_bytes   バイトの並び順を反転する単位を指定します。デフォルト値は 1 です。
@param[in]  num_swar_words  バイト列を分割するグループ数を指定します。デフォルト値は 1 です。
@return     結果を返します。
@pre        - 0 < num_swar_words <= sizeof( T )
            - sizeof( T ) % num_swar_words == 0
            - 0 < subword_bytes <= sizeof( T )
            - ( sizeof( T ) / num_swar_words ) % subword_bytes == 0
@details    バイト列を num_swar_words 個に分割した各グループ内で、subword_bytes 個のバイトの組を作り、その並び順を反転します。
            グループ自体の並び順は変化せず、組内のバイトの並び順も変化しません。@n

            例）8 バイトのバイト列を ABCDEFGH と表記した場合、結果は以下のようになります。 @n
            reverse_bytes(ABCDEFGH) -> HGFEDCBA@n
            reverse_bytes(ABCDEFGH, 1, 2) -> DCBAHGFE@n
            reverse_bytes(ABCDEFGH, 1, 4) -> BADCFEHG@n
            reverse_bytes(ABCDEFGH, 2, 1) -> GHEFCDAB@n
            reverse_bytes(ABCDEFGH, 2, 2) -> CDABGHEF@n
            reverse_bytes(ABCDEFGH, 4, 1) -> EFGHABCD
*/
template< typename T >
inline T reverse_bytes( T x, int subword_bytes=1, int num_swar_words=1) NN_NOEXCEPT
{
    return reverse_bits( x, subword_bytes * CHAR_BIT, num_swar_words );
}

/*!
@brief      値が指定したアライメントに揃っているかどうかを返します。
@tparam     T       数値の型です。
@param[in]  x       指定する値です。
@param[in]  align   指定するアライメントです。
@return     結果を返します。
@pre        align は 2 のべき乗である必要があります。
*/
template< typename T >
inline bool is_aligned( T x, size_t align ) NN_NOEXCEPT
{
    typedef typename nn::util::detail::MakeUnsigned< T >::Type UIntType;
    UIntType mask = static_cast< UIntType >( align - 1 );
    return ( x & mask ) == 0;
}

/*!
@brief      値を指定したアライメントまで切り上げます。
@tparam     T       数値の型です。
@param[in]  x       指定する値です。
@param[in]  align   指定するアライメントです。
@return     結果を返します。
@pre        align は 2 のべき乗である必要があります。
*/
template< typename T >
inline T align_up( T x, size_t align ) NN_NOEXCEPT
{
    typedef typename nn::util::detail::MakeUnsigned< T >::Type UIntType;
    UIntType mask = static_cast< UIntType >( align - 1 );
    return static_cast< T >( ( x + mask ) & ~mask );
}

/*!
@brief      値を指定したアライメントまで切り下げます。
@tparam     T       数値の型です。
@param[in]  x       指定する値です。
@param[in]  align   指定するアライメントです。
@return     結果を返します。
@pre        align は 2 のべき乗である必要があります。
*/
template< typename T >
inline T align_down( T x, size_t align ) NN_NOEXCEPT
{
    typedef typename nn::util::detail::MakeUnsigned< T >::Type UIntType;
    UIntType mask = static_cast< UIntType >( align - 1 );
    return static_cast< T >( x & ~mask );
}

/*!
@brief      最下位の 1 のビットをオフにします。
@tparam     T       数値の型です。
@param[in]  x       指定する値です。
@return     結果を返します。
@details    例) 1100 1100 -> 1100 1000
*/
template< typename T >
inline T rstls1b( T x ) NN_NOEXCEPT
{
    return x & ( x - 1 );
}

/*!
@brief      最下位の 0 のビットをオンにします。
@tparam     T       数値の型です。
@param[in]  x       指定する値です。
@return     結果を返します。
@details    例) 0011 0011 -> 0011 0111
*/
template< typename T >
inline T setls0b( T x ) NN_NOEXCEPT
{
    return x | ( x + 1 );
}

/*!
@brief      末尾から連続した 1 のビットをオフにします。
@tparam     T       数値の型です。
@param[in]  x       指定する値です。
@return     結果を返します。
@details    例) 0011 0011 -> 0011 0000
*/
template< typename T >
inline T rstt1( T x ) NN_NOEXCEPT
{
    return x & ( x + 1 );
}

/*!
@brief      末尾から連続した 0 のビットをオンにします。
@tparam     T       数値の型です。
@param[in]  x       指定する値です。
@return     結果を返します。
@details    例) 1100 1100 -> 1100 1111
*/
template< typename T >
inline T sett0( T x ) NN_NOEXCEPT
{
    return x | ( x - 1 );
}

/*!
@brief      最下位の 1 のビットを取り出します。
@tparam     T       数値の型です。
@param[in]  x       指定する値です。
@return     結果を返します。
@details    例) 1100 1100 -> 0000 0100
*/
template< typename T >
inline T isols1b( T x ) NN_NOEXCEPT
{
    return x & ~( x - 1 ); // <-> ~x | ( x - 1 )
}

/*!
@brief      最下位の 0 のビットを取り出します。
@tparam     T       数値の型です。
@param[in]  x       指定する値です。
@return     結果を返します。
@details    例) 0011 0011 -> 0000 0100
*/
template< typename T >
inline T isols0b( T x ) NN_NOEXCEPT
{
    return ~x & ( x + 1 ); // <-> x | ~( x + 1 )
}

/*!
@brief      末尾から連続した 1 のビットを取り出します。
@tparam     T       数値の型です。
@param[in]  x       指定する値です。
@return     結果を返します。
@details    例) 0011 0011 -> 0000 0011
*/
template< typename T >
inline T maskt1( T x ) NN_NOEXCEPT
{
    return x & ~( x + 1 ); // <-> ~x | ( x + 1 )
}

/*!
@brief      末尾から連続した 0 のビットを取り出します。
@tparam     T       数値の型です。
@param[in]  x       指定する値です。
@return     結果を返します。
@details    例) 1100 1100 -> 0000 0011
*/
template< typename T >
inline T maskt0( T x ) NN_NOEXCEPT
{
    return ~x & ( x - 1 ); // <-> x | ~( x - 1 );
}

/*!
@brief      末尾から連続した 1 のビットと最下位の 0 のビットを取り出します。
@tparam     T       数値の型です。
@param[in]  x       指定する値です。
@return     結果を返します。
@details    例) 0011 0011 -> 0000 0111
*/
template< typename T >
inline T maskt1ls0b( T x ) NN_NOEXCEPT
{
    return x ^ ( x + 1 );
}

/*!
@brief      末尾から連続した 0 のビットと最下位の 1 のビットを取り出します。
@tparam     T       数値の型です。
@param[in]  x       指定する値です。
@return     結果を返します。
@details    例) 1100 1100 -> 0000 0111
*/
template< typename T >
inline T maskt0ls1b( T x ) NN_NOEXCEPT
{
    return x ^ ( x - 1 );
}

/*!
@brief      1 のビットを数えます。
@tparam     T   数値の型です。
@param[in]  x   指定する値です。
@return     結果を返します。
*/
template< typename T >
int popcount( T x ) NN_NOEXCEPT;

/*!
@brief      最上位から連続する 0 のビットの数を返します。
@tparam     T   数値の型です。
@param[in]  x   指定する値です。
@return     結果を返します。
*/
template< typename T >
int cntl0( T x ) NN_NOEXCEPT;

/*!
@brief      最下位から連続する 0 のビットの数を返します。
@tparam     T   数値の型です。
@param[in]  x   指定する値です。
@return     結果を返します。
*/
template< typename T >
inline int cntt0( T x ) NN_NOEXCEPT
{
    return popcount( static_cast< T >( isols1b( x ) - 1 ) );
}

/*!
@brief      指定した値が 2 のべき乗かどうかを返します。
@tparam     T   数値の型です。
@param[in]  x   指定する値です。
@return     x が正の 2 のべき乗である場合に true を返します。
            x が 0 の場合は false を返します。
*/
template< typename T >
inline bool ispow2( T x ) NN_NOEXCEPT
{
    return x > 0 && rstls1b( x ) == 0;
}

/*!
@brief      指定した値以下の最大の 2 のべき乗の値を返します。
@tparam     T   数値の型です。
@param[in]  x   指定する値です。
@return     結果を返します。
@pre        x > 0
*/
template< typename T >
inline T floorp2( T x ) NN_NOEXCEPT
{
    NN_SDK_REQUIRES( x > 0 );
    return T( 1 ) << ( NN_BITSIZEOF( T ) - cntl0( x ) - 1 );
}

/*!
@brief      指定した値以上の最小の 2 のべき乗の値を返します。
@tparam     T   数値の型です。
@param[in]  x   指定する値です。
@return     結果を返します。
@pre        x > 0
*/
template< typename T >
inline T ceilp2( T x ) NN_NOEXCEPT
{
    NN_SDK_REQUIRES( x > 0 );
    return T( 1 ) << ( NN_BITSIZEOF( T ) - cntl0( T( x - 1 ) ) );
}




/*!
@brief      端数を切り上げつつ値を指定した数値で割ります。
@tparam     T       被除数の型です。
@tparam     U       除数の型です。
@param[in]  x       被除数です。
@param[in]  divider 除数です。
@return     結果を返します。
@pre
            - x > 0
            - divider > 0
*/
template< typename T, typename U >
inline T DivideUp( T x, U divider ) NN_NOEXCEPT
{
    typedef typename nn::util::detail::MakeUnsigned< U >::Type UIntType;
    UIntType adder = static_cast< UIntType >( divider ) - 1;
    return static_cast< T >( ( x + adder ) / divider );
}

//! @}

//---------------------------------------------------------------------------

namespace detail {

//! @cond

//! @brief 指定した数に対する2を底とする対数を返します。
template< int N > struct Log2
{
    static const int Value = Log2< N / 2 >::Value + 1;

private:
    NN_STATIC_ASSERT( N > 0 );
};

template<> struct Log2< 1 >
{
    static const int Value = 0;
};

//! @endcond

} // namespace detail

//---------------------------------------------------------------------------

template< typename T >
inline T reverse_bits( T x, int subword_bits, int num_swar_words ) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_RANGE( num_swar_words, 0, ( NN_BITSIZEOF( T ) ) + 1 );
    NN_SDK_REQUIRES_EQUAL( 0, NN_BITSIZEOF( T ) % num_swar_words );
    NN_SDK_REQUIRES_RANGE( subword_bits, 0, ( NN_BITSIZEOF( T ) / num_swar_words ) + 1 );
    NN_SDK_REQUIRES_EQUAL( 0, ( NN_BITSIZEOF( T ) / num_swar_words ) % subword_bits );

    typedef typename nn::util::detail::MakeUnsigned< T >::Type UIntType;
    int words_size = NN_BITSIZEOF( T ) / num_swar_words;
    int k = words_size - subword_bits;

    UIntType u;
    std::memcpy( &u, &x, sizeof( x ) );

    // 1bit単位 → 2bit単位 → 4bit単位 → … でグループを区切り、グループごとにスワップしていく
    for ( int i = 1; i < NN_BITSIZEOF( T ); i = i << 1 )
    {
        // m1=0x5555...., m2=0x3333...., m4=0x0F0F....,
        const UIntType mi = static_cast< UIntType >( UIntType( -1 ) / ( ( UIntType( 1 ) << i ) + 1 ) );
        const UIntType nmi = static_cast< UIntType >( ~mi );

        if ( k & i )
        {
            u = static_cast< UIntType >( (( u & mi ) << i ) | (( u & nmi ) >> i ) );
        }
    }

    std::memcpy( &x, &u, sizeof( x ) );
    return x;
}

template< typename T >
inline int popcount( T x ) NN_NOEXCEPT
{
    typedef typename nn::util::detail::MakeUnsigned< T >::Type UIntType;

    UIntType u;
    std::memcpy( &u, &x, sizeof( x ) );

    const UIntType m1 = UIntType( -1 ) / 0x03; // 0x5555...
    const UIntType m2 = UIntType( -1 ) / 0x05; // 0x3333...
    const UIntType m4 = UIntType( -1 ) / 0x11; // 0x0F0F...

    u -= ( u >> 1 ) & m1;
    u = static_cast< UIntType >( ( u & m2 ) + ( ( u >> 2 ) & m2 ) );
    u = static_cast< UIntType >( ( u + ( u >> 4 ) ) & m4 );

    for ( int i = 0; i < nn::util::detail::Log2 < static_cast< int >( sizeof( T ) ) >::Value; ++i )
    {
        const int shift = ( 0x1 << i ) * CHAR_BIT;
        u += u >> shift;
    }

    return static_cast< int >( u & 0x7Fu );
}

template< typename T >
inline int cntl0( T x ) NN_NOEXCEPT
{
    for ( int i = 0; i < nn::util::detail::Log2< NN_BITSIZEOF( T ) >::Value; ++i )
    {
        const int shift = 0x1 << i;
        x |= x >> shift;
    }
    return popcount( static_cast< T >( ~x ) );
}

#if !defined(NN_BUILD_CONFIG_COMPILER_CLANG)
NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_OVERFLOW
    NN_STATIC_ASSERT( INT_MAX + 1 == INT_MIN );
    NN_STATIC_ASSERT( INT_MIN - 1 == INT_MAX );
    NN_STATIC_ASSERT( -1 == ~( 1 - 1 ) );
NN_PRAGMA_POP_WARNINGS
#endif

}} // namespace nn::util
