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
* @brief キューに関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_QueueInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_UserPtr.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_Queue-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_Queue-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_Queue-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_Queue-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_Queue-api.vk.1.h>
#endif

namespace nn {
namespace gfx {

/**
* @brief キューを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TQueue
    : public detail::QueueImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TQueue );

    typedef detail::QueueImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief キューを初期化するための情報クラスです。
    */
    typedef QueueInfo InfoType;

    /**
    * @brief コンストラクターです。
    */
    TQueue() NN_NOEXCEPT
    {
    }

    /**
    * @brief キューを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - キューが初期化されていない
    *
    * @post
    * - キューが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief キューを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - キューが初期化されていない
    *
    * @post
    * - キューが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const TQueueInfo< Target >& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief キューを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - キューが初期化されている
    *
    * @post
    * - キューが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }

    /**
    * @brief コマンドを提出します。
    *
    * @param[in] pCommandBuffer コマンドバッファーへのポインター
    * @param[in] pFence コマンド終了を同期するためのフェンスへのポインター
    *
    * @pre
    * - キューが初期化されている
    * - pCommandBuffer != NULL
    * - pCommandBuffer が初期化されている
    * - pCommandBuffer が開始されていない
    * - pFence != NULL の場合、pFence が初期化されている
    *
    * @post
    * - コマンドが提出されている
    *
    * @details
    * pFence が NULL の場合は同期フェンスを提出しません。
    */
    void ExecuteCommand( TCommandBuffer< Target >* pCommandBuffer,
        TFence< Target >* pFence ) NN_NOEXCEPT
    {
        return Impl::ExecuteCommand( pCommandBuffer, pFence );
    }

    /**
    * @brief 提出済みのコマンドをフラッシュします。
    *
    * @pre
    * - キューが初期化されている
    *
    * @post
    * - 提出済みのコマンドがフラッシュされている
    */
    void Flush() NN_NOEXCEPT
    {
        return Impl::Flush();
    }

    /**
    * @brief 提出済みのコマンド実行を同期します。
    *
    * @pre
    * - キューが初期化されている
    *
    * @post
    * - 提出済みのすべてのコマンドがデバイス上で実行完了されている
    *
    * @details
    * すべてのコマンドがデバイス上で実行を終了するまで処理を返しません。
    */
    void Sync() NN_NOEXCEPT
    {
        return Impl::Sync();
    }

    /**
    * @brief セマフォをセットするコマンドを提出します。
    *
    * @param[in] pSemaphore 対象のセマフォへのポインター
    *
    * @pre
    * - キューが初期化されている
    * - pSemaphore != NULL
    * - pSemaphore が初期化されている
    *
    * @post
    * - セマフォをセットするコマンドが提出されている
    */
    void SetSemaphore( TSemaphore< Target >* pSemaphore ) NN_NOEXCEPT
    {
        return Impl::SetSemaphore( pSemaphore );
    }

    /**
    * @brief セマフォを同期するコマンドを提出します。
    *
    * @param[in] pSemaphore 対象のキューにセット済みのセマフォへのポインター
    *
    * @pre
    * - キューが初期化されている
    * - pSemaphore != NULL
    * - pSemaphore が初期化されている
    *
    * @post
    * - セマフォを同期するコマンドが提出されている
    */
    void SyncSemaphore( const TSemaphore< Target >* pSemaphore ) NN_NOEXCEPT
    {
        return Impl::SyncSemaphore( pSemaphore );
    }

    /**
    * @brief カラーターゲットをスキャンバッファーにコピーするコマンドを提出します。
    *
    * @deprecated 廃止予定です。nn::gfx::SwapChain::GetCurrentScanBuffer() によりスキャンバッファーを取得し、取得したスキャンバッファーに対して操作を行ってください。
    *
    * @param[in] pSwapChain 対象のスキャンバッファーを参照しているスワップチェーンへのポインター
    * @param[in] pColorTarget 対象のカラーターゲットへのポインター
    *
    * @pre
    * - キューが初期化されている
    * - pSwapChain != NULL
    * - pColorTarget != NULL
    * - pSwapChain が初期化されている
    * - pColorTarget が初期化されている
    *
    * @post
    * - カラーターゲットをスキャンバッファーにコピーするコマンドが提出されている
    */
    void CopyToScanBuffer( TSwapChain< Target >* pSwapChain,
        const TColorTargetView< Target >* pColorTarget ) NN_NOEXCEPT
    {
        return Impl::CopyToScanBuffer( pSwapChain, pColorTarget );
    }

    /**
    * @brief スワップチェーンの内容をディスプレイに描写します。
    *
    * @param[in] pSwapChain 対象のスワップチェーンへのポインター
    * @param[in] presentInterval 画面の垂直同期を同期する数
    *
    * @pre
    * - キューが初期化されている
    * - pSwapChain != NULL
    * - pSwapChain が初期化されている
    *
    * @post
    * - ディスプレイに描写するコマンドが提出されている
    *
    * @details
    * presentInterval に 0 を指定した場合はディスプレイの垂直同期を同期しません。@n
    * 低レベルグラフィックス API などに依存してこの呼び出しは垂直同期をとるまで処理を返さない場合があります。
    * CPU 上で同期することを確実にしたい場合はこの後に Sync を呼び出してください。
    */
    void Present( TSwapChain< Target >* pSwapChain, int presentInterval ) NN_NOEXCEPT
    {
        return Impl::Present( pSwapChain, presentInterval );
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
