
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
 *  @brief テクスチャ生成シェーダー定義です。
 *  @details 
 */

#include <nn/nn_Result.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/mii/mii_Common.h>

namespace nn { namespace gfx {

class ResShaderFile; //!< 前方参照

}}

namespace nn { namespace mii { namespace detail {

class TextureShaderImpl; //!< 前方参照(非公開)

}}}

namespace nn { namespace mii {

class Faceline;          //!< 前方参照
class Mask;              //!< 前方参照
class FacelineGpuBuffer; //!< 前方参照
class MaskGpuBuffer;     //!< 前方参照

//! @name 構造体・クラス
//! @{

/**
 * @brief TextureShader生成用のデータ
 * @details TextureShader生成に必要な情報をまとめた構造体です。
 */
struct TextureShaderInfoData {
    nn::gfx::ImageFormat facelineImageFormat; ///< Facelineテクスチャのイメージフォーマット
    nn::gfx::ImageFormat maskImageFormat;     ///< Maskテクスチャのイメージフォーマット
    bool                 facelineLeaveAlpha;  ///< Facelineテクスチャにアルファを残すか
};

/**
 * @brief TextureShader生成用のインフォ
 * @details TextureShaderの生成に必要な情報のアクセサです。
 */
class TextureShaderInfo : public TextureShaderInfoData {
public:
    TextureShaderInfo() NN_NOEXCEPT;
    ~TextureShaderInfo() NN_NOEXCEPT;

    //! @name 設定
    //! @{
    /**
     * @brief Facelineのイメージフォーマットを指定します。
     * @param[in] imageFormat Facelineテクスチャのイメージフォーマット
     * @return 自身の参照を返します。
     */
    TextureShaderInfo& SetFacelineImageFormat(nn::gfx::ImageFormat imageFormat) NN_NOEXCEPT;

    /**
     * @brief Maskのイメージフォーマットを指定します。
     * @param[in] imageFormat Maskテクスチャのイメージフォーマット
     * @return 自身の参照を返します。
     */
    TextureShaderInfo& SetMaskImageFormat(nn::gfx::ImageFormat imageFormat) NN_NOEXCEPT;

    /**
     * @brief Facelineテクスチャ生成時のアルファをテクスチャに残すか指定します。
     * @param[in] enable アルファを残す場合はtrueを設定します。
     * @details テクスチャにメイク、シワのアルファ値を書き込むかを指定します。
     * @return 自身の参照を返します。
     */
    TextureShaderInfo& SetFacelineLeaveAlpha(bool enable) NN_NOEXCEPT;
    //! @}

    //! @name 取得
    //! @{

    /**
     * @brief Facelineテスクチャのイメージフォーマットを取得します。
     * @return Facelineテクスチャのイメージフォーマットを返します。
     */
    nn::gfx::ImageFormat GetFacelineImageFormat() const NN_NOEXCEPT;

    /**
     * @brief Maskテスクチャのイメージフォーマットを取得します。
     * @return Maskテクスチャのイメージフォーマットを返します。
     */
    nn::gfx::ImageFormat GetMaskImageFormat() const NN_NOEXCEPT;

    /**
     * @brief Facelineテクスチャ生成時のアルファをテクスチャに残すか設定を取得します。
     * @return 設定が有効な場合はtrueを返します。
     * @details 
     */
    bool GetFacelineLeaveAlpha() const NN_NOEXCEPT;

    //! @}
};

/**
 * @brief テクスチャ生成シェーダー
 * @details Mask,Facelineテクスチャを生成するためのシェーダークラスです。
 *          @n Mask、Facelineテクスチャを簡単に作成するためのユーティリティとして用意しています。
 *          @n このクラスを使わずにアプリ側でシェーダーを用意してもかまいません。
 */
class TextureShader {
    NN_DISALLOW_COPY(TextureShader);
public:
    TextureShader() NN_NOEXCEPT;
    ~TextureShader() NN_NOEXCEPT;

    //! @name メモリ計算
    //! @{


    /**
     * @brief メモリのサイズの取得
     * @param[in] resShaderFile  初期化済みのシェーダーファイル
     * @param[in] shaderCodeType シェーダーコードの種類
     * @param[in] info             TextureShaderの初期化用のinfo
     * @param[in] pDevice        デバイスへのポインタ
     * @return サイズを返します。
     * @pre pDevice != nullptr && info の各イメージフォーマットに有効なフォーマットが設定されている。
     * @details Faceline生成に必要な通常メモリのサイズを取得します。
     */
    static size_t CalculateMemorySize(
        const nn::gfx::ResShaderFile& resShaderFile,
        nn::gfx::ShaderCodeType shaderCodeType,
        const TextureShaderInfo& info,
        nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
     * @brief メモリのアライメントの取得
     * @param[in] resShaderFile  初期化済みのシェーダーファイル
     * @param[in] shaderCodeType シェーダーコードの種類
     * @param[in] info             TextureShaderの初期化用のinfo
     * @param[in] pDevice        デバイスへのポインタ
     * @return アライメントを返します。
     * @pre pDevice != nullptr && info の各イメージフォーマットに有効なフォーマットが設定されている。
     * @details Faceline生成に必要な通常メモリのアライメントを取得します。
     */
    static size_t CalculateMemoryAlignment(
        const nn::gfx::ResShaderFile& resShaderFile,
        nn::gfx::ShaderCodeType shaderCodeType,
        const TextureShaderInfo& info,
        nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @}

    //! @name 初期化・終了処理
    //! @{
    /**
     * @brief TextureShaderを初期化します。
     * @attention pMemory およびは、@ref TextureShader::Finalize() まで保持する必要があります。
     * @attention 初期化済みのTextureShaderを再初期化することは出来ません。
     *            @n @ref Finalize() してから呼び出します。
     * @attention 初期化済みのresShaderFileは@ref TextureShader::Finalize() まで
     *            @n 初期化済みの状態で保持する必要があります。
     * @param[in] pMemory          TextureShaderが使用するメモリ
     * @param[in] memorySize       pMemoryのサイズ
     * @param[in] resShaderFile    初期化済みのシェーダーファイル
     * @param[in] shaderCodeType   シェーダーコードの種類
     * @param[in] pDevice          デバイス
     * @param[in] info             TextureShaderの初期化用のinfo
     * @return 成功するとnn::ResultSuccessを返します。
     * @pre pMemory != NULL
     *      @n && pDevice != NULL
     * @post 成功したら TextureShader::IsInitialized() = true
     */
    nn::Result Initialize(
        void* pMemory
        ,size_t memorySize
        ,nn::gfx::Device* pDevice
        ,const nn::gfx::ResShaderFile& resShaderFile
        ,nn::gfx::ShaderCodeType shaderCodeType
        ,const TextureShaderInfo& info
        ) NN_NOEXCEPT;

    /**
     * @brief TextureShaderを破棄します。
     * @param[in] pDevice デバイス
     * @pre TextureShader::IsInitialized() == true
     * @post TextureShader::IsInitialized() = false
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
     * @brief Facelineを描画する。
     * @details Facelineテクスチャへ描画します。
     * @attention @ref GetSamplerInfo() を使って作成したサンプラを使用してください。
     * @attention 内部でレンダーステートを変更します。
     * @param[out] pCommandBuffer    描画コマンド発行先
     * @param[in]  pGpuBuffer        構築済み定数バッファへのポインタ
     * @param[in]  samplerDescriptor サンプラデスクリプタ
     */
    void DrawFaceline(nn::gfx::CommandBuffer* pCommandBuffer
        ,FacelineGpuBuffer* pGpuBuffer
        ,const nn::gfx::DescriptorSlot& samplerDescriptor) const NN_NOEXCEPT;

    /**
     * @brief Maskを描画する。
     * @details Maskテクスチャへ描画します。
     * @attention @ref GetSamplerInfo() を使って作成したサンプラを使用してください。
     * @attention 内部でレンダーステートを変更します。 
     *            ChannelMaskがアルファのみに変更されることにご注意ください。
     * @param[out] pCommandBuffer    描画コマンド発行先
     * @param[in]  pGpuBuffer        構築済み定数バッファへのポインタ
     * @param[in]  samplerDescriptor サンプラデスクリプタ
     */
    void DrawMask(nn::gfx::CommandBuffer* pCommandBuffer
        ,MaskGpuBuffer* pGpuBuffer
        ,const nn::gfx::DescriptorSlot& samplerDescriptor) const NN_NOEXCEPT;

    //! @}

private:
    detail::TextureShaderImpl* m_pImpl;
};

//! @}

}}
