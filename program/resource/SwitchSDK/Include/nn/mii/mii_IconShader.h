/*---------------------------------------------------------------------------*
  Copyright (C) Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *---------------------------------------------------------------------------*/

#pragma once

/**
 *  @file
 *  @brief アイコン描画用シェーダー定義です。
 *  @details 
 */


#include <nn/nn_Result.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/util/util_MathTypes.h>
#include <nn/mii/mii_Common.h>

namespace nn { namespace gfx {

class ResShaderFile; //!< 前方参照

} }

namespace nn { namespace mii { namespace detail {

class IconShaderImpl; //!< 前方参照(非公開)

}}}

namespace nn { namespace mii {


class IconBody;               //!< 前方参照
class IconGpuBuffer;     //!< 前方参照
class IconBodyGpuBuffer; //!< 前方参照

//! @name 構造体・クラス
//! @{

/**
 * @brief アイコン生成シェーダに渡す情報
 */
struct IconShaderInfoData
{
    nn::gfx::ImageFormat colorTargetImageFormat;  //!< カラーターゲットのイメージフォーマット
    nn::gfx::ImageFormat depthStencilImageFormat; //!< デプスステンシルのイメージフォーマット
};

/**
 * @brief IconShader生成用のインフォ
 * @details IconShaderの生成に必要な情報のアクセサです。
 */
class  IconShaderInfo : public IconShaderInfoData
{
public:
    IconShaderInfo() NN_NOEXCEPT;
    ~IconShaderInfo() NN_NOEXCEPT;

    //! @name 設定
    //! @{
    /**
     * @brief 書き込み先テクスチャのイメージフォーマットを指定します。
     * @param[in] imageFormat テクスチャのイメージフォーマット
     * @return 自身の参照を返します。
     */
    IconShaderInfo& SetColorTargetImageFormat(nn::gfx::ImageFormat imageFormat) NN_NOEXCEPT;

    /**
     * @brief デプスステンシル用のテクスチャのイメージフォーマットを指定します。
     * @param[in] imageFormat テクスチャのイメージフォーマット
     * @return 自身の参照を返します。
     */
    IconShaderInfo& SetDepthStencilImageFormat(nn::gfx::ImageFormat imageFormat) NN_NOEXCEPT;

    //! @}

    //! @name 取得
    //! @{
    /**
     * @brief 書き込み先テスクチャのイメージフォーマットを取得します。
     * @return テクスチャのイメージフォーマットを返します。
     */
    nn::gfx::ImageFormat GetColorTargetImageFormat() const NN_NOEXCEPT;

    /**
     * @brief デプスステンシル用のテスクチャのイメージフォーマットを取得します。
     * @return テクスチャのイメージフォーマットを返します。
     */
    nn::gfx::ImageFormat GetDepthStencilImageFormat() const NN_NOEXCEPT;

    //! @}
};


/**
 * @brief アイコン生成シェーダー
 * @details アイコンテクスチャを生成するためのシェーダークラスです。
 *          @n アイコンテクスチャを簡単に作成するためのユーティリティとして用意しています。
 *          @n このクラスを使わずにアプリ側でシェーダーを用意してもかまいません。
 */
class IconShader {
    NN_DISALLOW_COPY(IconShader);
public:
    IconShader() NN_NOEXCEPT;
    ~IconShader() NN_NOEXCEPT;

    //! @name メモリ計算
    //! @{

    /**
     * @brief メモリのサイズの取得
     * @details 生成に必要な通常メモリのサイズを取得します。
     * @param[in] resShaderFile  初期化済みのシェーダーファイル
     * @param[in] shaderCodeType シェーダーコードの種類
     * @param[in]  info       IconShaderの初期化時に必要な情報
     * @pre pShader と shaderSize が正常
     * @return サイズを返します。
     */
    static size_t CalculateMemorySize(
        const nn::gfx::ResShaderFile& resShaderFile,
        nn::gfx::ShaderCodeType shaderCodeType,
        const IconShaderInfo& info) NN_NOEXCEPT;

    /**
     * @brief メモリのアライメントの取得
     * @details 生成に必要な通常メモリのアライメントを取得します。
     * @param[in] resShaderFile  初期化済みのシェーダーファイル
     * @param[in] shaderCodeType シェーダーコードの種類
     * @param[in]  info       IconShaderの初期化時に必要な情報
     * @pre pShader と shaderSize が正常
     * @return アライメントを返します。
     */
    static size_t CalculateMemoryAlignment(
        const nn::gfx::ResShaderFile& resShaderFile,
        nn::gfx::ShaderCodeType shaderCodeType,
        const IconShaderInfo& info) NN_NOEXCEPT;

    //! @}

    //! @name 初期化・終了処理
    //! @{
    /**
     * @brief 初期化します。
     * @attention pMemory は、@ref IconShader::Finalize() まで保持する必要があります。
     * @attention 初期化済みのIconShaderを再初期化することは出来ません。
     *            @n @ref Finalize() してから呼び出します。
     * @attention 初期化済みのresShaderFileは@ref IconShader::Finalize() まで
     *            @n 初期化済みの状態で保持する必要があります。
     * @param[in] pMemory          IconShaderが使用するメモリ
     * @param[in] memorySize       pMemoryのサイズ
     * @param[in] resShaderFile  初期化済みのシェーダーファイル
     * @param[in] shaderCodeType シェーダーコードの種類
     * @param[in] pDevice          デバイス
     * @param[in]  info       IconShaderの初期化時に必要な情報
     * @return 成功するとnn::ResultSuccessを返します。
     * @pre pMemory != NULL
     *      @n && pDevice != NULL
     *      @n && pShader != NULL
     * @post 成功したら IconShader::IsInitialized() = true
     */
    nn::Result Initialize(void* pMemory
        ,size_t memorySize
        ,nn::gfx::Device* pDevice
        ,const nn::gfx::ResShaderFile& resShaderFile
        ,nn::gfx::ShaderCodeType shaderCodeType
        ,const IconShaderInfo& info
        ) NN_NOEXCEPT;

    /**
     * @brief 破棄します。
     * @param[in] pDevice デバイス
     * @pre IconShader::IsInitialized() == true
     * @post IconShader::IsInitialized() = false
     */
    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
     * @brief Initialize済みかを判定
     * @return 初期化済みの場合はtrueを返します。
     */
    bool IsInitialized() const NN_NOEXCEPT;

    //! @}

    //! @name コマンド発行
    //! @{
    
    /**
     * @brief 体無しアイコンを描画します。
     * @details 体無しでCharModelアイコンを描画します。
     * @attention @ref GetSamplerInfo() を使って作成したサンプラを使用してください。
     * @param[out] pCommandBuffer    描画コマンド発行先
     * @param[in]  pGpuBuffer        構築済み定数バッファのポインタ
     * @param[in]  samplerDescriptor サンプラデスクリプタ
     */
    void DrawIcon(nn::gfx::CommandBuffer* pCommandBuffer
        ,IconGpuBuffer* pGpuBuffer
        ,const nn::gfx::DescriptorSlot& samplerDescriptor) const NN_NOEXCEPT;

    /**
     * @brief 体有りアイコンを描画します。
     * @details 体有りでCharModelアイコンを描画します。
     * @attention @ref GetSamplerInfo() を使って作成したサンプラを使用してください。
     * @param[out] pCommandBuffer    描画コマンド発行先
     * @param[in] body               描画する体
     * @param[in] pGpuBuffer         構築済み定数バッファのポインタ
     * @param[in] samplerDescriptor  サンプラデスクリプタ
     */
    void DrawIcon(nn::gfx::CommandBuffer* pCommandBuffer
        ,const IconBody&  body
        ,IconBodyGpuBuffer* pGpuBuffer
        , const nn::gfx::DescriptorSlot& samplerDescriptor) const NN_NOEXCEPT;

    //! @}
private:
    detail::IconShaderImpl* m_pImpl;
};

//! @}

}}
