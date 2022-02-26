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
 *  @brief 似顔絵ライブラリの共通する定数、関数定義です。
 *  @details 定数、列挙型、構造体、関数を定義します。
 */

#include <nn/util/util_MathTypes.h>
#include <nn/nn_Common.h>
#include <nn/nn_StaticAssert.h>

namespace nn { namespace mii {

//! @name 定数
//! @{

static const int HeightMin = 0;  //!< 身長の最小値
static const int HeightMax = 127;//!< 身長の最大値

static const int BuildMin = 0;   //!< 体格の最小値
static const int BuildMax = 127; //!< 体格の最大値

static const int FacelineTypeMin = 0;  //!< 顔の種類の最小値
static const int FacelineTypeMax = 11; //!< 顔の種類の最大値

static const int HairTypeMin = 0;   //!< 髪の種類の最小値
static const int HairTypeMax = 131; //!< 髪の種類の最大値

static const int SkinColorMin = 0; //!< 肌の色の最小値
static const int SkinColorMax = 9; //!< 肌の色の最大値

static const int DatabaseMiiCount = 100; //!< データベース上のMiiの最大数
static const int DefaultMiiCount = 6;    //!< デフォルトMiiの最大数

static const size_t ImportFileSize = 4840;   //!< インポート、エクスポート用ファイルサイズ
static const size_t ImportFileAlignment = 4; //!< インポート、エクスポート用アライメントサイズ

static const size_t ResourceMemoryAlignment = 4; //!< シェイプ/テクスチャ/Miiアイコン用のボディリソースが要求するアライメント

//! @}

//! @name 列挙型
//! @{

/**
 * @brief 年代を表す列挙型
 */
enum Age {
    Age_Child,           ///< 0 : 子供
    Age_Adult,           ///< 1 : 大人
    Age_Elder,           ///< 2 : 老人
    Age_End,             ///< 終端(3)
    Age_All = Age_End,   ///< ランダム生成時用のすべてを表すフラグ(3)
    Age_Min = Age_Child, ///< 最小値(0)
    Age_Max = Age_Elder, ///< 最大値(2)
};

/**
 * @brief 性別を表す列挙型
 */
enum Gender {
    Gender_Male,                ///< 0 : 男性
    Gender_Female,              ///< 1 : 女性
    Gender_End,                 ///< 終端(2)
    Gender_All = Gender_End,    ///< ランダム生成時用のすべてを表すフラグ(2)
    Gender_Min = Gender_Male,   ///< 最小値(0)
    Gender_Max = Gender_Female, ///< 最大値(1)
};

/**
 * @brief 肌の色を表す列挙型
 */
enum Race {
    Race_Black,            ///< 0 : 黒人
    Race_White,            ///< 1 : 白人
    Race_Asian,            ///< 2 : アジア人
    Race_End,              ///< 終端(3)
    Race_All = Race_End,   ///< ランダム生成時用のすべてを表すフラグ(3)
    Race_Min = Race_Black, ///< 最小値(0)
    Race_Max = Race_Asian, ///< 最大値(2)
};

/**
 * @brief Miiに使われている文字列の地域情報
 * @details Miiの名前は言語地域により異なります。
 *          この情報により使われている文字セットを確認できます。
 *          それぞれのリージョンがサポートする文字セットは
 *          文字フィルタファイルを参照してください。
 */
enum FontRegion {
    FontRegion_JpUsEu,                  ///< 0 : 日本、アメリカ、ヨーロッパ
    FontRegion_China,                   ///< 1 : 中国
    FontRegion_Korea,                   ///< 2 : 韓国
    FontRegion_Taiwan,                  ///< 3 : 台湾
    FontRegion_End,                     ///< 終端(4)
    FontRegion_Min = FontRegion_JpUsEu, ///< 0 : 最小値
    FontRegion_Max = FontRegion_Taiwan, ///< 3 : 最大値 
};

/**
* @brief FontRegion のビットフラグ
* @details サポートするFontRegionを指定するときに利用するマスクです。
*          必要に応じてビットごとの OR で複数指定してください。
*/
enum FontRegionFlag {
    FontRegionFlag_JpUsEu = (1 << FontRegion_JpUsEu), ///< 日本、アメリカ、ヨーロッパ
    FontRegionFlag_China  = (1 << FontRegion_China),  ///< 中国
    FontRegionFlag_Korea  = (1 << FontRegion_Korea),  ///< 韓国
    FontRegionFlag_Taiwan = (1 << FontRegion_Taiwan), ///< 台湾
    FontRegionFlag_All = ((1 << FontRegion_End) - 1), ///< すべてのFontRegionを表すマスク
};

/**
 * @brief 取得元を表す列挙型
 * @details CharInfoの出典を表します。
 */
enum Source
{
    Source_Database, ///< 0 : 似顔絵データベースから取得
    Source_Default,  ///< 1 : デフォルトから取得
    Source_End,      ///< 終端(2)
};

/**
 * @brief 取得元を指定するためのビットフラグ
 * @details CharInfoを取得するときの取得先を表すマスクです。
 */
enum SourceFlag 
{
    SourceFlag_Database = (1 << Source_Database), ///< 似顔絵データベースから取得
    SourceFlag_Default  = (1 << Source_Default),  ///< デフォルトから取得
    SourceFlag_All      = (1 << Source_End ) - 1, ///< 全部
};


/**
* @brief お気に入りの色
* @details お気に入りの色を表す列挙型です。
*/
enum FavoriteColor {
    FavoriteColor_Red,                       ///<  0 : 赤色
    FavoriteColor_Orange,                    ///<  1 : 橙色
    FavoriteColor_Yellow,                    ///<  2 : 黄色
    FavoriteColor_YellowGreen,               ///<  3 : 黄緑色
    FavoriteColor_Green,                     ///<  4 : 緑色
    FavoriteColor_Blue,                      ///<  5 : 青色
    FavoriteColor_SkyBlue,                   ///<  6 : 水色
    FavoriteColor_Pink,                      ///<  7 : ピンク色
    FavoriteColor_Purple,                    ///<  8 : 紫色
    FavoriteColor_Brown,                     ///<  9 : 茶色
    FavoriteColor_White,                     ///< 10 : 白色
    FavoriteColor_Black,                     ///< 11 : 黒色
    FavoriteColor_End,                       ///< 終端(12)
    FavoriteColor_Min = FavoriteColor_Red,   ///< 最小値(0)
    FavoriteColor_Max = FavoriteColor_Black, ///< 最大値(11)
};

/**
 * @brief 表情を表す列挙型
 * @details 
 */
enum Expression {
    Expression_Normal,                      ///<  0 : 通常。エディタで作成したままの顔です。
    Expression_Smile,                       ///<  1 : 笑い、喜び。目がニコニコ目になります。
    Expression_Anger,                       ///<  2 : 怒り。目と眉毛がつりあがり、口が口角の下がった口になります。
    Expression_Sorrow,                      ///<  3 : 悲しみ、困り。目と眉毛が垂れて、口が波線になります。
    Expression_Surprise,                    ///<  4 : 驚き。眉毛が上がり、目が見開いた目になります。
    Expression_Blink,                       ///<  5 : まばたき。目が線になります。
    Expression_OpenMouth,                   ///<  6 : 口パク。口が大きく開いた口になります。
    Expression_SmileOpenMouth,              ///<  7 : 目がニコニコ目で、口が大きく開いた口になります。
    Expression_AngerOpenMouth,              ///<  8 : 目と眉毛がつりあがり、口が大きく開いた口になります。
    Expression_SorrowOpenMouth,             ///<  9 : 目と眉毛が垂れて、口が大きく開いた口になります。
    Expression_SurpriseOpenMouth,           ///< 10 : 眉毛が上がり、目が見開いた目になり、口が大きく開いた口になります。
    Expression_BlinkOpenMouth,              ///< 11 : 目が線になり、口が大きく開いた口になります。
    Expression_WinkLeft,                    ///< 12 : 左目を閉じてウィンクします。
    Expression_WinkRight,                   ///< 13 : 右目を閉じてウィンクします。
    Expression_WinkLeftOpenMouth,           ///< 14 : 左目を閉じてウィンクし、口が大きく開いた口になります。
    Expression_WinkRightOpenMouth,          ///< 15 : 右目を閉じてウィンクし、口が大きく開いた口になります。
    Expression_LikeWinkLeft,                ///< 16 : 左目を閉じてウィンクし、笑い口になります。
    Expression_LikeWinkRight,               ///< 17 : 右目を閉じてウィンクし、笑い口になります。
    Expression_Frustrated,                  ///< 18 : 両目を閉じてウィンクし、口が波線になります。
    Expression_End,                         ///< 終端(19)
    Expression_Min = Expression_Normal,     ///< 最小値(0)
    Expression_Max = Expression_Frustrated, ///< 最大値(18)
};

/**
 * @brief 表情を表すビットフラグ
 * @details 表情を指定するフラグです。
 *          必要に応じてビットごとの OR で複数指定してください。
 * @ref Expression
 */
enum ExpressionFlag {
    ExpressionFlag_Normal             = 1 << Expression_Normal            , ///< 通常。エディタで作成したままの顔です。
    ExpressionFlag_Smile              = 1 << Expression_Smile             , ///< 笑い、喜び。目がニコニコ目になります。
    ExpressionFlag_Anger              = 1 << Expression_Anger             , ///< 怒り。目と眉毛がつりあがり、口が口角の下がった口になります。
    ExpressionFlag_Puzzled            = 1 << Expression_Sorrow            , ///< 悲しみ、困り。目と眉毛が垂れて、口が波線になります。
    ExpressionFlag_Surprise           = 1 << Expression_Surprise          , ///< 驚き。眉毛が上がり、目が見開いた目になります。
    ExpressionFlag_Blink              = 1 << Expression_Blink             , ///< まばたき。目が線になります。
    ExpressionFlag_OpenMouth          = 1 << Expression_OpenMouth         , ///< 口パク。口が大きく開いた口になります。
    ExpressionFlag_Happy              = 1 << Expression_SmileOpenMouth    , ///< 目がニコニコ目で、口が大きく開いた口になります。
    ExpressionFlag_AngerOpenMouth     = 1 << Expression_AngerOpenMouth    , ///< 目と眉毛がつりあがり、口が大きく開いた口になります。
    ExpressionFlag_SorrowOpenMouth    = 1 << Expression_SorrowOpenMouth   , ///< 目と眉毛が垂れて、口が大きく開いた口になります。
    ExpressionFlag_Surprised          = 1 << Expression_SurpriseOpenMouth , ///< 眉毛が上がり、目が見開いた目になり、口が大きく開いた口になります。
    ExpressionFlag_BlinkOpenMouth     = 1 << Expression_BlinkOpenMouth    , ///< 目が線になり、口が大きく開いた口になります。
    ExpressionFlag_WinkLeft           = 1 << Expression_WinkLeft          , ///< 左目を閉じてウィンクします。
    ExpressionFlag_WinkRight          = 1 << Expression_WinkRight         , ///< 右目を閉じてウィンクします。
    ExpressionFlag_WinkLeftOpenMouth  = 1 << Expression_WinkLeftOpenMouth , ///< 左目を閉じてウィンクし、口が大きく開いた口になります。
    ExpressionFlag_WinkRightOpenMouth = 1 << Expression_WinkRightOpenMouth, ///< 右目を閉じてウィンクし、口が大きく開いた口になります。
    ExpressionFlag_Like               = 1 << Expression_LikeWinkLeft      , ///< 左目を閉じてウィンクし、笑い口になります。
    ExpressionFlag_LikeWinkRight      = 1 << Expression_LikeWinkRight     , ///< 右目を閉じてウィンクし、笑い口になります。
    ExpressionFlag_Frustrated         = 1 << Expression_Frustrated        , ///< 両目を閉じてウィンクし、口が波線になります。
    ExpressionFlag_All                = (1 << Expression_End) - 1         , ///< 全表情を表す。
};

/**
 * @brief ガンマの種類を表す列挙型
 * @details 
 */
enum GammaType 
{
    GammaType_Linear,                 ///< 0 : リニアガンマ
    GammaType_Srgb,                   ///< 1 : sRGBガンマ
    GammaType_End,                    ///< 終端
    GammaType_Min = GammaType_Linear, ///< 最小値(0)
    GammaType_Max = GammaType_Srgb,   ///< 最大値(1)
};

//! @}

//! @name 構造体・クラス
//! @{


/**
 * @brief Miiを表す構造体
 * @details 内部は公開されていません。
 *          情報にアクセスする場合は、 @ref CharInfoAccessor を使用します。
 *          アプリケーションのセーブや通信に利用できます。
 */
struct CharInfo 
{
    static const int StructureSize = 88; //!< 構造体のサイズ
    typedef int32_t DataType;

    DataType _buffer[StructureSize / sizeof(DataType)]; //!< 非公開
    NN_STATIC_ASSERT(StructureSize % sizeof(DataType) == 0);
private:
    bool operator ==(const CharInfo& rhs) const NN_NOEXCEPT; ///< 比較禁止
    bool operator !=(const CharInfo& rhs) const NN_NOEXCEPT; ///< 比較禁止
};

/**
 * @brief 取得元を含めたCharInfo
 * @details 
 */
struct CharInfoElement
{
    CharInfo info;   //!< CharInfo
    Source   source; //!< 取得元
};

/**
 * @brief Miiのニックネームを表す構造体
 * @details 終端文字を含むニックネームを表す構造体です。
 *          この構造体に格納される文字は、
 *          該当プラットフォームのエンディアンに従います。
 */
struct Nickname
{
    static const int Length = 10; //!< 終端記号を含まないニックネームの長さ

    uint16_t name[Length + 1];
    
    /**
     * @brief 有効なニックネームか判定する
     * @return 有効ならtrue
     */
    bool IsValid() const NN_NOEXCEPT;
    
    
    
    /**
    * @brief ニックネームの置き換え
    * @param[in] fontRegion このニックネームのFontRegion
    * @param[in] fontRegionMask サポートするFontRegionのマスク
    * @pre IsValid() == true
    * @details @ref CharInfoAccessor::GetNickname() で利用されます。
    *          @n fontRegion が fontRegionMask に含まれていれば何も行いません。
    *          @n 含まれていなければアスキー文字以外を「?」に置換したものを返します。
    */
    void ReplaceFontRegion(FontRegion fontRegion,int fontRegionMask) NN_NOEXCEPT;

    /**
     * @brief Nicknameを比較します。
     * @param[in] nickname 比較対象の名前
     * @return 一致していればtrueを返します。
     * @details このメソッドは、メモリの完全一致比較を行います。
     */
    bool operator==(const Nickname& nickname) const NN_NOEXCEPT;

    /**
     * @brief Nicknameを比較します。
     * @param[in] nickname 比較対象の名前
     * @return 不一致ならtrueを返します。
     * @details このメソッドは、メモリの完全一致比較を行います。
     */
    bool operator!=(const Nickname& nickname) const NN_NOEXCEPT
    {
        return !(*this == nickname);
    }

    /**
     * @internal
     * @brief 名前を設定する(内部実装用)
     * @param[in] nickname 設定する名前
     */
    void Set(const uint16_t* nickname) NN_NOEXCEPT;

    /**
     * @internal
     * @brief "?"マークに置き換える(内部実装用)
     * @details 文字を"?"に置き換えます。
     */
    void ReplaceQuestionMark() NN_NOEXCEPT;

    /**
     * @internal
     * @brief ニックネームが空かを判定する(内部実装用)
     * @return ニックネームが空ならtrue
     * @details ニックネームが空文字列か判定します。
     */
    bool IsEmpty() const NN_NOEXCEPT;

};

/**
 * @brief 3DS/WiiU用StoreData
 * @details 内部は公開されていません。
 *          情報にアクセスする場合は、 @ref Database::Convert() を使用して CharInfo に変換します。
 */
struct Ver3StoreData
{
    static const int StructureSize = 96; //!< 構造体のサイズ
    typedef int32_t DataType;

    DataType _buffer[StructureSize / sizeof(DataType)];
    NN_STATIC_ASSERT(StructureSize % sizeof(DataType) == 0);
private:
    bool operator ==(const CharInfo& rhs) const NN_NOEXCEPT; ///< 比較禁止
    bool operator !=(const CharInfo& rhs) const NN_NOEXCEPT; ///< 比較禁止
};

#if defined(NN_BUILD_CONFIG_OS_WIN)
#pragma warning(push)
#pragma warning(disable:4201)
#endif

/**
 * @brief 色を表す構造体(rgb)
 * @details 
 */
struct Color3 {
    union {
        float v[3];
        struct {
            float r;
            float g;
            float b;
        };
    };
};

#if defined(NN_BUILD_CONFIG_OS_WIN)
#pragma warning(pop)
#endif

//! @}

//! @name 関数
//! @{

/**
 * @brief お気に入りの色のRGB値を取得する
 * @param[in] color お気に入りの色の種類
 * @param[in] gamma 取得するガンマ空間
 * @return お気に入りの色のRGB値
 */
const Color3& GetFavoriteColor(FavoriteColor color,GammaType gamma) NN_NOEXCEPT;

/**
 * @brief 肌の色を取得します。
 * @param[in] color 肌の色の種類
 * @param[in] gamma 取得するガンマ空間
 * @return RGB値
 * @pre @ref SkinColorMin <= color <= @ref SkinColorMax
 * @details 顔、おでこ、鼻に使われます。
 */
const Color3& GetSkinColor(int color, nn::mii::GammaType gamma);

/**
 * @brief 表情の数を取得します。
 * @param[in] expressions @ref ExpressionFlag
 * @return 指定している表情の数を返します。
 * @details ExpressionFlagで指定している表情の数を取得します。
 */
int GetExpressionCount(int expressions);

//! @}

}}
