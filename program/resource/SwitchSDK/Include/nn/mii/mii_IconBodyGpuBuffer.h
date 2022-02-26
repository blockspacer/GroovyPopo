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
 *  @brief IconShaderで体つきアイコンを描画するための定義です。
 *  @details 
 */


#include <nn/nn_Result.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/util/util_Color.h>
#include <nn/mii/mii_Common.h>
#include <nn/mii/mii_DrawCommon.h>
#include <nn/mii/mii_IconShader.h>

namespace nn { namespace mii { namespace detail {

class IconBodyGpuBufferImpl; //!< 前方参照(非公開)

}}}


namespace nn { namespace mii {

class CharModel; //!< 前方参照

//! @name 構造体・クラス
//! @{

/**
 * @brief 体つきアイコンテクスチャ描画用バッファ
 * @details @ref IconShader で体つきアイコンテクスチャを描画するときに
 *          @n 必要なバッファなどをまとめたクラスです。
 *          @n このクラスは、1つのアイコンテクスチャを描画するごとに1つ必要です。
 * @attention @ref IconShader を使わない場合は不要です。
 */
class IconBodyGpuBuffer {
    NN_DISALLOW_COPY(IconBodyGpuBuffer);
public:
    IconBodyGpuBuffer() NN_NOEXCEPT;
    ~IconBodyGpuBuffer() NN_NOEXCEPT;
    
    //! @name メモリ計算
    //! @{

    /**
     * @brief メモリのサイズの取得
     * @details IconBodyGpuBuffer生成に必要な通常メモリのサイズを計算します。
     * @return メモリサイズを返します。
     */
    static size_t CalculateMemorySize() NN_NOEXCEPT;

    /**
     * @brief メモリのアライメントの取得
     * @details IconBodyGpuBuffer生成に必要な通常メモリのアライメントを計算します。
     * @return 成功するとアライメントを返します。
     */
    static size_t CalculateMemoryAlignment() NN_NOEXCEPT;

    /**
     * @brief メモリプールのサイズの取得
     * @details IconBodyGpuBuffer生成に必要なメモリプールのサイズを取得します。
     * @param[in]  pDevice    デバイス
     * @pre pDevice != NULL
     * @return メモリプールのサイズを返します。
     */
    static size_t CalculateMemoryPoolSize(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
     * @brief メモリプールのアライメントの取得
     * @details IconBodyGpuBuffer生成に必要なメモリプールのサイズとアライメントを取得します。
     * @param[in]  pDevice    デバイス
     * @pre pDevice != NULL
     * @return メモリプールのアライメントを返します。
     */
    static size_t CalculateMemoryPoolAlignment(nn::gfx::Device* pDevice) NN_NOEXCEPT;
    //! @}

    //! @name 初期化・終了処理
    //! @{
    /**
     * @brief IconBodyGpuBufferを初期化します。
     * @param[in] pMemory          IconBodyGpuBufferが使用するメモリ
     * @param[in] memorySize       pMemoryのサイズ
     * @param[in] pDevice          デバイス
     * @param[in] pMemoryPool      IconBodyGpuBufferが使用するメモリプール
     * @param[in] memoryPoolOffset IconBodyGpuBufferが使用するメモリプールのオフセット
     * @param[in] memoryPoolSize   IconBodyGpuBufferが使用するメモリプールのサイズ
     * @return 成功するとnn::ResultSuccessを返します。
     * @pre pMemory != NULL
     *      @n && pDevice != NULL
     *      @n && pMemoryPool != NULL
     *      @n && ! @ref IconBodyGpuBuffer::IsInitialized()
     *      @n && オフセット、サイズが正常
     * @post 成功したら IconBodyGpuBuffer::IsInitialized() = true
     * @attention pMemory および pMemoryPoolは、@ref IconBodyGpuBuffer::Finalize() まで保持する必要があります。
     * @attention 初期化済みのIconBodyGpuBufferを再初期化することは出来ません。
     *            @n @ref Finalize() してから呼び出します。
     */
    nn::Result Initialize(
        void* pMemory
        ,size_t memorySize
        ,nn::gfx::Device* pDevice
        ,nn::gfx::MemoryPool* pMemoryPool
        ,ptrdiff_t memoryPoolOffset
        ,size_t memoryPoolSize) NN_NOEXCEPT;
    
    /**
     * @brief IconBodyGpuBufferを破棄します。
     * @param[in] pDevice デバイス
     * @pre IconBodyGpuBuffer::IsInitialized() == true
     * @post IconBodyGpuBuffer::IsInitialized() = false
     */
    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
     * @brief Initialize済みかを判定
     * @return 初期化済みの場合はtrueを返します。
     */
    bool IsInitialized() const NN_NOEXCEPT;

    //! @}

    //! @name 設定・取得
    //! @{


    /**
     * @brief 背景色を設定する
     * @param[in] color 背景色を指定します。
     * @details 
     * @attention @ref SetPremultipliedAlphaBlendForBackground() の影響を受けません。
     *            乗算済みアルファを使う場合は、カラーにアルファを乗算してください。
     */
    void SetBackgroundColor(const nn::util::Color4fType& color) NN_NOEXCEPT;

    /**
     * @brief 背景色を取得する
     * @details
     */
    const nn::util::Color4f& GetBackgroundColor() const NN_NOEXCEPT;

    /**
     * @brief カラーターゲットビューとデプスステンシルビューをクリアするかを指定します。
     * @param[in] clearColorTargetView  カラーターゲットビューをクリアするか
     * @param[in] clearDepthStencilView デプスステンシルビューをクリアするか
     * @details デフォルトでは、clearColorTargetView=true,clearDepthStencilView=trueです。
     *          clearColorTargetView=trueの場合、@ref SetBackgroundColor()で設定した色でクリアされます。
     *          clearDepthStencilView=trueの場合、depth=1.0,stencil=0でクリアされます。
     */
    void SetClearColorTargetViewAndDepthStencilView(bool clearColorTargetView,bool clearDepthStencilView) NN_NOEXCEPT;

    /**
     * @brief カラーターゲットビューをクリア設定を取得します。
     * @return カラーターゲットビューがクリアされる設定ならtrueを返します。
     * @details 
     */
    bool GetClearColorTargetView() const NN_NOEXCEPT;

    /**
     * @brief デプスステンシルビューをクリア設定を取得します。
     * @return デプスステンシルビューがクリアされる設定ならtrueを返します。
     * @details 
     */
    bool GetClearDepthStencilView() const NN_NOEXCEPT;

    /**
     * @brief 乗算済みアルファブレンドを利用するか設定します。
     * @param[in] enable 有効にするならtrueを指定します。
     * @details デフォルトでは,enable=falseです。
     *          trueに設定すると乗算済みアルファテクスチャとして出力されます。
     *          作成したテクスチャを他のテクスチャに重ねる場合(コンポジション)は
     *          こちらをお使いください。
     */
    void SetPremultipliedAlphaBlendForBackground(bool enable) NN_NOEXCEPT;

    /**
     * @brief 乗算済みアルファブレンドを利用しているか取得します。
     * @details 有効ならtrueを返します。
     */
    bool GetPremultipliedAlphaBlendForBackground() const NN_NOEXCEPT;

    /**
     * @brief CharModelを設定する
     *      @n ガンマの値を直接指定するタイプの本関数は廃止予定です。
     *      @n ガンマの値を指定しない SetCharModel を使用してください。
     * @param[in] model 構築済みmodel
     * @param[in] charInfo キャラインフォ
     * @param[in] modelType モデルの種類
     * @param[in] nose      鼻の種類
     * @param[in] mask      マスクの番号
     * @param[in] gammaType ガンマの種類
     * @details CharModelから必要な情報を設定します。
     *          @n 仮でガンマの種類を渡すようにしています。
     *          @n CharModel作成時に使用したリソースにあわせたガンマを設定してください。
     * @attention 初期化時に渡したメモリプールが CpuCached な場合、
     *          @n この関数の呼び出し後に、メモリプールをフラッシュする必要があります。
     */
    NN_DEPRECATED void SetCharModel(
        const CharModel& model, const CharInfo& charInfo,
        CreateModelType modelType, CreateNoseType nose, int mask,
        GammaType gammaType
        ) NN_NOEXCEPT;

    /**
     * @brief CharModelを設定する
     * @param[in] model 構築済みmodel
     * @param[in] charInfo キャラインフォ
     * @param[in] modelType モデルの種類
     * @param[in] nose      鼻の種類
     * @param[in] mask      マスクの番号
     * @details CharModelから必要な情報を設定します。
     * @attention 初期化時に渡したメモリプールが CpuCached な場合、
     *          @n この関数の呼び出し後に、メモリプールをフラッシュする必要があります。
     */
    void SetCharModel(
        const CharModel& model, const CharInfo& charInfo,
        CreateModelType modelType, CreateNoseType nose, int mask
        ) NN_NOEXCEPT;

    /**
     * @brief 書き込み先テクスチャを設定
     * @param[in] pDevice    デバイス
     * @param[in] colorTargetTexture  書き込み先のテクスチャ
     * @param[in] depthStencilTexture デプスステンシル用のテクスチャ
     * @param[in] info       IconShaderの初期化に必要な情報
     * @param[in] width      書き込み先のテクスチャの幅
     * @param[in] height     書き込み先のテクスチャの高さ
     * @details 書き込み先のテクスチャを設定します。
     *          @n 内部で nn::gfx::ColorTargetViewを構築します。
     *          @n 一つ前のColorTargetViewをFinalizeしてから処理します。
     *          @n IconShaderInfoからカラーターゲットビューやデプスステンシルビューの初期化に
     *          @n 必要な情報を取得します。
     * @attention 初期化時に渡したメモリプールが CpuCached な場合、
     *          @n この関数の呼び出し後に、メモリプールをフラッシュする必要があります。
     */
    void SetColorTargetAndDepthStencil(
        nn::gfx::Device* pDevice,
        const nn::gfx::Texture& colorTargetTexture,
        const nn::gfx::Texture& depthStencilTexture,
        const IconShaderInfo& info,
        int width,
        int height) NN_NOEXCEPT;

    //! @}

    //! @internal
    //! @name 内部実装用
    //! @{
    
    /**
     * @internal
     * @brief 実装を返します。
     * @attention 内部実装用です。使用しないでください。
     */
    detail::IconBodyGpuBufferImpl& GetImpl() NN_NOEXCEPT;
    const detail::IconBodyGpuBufferImpl& GetImpl() const NN_NOEXCEPT;

    //! @}
private:
    detail::IconBodyGpuBufferImpl* m_pImpl;
};

//! @}

}}
