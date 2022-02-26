
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
 *  @brief 似顔絵ライブラリの描画に共通する定数、関数定義です。
 *  @details 定数、列挙型、構造体、関数を定義します。
 */


#include <nn/nn_Result.h>
#include <nn/gfx.h>
#include <nn/mii/mii_Common.h>
#include <nn/util/util_Decompression.h>

namespace nn { namespace mii {

//! @name 列挙型
//! @{

/**
 * @brief モデルの種類を表す列挙型
 * @details 生成するモデルの種類を表します。
 */
enum CreateModelType {
    CreateModelType_Normal,                         ///< 0 : 通常モデルを表す
    CreateModelType_Hat,                            ///< 1 : かぶり物用モデルを表す
    CreateModelType_FaceOnly,                       ///< 2 : 顔のみモデルを表す
    CreateModelType_End,                            ///< 終端(3)
    CreateModelType_Min = CreateModelType_Normal,   ///< 最小値(0)
    CreateModelType_Max = CreateModelType_FaceOnly, ///< 最大値(2)
};

/**
 * @brief 鼻の種類を表す列挙型
 * @details 生成する鼻の種類を表します。
 */
enum CreateNoseType {
    CreateNoseType_Normal,                       ///< 0 : 通常の鼻
    CreateNoseType_Flatten,                      ///< 1 : 長さを調整した鼻
    CreateNoseType_End,                          ///< 終端(2)
    CreateNoseType_Min = CreateNoseType_Normal,  ///< 最小値(0)
    CreateNoseType_Max = CreateNoseType_Flatten, ///< 最大値(1)
};

/**
 * @brief モデル生成フラグ
 * @details モデル生成時の条件を指定するフラグです。
 *          必要に応じてビットごとの OR で複数指定してください。
 */
enum CreateFlag {
    CreateFlag_Normal      = (1 << 0), ///< 通常モデルを生成します。
    CreateFlag_Hat         = (1 << 1), ///< かぶり物用モデルを生成します。
    CreateFlag_FaceOnly    = (1 << 2), ///< 顔のみモデルを生成します。
    CreateFlag_NoseNormal  = (1 << 3), ///< 通常の鼻を生成します。
    CreateFlag_NoseFlatten = (1 << 4), ///< 長さを調整した鼻を生成します。
    CreateFlag_Default 
        = CreateFlag_Normal | CreateFlag_NoseNormal, ///< デフォルト
};

/**
 * @brief シェイプリソースの品質を表す列挙型
 * @details 
 */
enum ShapeQuality 
{
    ShapeQuality_Middle,                    ///< 0 : ミドルモデル
    ShapeQuality_High,                      ///< 1 : ハイモデル
    ShapeQuality_End,                       ///< 2: 終端
    ShapeQuality_Min = ShapeQuality_Middle, ///< 最小値(0)
    ShapeQuality_Max = ShapeQuality_High,   ///< 最大値(1)
};

/**
 * @brief テクスチャリソースの品質を表す列挙型
 * @details 
 */
enum TextureQuality
{
    TextureQuality_Low,                         ///< 0 : 低解像度
    TextureQuality_Middle,                      ///< 1 : 中解像度
    TextureQuality_End,                         ///< 終端
    TextureQuality_Min = TextureQuality_Low,    ///< 最小値(0)
    TextureQuality_Max = TextureQuality_Middle, ///< 最大値(1)
};


/**
 * @brief リソースの読み取り形式
 * @details Miiリソースの読み取り形式を表す列挙型です。
 */
enum ResourceReadType {
    ResourceReadType_OnMemory,       ///< 0 : 圧縮リソースをメモリに置く
    ResourceReadType_Min 
        = ResourceReadType_OnMemory, ///< 最小値(0)
    ResourceReadType_Max 
        = ResourceReadType_OnMemory, ///< 最大値(0)
};

/**
 * @brief 座標系を指定するための列挙型
 * @details 
 */
enum CoordinateType {
    CoordinateType_XPlus  = 0x00000001, ///< X軸プラス方向
    CoordinateType_XMinus = 0x000000FF, ///< X軸マイナス方向
    CoordinateType_YPlus  = 0x00000100, ///< Y軸プラス方向
    CoordinateType_YMinus = 0x0000FF00, ///< Y軸マイナス方向
    CoordinateType_ZPlus  = 0x00010000, ///< Z軸プラス方向
    CoordinateType_ZMinus = 0x00FF0000, ///< Z軸マイナス方向
    CoordinateType_DefaultUp
         = CoordinateType_YPlus,        ///< 上方向初期値
    CoordinateType_DefaultFront
         = CoordinateType_ZPlus,        ///< 前方向初期値
};

/**
 * @brief 頭部装飾物の位置の種類を指定するための列挙型
 * @details
 */
enum HeadPartsType {
    HeadPartsType_Front,                      ///< 0 : おでこ用アクセサリ
    HeadPartsType_Right,                      ///< 1 : 右耳用アクセサリ
    HeadPartsType_Left,                       ///< 2 : 左耳用アクセサリ
    HeadPartsType_Top,                        ///< 3 : 頭の上用アクセサリ

    HeadPartsType_End,                        ///< 終端
    HeadPartsType_Min = HeadPartsType_Front,  ///< 最小値(0)
    HeadPartsType_Max = HeadPartsType_Top,    ///< 最大値(3)
};

//! @}

//! @name 構造体・クラス
//! @{

/**
 * @brief パーツの配置情報
 * @details かぶり物やアクセサリの配置情報です。
 */
struct PartsTransform 
{
    ::nn::util::Float3 hatTranslate;       ///< かぶり物パーツの移動値
    ::nn::util::Float3 headFrontRotate;    ///< おでこ用パーツの回転値
    ::nn::util::Float3 headFrontTranslate; ///< おでこ用パーツの移動値
    ::nn::util::Float3 headSideRotate;     ///< 右耳用パーツの回転値
    ::nn::util::Float3 headSideTranslate;  ///< 右耳用パーツの移動値
    ::nn::util::Float3 headTopRotate;      ///< 頭の上用パーツの回転値
    ::nn::util::Float3 headTopTranslate;   ///< 頭の上用パーツの移動値
};

/**
 * @brief アクセサリの配置情報
 * @details 一か所分のアクセサリの配置情報です。
 */
struct HeadPartsTransform
{
    nn::util::Float3 rotate;    ///< 回転値
    nn::util::Float3 translate; ///< 移動値
};

/**
 * @brief バウンディング情報
 * @details AABBバウンディング情報です。
 */
struct BoundingBox 
{
    ::nn::util::Float3 min;    ///< xyzの最小値
    ::nn::util::Float3 max;    ///< xyzの最大値
};

/**
 * @brief 体無しアイコン描画に必要なパラメータを格納する構造体
 * @details コンスタントバッファに格納するためFloatColumnMajor4x3を利用しています。
 * @attention CharModelのモデル行列は単位行列です。
 */
struct IconInfo 
{
    nn::util::FloatColumnMajor4x4 projection; ///< 投影行列
    nn::util::FloatColumnMajor4x3 modelView;  ///< モデルビュー行列
    nn::util::Float3   lightDir;         ///< ライト方向
    nn::mii::Color3    lightColor;       ///< ライトカラー
};

/**
 * @brief 体有りアイコン描画に必要なパラメータを格納する構造体
 * @details コンスタントバッファに格納するためFloatColumnMajor4x3を利用しています。
 * @attention CharModelのモデル行列は単位行列です。
 */
struct IconBodyInfo 
{
    nn::util::FloatColumnMajor4x4 projection;         ///< 投影行列
    nn::util::FloatColumnMajor4x3 modelViewFace;      ///< 顔のモデルビュー行列
    nn::util::FloatColumnMajor4x3 modelViewBodyModel; ///< 体のモデルビュー行列
    nn::util::Float3         lightDir;           ///< ライト方向
    nn::mii::Color3          lightColor;         ///< ライトカラー
};

/**
 * @brief 体格反映体付きアイコンのワールドマトリックスを格納する構造体
 * @details 体格反映体つきアイコンの体格計算を行うため行列を保持しています。
 */
struct VariableIconBodyWorldMatrix
{
    static const int Count = 25; ///< マトリックスの要素数
    nn::util::Matrix4x3fType matrixes[Count]; ///< ボーンごとのワールドマトリックス
};

/**
 * @brief 体格反映体付きアイコンのスキニング用マトリックスを格納する構造体
 * @details 体格反映体つきアイコンの描画用のマトリックスバッファです。
 *          コンスタントバッファに格納するためFloatColumnMajor4x3を利用しています。
 */
struct VariableIconBodySkeleton
{
    static const int Count = 22; ///< マトリックスの要素数
    nn::util::FloatColumnMajor4x3 matrixes[Count]; ///< スキニングマトリックス
};

/**
 * @brief Faceline、Mask、CharModel の初期化時に使用する一時バッファ用の構造体のサイズ
 * @details 初期化時に使用する一時バッファ用の構造体のサイズです。
 */
const size_t TemporaryBufferSize = nn::util::DecompressZlibWorkBufferSize;

/**
 * @brief Faceline、Mask、CharModel の初期化時に使用する一時バッファ用の構造体
 * @details 初期化時に圧縮されたリソースを展開する際に使用する一時バッファ用の構造体です。
 */
struct TemporaryBuffer
{
    uint8_t _data[TemporaryBufferSize];
};
NN_STATIC_ASSERT(sizeof(TemporaryBuffer) == TemporaryBufferSize);

//! @}

//! @name 関数
//! @{

/**
 * @brief サンプラのInfoを取得
 * @param[out] pInfo サンプラInfoの格納先
 * @pre pInfo != NULL
 * @details Mii描画用のサンプラのinfoを取得します。
 *          @n 似顔絵を描画する場合はこのinfoを基にサンプラを作成してください。
 */
void GetSamplerInfo(::nn::gfx::SamplerInfo* pInfo) NN_NOEXCEPT;

/**
* @brief 体無しアイコン描画に必要なパラメータを取得します。
* @param[out] pInfo 情報格納先
*/
void GetIconInfo(IconInfo* pInfo) NN_NOEXCEPT;

/**
* @brief 体有りアイコン描画に必要なパラメータを取得します。
* @param[out] pInfo 情報格納先
*/
void GetIconBodyInfo(IconBodyInfo* pInfo) NN_NOEXCEPT;

/**
 * @brief 動的に生成されるテクスチャをレンダリングするためのカラーターゲットのフォーマットを取得
 * @param[in] textureFormat テクスチャのフォーマット
 * @param[in] pDevice       デバイスへのポインタ
 * @return 受け取ったテクスチャフォーマットを元にカラーターゲットのフォーマットを返します。
 * @pre     textureFormat がテクスチャとして使用可能なフォーマット
 *          @n && pDevice != nullptr
 *          @n && テクスチャターゲットのフォーマットがUnormSrgbかUnormである必要があります。
 * @details UnormSrgb 形式のイメージフォーマットを渡した場合、
 *          @n Unorm 形式のフォーマットに変換します。Unorm の場合はそのままです。
 *          @n 動的に生成するテクスチャ(MaskやFacelineテクスチャ)は
 *          @n Srgb空間で取り扱う必要があるため、
 *          @n UnormSrgb フォーマットのテクスチャに対してレンダリングする場合でも
 *          @n カラーターゲットは Unorm にする必要があります。
 */
nn::gfx::ImageFormat GetDynamicTextureColorTargetFormat(nn::gfx::ImageFormat textureFormat, nn::gfx::Device* pDevice);

//! @}

}}
