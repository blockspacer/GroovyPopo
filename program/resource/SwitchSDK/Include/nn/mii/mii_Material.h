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
 *  @brief Miiシェーダーパラメータに関する定義です。
 *  @details MiiEditと同じようにMiiを描画するための
 *           シェーダパラメータをまとめたヘッダです。
 */

#include <nn/mii/mii_Common.h>
#include <nn/mii/mii_CharModel.h>

namespace nn { namespace mii {

/**
 * @brief スペキュラマテリアル
 * @details スペキュラのマテリアル設定
 */
struct SpecularMaterial
{
    Color3 color;     ///< スペキュラ反射カラー
    float  factorA;   ///< 反射係数A
    float  factorB;   ///< 反射係数B
    float  shinness;  ///< 鋭さ
};

/**
 * @brief リムライトマテリアル
 * @details スペキュラのマテリアル設定
 */
struct RimLightMaterial
{
    Color3 color;  ///< リムライトカラー
    float  power;  ///< リムライト鋭さ
    float  width;  ///< リムライト幅
};

/**
 * @brief 1つのDrawParamのマテリアル
 * @details DrawParam1つあたりのマテリアル設定
 *          カラーは、すべてリニアガンマです。
 */
struct DrawParamMaterial
{
    float            halfLambertFactor;      ///< ハーフランバート係数
    float            sssSpecularBlendFactor; ///< スペキュラ・SSSのブレンド係数
    Color3           sssColor;               ///< サブサーフェイススキャッタカラー
    SpecularMaterial specular;               ///< スペキュラマテリアル
    RimLightMaterial rimLight;               ///< リムライトマテリアル
};



/**
* @brief 1つのMiiを表すマテリアル設定
* @details 
*/
class Material
{
    NN_DISALLOW_COPY(Material);
public:
    ///! 内部マテリアルの要素数
    static const int MaterialCount = 9;
    
    Material() NN_NOEXCEPT;
    ~Material() NN_NOEXCEPT;
    
    /**
     * @brief 初期化
     * @param[in] info  有効なCharInfo
     * @details マテリアル設定をロードします。
     */
    void Initialize(const CharInfo& info) NN_NOEXCEPT;
    
    /**
     * @brief 終了処理
     * @details 
     */
    void Finalize() NN_NOEXCEPT;
    
    /**
     * @brief 初期化済みか
     * @return 初期化済みの場合はtrueを返します。
     * @details 
     */
    bool IsInitialized() const NN_NOEXCEPT;
    
    /**
     * @brief マテリアルを取得します。
     * @details 
     * @pre IsInitialized() == true
     */
    const DrawParamMaterial& Get(CharModel::DrawType type) const NN_NOEXCEPT;
    
    /**
     * @brief 体用マテリアルを取得します。
     * @details
     * @pre IsInitialized() == true
     */
    const DrawParamMaterial& GetBody() const NN_NOEXCEPT;


    /**
    * @brief パンツ用マテリアルを取得します。
    * @details
    * @pre IsInitialized() == true
    */
    const DrawParamMaterial& GetPants() const NN_NOEXCEPT;

private:
    DrawParamMaterial m_Materials[MaterialCount];
    bool m_IsInitialized;
};

}}
