/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

/**
 *  @file
 *  @brief MiiのFacelineテクスチャ描画用の定義です。
 *  @details 
 */

#include <nn/util/util_MathTypes.h>
#include <nn/gfx.h>
#include <nn/nn_Result.h>
#include <nn/mii/mii_Common.h>

namespace nn { namespace mii {

namespace detail 
{
    /// 前方参照(非公開)
    class FacelineImpl;
}

/// 前方参照
class DrawParam;
/// 前方参照
class Resource;
/// 前方参照
struct TemporaryBuffer;

/**
 * @brief Facelineテクスチャ生成用クラス
 * @details Facelineテクスチャ生成に必要なDrawParamを保持するクラスです。
 */

class Faceline {
    NN_DISALLOW_COPY(Faceline);
public:
    //! @name 列挙型
    //! @{

    /**
     * @brief Facelineテクスチャの描画タイプを表す列挙型
     * @details Facelineテクスチャへ描画するときの描画要素を表します。
     * @attention 描画順も表しています。
     */
    enum DrawType {
        DrawType_Skin,                 ///< 0 : 肌の色
        DrawType_Make,                 ///< 1 : メイク
        DrawType_Wrinkle,              ///< 2 : しわ
        DrawType_Beard,                ///< 3 : あごひげ
        DrawType_End,                  ////< 終端(4)
        DrawType_Min = DrawType_Skin,  ///< 最小値(0)
        DrawType_Max = DrawType_Beard, ///< 最大値(3)
    };

    /**
     * @brief テクスチャの列挙型
     * @details Facelineが保持するテクスチャを表します。
     */
    enum TextureType {
        TextureType_Make,                    ///< 0 : メイク
        TextureType_Wrinkle,                 ///< 1 : しわ
        TextureType_Beard,                   ///< 2 : あごひげ
        TextureType_End,                     ///< 終端(3)
        TextureType_Min = TextureType_Make,  ///< 最小値(0)
        TextureType_Max = TextureType_Beard, ///< 最大値(2)
    };

    //! @}

public:
    //! @name コンストラクタ/デストラクタ
    //! @{
    /**
     * @brief コンストラクタです。
     * @details 
     */
    Faceline() NN_NOEXCEPT;
    /**
     * @brief デストラクタです。
     * @details 
     */
    ~Faceline() NN_NOEXCEPT;
     //! @}

    //! @name メモリ計算
    //! @{

    /**
     * @brief メモリのサイズの取得
     * @details Faceline生成に必要な通常メモリのサイズを取得します。
     * @return サイズを返します。
     */
    static size_t CalculateMemorySize() NN_NOEXCEPT;

    /**
     * @brief メモリのアライメントの取得
     * @details Faceline生成に必要な通常メモリのアライメントを取得します。
     * @return アライメントを返します。
     */
    static size_t CalculateMemoryAlignment() NN_NOEXCEPT;


    /**
     * @brief メモリプールのサイズ取得
     * @details Faceline生成に必要なメモリプールのサイズを取得します。
     * @param[in]  pDevice    デバイス
     * @param[in]  resource   Faceline生成用のリソース
     * @pre pSize != NULL 
     *      @n && pDevice != NULL
     *      @n && Resource::IsInitialized()
     * @return 成功するとメモリプールのサイズを返します。
     *         @n 失敗すると0を返します。
     */
    static size_t CalculateMemoryPoolSize(nn::gfx::Device* pDevice, const Resource& resource) NN_NOEXCEPT;

    /**
     * @brief メモリプールのアライメント取得
     * @details Faceline生成に必要なメモリプールのアライメントを取得します。
     * @param[in]  pDevice    デバイス
     * @param[in]  resource   Faceline生成用のリソース
     * @pre pSize != NULL 
     *      @n && pDevice != NULL
     *      @n && Resource::IsInitialized()
     * @return 成功するとメモリプールのアライメントを返します。
     *         @n 失敗すると0を返します。
     */
    static size_t CalculateMemoryPoolAlignment(nn::gfx::Device* pDevice, const Resource& resource) NN_NOEXCEPT;

    //! @}

    //! @name 初期化・終了処理
    //! @{

    /**
     * @brief Facelineを初期化します。
     * @param[in] pMemory          Facelineが使用するメモリ
     * @param[in] memorySize       pMemoryのサイズ
     * @param[in] pDevice          デバイス
     * @param[in] pMemoryPool      Facelineが使用するメモリプール
     * @param[in] memoryPoolOffset Facelineが使用するメモリプールのオフセット
     * @param[in] memoryPoolSize   Facelineが使用するメモリプールのサイズ
     * @param[in] pTemporaryBuffer      一時バッファ用の構造体へのポインタ
     * @param[in] resource         Facelineの構築用リソース
     * @param[in] charInfo         生成するMiiのCharInfo
     * @param[in] isVerticalFlip   上下反転するかのフラグ
     * @return 成功するとnn::ResultSuccessを返します。
     * @pre pMemory != NULL
     *      @n && pDevice != NULL
     *      @n && pMemoryPool != NULL
     *      @n && Resource::IsInitialized()
     *      @n && ! @ref Faceline::IsInitialized()
     *      @n && pTemporaryBuffer != NULL
     *      @n && オフセット、サイズが正常
     *      @n && charInfoが壊れていない。
     * @post 成功したらFaceline::IsInitialized() = true
     * @attention pMemory および pMemoryPool は、@ref Faceline::Finalize() まで保持する必要があります。
     * @attention pMemoryPool には MemoryPoolProperty_CpuCached が設定されたメモリプールを渡してください。
     * @attention resource は、Resource の設定条件によって
     *            @n @ref Faceline::Finalize() まで保持の必要があります。
     * @attention 初期化済みのFacelineを再初期化することは出来ません。
     *            @n @ref Finalize() してから呼び出します。
     * @attention pTemporaryBufferは初期化中のみ必要です。
     *            @n 初期化が終われば破棄してかまいません。
     * @attention 上下反転フラグ isVerticalFlip はアプリケーションの座標系設定に応じて設定してください。
     */
    nn::Result Initialize(
        void* pMemory
        ,size_t memorySize
        ,nn::gfx::Device* pDevice
        ,nn::gfx::MemoryPool* pMemoryPool
        ,ptrdiff_t memoryPoolOffset
        ,size_t memoryPoolSize
        ,TemporaryBuffer* pTemporaryBuffer
        ,const Resource& resource
        ,const CharInfo& charInfo
        ,bool isVerticalFlip) NN_NOEXCEPT;

    /**
     * @brief Facelineを破棄します。
     * @param[in] pDevice デバイス
     * @pre Faceline::IsInitialized() == true
     * @post Faceline::IsInitialized() = false
     */
    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
     * @brief @ref Initialize() 済みかを判定
     * @return 初期化済みの場合はtrueを返します。
     */
    bool IsInitialized() const NN_NOEXCEPT;

    //! @}

    //! @name 設定・取得処理
    //! @{

    /**
     * @brief テクスチャビューの取得
     * @details 指定typeのテクスチャービューを取得します。
     * @param[in] type 取得するテクスチャビューの種類
     * @return 存在すればテクスチャービューを返します。存在しなければNULL
     */
    const nn::gfx::TextureView* GetTextureView(TextureType type) const NN_NOEXCEPT;

    /**
     * @brief テクスチャデスクリプタスロットの設定
     * @details 指定typeのテクスチャデスクリプタスロットを設定します。
     * @param[in] type 設定先の種類 
     * @param[in] slot 設定するデスクリプタスロット
     * @pre Faceline::IsInitialized() == true
     *      @n && slot.IsValid() == ture
     */
    void SetTextureDescriptorSlot(TextureType type, const nn::gfx::DescriptorSlot& slot) NN_NOEXCEPT;

    /**
     * @brief テクスチャデスクリプタスロットの取得
     * @details 指定typeのテクスチャデスクリプタスロットを取得します。
     * @param[in] type 設定先の種類 
     * @return 設定されているデスクリプタスロット
     *      @n (未設定なら無効なデスクリプタスロットが返ります。)
     */
    const nn::gfx::DescriptorSlot& GetTextureDescriptorSlot(TextureType type) const NN_NOEXCEPT;

    /**
     * @brief 描画パラメータを取得
     * @param[in] type       描画種類
     * @pre @ref IsInitialized() == true
     * @details 指定した条件の描画パラメータを取得します。
     * @attention テクスチャデスクリプタスロットは、
     *            @n @ref SetTextureDescriptorSlot() で変更されることに注意してください。 
     */
    const DrawParam* GetDrawParam(DrawType type) const NN_NOEXCEPT;

    //! @}
private:
    detail::FacelineImpl* m_pFacelineImpl; //!< 非公開
};

//! @}

}}
