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

#include <nn/vfx/vfx_TargetDef.h>

namespace nn {
namespace vfx {
namespace detail {

//---------------------------------------------------
//! @brief メモリ操作ユーティリティー
//---------------------------------------------------
class MemUtil
{
public:
    //---------------------------------------------------------------------------
    //! @brief        メモリのある範囲を一定の値で埋める。
    //!
    //! @param[in]    buffer           メモリ位置。
    //! @param[in]    value           埋める値。
    //! @param[in]    size          サイズ。
    //! @return                     先頭アドレス
    //---------------------------------------------------------------------------
    static void* Fill( void* buffer, int value, size_t size ) NN_NOEXCEPT
    {
        return memset( buffer, value, size );
    }

    //---------------------------------------------------------------------------
    //! @brief        メモリのある範囲をゼロで埋める。
    //!
    //! @param[in]    buffer           メモリ位置。
    //! @param[in]    size          サイズ。
    //! @return                     先頭アドレス
    //---------------------------------------------------------------------------
    static void* FillZero( void* buffer, size_t size ) NN_NOEXCEPT
    {
        return memset( buffer, 0, size );
    }

    //---------------------------------------------------------------------------
    //! @brief        メモリのある範囲を別の範囲へコピー（高速だが範囲が重なるとエラー）。
    //!
    //! @param[in]    pDst           コピー先。
    //! @param[in]    pSrc           コピー元。
    //! @param[in]    size          サイズ。
    //! @return                     コピー先のアドレス
    //---------------------------------------------------------------------------
    static void* Copy( void* pDst, const void* pSrc, size_t size ) NN_NOEXCEPT
    {
#if defined( NN_BUILD_CONFIG_OS_COS )
        return OSBlockMove( pDst, pSrc, size, false );
#else
        return memcpy( pDst, pSrc, size );
#endif
    }

    //---------------------------------------------------------------------------
    //! @brief        キャッシュをフラッシュする。
    //!
    //! @param[in]    ptr           フラッシュ先のアドレス。
    //! @param[in]    size          フラッシュサイズ。
    //---------------------------------------------------------------------------
    static void FlushCache( void* ptr, size_t size ) NN_NOEXCEPT
    {
#if defined( NN_BUILD_CONFIG_OS_COS )
        DCFlushRange( ptr, size );
#else
        NN_UNUSED( ptr );
        NN_UNUSED( size );
#endif
    }
};

} // namespace detail
} // namespace vfx
} // namespace nn
