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
* @brief    エンディアン変換を行う API の宣言です。
*/

#include <type_traits>
#include <nn/nn_Common.h>
#include <nn/util/util_BitUtil.h>
#include <nn/util/detail/util_TypeTraits.h>

namespace nn { namespace util {

//! @name エンディアン変換
//! @{

/*!
@brief      エンディアンを反転します。
@tparam     T       データの型です。
@param[in]  pData   指定するデータのポインタです。
*/
template< typename T >
inline void SwapEndian( T* pData ) NN_NOEXCEPT
{
    typedef typename nn::util::detail::MakeUnsigned< T >::Type UIntType;
    UIntType u;
    std::memcpy( &u, pData, sizeof( T ) );
    u = reverse_bytes( u );
    std::memcpy( pData, &u, sizeof( T ) );
}

/*!
@brief      エンディアンを反転しながらデータをメモリから読み込みます。
@tparam     T       データの型です。
@param[in]  pSrc    指定するデータです。
@return     読み込んだ結果を返します。
*/
template< typename T >
inline T LoadEndianReversed( const T* pSrc ) NN_NOEXCEPT
{
    typedef typename nn::util::detail::MakeUnsigned< T >::Type UIntType;
    UIntType u;
    std::memcpy( &u, pSrc, sizeof( T ) );
    u = reverse_bytes( u );
    T result;
    std::memcpy( &result, &u, sizeof( T ) );
    return result;
}

/*!
@brief      エンディアンを反転しながらデータをメモリに書き出します。
@tparam     T       データの型です。
@param[out] pDst    データの書き出し先です。
@param[in]  src     書き出すデータです。
*/
template< typename T >
inline void StoreEndianReversed( T* pDst, T src ) NN_NOEXCEPT
{
    typedef typename nn::util::detail::MakeUnsigned< T >::Type UIntType;
    UIntType u;
    std::memcpy( &u, &src, sizeof( src ) );
    u = reverse_bytes( u );
    std::memcpy( pDst, &u, sizeof( src ) );
}

/*!
@brief      ビッグエンディアンのデータをメモリから読み込みます。
@tparam     T       データの型です。
@param[in]  pSrc    指定するデータです。
@return     読み込んだ結果を返します。
*/
template< typename T >
inline T LoadBigEndian( const T* pSrc ) NN_NOEXCEPT
{
#if defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_LITTLE )
    return LoadEndianReversed( pSrc );
#elif defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_BIG )
    return *pSrc;
#else
    #error Unknown NN_BUILD_TARGET_PLATFORM_ENDIAN
#endif
}

/*!
@brief      ビッグエンディアンでデータをメモリに書き出します。
@tparam     T       データの型です。
@param[out] pDst    データの書き出し先です。
@param[in]  src     書き出すデータです。
*/
template< typename T >
inline void StoreBigEndian( T* pDst, T src ) NN_NOEXCEPT
{
#if defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_LITTLE )
    StoreEndianReversed( pDst, src );
#elif defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_BIG )
    *pDst = src;
#else
    #error Unknown NN_BUILD_TARGET_PLATFORM_ENDIAN
#endif
}

/*!
@brief      リトルエンディアンのデータをメモリから読み込みます。
@tparam     T       データの型です。
@param[in]  pSrc    指定するデータです。
@return     読み込んだ結果を返します。
*/
template< typename T >
inline T LoadLittleEndian( const T* pSrc ) NN_NOEXCEPT
{
#if defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_LITTLE )
    return *pSrc;
#elif defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_BIG )
    return LoadEndianReversed( pSrc );
#else
    #error Unknown NN_BUILD_TARGET_PLATFORM_ENDIAN
#endif
}

/*!
@brief      リトルエンディアンでデータをメモリに書き出します。
@tparam     T       データの型です。
@param[out] pDst    データの書き出し先です。
@param[in]  src     書き出すデータです。
*/
template< typename T >
inline void StoreLittleEndian( T* pDst, T src ) NN_NOEXCEPT
{
#if defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_LITTLE )
    *pDst = src;
#elif defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_BIG )
    StoreEndianReversed( pDst, src );
#else
    #error Unknown NN_BUILD_TARGET_PLATFORM_ENDIAN
#endif
}

/*!
@brief      リトルエンディアンで表現された値をプラットフォームのエンディアンに変換します。
@tparam     T   データの型です。
@param[in]  var 変換するデータです。
@return     変換した結果を返します。
@pre        T は整数型である必要があります。
*/
template< typename T>
inline T FromLittleEndian( T var ) NN_NOEXCEPT
{
    NN_STATIC_ASSERT(std::is_integral<T>::value);
#if defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_LITTLE )
    return var;
#elif defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_BIG )
    SwapEndian( &var );
    return var;
#else
    #error Unknown NN_BUILD_TARGET_PLATFORM_ENDIAN
#endif
}

/*!
@brief      データをリトルエンディアンで表現された値に変換します。
@tparam     T   データの型です。
@param[in]  var 変換するデータです。
@return     変換した結果を返します。
@pre        T は整数型である必要があります。
*/
template< typename T>
inline T ToLittleEndian( T var ) NN_NOEXCEPT
{
    return FromLittleEndian( var );
}


/*!
@brief      ビッグエンディアンで表現された値をプラットフォームのエンディアンに変換します。
@tparam     T   データの型です。
@param[in]  var 変換するデータです。
@return     変換した結果を返します。
@pre        T は整数型である必要があります。
*/
template< typename T>
inline T FromBigEndian( T var ) NN_NOEXCEPT
{
    NN_STATIC_ASSERT(std::is_integral<T>::value);
#if defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_BIG )
    return var;
#elif defined( NN_BUILD_TARGET_PLATFORM_ENDIAN_LITTLE )
    SwapEndian( &var );
    return var;
#else
    #error Unknown NN_BUILD_TARGET_PLATFORM_ENDIAN
#endif
}

/*!
@brief      データをビッグエンディアンで表現された値に変換します。
@tparam     T   データの型です。
@param[in]  var 変換するデータです。
@return     変換した結果を返します。
@pre        T は整数型である必要があります。
*/
template< typename T>
inline T ToBigEndian( T var ) NN_NOEXCEPT
{
    return FromBigEndian( var );
}

//! @}

}} // namespace nn::util
