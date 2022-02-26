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
* @brief ステートに関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_StateInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_RequiredMemory.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_State-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_State-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_State-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_State-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_State-api.vk.1.h>
#endif

namespace nn {
namespace gfx {

/**
* @brief ラスタライザーステートを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TRasterizerState
    : public detail::RasterizerStateImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TRasterizerState );

    typedef detail::RasterizerStateImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief ラスタライザーステートを初期化するための情報クラスです。
    */
    typedef RasterizerStateInfo InfoType;

    /**
    * @brief コンストラクターです。
    */
    TRasterizerState() NN_NOEXCEPT
    {
    }

    /**
    * @brief ラスタライザーステートを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ラスタライザーステートが初期化されていない
    *
    * @post
    * - ラスタライザーステートが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief ラスタライザーステートを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ラスタライザーステートが初期化されている
    *
    * @post
    * - ラスタライザーステートが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }
};

/**
* @brief ブレンドステートを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TBlendState
    : public detail::BlendStateImpl< typename detail::TargetVariation< TTarget >::Type >
    , private detail::RequiredMemory< detail::BlendStateImpl<
        typename detail::TargetVariation< TTarget >::Type > >
{
    NN_DISALLOW_COPY( TBlendState );

    typedef detail::BlendStateImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief ブレンドステートを初期化するための情報クラスです。
    */
    typedef BlendStateInfo InfoType;

    /**
    * @brief 要求メモリー情報です。
    */
    enum RequiredMemoryInfo
    {
        RequiredMemoryInfo_Alignment = detail::RequiredMemory< Impl >::MemoryInfoImpl::RequiredMemoryInfo_Alignment //!< 要求されるアライメントです。1 の場合アライメント要求はありません。
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
    TBlendState() NN_NOEXCEPT
    {
    }

    /**
    * @brief 要求されるメモリーを設定します。
    *
    * @param[in] pMemory 設定するメモリーへのポインター
    * @param[in] size バイトでの設定するメモリーのサイズ
    *
    * @pre
    * - pMemory が NULL でない場合は、pMemory が RequiredMemoryInfo_Alignment でアライメントされている
    * - pMemory が NULL の場合は、size == 0
    *
    * @post
    * - pMemory が NULL でない場合は、要求されるメモリーが設定されている
    * - pMemory が NULL の場合は、要求されるメモリーが設定されていない
    */
    void SetMemory( void* pMemory, size_t size ) NN_NOEXCEPT
    {
        return detail::RequiredMemory< Impl >::MemoryFuncImpl::SetMemory( pMemory, size );
    }

    /**
    * @brief 設定した要求されるメモリーを取得します。
    *
    * @return 要求されるメモリーが設定されている場合はそのメモリーへのポインターを、設定されていない場合は NULL を返します。
    */
    void* GetMemory() NN_NOEXCEPT
    {
        return detail::RequiredMemory< Impl >::MemoryFuncImpl::GetMemory();
    }

    /**
    * @brief 設定した要求されるメモリーを取得します。
    *
    * @return 要求されるメモリーが設定されている場合はそのメモリーへのポインターを、設定されていない場合は NULL を返します。
    */
    const void* GetMemory() const NN_NOEXCEPT
    {
        return detail::RequiredMemory< Impl >::MemoryFuncImpl::GetMemory();
    }

    /**
    * @brief ブレンドステートを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ブレンドステートが初期化されていない
    * - 同じ info を引数とした GetRequiredMemorySize で返る値が 0 より大きい場合、その値以上のサイズで要求されるメモリーが設定されている
    *
    * @post
    * - ブレンドステートが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief ブレンドステートを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ブレンドステートが初期化されている
    *
    * @post
    * - ブレンドステートが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }
};

/**
* @brief 深度ステンシルステートを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TDepthStencilState
    : public detail::DepthStencilStateImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TDepthStencilState );

    typedef detail::DepthStencilStateImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief 深度ステンシルステートを初期化するための情報クラスです。
    */
    typedef DepthStencilStateInfo InfoType;

    /**
    * @brief コンストラクターです。
    */
    TDepthStencilState() NN_NOEXCEPT
    {
    }

    /**
    * @brief 深度ステンシルステートを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - 深度ステンシルステートが初期化されていない
    *
    * @post
    * - 深度ステンシルステートが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief 深度ステンシルステートを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - 深度ステンシルステートが初期化されている
    *
    * @post
    * - 深度ステンシルステートが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }
};

/**
* @brief 頂点ステートを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TVertexState
    : public detail::VertexStateImpl< typename detail::TargetVariation< TTarget >::Type >
    , private detail::RequiredMemory< detail::VertexStateImpl<
        typename detail::TargetVariation< TTarget >::Type > >
{
    NN_DISALLOW_COPY( TVertexState );

    typedef detail::VertexStateImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief 頂点ステートを初期化するための情報クラスです。
    */
    typedef VertexStateInfo InfoType;

    /**
    * @brief 要求メモリー情報です。
    */
    enum RequiredMemoryInfo
    {
        RequiredMemoryInfo_Alignment = detail::RequiredMemory< Impl >::MemoryInfoImpl::RequiredMemoryInfo_Alignment //!< 要求されるアライメントです。1 の場合アライメント要求はありません。
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
    TVertexState() NN_NOEXCEPT
    {
    }

    /**
    * @brief 要求されるメモリーを設定します。
    *
    * @param[in] pMemory 設定するメモリーへのポインター
    * @param[in] size バイトでの設定するメモリーのサイズ
    *
    * @pre
    * - pMemory が NULL でない場合は、pMemory が RequiredMemoryInfo_Alignment でアライメントされている
    * - pMemory が NULL の場合は、size == 0
    *
    * @post
    * - pMemory が NULL でない場合は、要求されるメモリーが設定されている
    * - pMemory が NULL の場合は、要求されるメモリーが設定されていない
    */
    void SetMemory( void* pMemory, size_t size ) NN_NOEXCEPT
    {
        return detail::RequiredMemory< Impl >::MemoryFuncImpl::SetMemory( pMemory, size );
    }

    /**
    * @brief 設定した要求されるメモリーを取得します。
    *
    * @return 要求されるメモリーが設定されている場合はそのメモリーへのポインターを、設定されていない場合は NULL を返します。
    */
    void* GetMemory() NN_NOEXCEPT
    {
        return detail::RequiredMemory< Impl >::MemoryFuncImpl::GetMemory();
    }

    /**
    * @brief 設定した要求されるメモリーを取得します。
    *
    * @return 要求されるメモリーが設定されている場合はそのメモリーへのポインターを、設定されていない場合は NULL を返します。
    */
    const void* GetMemory() const NN_NOEXCEPT
    {
        return detail::RequiredMemory< Impl >::MemoryFuncImpl::GetMemory();
    }

    /**
    * @brief 頂点ステートを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    * @param[in] pVertexShader 頂点ステートが対象とする頂点シェーダーへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - pVertexShader が NULL でない場合、pVertexShader が初期化されている
    * - 頂点ステートが初期化されていない
    * - 同じ info を引数とした GetRequiredMemorySize で返る値が 0 より大きい場合、その値以上のサイズで要求されるメモリーが設定されている
    *
    * @post
    * - 頂点ステートが初期化されている
    *
    * @details
    * pVertexShader が NULL でない場合は名前による結びつけを行います
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info,
        const TShader< Target >* pVertexShader = NULL ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info, pVertexShader );
    }

    /**
    * @brief 頂点ステートを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - 頂点ステートが初期化されている
    *
    * @post
    * - 頂点ステートが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }
};

/**
* @brief テッセレーションステートを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TTessellationState
    : public detail::TessellationStateImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TTessellationState );

    typedef detail::TessellationStateImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief テッセレーションステートを初期化するための情報クラスです。
    */
    typedef TessellationStateInfo InfoType;

    /**
    * @brief コンストラクターです。
    */
    TTessellationState() NN_NOEXCEPT
    {
    }

    /**
    * @brief テッセレーションステートを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - テッセレーションステートが初期化されていない
    *
    * @post
    * - テッセレーションステートが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief テッセレーションステートを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - テッセレーションステートが初期化されている
    *
    * @post
    * - テッセレーションステートが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }
};

/**
* @brief ビューポートシザーステートを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TViewportScissorState
    : public detail::ViewportScissorStateImpl< typename detail::TargetVariation< TTarget >::Type >
    , private detail::RequiredMemory< detail::ViewportScissorStateImpl<
        typename detail::TargetVariation< TTarget >::Type > >
{
    NN_DISALLOW_COPY( TViewportScissorState );

    typedef detail::ViewportScissorStateImpl<
        typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief ビューポートシザーステートを初期化するための情報クラスです。
    */
    typedef ViewportScissorStateInfo InfoType;

    /**
    * @brief 要求メモリー情報です。
    */
    enum RequiredMemoryInfo
    {
        RequiredMemoryInfo_Alignment = detail::RequiredMemory< Impl >::MemoryInfoImpl::RequiredMemoryInfo_Alignment //!< 要求されるアライメントです。1 の場合アライメント要求はありません。
    };

    /**
    * @brief 要求されるメモリーのサイズを取得します。
    *
    * @param[in] info 初期化用の情報
    *
    * @return バイトでの要求されるメモリーのサイズを返します。
    *
    * @details
    * ビューポートの数がひとつの場合は必ず要求されるメモリーはなく、0 を返します。
    */
    static size_t GetRequiredMemorySize( const InfoType& info ) NN_NOEXCEPT
    {
        return detail::RequiredMemory< Impl >::MemoryFuncImpl::GetRequiredMemorySize( info );
    }

    /**
    * @brief コンストラクターです。
    */
    TViewportScissorState() NN_NOEXCEPT
    {
    }

    /**
    * @brief 要求されるメモリーを設定します。
    *
    * @param[in] pMemory 設定するメモリーへのポインター
    * @param[in] size バイトでの設定するメモリーのサイズ
    *
    * @pre
    * - pMemory が NULL でない場合は、pMemory が RequiredMemoryInfo_Alignment でアライメントされている
    * - pMemory が NULL の場合は、size == 0
    *
    * @post
    * - pMemory が NULL でない場合は、要求されるメモリーが設定されている
    * - pMemory が NULL の場合は、要求されるメモリーが設定されていない
    */
    void SetMemory( void* pMemory, size_t size ) NN_NOEXCEPT
    {
        return detail::RequiredMemory< Impl >::MemoryFuncImpl::SetMemory( pMemory, size );
    }

    /**
    * @brief 設定した要求されるメモリーを取得します。
    *
    * @return 要求されるメモリーが設定されている場合はそのメモリーへのポインターを、設定されていない場合は NULL を返します。
    */
    void* GetMemory() NN_NOEXCEPT
    {
        return detail::RequiredMemory< Impl >::MemoryFuncImpl::GetMemory();
    }

    /**
    * @brief 設定した要求されるメモリーを取得します。
    *
    * @return 要求されるメモリーが設定されている場合はそのメモリーへのポインターを、設定されていない場合は NULL を返します。
    */
    const void* GetMemory() const NN_NOEXCEPT
    {
        return detail::RequiredMemory< Impl >::MemoryFuncImpl::GetMemory();
    }

    /**
    * @brief ビューポートシザーステートを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ビューポートシザーステートが初期化されていない
    * - 同じ info を引数とした GetRequiredMemorySize で返る値が 0 より大きい場合、その値以上のサイズで要求されるメモリーが設定されている
    *
    * @post
    * - ビューポートシザーステートが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief ビューポートシザーステートを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ビューポートシザーステートが初期化されている
    *
    * @post
    * - ビューポートシザーステートが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }
};

}
}
