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
* @brief サンプラーに関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_SamplerInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_UserPtr.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_Sampler-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_Sampler-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_Sampler-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_Sampler-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_Sampler-api.vk.1.h>
#endif

namespace nn {
namespace gfx {

/**
* @brief サンプラーを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TSampler
    : public detail::SamplerImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TSampler );

    typedef detail::SamplerImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief サンプラーを初期化するための情報クラスです。
    */
    typedef SamplerInfo InfoType;

    /**
    * @brief コンストラクターです。
    */
    TSampler() NN_NOEXCEPT
    {
    }

    /**
    * @brief サンプラーを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - サンプラーが初期化されていない
    *
    * @post
    * - サンプラーが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief サンプラーを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - サンプラーが初期化されていない
    *
    * @post
    * - サンプラーが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const TSamplerInfo< Target >& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief サンプラーを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - サンプラーが初期化されている
    *
    * @post
    * - サンプラーが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }

    /**
    * @brief デバッグ用のラベルを設定します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] label NULL で終端されるラベル文字列へのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - サンプラーが初期化されている
    *
    * @details
    * すでにラベルが設定されていた場合は、新しく指定されるラベルで設定を上書きします。@n
    * label に NULL を指定した場合は、設定されているラベルを削除します。
    *
    */
    void SetDebugLabel( TDevice< Target >* pDevice, const char* label ) NN_NOEXCEPT
    {
        return detail::DebugLabelHelper::SetDebugLabel< Impl >( this, pDevice, label );
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
