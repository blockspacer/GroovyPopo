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
* @brief 同期に関する API の宣言
*/

#pragma once

#include <nn/nn_TimeSpan.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_SyncInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_Sync-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_Sync-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_Sync-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_Sync-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_Sync-api.vk.1.h>
#endif

namespace nn {
namespace gfx {

/**
* @brief フェンスを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TFence
    : public detail::FenceImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TFence );

    typedef detail::FenceImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief フェンスを初期化するための情報クラスです。
    */
    typedef FenceInfo InfoType;

    /**
    * @brief コンストラクターです。
    */
    TFence() NN_NOEXCEPT
    {
    }

    /**
    * @brief フェンスを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - フェンスが初期化されていない
    *
    * @post
    * - フェンスが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief フェンスを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - フェンスが初期化されている
    *
    * @post
    * - フェンスが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }

    /**
    * @brief フェンスがシグナル状態になっているかどうかを取得します。
    *
    * @return
    * フェンスがシグナル状態になっているかどうかを表す真偽値を返します。
    *
    * @pre
    * - フェンスがキューにセットされている
    */
    bool IsSignaled() NN_NOEXCEPT
    {
        return Impl::IsSignaled();
    }

    /**
    * @brief フェンスがシグナル状態になるのを同期します。
    *
    * @param[in] timeout 同期する最大待ち時間
    *
    * @return 同期処理の結果を返します。
    * @retval SyncResult_Success 成功しました。
    * @retval SyncResult_TimeoutExpired フェンスがシグナル状態になる前に timeout で指定されている時間を過ぎました
    *
    * @pre
    * - フェンスがキューにセットされている
    *
    * @details
    * フェンスがシグナル状態になるまで、または指定された時間に達するまで処理を返しません。
    */
    SyncResult Sync( TimeSpan timeout ) NN_NOEXCEPT
    {
        return Impl::Sync( timeout );
    }
};

/**
* @brief セマフォを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TSemaphore
    : public detail::SemaphoreImpl< typename detail::TargetVariation< TTarget >::Type >
{
    typedef detail::SemaphoreImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

    NN_DISALLOW_COPY( TSemaphore );

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief セマフォを初期化するための情報クラスです。
    */
    typedef SemaphoreInfo InfoType;

    /**
    * @brief コンストラクターです。
    */
    TSemaphore() NN_NOEXCEPT
    {
    }

    /**
    * @brief セマフォを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - セマフォが初期化されていない
    *
    * @post
    * - セマフォが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief セマフォを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - セマフォが初期化されている
    *
    * @post
    * - セマフォが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }
};

}
}
