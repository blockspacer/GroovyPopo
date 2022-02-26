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
* @brief パイプラインに関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_PipelineInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_RequiredMemory.h>
#include <nn/gfx/detail/gfx_UserPtr.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_Pipeline-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_Pipeline-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_Pipeline-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_Pipeline-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_Pipeline-api.vk.1.h>
#endif

namespace nn {
namespace gfx {

/**
* @brief パイプラインを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TPipeline
    : public detail::PipelineImpl< typename detail::TargetVariation< TTarget >::Type >
    , private detail::RequiredMemory< detail::PipelineImpl<
        typename detail::TargetVariation< TTarget >::Type > >
{
    NN_DISALLOW_COPY( TPipeline );

    typedef detail::PipelineImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief グラフィックスパイプラインを初期化するための情報クラスです。
    */
    typedef GraphicsPipelineInfo GraphicsInfoType;

    /**
    * @brief 演算パイプラインを初期化するための情報クラスです。
    */
    typedef ComputePipelineInfo ComputeInfoType;

    /**
    * @brief 要求メモリー情報です。
    */
    enum RequiredMemoryInfo
    {
        RequiredMemoryInfo_Alignment = detail::RequiredMemory< Impl >::MemoryInfoImpl::RequiredMemoryInfo_Alignment, //!< 要求されるアライメントです。1 の場合アライメント要求はありません。
    };

    /**
    * @brief 要求されるメモリーのサイズを取得します。
    *
    * @param[in] info グラフィックスパイプライン初期化用の情報
    *
    * @return バイトでの要求されるメモリーのサイズを返します。
    */
    static size_t GetRequiredMemorySize( const GraphicsInfoType& info ) NN_NOEXCEPT
    {
        return detail::RequiredMemory< Impl >::MemoryFuncImpl::GetRequiredMemorySize( info );
    }

    /**
    * @brief 要求されるメモリーのサイズを取得します。
    *
    * @param[in] info 演算パイプライン初期化用の情報
    *
    * @return バイトでの要求されるメモリーのサイズを返します。
    */
    static size_t GetRequiredMemorySize( const ComputeInfoType& info ) NN_NOEXCEPT
    {
        return detail::RequiredMemory< Impl >::MemoryFuncImpl::GetRequiredMemorySize( info );
    }

    /**
    * @brief コンストラクターです。
    */
    TPipeline() NN_NOEXCEPT
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
    * @brief グラフィックスパイプラインを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - パイプラインが初期化されていない
    * - 同じ info を引数とした GetRequiredMemorySize で返る値が 0 より大きい場合、その値以上のサイズで要求されるメモリーが設定されている
    *
    * @post
    * - パイプラインがグラフィックスパイプラインで初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const GraphicsInfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief 演算パイプラインを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - パイプラインが初期化されていない
    * - 同じ info を引数とした GetRequiredMemorySize で返る値が 0 より大きい場合、その値以上のサイズで要求されるメモリーが設定されている
    *
    * @post
    * - パイプラインが演算パイプラインで初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const ComputeInfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief パイプラインを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - パイプラインが初期化されている
    *
    * @post
    * - パイプラインが初期化されていない
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

}
}
