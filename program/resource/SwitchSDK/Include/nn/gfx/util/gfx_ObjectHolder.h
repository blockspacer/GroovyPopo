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
* @brief nn::gfx に用意されるクラスのうち、追加のメモリを要求するものについて、
*   必要なメモリをメンバとして抱えるラッパークラスを定義します。
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/util/util_BytePtr.h>
#include <nn/gfx.h>
#include <nn/gfx/util/detail/gfx_AlignedStorage.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/util/detail/gfx_ObjectHolder-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/util/detail/gfx_ObjectHolder-api.nvn.8.h>
#endif

#if defined ( NN_GFX_CONFIG_INCLUDE_VK1 )
#include <nn/gfx/util/detail/gfx_ObjectHolder-api.vk.1.h>
#endif

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS

namespace nn {
namespace gfx {
namespace util {

/**
* @brief nn::gfx::BlendState に必要な追加メモリをメンバとして確保するラッパークラスです。
*   確保するメモリ量を静的に決定するために必要な値をテンプレート引数として受け取ります。
*
* @tparam TBlendTargetCount ブレンドターゲットの数。 nn::gfx::BlendStateInfo::SetBlendTargetStateInfoArray() に与えるブレンドターゲットステート情報の数と同じかそれ以上の値を指定してください。
*/
template<int TBlendTargetCount>
class BlendStateHolder
{
    NN_DISALLOW_COPY(BlendStateHolder);

public:

    /**
    * @brief コンストラクタです。
    */
    BlendStateHolder() NN_NOEXCEPT
    {
    }

    /**
    * @brief ブレンドステートを初期化します。
    *
    * @param[in] pDevice デバイスへのポインタ
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - info.GetBlendTargetCount() <= TBlendTargetCount
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ブレンドステートが初期化されていない
    *
    * @post
    * - ブレンドステートが初期化されている
    */
    void Initialize(nn::gfx::Device* pDevice, const nn::gfx::BlendState::InfoType& info) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( info.GetBlendTargetCount() <= TBlendTargetCount );

        NN_SDK_ASSERT( nn::gfx::BlendState::GetRequiredMemorySize(info) <= sizeof(m_Memory) );
        NN_SDK_ASSERT( nn::util::BytePtr(&m_Memory).IsAligned(nn::gfx::BlendState::RequiredMemoryInfo_Alignment) );

        m_Object.SetMemory(&m_Memory, nn::gfx::BlendState::GetRequiredMemorySize(info));
        m_Object.Initialize(pDevice, info);
    }

    /**
    * @brief ブレンドステートを破棄します。
    *
    * @param[in] pDevice デバイスへのポインタ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ブレンドステートが初期化されている
    *
    * @post
    * - ブレンドステートが初期化されていない
    */
    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT
    {
        m_Object.Finalize(pDevice);
    }

    /**
    * @brief ブレンドステートのインスタンスを取得します。
    *
    * @return ブレンドステートのインスタンスを返します。
    */
    nn::gfx::BlendState* Get() NN_NOEXCEPT
    {
        return &m_Object;
    }

    /**
    * @brief ブレンドステートのインスタンスを取得します。
    *
    * @return ブレンドステートのインスタンスを返します。
    */
    const nn::gfx::BlendState* Get() const NN_NOEXCEPT
    {
        return &m_Object;
    }

private:
    typename nn::gfx::util::detail::AlignedStorage<
        detail::BlendStateHolderRequiredMemorySize<TBlendTargetCount>::Value,
        nn::gfx::BlendState::RequiredMemoryInfo_Alignment
    >::Type m_Memory;

    nn::gfx::BlendState m_Object;
};

/**
* @brief nn::gfx::VertexState に必要な追加メモリをメンバとして確保するラッパークラスです。
*   確保するメモリ量を静的に決定するために必要な値をテンプレート引数として受け取ります。
*
* @tparam TVertexAttributeCount 頂点属性の数。 nn::gfx::VertexStateInfo::SetVertexAttributeStateInfoArray() に与える頂点属性ステート情報の数と同じかそれ以上の値を指定してください。
* @tparam TVertexBufferCount 頂点バッファの数。 nn::gfx::VertexStateInfo::SetVertexBufferStateInfoArray() に与える頂点バッファステート情報の数と同じかそれ以上の値を指定してください。
*/
template<int TVertexAttributeCount, int TVertexBufferCount>
class VertexStateHolder
{
    NN_DISALLOW_COPY(VertexStateHolder);

public:

    /**
    * @brief コンストラクタです。
    */
    VertexStateHolder() NN_NOEXCEPT
    {
    }

    /**
    * @brief 頂点ステートを初期化します。
    *
    * @param[in] pDevice デバイスへのポインタ
    * @param[in] info 初期化用の情報
    * @param[in] pVertexShader 頂点ステートが対象とする頂点シェーダへのポインタ
    *
    * @pre
    * - info.GetVertexAttributeCount() <= TBlendTargetCount
    * - info.GetVertexBufferCount() <= TBlendTargetCount
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - pVertexShader が NULL でない場合、pVertexShader が初期化されている
    * - 頂点ステートが初期化されていない
    *
    * @post
    * - 頂点ステートが初期化されている
    *
    * @details
    * pVertexShader が NULL でない場合は名前による結びつけを行います
    */
    void Initialize(nn::gfx::Device* pDevice, const nn::gfx::VertexState::InfoType& info, const nn::gfx::Shader* pVertexShader = NULL) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( info.GetVertexAttributeCount() <= TVertexAttributeCount );
        NN_SDK_REQUIRES( info.GetVertexBufferCount() <= TVertexBufferCount );

        NN_SDK_ASSERT( nn::gfx::VertexState::GetRequiredMemorySize(info) <= sizeof(m_Memory) );
        NN_SDK_ASSERT( nn::util::BytePtr(&m_Memory).IsAligned(nn::gfx::VertexState::RequiredMemoryInfo_Alignment) );

        m_Object.SetMemory(&m_Memory, nn::gfx::VertexState::GetRequiredMemorySize(info));
        m_Object.Initialize(pDevice, info, pVertexShader);
    }

    /**
    * @brief 頂点ステートを破棄します。
    *
    * @param[in] pDevice デバイスへのポインタ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - 頂点ステートが初期化されている
    *
    * @post
    * - 頂点ステートが初期化されていない
    */
    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT
    {
        m_Object.Finalize(pDevice);
    }

    /**
    * @brief 頂点ステートのインスタンスを取得します。
    *
    * @return 頂点ステートのインスタンスを返します。
    */
    nn::gfx::VertexState* Get() NN_NOEXCEPT
    {
        return &m_Object;
    }

    /**
    * @brief 頂点ステートのインスタンスを取得します。
    *
    * @return 頂点ステートのインスタンスを返します。
    */
    const nn::gfx::VertexState* Get() const NN_NOEXCEPT
    {
        return &m_Object;
    }

private:
    typename nn::gfx::util::detail::AlignedStorage<
        detail::VertexStateHolderRequiredMemorySize<TVertexAttributeCount, TVertexBufferCount>::Value,
        nn::gfx::VertexState::RequiredMemoryInfo_Alignment
    >::Type m_Memory;

    nn::gfx::VertexState m_Object;
};

/**
* @brief nn::gfx::ViewportScissorState に必要な追加メモリをメンバとして確保するラッパークラスです。
*
* @tparam TViewportCount ビューポートの数。 nn::gfx::ViewportScissorState::SetViewportStateInfoArray() に与えるビューポートステート情報の数と同じかそれ以上の値を指定してください。
*/
template<int TViewportCount>
class ViewportScissorStateHolder
{
    NN_DISALLOW_COPY(ViewportScissorStateHolder);

public:

    /**
    * @brief コンストラクタです。
    */
    ViewportScissorStateHolder() NN_NOEXCEPT
    {
    }

    /**
    * @brief ビューポートシザーステートを初期化します。
    *
    * @param[in] pDevice デバイスへのポインタ
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - info.GetViewportCount() <= TViewportCount
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ビューポートシザーステートが初期化されていない
    *
    * @post
    * - ビューポートシザーステートが初期化されている
    */
    void Initialize(nn::gfx::Device* pDevice, const nn::gfx::ViewportScissorState::InfoType& info) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( info.GetViewportCount() <= TViewportCount );

        NN_SDK_ASSERT( nn::gfx::ViewportScissorState::GetRequiredMemorySize(info) <= sizeof(m_Memory) );
        NN_SDK_ASSERT( nn::util::BytePtr(&m_Memory).IsAligned(nn::gfx::ViewportScissorState::RequiredMemoryInfo_Alignment) );

        m_Object.SetMemory(&m_Memory, nn::gfx::ViewportScissorState::GetRequiredMemorySize(info));
        m_Object.Initialize(pDevice, info);
    }

    /**
    * @brief ビューポートシザーステートを破棄します。
    *
    * @param[in] pDevice デバイスへのポインタ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ビューポートシザーステートが初期化されている
    *
    * @post
    * - ビューポートシザーステートが初期化されていない
    */
    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT
    {
        m_Object.Finalize(pDevice);
    }

    /**
    * @brief ビューポートシザーステートのインスタンスを取得します。
    *
    * @return ビューポートシザーステートのインスタンスを返します。
    */
    nn::gfx::ViewportScissorState* Get() NN_NOEXCEPT
    {
        return &m_Object;
    }

    /**
    * @brief ビューポートシザーステートのインスタンスを取得します。
    *
    * @return ビューポートシザーステートのインスタンスを返します。
    */
    const nn::gfx::ViewportScissorState* Get() const NN_NOEXCEPT
    {
        return &m_Object;
    }

private:
    typename nn::gfx::util::detail::AlignedStorage<
        detail::ViewportScissorStateHolderRequiredMemorySize<TViewportCount>::Value,
        nn::gfx::ViewportScissorState::RequiredMemoryInfo_Alignment
    >::Type m_Memory;

    nn::gfx::ViewportScissorState m_Object;
};

/**
* @brief nn::gfx::Pipeline をグラフィックスパイプラインとして使う場合に必要な追加メモリをメンバとして確保する
*   ラッパークラスです。確保するメモリ量を静的に決定するために必要な値をテンプレート引数として受け取ります。
*
* @tparam TBlendTargetCount ブレンドターゲットの数。 nn::gfx::GraphicsPipelineInfo::SetBlendStateInfo() に与える nn::gfx::BlendStateInfo について、
*   nn::gfx::BlendStateInfo::SetBlendTargetStateInfoArray() に与えるブレンドターゲットステート情報の数と同じかそれ以上の値を指定してください。
* @tparam TVertexAttributeCount 頂点属性の数。 nn::gfx::GraphicsPipelineInfo::SetVertexStateInfo() に与える nn::gfx::VertexStateInfo について、
*   nn::gfx::VertexStateInfo::SetVertexAttributeStateInfoArray() に与える頂点属性ステート情報の数と同じかそれ以上の値を指定してください。
* @tparam TVertexBufferCount 頂点バッファの数。nn::gfx::GraphicsPipelineInfo::SetVertexStateInfo() に与える nn::gfx::VertexStateInfo について、
*   nn::gfx::VertexStateInfo::SetVertexBufferStateInfoArray() に与える頂点バッファステート情報の数と同じかそれ以上の値を指定してください。
*/
template<int TBlendTargetCount, int TVertexAttributeCount, int TVertexBufferCount>
class GraphicsPipelineHolder
{
    NN_DISALLOW_COPY(GraphicsPipelineHolder);

public:

    /**
    * @brief コンストラクタです。
    */
    GraphicsPipelineHolder() NN_NOEXCEPT
    {
    }

    /**
    * @brief グラフィックスパイプラインを初期化します。
    *
    * @param[in] pDevice デバイスへのポインタ
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - info.GetBlendStateInfo()->GetBlendTargetCount() <= TBlendTargetCount
    * - info.GetVertexStateInfo()->GetVertexAttributeCount() <= TVertexAttributeCount
    * - info.GetVertexStateInfo()->GetVertexBufferCount() <= TVertexBufferCount
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - パイプラインが初期化されていない
    *
    * @post
    * - パイプラインがグラフィックスパイプラインで初期化されている
    */
    void Initialize(nn::gfx::Device* pDevice, const nn::gfx::Pipeline::GraphicsInfoType& info) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( info.GetBlendStateInfo()->GetBlendTargetCount() <= TBlendTargetCount );
        NN_SDK_REQUIRES( info.GetVertexStateInfo()->GetVertexAttributeCount() <= TVertexAttributeCount );
        NN_SDK_REQUIRES( info.GetVertexStateInfo()->GetVertexBufferCount() <= TVertexBufferCount );

        NN_SDK_ASSERT( nn::gfx::Pipeline::GetRequiredMemorySize(info) <= sizeof(m_Memory) );
        NN_SDK_ASSERT( nn::util::BytePtr(&m_Memory).IsAligned(nn::gfx::Pipeline::RequiredMemoryInfo_Alignment) );

        m_Object.SetMemory(&m_Memory, nn::gfx::Pipeline::GetRequiredMemorySize(info));
        m_Object.Initialize(pDevice, info);
    }

    /**
    * @brief パイプラインを破棄します。
    *
    * @param[in] pDevice デバイスへのポインタ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - パイプラインが初期化されている
    *
    * @post
    * - パイプラインが初期化されていない
    */    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT
    {
        m_Object.Finalize(pDevice);
    }

    /**
    * @brief パイプラインのインスタンスを取得します。
    *
    * @return パイプラインのインスタンスを返します。
    */
    nn::gfx::Pipeline* Get() NN_NOEXCEPT
    {
        return &m_Object;
    }

    /**
    * @brief パイプラインのインスタンスを取得します。
    *
    * @return パイプラインのインスタンスを返します。
    */
    const nn::gfx::Pipeline* Get() const NN_NOEXCEPT
    {
        return &m_Object;
    }

private:
    typename nn::gfx::util::detail::AlignedStorage<
        detail::GraphicsPipelineHolderRequiredMemorySize<
            TBlendTargetCount,
            TVertexAttributeCount,
            TVertexBufferCount
        >::Value,
        nn::gfx::Pipeline::RequiredMemoryInfo_Alignment
    >::Type m_Memory;

    nn::gfx::Pipeline m_Object;
};

/**
* @brief nn::gfx::Pipeline をコンピュートパイプラインとして使う場合に必要な追加メモリを
*   メンバとして確保するラッパークラスです。
*/
class ComputePipelineHolder
{
    NN_DISALLOW_COPY(ComputePipelineHolder);

public:

    /**
    * @brief コンストラクタです。
    */
    ComputePipelineHolder() NN_NOEXCEPT
    {
    }

    /**
    * @brief コンピュートパイプラインを初期化します。
    *
    * @param[in] pDevice デバイスへのポインタ
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - パイプラインが初期化されていない
    *
    * @post
    * - パイプラインが演算パイプラインで初期化されている
    */
    void Initialize(nn::gfx::Device* pDevice, const nn::gfx::Pipeline::ComputeInfoType& info) NN_NOEXCEPT
    {
        m_Object.SetMemory(&m_Memory, nn::gfx::Pipeline::GetRequiredMemorySize(info));
        m_Object.Initialize(pDevice, info);
    }

    /**
    * @brief パイプラインを破棄します。
    *
    * @param[in] pDevice デバイスへのポインタ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - パイプラインが初期化されている
    *
    * @post
    * - パイプラインが初期化されていない
    */
    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT
    {
        m_Object.Finalize(pDevice);
    }

    /**
    * @brief パイプラインのインスタンスを取得します。
    *
    * @return パイプラインのインスタンスを返します。
    */
    nn::gfx::Pipeline* Get() NN_NOEXCEPT
    {
        return &m_Object;
    }

    /**
    * @brief パイプラインのインスタンスを取得します。
    *
    * @return パイプラインのインスタンスを返します。
    */
    const nn::gfx::Pipeline* Get() const NN_NOEXCEPT
    {
        return &m_Object;
    }

private:
    nn::gfx::util::detail::AlignedStorage<
        detail::ComputePipelineHolderRequiredMemorySize::Value,
        nn::gfx::Pipeline::RequiredMemoryInfo_Alignment
    >::Type m_Memory;

    nn::gfx::Pipeline m_Object;
};

/**
* @brief nn::gfx::RootSignature に必要な追加メモリをメンバとして確保するラッパークラスです。
*   確保するメモリ量を静的に決定するために必要な値をテンプレート引数として受け取ります。
*
* @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
*
* @tparam TDescriptorTableCount デスクリプタテーブルの数。 nn::gfx::RootSignatureInfo::SetDescriptorTableInfoArray() に与えるデスクリプタテーブルの数と同じかそれ以上の値を指定してください。
* @tparam TDynamicDescriptorCount 動的なデスクリプタの数。 nn::gfx::RootSignatureInfo::SetDynamicDescriptorInfoArray() に与える動的なデスクリプタの数と同じかそれ以上の値を指定してください。
* @tparam TTotalDescriptorRangeCount すべてのデスクリプタテーブルのデスクリプタ範囲の数を合計した数。 nn::gfx::RootSignatureInfo::SetDescriptorTableInfoArray() に与える nn::gfx::DescriptorTableInfo について、
*   nn::gfx::DescriptorTableInfo::SetDescriptorRangeInfoArray() に与えるデスクリプタ範囲の数を合計した数と同じかそれ以上の値を指定してください。
*/
template<int TDescriptorTableCount, int TDynamicDescriptorCount, int TTotalDescriptorRangeCount>
class NN_DEPRECATED RootSignatureHolder
{
    NN_DISALLOW_COPY(RootSignatureHolder);

public:

    /**
    * @brief コンストラクタです。
    */
    RootSignatureHolder() NN_NOEXCEPT
    {
    }

    /**
    * @brief ルートシグネチャを初期化します。
    *
    * @param[in] pDevice デバイスへのポインタ
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - info.GetDescriptorTableCount() <= TDescriptorTableCount
    * - info.GetDynamicDescriptorCount() <= TDynamicDescriptorCount
    * - すべてのデスクリプタテーブルのデスクリプタ範囲の数を合計した数 <= TTotalDescriptorRangeCount
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ルートシグネチャが初期化されていない
    *
    * @post
    * - ルートシグネチャが初期化されている
    */
    void Initialize(nn::gfx::Device* pDevice, const nn::gfx::RootSignature::InfoType& info) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( info.GetDescriptorTableCount() <= TDescriptorTableCount );
        NN_SDK_REQUIRES( info.GetDynamicDescriptorCount() <= TDynamicDescriptorCount );

        int descriptorTableCount = info.GetDescriptorTableCount();
        int sumDescriptorRange = 0;
        for( int idxDescriptorTable = 0; idxDescriptorTable < descriptorTableCount; ++idxDescriptorTable )
        {
            sumDescriptorRange += info.GetDescriptorTableInfoArray()[ idxDescriptorTable ].GetDescriptorRangeCount();
        }
        NN_SDK_REQUIRES( sumDescriptorRange <= TTotalDescriptorRangeCount );
        NN_UNUSED( sumDescriptorRange );

        NN_SDK_ASSERT( nn::gfx::RootSignature::GetRequiredMemorySize(info) <= sizeof(m_Memory) );
        NN_SDK_ASSERT( nn::util::BytePtr(&m_Memory).IsAligned(nn::gfx::RootSignature::RequiredMemoryInfo_Alignment) );

        m_Object.SetMemory(&m_Memory, nn::gfx::RootSignature::GetRequiredMemorySize(info));
        m_Object.Initialize(pDevice, info);
    }

    /**
    * @brief ルートシグネチャを破棄します。
    *
    * @param[in] pDevice デバイスへのポインタ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - ルートシグネチャが初期化されている
    *
    * @post
    * - ルートシグネチャが初期化されていない
    */
    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT
    {
        m_Object.Finalize(pDevice);
    }

    /**
    * @brief ルートシグネチャのインスタンスを取得します。
    *
    * @return ルートシグネチャのインスタンスを返します。
    */
    nn::gfx::RootSignature* Get() NN_NOEXCEPT
    {
        return &m_Object;
    }

    /**
    * @brief ルートシグネチャのインスタンスを取得します。
    *
    * @return ルートシグネチャのインスタンスを返します。
    */
    const nn::gfx::RootSignature* Get() const NN_NOEXCEPT
    {
        return &m_Object;
    }

private:
    typename nn::gfx::util::detail::AlignedStorage<
        detail::RootSignatureHolderRequiredMemorySize<
            TDescriptorTableCount,
            TDynamicDescriptorCount,
            TTotalDescriptorRangeCount
        >::Value,
        nn::gfx::RootSignature::RequiredMemoryInfo_Alignment
    >::Type m_Memory;

    nn::gfx::RootSignature m_Object;
};

}
}
}

NN_PRAGMA_POP_WARNINGS
