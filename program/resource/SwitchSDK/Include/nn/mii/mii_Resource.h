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
 *  @brief リソースアクセス用の定義です。
 *  @details 
 */


#include <nn/mii/mii_Common.h>
#include <nn/mii/mii_DrawCommon.h>

#include <nn/gfx.h>
#include <nn/util/util_MathTypes.h>
#include <nn/nn_Result.h>

namespace nn { namespace mii { namespace detail {
class ResourceImpl; //! 前方参照(非公開)
}}}

namespace nn { namespace mii {

//! @name 構造体・クラス
//! @{

/**
 * @brief Resource生成用のデータ
 * @details Resource生成に必要な情報をまとめた構造体です。
 */
struct ResourceInfoData {
    TextureQuality   textureQuality; ///< テクスチャリソースの品質
    ShapeQuality     shapeQuality;   ///< シェイプリソースの品質
    ResourceReadType read;    ///< リソースの読み取り形式
    GammaType        gamma;   ///< ガンマの種類
};

/**
 * @brief Resource生成用のインフォ
 * @details Resourceの生成に必要な情報のアクセサです。
 */
class ResourceInfo : public ResourceInfoData {
public:
    ResourceInfo() NN_NOEXCEPT;

    //! @name 設定
    //! @{
    /**
     * @brief デフォルト値を設定
     * @details デフォルト値は以下の通りです。
     *          @n @ref ResourceInfoData::textureQuality = TextureQuality_Middle
     *          @n @ref ResourceInfoData::shapeQuality   = ShapeQuality_Middle
     *          @n @ref ResourceInfoData::read    = ResourceReadType_OnMemory
     *          @n @ref ResourceInfoData::gamma   = GammaType_Linear
     * @return 自身の参照を返します。
     */
    ResourceInfo& SetDefault() NN_NOEXCEPT;

    /**
     * @brief テクスチャリソースの品質を指定します。
     * @param[in] quality リソースの品質
     * @return 自身の参照を返します。
     */
    ResourceInfo& SetTextureQuality(TextureQuality quality) NN_NOEXCEPT;

    /**
     * @brief シェイプリソースの品質を指定します。
     * @param[in] quality リソースの品質
     * @return 自身の参照を返します。
     */
    ResourceInfo& SetShapeQuality(ShapeQuality quality) NN_NOEXCEPT;

    /**
     * @brief リソースの読み取り方式を指定します。
     * @param[in] readType 読み取り方式
     * @return 自身の参照を返します。
     * @details ResourceReadType の種類が1種類のため効果はありません。
     *          この関数は、過去の互換性維持のために残されています。
     */
    ResourceInfo& SetResourceReadType(ResourceReadType readType) NN_NOEXCEPT;

    /**
     * @brief リソースのガンマを指定します。
     * @param[in] gammaType ガンマ
     * @return 自身の参照を返します。
     */
    ResourceInfo& SetGammaType(GammaType gammaType) NN_NOEXCEPT;

    //! @}

    //! @name 取得
    //! @{

    /**
     * @brief テクスチャリソースの品質を取得します。
     * @return テクスチャリソースの品質を返します。
     */
    TextureQuality GetTextureQuality() const NN_NOEXCEPT;

    /**
     * @brief シェイプリソースの品質を取得します。
     * @return シェイプリソースの品質を返します。
     */
    ShapeQuality GetShapeQuality() const NN_NOEXCEPT;

    /**
     * @brief リソースの読み取り方式を取得します。
     * @return リソースの読み取り方式を返します。
     */
    ResourceReadType GetResourceReadType() const NN_NOEXCEPT;

    /**
     * @brief リソースのガンマを取得します。
     * @return ガンマの種類を返します。
     */
    GammaType GetGammaType() const NN_NOEXCEPT;

    //! @}
};

/**
 * @brief リソースクラス
 * @details 似顔絵リソースデータを管理するクラスです。
 */
class Resource {
    NN_DISALLOW_COPY(Resource) NN_NOEXCEPT;
public:
    Resource() NN_NOEXCEPT;
    ~Resource() NN_NOEXCEPT;

    //! @name メモリ計算
    //! @{

    /**
     * @brief メモリのサイズの取得
     * @details Resource生成に必要な通常メモリのサイズを取得します。
     * @param[in]  pTextureResource    テクスチャリソースファイルへのポインタ
     * @param[in]  textureResourceSize テクスチャリソースのサイズ
     * @param[in]  pShapeResource      シェイプリソースファイルへのポインタ
     * @param[in]  shapeResourceSize   シェイプリソースのサイズ
     * @param[in]  info         設定済みの @ref ResourceInfo
     * @pre pTextureResource と textureResourceSize が正常
     *          @n && pShapeResource と shapeResourceSize が正常
     *          @n && pTextureResource と pShapeResource がともに
     *          @n && リソースのアライメント要求 ResourceMemoryAlignment を満たしている。
     * @return メモリサイズを返します。
     */
    static size_t CalculateMemorySize(
        const void* pTextureResource
        , size_t textureResourceSize
        , const void* pShapeResource
        , size_t shapeResourceSize
        ,const ResourceInfo& info) NN_NOEXCEPT;

    /**
     * @brief メモリのアライメントの取得
     * @details Resource生成に必要な通常メモリのアライメントを取得します。
     * @param[in]  pTextureResource    テクスチャリソースファイルへのポインタ
     * @param[in]  textureResourceSize テクスチャリソースのサイズ
     * @param[in]  pShapeResource      シェイプリソースファイルへのポインタ
     * @param[in]  shapeResourceSize   シェイプリソースのサイズ
     * @param[in]  info         設定済みの @ref ResourceInfo
     * @pre pTextureResource と textureResourceSize が正常
     *          @n && pShapeResource と shapeResourceSize が正常
     *          @n && pTextureResource と pShapeResource がともに
     *          @n && リソースのアライメント要求 ResourceMemoryAlignment を満たしている。
     * @return メモリアライメントを返します。
     */
    static size_t CalculateMemoryAlignment(
        const void* pTextureResource
        , size_t textureResourceSize
        , const void* pShapeResource
        , size_t shapeResourceSize
        , const ResourceInfo& info) NN_NOEXCEPT;

    //! @}

    //! @name 初期化・終了処理
    //! @{

    /**
     * @brief Resourceを初期化します。
     * @attention pMemory は、@ref Resource::Finalize() まで保持する必要があります。
     * @attention 初期化済みのResourceを再初期化することは出来ません。
     *            @n @ref Finalize() してから呼び出します。
     * @param[in] pMemory          Resourceが使用するメモリ
     * @param[in] memorySize       pMemoryのサイズ
     * @param[in]  pTextureResource    テクスチャリソースファイルへのポインタ
     * @param[in]  textureResourceSize テクスチャリソースのサイズ
     * @param[in]  pShapeResource      シェイプリソースファイルへのポインタ
     * @param[in]  shapeResourceSize   シェイプリソースのサイズ
     * @param[in] info             ResourceInfo
     * @param[in] pDevice          デバイス
     * @return 成功するとnn::ResultSuccessを返します。
     * @pre pMemory != NULL
     *      @n && pDevice != NULL
     *      @n && modelInfo.IsValid() != NULL
     *      @n && ! Resource::IsInitialized()
     *      @n && オフセット、サイズが正常
     *      @n && charInfoが壊れていない。
     *      @n && pTextureResource と textureResourceSize が正常
     *      @n && pShapeResource と shapeResourceSize が正常
     *      @n && pTextureResource と pShapeResource がともに
     *      @n && リソースのアライメント要求 ResourceMemoryAlignment を満たしている。
     * @post 成功したら Resource::IsInitialized() = true
     */
    nn::Result Initialize(
        void* pMemory, 
        size_t memorySize,
        const void* pTextureResource,
        size_t textureResourceSize,
        const void* pShapeResource,
        size_t shapeResourceSize,
        const ResourceInfo& info, 
        nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
     * @brief Resourceを破棄します。
     * @param[in] pDevice デバイス
     * @pre Resource::IsInitialized() == true
     * @post Resource::IsInitialized() = false
     */
    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
     * @brief Initialize済みかを判定
     * @return 初期化済みの場合はtrueを返します。
     */
    bool IsInitialized() const NN_NOEXCEPT;

    //! @}

    //! @internal
    //! @name 内部実装用
    //! @{
    
    /**
     * @internal
     * @brief 実装を返します。
     * @attention 内部実装用です。使用しないでください。
     */
    nn::mii::detail::ResourceImpl& GetImpl() NN_NOEXCEPT;
    const nn::mii::detail::ResourceImpl& GetImpl() const NN_NOEXCEPT;

    //! @}
private:
    nn::mii::detail::ResourceImpl* m_ResourceImpl;
};

//! @}

//! @name 関数
//! @{

/**
 * @brief 似顔絵リソースローダーの初期化を行います。
 * @return 初期化に成功するとnn::ResultSuccess()を返します。
 * @pre すでに似顔絵リソースローダーが初期化されていない。
 * @details Windows環境では何も処理を行いません。
 */
nn::Result InitializeResourceLoader() NN_NOEXCEPT;


/**
 * @brief 似顔絵リソースローダーの終了処理を行います。
 * @pre 似顔絵リソースローダーが初期化されている
 * @details Windows環境では何も処理を行いません。
 */
void FinalizeResourceLoader() NN_NOEXCEPT;

/**
 * @brief 似顔絵テクスチャリソースのサイズを取得します。
 * @param[in] info 取得するリソースの情報が入ったinfo
 * @return リソースのサイズを返します。
 * @pre    似顔絵リソースローダーが初期化されている
 * @details Windows環境では動作しません。
 */
size_t GetResourceTextureSize(const ResourceInfo& info) NN_NOEXCEPT;

/**
* @brief 似顔絵シェイプリソースのサイズを取得します。
* @param[in] info 取得するリソースの情報が入ったinfo
* @return リソースのサイズを返します。
* @pre    似顔絵リソースローダーが初期化されている
* @details Windows環境では動作しません。
*/
size_t GetResourceShapeSize(const ResourceInfo& info) NN_NOEXCEPT;

/**
 * @brief 似顔絵テクスチャリソースのロード
 * @param[out] pOut 似顔絵テクスチャリソースのロード先バッファ
 * @param[in]  size pOutのバッファサイズ
 * @param[in]  info 取得するリソースの情報が入ったinfo
 * @return nn::Resultを返します。
 * @pre 似顔絵リソースローダーが初期化されている
 *      && pOutがResourceMemoryAlignmentアライメントされている
 *      && pOut != nullptr
 *      && size == GetResourceTextureSize()
 * @details バッファに似顔絵テクスチャリソースをロードします。
 *          Windows環境では動作しません。
 */
nn::Result LoadResourceTexture(void* pOut,size_t size, const ResourceInfo& info) NN_NOEXCEPT;

/**
 * @brief 似顔絵シェイプリソースのロード
 * @param[out] pOut 似顔絵シェイプリソースのロード先バッファ
 * @param[in]  size pOutのバッファサイズ
 * @param[in]  info 取得するリソースの情報が入ったinfo
 * @return nn::Resultを返します。
 * @pre 似顔絵リソースローダーが初期化されている
 *      && pOutがResourceMemoryAlignmentアライメントされている
 *      && pOut != nullptr
 *      && size == GetResourceShapeSize()
 * @details バッファに似顔絵シェイプリソースをロードします。
 *          Windows環境では動作しません。
 */
nn::Result LoadResourceShape(void* pOut, size_t size, const ResourceInfo& info) NN_NOEXCEPT;





//! @}

}}
