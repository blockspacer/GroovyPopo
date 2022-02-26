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
 *  @brief CharInfoAccessor定義です。
 *  @details CharInfoに関するアクセサを定義します。
 */

#include <nn/mii/mii_Common.h>

namespace nn { namespace mii {

/**
 * @brief @ref CharInfo アクセサ
 * @details CharInfoから情報を取得するためのアクセサです。
 */
class CharInfoAccessor 
{
    NN_DISALLOW_COPY(CharInfoAccessor) NN_NOEXCEPT;
    CharInfoAccessor() NN_NOEXCEPT;
    bool operator ==(const CharInfoAccessor& rhs) const NN_NOEXCEPT; ///< 比較禁止
    bool operator !=(const CharInfoAccessor& rhs) const NN_NOEXCEPT; ///< 比較禁止
public:
    /**
     * @brief コンストラクタ
     * @param[in] info 情報を取得する @ref CharInfo を指定します。
     * @details 引数で渡したCharInfoはコピーされ、クラスで保持されます。
     */
    explicit CharInfoAccessor(const CharInfo& info) NN_NOEXCEPT;

    /**
     * @brief @ref CharInfo が正常かを判定します。
     * @return 正常なCharInfoであればtrueを返します。
     * @details 内部のパラメータをチェックして正常であるかを判定します。
     */
    bool IsValid() const NN_NOEXCEPT;

    /**
     * @brief ニックネームを取得
     * @param[out] pName ニックネーム格納先
     * @param[in]  fontRegionMask サポートするFontRegionのマスク
     * @pre IsValid() == true && pName != 0
     * @details CharInfoからニックネームを取得します。
     *          内部で @ref Nickname::ReplaceFontRegion() を呼び出します。
     */
    void GetNickname(Nickname* pName,int fontRegionMask) const NN_NOEXCEPT;
    
    /**
     * @brief FontRegion を取得
     * @return FontRegionを返します。
     * @pre IsValid() == true
     * @details ニックネームの FontRegionを返します。
     */
    FontRegion GetFontRegion() const NN_NOEXCEPT;

    /**
     * @brief お気に入りの色を取得
     * @return お気に入りの色
     * @pre IsValid() == true
     */
    FavoriteColor GetFavoriteColor() const NN_NOEXCEPT;

    /**
     * @brief 性別を取得
     * @return 性別を返す
     * @pre IsValid() == true
     * @retval Gender_Male
     * @retval Gender_Female
     */
    Gender GetGender() const NN_NOEXCEPT;

    /**
     * @brief 身長を取得
     * @return 身長
     * @pre IsValid() == true
     * @details 値の範囲は、 HeightMin <= height <= HeightMax です。
     */
    int GetHeight() const NN_NOEXCEPT;

    /**
     * @brief 体格を取得
     * @return 体格
     * @pre IsValid() == true
     * @details 値の範囲は、 BuildMin <= build <= BuildMax です。
     */
    int GetBuild() const NN_NOEXCEPT;

    /**
     * @brief 髪の左右反転を返す
     * @return 髪型が左右反転している場合はtrueを返します。
     * @pre IsValid() == true
     */
    bool IsHairFlip() const NN_NOEXCEPT;

    /**
     * @brief 髪の種類を取得
     * @return 髪の種類を返します。
     * @pre IsValid() == true
     * @details 値の範囲は、 HairTypeMin <= build <= HairTypeMax です。
     */
    int GetHairType() const NN_NOEXCEPT;

    /**
     * @brief 顔の種類を取得
     * @return 顔の種類を返します。
     * @pre IsValid() == true
     * @details 値の範囲は、 FacelineTypeMin <= build <= FacelineTypeMax です。
     */
    int GetFacelineType() const NN_NOEXCEPT;
    
    /**
     * @brief 肌の色を取得
     * @return 肌の色を返します。
     * @pre IsValid() == true
     * @details 値の範囲は、 SkinColorMin <= build <= SkinColorMax です。
     */
    int GetSkinColor() const NN_NOEXCEPT;

private:
    CharInfo m_Info;
};


}}
