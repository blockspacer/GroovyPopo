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
 *  @brief 線形補間による透明ピクセルの色にじみ防止機能用ヘッダ
 *  @details TextureConevrterの--adjust-transparent-rgbをGPUを使ってランタイム実行します。
 */

#include <nn/nn_Result.h>
#include <nn/gfx.h>
#include <nn/mii/mii_Common.h>

namespace nn { namespace gfx {

class ResShaderFile; //!< 前方参照

}}

namespace nn { namespace mii { namespace detail {

class TransparentAdjusterImpl;

}}}

namespace nn { namespace mii {


//! @name 構造体・クラス
//! @{

/**
 * @brief TextureShader生成用のデータ
 * @details TextureShader生成に必要な情報をまとめた構造体です。
 */
struct TransparentAdjusterInfoData
{
    bool                 verticalFlip;                ///< UVのVを反転するか
    int                  colorTargetViewMipLevel;     ///< カラーターゲットビューの出力MipLevel
    nn::gfx::ImageFormat colorTargetViewImageFormat;  ///< カラーターゲットビューのイメージビューフォーマット
    nn::gfx::ImageFormat textureViewImageFormat;      ///< テクスチャビューのイメージフォーマット
};

/**
 * @brief TextureShader生成用のインフォ
 * @details TextureShaderの生成に必要な情報のアクセサです。
 */
class TransparentAdjusterInfo : public TransparentAdjusterInfoData
{
public:
    TransparentAdjusterInfo() NN_NOEXCEPT;
    ~TransparentAdjusterInfo() NN_NOEXCEPT;

    //! @name 設定・取得
    //! @{

    /**
     * @brief カラーターゲットビューのMipLevelを指定します。
     * @param[in] level
     * @pre level >= 0
     * @details 
     */
    void SetColorTargetViewMipLevel(int level) NN_NOEXCEPT;

    /**
     * @brief カラーターゲットビューのMipLevelを取得します。
     * @return 設定されているMipLevelを返します。
     * @details 
     */
    int GetColorTargetViewMipLevel() const NN_NOEXCEPT;

    /**
     * @brief カラーターゲットビューのイメージフォーマットを指定します。
     * @param[in] imageFormat カラーターゲットビューのイメージフォーマット
     * @details 
     */
    void SetColorTargetViewImageFormat(nn::gfx::ImageFormat imageFormat) NN_NOEXCEPT;

    /**
     * @brief カラーターゲットビューのイメージフォーマットを取得します。
     * @return カラーターゲットビューのイメージフォーマットを返します。
     */
    nn::gfx::ImageFormat GetColorTargetViewImageFormat() const NN_NOEXCEPT;

    /**
     * @brief テクスチャビューのイメージフォーマットを指定します。
     * @param[in] imageFormat テクスチャビューのイメージフォーマット
     * @details
     */
    void SetTextureViewImageFormat(nn::gfx::ImageFormat imageFormat) NN_NOEXCEPT;

    /**
     * @brief テクスチャビューのイメージフォーマットを取得します。
     * @return テクスチャビューのイメージフォーマットを返します。
     */
    nn::gfx::ImageFormat GetTextureViewImageFormat() const NN_NOEXCEPT;

    /**
     * @brief 垂直方向に画像を反転するか指定します。
     * @param[in] flip UVのV方向反転を行うか
     * @details 
     */
    void SetVerticalFlip(bool flip) NN_NOEXCEPT;

    /**
    * @brief 垂直方向に画像を反転するか取得します。
    * @return UVのV方向反転を行うか
    * @details
    */
    bool GetVerticalFlip() const NN_NOEXCEPT;
    //! @}
};


/**
 * @brief TransparentAdjuster用のGPUバッファ
 * @details TransparentAdjusterを実行するのに必要なバッファです。
 */
class TransparentAdjusterGpuBuffer
{
    NN_DISALLOW_COPY(TransparentAdjusterGpuBuffer);
public:
    TransparentAdjusterGpuBuffer();
    ~TransparentAdjusterGpuBuffer();
   
    //! @name 初期化
    //! @{
    
    
    /**
     * @brief メモリープールのサイズを計算します。
     * @param[in] pDevice デバイス
     * @return 必要なメモリプールのサイズを返します。
     * @details 
     */
    static size_t CalculateMemoryPoolSize(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
     * @brief メモリープールのアライメントを計算します。
     * @param[in] pDevice デバイス
     * @return 必要なメモリプールのアライメントを返します。
     * @details 
     */
    static size_t CalculateMemoryPoolAlignment(nn::gfx::Device* pDevice) NN_NOEXCEPT;
    
    /**
     * @brief TransparentAdjusterGpuBuffer を初期化します。
     * @param[in] pDevice          デバイス
     * @param[in] pMemoryPool      メモリプール
     * @param[in] memoryPoolOffset メモリプールのオフセット
     * @param[in] memoryPoolSize   メモリプールのサイズ
     * @details 
     */
    void Initialize(nn::gfx::Device* pDevice
                    , nn::gfx::MemoryPool* pMemoryPool
                    , ptrdiff_t memoryPoolOffset
                    , size_t memoryPoolSize) NN_NOEXCEPT;
    
    /**
     * @brief TransparentAdjusterGpuBuffer を破棄します。
     * @param[in] pDevice          デバイス
     * @details 
     */
    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT;
    
    
    //! @}
   
    //! @name 設定・取得
    //! @{
    /**
     * @brief パラメータをリセットします。
     * @details SetParameterで初期化したnn::gfxのオブジェクトをFinalize()します。
     *          未初期化の場合は何もしません。
     */
    void ResetParameter(nn::gfx::Device* pDevice) NN_NOEXCEPT;
    
    /**
     * @brief パラメータ設定
     * @param[in] pDevice             デバイス
     * @param[in] pColorTargetTexture 出力先のテクスチャ
     * @param[in] pTexture            元のテクスチャ
     * @param[in] width               テクスチャの幅
     * @param[in] height              テクスチャの高さ
     * @param[in] info                設定済みinfo
     * @details nn::gfxオブジェクトの生成とバッファのMapを行います。
     * @attention 初期化時に渡したメモリプールが CpuCached な場合、
     *          @n この関数の呼び出し後に、メモリプールをフラッシュする必要があります。
     */
    void SetParameter(nn::gfx::Device* pDevice
                      ,const nn::gfx::Texture* pColorTargetTexture
                      ,const nn::gfx::Texture* pTexture
                      ,int width
                      ,int height
                      ,const TransparentAdjusterInfo& info) NN_NOEXCEPT;
    //! @}

    //! @internal
    //! @name 内部実装用
    //! @{

    /**
     * @internal
     * @brief ViewportScissorStateの取得
     * @return ViewportScissorStateを返します。
     * @details 
     */
    const nn::gfx::ViewportScissorState& GetViewportScissorState() const NN_NOEXCEPT;

    /**
     * @internal
     * @brief ColorTargetViewの取得
     * @return ColorTargetViewを返します。
     * @details
     */
    const nn::gfx::ColorTargetView& GetColorTargetView() const NN_NOEXCEPT;

    /**
     * @internal
     * @brief TextureViewの取得
     * @return TextureViewを返します。
     * @details
     */
    const nn::gfx::TextureView& GetTextureView() const NN_NOEXCEPT;

    /**
     * @internal
     * @brief VertexBufferの取得
     * @return VertexBufferを返します。
     * @details
     */
    const nn::gfx::Buffer& GetVertexBuffer() const NN_NOEXCEPT;
    /**
     * @internal
     * @brief ConstantBufferの取得
     * @return ConstantBufferを返します。
     * @details
     */
    const nn::gfx::Buffer& GetConstantBuffer() const NN_NOEXCEPT;

    //! @}

private:
    nn::gfx::ViewportScissorState m_ViewportScissor;
    nn::gfx::ColorTargetView      m_ColorTargetView;
    nn::gfx::TextureView          m_TextureView;
    nn::gfx::Buffer               m_VertexBuffer;
    nn::gfx::Buffer               m_ConstantBuffer;
};


/**
 * @brief 線形補間による透明ピクセルの色のにじみを防止シェーダー
 * @details 透明(a=0)ピクセルのカラー値を周辺の不透明(a!=0)なピクセルから算出します。
 */
class TransparentAdjuster
{
    NN_DISALLOW_COPY(TransparentAdjuster);
public:
    TransparentAdjuster() NN_NOEXCEPT;
    ~TransparentAdjuster() NN_NOEXCEPT;

    //! @name メモリ計算
    //! @{
    
    /**
     * @brief メモリのサイズの取得
     * @param[in] resShaderFile  初期化済みのシェーダーファイル
     * @param[in] shaderCodeType シェーダーコードの種類
     * @param[in] info           TransparentAdjusterの初期化用のinfo
     * @return サイズを返します。
     * @details 生成に必要な通常メモリのサイズを取得します。
     */
    static size_t CalculateMemorySize(
        const nn::gfx::ResShaderFile& resShaderFile,
        nn::gfx::ShaderCodeType shaderCodeType,
        const TransparentAdjusterInfo& info) NN_NOEXCEPT;

    /**
     * @brief メモリのアライメントの取得
     * @return アライメントを返します。
     * @details 生成に必要な通常メモリのアライメントを取得します。
     */
    static size_t GetAlignment() NN_NOEXCEPT;
    
    //! @}

    //! @name 初期化・終了処理
    //! @{
    /**
     * @brief TransparentAdjusterを初期化します。
     * @attention pMemory およびは、@ref TransparentAdjuster::Finalize() まで保持する必要があります。
     * @attention 初期化済みのTransparentAdjusterを再初期化することは出来ません。
     *            @n @ref Finalize() してから呼び出します。
     * @attention 初期化済みのresShaderFileは@ref TransparentAdjuster::Finalize() まで
     *            @n 初期化済みの状態で保持する必要があります。
     * @param[in] pMemory          TransparentAdjusterが使用するメモリ
     * @param[in] memorySize       pMemoryのサイズ
     * @param[in] resShaderFile    初期化済みのシェーダーファイル
     * @param[in] shaderCodeType   シェーダーコードの種類
     * @param[in] pDevice          デバイス
     * @param[in] info             TransparentAdjusterの初期化用のinfo
     * @pre pMemory != NULL
     *      @n && pDevice != NULL
     * @post 成功したら TransparentAdjuster::IsInitialized() = true
     */
    void Initialize(
        void* pMemory
        ,size_t memorySize
        ,nn::gfx::Device* pDevice
        ,const nn::gfx::ResShaderFile& resShaderFile
        ,nn::gfx::ShaderCodeType shaderCodeType
        ,const TransparentAdjusterInfo& info
        ) NN_NOEXCEPT;

    /**
     * @brief TransparentAdjusterを破棄します。
     * @param[in] pDevice デバイス
     * @pre TransparentAdjuster::IsInitialized() == true
     * @post TransparentAdjuster::IsInitialized() = false
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
     * @brief コマンドバッファにコマンドを発行する
     * @param[out] pCommandBuffer コマンド発行先
     * @param[in]  pGpuBuffer     構築済み定数バッファ
     * @param[in]  samplerSlot    サンプラデスクリプタ
     * @param[in]  textureSlot    テクスチャデスクリプタ
     * @details テクスチャデスクリプタスロットは呼び出し側で設定する必要があります。
     *          このAPIは以下のコマンドを発行します。
     *          @n ・カラーターゲットの設定
     *          @n ・ViewportScissorの設定
     *          @n ・パイプラインの設定
     *          @n ・コンスタントバッファの設定
     *          @n ・テクスチャ・サンプラの設定
     *          @n ・頂点バッファの設定
     *          @n ・描画
     */
    void Enqueue(nn::gfx::CommandBuffer* pCommandBuffer
        ,TransparentAdjusterGpuBuffer* pGpuBuffer
        ,const nn::gfx::DescriptorSlot& samplerSlot
        ,const nn::gfx::DescriptorSlot& textureSlot) const NN_NOEXCEPT;
    
    //! @}

private:
    detail::TransparentAdjusterImpl* m_pImpl;
};

//! @}

}}
