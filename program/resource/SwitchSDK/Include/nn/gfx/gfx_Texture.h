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
* @brief テクスチャーに関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_TextureInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_UserPtr.h>
#include <nn/gfx/detail/gfx_Misc.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_Texture-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_Texture-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_Texture-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_Texture-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_Texture-api.vk.1.h>
#endif

namespace nn {
namespace gfx {

/**
* @brief テクスチャーを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TTexture
    : public detail::TextureImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TTexture );

    typedef detail::TextureImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief テクスチャーを初期化するための情報クラスです。
    */
    typedef TextureInfo InfoType;

    /**
    * @brief 初期化にメモリープールが要求されるかどうかの真偽値です
    */
    static const bool IsMemoryPoolRequired = Impl::IsMemoryPoolRequired;

    /**
    * @brief ミップマップデータのベースレベルのデータポインターに要求されるアライメントを計算します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return ミップマップデータの最初のデータに要求されるアライメントを返します。
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    *
    * @details
    * 1 の返り値はアライメント要求がないことを表します。
    */
    static size_t CalculateMipDataAlignment( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::CalculateMipDataAlignment( pDevice, info );
    }

    /**
    * @brief ミップマップデータのベースレベルのデータポインターに要求されるアライメントを計算します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return ミップマップデータの最初のデータに要求されるアライメントを返します。
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    *
    * @details
    * 1 の返り値はアライメント要求がないことを表します。
    */
    static size_t CalculateMipDataAlignment( TDevice< Target >* pDevice, const TTextureInfo< Target >& info ) NN_NOEXCEPT
    {
        return Impl::CalculateMipDataAlignment( pDevice, info );
    }

    /**
    * @brief ミップマップデータに要求される全体のサイズを計算します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return バイトでのベースレベルも含めたミップマップデータ全体のサイズを返します。
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    */
    static size_t CalculateMipDataSize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::CalculateMipDataSize( pDevice, info );
    }

    /**
    * @brief ミップマップデータに要求される全体のサイズを計算します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return バイトでのベースレベルも含めたミップマップデータ全体のサイズを返します。
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    */
    static size_t CalculateMipDataSize( TDevice< Target >* pDevice, const TTextureInfo< Target >& info ) NN_NOEXCEPT
    {
        return Impl::CalculateMipDataSize( pDevice, info );
    }

    /**
    * @brief 各ミップマップデータについて、ベースレベルのデータポインターからの適切なオフセットを計算します。
    *
    * @param[out] pMipOffsets info.GetMipCount() 要素をもつ、ミップマップのオフセットを格納するための配列へのポインター
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pMipOffsets != NULL
    * - pDevice != NULL
    * - pDevice が初期化されている
    */
    static void CalculateMipDataOffsets( ptrdiff_t* pMipOffsets, TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::CalculateMipDataOffsets( pMipOffsets, pDevice, info );
    }

    /**
    * @brief 各ミップマップデータについて、ベースレベルのデータポインターからの適切なオフセットを計算します。
    *
    * @param[out] pMipOffsets info.GetMipCount() 要素をもつ、ミップマップのオフセットを格納するための配列へのポインター
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pMipOffsets != NULL
    * - pDevice != NULL
    * - pDevice が初期化されている
    */
    static void CalculateMipDataOffsets( ptrdiff_t* pMipOffsets, TDevice< Target >* pDevice, const TTextureInfo< Target >& info ) NN_NOEXCEPT
    {
        return Impl::CalculateMipDataOffsets( pMipOffsets, pDevice, info );
    }

    /**
    * @brief 線形テクスチャーについて行ピッチを取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return バイトでの行ピッチを返します。
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    *
    * @details
    * 行ピッチは、あるピクセルから同じ列の次の行のピクセルまでのバイト数を表します。
    */
    static size_t GetRowPitch( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::GetRowPitch( pDevice, info );
    }

    /**
    * @brief 線形テクスチャーについて行ピッチを取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @return バイトでの行ピッチを返します。
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    *
    * @details
    * 行ピッチは、あるピクセルから同じ列の次の行のピクセルまでのバイト数を表します。
    */
    static size_t GetRowPitch( TDevice< Target >* pDevice, const TTextureInfo< Target >& info ) NN_NOEXCEPT
    {
        return Impl::GetRowPitch( pDevice, info );
    }

    /**
    * @brief コンストラクターです。
    */
    TTexture() NN_NOEXCEPT
    {
    }

    /**
    * @brief テクスチャーを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    * @param[in] pMemoryPool テクスチャーのミップマップデータを配置するメモリープールへのポインター
    * @param[in] memoryPoolOffset バイトでのテクスチャーのミップマップデータを配置するメモリープールへのオフセット
    * @param[in] memoryPoolSize バイトでのテクスチャーが使用するメモリープールのサイズ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - テクスチャーが初期化されていない
    * - IsMemoryPoolRequired が true の場合、pMemoryPool != NULL
    * - IsMemoryPoolRequired が true の場合、memoryPoolSize が CalculateMipDataSize で取得されるサイズ以上
    *
    * @post
    * - テクスチャーが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info,
        TMemoryPool< Target >* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info, pMemoryPool, memoryPoolOffset, memoryPoolSize );
    }

    /**
    * @brief テクスチャーを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    * @param[in] pMemoryPool テクスチャーのミップマップデータを配置するメモリープールへのポインター
    * @param[in] memoryPoolOffset バイトでのテクスチャーのミップマップデータを配置するメモリープールへのオフセット
    * @param[in] memoryPoolSize バイトでのテクスチャーが使用するメモリープールのサイズ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - テクスチャーが初期化されていない
    * - IsMemoryPoolRequired が true の場合、pMemoryPool != NULL
    * - IsMemoryPoolRequired が true の場合、memoryPoolSize が CalculateMipDataSize で取得されるサイズ以上
    *
    * @post
    * - テクスチャーが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const TTextureInfo< Target >& info,
        TMemoryPool< Target >* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info, pMemoryPool, memoryPoolOffset, memoryPoolSize );
    }

    /**
    * @brief テクスチャーを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - テクスチャーが初期化されている
    *
    * @post
    * - テクスチャーが初期化されていない
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
    * - テクスチャーが初期化されている
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
* @brief テクスチャービューを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TTextureView
    : public detail::TextureViewImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TTextureView );

    typedef detail::TextureViewImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief テクスチャービューを初期化するための情報クラスです。
    */
    typedef TextureViewInfo InfoType;

    /**
    * @brief コンストラクターです。
    */
    TTextureView() NN_NOEXCEPT
    {
    }

    /**
    * @brief テクスチャービューを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - テクスチャービューが初期化されていない
    *
    * @post
    * - テクスチャービューが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief テクスチャービューを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - テクスチャービューが初期化されている
    *
    * @post
    * - テクスチャービューが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
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
* @brief カラーターゲットビューを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TColorTargetView
    : public detail::ColorTargetViewImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TColorTargetView );

    typedef detail::ColorTargetViewImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief カラーターゲットビューを初期化するための情報クラスです。
    */
    typedef ColorTargetViewInfo InfoType;

    /**
    * @brief コンストラクターです。
    */
    TColorTargetView() NN_NOEXCEPT
    {
    }

    /**
    * @brief カラーターゲットビューを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - カラーターゲットビューが初期化されていない
    *
    * @post
    * - カラーターゲットビューが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief カラーターゲットビューを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - カラーターゲットビューが初期化されている
    *
    * @post
    * - カラーターゲットビューが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
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
* @brief 深度ステンシルビューを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TDepthStencilView
    : public detail::DepthStencilViewImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TDepthStencilView );

    typedef detail::DepthStencilViewImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief 深度ステンシルビューを初期化するための情報クラスです。
    */
    typedef DepthStencilViewInfo InfoType;

    /**
    * @brief コンストラクターです。
    */
    TDepthStencilView() NN_NOEXCEPT
    {
    }

    /**
    * @brief 深度ステンシルビューを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - 深度ステンシルビューが初期化されていない
    *
    * @post
    * - 深度ステンシルビューが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief 深度ステンシルビューを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - 深度ステンシルビューが初期化されている
    *
    * @post
    * - 深度ステンシルビューが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
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
* @brief イメージフォーマットのプロパティーを取得します。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*
* @param[out] pOutImageFormatProperty 取得したイメージフォーマットのプロパティーを格納する領域へのポインター
* @param[in] pDevice デバイスへのポインター
* @param[in] imageFormat 取得する対象のイメージフォーマット
*
* @pre
* - pOutImageFormatProperty != NULL
* - pDevice != NULL
* - pDevice が初期化されている
*
* @post
* - pOutImageFormatProperty の指す領域に取得されたイメージフォーマットのプロパティーが格納されている
*/
template< typename TTarget >
inline void GetImageFormatProperty( ImageFormatProperty* pOutImageFormatProperty,
    TDevice< TTarget >* pDevice, ImageFormat imageFormat ) NN_NOEXCEPT
{
    return detail::GetImageFormatProperty< typename detail::TargetVariation<
        TTarget >::Type >( pOutImageFormatProperty, pDevice, imageFormat );
}

}
}
