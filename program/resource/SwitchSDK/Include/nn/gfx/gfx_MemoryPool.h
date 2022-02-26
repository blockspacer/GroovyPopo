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
* @brief メモリープールに関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_MemoryPoolInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_UserPtr.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_MemoryPool-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_MemoryPool-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_MemoryPool-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_MemoryPool-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_MemoryPool-api.vk.1.h>
#endif

namespace nn {
namespace gfx {

/**
* @brief メモリープールを表すグラフィックス抽象レイヤーです
*/
template< typename TTarget >
class TMemoryPool
    : public detail::MemoryPoolImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TMemoryPool );

    typedef detail::MemoryPoolImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief メモリープールを初期化するための情報クラスです。
    */
    typedef MemoryPoolInfo InfoType;

    /**
    * @brief プール用のメモリーに要求されるアライメントを取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return プール用のメモリーに要求されるアライメントを返します。1 の場合はアライメント要求はありません。
    */
    static size_t GetPoolMemoryAlignment( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::GetPoolMemoryAlignment( pDevice, info );
    }

    /**
    * @brief プール用のメモリーに要求されるアライメントを取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return プール用のメモリーに要求されるアライメントを返します。1 の場合はアライメント要求はありません。
    */
    static size_t GetPoolMemoryAlignment( TDevice< Target >* pDevice, const TMemoryPoolInfo< Target >& info ) NN_NOEXCEPT
    {
        return Impl::GetPoolMemoryAlignment( pDevice, info );
    }

    /**
    * @brief プール用のメモリーに要求されるサイズの粒度を取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return プール用のメモリーに要求されるサイズの粒度を返します。プール用のメモリーのサイズはこの値の倍数である必要があります。
    */
    static size_t GetPoolMemorySizeGranularity( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::GetPoolMemorySizeGranularity( pDevice, info );
    }

    /**
    * @brief プール用のメモリーに要求されるサイズの粒度を取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return プール用のメモリーに要求されるサイズの粒度を返します。プール用のメモリーのサイズはこの値の倍数である必要があります。
    */
    static size_t GetPoolMemorySizeGranularity( TDevice< Target >* pDevice, const TMemoryPoolInfo< Target >& info ) NN_NOEXCEPT
    {
        return Impl::GetPoolMemorySizeGranularity( pDevice, info );
    }

    /**
    * @brief コンストラクターです。
    */
    TMemoryPool() NN_NOEXCEPT
    {
    }

    /**
    * @brief メモリープールを初期化します
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - メモリープールが初期化されていない
    *
    * @post
    * - メモリープールが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief メモリープールを初期化します
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - メモリプールが初期化されていない
    *
    * @post
    * - メモリープールが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const TMemoryPoolInfo< Target >& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief メモリープールを破棄します
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - メモリープールが初期化されている
    *
    * @post
    * - メモリープールが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }

    /**
    * @brief CPU からアクセスするためのメモリープールをマップします。
    *
    * @return
    * マップ済みメモリープールへの先頭ポインターを返します。
    *
    * @pre
    * - メモリープールが初期化されている
    *
    * @post
    * - メモリープールがマップされている
    *
    * @details
    * この関数は CPU のキャッシュについて扱いません。
    * GPU により更新された値を見るためには nn::gfx::MemoryPool::InvalidateMappedRange により CPU キャッシュを無効化してください。
    * GL 版では、メモリープールを書き換えても初期化済みのリソースに対して影響をもちません。
    */
    void* Map() const NN_NOEXCEPT
    {
        return Impl::Map();
    }

    /**
    * @brief CPU からアクセスするためのメモリープールをマップします。
    *
    * @tparam T マップされたポインターを受けとる際の要素の型
    *
    * @return
    * マップ済みメモリープールへの先頭ポインターを返します。
    *
    * @pre
    * - メモリープールが初期化されている
    *
    * @post
    * - メモリープールがマップされている
    *
    * @details
    * この関数は CPU のキャッシュについて扱いません。
    * GPU により更新された値を見るためには nn::gfx::MemoryPool::InvalidateMappedRange により CPU キャッシュを無効化してください。
    * GL 版では、メモリープールを書き換えても初期化済みのリソースに対して影響をもちません。
    */
    template< typename T >
    T* Map() const NN_NOEXCEPT
    {
        return Impl::template Map< T >();
    }

    /**
    * @brief CPU からのアクセスを終えたことを示すためにメモリープールをアンマップします。
    *
    * @pre
    * - メモリープールがマップされている
    *
    * @post
    * - メモリープールがマップされていない
    *
    * @details
    * この関数は CPU のキャッシュについて扱いません。
    * 書き込んだ内容を GPU から参照可能にするためには nn::gfx::MemoryPool::FlushMappedRange により CPU キャッシュをフラッシュしてください。
    */
    void Unmap() const NN_NOEXCEPT
    {
        return Impl::Unmap();
    }

    /**
    * @brief マップ中に書き込んだ CPU キャッシュをメモリーへとフラッシュします。
    *
    * @param[in] offset メモリープールの先頭からフラッシュする範囲の先頭へのバイトでのオフセット
    * @param[in] size フラッシュする範囲のバイトでのサイズ
    *
    * @pre
    * - メモリープールがマップされている
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
    * @param[in] offset メモリープールの先頭から無効化する範囲の先頭へのバイトでのオフセット
    * @param[in] size 無効化する範囲のバイトでのサイズ
    *
    * @pre
    * - メモリープールがマップされている
    *
    * @post
    * - 指定された範囲のキャッシュの内容が無効化されている
    */
    void InvalidateMappedRange( ptrdiff_t offset, size_t size ) const NN_NOEXCEPT
    {
        return Impl::InvalidateMappedRange( offset, size );
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
    * - メモリープールが初期化されている
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
