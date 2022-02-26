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
* @brief デバイスに関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_DeviceInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_RequiredMemory.h>
#include <nn/gfx/detail/gfx_UserPtr.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_Device-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_Device-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_Device-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_Device-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_Device-api.vk.1.h>
#endif

namespace nn {
namespace gfx {

class DisplayHandle;

/**
* @brief デバイスを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TDevice
    : public detail::DeviceImpl< typename detail::TargetVariation< TTarget >::Type >
    , private detail::RequiredMemory< detail::DeviceImpl<
        typename detail::TargetVariation< TTarget >::Type > >
{
    NN_DISALLOW_COPY( TDevice );

    typedef detail::DeviceImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief デバイスを初期化するための情報クラスです。
    */
    typedef DeviceInfo InfoType;

    /**
    * @brief 要求メモリー情報です。
    */
    enum RequiredMemoryInfo
    {
        RequiredMemoryInfo_Alignment = detail::RequiredMemory< Impl >::MemoryInfoImpl::RequiredMemoryInfo_Alignment
    };

    /**
    * @brief 要求されるメモリーのサイズを取得します。
    *
    * @param[in] info 初期化用の情報
    *
    * @return バイトでの要求されるメモリーのサイズを返します。
    */
    static size_t GetRequiredMemorySize( const InfoType& info ) NN_NOEXCEPT
    {
        return detail::RequiredMemory< Impl >::MemoryFuncImpl::GetRequiredMemorySize( info );
    }

    /**
    * @brief コンストラクターです。
    */
    TDevice() NN_NOEXCEPT
    {
    }

    /**
    * @brief デバイスを初期化します。
    *
    * @param[in] info 初期化用の情報
    *
    * @post
    * - デバイスが初期化されている
    */
    void Initialize( const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( info );
    }

    /**
    * @brief デバイスを破棄します。
    *
    * @post
    * - デバイスが初期化されていない
    */
    void Finalize() NN_NOEXCEPT
    {
        return Impl::Finalize();
    }

    /**
    * @brief 任意のユーザーポインターを設定します。
    *
    * @param[in] value 設定するユーザーポインター
    *
    * @post
    * - ユーザーポインターが設定されている
    */
    void SetUserPtr( void* value ) NN_NOEXCEPT
    {
        return SetUserPtrImpl( this, value );
    }

    /**
    * @brief 設定されている任意のユーザーポインターを取得します。
    *
    * @return 設定されている任意のユーザーポインターを返します。
    */
    void* GetUserPtr() NN_NOEXCEPT
    {
        return GetUserPtrImpl( this );
    }

    /**
    * @brief 設定されている任意のユーザーポインターを取得します。
    *
    * @return 設定されている任意のユーザーポインターを返します。
    */
    const void* GetUserPtr() const NN_NOEXCEPT
    {
        return GetUserPtrImpl( this );
    }
};

}
}
