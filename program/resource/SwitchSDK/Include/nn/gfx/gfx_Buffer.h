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
* @brief バッファーに関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_BufferInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_UserPtr.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_Buffer-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_Buffer-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_Buffer-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_Buffer-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_Buffer-api.vk.1.h>
#endif

namespace nn {
namespace gfx {

class GpuAddress;

/**
* @brief バッファーを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TBuffer
    : public detail::BufferImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TBuffer );

    typedef detail::BufferImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief バッファーを初期化するための情報クラスです。
    */
    typedef BufferInfo InfoType;

    /**
    * @brief 初期化にメモリープールが要求されるかどうかの真偽値です。
    */
    static const bool IsMemoryPoolRequired = Impl::IsMemoryPoolRequired;

    /**
    * @brief バッファーに要求されるアライメントを取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return バッファーに要求されるアライメントを返します。1 の場合はアライメント要求はありません。
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    */
    static size_t GetBufferAlignment( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::GetBufferAlignment( pDevice, info );
    }

    /**
    * @brief バッファーに要求されるアライメントを取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return バッファーに要求されるアライメントを返します。1 の場合はアライメント要求はありません。
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    */
    static size_t GetBufferAlignment( TDevice< Target >* pDevice, const TBufferInfo< Target >& info ) NN_NOEXCEPT
    {
        return Impl::GetBufferAlignment( pDevice, info );
    }

    /**
    * @brief コンストラクターです。
    */
    TBuffer() NN_NOEXCEPT
    {
    }

    /**
    * @brief バッファーを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    * @param[in] pMemoryPool バッファーを配置するメモリープールへのポインター
    * @param[in] memoryPoolOffset バイトでのバッファーを配置するメモリープールへのオフセット
    * @param[in] memoryPoolSize バイトでのバッファーが使用するメモリープールのサイズ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - バッファーが初期化されていない
    * - IsMemoryPoolRequired が true の場合、pMemoryPool != NULL
    * - IsMemoryPoolRequired が true の場合、memoryPoolSize が info に設定されているサイズ以上
    *
    * @post
    * - バッファーが初期化されている
    *
    * @details
    * IsMemoryPoolRequired が false かつ pMemoryPool を指定した場合、指定されたメモリープールの内容を初期値として扱います。
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info,
        TMemoryPool< Target >* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info, pMemoryPool, memoryPoolOffset, memoryPoolSize );
    }

    /**
    * @brief バッファーを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    * @param[in] pMemoryPool バッファを配置するメモリープールへのポインター
    * @param[in] memoryPoolOffset バイトでのバッファーを配置するメモリープールへのオフセット
    * @param[in] memoryPoolSize バイトでのバッファーが使用するメモリープールのサイズ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - バッファーが初期化されていない
    * - IsMemoryPoolRequired が true の場合、pMemoryPool != NULL
    * - IsMemoryPoolRequired が true の場合、memoryPoolSize が info に設定されているサイズ以上
    *
    * @post
    * - バッファーが初期化されている
    *
    * @details
    * IsMemoryPoolRequired が false かつ pMemoryPool を指定した場合、指定されたメモリープールの内容を初期値として扱います。
    */
    void Initialize( TDevice< Target >* pDevice, const TBufferInfo< Target >& info,
        TMemoryPool< Target >* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info, pMemoryPool, memoryPoolOffset, memoryPoolSize );
    }

    /**
    * @brief バッファーを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - バッファーが初期化されている
    * - バッファーがマップされていない
    *
    * @post
    * - バッファーが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }

    /**
    * @brief CPU からアクセスするためにバッファーをマップします。
    *
    * @return
    * マップ済みバッファーへの先頭ポインターを返します。
    *
    * @pre
    * - バッファーが初期化されている
    *
    * @post
    * - バッファーがマップされている
    *
    * @details
    * この関数は CPU のキャッシュについて扱いません。
    * GPU により更新された値を見るためには nn::gfx::Buffer::InvalidateMappedRange により CPU キャッシュを無効化してください。
    */
    void* Map() const NN_NOEXCEPT
    {
        return Impl::Map();
    }

    /**
    * @brief CPU からアクセスするためにバッファーをマップします。
    *
    * @tparam T マップされたポインターを受けとる際の要素の型
    *
    * @return
    * マップ済みバッファーへの先頭ポインターを返します。
    *
    * @pre
    * - バッファーが初期化されている
    *
    * @post
    * - バッファーがマップされている
    *
    * @details
    * この関数は CPU のキャッシュについて扱いません。
    * GPU により更新された値を見るためには nn::gfx::Buffer::InvalidateMappedRange により CPU キャッシュを無効化してください。
    */
    template< typename T >
    T* Map() const NN_NOEXCEPT
    {
        return Impl::template Map< T >();
    }

    /**
    * @brief CPU からのアクセスを終えたことを示すためにバッファーをアンマップします。
    *
    * @pre
    * - バッファーがマップされている
    *
    * @post
    * - バッファーがマップされていない
    *
    * @details
    * この関数は CPU のキャッシュについて扱いません。
    * 書き込んだ内容を GPU から参照可能にするためには nn::gfx::Buffer::FlushMappedRange により CPU キャッシュをフラッシュしてください。
    */
    void Unmap() const NN_NOEXCEPT
    {
        return Impl::Unmap();
    }

    /**
    * @brief マップ中に書き込んだ CPU キャッシュをメモリーへとフラッシュします。
    *
    * @param[in] offset バッファーの先頭からフラッシュする範囲の先頭へのバイトでのオフセット
    * @param[in] size フラッシュする範囲のバイトでのサイズ
    *
    * @pre
    * - バッファーがマップされている
    *
    * @post
    * - 指定された範囲のキャッシュの内容がフラッシュされている
    */
    void FlushMappedRange( ptrdiff_t offset, size_t size ) const NN_NOEXCEPT
    {
        return Impl::FlushMappedRange( offset, size );
    }

    /**
    * @brief マップ中に最新の値を参照するために CPU キャッシュを無効化します。
    *
    * @param[in] offset バッファーの先頭から無効化する範囲の先頭へのバイトでのオフセット
    * @param[in] size 無効化する範囲のバイトでのサイズ
    *
    * @pre
    * - バッファーがマップされている
    *
    * @post
    * - 指定された範囲のキャッシュの内容が無効化されている
    */
    void InvalidateMappedRange( ptrdiff_t offset, size_t size ) const NN_NOEXCEPT
    {
        return Impl::InvalidateMappedRange( offset, size );
    }

    /**
    * @brief GPU アクセスに使うバッファーの先頭アドレスを取得します。
    *
    * @param[out] pOutGpuAddress GPU アドレスを格納するためのポインター
    *
    * @pre
    * - バッファーが初期化されている
    */
    void GetGpuAddress( GpuAddress* pOutGpuAddress ) const NN_NOEXCEPT
    {
        return Impl::GetGpuAddress( pOutGpuAddress );
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
    * - バッファーが初期化されている
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

/**
* @brief バッファーテクスチャービューを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TBufferTextureView
    : public detail::BufferTextureViewImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TBufferTextureView );

    typedef detail::BufferTextureViewImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief バッファーテクスチャービューを初期化するための情報クラスです。
    */
    typedef BufferTextureViewInfo InfoType;

    /**
    * @brief ビューのオフセットに要求されるアライメントを取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return ビューのオフセットに要求されるバイトでのアライメントを返します。1 の場合はアライメント要求はありません。
    *
    * @pre
    * - pDevice != NULLL
    * - pDevice が初期化されている
    */
    static size_t GetOffsetAlignment( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::GetOffsetAlignment( pDevice, info );
    }

    /**
    * @brief コンストラクターです。
    */
    TBufferTextureView() NN_NOEXCEPT
    {
    }

    /**
    * @brief バッファーテクスチャービューを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - バッファーテクスチャービューが初期化されていない
    *
    * @post
    * - バッファーテクスチャービューが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief バッファーテクスチャービューを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - バッファーテクスチャービューが初期化されている
    *
    * @post
    * - バッファーテクスチャービューが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }
};

}
}
