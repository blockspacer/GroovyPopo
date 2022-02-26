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

//--------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------

#ifndef _VFX_TOOL
#include <nn/vfx/vfx_TargetDef.h>
#endif

namespace nn {
namespace vfx {
namespace detail {

//--------------------------------------------------------------------------------------
//! @brief      エンディアンユーティリティークラスです.
//--------------------------------------------------------------------------------------
class EndianUtil
{
public:
    //----------------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //!
    //! @tparam T                   フリップする値の型
    //! @param [in,out]      ptr    フリップする値のポインタ.
    //----------------------------------------------------------------------------------
    template< typename T >
    static void Flip( T* ptr ) NN_NOEXCEPT
    {
#if defined( VFX_ENDIAN_LITTLE )
        T orig = ( *ptr );
        char *src = reinterpret_cast< char* >( &orig );
        char *dst = reinterpret_cast< char* >( ptr );

        for( uint32_t i = 0; i < sizeof( T ); i++ )
        {
            dst[ i ] = src[ sizeof( T ) - 1 - i ];
        }
#else
        NN_UNUSED( ptr );
#endif
    }

    //----------------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //!
    //! @param [in,out]      ptr     フリップする値のポインタ.
    //----------------------------------------------------------------------------------
    static void Flip( nn::util::Float2* ptr ) NN_NOEXCEPT
    {
        FlipArray( 2, &( ptr->v[ 0 ] ) );
    }

    //----------------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //!
    //! @param [in,out]      ptr     フリップする値のポインタ.
    //----------------------------------------------------------------------------------
    static void Flip( nn::util::Float3* ptr ) NN_NOEXCEPT
    {
        FlipArray( 3, &( ptr->v[ 0 ] ) );
    }

    //----------------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //!
    //! @param [in,out]      ptr     フリップする値のポインタ.
    //----------------------------------------------------------------------------------
    static void Flip( nn::util::Float4* ptr ) NN_NOEXCEPT
    {
        FlipArray( 4, &( ptr->v[ 0 ] ) );
    }

    //----------------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //!
    //! @tparam T                           フリップする値の型
    //! @param [in]          arraySize      配列サイズ.
    //! @param [in,out]      pArray         フリップする配列の先頭ポインタ.
    //----------------------------------------------------------------------------------
    template< typename T >
    static void FlipArray( const size_t arraySize, T* pArray ) NN_NOEXCEPT
    {
        for( size_t i = 0; i < arraySize; ++i )
        {
            Flip( &pArray[ i ] );
        }
    }

    //----------------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //!
    //! @param [in]          arraySize     配列サイズ.
    //! @param [in,out]      pArray        フリップする配列の先頭ポインタ.
    //----------------------------------------------------------------------------------
    static void FlipArray( const size_t arraySize, unsigned char* pArray ) NN_NOEXCEPT
    {
        uint32_t* pTmpArray = reinterpret_cast<uint32_t*>( pArray );
        size_t      tmpSize = arraySize / 4;
        for( size_t i = 0; i < tmpSize; ++i )
        {
            Flip( &pTmpArray[i] );
        }
    }
};

} // namespace detail
} // namespace vfx
} // namespace nn
