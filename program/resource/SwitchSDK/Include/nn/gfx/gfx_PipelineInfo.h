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
* @brief パイプライン初期化用の情報に関する API の宣言
*/

#pragma once

#include <nn/gfx/gfx_DataAccessorConverter.h>
#include <nn/gfx/gfx_PipelineInfoData.h>
#include <nn/gfx/gfx_ShaderInfo.h>
#include <nn/gfx/gfx_StateInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

/**
* @brief グラフィックスパイプラインを初期化するための情報を表すクラスです。
*/
class GraphicsPipelineInfo
    : public detail::DataContainer< GraphicsPipelineInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    GraphicsPipelineInfo() NN_NOEXCEPT
    {
    }

    /**
    * @brief 各パラメーターを既定値に設定するためのヘルパー関数です。
    *
    * @post
    * - 各パラメーターが設定されている
    *
    * @details
    * 以下を呼ぶことに相当します。
    * - SetRasterizerStateInfo( NULL );
    * - SetBlendStateInfo( NULL );
    * - SetDepthStencilStateInfo( NULL );
    * - SetRenderTargetStateInfo( NULL );
    * - SetVertexStateInfo( NULL );
    * - SetTessellationStateInfo( NULL );
    * - SetShaderPtr( NULL );
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief ラスタライザーステート情報を設定します。
    *
    * @param[in] value ラスタライザーステート情報へのポインター
    *
    * @post
    * - ラスタライザーステート情報が設定されている
    *
    * @details
    * ポインター先はオブジェクトの初期化後に破棄できます。
    */
    void SetRasterizerStateInfo( const RasterizerStateInfo* value ) NN_NOEXCEPT
    {
        this->pRasterizerState.ptr = nn::gfx::AccessorToData( value );
    }

    /**
    * @brief ブレンドステート情報を設定します。
    *
    * @param[in] value ブレンドステート情報へのポインター
    *
    * @post
    * - ブレンドステート情報が設定されている
    *
    * @details
    * ポインター先はオブジェクトの初期化後に破棄できます。
    */
    void SetBlendStateInfo( const BlendStateInfo* value ) NN_NOEXCEPT
    {
        this->pBlendState.ptr = nn::gfx::AccessorToData( value );
    }

    /**
    * @brief 深度ステンシルステート情報を設定します。
    *
    * @param[in] value 深度ステンシルステート情報へのポインター
    *
    * @post
    * - 深度ステンシルステート情報が設定されている
    *
    * @details
    * ポインター先はオブジェクトの初期化後に破棄できます。
    */
    void SetDepthStencilStateInfo( const DepthStencilStateInfo* value ) NN_NOEXCEPT
    {
        this->pDepthStencilState.ptr = nn::gfx::AccessorToData( value );
    }

    /**
    * @brief レンダーターゲットステート情報を設定します。
    *
    * @param[in] value レンダーターゲットステート情報へのポインター
    *
    * @post
    * - レンダーターゲットステート情報が設定されている
    *
    * @details
    * ポインター先はオブジェクトの初期化後に破棄できます。
    */
    void SetRenderTargetStateInfo( const RenderTargetStateInfo* value ) NN_NOEXCEPT
    {
        this->pRenderTargetState.ptr = nn::gfx::AccessorToData( value );
    }

    /**
    * @brief 頂点ステート情報を設定します。
    *
    * @param[in] value 頂点ステート情報へのポインター
    *
    * @post
    * - 頂点ステート情報が設定されている
    *
    * @details
    * ポインター先はオブジェクトの初期化後に破棄できます。
    */
    void SetVertexStateInfo( const VertexStateInfo* value ) NN_NOEXCEPT
    {
        this->pVertexState.ptr = nn::gfx::AccessorToData( value );
    }

    /**
    * @brief テッセレーションステート情報を設定します。
    *
    * @param[in] value テッセレーションステート情報へのポインター
    *
    * @post
    * - テッセレーションステート情報が設定されている
    *
    * @details
    * ポインター先はオブジェクトの初期化後に破棄できます。
    */
    void SetTessellationStateInfo( const TessellationStateInfo* value ) NN_NOEXCEPT
    {
        this->pTessellationState.ptr = nn::gfx::AccessorToData( value );
    }

    /**
    * @brief シェーダーへのポインターを設定します。
    *
    * @tparam TTarget シェーダーの低レベルグラフィックス API
    *
    * @param[in] value シェーダーへのポインター
    *
    * @post
    * - シェーダーへのポインターが設定されている
    *
    * @details
    * TTarget は初期化するつもりのパイプラインの TTarget と同じである必要があります。
    */
    template< typename TTarget >
    void SetShaderPtr( const TShader< TTarget >* value ) NN_NOEXCEPT
    {
        this->pShader = value;
    }

    /**
    * @brief シェーダーへのポインターを設定します。
    *
    * @param[in] value シェーダーへのポインター
    *
    * @post
    * - シェーダーへのポインターが設定されている
    *
    * @details
    * NULL 以外の値を設定する場合はテンプレート引数付きのほうを呼んでください
    */
    void SetShaderPtr( const void* value ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( value == NULL );
        this->pShader = value;
    }

    /**
    * @brief ラスタライザーステート情報を取得します。
    *
    * @return ラスタライザーステート情報へのポインターを返します。
    */
    const RasterizerStateInfo* GetRasterizerStateInfo() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pRasterizerState.ptr );
    }

    /**
    * @brief ブレンドステート情報を取得します。
    *
    * @return ブレンドステート情報へのポインターを返します。
    */
    const BlendStateInfo* GetBlendStateInfo() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pBlendState.ptr );
    }

    /**
    * @brief 深度ステンシルステート情報を取得します。
    *
    * @return 深度ステンシルステート情報へのポインターを返します。
    */
    const DepthStencilStateInfo* GetDepthStencilStateInfo() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pDepthStencilState.ptr );
    }

    /**
    * @brief レンダーターゲットステート情報を取得します。
    *
    * @return レンダーターゲットステート情報へのポインターを返します。
    */
    const RenderTargetStateInfo* GetRenderTargetStateInfo() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pRenderTargetState.ptr );
    }

    /**
    * @brief 頂点ステート情報を取得します。
    *
    * @return 頂点ステート情報へのポインターを返します。
    */
    const VertexStateInfo* GetVertexStateInfo() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pVertexState.ptr );
    }

    /**
    * @brief テッセレーションステート情報を取得します。
    *
    * @return テッセレーションステート情報へのポインターを返します。
    */
    const TessellationStateInfo* GetTessellationStateInfo() const NN_NOEXCEPT
    {
        return nn::gfx::DataToAccessor( this->pTessellationState.ptr );
    }

#if defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
    /**
    * @brief シェーダーへのポインターを取得します。
    *
    * @return シェーダーへのポインターを返します。
    *
    * @details
    * この関数の返り値は、nn::gfx::Shader 型のポインターに代入してください
    */
    const Shader* GetShaderPtr() const NN_NOEXCEPT;
#else
    detail::Caster< const void > GetShaderPtr() const NN_NOEXCEPT
    {
        return detail::Caster< const void >( pShader.ptr );
    }
#endif
};

/**
* @brief 演算パイプラインを初期化するための情報を表すクラスです。
*/
class ComputePipelineInfo
    : public detail::DataContainer< ComputePipelineInfoData >
{
public:
    /**
    * @brief コンストラクターです。
    *
    * @post
    * - 各パラメーターがゼロ初期化されている
    */
    ComputePipelineInfo() NN_NOEXCEPT
    {
    }

    /**
    * @brief 各パラメーターを既定値に設定するためのヘルパー関数です。
    *
    * @post
    * - 各パラメーターが設定されている
    */
    void SetDefault() NN_NOEXCEPT;

    /**
    * @brief シェーダーへのポインターを設定します。
    *
    * @tparam TTarget シェーダーの低レベルグラフィックス API
    *
    * @param[in] value シェーダーへのポインター
    *
    * @post
    * - シェーダーへのポインターが設定されている
    *
    * @details
    * TTarget は初期化するつもりのパイプラインの TTarget と同じである必要があります。
    */
    template< typename TTarget >
    void SetShaderPtr( const TShader< TTarget >* value ) NN_NOEXCEPT
    {
        this->pShader = value;
    }

    /**
    * @brief シェーダーへのポインターを設定します。
    *
    * @param[in] value シェーダーへのポインター
    *
    * @post
    * - シェーダーへのポインターが設定されている
    *
    * @details
    * NULL 以外の値を設定する場合はテンプレート引数付きのほうを呼んでください
    */
    void SetShaderPtr( const void* value ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( value == NULL );
        this->pShader = value;
    }

#if defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
    /**
    * @brief シェーダーへのポインターを取得します。
    *
    * @return シェーダーへのポインターを返します。
    *
    * @details
    * この関数の返り値は、nn::gfx::Shader 型のポインターに代入してください
    */
    const Shader* GetShaderPtr() const NN_NOEXCEPT;
#else
    detail::Caster< const void > GetShaderPtr() const NN_NOEXCEPT
    {
        return detail::Caster< const void >( pShader.ptr );
    }
#endif
};

}
}
