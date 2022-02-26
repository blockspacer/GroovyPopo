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
* @brief ルートシグネチャーに関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_RootSignatureInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_RequiredMemory.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_RootSignature-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_RootSignature-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_RootSignature-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_RootSignature-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_RootSignature-api.vk.1.h>
#endif

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS

namespace nn {
namespace gfx {

/**
* @brief ルートシグネチャーを表すグラフィックス抽象レイヤーです。
*
* @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class NN_DEPRECATED TRootSignature
    : public detail::RootSignatureImpl< typename detail::TargetVariation< TTarget >::Type >
    , private detail::RequiredMemory< detail::RootSignatureImpl<
        typename detail::TargetVariation< TTarget >::Type > >
{
    NN_DISALLOW_COPY( TRootSignature );

    typedef detail::RootSignatureImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief ルートシグネチャーを初期化するための情報クラスです。
    */
    typedef RootSignatureInfo InfoType;

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
    TRootSignature() NN_NOEXCEPT
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
    * @brief ルートシグネチャーを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ルートシグネチャーが初期化されていない
    *
    * @post
    * - ルートシグネチャーが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief ルートシグネチャーを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ルートシグネチャーが初期化されている
    *
    * @post
    * - ルートシグネチャーが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }
};

}
}

NN_PRAGMA_POP_WARNINGS
