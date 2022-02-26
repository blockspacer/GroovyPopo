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
 *  @brief TextureShaderでFacelineテクスチャを描画するための定義です。
 *  @details 
 */


#include <nn/nn_Result.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/util/util_MathTypes.h>
#include <nn/mii/mii_Common.h>

namespace nn { namespace mii { namespace detail {

/// 前方参照(非公開)
class FacelineGpuBufferImpl;

}}}

namespace nn { namespace mii {

class Faceline;          //!< 前方参照
class TextureShaderInfo; //!< 前方参照

//! @name 構造体・クラス
//! @{

/**
 * @brief Facelineテクスチャ描画用バッファ
 * @details @ref TextureShader でFacelineテクスチャを描画するときに
 *          @n 必要なバッファなどをまとめたクラスです。
 *          @n このクラスは、1つのFacelineテクスチャを描画するごとに1つ必要です。
 * @attention @ref TextureShader を使わない場合は不要です。
 */
class FacelineGpuBuffer {
    NN_DISALLOW_COPY(FacelineGpuBuffer);
public:
    FacelineGpuBuffer() NN_NOEXCEPT;
    ~FacelineGpuBuffer() NN_NOEXCEPT;
    
    //! @name メモリ計算
    //! @{

    /*
     * @brief メモリのサイズの取得
     * @details FacelineGpuBuffer生成に必要な通常メモリのサイズを取得します。
     * @return サイズを返します。
     */
    static size_t CalculateMemorySize() NN_NOEXCEPT;

    /**
     * @brief メモリのアライメントの取得
     * @details FacelineGpuBuffer生成に必要な通常メモリのアライメントを取得します。
     * @return アライメントを返します。
     */
    static size_t CalculateMemoryAlignment() NN_NOEXCEPT;


    /**
     * @brief メモリプールのサイズ取得
     * @details FacelineGpuBuffer生成に必要なメモリプールのサイズを取得します。
     * @param[in]  pDevice    デバイス
     * @pre pSize != NULL 
     *      @n && pDevice != NULL
     * @return 成功するとメモリプールのサイズを返します。
     *         @n 失敗すると0を返します。
     */
    static size_t CalculateMemoryPoolSize(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
     * @brief メモリプールのアライメント取得
     * @details FacelineGpuBuffer生成に必要なメモリプールのアライメントを取得します。
     * @param[in]  pDevice    デバイス
     * @pre pSize != NULL 
     *      @n && pDevice != NULL
     * @return 成功するとメモリプールのアライメントを返します。
     *         @n 失敗すると0を返します。
     */
    static size_t CalculateMemoryPoolAlignment(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @}

    //! @name 初期化・終了処理
    //! @{
    /**
     * @brief FacelineGpuBufferを初期化します。
     * @attention pMemory および pMemoryPoolは、@ref FacelineGpuBuffer::Finalize() まで保持する必要があります。
     * @attention 初期化済みのFacelineGpuBufferを再初期化することは出来ません。
     *            @n @ref Finalize() してから呼び出します。
     * @param[in] pMemory          FacelineGpuBufferが使用するメモリ
     * @param[in] memorySize       pMemoryのサイズ
     * @param[in] pDevice          デバイス
     * @param[in] pMemoryPool      FacelineGpuBufferが使用するメモリプール
     * @param[in] memoryPoolOffset FacelineGpuBufferが使用するメモリプールのオフセット
     * @param[in] memoryPoolSize   FacelineGpuBufferが使用するメモリプールのサイズ
     * @return 成功するとnn::ResultSuccessを返します。
     * @pre pMemory != NULL
     *      @n && pDevice != NULL
     *      @n && pMemoryPool != NULL
     *      @n && ! @ref FacelineGpuBuffer::IsInitialized()
     *      @n && オフセット、サイズが正常
     * @post 成功したら FacelineGpuBuffer::IsInitialized() = true
     */
    nn::Result Initialize(
        void* pMemory
        ,size_t memorySize
        ,nn::gfx::Device* pDevice
        ,nn::gfx::MemoryPool* pMemoryPool
        ,ptrdiff_t memoryPoolOffset
        ,size_t memoryPoolSize) NN_NOEXCEPT;
    
    /**
     * @brief FacelineGpuBufferを破棄します。
     * @param[in] pDevice デバイス
     * @pre FacelineGpuBuffer::IsInitialized() == true
     * @post FacelineGpuBuffer::IsInitialized() = false
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
     * @brief Facelineの設定
     * @param[in] faceline 構築済みFacelineクラス
     * @details 描画対象のFacelineクラスを指定します。
     * @attention 初期化時に渡したメモリプールが CpuCached な場合、
     *          @n この関数の呼び出し後に、メモリプールをフラッシュする必要があります。
     */
    void SetFaceline(const Faceline& faceline) NN_NOEXCEPT;

    /**
     * @brief 書き込み先テクスチャを設定
     * @param[in] pDevice デバイス
     * @param[in] pTexture 書き込み先のテクスチャのポインタ
     * @param[in] info    TextureShader のTextureShaderInfo
     * @param[in] width   書き込み先のテクスチャの幅 
     * @param[in] height  書き込み先のテクスチャの高さ
     * @param[in] mipCount ミップマップ数
     * @pre pDevice != NULL 
     *      @n && pTexture != NULL && pTexture が初期化されている
     *      @n && TextureShaderInfo にテクスチャとして有効なフォーマットが設定されている
     *      @n && 0 < width && 0 < height && 0 < mipCount
     * @details 書き込み先のテクスチャを設定します。
     *          @n 内部で nn::gfx::ColorTargetViewを構築します。
     *          @n 一つ前のColorTargetViewをFinalizeしてから処理します。
     *          @n このFacelineGpuBuffer渡すをTextureShaderの初期化時に設定した
     *          @n TextureShaderInfoを渡してください。
     */
    void SetColorTarget(nn::gfx::Device* pDevice,
        nn::gfx::Texture* pTexture,
        const TextureShaderInfo& info,
        int width,
        int height,
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
    detail::FacelineGpuBufferImpl& GetImpl() NN_NOEXCEPT;
    const detail::FacelineGpuBufferImpl& GetImpl() const NN_NOEXCEPT;

    //! @}
private:
    detail::FacelineGpuBufferImpl* m_pImpl;
};

//! @}

}}
