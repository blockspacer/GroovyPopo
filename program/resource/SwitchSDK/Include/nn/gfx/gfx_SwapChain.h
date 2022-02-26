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
* @brief スワップチェーンに関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_SwapChainInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_UserPtr.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_SwapChain-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_SwapChain-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_SwapChain-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_SwapChain-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_SwapChain-api.vk.1.h>
#endif

namespace nn {
namespace gfx {

/**
* @brief スワップチェーンを表すグラフィックス抽象レイヤです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TSwapChain
    : public detail::SwapChainImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TSwapChain );

    typedef detail::SwapChainImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief スワップチェーンを初期化するための情報クラスです。
    */
    typedef SwapChainInfo InfoType;

    /**
    * @brief 初期化にメモリープールが要求されるかどうかの真偽値です。
    */
    static const bool IsMemoryPoolRequired = Impl::IsMemoryPoolRequired;

    /**
    * @brief スキャバッファーに要求されるアライメントを取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return スキャバッファーに要求されるアライメントを返します。1 の場合はアライメント要求はありません。
    */
    static size_t GetScanBufferAlignment( TDevice< Target >* pDevice, const InfoType& info )
    {
        return Impl::GetScanBufferAlignment( pDevice, info );
    }

    /**
    * @brief スキャンバッファーのサイズを計算します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return スキャンバッファーのサイズを返します。
    */
    static size_t CalculateScanBufferSize( TDevice< Target >* pDevice, const InfoType& info )
    {
        return Impl::CalculateScanBufferSize( pDevice, info );
    }

    /**
    * @brief コンストラクターです。
    */
    TSwapChain() NN_NOEXCEPT
    {
    }

    /**
    * @brief スワップチェーンを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    * @param[in] pMemoryPool スキャンバッファーを配置するメモリープールへのポインター
    * @param[in] memoryPoolOffset バイトでのスキャンバッファーを配置するメモリープールへのオフセット
    * @param[in] memoryPoolSize バイトでのスキャンバッファーが使用するメモリープールのサイズ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - スワップチェーンが初期化されていない
    * - IsMemoryPoolRequired が true の場合、pMemoryPool != NULL
    * - IsMemoryPoolRequired が true の場合、memoryPoolSize が CalculateScanBufferSize で取得されるサイズ以上
    *
    * @post
    * - スワップチェーンが初期化されている
    *
    * @details
    * 渡すメモリープールは圧縮可能として作成されている必要があります。
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info,
        TMemoryPool< Target >* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info, pMemoryPool, memoryPoolOffset, memoryPoolSize );
    }

    /**
    * @brief スワップチェーンを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - スワップチェーンが初期化されている
    *
    * @post
    * - スワップチェーンが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }

    /**
    * @brief スキャンバッファービューを取得します。
    *
    * @param[in] ppOutScanBufferArray スキャンバッファービューのポインターを受けとるための配列へのポインター
    * @param[in] maxScanBufferCount 受けとる最大のスキャンバッファービューのポインターの数
    *
    * @return ppOutScanBufferArray が NULL のときはスワップチェーンが所持しているスキャンバッファーの数を、そうでなければ取得されたスキャンバッファービューの数を返します。
    *
    * @pre
    * - スワップチェーンが初期化されている
    * - ppOutScanBufferArray で指定される配列の要素数が maxScanBufferCount 以上
    */
    int GetScanBufferViews( TColorTargetView< Target >** ppOutScanBufferArray,
        int maxScanBufferCount ) NN_NOEXCEPT
    {
        return Impl::GetScanBufferViews( ppOutScanBufferArray, maxScanBufferCount );
    }

    /**
    * @brief スキャンバッファーを取得します。
    *
    * @param[in] ppOutScanBufferTextureArray スキャンバッファーのポインターを受けとるための配列へのポインター
    * @param[in] maxScanBufferTextureCount 受けとる最大のスキャンバッファーのポインターの数
    *
    * @return ppOutScanBufferTextureArray が NULL のときはスワップチェーンが所持しているスキャンバッファーの数を、そうでなければ取得されたスキャンバッファーの数を返します。
    *
    * @pre
    * - スワップチェーンが初期化されている
    * - ppOutScanBufferTextureArray で指定される配列の要素数が maxScanBufferTextureCount 以上
    */
    int GetScanBuffers( TTexture< Target >** ppOutScanBufferTextureArray,
        int maxScanBufferTextureCount ) NN_NOEXCEPT
    {
        return Impl::GetScanBuffers( ppOutScanBufferTextureArray, maxScanBufferTextureCount );
    }

    /**
    * @brief 次の Present 対象のスキャンバッファーのインデックスを取得します。
    *
    * @param[out] pOutScanBufferIndex 次の Present 対象のスキャンバッファーのインデックスを受けとるためのポインター
    * @param[in] pSemaphore 対象のスキャンバッファーが使用可能になる際にシグナル状態になるセマフォへのポインター
    * @param[in] pFence 対象のスキャンバッファーが使用可能になる際にシグナルに状態なるフェンスへのポインター
    *
    * @return スキャンバッファーの取得処理の結果を返します。
    * @retval AcquireScanBufferResult_Success スキャンバッファーの取得に成功しました。
    * @retval AcquireScanBufferResult_Failed スキャンバッファーの取得に失敗しました。
    *
    * @pre
    * - スワップチェーンが初期化されている
    * - pOutScanBufferIndex != NULL
    * - pSemaphore != NULL
    * - pSemaphore が初期化されている
    * - pSemaphore がシグナル状態でない
    * - pFence != NULL の場合、pFence が初期化されている
    * - pFence != NULL の場合、pFence がシグナル状態でない
    *
    * @details
    * GL 版では pSemaphore と pFence に対する Sync は効果をもちません。
    */
    AcquireScanBufferResult AcquireNextScanBufferIndex( int* pOutScanBufferIndex,
        TSemaphore< Target >* pSemaphore, TFence< Target >* pFence ) NN_NOEXCEPT
    {
        return Impl::AcquireNextScanBufferIndex( pOutScanBufferIndex, pSemaphore, pFence );
    }

    /**
    * @brief 次の Present 対象のスキャンバッファーのインデックスを取得します。
    *
    * @deprecated 廃止予定です。3 引数版の AcquireNextScanBufferIndex を使用してください。また、新しい関数と併用することはできません。
    *
    * @return 次の Present 対象のスキャンバッファーのインデックスを返します。
    *
    * @pre
    * - スワップチェーンが初期化されている
    */
    int AcquireNextScanBufferIndex() NN_NOEXCEPT
    {
        return Impl::AcquireNextScanBufferIndex();
    }

    /**
    * @brief 次の Present 対象のスキャンバッファービューを取得します。
    *
    * @deprecated 廃止予定です。3 引数版の AcquireNextScanBufferIndex を使用してください。また、新しい関数と併用することはできません。
    *
    * @return 次の Present 対象のスキャンバッファービューを返します。
    *
    * @pre
    * - スワップチェーンが初期化されている
    *
    * @details
    * nn::gfx::Queue::CopyToScanBuffer と併用する場合、nn::gfx::Queue::CopyToScanBuffer よりも前に呼び出す必要があります。
    */
    TColorTargetView< Target >* AcquireNextScanBufferView() NN_NOEXCEPT
    {
        return static_cast< TColorTargetView< Target >* >( Impl::AcquireNextScanBufferView() );
    }

    /**
    * @brief 次の Present 対象のスキャンバッファーを取得します。
    *
    * @deprecated 廃止予定です。3 引数版の AcquireNextScanBufferIndex を使用してください。また、新しい関数と併用することはできません。
    *
    * @return 次の Present 対象のスキャンバッファーを返します。
    *
    * @pre
    * - スワップチェーンが初期化されている
    */
    TTexture< Target >* AcquireNextScanBuffer() NN_NOEXCEPT
    {
        return static_cast< TTexture< Target >* >( Impl::AcquireNextScanBuffer() );
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
