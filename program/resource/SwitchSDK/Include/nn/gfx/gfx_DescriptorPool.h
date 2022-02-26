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
* @brief デスクリプタープールに関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_Common.h>

#include <nn/gfx/gfx_DescriptorPoolInfo.h>
#include <nn/gfx/gfx_DescriptorSlot.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_RequiredMemory.h>
#include <nn/gfx/detail/gfx_UserPtr.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_DescriptorPool-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_DescriptorPool-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_DescriptorPool-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_DescriptorPool-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_DescriptorPool-api.vk.1.h>
#endif

namespace nn {
namespace gfx {

/**
* @brief デスクリプタープールを表すグラフィックス抽象レイヤーです。
*/
template< typename TTarget >
class TDescriptorPool
    : public detail::DescriptorPoolImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TDescriptorPool );

    typedef detail::DescriptorPoolImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief デスクリプタープールを初期化するための情報クラスです。
    */
    typedef DescriptorPoolInfo InfoType;

    /**
    * @brief デスクリプタープールのサイズを計算します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return デスクリプタープールのサイズを返します。
    */
    static size_t CalculateDescriptorPoolSize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::CalculateDescriptorPoolSize( pDevice, info );
    }

    /**
    * @brief デスクリプタープールに要求されるアライメントを取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return デスクリプタープールに要求されるアライメントを返します。
    */
    static size_t GetDescriptorPoolAlignment( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::GetDescriptorPoolAlignment( pDevice, info );
    }

    /**
    * @brief デスクリプタースロットをひとつ進めるのに必要なサイズを計算します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] type デスクリプタープールの種類
    *
    * @return デスクリプタースロットをひとつ進めるのに必要なバイトでのサイズを返します。
    */
    static ptrdiff_t GetDescriptorSlotIncrementSize( TDevice< Target >* pDevice, DescriptorPoolType type ) NN_NOEXCEPT
    {
        return Impl::GetDescriptorSlotIncrementSize( pDevice, type );
    }

    /**
    * @brief コンストラクターです。
    */
    TDescriptorPool() NN_NOEXCEPT
    {
    }

    /**
    * @brief デスクリプタープールを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    * @param[in] pMemoryPool デスクリプタープールを配置するメモリープールへのポインター
    * @param[in] memoryPoolOffset バイトでのデスクリプタープールを配置するメモリープールへのオフセット
    * @param[in] memoryPoolSize バイトでのメモリープールのサイズ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - デスクリプタープールが初期化されていない
    * - pMemoryPool != NULL
    * - memoryPoolSize が CalculateDescriptorPoolSize で得られるサイズ以上
    *
    * @post
    * - デスクリプタープールが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info,
        TMemoryPool< Target >* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info, pMemoryPool, memoryPoolOffset, memoryPoolSize );
    }

    /**
    * @brief デスクリプタープールを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - デスクリプタープールが初期化されている
    * - デスクリプタープールの更新が開始されていない
    *
    * @post
    * - デスクリプタープールが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }

    /**
    * @brief デスクリプタープールの更新を開始します。
    *
    * @pre
    * - デスクリプタープールが初期化されている
    * - デスクリプタープールの更新が開始されていない
    *
    * @post
    * - デスクリプタープールの更新が開始されている
    */
    void BeginUpdate() NN_NOEXCEPT
    {
        return Impl::BeginUpdate();
    }

    /**
    * @brief デスクリプタープールの更新を終了します。
    *
    * @pre
    * - デスクリプタープールの更新が開始されている
    *
    * @post
    * - デスクリプタープールの更新が開始されていない
    */
    void EndUpdate() NN_NOEXCEPT
    {
        return Impl::EndUpdate();
    }

    /**
    * @brief 指定されるスロットにバッファービューを設定します。
    *
    * @param[in] indexSlot 対象のスロット番号
    * @param[in] gpuAddress 対象のバッファーの GPU アドレス
    * @param[in] size 設定するバッファーのサイズ
    *
    * @pre
    * - indexSlot の値がデスクリプタープールのスロット数よりも小さい
    * - デスクリプタープールの更新が開始されている
    * - デスクリプタープールの種類がバッファーである
    *
    * @post
    * - 指定されたスロットにバッファービューが設定されている
    */
    void SetBufferView( int indexSlot, const GpuAddress& gpuAddress, size_t size ) NN_NOEXCEPT
    {
        return Impl::SetBufferView( indexSlot, gpuAddress, size );
    }

    /**
    * @brief 指定されるスロットにサンプラーを設定します。
    *
    * @param[in] indexSlot 対象のスロット番号
    * @param[in] pSampler 対象のサンプラー
    *
    * @pre
    * - indexSlot の値がデスクリプタープールのスロット数よりも小さい
    * - pSampler != NULL
    * - pSampler が初期化されている
    * - デスクリプタープールの更新が開始されている
    * - デスクリプタープールの種類がサンプラーである
    *
    * @post
    * - 指定されたスロットにサンプラーが設定されている
    */
    void SetSampler( int indexSlot, const TSampler< Target >* pSampler ) NN_NOEXCEPT
    {
        return Impl::SetSampler( indexSlot, pSampler );
    }

    /**
    * @brief 指定されるスロットにテクスチャービューを設定します。
    *
    * @param[in] indexSlot 対象のスロット番号
    * @param[in] pTextureView 対象のテクスチャービュー
    *
    * @pre
    * - indexSlot の値がデスクリプタープールのスロット数よりも小さい
    * - pTextureView != NULL
    * - pTextureView が初期化されている
    * - デスクリプタープールの更新が開始されている
    * - デスクリプタープールの種類がテクスチャーである
    *
    * @post
    * - 指定されたスロットにテクスチャービューが設定されている
    */
    void SetTextureView( int indexSlot, const TTextureView< Target >* pTextureView ) NN_NOEXCEPT
    {
        return Impl::SetTextureView( indexSlot, pTextureView );
    }

    /**
    * @brief 指定されるスロットにイメージを設定します。
    *
    * @param[in] indexSlot 対象のスロット番号
    * @param[in] pImage 対象のイメージ
    *
    * @pre
    * - indexSlot の値がデスクリプタープールのスロット数よりも小さい
    * - pImage != NULL
    * - pImage が初期化されている
    * - デスクリプタープールの更新が開始されている
    * - デスクリプタープールの種類がテクスチャーである
    *
    * @post
    * - 指定されたスロットにイメージが設定されている
    */
    void SetImage( int indexSlot, const TTextureView< Target >* pImage ) NN_NOEXCEPT
    {
        return Impl::SetImage( indexSlot, pImage );
    }

    /**
    * @brief 指定されるスロットにバッファーテクスチャービューを設定します。
    *
    * @param[in] indexSlot 対象のスロット番号
    * @param[in] pBufferTextureView 対象のバッファーテクスチャービュー
    *
    * @pre
    * - indexSlot の値がデスクリプタープールのスロット数よりも小さい
    * - pBufferTextureView != NULL
    * - pBufferTextureView が初期化されている
    * - デスクリプタープールの更新が開始されている
    * - デスクリプタープールの種類がテクスチャーである
    *
    * @post
    * - 指定されたスロットにバッファーテクスチャービューが設定されている
    */
    void SetBufferTextureView( int indexSlot,
        const TBufferTextureView< Target >* pBufferTextureView ) NN_NOEXCEPT
    {
        return Impl::SetBufferTextureView( indexSlot, pBufferTextureView );
    }

    /**
    * @brief 指定されるスロットにバッファーイメージを設定します。
    *
    * @param[in] indexSlot 対象のスロット番号
    * @param[in] pBufferTextureView 対象のバッファーテクスチャービュー
    *
    * @pre
    * - indexSlot の値がデスクリプタープールのスロット数よりも小さい
    * - pBufferTextureView != NULL
    * - pBufferTextureView が初期化されている
    * - デスクリプタープールの更新が開始されている
    * - デスクリプタープールの種類がテクスチャーである
    *
    * @post
    * - 指定されたスロットにバッファーイメージが設定されている
    */
    void SetBufferImage( int indexSlot,
        const TBufferTextureView< Target >* pBufferTextureView ) NN_NOEXCEPT
    {
        return Impl::SetBufferImage( indexSlot, pBufferTextureView );
    }

    /**
    * @brief デスクリプタースロットを取得します。
    *
    * @param[out] pOutDescriptorSlot デスクリプタースロットを受け取るためのポインター
    * @param[in] indexSlot 対象のスロット番号
    *
    * @pre
    * - indexSlot の値がデスクリプタープールのスロット数よりも小さい
    * - デスクリプタープールが初期化されている
    */
    void GetDescriptorSlot( DescriptorSlot* pOutDescriptorSlot, int indexSlot ) const NN_NOEXCEPT
    {
        return Impl::GetDescriptorSlot( pOutDescriptorSlot, indexSlot );
    }

    /**
    * @brief デスクリプタースロットのインデックスを取得します。
    *
    * @param[in] descriptorSlot インデックスを問い合わせるデスクリプタースロット
    *
    * @return デスクリプタープールにおける descriptorSlot のインデックスを返します。
    *
    * @pre
    * - descriptorSlot がこのデスクリプタープールから取得されたデスクリプタースロットである
    * - デスクリプタープールが初期化されている
    */
    int GetDescriptorSlotIndex( const DescriptorSlot& descriptorSlot ) const NN_NOEXCEPT
    {
        return Impl::GetDescriptorSlotIndex( descriptorSlot );
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
