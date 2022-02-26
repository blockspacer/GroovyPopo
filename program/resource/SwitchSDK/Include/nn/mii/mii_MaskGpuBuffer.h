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
 *  @brief TextureShaderでMaskテクスチャを描画するための定義です。
 *  @details 
 */

#include <nn/nn_Result.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/util/util_MathTypes.h>
#include <nn/mii/mii_Common.h>

namespace nn { namespace mii { namespace detail {

class MaskGpuBufferImpl; //!< 前方参照(非公開)

}}}

namespace nn { namespace mii {

class Mask;              //!< 前方参照
class TextureShaderInfo; //!< 前方参照

//! @name 構造体・クラス
//! @{

/**
 * @brief Maskテクスチャ描画用バッファ
 * @details @ref TextureShader でMaskテクスチャを描画するときに
 *          @n 必要なバッファなどをまとめたクラスです。
 *          @n このクラスは、1つのMaskテクスチャを描画するごとに1つ必要です。
 * @attention @ref TextureShader を使わない場合は不要です。
 */
class MaskGpuBuffer {
    NN_DISALLOW_COPY(MaskGpuBuffer);
public:
    MaskGpuBuffer() NN_NOEXCEPT;
    ~MaskGpuBuffer() NN_NOEXCEPT;
    
    //! @name メモリ計算
    //! @{

    /**
     * @brief メモリのサイズの取得
     * @details MaskGpuBuffer生成に必要な通常メモリのサイズを取得します。
     * @return サイズを返します。
     */
    static size_t CalculateMemorySize() NN_NOEXCEPT;
    /**
    * @brief メモリのアライメントの取得
    * @details MaskGpuBuffer生成に必要な通常メモリのアライメントを取得します。
    * @return アライメントを返します。
    */
    static size_t CalculateMemoryAlignment() NN_NOEXCEPT;

    /**
     * @brief メモリプールのサイズの取得
     * @details MaskGpuBuffer生成に必要なメモリプールのサイズを取得します。
     * @param[in]  pDevice     デバイス
     * @pre pDevice != NULL
     * @return 成功するとメモリプールのサイズを返します。
     */
    static size_t CalculateMemoryPoolSize(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
    * @brief メモリプールのアライメントの取得
    * @details MaskGpuBuffer生成に必要なメモリプールのアライメントを取得します。
    * @param[in]  pDevice     デバイス
    * @pre pDevice != NULL
    * @return 成功するとメモリプールのアライメントを返します。
    */
    static size_t CalculateMemoryPoolAlignment(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @}

    //! @name 初期化・終了処理
    //! @{
    /**
     * @brief MaskGpuBufferを初期化します。
     * @details 初期化後のデフォルト値は以下の通りです。
     * @attention pMemory および pMemoryPoolは、@ref MaskGpuBuffer::Finalize() まで保持する必要があります。
     * @attention 初期化済みのMaskGpuBufferを再初期化することは出来ません。
     *            @n @ref Finalize() してから呼び出します。
     * @param[in] pMemory          MaskGpuBufferが使用するメモリ
     * @param[in] memorySize       pMemoryのサイズ
     * @param[in] pDevice          デバイス
     * @param[in] pMemoryPool      MaskGpuBufferが使用するメモリプール
     * @param[in] memoryPoolOffset MaskGpuBufferが使用するメモリプールのオフセット
     * @param[in] memoryPoolSize   MaskGpuBufferが使用するメモリプールのサイズ
     * @return 成功するとnn::ResultSuccessを返します。
     * @pre pMemory != NULL
     *      @n && pDevice != NULL
     *      @n && pMemoryPool != NULL
     *      @n && ! @ref MaskGpuBuffer::IsInitialized()
     *      @n && オフセット、サイズが正常
     * @post 成功したらMaskGpuBuffer::IsInitialized() = true
     */
    nn::Result Initialize(
        void* pMemory
        ,size_t memorySize
        ,nn::gfx::Device* pDevice
        ,nn::gfx::MemoryPool* pMemoryPool
        ,ptrdiff_t memoryPoolOffset
        ,size_t memoryPoolSize) NN_NOEXCEPT;
    
    /**
     * @brief MaskGpuBufferを破棄します。
     * @param[in] pDevice デバイス
     * @pre MaskGpuBuffer::IsInitialized() == true
     * @post MaskGpuBuffer::IsInitialized() = false
     */
    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
     * @brief Initialize済みかを判定
     * @return 初期化済みの場合はtrueを返します。
     */
    bool IsInitialized() const NN_NOEXCEPT;

    //! @}

    //! @name 設定
    //! @{

    /**
     * @brief Maskの設定
     * @param[in] mask       構築済みMaskクラス
     * @param[in] expression 表情
     * @details 描画対象のMaskクラス、表情を指定します。
     * @attention 初期化時に渡したメモリプールが CpuCached な場合、
     *          @n この関数の呼び出し後に、メモリプールをフラッシュする必要があります。
     */
    void SetMask(const Mask& mask,Expression expression) NN_NOEXCEPT;

    /**
     * @brief 書き込み先テクスチャを設定
     * @param[in] pDevice      デバイス
     * @param[in] pTexture     書き込み先のテクスチャへのポインタ
     * @param[in] info         TextureShader のTextureShaderInfo
     * @param[in] resolution   書き込み先のテクスチャの解像度
     *    @n nn::mii::CharModelInfo::SetDynamicTextureResolution(int facelineHeightValue,int mask)  
     *    で設定した Mask テクスチャの解像度を指定してください。
     * @param[in] mipCount     書き込み先のテクスチャのミップマップ数
     * @pre pDevice != NULL
     *      @n && pTexture != NULL && pTexture が初期化されている
     *      @n && info に有効なテクスチャフォーマットが設定されている
     *      @n && 0 < resolution && 0 < mipCount
     * @details 書き込み先のテクスチャを設定します。
     *          @n Mask テクスチャは縦と幅のサイズが等しいテクスチャです。
     *          @n 内部で nn::gfx::ColorTargetViewを構築します。
     *          @n 一つ前のColorTargetViewをFinalizeしてから処理します。
     *          @n このMaskGpuBuffer渡すをTextureShaderの初期化時に設定した
     *          @n TextureShaderInfoを渡してください。
     */
    void SetColorTarget(nn::gfx::Device* pDevice,
        nn::gfx::Texture* pTexture,
        const TextureShaderInfo& info,
        int resolution,
        int mipCount) NN_NOEXCEPT;

    //! @}

    //! @internal
    //! @name 内部実装用
    //! @{
    
    /**
     * @internal
     * @brief 実装を返します。
     * @attention 内部実装用です。使用しないでください。
     */
    detail::MaskGpuBufferImpl& GetImpl() NN_NOEXCEPT;
    const detail::MaskGpuBufferImpl& GetImpl() const NN_NOEXCEPT;

    //! @}
private:
    detail::MaskGpuBufferImpl* m_pImpl;
};

//! @}

}}
