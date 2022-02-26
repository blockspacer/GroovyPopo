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

#include <nn/util/util_MathTypes.h>
#include <nn/util/util_Vector.h>

#include <nn/ui2d/ui2d_Types.h>
#include <nn/ui2d/ui2d_TexMap.h>
#include <nn/font/font_ResourceFormat.h>

namespace nn
{
namespace ui2d
{
namespace detail
{
class PaneEffect;
class VectorGraphicsTexture;
}

class ShaderInfo;
class CaptureTexture;

/* ========================================================================
    定数定義
   ======================================================================== */

//! @name バージョン
//@{

//! @brief リソースのメジャーバージョンです。
//!
//! @details
//! 互換性のない変更を表します。
//!
const uint8_t     MajorVersion  = 9;

//! @brief リソースのマイナーバージョンです。
//!
//! @details
//! 後方互換のある変更を表します。新しいライブラリは古いリソースを解釈できます。
//!
const uint8_t     MinorVersion  = 0;

//! @brief リソースのマイクロバージョンです。
//!
//! @details
//! 両互換の変更を表します。新旧のライブラリでお互いのリソースを解釈できます。
//!
const uint8_t     MicroVersion  = 0;

//! @brief リソースのバグフィックスバージョンです。
//!
//! @details
//! 前方互換のみの変更を表します。新しいライブラリで古いリソースが解釈できないことを表します。
//!
const uint8_t     BugfixVersion = 0;

//! @brief リソースのバージョンです。
const uint32_t    BinaryFileFormatVersion     = NN_DETAIL_FONT_MAKE_VERSION(MajorVersion, MinorVersion, MicroVersion, BugfixVersion);

//! @brief bfcpx のバージョンです。
const uint32_t    BinaryCpxFileFormatVersion     = NN_DETAIL_FONT_MAKE_VERSION(9, 3, 0, 0);

//@}

//! @name レイアウト・リソース
//@{

//! @brief レイアウト・リソースファイルのシグニチャです。
//! @sa ResLayoutFile
const uint32_t    FileSignatureFlyt          = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'Y', 'T');

//! @brief ResLayout データブロックのシグニチャです。
const uint32_t    DataBlockKindLayout        = NN_DETAIL_FONT_MAKE_SIGWORD('l', 'y', 't', '1');
//! @brief ResControl データブロックのシグニチャです。
const uint32_t    DataBlockKindControl       = NN_DETAIL_FONT_MAKE_SIGWORD('c', 'n', 't', '1');
//! @brief ResPaneBegin データブロックのシグニチャです。
const uint32_t    DataBlockKindPaneBegin     = NN_DETAIL_FONT_MAKE_SIGWORD('p', 'a', 's', '1');
//! @brief ResPaneEnd データブロックのシグニチャです。
const uint32_t    DataBlockKindPaneEnd       = NN_DETAIL_FONT_MAKE_SIGWORD('p', 'a', 'e', '1');
//! @brief ResPane データブロックのシグニチャです。
const uint32_t    DataBlockKindPane          = NN_DETAIL_FONT_MAKE_SIGWORD('p', 'a', 'n', '1');
//! @brief ResPicture データブロックのシグニチャです。
const uint32_t    DataBlockKindPicture       = NN_DETAIL_FONT_MAKE_SIGWORD('p', 'i', 'c', '1');
//! @brief ResTextBox データブロックのシグニチャです。
const uint32_t    DataBlockKindTextBox       = NN_DETAIL_FONT_MAKE_SIGWORD('t', 'x', 't', '1');
//! @brief ResWindow データブロックのシグニチャです。
const uint32_t    DataBlockKindWindow        = NN_DETAIL_FONT_MAKE_SIGWORD('w', 'n', 'd', '1');
//! @brief ResBounding データブロックのシグニチャです。
const uint32_t    DataBlockKindBounding      = NN_DETAIL_FONT_MAKE_SIGWORD('b', 'n', 'd', '1');
//! @brief ResParts データブロックのシグニチャです。
const uint32_t    DataBlockKindParts         = NN_DETAIL_FONT_MAKE_SIGWORD('p', 'r', 't', '1');
//! @brief ResCaptureデータブロックのシグニチャです。
const uint32_t    DataBlockKindCapture       = NN_DETAIL_FONT_MAKE_SIGWORD('c', 'p', 't', '1');
//! @brief ResAlignmentデータブロックのシグニチャです。
const uint32_t    DataBlockKindAlignment     = NN_DETAIL_FONT_MAKE_SIGWORD('a', 'l', 'i', '1');
//! @brief ResScissorデータブロックのシグニチャです。
const uint32_t    DataBlockKindScissor       = NN_DETAIL_FONT_MAKE_SIGWORD('s', 'c', 'r', '1');
//! @brief ResGroupBegin データブロックのシグニチャです。
const uint32_t    DataBlockKindGroupBegin    = NN_DETAIL_FONT_MAKE_SIGWORD('g', 'r', 's', '1');
//! @brief ResGroupEnd データブロックのシグニチャです。
const uint32_t    DataBlockKindGroupEnd      = NN_DETAIL_FONT_MAKE_SIGWORD('g', 'r', 'e', '1');
//! @brief ResGroup データブロックのシグニチャです。
const uint32_t    DataBlockKindGroup         = NN_DETAIL_FONT_MAKE_SIGWORD('g', 'r', 'p', '1');
//! @brief ResFontList データブロックのシグニチャです。
const uint32_t    DataBlockKindFontList      = NN_DETAIL_FONT_MAKE_SIGWORD('f', 'n', 'l', '1');
//! @brief ResTextureList データブロックのシグニチャです。
const uint32_t    DataBlockKindTextureList   = NN_DETAIL_FONT_MAKE_SIGWORD('t', 'x', 'l', '1');
//! @brief ResMaterialList データブロックのシグニチャです。
const uint32_t    DataBlockKindMaterialList  = NN_DETAIL_FONT_MAKE_SIGWORD('m', 'a', 't', '1');
//! @brief ResShapeInfoList データブロックのシグニチャです。
const uint32_t    DataBlockKindShapeInfoList = NN_DETAIL_FONT_MAKE_SIGWORD('s', 'p', 'i', '1');
//! @brief ResCaptureTextureList データブロックのシグニチャです。
const uint32_t    DataBlockKindCaptureTextureList = NN_DETAIL_FONT_MAKE_SIGWORD('c', 't', 'l', '1');
//! @brief ResExtUserDataList データブロックのシグニチャです。
const uint32_t    DataBlockKindUserDataList  = NN_DETAIL_FONT_MAKE_SIGWORD('u', 's', 'd', '1');
//! @brief ResStateMachine データブロックのシグニチャです。
const uint32_t    DataBlockKindStateMachine = NN_DETAIL_FONT_MAKE_SIGWORD('s', 't', 'm', '1');
//! @brief ResVectorGraphicsTextureList データブロックのシグニチャです。
const uint32_t    DataBlockKindVectorGraphicsTextureList = NN_DETAIL_FONT_MAKE_SIGWORD('v', 'g', 'l', '1');

//@}

//! @name アニメーション・リソース
//@{

//! @brief アニメーション・リソースファイルのシグニチャです。
//! @sa ResLayoutAnimationFile
const uint32_t    FileSignatureFlan          = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'A', 'N');

//! @brief ResAnimationTagBlock データブロックのシグニチャです。
const uint32_t    DataBlockKindPaneAnimTag   = NN_DETAIL_FONT_MAKE_SIGWORD('p', 'a', 't', '1');
//! @brief ResAnimationShareBlock データブロックのシグニチャです。
const uint32_t    DataBlockKindPaneAnimShare = NN_DETAIL_FONT_MAKE_SIGWORD('p', 'a', 'h', '1');
//! @brief ResAnimationBlock データブロックのシグニチャです。
const uint32_t    DataBlockKindPaneAnimInfo  = NN_DETAIL_FONT_MAKE_SIGWORD('p', 'a', 'i', '1');

//@}

//! @name アニメーション・タイプ
//@{

//! @brief アニメーションの対象はペインのSrtアニメーションです。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypePaneSrt       = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'P', 'A');
//! @brief アニメーションの対象はビジビリティ・アニメーションです。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypeVisibility    = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'V', 'I');
//! @brief アニメーションの対象は頂点カラー・アニメーションです。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypeVertexColor      = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'V', 'C');
//! @brief アニメーションの対象はマテリアル・アニメーションです。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypeMaterialColor      = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'M', 'C');
//! @brief アニメーションの対象はテクスチャのSrtアニメーションです。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypeTextureSrt        = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'T', 'S');
//! @brief アニメーションの対象はテクスチャパターン・アニメーションです。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypeTexturePattern    = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'T', 'P');
//! @brief アニメーションの対象はインダイレクトのSrtです。(現状Transはサポートしていません)
//! @sa ResAnimationInfo
const uint32_t    AnimationTypeIndirectSrt   = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'I', 'M');
//! @brief アニメーションの対象はアルファコンペアです。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypeAlphaCompare   = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'A', 'C');
//! @brief アニメーションの対象はフォント影のパラメータです。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypeFontShadow   = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'F', 'S');
//! @brief アニメーションの対象は文字単位アニメーションのパラメータです。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypePerCharacterTransform   = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'C', 'T');
//! @brief アニメーションの対象は文字単位アニメーションカーブのパラメータです。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypePerCharacterTransformCurve    = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'C', 'C');
//! @brief アニメーションの対象はWindowペインです。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypeWindow = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'W', 'N');
//! @brief アニメーションの対象は拡張ユーザーデータアニメーションです。
//! @sa AnimationInfo
const uint32_t    AnimationTypeExtUserData   = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'E', 'U');
//! @brief アニメーションの対象はマスクテクスチャ SRT アニメーションです。
//! @sa AnimationInfo
const uint32_t    AnimationTypeMaskTexSrt   = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'M', 'T');
//! @brief アニメーションの対象はドロップシャドウパラメータアニメーションです。
//! @sa AnimationInfo
const uint32_t    AnimationTypeDropShadow   = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'D', 'S');
//! @brief アニメーションの対象は任意形状パラメータアニメーションです。
//! @sa AnimationInfo
const uint32_t    AnimationTypeProceduralShape = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'P', 'S');
//! @brief アニメーションの対象はステートマシンです。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypeStateMachine = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'S', 'M', 'A');

//! @brief アニメーションの対象はステートマシンです。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypePartsStateMachine = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'P', 'S', 'M');

//! @brief アニメーションの対象はベクターグラフィックステクスチャ情報です。
//! @sa ResAnimationInfo
const uint32_t    AnimationTypeVectorGraphicsTextureInfo = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'V', 'G', 'A');
//@}

//! @name テクスチャイメージ・リソース
//@{

//! @brief テクスチャイメージ・リソースファイルのシグニチャです。
//! @sa ResLayoutImageFile
const uint32_t    FileSignatureFlim          = NN_DETAIL_FONT_MAKE_SIGWORD('F', 'L', 'I', 'M');

//! @brief ResImage データブロックのシグニチャです。
const uint32_t    DataBlockKindImage         = NN_DETAIL_FONT_MAKE_SIGWORD('i', 'm', 'a', 'g');

//@}

//! @name 複合フォントリソースファイル
//@{

//! @brief 複合フォントリソースファイルシグニチャです。
const uint32_t BinFileSignatureFcpx              = NN_DETAIL_FONT_MAKE_SIGWORD('F','C','P','X');

//@}

//! @name ベクターグラフィックスバイナリリソース(bnvg)
//@{

const int64_t BnvgBinaryFileSigniture = NN_UTIL_CREATE_SIGNATURE_8('B', 'N', 'V', 'G', 0, 0, 0, 0);
//! @brief Bnvg ファイルのコンポジションブロックのシグニチャです。
const uint32_t BnvgBinaryBlockCompositionSinature = NN_UTIL_CREATE_SIGNATURE_4('V', 'G', 'C', 'P');

//@}

//! @name リソースタイプ
//@{

//! @brief リソースはレイアウトです。
//! @sa ResType
const uint32_t    ResourceTypeLayout         = 'blyt';
//! @brief リソースはアニメーションです。
//! @sa ResType
const uint32_t    ResourceTypeAnimation      = 'anim';
//! @brief リソースはテクスチャイメージです。
//! @sa ResType
const uint32_t    ResourceTypeTexture        = 'timg';
//! @brief リソースはリソースフォントです。
//! @sa ResType
const uint32_t    ResourceTypeFont           = 'font';
//! @brief リソースはアーカイブフォントです。
//! @sa ResType
const uint32_t    ResourceTypeArchiveFont    = 'fnta';
//! @brief リソースはシェーダイメージです。
//! @sa ResType
const uint32_t    ResourceTypeShader         = 'bgsh';
//! @brief リソースは複合フォントです。
//! @sa ResType
const uint32_t    ResourceTypeComplexFont    = 'fcpx';
//! @brief リソースはスケーラブルフォントです。
//! @sa ResType
const uint32_t    ResourceTypeScalableFont    = 'scft';
//! @brief リソースはベクターグラフィックスデータです。
//! @sa ResType
const uint32_t    ResourceTypeVectorGraphics         = 'bnvg';
//@}

// シェーダバリエーションテーブル内のバリエーションの種類を示すシグネチャ
const uint32_t ShaderSignatureDetailedCombiner      = NN_DETAIL_FONT_MAKE_SIGWORD('D', 'T', 'C', 'B');
const uint32_t ShaderSignatureCombinerUserShader    = NN_DETAIL_FONT_MAKE_SIGWORD('C', 'B', 'U', 'S');
const uint32_t ShaderSignatureNormalCombinerShader  = NN_DETAIL_FONT_MAKE_SIGWORD('N', 'O', 'R', 'M');
const uint32_t ShaderSignatureMask                  = NN_DETAIL_FONT_MAKE_SIGWORD('M', 'A', 'S', 'K');
const uint32_t ShaderSignatureDropShadow            = NN_DETAIL_FONT_MAKE_SIGWORD('D', 'R', 'S', 'H');
const uint32_t ShaderSignatureVectorGraphics        = NN_DETAIL_FONT_MAKE_SIGWORD('V', 'T', 'G', 'R');

/* ========================================================================
    型定義
   ======================================================================== */

//!--------------------------------------------------------------------------*
//! @name レイアウト・リソース
//@{

//---------------------------------------------------------------------------
//! @brief リソース内で２次元ベクトルを保持します。
//!
//---------------------------------------------------------------------------
struct ResVec2
{
    //! @brief 値を設定します。
    //!
    //! @param[in] xValue   X座標の値です。
    //! @param[in] yValue   Y座標の値です。
    //!
    void Set(float xValue, float yValue)
    {
        this->x = xValue;
        this->y = yValue;
    }

    //! @brief 変換オペレータです。
    //!
    NN_EXPLICIT_OPERATOR const nn::util::Float2() const
    {
        const nn::util::Float2 result = NN_UTIL_FLOAT_2_INITIALIZER(x, y);
        return result;
    }

    //! X座標の値です。
    float x;
    //! Y座標の値です。
    float y;
};

//---------------------------------------------------------------------------
//! @brief リソース内で３次元ベクトルを保持します。
//!
//---------------------------------------------------------------------------
struct ResVec3
{
    //! @brief 値を設定します。
    //!
    //! @param[in] xValue   X座標の値です。
    //! @param[in] yValue   Y座標の値です。
    //! @param[in] zValue   Z座標の値です。
    //!
    void Set(float xValue, float yValue, float zValue)
    {
        this->x = xValue;
        this->y = yValue;
        this->z = zValue;
    }

    //@}

    //! @brief 変換オペレータです。
    //!
    NN_EXPLICIT_OPERATOR const nn::util::Vector3fType() const
    {
        nn::util::Vector3fType result;
        nn::util::VectorSet(&result, x, y, z);
        return result;
    }

    //! X座標の値です。
    float x;

    //! Y座標の値です。
    float y;

    //! Z座標の値です。
    float z;
};

//---------------------------------------------------------------------------
//! @brief テクスチャ SRT 変換行列の構造体です。
//!
//---------------------------------------------------------------------------
struct ResTexSrt // 以降の型は、nn::font::detail::Res を使わないようにして、POD になるので修正しません。
{
    //! 平行移動です。
    ResVec2 translate;
    //! 回転です。
    float rotate;
    //! 拡大率です。
    ResVec2 scale;
};

//---------------------------------------------------------------------------
//! @brief 投影テクスチャ座標生成の構造体です。
//!
//---------------------------------------------------------------------------
struct ResProjectionTexGenParameters
{
    //! 平行移動です。
    ResVec2 translate;
    //! 拡大率です。
    ResVec2 scale;

    //! フラグです(TexProjectionFlag)。
    uint8_t   flag;
    //! （未使用）
    uint8_t   reserve[3];
};

//---------------------------------------------------------------------------
//! @brief テクスチャ座標生成に設定する値を持つ構造体です。
//!
//---------------------------------------------------------------------------
class ResTexCoordGen
{
public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    ResTexCoordGen()
    {
        Set(TexGenType_Mtx2x4, TexGenSrc_Tex0);
        m_AddressOfProjectionTexGenParams = 0;
    }

    //! @brief コンストラクタです。
    //!
    //! @param[in] aTexGenType  テクスチャ座標の生成方法です。
    //! @param[in] aTexGenSrc   テクスチャ座標のソースです。
    //!
    ResTexCoordGen(
        TexGenType      aTexGenType,
        TexGenSrc       aTexGenSrc
    )
    {
        Set(aTexGenType, aTexGenSrc);
        m_AddressOfProjectionTexGenParams = 0;
    }

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief テクスチャ座標生成の設定を変更します。
    //!
    //! @param[in] aTexGenType  テクスチャ座標の生成方法です。
    //! @param[in] aTexGenSrc   テクスチャ座標のソースです。
    //!
    void Set(
        TexGenType    aTexGenType,
        TexGenSrc     aTexGenSrc
                        )
    {
        m_TexGenType = static_cast<uint8_t>(aTexGenType);
        m_TexGenSrc = static_cast<uint8_t>(aTexGenSrc);
        std::memset(padding, 0, sizeof(padding));
    }

    //! @brief テクスチャ座標の生成方法を取得します。
    //!
    //! @return テクスチャ座標の生成方法を返します。
    //!
    TexGenType GetTexGenType() const
    {
        return TexGenType(m_TexGenType);
    }

    //! @brief テクスチャ座標のソースを取得します。
    //!
    //! @return テクスチャ座標のソースを返します。
    //!
    TexGenSrc GetTexGenSrc() const
    {
        return TexGenSrc(m_TexGenSrc);
    }

    //! @brief 投影テクスチャ座標かどうかを取得します。
    //!
    //! @return 投影テクスチャなら true を返します。
    //!
    bool IsProjection() const
    {
        const TexGenSrc texGenSource = GetTexGenSrc();
        return texGenSource == TexGenSrc_OrthoProjection ||
            texGenSource == TexGenSrc_PaneBaseOrthoProjection ||
            texGenSource == TexGenSrc_PerspectiveProjection ||
            texGenSource == TexGenSrc_PaneBasePerspectiveProjection;
    }

    //! @brief 透視投影テクスチャ座標かどうかを取得します。
    //!
    //! @return 透視投影テクスチャなら true を返します。
    //!
    bool IsPerspectiveProjection() const
    {
        const TexGenSrc texGenSource = GetTexGenSrc();
        return texGenSource == TexGenSrc_PerspectiveProjection ||
            texGenSource == TexGenSrc_PaneBasePerspectiveProjection;
    }


    //! @brief 投影テクスチャパラメータを取得します。
    //!
    //! @param[in] pParam   投影テクスチャパラメータです。
    //!
    void SetProjectionTexGenParameters(const ResProjectionTexGenParameters* pParam)
    {
        m_AddressOfProjectionTexGenParams = reinterpret_cast<uint64_t>(pParam);
    }

    //@}

private:
    //! テクスチャ座標生成のタイプです。
    uint8_t           m_TexGenType;
    //! テクスチャ座標生成に使用するソースです。
    uint8_t           m_TexGenSrc;
    //! （未使用）
    char padding[2];

    // m_AddressOfProjectionTexGenParams は、Material::GetProjectionTexGenArray 内で
    // アドレス演算により直接参照されます。
    //! 投影テクスチャ座標生成のアドレスです。(初期化時に設定されます。)
    uint64_t                          m_AddressOfProjectionTexGenParams;
};

//---------------------------------------------------------------------------
//! @brief TEV ステージの設定を持つ構造体です。
//!
//---------------------------------------------------------------------------
class ResTevStage
{
public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    ResTevStage()
    {
        Set(
            TevMode_Replace,
            TevMode_Replace);
    }

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief すべての項目を設定します。
    //!
    //! @param[in] combineRgb      RGBの演算のタイプです。
    //! @param[in] combineAlpha    アルファの演算のタイプです。
    //!
    void Set(
        TevMode         combineRgb,
        TevMode         combineAlpha
    )
    {
        m_CombineRgb = static_cast<uint8_t>(combineRgb);
        m_CombineAlpha = static_cast<uint8_t>(combineAlpha);
        std::memset(padding, 0, sizeof(padding));
    }

    //! @brief RGBの演算のタイプを設定します。
    //!
    //! @param[in] value    設定する値です。
    //!
    void SetCombineRgb(TevMode value)
    {
        m_CombineRgb = static_cast<uint8_t>(value);
    }

    //! @brief アルファの演算のタイプを設定します。
    //!
    //! @param[in] value    設定する値です。
    //!
    void SetCombineAlpha(TevMode value)
    {
        m_CombineAlpha = static_cast<uint8_t>(value);
    }

    //! @brief RGBの演算のタイプを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    TevMode GetCombineRgb() const
    {
        return static_cast<TevMode>(m_CombineRgb);
    }

    //! @brief アルファの演算のタイプを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    TevMode GetCombineAlpha() const
    {
        return static_cast<TevMode>(m_CombineAlpha);
    }
    //@}

private:
    uint8_t   m_CombineRgb;
    uint8_t   m_CombineAlpha;
    //! （未使用）
    char padding[2];

};

//---------------------------------------------------------------------------
//! @brief アルファコンペアの設定を持つ構造体です。
//!
//---------------------------------------------------------------------------
class ResAlphaCompare
{
public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 常に通過する設定で初期化します。
    //!
    ResAlphaCompare()
    {
        Set(AlphaTest_Always, 0.f);
    }

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 指定の設定で初期化します。
    //!
    //! @param[in] aFunc    比較の条件です。
    //! @param[in] aRef     比較の対象値です。
    //!
    ResAlphaCompare(
        AlphaTest   aFunc,
        float         aRef
    )
    {
        Set(aFunc, aRef);
    }

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief 設定を変更します
    //!
    //! @param[in] aFunc    比較の条件です。
    //! @param[in] aRef     比較の対象値です。
    //!
    void Set(
        AlphaTest   aFunc,
        float         aRef
    )
    {
        NN_SDK_ASSERT(0 <= aFunc && aFunc < AlphaTest_MaxAlphaTest, "out of bounds: 0 <= aFunc[%d] < AlphaTest_MaxAlphaTest", aFunc);

        m_AlphaCompareFunc = uint8_t (aFunc);
        m_AlphaRef = aRef;
    }

    //! @brief 設定を変更します
    //!
    //! @param[in] aRef 比較の対象値です。
    //!
    void SetRef(
        float         aRef
    )
    {
        m_AlphaRef = aRef;
    }

    //! @brief アルファ比較の条件を取得します。
    //!
    //! @return 設定値を返します。
    //!
    AlphaTest GetFunc() const
    {
        return AlphaTest(m_AlphaCompareFunc);
    }

    //! @brief アルファ比較の対象の値を取得します。
    //!
    //! @return 設定値を返します。
    //!
    float GetRef() const
    {
        return m_AlphaRef;
    }

    //@}

private:
    uint8_t  m_AlphaCompareFunc;
    float m_AlphaRef;
};

//---------------------------------------------------------------------------
//! @brief ブレンドモードの設定を持つ構造体です。
//!
//---------------------------------------------------------------------------
class ResBlendMode
{
public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! ブレンドを行わない(上書き)で初期化します。
    //!
    ResBlendMode()
    {
        Set(
            BlendOp_Disable,
            BlendFactor_SrcAlpha,
            BlendFactor_InvSrcAlpha,
            LogicOp_Disable);
    }

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 指定の設定で初期化します。
    //!
    //! @param[in] aBlendOp     ブレンド演算のタイプを指定します。
    //! @param[in] aSrcFactor   ピクセルカラーに乗算する値を指定します。
    //! @param[in] aDstFactor   フレームバッファカラーに乗算する値を指定します。
    //! @param[in] aLogicOp     論理演算のタイプを指定します。
    //!
    ResBlendMode(
        BlendOp         aBlendOp,
        BlendFactor     aSrcFactor,
        BlendFactor     aDstFactor,
        LogicOp         aLogicOp
    )
    {
        Set(aBlendOp, aSrcFactor, aDstFactor, aLogicOp);
    }

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief 設定を変更します。
    //!
    //! @param[in] aBlendOp     ブレンド演算のタイプを指定します。
    //! @param[in] aSrcFactor   ピクセルカラーに乗算する値を指定します。
    //! @param[in] aDstFactor   フレームバッファカラーに乗算する値を指定します。
    //! @param[in] aLogicOp     論理演算のタイプを指定します。
    //!
    void Set(
        BlendOp         aBlendOp,
        BlendFactor     aSrcFactor,
        BlendFactor     aDstFactor,
        LogicOp         aLogicOp
    )
    {
        m_BlendOperation = uint8_t (aBlendOp);
        m_SrcBlendFactor = uint8_t (aSrcFactor);
        m_DestBlendFactor = uint8_t (aDstFactor);
        m_LogicOp = uint8_t (aLogicOp);
    }

    //! @brief ブレンド演算がDisableかどうかを取得します。
    //!
    //! @return ブレンド演算がDisableかどうかです。
    //!
    bool IsBlendOpDisable() const
    {
        return static_cast<BlendOp>(m_BlendOperation) == BlendOp_Disable;
    }

    //! @brief ブレンド演算のタイプを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    BlendOp GetBlendOp() const
    {
        return static_cast<BlendOp>(m_BlendOperation);
    }

    //! @brief ブレンド演算のタイプを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    nn::gfx::BlendFunction GetGfxBlendOp() const
    {
        switch(static_cast<BlendOp>(m_BlendOperation))
        {
            case BlendOp_Add: return nn::gfx::BlendFunction_Add;
            case BlendOp_Subtract: return nn::gfx::BlendFunction_Subtract;
            case BlendOp_ReverseSubtract: return nn::gfx::BlendFunction_ReverseSubtract;
            case BlendOp_SelectMin: return nn::gfx::BlendFunction_Min;
            case BlendOp_SelectMax: return nn::gfx::BlendFunction_Max;
            default:NN_SDK_ASSERT(false, "Wrong blend function."); return nn::gfx::BlendFunction_Add;
        }
    }

    //! @brief ピクセルカラーに乗算する値を取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    BlendFactor GetSrcFactor() const
    {
        return static_cast<BlendFactor>(m_SrcBlendFactor);
    }

    //! @brief ピクセルカラーに乗算する値を取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    nn::gfx::BlendFactor GetGfxSrcFactor() const
    {
        return ConvertBlendFactor(static_cast<BlendFactor>(m_SrcBlendFactor));
    }

    //! @brief フレームバッファカラーに乗算する値を取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    BlendFactor GetDstFactor() const
    {
        return static_cast<BlendFactor>(m_DestBlendFactor);
    }

    //! @brief フレームバッファカラーに乗算する値を取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    nn::gfx::BlendFactor GetGfxDstFactor() const
    {
        return ConvertBlendFactor(static_cast<BlendFactor>(m_DestBlendFactor));
    }

    //! @brief 論理演算がDisableかどうかを取得します。
    //!
    //! @return 論理演算がDisableかどうかです。
    //!
    bool IsLogicOpDisable() const
    {
        return static_cast<LogicOp>(m_LogicOp) == LogicOp_Disable;
    }

    //! @brief 論理演算のタイプを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    LogicOp GetLogicOp() const
    {
        return static_cast<LogicOp>(m_LogicOp);
    }

    //! @brief 論理演算のタイプを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    nn::gfx::LogicOperation GetGfxLogicOp() const
    {
        switch(static_cast<LogicOp>(m_LogicOp))
        {
            case LogicOp_Noop: return nn::gfx::LogicOperation_NoOp;
            case LogicOp_Clear: return nn::gfx::LogicOperation_Clear;
            case LogicOp_Set: return nn::gfx::LogicOperation_Set;
            case LogicOp_Copy: return nn::gfx::LogicOperation_Copy;
            case LogicOp_InvCopy: return nn::gfx::LogicOperation_CopyInverted;
            case LogicOp_Inv: return nn::gfx::LogicOperation_Invert;
            case LogicOp_And: return nn::gfx::LogicOperation_And;
            case LogicOp_Nand: return nn::gfx::LogicOperation_Nand;
            case LogicOp_Or: return nn::gfx::LogicOperation_Or;
            case LogicOp_Nor: return nn::gfx::LogicOperation_Nor;
            case LogicOp_Xor: return nn::gfx::LogicOperation_Xor;
            case LogicOp_Equiv: return nn::gfx::LogicOperation_Equiv;
            case LogicOp_RevAnd: return nn::gfx::LogicOperation_AndReverse;
            case LogicOp_InvAnd: return nn::gfx::LogicOperation_AndInverted;
            case LogicOp_RevOr: return nn::gfx::LogicOperation_OrReverse;
            case LogicOp_InvOr: return nn::gfx::LogicOperation_OrInverted;
            default: NN_SDK_ASSERT(false, "Wrong logic operation."); return nn::gfx::LogicOperation_Clear;
        }
    }

    //! @brief 別の ResBlendMode と一致しているかどうかを判定します。
    //!
    //! @param[in] pBlendMode 別の ResBlendMode です。
    //!
    //! @return 別の ResBlendMode と一致している場合は true が返ります。
    //!
    bool Equals(const ResBlendMode* pBlendMode) const
    {
        return m_BlendOperation == pBlendMode->m_BlendOperation &&
            m_SrcBlendFactor == pBlendMode->m_SrcBlendFactor &&
            m_DestBlendFactor == pBlendMode->m_DestBlendFactor &&
            m_LogicOp == pBlendMode->m_LogicOp;
    }

    //@}

private:
    nn::gfx::BlendFactor ConvertBlendFactor(BlendFactor blendFactor) const
    {
        switch(blendFactor)
        {
            case BlendFactor_0: return nn::gfx::BlendFactor_Zero;
            case BlendFactor_1: return nn::gfx::BlendFactor_One;
            case BlendFactor_DstColor: return nn::gfx::BlendFactor_DestinationColor;
            case BlendFactor_InvDstColor: return nn::gfx::BlendFactor_OneMinusDestinationColor;
            case BlendFactor_SrcAlpha: return nn::gfx::BlendFactor_SourceAlpha;
            case BlendFactor_InvSrcAlpha: return nn::gfx::BlendFactor_OneMinusSourceAlpha;
            case BlendFactor_DstAlpha: return nn::gfx::BlendFactor_DestinationAlpha;
            case BlendFactor_InvDstAlpha: return nn::gfx::BlendFactor_OneMinusDestinationAlpha;
            case BlendFactor_SrcColor: return nn::gfx::BlendFactor_SourceColor;
            case BlendFactor_InvSrcColor: return nn::gfx::BlendFactor_OneMinusSourceColor;
            default:NN_SDK_ASSERT(false, "Wrong blend factor."); return nn::gfx::BlendFactor_Zero;
        }
    }

    uint8_t           m_BlendOperation;
    uint8_t           m_SrcBlendFactor;
    uint8_t           m_DestBlendFactor;
    uint8_t           m_LogicOp;
};

//---------------------------------------------------------------------------
//! @brief インダイレクトのパラメータを持つ構造体です。
//!
//---------------------------------------------------------------------------
class ResIndirectParameter
{
public:// TODO:アクセサ
    float      rotate;     //!< ローテート値です。
    ResVec2                scale;      //!< スケール値です。

    ResIndirectParameter() : rotate(0.f)
    {
        scale.Set(1.0f, 1.0f);
    }
};

//---------------------------------------------------------------------------
//! @brief フォント影のパラメータを持つ構造体です。
//!
//---------------------------------------------------------------------------
class ResFontShadowParameter
{
public:// TODO:アクセサ
    uint8_t blackInterporateColor[3]; //!< 黒補間色です。
    uint8_t whiteInterporateColor[4]; //!< 白補間色です。
    uint8_t reserve; //!<（未使用）

    ResFontShadowParameter()
    {
        memset(blackInterporateColor, 0, sizeof(blackInterporateColor));
        memset(whiteInterporateColor, 255, sizeof(whiteInterporateColor));
        reserve = 0;
    }
};

//---------------------------------------------------------------------------
//! @brief ペインの各辺の拡大量を持つ構造体です。
//!
//---------------------------------------------------------------------------
struct ResWindowInflation
{
    int16_t          left; //!< 左辺の拡大量です。固定少数(WindowInflationFixedPoint)です。
    int16_t          right; //!< 右辺の拡大量です。固定少数(WindowInflationFixedPoint)です。
    int16_t          top; //!< 上辺の拡大量です。固定少数(WindowInflationFixedPoint)です。
    int16_t          bottom; //!< 下辺の拡大量です。固定少数(WindowInflationFixedPoint)です。
};

//---------------------------------------------------------------------------
//! @brief ウィンドウフレームサイズを持つ構造体です。
//!
//---------------------------------------------------------------------------
struct ResWindowFrameSize
{
    uint16_t          left; //!< 左辺のサイズです。
    uint16_t          right; //!< 右辺のサイズです。
    uint16_t          top; //!< 上辺のサイズです。
    uint16_t          bottom; //!< 下辺のサイズです。
};

//---------------------------------------------------------------------------
//! @brief  システム用拡張ユーザーデータ構造体です。
//!
//! この構造体には以下のデータ構造が続きます。
//!
//! ・ uint32_t  offsets[count]  4 バイトアラインメント
//---------------------------------------------------------------------------
struct ResSystemExtUserData
{
    uint16_t        version;    //!< バイナリバージョンです。
    uint16_t        count;      //!< 含まれているデータ数です。
};

//---------------------------------------------------------------------------
//! @brief 拡張ユーザデータクラスです。
//!
//! @details
//! 名前文字列とデータがオブジェクトに続いて格納されます。
//!
//! @sa ResPane
//! @sa ResExtUserDataList
//!
//---------------------------------------------------------------------------
class ResExtUserData
{
public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @param[in] nameStrOffset    拡張ユーザデータの名前へのオフセットです(オブジェクト先頭から)。
    //! @param[in] dataOffset       拡張ユーザデータへのオフセットです(オブジェクト先頭から)。
    //! @param[in] count            拡張ユーザデータの個数です。
    //! @param[in] type             拡張ユーザデータのデータタイプです。
    //!
    ResExtUserData(uint32_t  nameStrOffset, uint32_t  dataOffset, uint16_t  count, uint8_t  type)
        : m_NameStrOffset(nameStrOffset),
          m_DataOffset(dataOffset),
          m_Count(count),
          m_Type(type)
    {
        // コンパイラ警告([-Wunused-private-field])抑止のため、コンストラクタ内で初期化しています。
        m_Padding1 = 0;
    }

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief 拡張ユーザデータの名前を取得します。
    //!
    //! @return 拡張ユーザデータに設定されている名前を返します。
    //!
    const char* GetName() const
    {
        return m_NameStrOffset ? nn::util::ConstBytePtr(this, m_NameStrOffset).Get<const char>(): 0;
    }

    //! @brief 拡張ユーザデータの型を取得します。
    //!
    //! @details
    //! 文字列、整数配列、浮動小数点数配列のいずれかになります。
    //!
    //! @return データタイプを返します。
    //!
    ExtUserDataType GetType() const
    {
        return ExtUserDataType(m_Type);
    }

    //! @brief 拡張ユーザデータの個数を取得します。
    //!
    //! @details
    //! データのタイプが文字列の場合は、文字列の長さになります。
    //!
    //! データのタイプが整数配列の場合は、配列の要素数になります。
    //!
    //! データのタイプが浮動小数点数配列の場合は、配列の要素数になります。
    //!
    //! @return データの個数を返します。
    //!
    uint16_t  GetCount() const
    {
        return m_Count;
    }

    //! @brief 拡張ユーザデータを文字列として取得します。
    //!
    //! @details
    //! データのタイプが文字列である場合に有効です。
    //!
    //! @return 文字列のポインタを返します。
    //!
    const char* GetString() const
    {
        NN_SDK_ASSERT(GetType() == ExtUserDataType_String, "GetType()[%d] must be equal to ExtUserDataType_String", GetType());
        return nn::util::ConstBytePtr(this, m_DataOffset).Get<const char>();
    }

    //! @brief 拡張ユーザデータを整数配列として取得します。
    //!
    //! @details
    //! データのタイプが整数配列である場合に有効です。
    //!
    //! @return int32_t配列の先頭要素のポインタを返します。
    //!
    const int32_t* GetIntArray() const
    {
        NN_SDK_ASSERT(GetType() == ExtUserDataType_Int, "GetType()[%d] must be equal to ExtUserDataType_Int", GetType());
        return nn::util::ConstBytePtr(this, m_DataOffset).Get<const int32_t>();
    }

    //! @brief 拡張ユーザデータを浮動小数点数配列として取得します。
    //!
    //! @details
    //! データのタイプが浮動小数点配列である場合に有効です。
    //!
    //! @return float配列の先頭要素のポインタを返します。
    //!
    const float* GetFloatArray() const
    {
        NN_SDK_ASSERT(GetType() == ExtUserDataType_Float, "GetType()[%d] must be equal to ExtUserDataType_Float", GetType());
        return nn::util::ConstBytePtr(this, m_DataOffset).Get<const float>();
    }

    // システム用拡張ユーザデータのバージョンを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details
    //! データのタイプがシステムデータである場合に有効です。
    //!
    //! @return システム用拡張ユーザーデータのバージョンを取得します。
    //!
    int GetSystemDataVersion() const
    {
        NN_SDK_ASSERT(GetType() == ExtUserDataType_SystemData, "GetType()[%d] must be equal to ExtUserDataType_SystemData", GetType());
        const ResSystemExtUserData* pSystemData = nn::util::ConstBytePtr(this, m_DataOffset).Get<ResSystemExtUserData>();

        return pSystemData->version;
    }

    //  システム用拡張ユーザデータの要素数を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details
    //! データのタイプがシステムデータである場合に有効です。
    //!
    //! @return システム用拡張ユーザーデータの要素数を取得します。
    //!
    int GetSystemDataCount() const
    {
        NN_SDK_ASSERT(GetType() == ExtUserDataType_SystemData, "GetType()[%d] must be equal to ExtUserDataType_SystemData", GetType());
        const ResSystemExtUserData* pSystemData = nn::util::ConstBytePtr(this, m_DataOffset).Get<ResSystemExtUserData>();

        return pSystemData->count;
    }

    // システム用拡張ユーザデータを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  index   取得するデータのインデックス。
    //!
    //! @details
    //! データのタイプがシステムデータである場合に有効です。
    //!
    //! @return システム用拡張ユーザーデータの要素数を取得します。
    //!
    const void* GetSystemData(int index) const
    {
        NN_SDK_ASSERT(GetType() == ExtUserDataType_SystemData, "GetType()[%d] must be equal to ExtUserDataType_SystemData", GetType());
        NN_SDK_ASSERT(index < GetSystemDataCount(), "GetSystemData(%d) must be less than GetSystemDataCount.", index);
        const int* pOffset = nn::util::ConstBytePtr(this, m_DataOffset + sizeof(ResSystemExtUserData) + sizeof(uint32_t) * index).Get<const int>();
        return nn::util::ConstBytePtr(this, m_DataOffset + *pOffset).Get<const void>();
    }


    //! @brief 内部機能のため使用禁止です。
    //!
    //! @details
    //! データのタイプが整数配列である場合に有効です。
    //!
    //! @param[in]  value   書き込む値
    //! @param[in]  index   値を書き込む配列内でのインデックス
    //!
    void WriteIntValue(int32_t value, int index)
    {
        NN_SDK_ASSERT(GetType() == ExtUserDataType_Int, "GetType()[%d] must be equal to ExtUserDataType_Int", GetType());
        NN_SDK_ASSERT(index < m_Count);
        int32_t* pValues = nn::util::BytePtr(this, m_DataOffset).Get<int32_t>();
        pValues[index] = value;
    }

    //! @brief 内部機能のため使用禁止です。
    //!
    //! @details
    //! データのタイプが浮動小数点配列である場合に有効です。
    //!
    //! @param[in]  value   書き込む値
    //! @param[in]  index   値を書き込む配列内でのインデックス
    //!
    void WriteFloatValue(float value, int index)
    {
        NN_SDK_ASSERT(GetType() == ExtUserDataType_Float, "GetType()[%d] must be equal to ExtUserDataType_Float", GetType());
        NN_SDK_ASSERT(index < m_Count);
        float* pValues = nn::util::BytePtr(this, m_DataOffset).Get<float>();
        pValues[index] = value;
    }

    // m_DataOffset の値を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return m_DataOffset の値です。
    //!
    uint32_t GetDataOffset() const
    {
        return m_DataOffset;
    }

    // m_NameOffset の値を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return m_NameOffset の値です。
    //!
    uint32_t GetNameOffset() const
    {
        return m_NameStrOffset;
    }

    //@}

private:
    //! データ名のオブジェクトの先頭を基準にしたオフセットです。
    uint32_t m_NameStrOffset;
    //! データのオブジェクトの先頭を基準にしたオフセットです。
    uint32_t m_DataOffset;

    uint16_t m_Count;
    uint8_t m_Type;
    char                    m_Padding1;
};

//---------------------------------------------------------------------------
//! @brief アニメーション区間タグで指定されるグループの情報です。
//!
//---------------------------------------------------------------------------
struct ResAnimationGroupRef
{
    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief デフォルト値を設定します。
    //!
    void SetDefault()
    {
        flag = 0;
        std::memset(name,    0, sizeof(name));
        std::memset(padding, 0, sizeof(padding));
    }

    //! @brief グループ名を取得します。
    //!
    //! @return グループ名を返します。
    //!
    const char* GetName() const
    {
        return name;
    }

    //@}

    //! グループ名です。
    char                name[GroupNameStrMax + 1];
    //! フラグです。
    uint8_t           flag;
    //! （未使用です）
    char                              padding[2];
};

//---------------------------------------------------------------------------
//! @brief アニメーション共有情報です。
//!
//---------------------------------------------------------------------------
struct ResAnimationShareInfo
{
    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief デフォルト値を設定します。
    //!
    void SetDefault()
    {
        std::memset(srcPaneName,     0, sizeof(srcPaneName));
        std::memset(targetGroupName, 0, sizeof(targetGroupName));
        std::memset(padding,         0, sizeof(padding));
    }

    //! @brief アニメーションの共有元となるペインの名前を取得します。
    //!
    //! @return ペインの名前を返します。
    //!
    const char* GetSrcPaneName() const
    {
        return srcPaneName;
    }

    //! @brief アニメーションの共有対象となるペインが登録されているグループの名前を取得します。
    //!
    //! @return グループの名前を返します。
    //!
    const char* GetTargetGroupName() const
    {
        return targetGroupName;
    }

    //@}

    //! アニメーションの共有元となるペインの名前です。
    char                srcPaneName[ResourceNameStrMax + 1];
    //! アニメーションの共有対象となるペインが登録されているグループの名前です。
    char                targetGroupName[ResourceNameStrMax + 1];
    //! （未使用です）
    char                padding[2];
};

//---------------------------------------------------------------------------
//! @brief レイアウト・リソースファイルのヘッダです。
//!
//! @details
//! fileHeader の kind には FileSignatureFlyt が指定されます。
//!
//! この構造体に以下のデータブロックの組み合わせが続きます。
//!
//! ・ ResLayout\n
//! ・ ResFontList\n
//! ・ ResTextureList\n
//! ・ ResMaterialList\n
//! ・ ResExtUserDataList\n
//! ・ ResPaneBegin\n
//! ・ ResPaneEnd\n
//! ・ ResPane\n
//! ・ ResPicture\n
//! ・ ResTextBox\n
//! ・ ResWindow\n
//! ・ ResBounding\n
//! ・ ResGroupBegin\n
//! ・ ResGroupEnd\n
//! ・ ResGroup
//! ・ ResCapture
//!
//---------------------------------------------------------------------------
struct ResLayoutFile
{
    //! ファイルヘッダです。
    nn::font::detail::BinaryFileHeader fileHeader;
};

//---------------------------------------------------------------------------
//! @brief レイアウトを定義するデータブロックです。
//!
//---------------------------------------------------------------------------
struct ResLayout
{
    //! @brief ヘッダです。
    //!
    //! @details
    //! kind には DataBlockKindLayout が指定されます。
    //!
    nn::font::detail::BinaryBlockHeader blockHeader;

    //! @brief スクリーンの向きです。(未使用です)
    //! @sa ScreenOriginType
    uint8_t           originType;

    //! @brief (未使用です)
    char                              padding[3];

    //! @brief レイアウトの画面サイズです。
    ResVec2                layoutSize;

    //! @brief 部品としてのサイズ(ペインが実際に覆っている領域のサイズ)です。
    ResVec2                partsSize;

    /* Additional Info

    //! @brief レイアウト名です。長さの上限はありませんが、必ずnull終端されます。また、バッファのサイズは4の倍数になります。
    char                name[?];

    */
};

//---------------------------------------------------------------------------
//! @brief コントロールを定義するデータブロックです。
//!
//---------------------------------------------------------------------------
struct ResControl
{
    //! @brief ヘッダです。
    //!
    //! @details
    //! kind には DataBlockKindControl が指定されます。
    //!
    nn::font::detail::BinaryBlockHeader blockHeader;

    //! コントロールにユーザーが付けた名前の、オブジェクトの先頭からの相対位置です。
    uint32_t          controlUserNameOffset;

    //! Additional Infoのメンバー、controlFunctionalPaneNamesの、オブジェクトの先頭からの相対位置です。
    uint32_t          controlFunctionalPaneNamesOffset;

    //! コントロールの機能ペインの数
    uint16_t          controlFunctionalPaneCount;

    //! コントロールの機能アニメーションの数
    uint16_t          controlFunctionalAnimCount;

    //! Additional Infoのメンバー、controlFunctionalPaneParameterNameOffsetsの、オブジェクトの先頭からの相対位置です。
    uint32_t          controlFunctionalPaneParameterNameOffsetsOffset;

    //! Additional Infoのメンバー、controlFunctionalAnimParameterNameOffsetsの、オブジェクトの先頭からの相対位置です。
    uint32_t          controlFunctionalAnimParameterNameOffsetsOffset;

    /* Additional Info

    //! コントロールの名前。長さの上限はありませんが、必ずnull終端されます。また、バッファのサイズは4の倍数になります。
    char        controlName[?];

    //! コントロールのユーザーが付けた名前。長さの上限はありませんが、必ずnull終端されます。また、バッファのサイズは4の倍数になります。
    char        controlUserName[?];

    //! コントロールの機能ペインの名前の配列。XMLに定義された順番になっている。controlFunctionalPaneCountだけある
    char        controlFunctionalPaneNames[?][ResourceNameStrMax];

    //! コントロールの機能アニメーションの名前(タグ名)の、controlFunctionalAnimNameOffsetsからのオフセット。
    //! XMLに定義された順番になっている。controlFunctionalAnimCountだけある
    //! オブジェクトの先頭からではなく、このメンバからのオフセットになっていることに注意。
    uint32_t  controlFunctionalAnimNameOffsets[?];

    //! コントロールの機能アニメーションの名前(タグ名)。設定されていない場合はnull文字列。可変長で、null終端されている。controlFunctionalAnimCountだけある
    char*       controlFunctionalAnimNames;

    //! コントロールの機能ペインのパラメータ名の、controlFunctionalPaneParameterNameOffsetsからのオフセット。
    //! XMLに定義された順番になっている。controlFunctionalPaneCountだけある
    //! オブジェクトの先頭からではなく、このメンバからのオフセットになっていることに注意。
    uint32_t  controlFunctionalPaneParameterNameOffsets[?];

    //! コントロールの機能ペインのパラメータ名。設定されていない場合はnull文字列。可変長で、null終端されている。controlFunctionalPaneCountだけある
    char*       controlFunctionalPaneParameterNames;

    //! コントロールの機能アニメーションのパラメータ名の、controlFunctionalAnimParameterNameOffsetsからのオフセット。
    //! XMLに定義された順番になっている。controlFunctionalAnimCountだけある
    //! オブジェクトの先頭からではなく、このメンバからのオフセットになっていることに注意。
    uint32_t  controlFunctionalAnimParameterNameOffsets[?];

    //! コントロールの機能アニメーションのパラメータ名。設定されていない場合はnull文字列。可変長で、null終端されている。controlFunctionalAnimCountだけある
    char*       controlFunctionalAnimParameterNames;

    */
};

//---------------------------------------------------------------------------
//! @brief フォントの参照情報です。
//!
//! @sa ResFontList
//!
//---------------------------------------------------------------------------
struct ResFont
{
    //! @brief リソース名のオフセットです(FontList の次のアドレスから)。
    uint32_t    nameStrOffset;
};

//---------------------------------------------------------------------------
//! @brief フォントの参照情報のリストを格納したデータブロックです。
//!
//! @details
//! ペインが参照するフォントのリストをペインに先立って定義します。
//!
//! blockHeader の kind には DataBlockKindFontList が指定されます。
//!
//! blockHeader の size は nameStrPool までを含みます。
//!
//! この構造体には以下のデータ構造が続きます。
//!
//! ・ ResSFont fonts[fontCount]\n
//! ・ uint8_t  nameStrPool[]
//!
//! @sa ResLayout
//!
//---------------------------------------------------------------------------
struct ResFontList
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;

    //! リストに含まれるフォント参照の総数です。
    uint16_t          fontCount;

    //! (未使用です)
    char                              padding[2];

    /* Additional Info
    ResFont                fonts[fontCount];
    uint8_t                   nameStrPool[];
    */
};

//---------------------------------------------------------------------------
//! @brief テクスチャイメージの参照情報です。
//!
//! @sa ResTextureList
//!
//---------------------------------------------------------------------------
struct ResTexture
{
    //! @brief リソース名のオフセットです(TextureList の次のアドレスから)。
    uint32_t    nameStrOffset;
};

//---------------------------------------------------------------------------
//! @brief テクスチャイメージの参照情報のリストを格納したデータブロックです。
//!
//! @details
//! ペインが参照するテクスチャのリストをペインに先立って定義します。
//!
//! blockHeader の kind には DataBlockKindTextureList が指定されます。
//!
//! blockHeader の size は nameStrPool までを含みます。
//!
//! この構造体には以下のデータ構造が続きます。
//!
//! ・ ResTexture textures[texCount]\n
//! ・ uint8_t  nameStrPool[]
//!
//! @sa ResLayout
//!
//---------------------------------------------------------------------------
struct ResTextureList
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;

    //! リストに含まれるテクスチャイメージ参照の総数です。
    uint16_t          texCount;

    //! (未使用です)
    char                              padding[2];

    /* Additional Info
    ResTexture             textures[texCount];
    uint8_t                   nameStrPool[];
    */
};

//---------------------------------------------------------------------------
//! @brief テクスチャのマッピング情報です。
//!
//! @sa ResMaterial
//!
//---------------------------------------------------------------------------
struct ResTexMap
{
    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief デフォルト値を設定します。
    //!
    void SetDefault()
    {
        texIdx = 0;
        wrapSflt = 0;
        wrapTflt = 0;
    }

    //! @brief S方向のラップモードを取得します。
    //!
    //! @return S方向のラップモードを返します。
    //!
    TexWrap GetWarpModeS() const
    {
        return TexWrap(detail::GetBits(wrapSflt,  0, 2));
    }

    //! @brief T方向のラップモードを取得します。
    //!
    //! @return T方向のラップモードを返します。
    //!
    TexWrap GetWarpModeT() const
    {
        return TexWrap(detail::GetBits(wrapTflt,  0, 2));
    }

    //! @brief 縮小フィルタの設定を取得します。
    //!
    //! @return 縮小フィルタの設定を返します。
    //!
    TexFilter GetMinFilter() const
    {
        return TexFilter(detail::GetBits(wrapSflt,  2, 2));
    }

    //! @brief 拡大フィルタの設定を取得します。
    //!
    //! @return 拡大フィルタの設定を返します。
    //!
    TexFilter GetMagFilter() const
    {
        return TexFilter(detail::GetBits(wrapTflt,  2, 2));
    }

    //! @brief S方向のラップモードを設定します。
    //!
    //! @param[in] value    S方向のラップモードです。 nn::ui2d::TexWrap を指定します。
    //!
    void SetWarpModeS(uint8_t  value)
    {
        NN_SDK_ASSERT(value < TexWrap_MaxTexWrap, "out of bounds: value[%d] < TexWrap_MaxTexWrap", value);
        detail::SetBits(&wrapSflt,  0, 2, value);
    }

    //! @brief T方向のラップモードを設定します。
    //!
    //! @param[in] value    T方向のラップモードです。 nn::ui2d::TexWrap を指定します。
    //!
    void SetWarpModeT(uint8_t  value)
    {
        NN_SDK_ASSERT(value < TexWrap_MaxTexWrap, "out of bounds: value[%d] < TexWrap_MaxTexWrap", value);
        detail::SetBits(&wrapTflt,  0, 2, value);
    }

    //! @brief 縮小フィルタを設定します。
    //!
    //! @param[in] value    縮小フィルタの設定です。 nn::ui2d::TexFilter を指定します。
    //!
    void SetMinFilter(uint8_t  value)
    {
        NN_SDK_ASSERT(value < TexFilter_MaxTexFilter, "out of bounds: value[%d] < TexFilter_MaxTexFilter", value);
        detail::SetBits(&wrapSflt,  2, 2, value);
    }

    //! @brief 拡大フィルタを設定します。
    //!
    //! @param[in] value    拡大フィルタの設定です。 nn::ui2d::TexFilter を指定します。
    //!
    void SetMagFilter(uint8_t  value)
    {
        NN_SDK_ASSERT(value < TexFilter_MaxTexFilter, "out of bounds: value[%d] < TexFilter_MaxTexFilter", value);
        detail::SetBits(&wrapTflt,  2, 2, value);
    }

    //@}

    //! @brief 内部用機能のため使用禁止です。
    uint16_t texIdx;

    //! @brief 内部用機能のため使用禁止です。
    uint8_t wrapSflt;                       // TexWrap, TexFilter

    //! @brief 内部用機能のため使用禁止です。
    uint8_t wrapTflt;                       // TexWrap, TexFilter
};

//---------------------------------------------------------------------------
//! @brief TexMap の追加情報です。
//!
//! @sa ResTexMap
//!
//---------------------------------------------------------------------------
struct ResTexMapAdditionalInfo
{
    //! @brief TexMap の追加情報を表す列挙子です。
    //!
    enum InfoType
    {
        InfoType_CaptureTextureEnabled = 0,
        InfoType_VectorGraphicsTextureEnabled,
        InfoType_Max
    };

    //! @brief 内部用機能のため使用禁止です。
    uint32_t    info;
};

//---------------------------------------------------------------------------
//! @brief マテリアルが持つ情報の数です。
//!
//! @sa ResMaterial
//!
//---------------------------------------------------------------------------
struct ResMaterialResourceCount
{
    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief コンストラクタです。
    //!
    void SetDefault()
    {
        bits = 0;
    }

    //! @brief テクスチャの数を取得します。
    //!
    //! @return テクスチャの数を返します。
    //!
    uint8_t  GetTexMapCount() const
    {
        return uint8_t (detail::GetBits(bits,  0, 2));
    }

    //! @brief ResTexSrt の数を取得します。
    //!
    //! @return ResTexSrt の数を返します。
    //!
    uint8_t  GetTexSrtCount() const
    {
        return uint8_t (detail::GetBits(bits,  2, 2));
    }

    //! @brief ResTexCoordGen の数を取得します。
    //!
    //! @return ResTexCoordGen の数を返します。
    //!
    uint8_t  GetTexCoordGenCount() const
    {
        return uint8_t (detail::GetBits(bits,  4, 2));
    }

    //! @brief ResTevStage の数を取得します。
    //!
    //! @return ResTevStage の数を返します。
    //!
    uint8_t  GetTevStageCount() const
    {
        return uint8_t (detail::GetBits(bits,  6, 3));
    }

    //! @brief ResAlphaCompare を持っているか調べます。
    //!
    //! @return ResAlphaCompare を持っていれば true を返します。
    //!
    bool HasAlphaCompare() const
    {
        return detail::TestBit(bits, 9);
    }

    //! @brief ResBlendMode を持っているか調べます。
    //!
    //! @return ResBlendMode を持っていれば true を返します。
    //!
    bool HasBlendMode() const
    {
        return detail::TestBit(bits, 10);
    }

    //! @brief テクスチャのみを使うか調べます。
    //!
    //! @return テクスチャのみを使う場合は true を返します。
    //!
    bool IsTextureOnly() const
    {
        return detail::TestBit(bits, 11);
    }

    //! @brief アルファの BlendMode をカラーとは別に持っているか調べます。
    //!
    //! @return アルファの BlendMode をカラーとは別に持っていれば true を返します。
    //!
    bool IsSeparateBlendMode() const
    {
        return detail::TestBit(bits, 12);
    }

    //! @brief インダイレクトのパラメータを持っているか調べます。
    //!
    //! @return インダイレクトのパラメータを持っていればtrueを返します。
    //!
    bool HasIndirectParameter() const
    {
        return detail::TestBit(bits, 14);
    }

    //! @brief 投影テクスチャのパラメータ数を調べます。
    //!
    //! @return 投影テクスチャのパラメータ数を返します。
    //!
    uint8_t  GetProjectionTexGenCount() const
    {
        return uint8_t (detail::GetBits(bits,  15, 2));
    }

    //! @brief フォント影のパラメータを持っているか調べます。
    //!
    //! @return フォント影のパラメータを持っていれば true を返します。
    //!
    bool HasFontShadowParameter() const
    {
        return detail::TestBit(bits, 17);
    }

    //! @brief 閾値によるアルファ補間を用いるか調べます。
    //!
    //! @return 閾値によるアルファ補間を用いる場合は true を返します。
    //!
    bool IsThresholdingAlphaInterpolation() const
    {
        return detail::TestBit(bits, 18);
    }

    //! @brief 詳細なコンバイナ設定を持っているか調べます。
    //!
    //! @return 詳細なコンバイナ設定を持っている場合は true を返します。
    //!
    bool HasDetailedCombiner() const
    {
        return detail::TestBit(bits, 19);
    }

    //! @brief コンバイナユーザーシェーダ設定を持っているか調べます。
    //!
    //! @return コンバイナユーザーシェーダ設定を持っている場合は true を返します。
    //!
    bool HasCombinerUserShader() const
    {
        return detail::TestBit(bits, 20);
    }

    //! @brief TexMap の追加情報を持っているかを調べます。
    //!
    //! @return TexMap の追加情報を持っている場合は true を返します。
    //!
    bool HasTexMapAdditionalInfo() const
    {
        return detail::TestBit(bits, 21);
    }

    //! @brief ベクターグラフィックステクスチャの数を取得します。
    //!
    //! @return ベクターグラフィックステクスチャの数を返します。
    //!
    uint8_t  GetVectorGraphicsTexInfoCount() const
    {
        return uint8_t (detail::GetBits(bits,  22, 2));
    }

    //! @brief ResTexMap の数を設定します。
    //!
    //! @param[in] value    ResTexMap の数です。
    //!
    void SetTexMapCount(int  value)
    {
        NN_SDK_ASSERT(value <= TexMapMax, "out of bounds: value[%u] <= TexMapMax", value);
        this->SetBits(0, 2, value);
    }

    //! @brief ResTexSrt の数を設定します。
    //!
    //! @param[in] value    ResTexSrt の数です。
    //!
    void SetTexSrtCount(int  value)
    {
        NN_SDK_ASSERT(value <= TexMapMax, "out of bounds: value[%u] <= TexMapMax", value);
        this->SetBits(2, 2, value);
    }

    //! @brief ResTexCoordGen の数を設定します。
    //!
    //! @param[in] value    ResTexCoordGen の数です。
    //!
    void SetTexCoordGenCount(int  value)
    {
        NN_SDK_ASSERT(value <= TexMapMax, "out of bounds: value[%u] <= TexMapMax", value);
        this->SetBits(4, 2, value);
    }

    //! @brief ResTevStage の数を設定します。
    //!
    //! @param[in] value    ResTevStage の数です。
    //!
    void SetTevStageCount(int  value)
    {
        NN_SDK_ASSERT(value <= TevStageMax, "out of bounds: value[%u] <= TevStageMax", value);
        this->SetBits(6, 3, value);
    }

    //! @brief ResAlphaCompare を持つかを設定します。
    //!
    //! @param[in] b    ResAlphaCompare を持つ場合には true を指定します。
    //!
    void SetAlphaCompare(bool b)
    {
        this->SetBit(9, b);
    }

    //! @brief ResBlendMode を持つかを設定します。
    //!
    //! @param[in] b    ResBlendMode を持つ場合には true を指定します。
    //!
    void SetBlendMode(bool b)
    {
        this->SetBit(10, b);
    }

    //! @brief テクスチャのみを使うかを設定します。
    //!
    //! @param[in] b    テクスチャのみを使う場合には true を指定します。
    //!
    void SetTextureOnly(bool b)
    {
        this->SetBit(11, b);
    }

    //! @brief アルファの BlendMode をカラーとは別に持っているかを設定します。
    //!
    //! @param[in] b    アルファの BlendMode をカラーとは別に持っている場合には true を指定します。
    //!
    void SetSeparateBlendMode(bool b)
    {
        this->SetBit(12, b);
    }

    //! @brief インダイレクトのパラメータを持っているかを設定します。
    //!
    //! @param[in] b    インダイレクトのパラメータを持っている場合には true を指定します。
    //!
    void SetIndirectParameter(bool b)
    {
        this->SetBit(14, b);
    }

    //! @brief 投影テクスチャのパラメータ数を設定します。
    //!
    //! @param[in] value    投影テクスチャのパラメータ数 を指定します。
    //!
    void SetProjectionTexGenCount(int  value)
    {
        NN_SDK_ASSERT(value <= TevStageMax, "out of bounds: value[%u] <= TevStageMax", value);
        this->SetBits(15, 2, value);
    }

    //! @brief フォント影のパラメータを持っているかを設定します。
    //!
    //! @param[in] b    フォント影のパラメータを持っている場合には true を指定します。
    //!
    void SetFontShadowParameter(bool b)
    {
        this->SetBit(17, b);
    }

    //! @brief 閾値によるアルファ補間を用いるか設定します。
    //!
    //! @param[in] b    閾値によるアルファ補間を用いる場合には true を設定します。
    //!
    void SetThresholdingAlphaInterpolation(bool b)
    {
        this->SetBit(18, b);
    }

    //! @brief 詳細なコンバイナ設定を持っているかを設定します。
    //!
    //! @param[in] b 詳細なコンバイナ設定を用いる場合には true を設定します。
    //!
    void SetDetailedCombinerStage(bool b)
    {
        this->SetBit(19, b);
    }

    //! @brief コンバイナユーザーシェーダ設定を持っているかを設定します。
    //!
    //! @param[in] b コンバイナユーザーシェーダ設定用いる場合には true を設定します。
    //!
    void SetCombinerUserShader(bool b)
    {
        this->SetBit(20, b);
    }

    //! @brief TexMap の追加情報を持っているかを設定します。
    //!
    //! @param[in] b TexMap の追加情報を用いる場合には true を設定します。
    //!
    void SetTexMapAdditionalInfo(bool b)
    {
        this->SetBit(21, b);
    }

    //! @brief ResVectorGraphicsTextureInfo の数を設定します。
    //!
    //! @param[in] value    ResVectorGraphicsTextureInfo の数です。
    //!
    void SetVectorGraphicsTextureInfoCount(int  value)
    {
        NN_SDK_ASSERT(value <= TexMapMax, "out of bounds: value[%u] <= TexMapMax", value);
        this->SetBits(22, 2, value);
    }


    //@}

    //! @brief 内部用機能のため使用禁止です。
    uint32_t bits;

private:
    void SetBits(int pos, int len, int  value)
    {
        uint32_t  work = bits;
        detail::SetBits(&work, pos, len, static_cast<uint32_t>(value));
        bits = work;
    }

    void SetBit(int pos, bool value)
    {
        uint32_t  work = bits;
        detail::SetBit(&work, pos, value);
        bits = work;
    }

};

//---------------------------------------------------------------------------
//! @brief カラー情報です。
//!
//! @sa ResLayoutFile
//! @sa ResMaterial
//! @sa ResPicture
//! @sa ResWindowContent
//!
//---------------------------------------------------------------------------
struct ResColor
{
    //! @brief 型変換を行います。
    //!
    //! @return nn::util::Unorm8x4 型のオブジェクトを返します。
    //!
    NN_EXPLICIT_OPERATOR nn::util::Unorm8x4() const
    {
        const nn::util::Unorm8x4 result = { { r, g, b, a } };
        return result;
    }

    //! @brief 値を 0.0 - 1.0 に変換して型変換を行います。
    //!
    //! @return nn::util::Float4 型のオブジェクトを返します。
    //!
    NN_EXPLICIT_OPERATOR nn::util::Float4() const
    {
        nn::util::Float4 result = nn::util::MakeFloat4(
            static_cast<float>(r) / 255.0f,
            static_cast<float>(g) / 255.0f,
            static_cast<float>(b) / 255.0f,
            static_cast<float>(a) / 255.0f);
        return result;
    }

    //! 赤色の成分です。
    uint8_t r;
    //! 緑色の成分です。
    uint8_t g;
    //! 青色の成分です。
    uint8_t b;
    //! アルファ成分です。
    uint8_t a;
};

//---------------------------------------------------------------------------
//! @brief マテリアル情報です。
//!
//! @details
//! この構造体に続いて、詳細情報の構造体が続きます。
//!
//! ・ ResTexMap resTexMaps[texCount]\n
//! ・ ResTexSrt texSrts[texSrtCount]\n
//! ・ ResTexCoordGen texCoordGen[texCoordGenCount]\n
//! ・ ResTevStage tevStages[tevStageCount]\n
//! ・ ResAlphaCompare alphaCompare\n
//! ・ ResBlendMode blendMode
//! ・ ResBlendMode blendModeAlpha
//! ・ ResHsbAdjustment hsbAdjustment
//! ・ ResIndirectParameter indirectParameter
//!
//! どの詳細情報をいくつ持つかは resCount の値によって決まります。
//!
//! @sa ResMaterialList
//!
//---------------------------------------------------------------------------
struct ResMaterial
{
    //! 名前です。
    char                name[MaterialNameStrMax];

    //! 詳細設定の数です。
    ResMaterialResourceCount resCount;

    /* Additional Info
    ResConstantColor       constantColor;
    ResTexMap              resTexMaps[texCount];
    ResTexSrt              texSrts[texSrtCount];
    ResTexCoordGen         texCoordGen[texCoordGenCount];
    ResTevStage            tevStages[tevStageCount];
    ResAlphaCompare        alphaCompare;
    ResBlendMode           blendMode;
    ResBlendMode           blendModeAlpha;
    HsbAdjustment       hsbAdjustment;
    ResIndirectParameter   indirectParameter;
    ResFontShadowParameter fontShadowParameter;
    */
};

//---------------------------------------------------------------------------
//! @brief マテリアル情報のリストです。
//!
//! @details
//! ペインが参照するマテリアルのリストをペインに先立って定義します。
//!
//! blockHeader の kind には DataBlockKindMaterialList が指定されます。
//!
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ uint32_t materialOffsetTable[materialCount]\n
//! ・ ResMaterial material0\n
//! ・ …
//!
//! materialOffsetTable は各マテリアル情報へのオフセット(データブロック先頭から)です。
//!
//! @sa ResLayout
//!
//---------------------------------------------------------------------------
struct ResMaterialList
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;

    //! マテリアルの数です。
    uint16_t          materialCount;

    //! (未使用です)
    char                              padding[2];

    /* Additional Info
    uint32_t          materialOffsetTable[materialCount];
    ResMaterial            materials[materialCount];
    */
};

//---------------------------------------------------------------------------
//! @brief  角丸矩形のメッシュ生成パラメータ
//---------------------------------------------------------------------------
struct ResShapeInfoGfxPrimitiveRoundRect
{
    //! 角丸のサイズです(pixel単位)。
    uint32_t    radius;
    //! 角丸の分割数です。
    uint32_t    slice;
};

//---------------------------------------------------------------------------
//! @brief  円のメッシュ生成パラメータ
//---------------------------------------------------------------------------
struct ResShapeInfoGfxPrimitiveCircle
{
    //! 円の分割数です。
    uint32_t    slice;
};

//---------------------------------------------------------------------------
//! @brief シェイプ情報です。
//!
//---------------------------------------------------------------------------
struct ResShapeInfo
{
    //! @brief シェイプ情報の形式を表す列挙子です。
    //!
    enum ShapeInfoType
    {
        ShapeInfo_Quad,
        ShapeInfo_GfxPrimitiveRoundRect,
        ShapeInfo_GfxPrimitiveCircle,
        ShapeInfo_Max
    };

    //! シェイプ情報の形式です。
    uint16_t    type;
    uint8_t     padding[2];

    /*  Additional Info
    ResShapeInfoGfxPrimitiveCircle
    ResShapeInfoGfxPrimitiveRoundRect
    */
};

//---------------------------------------------------------------------------
//! @brief シェイプ情報のリストです。
//!
//! @details
//! ペインが参照するシェイプ情報のリストをペインに先立って定義します。
//!
//! blockHeader の kind には DataBlockKindShapeList が指定されます。
//!
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ …
//!
//! shapeInfoOffsetTable は各シェイプ情報へのオフセット(データブロック先頭から)です。
//!
//! @sa ResLayout
//!
//---------------------------------------------------------------------------
struct ResShapeInfoList
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;

    //! シェイプ情報の数です。
    uint16_t          shapeInfoCount;

    //! (未使用です)
    char                              padding[2];

    /* Additional Info
    */
};


//---------------------------------------------------------------------------
//! @brief キャプチャテクスチャイメージの参照情報です。バイナリフォーマット 8.6.0.0 以降のフォーマットです。
//!
//! @sa ResCaptureTextureList
//!
//---------------------------------------------------------------------------
struct ResCaptureTexture
{
    //! @brief キャプチャテクスチャ名のオフセットです(CaptureTextureList の次のアドレスから)。
    uint32_t    textureNameStrOffset;
    //! @brief 対象ペイン名のオフセットです(CaptureTextureList の次のアドレスから)。
    uint32_t    paneNameStrOffset;
    //! @brief 将来の拡張のための予約領域です。
    NN_PADDING8;
    //! クリアカラーです
    float       clearColor[4];
    //! フィルターテクスチャのフォーマットです
    uint16_t    textureFormat;
    //! @brief フレームバッファをキャプチャするかどうかのフラグです。
    bool        framebufferCaptureEnabled;
    //! @brief 初回のみキャプチャフラグ
    bool        captureOnlyFirstFrame;
    //! フィルター数です
    uint16_t    filterCount;
    //! @brief 将来の拡張のための予約領域です。
    NN_PADDING2;

    /* Additional Info
    ResCaptureTextureFilter filters[];
    */
};

//---------------------------------------------------------------------------
//! @brief キャプチャテクスチャイメージのフィルター情報です。
//!
//! @sa ResCaptureTexture
//!
//---------------------------------------------------------------------------
struct ResCaptureTextureFilter
{
    //! フィルタータイプです
    uint16_t    type;
    //! (未使用です)
    char        padding[2];
    //! フィルターテクスチャのスケールです
    float       textureScale;
};


//---------------------------------------------------------------------------
//! @brief キャプチャテクスチャイメージの参照情報のリストを格納したデータブロックです。
//!
//! @details
//! ペインが参照するキャプチャテクスチャのリストをペインに先立って定義します。
//!
//! blockHeader の kind には DataBlockKindCaptureTextureList が指定されます。
//!
//! blockHeader の size は nameStrPool までを含みます。
//!
//! この構造体には以下のデータ構造が続きます。
//!
//! ・ ResCaptureTexture textures[texCount]\n
//! ・ uint8_t  nameStrPool[]
//!
//! @sa ResLayout
//!
//---------------------------------------------------------------------------
struct ResCaptureTextureList
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;

    //! リストに含まれるテクスチャイメージ参照の総数です。
    uint16_t          texCount;

    //! (未使用です)
    char                              padding[2];

    /* Additional Info
    ResCaptureTexture         textures[texCount];
    uint8_t                   nameStrPool[];
    */
};

//! ResVectorGraphicsTexture で指定可能な bnvg 名の最大サイズ。
const uint32_t    VectorGraphicsNameLengthMax = 64;

//---------------------------------------------------------------------------
//! @brief ベクターグラフィックスデータの参照情報です。
//!
//! @sa ResVectorGraphicsData
//!
//---------------------------------------------------------------------------
struct ResVectorGraphicsTexture
{
    //! @brief ベクターグラフィックステクスチャに適用可能なアンチエイリアスの形式を表す列挙子です。
    //!
    enum AntiAliasType
    {
        //! アンチエイリアスなし
        AntiAliasType_None,
        //! MSAA x2
        AntiAliasType_MSAAx2,
        //! MSAA x4
        AntiAliasType_MSAAx4,
        //! MSAA x8
        AntiAliasType_MSAAx8,

        AntiAliasType_Max
    };

    //! @brief bnvg データ名のオフセットです(ResVectorGraphicsTexture の次のアドレスから)。
    uint32_t    bnvgNameStrOffset;
    //! バイナリコンバーターでコピーされた際に振られたID。TexMap では bnvg の名前 + "_id" で参照が設定されている。
    uint32_t    instanceId;
    //! テクスチャのフォーマットです。
    uint16_t    textureFormat;
    //! アンチエイリアスタイプ
    uint16_t    aaType;
    //! @brief 将来の拡張のための予約領域です。
    NN_PADDING4;
};

//---------------------------------------------------------------------------
//! @brief ベクターグラフィックスデータの参照情報のリストを格納したデータブロックです。
//!
//! blockHeader の kind には DataBlockKindVectorGraphicsDataList が指定されます。
//!
//! blockHeader の size は nameStrPool までを含みます。
//!
//! この構造体には以下のデータ構造が続きます。
//!
//! ・ ResVectorGraphicsData vectorGraphicsData[dataCount]\n
//! ・ uint8_t  nameStrPool[]
//!
//! @sa ResVectorGraphicsData
//!
//---------------------------------------------------------------------------
struct ResVectorGraphicsTextureList
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;

    //! リストに含まれるベクターグラフィックスデータ参照の総数です。
    uint16_t          count;

    //! (未使用です)
    NN_PADDING2;

    /* Additional Info
    ResVectorGraphicsData   data[dataCount];
    uint8_t                   nameStrPool[];
    */
};

//---------------------------------------------------------------------------
//! @brief 子ペインの定義を開始するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindPaneBegin が指定されます。
//!
//! ResPaneBegin と ResPaneEnd により子ペイン列が定義されます。
//!
//! ResPaneBegin の前の ResPane が親ペインとなります。
//!
//! ・ ResPane : pane0\n
//! ・ ResPaneBegin\n
//! ・ ResPane : pane1\n
//! ・ ResPane : pane2\n
//! ・ ResPaneEnd
//!
//! この場合、 pane1 と pane2 は pane0 の子ペインとなります。
//!
//! ResPane の代わりに ResPicture, ResWindow, ResTextBox, ResBounding, ResCapture も指定可能です。
//!
//! @sa ResLayoutFile
//!
//---------------------------------------------------------------------------
struct ResPaneBegin
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;
};

//---------------------------------------------------------------------------
//! @brief 子ペインの定義を完了するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindPaneEnd が指定されます。
//!
//! @sa ResLayoutFile
//! @sa ResPaneBegin
//!
//---------------------------------------------------------------------------
struct ResPaneEnd
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;
};

//---------------------------------------------------------------------------
//! @brief Nullペインを定義するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindPane が指定されます。
//!
//! @sa ResLayoutFile
//! @sa ResPaneBegin
//!
//---------------------------------------------------------------------------
struct ResPane
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;

    //! フラグです (PaneFlag)。
    uint8_t           flag;
    //! 位置基準です (ベースポジションの H と V、親ペインでの原点位置のHとVがそれぞれ 2 ビットずつ)。
    //! 型は H が nn::ui2d::HorizontalPosition、V が nn::ui2d::VerticalPosition となります。
    uint8_t           basePosition;
    //! アルファ値です。
    uint8_t           alpha;
    //! 追加のフラグです (PaneFlagEx)。
    uint8_t           flagEx;

    //! ペインの名前です。
    char                name[ResourceNameStrMax];
    //! ユーザデータです。
    char                userData[UserDataStrMax];
    //! トランスレート値です。
    ResVec3                translate;
    //! ローテート値です。
    ResVec3                rotate;
    //! スケール値です。
    ResVec2                scale;
    //! サイズです。
    ResVec2                size;
};

//---------------------------------------------------------------------------
//! @brief Pictureペインを定義するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindPicture が指定されます。
//!
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ ResVec2 texCoords[texCoordCount][VERTEX_MAX];
//!
//! @sa ResLayoutFile
//! @sa ResPaneBegin
//!
//---------------------------------------------------------------------------
struct ResPicture : public ResPane
{
    //! 頂点カラーです。
    ResColor               vtxCols[VertexColor_MaxVertexColor];

    //! マテリアルのインデックスです。
    uint16_t          materialIdx;
    //! テクスチャ座標の数です。
    uint8_t           texCoordCount;
    //! 追加情報フラグです。
    uint8_t           flags;

    /* Additional Info
    ResVec2           texCoords[texCoordCount][VERTEX_MAX];
    uint32_t          shapeBinaryIndex;
    */
};

//---------------------------------------------------------------------------
//! @brief テキストボックスの文字単位のアニメーションの情報です。
//!
//! @sa ResTextBox
//!
//---------------------------------------------------------------------------
struct ResPerCharacterTransform
{
    //! 文字単位のアニメーションのカーブオフセットです。
    float          evalTimeOffset;

    //! 文字単位のアニメーションのカーブ幅です。
    float          evalTimeWidth;

    //! 文字単位のアニメーションのループの種類です。
    uint8_t           loopType;

    //! 文字単位のアニメーションの回転中心です。
    uint8_t           originV;

    //! 文字単位のアニメーションのAnimationInfoがあるか否かです。0以外の場合は、この構造体の
    //! すぐ後ろにAnimationInfoがあります。
    uint8_t           hasAnimationInfo;

    //! (未使用です)
    char                              padding[1];

    // この先に ResAnimationInfo が続く
};

//---------------------------------------------------------------------------
//! @brief テキストボックスの文字単位のアニメーションの情報です(8.1.0.0 以降版)。
//!
//! @sa ResPerCharacterTransform
//!
//---------------------------------------------------------------------------
struct ResPerCharacterTransformExtended : public ResPerCharacterTransform
{
    //! 文字単位のアニメーションの回転中心のオフセットです。
    float             originVOffset;

    //! 文字毎のアニメーションの文字幅固定の文字幅
    float             fixSpaceWidth;

    //! 文字毎のアニメーションの文字幅固定もしくは空白挿入の文字揃え
    uint8_t           fixSpaceOrInsertSpaceOrigin;

    //! (未使用です。将来の拡張のために予約しています)
    char              padding[11];

    // この先に ResAnimationInfo が続く
};

//---------------------------------------------------------------------------
//! @brief TextBoxペインを定義するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindTextBox が指定されます。
//!
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ uint16_t text[];
//!
//! @sa ResLayoutFile
//! @sa ResPaneBegin
//!
//---------------------------------------------------------------------------
struct ResTextBox : public ResPane
{
    //! 文字列用バッファに確保するサイズです。
    uint16_t          textBufBytes;
    //! 初期テキスト文字列のバイト数です。
    uint16_t          textStrBytes;

    //! マテリアルのインデックスです。
    uint16_t          materialIdx;
    //! フォントへのインデックスです。
    uint16_t          fontIdx;

    //! テキスト表示位置基準です(nn::ui2d::HorizontalPosition * nn::ui2d::HorizontalPosition_MaxHorizontalPosition + nn::ui2d::VerticalPosition)。
    uint8_t           textPosition;
    //! 行そろえ指定です(TextAlignment)。
    uint8_t           textAlignment;
    //! フラグです(TextBoxFlag)。
    uint16_t           textBoxFlag;

    //! 斜体の傾斜率です(-1.0 ～ 1.0 ゼロの時にOFF)。
    float          italicRatio;

    //! text へのオフセットです(データブロック先頭から)。
    uint32_t          textStrOffset;
    //! テキストの表示色です。
    ResColor               textCols[TextColor_MaxTextColor];
    //! フォントのサイズです。nn::ui2d::TextBoxFlag::TextBoxFlag_KeepingFontScaleEnabled が true のときは代わりにフォントスケールが格納されます。
    ResVec2                fontSize;
    //! テキストの文字間隔です。
    float          charSpace;
    //! テキストの行間隔です。
    float          lineSpace;
    //! textID へのオフセットです(データブロック先頭から)。textIDが存在しない場合は0となります。
    uint32_t          textIdOffset;

    //! 影のオフセットです。
    ResVec2                shadowOffset;

    //! 影のスケールです。
    ResVec2                shadowScale;

    //! 影の色です。
    ResColor               shadowCols[TextColor_MaxTextColor];

    //! 影の斜体傾斜率です(-1.0 ～ 1.0 ゼロの時にOFF)。
    float          shadowItalicRatio;

    //! 行ごとの幅とオフセットへのオフセットです(データブロック先頭から)。存在しない場合は0となります。
    uint32_t          lineWidthOffsetOffset;

    //! 文字単位のアニメーションの情報へのオフセットです(データブロック先頭から)。存在しない場合は0となります。
    uint32_t          perCharacterTransformOffset;

    /* Additional Info
    uint16_t           text[];                     // テキスト
    char                textId[];                   // テキストID
    u8 lineWidthOffsetCount; // 行ごとの幅とオフセットの数
    float lineOffset[]; // 行ごとのオフセット
    float lineWidth[]; // 行ごとの幅
    ResPerCharacterTransform perCharacterTransform     // 文字単位のアニメーションの情報
    ResAnimationInfo       perCharacterTransformAnimationInfo;     // 文字単位のアニメーションのアニメーション情報
    */
};

//---------------------------------------------------------------------------
//! @brief Windowのフレーム情報です。
//!
//! @sa ResWindow
//!
//---------------------------------------------------------------------------
struct ResWindowFrame
{
    //! マテリアルのインデックスです。
    uint16_t          materialIdx;
    //! テクスチャの反転表示の指定です(nn::ui2d::TextureFlip)。
    uint8_t           textureFlip;
    //! (未使用です)
    char                              padding[1];
};

//---------------------------------------------------------------------------
//! @brief Windowのコンテント情報です。
//!
//! @details
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ ResVec2 texCoords[texCoordCount][VERTEX_MAX]
//!
//! @sa ResWindow
//!
//---------------------------------------------------------------------------
struct ResWindowContent
{
    //! 頂点カラーです。
    ResColor               vtxCols[VertexColor_MaxVertexColor];

    //! マテリアルのインデックスです。
    uint16_t          materialIdx;
    //! テクスチャ座標の数です。
    uint8_t           texCoordCount;
    //! (未使用です)
    char                              padding[1];

    /* Additional Info
    nn::util::Float2          texCoords[texCoordCount][VERTEX_MAX];
    */
};

//---------------------------------------------------------------------------
//! @brief Windowペインを定義するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindWindow が指定されます。
//!
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ ResWindowContent content\n
//! ・ uint32_t frameOffsetTable[frameCount]\n
//! ・ ResWindowFrame frame0\n
//! ・ …
//!
//! frameOffsetTable は ResWindowFrame へのオフセット(データブロック先頭から)です。
//!
//! @sa ResLayoutFile
//! @sa ResPaneBegin
//!
//---------------------------------------------------------------------------
struct ResWindow : public ResPane
{
    //! ペインの各辺の拡大量です。
    ResWindowInflation       inflation;

    //! フレームサイズです。
    ResWindowFrameSize       frameSize;

    //! フレーム数です。
    uint8_t           frameCount;
    //! ウィンドウ設定フラグです(WindowFlag 列挙子を使ってアクセスします)。
    uint8_t           windowFlags;
    //! (未使用です)
    char                              padding[2];

    //! content へのオフセットです(データブロック先頭から)。
    uint32_t          contentOffset;
    //! frameOffsetTable へのオフセットです(データブロック先頭から)。
    uint32_t          frameOffsetTableOffset;

    /* Additional Info

    ResWindowContent       content;

    detail::uint32_t    frameOffsetTable[frameCount];
    ResWindowFrame         frames;

    */
};

//---------------------------------------------------------------------------
//! @brief Boundingペインを定義するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindBounding が指定されます。
//!
//! @sa ResLayoutFile
//! @sa ResPaneBegin
//!
//---------------------------------------------------------------------------
struct ResBounding : public ResPane
{
};

//---------------------------------------------------------------------------
//! @brief Partsペインで、ペイン基本情報を上書きするための構造体です。
//!
//---------------------------------------------------------------------------
struct ResPartsPaneBasicInfo
{
    //! ユーザデータです。
    char                userData[UserDataStrMax];
    //! トランスレート値です。
    ResVec3                translate;
    //! ローテート値です。
    ResVec3                rotate;
    //! スケール値です。
    ResVec2                scale;
    //! サイズです。
    ResVec2                size;
    //! 透明度です。
    uint8_t           alpha;
    //! (未使用です)
    char                    padding[3];
};

//---------------------------------------------------------------------------
//! @brief Partsペインのプロパティを定義するための構造体です。
//!
//---------------------------------------------------------------------------
struct ResPartsProperty
{
    //! プロパティのペイン名です。
    char        name[ResourceNameStrMax];

    //! 上書きの用途を指定するフラグです。解釈は上書き対象の種類によって異なります。
    uint8_t   usageFlag;

    //! 基本部分の上書きの用途を指定するフラグです。(BasicOverrideUsageFlag列挙子のビットフラグです。)
    uint8_t   basicUsageFlag;

    //! マテリアルの部分上書きの用途を指定するフラグです。(MaterialOverrideUsageFlag列挙子のビットフラグです。)
    uint8_t   materialUsageFlag;

    //! (未使用です)
    char                      padding[1];

    //! プロパティの実データ(ResPictureまたはResTextBoxまたはResWindowまたはResCaptureTexture)のオフセット値。ResPartデータブロックの先頭からの値になります。
    uint32_t  propertyOffset;

    //! ExtUserDataOverrideOffsetの値、もしくは拡張ユーザーデータへのオフセット値(ResPartデータブロックの先頭からの値)です。
    uint32_t  extUserDataOffset;

    //! ペイン基本情報の上書きを行うための情報(ResPartsPaneBasicInfo)へのオフセット値。ResPartデータブロックの先頭からの値になります。
    uint32_t  paneBasicInfoOffset;
};

//---------------------------------------------------------------------------
//! @brief Partsペインを定義するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindParts が指定されます。
//!
//! @sa ResLayoutFile
//! @sa ResPaneBegin
//!
//---------------------------------------------------------------------------
struct ResParts : public ResPane
{
    //! プロパティの数です。
    uint32_t      propertyCount;

    //! 部品倍率です。
    ResVec2            magnify;
    /* Additional Info

    //! プロパティの情報がどこにあるかを示すテーブルです。propertyCount個の配列となります。
    ResPartsProperty   propertyTable[propertyCount];

    //! @brief 部品になるレイアウト名です。長さの上限はありませんが、必ずnull終端されます。また、バッファのサイズは4の倍数になります。
    char            name[?];

    //! プロパティの数だけ、ResPictureデータブロックまたはResTextBoxデータブロックまたはResPartsデータブロックが続きます。
    //! propertyTable内のオフセットを利用してアクセスします。
    ResPicture         picture;
    ResTextBox         textBox;
    ResParts           parts;
    */
};

//---------------------------------------------------------------------------
//! @brief 拡張ユーザデータを定義するデータブロックです。
//!
//! @details
//! 直近に定義されたペインに拡張ユーザデータを設定します。
//!
//! blockHeader の kind には DataBlockKindUserDataList が指定されます。
//!
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ ResExtUserData extUserData[num]\n
//! ・ uint8_t  data[]
//!
//! @sa ResLayoutFile
//! @sa ResPane
//!
//---------------------------------------------------------------------------
struct ResExtUserDataList
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;

    //! ユーザデータの数です。
    uint16_t            count;
    //! (未使用です)
    char                                padding[2];

    /* Additional Info

    ResExtUserData         extUserData[count];

    */
};

//---------------------------------------------------------------------------
//! @brief Captureペインを定義するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindCapture が指定されます。
//!
//! @sa ResLayoutFile
//! @sa ResPaneBegin
//!
//---------------------------------------------------------------------------
struct ResCapture : public ResPane
{
};

//---------------------------------------------------------------------------
//! @brief 整列ペインを定義するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindAlignment が指定されます。
//!
//---------------------------------------------------------------------------
struct ResAlignment : public ResPane
{
    enum FlagLen
    {
        LenAlignmentDirection  = 1,               // 0:水平 1: 垂直
    };

    enum FlagPos
    {
        PosAlignmentDirection   = 0,
    };

    union
    {
        //! 水平方向の配置設定です。
        int horizontalAlignment;

        //! 垂直方向の配置設定です。
        int verticalAlignment;
    };

    //! デフォルトマージンです。子ペインの二つ目以降の間隔に利用します。
    float defaultMergine;

    //! 端のペインを延長します。
    bool isExtendEdgeEnabled;

    //! フラグです。
    uint8_t flag;

    //! 将来の拡張のための予約領域です。
    NN_PADDING2;
};

//---------------------------------------------------------------------------
//! @brief シザーペインを定義するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindScissor が指定されます。
//!
//---------------------------------------------------------------------------
struct ResScissor : public ResPane
{
};


//---------------------------------------------------------------------------
//! @brief 子グループの定義を開始するデータブロックです。
//!
//! @details
//! ResGroupBegin と GroupEnd により子グループ列が定義されます。
//!
//! ResGroupBegin の前の Group が親グループとなります。
//!
//! @sa ResLayoutFile
//!
//---------------------------------------------------------------------------
struct ResGroupBegin
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;
};

//---------------------------------------------------------------------------
//! @brief 子グループの定義を完了するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindGroupEnd が指定されます。
//!
//! @sa ResLayoutFile
//! @sa ResGroupBegin
//!
//---------------------------------------------------------------------------
struct ResGroupEnd
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;
};

//---------------------------------------------------------------------------
//! @brief グループを定義するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindGroup が指定されます。
//!
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ char panes[paneCount][ResourceNameStrMax]
//!
//! panes にはグループに属するペインの名前が格納されます。
//!
//! @sa ResLayoutFile
//! @sa ResGroupBegin
//!
//---------------------------------------------------------------------------
struct ResGroup
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;

    //! グループ名です。
    char                name[GroupNameStrMax + 1];

    //! (未使用です)
    char                padding[1];
    //! グループに属するペインの数です。
    uint16_t          paneCount;

    /* Additional Info

    char                panes[paneCount][ResourceNameStrMax];

    */
};

//---------------------------------------------------------------------------
//! @brief 詳細なコンバイナ内のステージの詳細設定を持つ構造体です。
//!
//---------------------------------------------------------------------------
struct ResDetailedCombinerStage
{
    //! @brief 入力のソースを表す列挙子です。
    //!
    enum Src
    {
        //! TEXTURE0を使用します。
        Src_Texture0 = 0x3,
        //! TEXTURE1を使用します。
        Src_Texture1 = 0x4,
        //! TEXTURE2を使用します。
        Src_Texture2 = 0x5,
        //! TEXTURE3を使用します。
        Src_Texture3 = 0x6,
        //! CONSTANTを使用します。
        Src_Constant = 0xe,
        //! PRIMARYを使用します。
        Src_Primary = 0x0,
        //! 前段の出力を使用します。
        Src_Previous = 0xf,
        //! 前段のバッファを使用しますす。
        Src_PreviousBuffer = 0xd,

        //! 列挙子の総数です。
        Src_Num = 8
    };

    //! @brief 入力のRGBのオペランドを指定する列挙子です。
    //!
    enum OperandRgb
    {
        //! Rgbモードです。
        OperandRgb_Rgb = 0x0,
        //! InverseRgbモードです。
        OperandRgb_InverseRgb = 0x1,
        //! Alphaモードです。
        OperandRgb_Alpha = 0x2,
        //! InverseAlphaモードです。
        OperandRgb_InverseAlpha = 0x3,
        //! Rrrモードです。
        OperandRgb_Rrr = 0x4,
        //! InverseRrrモードです。
        OperandRgb_InverseRrr = 0x5,
        //! Gggモードです。
        OperandRgb_Ggg = 0x8,
        //! InverseGggモードです。
        OperandRgb_InverseGgg = 0x9,
        //! Bbbモードです。
        OperandRgb_Bbb = 0xc,
        //! InverseBbbモードです。
        OperandRgb_InverseBbb = 0xd,

        //! 列挙子の総数です。
        OperandRgb_Num = 10
    };

    //! @brief 入力のアルファのオペランドを指定する列挙子です。
    //!
    enum OperandAlpha
    {
        //! Alphaモードです。
        OperandAlpha_Alpha = 0x0,
        //! InverseAlpha モードです。
        OperandAlpha_InverseAlpha = 0x1,
        //! Rモードです。
        OperandAlpha_R = 0x2,
        //! InverseRモードです。
        OperandAlpha_InverseR = 0x3,
        //! Gモードです。
        OperandAlpha_G = 0x4,
        //! InverseGモードです。
        OperandAlpha_InverseG = 0x5,
        //! Bモードです。
        OperandAlpha_B = 0x6,
        //! InverseBモードです。
        OperandAlpha_InverseB = 0x7,

        //! 列挙子の総数です。
        OperandAlpha_Num = 8
    };

    //! @brief 演算モードを表す列挙子です。
    //!
    enum Combine
    {
        //! REPLACEモードです。
        Combine_Replace = 0x0,
        //! MODULATEモードです。
        Combine_Modulate = 0x1,
        //! ADDモードです。
        Combine_Add = 0x2,
        //! ADD_SIGNEDモードです。
        Combine_AddSigned = 0x3,
        //! INTERPOLATEモードです。
        Combine_Interpolate = 0x4,
        //! SUBTRACTモードです。
        Combine_Subtract = 0x5,
        //! DOT3_RGBモードです。
        Combine_Dot3Rgb = 0x6,
        //! DOT3_RGBAモードです。
        Combine_Dot3Rgba = 0x7,
        //! ADD_MULTモードです。
        Combine_AddMult = 0x8,
        //! MULT_ADDモードです。
        Combine_MultAdd = 0x9,

        //! 列挙子の総数です。
        Combine_Num = 10
    };

    //! @brief 定数カラーレジスタにロードするカラーを指定する列挙子です。
    enum ConstColorSelect
    {
        //! 二色補間の黒色をロードします。
        ConstColorSelect_Black,
        //! 二色補間の白色をロードします。
        ConstColorSelect_White,
        //! コンスタントカラー 0 をロードします。
        ConstColorSelect_C0,
        //! コンスタントカラー 1 をロードします。
        ConstColorSelect_C1,
        //! コンスタントカラー 2 をロードします。
        ConstColorSelect_C2,
        //! コンスタントカラー 3 をロードします。
        ConstColorSelect_C3,
        //! コンスタントカラー 4 をロードします。
        ConstColorSelect_C4,
        //! コンスタントカラー 5 をロードします。
        ConstColorSelect_C5,

        //! 列挙子の総数です。
        ConstColorSelect_Max
    };

    //! @brief 出力のスケールを指定する列挙子です。
    //!
    enum Scale
    {
        //! 演算結果をそのまま出力します。
        Scale_1 = 0x0,
        //! 演算結果を 2 倍にします。
        Scale_2 = 0x1,
        //! 演算結果を 4 倍にします。
        Scale_4 = 0x2,

        //! 列挙子の総数です。
        Scale_NUM = 3
    };

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief すべての項目を設定します。
    //!
    //! @param[in] combineRgb RGBの演算のタイプです。
    //! @param[in] combineAlpha アルファの演算のタイプです。
    //! @param[in] srcRgb0 RGB演算のソース 0 です。
    //! @param[in] srcRgb1 RGB演算のソース 1 です。
    //! @param[in] srcRgb2 RGB演算のソース 2 です。
    //! @param[in] srcAlpha0 アルファ演算のソース 0 です。
    //! @param[in] srcAlpha1 アルファ演算のソース 1 です。
    //! @param[in] srcAlpha2 アルファ演算のソース 2 です。
    //! @param[in] operandRgb0 RGB演算のソース 0 のオペランドです。
    //! @param[in] operandRgb1 RGB演算のソース 1 のオペランドです。
    //! @param[in] operandRgb2 RGB演算のソース 2 のオペランドです。
    //! @param[in] operandAlpha0 アルファ演算のソース 0 のオペランドです。
    //! @param[in] operandAlpha1 アルファ演算のソース 1 のオペランドです。
    //! @param[in] operandAlpha2 アルファ演算のソース 2 のオペランドです。
    //! @param[in] scaleRgb RGB演算に対するスケールです。
    //! @param[in] scaleAlpha アルファ演算に対するスケールです。
    //! @param[in] constColorSelectRgb RGB演算で使用する定数レジスタのソースです。
    //! @param[in] constColorSelectAlpha アルファ演算で使用する定数レジスタのソースです。
    //! @param[in] savePrevRgb 前段の演算結果（RGB）をバッファに保存する場合は true を指定します。
    //! @param[in] savePrevAlpha 前段の演算結果（A）をバッファに保存する場合は true を指定します。
    //!
    void Set(
        Combine         combineRgb,
        Combine         combineAlpha,
        Src             srcRgb0,
        Src             srcRgb1,
        Src             srcRgb2,
        Src             srcAlpha0,
        Src             srcAlpha1,
        Src             srcAlpha2,
        OperandRgb      operandRgb0,
        OperandRgb      operandRgb1,
        OperandRgb      operandRgb2,
        OperandAlpha    operandAlpha0,
        OperandAlpha    operandAlpha1,
        OperandAlpha    operandAlpha2,
        Scale           scaleRgb,
        Scale           scaleAlpha,
        ConstColorSelect constColorSelectRgb,
        ConstColorSelect constColorSelectAlpha,
        bool            savePrevRgb,
        bool            savePrevAlpha
    )
    {
        SetCombineRgb(combineRgb);
        SetCombineAlpha(combineAlpha);
        SetSrcRgb0(srcRgb0);
        SetSrcRgb1(srcRgb1);
        SetSrcRgb2(srcRgb2);
        SetSrcAlpha0(srcAlpha0);
        SetSrcAlpha1(srcAlpha1);
        SetSrcAlpha2(srcAlpha2);
        SetOperandRgb0(operandRgb0);
        SetOperandRgb1(operandRgb1);
        SetOperandRgb2(operandRgb2);
        SetOperandAlpha0(operandAlpha0);
        SetOperandAlpha1(operandAlpha1);
        SetOperandAlpha2(operandAlpha2);
        SetScaleRgb(scaleRgb);
        SetScaleAlpha(scaleAlpha);
        SetConstColorSelectRgb(constColorSelectRgb);
        SetConstColorSelectAlpha(constColorSelectAlpha);
        SetSavePrevRgb(savePrevRgb);
        SetSavePrevAlpha(savePrevAlpha);
        SetCombineRgbSourceCount(GetCombineRgb());
        SetCombineAlphaSourceCount(GetCombineAlpha());
    }

    //! @brief RGBの演算のタイプを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetCombineRgb(Combine value)
    {
        bits0 = detail::SetBits(bits0, Pos0_CombineRgb, Bits_CombineRgb, Bit32(value));
    }

    //! @brief アルファの演算のタイプを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetCombineAlpha(Combine value)
    {
        bits1 = detail::SetBits(bits1, Pos1_CombineAlpha, Bits_CombineAlpha, Bit32(value));
    }

    //! @brief RGB演算のソース 0 を設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetSrcRgb0(Src value)
    {
        bits0 = detail::SetBits(bits0, Pos0_SrcRgb0, Bits_SrcRgb, Bit32(value));
    }

    //! @brief RGB演算のソース 1 を設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetSrcRgb1(Src value)
    {
        bits0 = detail::SetBits(bits0, Pos0_SrcRgb1, Bits_SrcRgb, Bit32(value));
    }

    //! @brief RGB演算のソース 2 を設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetSrcRgb2(Src value)
    {
        bits0 = detail::SetBits(bits0, Pos0_SrcRgb2, Bits_SrcRgb, Bit32(value));
    }

    //! @brief アルファ演算のソース 0 を設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetSrcAlpha0(Src value)
    {
        bits1 = detail::SetBits(bits1, Pos1_SrcAlpha0, Bits_SrcAlpha, Bit32(value));
    }

    //! @brief アルファ演算のソース 1 を設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetSrcAlpha1(Src value)
    {
        bits1 = detail::SetBits(bits1, Pos1_SrcAlpha1, Bits_SrcAlpha, Bit32(value));
    }

    //! @brief アルファ演算のソース 2 を設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetSrcAlpha2(Src value)
    {
        bits1 = detail::SetBits(bits1, Pos1_SrcAlpha2, Bits_SrcAlpha, Bit32(value));
    }

    //! @brief RGB演算のソース 0 のオペランドを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetOperandRgb0(OperandRgb value)
    {
        bits0 = detail::SetBits(bits0, Pos0_OperandRgb0, Bits_OperandRgb, Bit32(value));
    }

    //! @brief RGB演算のソース 1 のオペランドを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetOperandRgb1(OperandRgb value)
    {
        bits0 = detail::SetBits(bits0, Pos0_OperandRgb1, Bits_OperandRgb, Bit32(value));
    }

    //! @brief RGB演算のソース 2 のオペランドを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetOperandRgb2(OperandRgb value)
    {
        bits0 = detail::SetBits(bits0, Pos0_OperandRgb2, Bits_OperandRgb, Bit32(value));
    }

    //! @brief アルファ演算のソース 0 のオペランドを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetOperandAlpha0(OperandAlpha value)
    {
        bits1 = detail::SetBits(bits1, Pos1_OperandAlpha0, Bits_OperandAlpha, Bit32(value));
    }

    //! @brief アルファ演算のソース 1 のオペランドを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetOperandAlpha1(OperandAlpha value)
    {
        bits1 = detail::SetBits(bits1, Pos1_OperandAlpha1, Bits_OperandAlpha, Bit32(value));
    }

    //! @brief アルファ演算のソース 2 のオペランドを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetOperandAlpha2(OperandAlpha value)
    {
        bits1 = detail::SetBits(bits1, Pos1_OperandAlpha2, Bits_OperandAlpha, Bit32(value));
    }

    //! @brief RGB演算に対するスケールを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetScaleRgb(Scale value)
    {
        bits0 = detail::SetBits(bits0, Pos0_ScaleRgb, Bits_Scale, Bit32(value));
    }

    //! @brief アルファ演算に対するスケールを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetScaleAlpha(Scale value)
    {
        bits1 = detail::SetBits(bits1, Pos1_ScaleAlpha, Bits_Scale, Bit32(value));
    }

    //! @brief RGB演算で使用する定数レジスタのソースを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetConstColorSelectRgb(ConstColorSelect value)
    {
        bits2 = detail::SetBits(bits2, Pos2_ConstColorSelectRgb, Bits_ConstColorSelect, Bit32(value));
    }

    //! @brief アルファ演算で使用する定数レジスタのソースを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetConstColorSelectAlpha(ConstColorSelect value)
    {
        bits2 = detail::SetBits(bits2, Pos2_ConstColorSelectAlpha, Bits_ConstColorSelect, Bit32(value));
    }

    //! @brief 前段の演算結果（RGB）をバッファに保存するか設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetSavePrevRgb(bool value)
    {
        bits0 = detail::SetBit(bits0, Pos0_SavePrevRgb, value);
    }

    //! @brief 前段の演算結果（A）をバッファに保存するか設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetSavePrevAlpha(bool value)
    {
        bits1 = detail::SetBit(bits1, Pos1_SavePrevAlpha, value);
    }

    //! @brief RGB演算タイプのソース個数を設定します。
    //!
    //! @param[in] value 設定する値です。
    void SetCombineRgbSourceCount(Combine value)
    {
        const uint32_t stageCombineRgbSourceCount[10] = {
            1, //0 Combine_Replace     ソース0 の色で置き換え
            2, //1 Combine_Modulate    ソース0 x ソース1
            2, //2 Combine_Add         ソース0 + ソース1
            2, //3 Combine_AddSigned   ソース0 + ソース1 - 0.5
            3, //4 Combine_Interpolate (ソース0 x ソース2) + (ソース1 x (1 - ソース2))
            2, //5 Combine_Subtract    ソース0 - ソース1
            0, //6 COMBINE_Dot3Rgb     --- 未使用
            0, //7 COMBINE_Dot3RgbA    --- 未使用
            3, //8 COMBINE_AddMult     (ソース0 + ソース1) x ソース2
            3  //9 COMBINE_MultAdd     (ソース0 x ソース1) + ソース2
        };
        NN_SDK_ASSERT(static_cast<int>(value) < sizeof(stageCombineRgbSourceCount), "SetCombineRgbSourceCount() out of bounds %d / %d", value, sizeof(stageCombineRgbSourceCount));
        bits3 = detail::SetBits(bits3, Pos3_Rgb_Count, Bits_CombineRgb_Count, Bit32(stageCombineRgbSourceCount[static_cast<int>(value)]));
    }

    //! @brief アルファ演算タイプのソース個数を設定します。
    //!
    //! @param[in] value 設定する値です。
    void SetCombineAlphaSourceCount(Combine value)
    {
        const uint32_t stageCombineAlphaSourceCount[10] = {
            1, //0 Combine_Replace     ソース0 の色で置き換え
            2, //1 Combine_Modulate    ソース0 x ソース1
            2, //2 Combine_Add         ソース0 + ソース1
            2, //3 Combine_AddSigned   ソース0 + ソース1 - 0.5
            3, //4 Combine_InterPolate (ソース0 x ソース2) + (ソース1 x (1 - ソース2))
            2, //5 Combine_Subtract    ソース0 - ソース1
            0, //6 Combine_Dot3Rgb     --- 未使用
            0, //7 Combine_Dot3Rgba    --- 未使用
            3, //8 COMBINE_AddMult     (ソース0 + ソース1) x ソース2
            3  //9 COMBINE_MultAdd     (ソース0 x ソース1) + ソース2
        };
        NN_SDK_ASSERT(static_cast<int>(value) < sizeof(stageCombineAlphaSourceCount), "SetCombineAlphaSourceCount() out of bounds %d / %d", value, sizeof(stageCombineAlphaSourceCount));
        bits3 = detail::SetBits(bits3, Pos3_Alpha_Count, Bits_CombineAlpha_Count, Bit32(stageCombineAlphaSourceCount[static_cast<int>(value)]));
    }


    //! @brief RGBの演算のタイプを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    Combine GetCombineRgb() const
    {
        return Combine(detail::GetBits(bits0, Pos0_CombineRgb, Bits_CombineRgb));
    }

    //! @brief アルファの演算のタイプを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    Combine GetCombineAlpha() const
    {
        return Combine(detail::GetBits(bits1, Pos1_CombineAlpha, Bits_CombineAlpha));
    }

    //! @brief RGB演算の入力 0 のソースを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    Src GetSrcRgb0() const
    {
        return Src(detail::GetBits(bits0, Pos0_SrcRgb0, Bits_SrcRgb));
    }

    //! @brief RGB演算の入力 1 のソースを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    Src GetSrcRgb1() const
    {
        return Src(detail::GetBits(bits0, Pos0_SrcRgb1, Bits_SrcRgb));
    }

    //! @brief RGB演算の入力 2 のソースを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    Src GetSrcRgb2() const
    {
        return Src(detail::GetBits(bits0, Pos0_SrcRgb2, Bits_SrcRgb));
    }

    //! @brief アルファ演算の入力 0 のソースを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    Src GetSrcAlpha0() const
    {
        return Src(detail::GetBits(bits1, Pos1_SrcAlpha0, Bits_SrcAlpha));
    }

    //! @brief アルファ演算の入力 1 のソースを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    Src GetSrcAlpha1() const
    {
        return Src(detail::GetBits(bits1, Pos1_SrcAlpha1, Bits_SrcAlpha));
    }

    //! @brief アルファ演算の入力 2 のソースを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    Src GetSrcAlpha2() const
    {
        return Src(detail::GetBits(bits1, Pos1_SrcAlpha2, Bits_SrcAlpha));
    }

    //! @brief RGB演算の入力 0 のオペランドを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    OperandRgb GetOperandRgb0() const
    {
        return OperandRgb(detail::GetBits(bits0, Pos0_OperandRgb0, Bits_OperandRgb));
    }

    //! @brief RGB演算の入力 1 のオペランドを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    OperandRgb GetOperandRgb1() const
    {
        return OperandRgb(detail::GetBits(bits0, Pos0_OperandRgb1, Bits_OperandRgb));
    }

    //! @brief RGB演算の入力 2 のオペランドを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    OperandRgb GetOperandRgb2() const
    {
        return OperandRgb(detail::GetBits(bits0, Pos0_OperandRgb2, Bits_OperandRgb));
    }

    //! @brief アルファ演算の入力 0 のオペランドを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    OperandAlpha GetOperandAlpha0() const
    {
        return OperandAlpha(detail::GetBits(bits1, Pos1_OperandAlpha0, Bits_OperandAlpha));
    }

    //! @brief アルファ演算の入力 1 のオペランドを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    OperandAlpha GetOperandAlpha1() const
    {
        return OperandAlpha(detail::GetBits(bits1, Pos1_OperandAlpha1, Bits_OperandAlpha));
    }

    //! @brief アルファ演算の入力 2 のオペランドを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    OperandAlpha GetOperandAlpha2() const
    {
        return OperandAlpha(detail::GetBits(bits1, Pos1_OperandAlpha2, Bits_OperandAlpha));
    }

    //! @brief RGB演算に対するスケールを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    Scale GetScaleRgb() const
    {
        return Scale(detail::GetBits(bits0, Pos0_ScaleRgb, Bits_Scale));
    }

    //! @brief アルファ演算に対するスケールを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    Scale GetScaleAlpha() const
    {
        return Scale(detail::GetBits(bits1, Pos1_ScaleAlpha, Bits_Scale));
    }

    //! @brief RGB演算で使用する定数レジスタのソースを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    ConstColorSelect GetConstColorSelectRgb() const
    {
        return ConstColorSelect(detail::GetBits(bits2, Pos2_ConstColorSelectRgb, Bits_ConstColorSelect));
    }

    //! @brief アルファ演算で使用する定数レジスタのソースを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    ConstColorSelect GetConstColSelAlpha() const
    {
        return ConstColorSelect(detail::GetBits(bits2, Pos2_ConstColorSelectAlpha, Bits_ConstColorSelect));
    }

    //! @brief 前段の演算結果（RGB）をバッファに保存するかの設定を取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    bool GetSavePrevRgb() const
    {
        return detail::TestBit(bits0, Pos0_SavePrevRgb);
    }

    //! @brief 前段の演算結果（A）をバッファに保存するかの設定を取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    bool GetSavePrevAlpha() const
    {
        return detail::TestBit(bits1, Pos1_SavePrevAlpha);
    }

    //! @brief RGB演算タイプのソース個数を取得します。
    //!
    //! @return 演算タイプのソース個数を返します。
    //!
    uint32_t GetCombineRgbSourceCount() const
    {
        return static_cast<uint32_t>(detail::GetBits(bits3, Pos3_Rgb_Count, Bits_CombineRgb_Count));
    }

    //! @brief アルファ演算タイプのソース個数を取得します。
    //!
    //! @return 演算タイプのソース個数を返します。
    //!
    uint32_t GetCombineAlphaSourceCount() const
    {
        return static_cast<uint32_t>(detail::GetBits(bits3, Pos3_Alpha_Count, Bits_CombineAlpha_Count));
    }

    //@}

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! すべての項目は無効な状態に設定されます。
    //!
    void SetDefault()
    {
        bits0 = 0;
        bits1 = 0;
        bits2 = 0;
        bits3 = 0;

        Set(
            Combine_Replace,
            Combine_Replace,
            Src_Primary,
            Src_Primary,
            Src_Primary,
            Src_Primary,
            Src_Primary,
            Src_Primary,
            OperandRgb_Rgb,
            OperandRgb_Rgb,
            OperandRgb_Rgb,
            OperandAlpha_Alpha,
            OperandAlpha_Alpha,
            OperandAlpha_Alpha,
            Scale_1,
            Scale_1,
            ConstColorSelect_C0,
            ConstColorSelect_C0,
            false,
            false);
    }

    //@}

    //! @details :private
    Bit32 bits0;

    //! @details :private
    Bit32 bits1;

    //! @details :private
    Bit32 bits2;

    //! @details :private
    Bit32 bits3;

    //! @details :private
    enum Bits
    {
        Bits_CombineRgb     = 4,
        Bits_CombineAlpha   = 4,
        Bits_SrcRgb         = 4,
        Bits_SrcAlpha       = 4,
        Bits_OperandRgb     = 4,
        Bits_OperandAlpha   = 4, // at least 3
        Bits_Scale          = 2,
        Bits_ConstColorSelect = 4,
        Bits_SavePrev       = 1,
        Bits_CombineRgb_Count = 4,
        Bits_CombineAlpha_Count = 4
    };

    //! @details :private
    enum Pos0
    {
        Pos0_SrcRgb0        = 0,
        Pos0_SrcRgb1        = 4,
        Pos0_SrcRgb2        = 8,
        Pos0_OperandRgb0    = 12,
        Pos0_OperandRgb1    = 16,
        Pos0_OperandRgb2    = 20,
        Pos0_CombineRgb     = 24,
        Pos0_ScaleRgb       = 28,
        Pos0_SavePrevRgb    = 30,

        Field0_Size         = 31
    };

    //! @details :private
    enum Pos1
    {
        Pos1_SrcAlpha0      = 0,
        Pos1_SrcAlpha1      = 4,
        Pos1_SrcAlpha2      = 8,
        Pos1_OperandAlpha0  = 12,
        Pos1_OperandAlpha1  = 16,
        Pos1_OperandAlpha2  = 20,
        Pos1_CombineAlpha   = 24,
        Pos1_ScaleAlpha     = 28,
        Pos1_SavePrevAlpha  = 30,

        Field1_Size         = 31
    };

    //! @details :private
    enum Pos2
    {
        Pos2_ConstColorSelectRgb    = 0,
        Pos2_ConstColorSelectAlpha  = 4,

        Field2_Size         = 8
    };

    enum Pos3
    {
        Pos3_Rgb_Count = 0,
        Pos3_Alpha_Count = 4,

        Field3_Size         = 8
    };

};

//---------------------------------------------------------------------------
//! @brief 詳細なコンバイナステージ設定の情報を持つ構造体です。
//!
//---------------------------------------------------------------------------
struct ResDetailedCombinerStageInfo
{
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! すべての項目は無効な状態に設定されます。
    //!
    void SetDefault()
    {
        NN_STATIC_ASSERT(ResDetailedCombinerStage::ConstColorSelect_Black == 0);
        m_Bits = 0;
    }

    // @}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief コンスタントカラーを設定します。
    //!
    //! @param[in] idx コンスタントカラーのインデックスです。
    //! @param[in] value 設定する値です。
    //!
    void SetConstantColor(uint32_t idx, ResColor value)
    {
        if (0 <= static_cast<int>(idx) && idx < DetailedCombinerConstantColor)
        {
            m_ConstColor[idx] = value;
        }
    }

    //! @brief コンスタントカラーを取得します。
    //!
    //! @param[in] idx コンスタントカラーのインデックスです。
    //!
    //! @return コンスタントカラーの設定値を返します。
    //!
    ResColor GetConstantColor(uint32_t idx) const
    {
        return m_ConstColor[idx % DetailedCombinerConstantColor];
    }

    //! @brief bufferColorレジスタのRGBに設定するカラーのソースを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetBufferColorSelectRgb(ResDetailedCombinerStage::ConstColorSelect value)
    {
        m_Bits = detail::SetBits(
            m_Bits,
            Pos_BufferColorSelectRgb,
            Bits_ConstColorSelect,
            Bit32(value));
    }

    //! @brief bufferColorレジスタのRGBに設定するカラーのソースを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    ResDetailedCombinerStage::ConstColorSelect GetBufferColorSelectRgb() const
    {
        return ResDetailedCombinerStage::ConstColorSelect(
            detail::GetBits(
                m_Bits,
                Pos_BufferColorSelectRgb,
                Bits_ConstColorSelect));
    }

    //! @brief bufferColorレジスタのAlphaに設定するカラーのソースを設定します。
    //!
    //! @param[in] value 設定する値です。
    //!
    void SetBufferColorSelectAlpha(ResDetailedCombinerStage::ConstColorSelect value)
    {
        m_Bits = detail::SetBits(
            m_Bits,
            Pos_BufferColorSelectAlpha,
            Bits_ConstColorSelect,
            Bit32(value));
    }

    //! @brief bufferColorレジスタのAlphaに設定するカラーのソースを取得します。
    //!
    //! @return 現在の設定値を返します。
    //!
    ResDetailedCombinerStage::ConstColorSelect GetBufferColorSelectAlpha() const
    {
        return ResDetailedCombinerStage::ConstColorSelect(
            detail::GetBits(
                Bit32(m_Bits),
                Pos_BufferColorSelectAlpha,
                Bits_ConstColorSelect));
    }

    //@}

private:
    //! @details :private
    enum Bits
    {
        Bits_ConstColorSelect       = 4
    };

    //! @details :private
    enum Pos
    {
        Pos_BufferColorSelectAlpha = 4,
        Pos_BufferColorSelectRgb = 0
    };

    Bit32 m_Bits;

    //! @brief コンスタントカラーです。
    ResColor m_ConstColor[TevStageMax];
};

//---------------------------------------------------------------------------
//! @brief コンスタントカラーを管理するリソースです。
//!
//---------------------------------------------------------------------------
struct ResConstantColor
{
public:
    //! @brief デフォルト値設定
    //!
    void SetDefault()
    {
        types = 0;
        count = 0;
    }

    // DetailedCombiner のすべての ConstantColor を入れるのであれば types は 32bit 必要
    uint8_t types;
    uint8_t count;
    /* Additional Info
    uint8_t offsets[count];
    void*   data;
    */

    size_t CalculateSize() const
    {
        size_t  size = sizeof(ResConstantColor);

        // オフセットリストのサイズ。
        size += sizeof(uint8_t) * count;
        for (int i = 0; i < count; i++)
        {
            if (detail::TestBit(types, i))
            {
                size += sizeof(float) * 4;
            }
            else
            {
                size += sizeof(uint32_t);
            }
        }

        return size;
    }

    const nn::util::Float4* GetColorAsFloat(int index) const
    {
        NN_SDK_ASSERT(index < count);
        NN_SDK_ASSERT(detail::TestBit(types, index));

        const uint8_t*  pOffsetTable = nn::util::ConstBytePtr(this, sizeof(types) + sizeof(count)).Get<uint8_t>();
        return nn::util::ConstBytePtr(this, pOffsetTable[index]).Get<nn::util::Float4>();
    }

    const nn::util::Unorm8x4* GetColorAsByte(int index) const
    {
        NN_SDK_ASSERT(index < count);
        NN_SDK_ASSERT(!detail::TestBit(types, index));

        const uint8_t*  pOffsetTable = nn::util::ConstBytePtr(this, sizeof(types) + sizeof(count)).Get<uint8_t>();
        return nn::util::ConstBytePtr(this, pOffsetTable[index]).Get<nn::util::Unorm8x4>();
    }
};

//---------------------------------------------------------------------------
//! @brief コンバイナユーザーシェーダの情報を持つ構造体です。
//!
//---------------------------------------------------------------------------
struct ResCombinerUserShader
{
public:
    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief コンスタントカラーを設定します。
    //!
    //! @param[in] idx コンスタントカラーのインデックスです。
    //! @param[in] value 設定する値です。
    //!
    void SetConstantColor(uint32_t idx, ResColor value)
    {
        if (0 <= static_cast<int>(idx) && idx < CombinerUserShaderConstantColor)
        {
            m_ConstColor[idx] = value;
        }
    }

    //! @brief コンスタントカラーを取得します。
    //!
    //! @param[in] idx コンスタントカラーのインデックスです。
    //!
    //! @return コンスタントカラーの設定値を返します。
    //!
    ResColor GetConstantColor(uint32_t idx) const
    {
        return m_ConstColor[idx % CombinerUserShaderConstantColor];
    }

    //! @brief バリエーション識別用コードを取得します。
    //!
    //! @brief return バリエーション識別用コードを返します。
    //!
    const uint32_t* GetKeyCode() const
    {
        return &m_KeyCode[0];
    }

    //! @brief バリエーション識別用コードを設定します。
    //!
    //! @param[in] pKeyCode バリエーション識別コードのポインタです。
    //! @param[in] size     バリエーション識別コードのサイズです。
    //!
    //! @details
    //! size がDetailedCombinerStageBitsCountWithVariationTable より小さい場合に、設定以外のメモリには 0 が設定されます。
    //!
    void SetKeyCode(uint32_t* pKeyCode, size_t size)
    {
        NN_SDK_ASSERT(size <= sizeof(uint32_t) * DetailedCombinerStageBitsCountWithVariationTable);
        memset(m_KeyCode, 0, sizeof(uint32_t) * DetailedCombinerStageBitsCountWithVariationTable);
        memcpy(m_KeyCode, pKeyCode, size);
    }

    //@}

private:
    //! @brief 識別用コード
    uint32_t m_KeyCode[DetailedCombinerStageBitsCountWithVariationTable];

    //! @brief コンスタントカラーです。
    ResColor m_ConstColor[CombinerUserShaderConstantColor];

};

//---------------------------------------------------------------------------
//! @brief ベクターテクスチャをコントロールする情報を持つ構造体です。
//!
//---------------------------------------------------------------------------
struct ResVectorGraphicsTextureInfo
{
    //! ベクターグラフィックスシーンの再生時刻
    float   time;
    //! ベクターグラフィックステクスチャのクリアカラー
    ResColor    clearColor;
    //! 拡張のための予約領域
    NN_PADDING4;
    //! 拡張のための予約領域
    NN_PADDING4;
};

//@}

//!--------------------------------------------------------------------------*
//! @name アニメーション・リソース
//@{

//---------------------------------------------------------------------------
//! @brief アニメーション・リソースファイルのヘッダです。
//!
//! @details
//! fileHeader の kind には FileSignatureFlan が指定されます。
//!
//! この構造体に以下のデータブロックの組み合わせが続きます。
//!
//! ・ ResAnimationTagBlock\n
//! ・ ResAnimationShareBlock\n
//! ・ ResAnimationBlock
//!
//---------------------------------------------------------------------------
struct ResLayoutAnimationFile
{
    //! ファイルヘッダです。
    nn::font::detail::BinaryFileHeader fileHeader;
};

//---------------------------------------------------------------------------
//! @brief アニメーション区間タグを定義するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindPaneAnimTag が指定されます。
//!
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ char name[]\n
//! ・ ResAnimationGroupRef groups[groupCount]
//!
//! @sa ResLayoutAnimationFile
//!
//---------------------------------------------------------------------------
struct ResAnimationTagBlock
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;

    //! アニメーション区間タグの通し番号です。
    uint16_t          tagOrder;
    //! アニメーション区間タグのグループ数です。
    uint16_t          groupCount;

    //! name へのオフセットです(データブロック先頭から)。
    uint32_t          nameOffset;
    //! groups へのオフセットです(データブロック先頭から)。
    uint32_t          groupsOffset;

    //! 拡張ユーザー情報 へのオフセットです(データブロック先頭から)。存在しない場合は、0 が設定されます。
    uint32_t          userDataListOffset;

    //! アニメーション区間の開始フレームです。
    int16_t          startFrame;
    //! アニメーション区間の終了フレームです。
    int16_t          endFrame;

    //! フラグです(nn::ui2d::AnimTagFlag)。
    uint8_t           flag;
    //! (未使用です)
    char                              padding[3];

    /* Additional Info

    char                name[];
    ResAnimationGroupRef   groups[groupCount];
    ResExtUserDataList     userDataList;
    */
};

//---------------------------------------------------------------------------
//! @brief アニメーション共有情報を定義するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindPaneAnimShare が指定されます。
//!
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ ResAnimationShareInfo animShareInfos[shareCount]
//!
//! @sa ResLayoutAnimationFile
//!
//---------------------------------------------------------------------------
struct ResAnimationShareBlock
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;

    //! animShareInfos へのオフセットです(データブロック先頭から)。
    uint32_t          animShareInfoOffset;

    //! アニメーション共有情報の数です。
    uint16_t          shareCount;
    //! (未使用です)
    char                              padding[2];

    /* Additional Info

    ResAnimationShareInfo  animShareInfos[shareCount];

    */
};

//---------------------------------------------------------------------------
//! @brief アニメーションを定義するデータブロックです。
//!
//! @details
//! blockHeader の kind には DataBlockKindPaneAnimInfo が指定されます。
//!
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ uint32_t fileNameOffsets[fileCount]\n
//! ・ uint8_t  namePool[]\n
//! ・ uint32_t animContOffsets[paneCount]\n
//! ・ ResAnimationContent animCont0\n
//! ・ …
//!
//! fileNameOffsets 配列には参照するテクスチャリソース名への
//! オフセット(fileNameOffsetsより)を格納します。
//!
//! animContOffsets 配列にはアニメーション内容への
//! オフセット(データブロック先頭より)を格納します。
//!
//! @sa ResLayoutAnimationFile
//!
//---------------------------------------------------------------------------
struct ResAnimationBlock
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;
    //! フレーム数です。
    uint16_t          frameSize;
    //! ループする場合は 1 を格納します。
    uint8_t           loop;
    //! (未使用です)
    char                              padding[1];

    //! 参照するテクスチャリソースの数です。
    uint16_t          fileCount;
    //! アニメーション内容の数です。
    uint16_t          animContCount;

    //! animContOffsets へのオフセットです(データブロック先頭から)。
    uint32_t          animContOffsetsOffset;

    /* Additional Info

    uint32_t          fileNameOffsets[fileCount];
    uint8_t                   namePool[];
    uint32_t          animContOffsets[paneCount];

    */
};

//---------------------------------------------------------------------------
//! @brief アニメーション内容を定義します。
//!
//! @details
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ uint32_t animInfoOffsets[num]\n
//! ・ ResAnimationInfo animInfo0\n
//! ・ …
//!
//! @sa ResLayoutAnimationFile
//! @sa ResAnimationBlock
//!
//---------------------------------------------------------------------------
struct ResAnimationContent
{
    //! アニメーションの名前です。
    char                name[MaterialNameStrMax];   // ペイン名とマテリアル名の長い方の文字数分確保

    //! アニメーション情報の数です。
    uint8_t           count;
    //! タイプ(nn::ui2d::AnimContentType)です。
    uint8_t           type;
    //! (未使用です)
    char                              padding[2];

    /* Additional Info

    uint32_t    animInfoOffsets[count];

    */
};

//---------------------------------------------------------------------------
//! @brief アニメーション情報を定義します。
//!
//! @details
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ uint32_t animTargetOffsets[num]\n
//! ・ ResAnimationTarget animTarget0\n
//! ・ …
//!
//! animTargetOffsets 配列は ResAnimationTarget へのオフセット(ResAnimationInfo 先頭から)を格納します。
//!
//! @sa ResLayoutAnimationFile
//! @sa ResAnimationContent
//!
//---------------------------------------------------------------------------
struct ResAnimationInfo
{
    uint32_t         kind;

    //! アニメーション・ターゲットの数です。
    uint8_t           count;
    //! (未使用です)
    char                              padding[3];

    /* Additional Info

    uint32_t          animTargetOffsets[count];

    */
};

//---------------------------------------------------------------------------
//! @brief アニメーション・ターゲットを定義します。
//!
//! @details
//! アニメーションの対象とカーブの情報です。
//!
//! target にはアニメーションの対象に応じて以下の列挙型のいずれかが指定されます。
//!
//! ・ nn::ui2d::AnimTargetPane\n
//! ・ nn::ui2d::AnimTargetVisibility\n
//! ・ nn::ui2d::AnimTargetPaneColor\n
//! ・ nn::ui2d::AnimTargetMatColor\n
//! ・ nn::ui2d::AnimTargetTexSrt\n
//! ・ nn::ui2d::AnimTargetIndirectSrt\n
//! ・ nn::ui2d::AnimTargetTexPattern
//! ・ nn::ui2d::AnimTargetFontShadow
//! ・ nn::ui2d::AnimTargetPerCharacterTransform
//! ・ nn::ui2d::AnimTargetPerCharacterTransformCurve
//! ・ nn::ui2d::AnimTargetWindow
//! ・ nn::ui2d::AnimTargetMaskTexMtxSrt
//! ・ nn::ui2d::AnimTargetDropShadow
//! ・ nn::ui2d::AnimTargetProceduralShape
//!
//! この構造体に以下のデータ構造が続きます。
//!
//! ・ ResHermiteKey keys[keyCount]
//!
//! または
//!
//! ・ ResStepKey keys[keyCount]
//!
//! @sa ResLayoutAnimationFile
//! @sa ResAnimationInfo
//!
//---------------------------------------------------------------------------
struct ResAnimationTarget
{
    //! アニメーション対象が配列の場合のインデックスです。
    uint8_t           id;
    //! アニメーション対象の属性をあらわす列挙値です。
    uint8_t           target;
    //! カーブの種類です(nn::ui2d::AnimCurve)。
    uint8_t           curveType;
    //! (未使用です)
    char                              padding[1];

    //! キーの数です。
    uint16_t          keyCount;
    //! (未使用です)
    char                              padding2[2];

    //! keys へのオフセットです(AnimationTarget先頭から)。
    uint32_t          keysOffset;

    /* Additional Info

    ResHermiteKey keys[keyCount]|ResStepKey keys[keyCount]|ResParameterizedAnim anim;

    */
};

//---------------------------------------------------------------------------
//! @brief エルミート曲線のキーを定義します。
//!
//! @sa ResLayoutAnimationFile
//! @sa ResAnimationTarget
//!
//---------------------------------------------------------------------------
struct ResHermiteKey
{
    //! フレームです。
    float          frame;
    //! 値です。
    float          value;
    //! 傾きです。
    float          slope;
};

//---------------------------------------------------------------------------
//! @brief 階段状に変化するアニメーションカーブのキーを定義します。
//!
//! @sa ResLayoutAnimationFile
//! @sa ResAnimationTarget
//!
//---------------------------------------------------------------------------
struct ResStepKey
{
    //! フレームです。
    float          frame;
    //! 値です。
    uint16_t          value;
    //! (未使用です)
    uint16_t          padding[1];
};

//---------------------------------------------------------------------------
//! @brief パラメタライズドアニメーションを定義します。
//---------------------------------------------------------------------------
struct ResParameterizedAnim
{
    //! パラメタライズドアニメーションの数です。同じターゲットに複数のアニメーションが設定できます。
    uint16_t parameterizedAnimCount;
    //! 未使用です
    char padding[2];

    /* Additional Info
    uint32_t parameterizedAnimOffsets[parameterizedAnimCount];
    */
};

//---------------------------------------------------------------------------
//! @brief パラメタライズドアニメーションを定義します。
//---------------------------------------------------------------------------
struct ResParameterizedAnimParameterValue
{
    //! 初期値です。
    float startValue;
    //! 目標値です。
    float targetValue;
};

//---------------------------------------------------------------------------
//! @brief パラメタライズドアニメーションイベントを定義します。（ParameterizedAnimType_Event で利用）
//---------------------------------------------------------------------------
struct ResParameterizedAnimParameterEvent
{
    //! イベントパラメーターへのポインターです。
    const void* pParam1;
    //! イベントパラメーターへのポインターです。
    const void* pParam2;
    //! イベントパラメーターへのポインターです。
    const void* pParam3;
};

//---------------------------------------------------------------------------
//! @brief パラメタライズドアニメーションのパラメータです。
//---------------------------------------------------------------------------
struct ResParameterizedAnimParameter
{
    //! パラメタライズドアニメーションの種類です。
    uint8_t parameterizedAnimType; // ParameterizedAnimType
    //! (未使用です)
    uint8_t padding[3];

    union
    {
        ResParameterizedAnimParameterValue  value;
        ResParameterizedAnimParameterEvent* pEvent;
    };

    //! アニメーションフレームのオフセットです。
    float offset;
    //! アニメーションフレームの長さです。
    float duration;
};

//---------------------------------------------------------------------------
//! @brief フィルレート最適化のシンプル AABB メッシュデータです。
//!
//---------------------------------------------------------------------------
struct SystemDataSimpleAABB
{
    //!  システムデータタイプ
    uint32_t    type;
    //! AABB の幅
    float       width;
    //! AABB の高さ
    float       height;
    //! AABB の横方向のオフセット
    float       offsetX;
    //! AABB の縦方向のオフセット
    float       offsetY;
};

//---------------------------------------------------------------------------
//! @brief フィルレート最適化のシンプル OBB メッシュデータです。
//!
//---------------------------------------------------------------------------
struct SystemDataSimpleOBB
{
    //!  システムデータタイプ
    uint32_t    type;
    //! OBB の幅
    float       width;
    //! OBB の高さ
    float       height;
    //! OBB の横方向のオフセット
    float       offsetX;
    //! OBB の縦方向のオフセット
    float       offsetY;
    //! OBB の傾きを表す Z 軸回転(Rad)
    float       rotateZ;
};

//---------------------------------------------------------------------------
//! @brief 整列ペインの追加情報です。
//!
//---------------------------------------------------------------------------
struct SystemDataAlignmentExInfo
{
    //! @brief 整列ペインのフラグを表す列挙子
    //!
    enum AlignmentFlags
    {
        //! 整列マージンの有効無効
        AlignmentFlags_AlignmentMarginEnabled,
        //! 整列ペインの処理を除外
        AlignmentFlags_AlignmentIgnore,
        //! 整列ペイン配下の Null ペイン判定
        AlignmentFlags_AlignmentNullPane,

        AlignmentFlags_Max
    };

    //! システムデータタイプ
    uint32_t    type;

    uint32_t    flags;

    //! 整列マージン値
    float alignmentMargin;
};

//---------------------------------------------------------------------------
//! @brief マスク機能のためのデータです。
//!
//---------------------------------------------------------------------------
struct SystemDataMaskTexture
{
    //! マスクバイナリに含まれるフラグの値
    enum MaskFlag
    {
        //! マスクをする・されるの設定フラグ。true だと描画結果で設定テクスチャをマスク、false だとその逆になります。
        MaskFlag_MaskWithPane,
        //! 静的レンダリングが有効
        MaskFlag_StaticRenderingEnabled,

        MaskFlag_Max
    };

    //! システムデータタイプ
    uint32_t    type;

    //! マスクをする・されるの設定フラグ。true だと描画結果で設定テクスチャをマスク、false だとその逆になります。
    uint8_t     flags;

    //! パディング
    uint8_t     padding[3];

    //! マスク用として設定されたテクスチャの TexMap
    ResTexMap   resMaskTexMap;
    //! マスク用として設定されたテクスチャの追加情報
    uint32_t    resMaskTexMapAdditionalInfo;
    //! マスクするキャプチャテクスチャの TexMap
    ResTexMap   resMaskCaptureTexMap;
    //! マスクするテクスチャの追加情報
    uint32_t    resMaskCaptureTexMapAdditionalInfo;

    //! マスクテクスチャのテクスチャ行列
    ResTexSrt   maskTexSrt;
};

//---------------------------------------------------------------------------
//! @brief ドロップシャドウ機能のためのデータです。
//!
//---------------------------------------------------------------------------
struct SystemDataDropShadow
{
    //! ドロップシャドウ系エフェクトのタイプ
    enum DropShadowType
    {
        //! 境界線
        DropShadowType_Stroke,
        //! 光彩
        DropShadowType_OuterGlow,
        //! ドロップシャドウ
        DropShadowType_DropShadow,

        DropShadowType_Max
    };

    //! ドロップシャドウバイナリに含まれるフラグの値
    enum DropShadowFlag
    {
        //! 境界線が有効
        DropShadowFlag_StrokeEnabled,
        //! 光彩が有効
        DropShadowFlag_OuterGlowEnabled,
        //! ドロップシャドウが有効
        DropShadowFlag_DropShadowEnabled,
        //! ノックアウトが有効
        DropShadowFlag_KnockoutEnabled,
        //! エフェクトのみ描画
        DropShadowFlag_DrawEffectOnlyEnabled,
        //! キャッシュしたレンダリング結果を使用する
        DropShadowFlag_StaticRendering,

        DropShadowFlag_Max
    };



    //! システムデータタイプ
    uint32_t    type;
    //! ドロップシャドウのインプットとなるキャプチャされたペインのテクスチャ
    ResTexMap   resSourceCapturedImage;

    //! 使用フラグ
    uint8_t     flags;
    //! パディング
    uint8_t     padding[3];

    //! ドロップシャドウで使用するぼかしサイズの最大値
    uint8_t     blurSizeMax;
    //! 各エフェクトのブレンドモード
    uint8_t     blendType[DropShadowType_Max];

    //! 拡張のための予約領域
    uint32_t    padding2[4];

    // 以下はアニメーションする際に先頭から AnimTargetDropShadow の並びに合わせて float のポインタとしてアクセスされるため並びを合わせておく

    //! 境界線のサイズ
    float       strokeSize;
    //! 境界線のカラー
    float       strokeColor[4];
    //! 光彩のカラー
    float       outerGlowColor[4];
    //! 光彩のスプレッド
    float       outerGlowSpread;
    //! 光彩のサイズ
    float       outerGlowSize;

    //! ドロップシャドウの色
    float       dropShadowColor[4];
    //! ドロップシャドウの描画位置を調整する角度
    float       dropShadowAngle;
    //! ドロップシャドウの描画位置オフセット
    float       dropShadowDistance;
    //! ドロップシャドウのスプレッド
    float       dropShadowSpread;
    //! ドロップシャドウのサイズ
    float       dropShadowSize;

    //! 拡張のための予約領域
    uint32_t    padding3[4];
};

//---------------------------------------------------------------------------
//! @brief 角丸機能のためのデータです。
//!
//---------------------------------------------------------------------------
struct SystemDataProceduralShape
{
    //! グラデーションオーバーレイのコントロールポイント数
    static const int GradationOverlayControlPointCount = 4;

    //! 角丸エフェクトのタイプ
    enum ProceduralShapeEffectType
    {
        //! 境界線(内側)
        ProceduralShapeEffectType_InnerStroke,
        //! シャドウ(内側)
        ProceduralShapeEffectType_InnerShadow,
        //! カラーオーバーレイ
        ProceduralShapeEffectType_ColorOverlay,
        //! グラデーションオーバーレイ
        ProceduralShapeEffectType_GradationOverlay,
        //! ドロップシャドウ
        ProceduralShapeEffectType_OuterShadow,

        ProceduralShapeEffectType_Max
    };

    //! 角丸バイナリに含まれるフラグの値
    enum ProceduralShapeFlag
    {
        //! 境界線(内側)が有効
        ProceduralShapeFlag_InnerStrokeEnabled,
        //! シャドウ(内側)が有効
        ProceduralShapeFlag_InnerShadowEnabled,
        //! カラーオーバーレイが有効
        ProceduralShapeFlag_ColorOverlayEnabled,
        //! グラデーションオーバーレイが有効
        ProceduralShapeFlag_GradationOverlayEnabled,
        //! ドロップシャドウが有効
        ProceduralShapeFlag_DropShadowEnabled,
        //! ドロップシャドウのノックアウト処理が有効
        ProceduralShapeFlag_DropShadowKnockoutEnabled,
        //! 角個別のパラメータ設定が有効
        ProceduralShapeFlag_SeparateCornerParamsEnabled,

        ProceduralShapeFlag_Max
    };



    //! システムデータタイプ
    uint32_t    type;

    //! 使用フラグ
    uint8_t     flags;
    //! 境界線(内側)のブレンドモード
    uint8_t     innerStrokeBlendType;
    //! シャドウ(内側)のブレンドモード
    uint8_t     innerShadowBlendType;
    //! シャドウ(内側)の影の減衰タイプ
    uint8_t     innerShadowAttenuationType;
    //! カラーオーバーレイのブレンドモード
    uint8_t     colorOverlayBlendType;
    //! グラデーションオーバーレイのブレンドモード
    uint8_t     gradationOverlayBlendType;
    //! ドロップシャドウのブレンドモード
    uint8_t     outerShadowBlendType;
    //! ドロップシャドウの影の減衰タイプ
    uint8_t     outerShadowAttenuationType;

    //! 拡張のための予約領域
    uint32_t    padding[4];

    // 以下はアニメーションする際に先頭から AnimTargetDropShadow の並びに合わせて float のポインタとしてアクセスされるため並びを合わせておく

    //! 角丸の形状設定
    float       exp[ProceduralShapeSeparateParamType_Max];
    //! 角丸の半径
    float       radius[ProceduralShapeSeparateParamType_Max];
    //! 境界線(内側)のサイズ
    float       innerStrokeSize;
    //! 境界線(内側)のカラー
    float       innerStrokeColor[4];

    //! シャドウ(内側)のカラー
    float       innerShadowColor[4];
    //! シャドウ(内側)の角度
    float       innerShadowAngle;
    //! シャドウ(内側)の距離
    float       innerShadowDistance;
    //! シャドウ(内側)のサイズ
    float       innerShadowSize;

    //! カラーオーバーレイのカラー
    float       colorOverlayColor[4];

    //! グラデーションオーバーレイのコントロールポイント
    float       gradationOverlayControlPoint[GradationOverlayControlPointCount];
    //! グラデーションオーバーレイのカラー
    float       gradationOverlayColor[GradationOverlayControlPointCount][4];
    //! グラデーションオーバーレイの角度
    float       gradationOverlayAngle;

    //! ドロップシャドウのカラー
    float       outerShadowColor[4];
    //! ドロップシャドウの角度
    float       outerShadowAngle;
    //! ドロップシャドウの距離
    float       outerShadowDistance;
    //! ドロップシャドウのサイズ
    float       outerShadowSize;

    //! 拡張のための予約領域
    uint32_t    padding2[4];
};

//---------------------------------------------------------------------------
//! @brief サブスクリーン情報です。
struct SystemDataSubScreen
{
    //! @brief 未定義を表す番号です。
    static const int Undefined = -1;
    //! @brief 前のスクリーンを表す番号です。
    static const int PrevScreen = -2;
    //! @brief 次のスクリーンを表す番号です。
    static const int NextScreen = -3;

    //! @brief 部品レイアウト名です（仮に固定長にしています）。
    char partsName[ResourceNameStrMax];

    // スクリーン中のボタン番号をキーにして、
    // 次の SubScreen を探すためのテーブルです。
    // nextScreenIdx = nextSubScreenIndexTable[buttonIdx];
    // nextScreen = SystemDataScreen.subScreens[nextScreenIdx];
    int8_t nextSubScreenIndexTable[8];
};

//---------------------------------------------------------------------------
//! @brief スクリーン情報です。
struct SystemDataScreen
{
    //! システムデータタイプ
    uint32_t    type;

    //! サブスクリーン配列です。
    SystemDataSubScreen subScreens[8];

    //! サブスクリーンの数です。
    int                 subScreenCount;
};

//---------------------------------------------------------------------------
//! @brief ペインのコピー時に動的に生成されたテクスチャをコピーするために必要な情報です。
//!
//! @deprecated 使用されなくなったため廃止される予定です。
//!
//---------------------------------------------------------------------------
struct NN_DEPRECATED DynamicGenerateTextureCopyInfo
{
    //! 動的生成テクスチャ使用ビットフラグ
    uint8_t     useFlags;
    //! 動的生成テクスチャを適用するマテリアルを識別するための ID。この ID は各ペインごとの事情に応じて適当に使用されます。
    uint8_t     targetId;
    //! パディング
    uint8_t     padding[2];
    //! 使用動的生成テクスチャ名データへの自分自身からのオフセットリスト
    char        names[TexMapMax][CaptureTexturePathMax];
};

//---------------------------------------------------------------------------
//! @brief 動的生成テクスチャをランタイムでコピーするための情報です。
//!
//! この構造体に以下のデータ構造が続きます。
//!
//! - DynamicGenerateTextureCopyInfo  info[count]
//!
//! @deprecated 使用されなくなったため廃止される予定です。
//!
//---------------------------------------------------------------------------
struct NN_DEPRECATED SystemDataCopyCaptureTextureInfo
{
    //! システムデータタイプ
    uint32_t    type;

    //! データ数
    uint32_t    count;

    /* Additional Info
    DynamicGenerateTextureCopyInfo  info[count]
    */
};

//---------------------------------------------------------------------------
//! @brief ペインエフェクト用のランタイムデータです。
//!
//---------------------------------------------------------------------------
struct SystemDataPaneEffectRuntimeInfo
{
    //! システムデータタイプ
    uint32_t    type;

    //! ペインエフェクトのインスタンス
    detail::PaneEffect* pPaneEffect;
};

//---------------------------------------------------------------------------
//! @brief 角丸用のランタイムデータです。
//!
//---------------------------------------------------------------------------
struct SystemDataProceduralShapeRuntimeInfo
{
    //! システムデータタイプ
    uint32_t    type;

    //! 角丸用コンスタントバッファのスロット番号です。
    int         constantBufferSlot;
    //! 角丸用コンスタントバッファの GPU バッファオフセットです。
    uint32_t    constantBufferOffset;

    //! ドロップシャドウ用の頂点シェーダーコンスタントバッファオフセット
    int         dropShadowVertexShaderConstantBufferOffset;
    //! ドロップシャドウ用のピクセルシェーダーコンスタントバッファオフセット
    int         dropShadowPixelShaderConstantBufferOffset;
};

//---------------------------------------------------------------------------
//! @brief レイアウトに関連づくアニメーション区間タグ名です。
//!
//---------------------------------------------------------------------------
struct SystemDataLayoutTagNames
{
    //! システムデータタイプ
    uint32_t    type;

    //! レイアウトに関連づくアニメーション区間タグ名の数です。
    uint32_t     count;

    /* Additional Info
    // count 分 文字列への（SystemDataLayoutTagNamesの先頭を基準とした）オフセットが続きます。
    uint32_t*   pNameOffsetArray;
    // count 分 null終端された文字列が続きます。終端は 64byte 整列されます。
    char*       pNameBuffer;
    */
};

class StateMachine;

//---------------------------------------------------------------------------
//! @brief ステートマシンです。
//---------------------------------------------------------------------------
struct SystemDataLayoutStateMachine
{
    //! システムデータタイプ
    uint32_t    type;
    nn::ui2d::StateMachine* pStateMachine;
};

//---------------------------------------------------------------------------
//! @brief ステートマシンの特徴パラメーターの種類です。
//---------------------------------------------------------------------------
enum StateMachineFeatureParameterKind
{
    StateMachineFeatureParameterKind_None,
    StateMachineFeatureParameterKind_Position,
    StateMachineFeatureParameterKind_Scale,
    StateMachineFeatureParameterKind_Rotate,
    StateMachineFeatureParameterKind_Size,
    StateMachineFeatureParameterKind_Visibility,
    StateMachineFeatureParameterKind_PaneAlpha,
    StateMachineFeatureParameterKind_PerCharacterTransform,
    StateMachineFeatureParameterKind_MaskTexS,
    StateMachineFeatureParameterKind_MaskTexR,
    StateMachineFeatureParameterKind_MaskTexT,
    StateMachineFeatureParameterKind_RoundRect,

    StateMachineFeatureParameterKind_WhiteColor,
    StateMachineFeatureParameterKind_BlackColor,

    StateMachineFeatureParameterKind_Tex0S,
    StateMachineFeatureParameterKind_Tex0R,
    StateMachineFeatureParameterKind_Tex0T,

    StateMachineFeatureParameterKind_Tex1S,
    StateMachineFeatureParameterKind_Tex1R,
    StateMachineFeatureParameterKind_Tex1T,

    StateMachineFeatureParameterKind_Tex2S,
    StateMachineFeatureParameterKind_Tex2R,
    StateMachineFeatureParameterKind_Tex2T,

    StateMachineFeatureParameterKind_Indirect,
    StateMachineFeatureParameterKind_TexturePattern,

    StateMachineFeatureParameterKind_StateMachineEvent,
    StateMachineFeatureParameterKind_PartsStateMachineLayer,
    StateMachineFeatureParameterKind_Max

    //　flyt の FeatureParameterKind と同期して編集します。
};

//---------------------------------------------------------------------------
//! @brief ステートマシンのトリガーの種類です。
//---------------------------------------------------------------------------
enum StateMachineTriggerKind
{
    StateMachineTriggerKind_IsTick,
    StateMachineTriggerKind_IsHit,
    StateMachineTriggerKind_IsNoHit,
    StateMachineTriggerKind_IsDecided,
    StateMachineTriggerKind_IsStateTransitionCompleted,
    StateMachineTriggerKind_IsSubStateTransitionCompleted,
    StateMachineTriggerKind_IsVariableChanged,
    StateMachineTriggerKind_IsStateChangeRequested,
    StateMachineTriggerKind_IsButtonDecided,
    StateMachineTriggerKind_Max
};

//---------------------------------------------------------------------------
//! @brief ステートマシンのトリガーの種類です。
//---------------------------------------------------------------------------
enum StateMachineLayerType
{
    StateMachineLayerType_Normal,
    StateMachineLayerType_StateDirect,
    StateMachineLayerType_TimeLineDirect,
    StateMachineLayerType_Max
};

//---------------------------------------------------------------------------
//! @brief ステートマシンの名前文字数です。
//---------------------------------------------------------------------------
const int CountOfStateMachineName = 32;

//---------------------------------------------------------------------------
//! @brief ステートマシンの特徴パラメーターです。
//---------------------------------------------------------------------------
struct ResStateParameterStore
{
    char targetName[CountOfStateMachineName];
    uint32_t type;

    float x;
    float y;
    float z;
    float w;

    uint32_t padding;
};

//---------------------------------------------------------------------------
//! @brief ステートマシンの特徴パラメーターの文字列オプションです。
//! 現状は、使わない場合も含めて必ず固定で埋まっています。
//! 利用しない項目は、ゼロで埋められています。
//---------------------------------------------------------------------------
struct ResStateParameterNameOption
{
    char targetNameX[CountOfStateMachineName];
    char targetNameY[CountOfStateMachineName];
    char targetNameZ[CountOfStateMachineName];
    char targetNameW[CountOfStateMachineName];
};

//---------------------------------------------------------------------------
//! @brief ステートマシンの変数です。
//---------------------------------------------------------------------------
struct ResStateVariableDescriptions
{
    char name[CountOfStateMachineName];

    int32_t type;

    float defaultValue;
    float minValue;
    float maxValue;
    float reserved;

    uint32_t padding;
};

//---------------------------------------------------------------------------
//! @brief ステートマシンの変数です。
//---------------------------------------------------------------------------
struct ResStateVariable
{
    char name[CountOfStateMachineName];

    int32_t type;

    float value0;
    float value1;
    float value2;
    float value3;

    uint32_t padding;
};

//---------------------------------------------------------------------------
//! @brief ステートマシン変数の同期設定です。
//---------------------------------------------------------------------------
struct ResStateVariableConnector
{
    char inStateMachineName[CountOfStateMachineName];
    char inStateVariablesName[CountOfStateMachineName];
    char outStateMachineName[CountOfStateMachineName];
    char outStateVariablesName[CountOfStateMachineName];
};

//---------------------------------------------------------------------------
//! @brief ステートマシンの遷移トラックイベントです。
//---------------------------------------------------------------------------
struct ResStateTransitionTrackKeyEvent
{
    float delay;

    uint8_t kind;
    uint8_t padding1;
    uint16_t padding2;

    char param1[CountOfStateMachineName];
    char param2[CountOfStateMachineName];
    char param3[CountOfStateMachineName];
};

//---------------------------------------------------------------------------
//! @brief ステートマシンの遷移トラックキーです。
//---------------------------------------------------------------------------
struct ResStateTransitionTrackKey
{
    float offset;
    float easingExtraParameter;

    uint8_t easingType;
    uint8_t padding1;
    uint16_t padding2;

    ResStateParameterStore          keyParameter;
    ResStateTransitionTrackKeyEvent keyEvent;
};

//---------------------------------------------------------------------------
//! @brief ステートマシンの遷移トラック（特徴パラメーター毎の遷移情報）です。
//---------------------------------------------------------------------------
struct ResStateTransitionTrack
{
    char name[CountOfStateMachineName];

    float offset;
    float duration;
    float easingExtraParameter;

    uint8_t easingType;
    uint8_t animationKind;
    uint16_t padding1;

    nn::util::BinTPtr<ResStateTransitionTrackKey> pKeys;
    uint16_t keyCount;

    uint16_t padding2;
    uint32_t padding3;
};

//---------------------------------------------------------------------------
//! @brief ステートマシンの遷移トリガーです。
//---------------------------------------------------------------------------
struct ResStateTransitionTrigger
{
    int32_t triggerKind;
    int32_t stateMachineOperator;

    nn::util::BinTPtr<ResStateVariable> pVariableArray;

    uint16_t variableCount;
    uint16_t padding1;
    uint32_t padding2;
};

//---------------------------------------------------------------------------
//! @brief ステートマシンの遷移です。
//---------------------------------------------------------------------------
struct ResStateTransition
{
    float offset;
    float duration;
    float totalDuration;
    float easingExtraParameter;
    uint8_t isCancelable;
    uint8_t isLoop;
    uint8_t easingType;
    uint8_t padding1;

    uint32_t padding2;

    char startName[CountOfStateMachineName];
    char endName[CountOfStateMachineName];

    // Track配列を持つようにする。
    nn::util::BinTPtr<ResStateTransitionTrack> pTracks;
    uint16_t trackCount;
    uint16_t padding3;
    uint32_t padding4;

    // Trigger を一つだけべたで持つ。
    ResStateTransitionTrigger trigger;
};

//---------------------------------------------------------------------------
//! @brief ステートマシンの状態です。
//---------------------------------------------------------------------------
struct ResState
{
    char name[CountOfStateMachineName];

    nn::util::BinTPtr<ResStateParameterStore> pParameters;
    nn::util::BinTPtr<ResStateParameterNameOption> pParameterNameOptions;

    uint16_t parameterCount;
    uint16_t padding1;
    uint32_t padding2;
};

//---------------------------------------------------------------------------
//! @brief ステートマシンの状態レイヤーです。
//---------------------------------------------------------------------------
struct ResStateLayer
{
    char name[CountOfStateMachineName];
    char initialStateName[CountOfStateMachineName];
    char boundingPaneName[CountOfStateMachineName];

    uint8_t  layerType; // StateMachineLayerType
    uint8_t  pad1;
    uint16_t pad2;
    int32_t  initialTransitionIndex;

    nn::util::BinTPtr<ResState> pStates;
    nn::util::BinTPtr<ResStateTransition> pTransitions;

    uint16_t stateCount;
    uint16_t transitionCount;

    int32_t  padding2;
};

//---------------------------------------------------------------------------
//! @brief  部品ペインのステートマシンレイヤープロパティ値です。
//---------------------------------------------------------------------------
struct ResPartsStateMachineLayerValue
{
    //! 部品ペインのステートマシンレイヤープロパティ値です。
    float   value;
};

//---------------------------------------------------------------------------
//! @brief ステートマシンです。
//---------------------------------------------------------------------------
struct ResStateMachine
{
    //! ヘッダです。
    nn::font::detail::BinaryBlockHeader blockHeader;

    char name[CountOfStateMachineName];

    nn::util::BinTPtr<ResStateLayer> pLayers;
    nn::util::BinTPtr<ResStateVariableDescriptions> pVariables;
    nn::util::BinTPtr<ResStateVariableConnector> pVariablesConnectors;

    uint16_t layerCount;
    uint16_t variableCount;
    uint16_t variableConnectorCount;

    uint16_t isRelocated;
};

//---------------------------------------------------------------------------
//! @brief システム用拡張ユーザーデータ参照用テーブルです。
//---------------------------------------------------------------------------
struct SystemDataReferenceTable
{
    //! テーブルの要素の最大数(メモリを節約するために必要最低限数使用しているが、将来的にシステム用拡張ユーザデータの最大数である 32 まで増加する可能性がある)。
    static const uint32_t TableElementCountMax = 12;

    //! システムデータタイプ
    uint32_t    type;

    //! 参照テーブル
    int8_t      table[TableElementCountMax];
};


//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのレイヤータイプです。
//---------------------------------------------------------------------------
enum BnvgLayerType
{
    //! プリコンポ
    BnvgLayerType_Precomp,
    //! (非対応)
    BnvgLayerType_Solid,
    //! (非対応)
    BnvgLayerType_Image,
    //! ヌルレイヤー
    BnvgLayerType_Null,
    //! シェイプレイヤー
    BnvgLayerType_Shape,
    //! (非対応)
    BnvgLayerType_Text,
    //! マスク情報のための仮想レイヤー情報
    BnvgLayerType_MaskInfoLayer,

    BnvgLayerType_Max
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのレイヤーに関するフラグ情報です。
//---------------------------------------------------------------------------
enum BnvgLayerBasicFlag
{
    //! 3D レイヤー
    BnvgLayerBasicFlag_3D = 0,
    //! マスク使用フラグ
    BnvgLayerBasicFlag_MaskEnabled,

    BnvgLayerBasicFlag_Max
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのパス情報タイプです。
//---------------------------------------------------------------------------
enum BnvgShapePathType
{
    //! パス情報
    BnvgShapePathType_Path,
    //! 基本形状(円)
    BnvgShapePathType_Ellipse,
    //! 基本形状(長方形)
    BnvgShapePathType_Rect,
    //! 基本形状(多角形)
    BnvgShapePathType_Star,

    BnvgShapePathType_Max
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのパスデータに関するフラグです。
//---------------------------------------------------------------------------
enum BnvgShapePathInfoFlag
{
    //! パスが閉じているかどうか
    BnvgShapePathInfoFlag_Closed,

    BnvgShapePathInfoFlag_Max
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのパス全般に関するフラグです。
//---------------------------------------------------------------------------
enum BnvgShapePathFlag
{
    //! パスの非表示フラグ
    BnvgShapePathFlag_Hide,
    //! パスの方向転換
    BnvgShapePathFlag_Inverse,

    BnvgShapePathFlag_Max
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データの多角形パスのタイプです。
//---------------------------------------------------------------------------
enum BnvgShapePathStarType
{
    //! 星形
    BnvgShapePathStarType_Star,
    //! 多角形
    BnvgShapePathStarType_Polygon,

    BnvgShapePathStarType_Max
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのシェイプエフェクトタイプです。
//---------------------------------------------------------------------------
enum BnvgShapeEffectType
{
    //! 塗りつぶし
    BnvgShapeEffectType_Fill,
    //! 線
    BnvgShapeEffectType_Stroke,
    //! トリム
    BnvgShapeEffectType_Trim,

    BnvgShapeEffectType_Max
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データの線の終端タイプです。
//---------------------------------------------------------------------------
enum BnvgShapeLineCap
{
    //! バット
    BnvgShapeLineCap_Butt,
    //! ラウンド
    BnvgShapeLineCap_Round,
    //! スクエア
    BnvgShapeLineCap_Square,

    BnvgShapeLineCap_Max
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データの線の結合タイプです。
//---------------------------------------------------------------------------
enum BnvgShapeLineJoin
{
    //! マイター
    BnvgShapeLineJoin_Miter,
    //! ラウンド
    BnvgShapeLineJoin_Round,
    //! ベベル
    BnvgShapeLineJoin_Bevel,

    BnvgShapeLineJoin_Max
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのエフェクト全般に関するフラグです。
//---------------------------------------------------------------------------
enum BnvgShapeEffectFlag
{
    //! エフェクトの非表示フラグ
    BnvgShapeEffectFlag_Hide,

    BnvgShapeEffectFlag_Max
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのマスクモードです。
//---------------------------------------------------------------------------
enum BnvgMaskMode
{
    //! マスクなし
    BnvgMaskMode_None,
    //! 加算
    BnvgMaskMode_Additive,
    //! 減算
    BnvgMaskMode_Subtract,
    //! 交差
    BnvgMaskMode_Intersect,
    //! 明るく
    BnvgMaskMode_Lighten,
    //! 暗く
    BnvgMaskMode_Darken,
    //! 差
    BnvgMaskMode_Difference,

    BnvgMaskMode_Max
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データの基本データヘッダです。
//---------------------------------------------------------------------------
struct ResBnvgInfoHeader
{
    //! 幅
    float   width;
    //! 高さ
    float   height;
    //! フレームレート
    float   frameRate;
    //! 初期フレーム
    float   inPoint;
    //! 最終フレーム
    float   outPoint;
    //! 3d
    bool    is3d;
    //! padding
    NN_PADDING3;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのアニメーションキーデータです。
//---------------------------------------------------------------------------
struct ResBnvgAnimationKey
{
    //! アニメーションキーの時刻
    float   time;
    //! アニメーションカーブのタンジェント
    float   in[2];
    //! アニメーションカーブのタンジェント
    float   out[2];
    //! アニメーションキーの開始時の値
    float   startValue;
    //! アニメーションキーの終了時の値(次のキーの時刻でこの値に変化する)
    float   endValue;
    //! padding
    NN_PADDING4;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのベジェカーブアニメーションキーデータです。
//---------------------------------------------------------------------------
struct ResBnvgBezierAnimationKey
{
    //! アニメーションキーの時刻
    float   time;
    //! アニメーションカーブのタンジェント
    float   in[2];
    //! アニメーションカーブのタンジェント
    float   out[2];
    //! アニメーションキーのコントロールポイント 0
    float   controlPoint0[2];
    //! アニメーションキーのコントロールポイント 1
    float   controlPoint1[2];
    //! コントロールポイント 0 のアウトタンジェント
    float   valueOutTangent[2];
    //! コントロールポイント 1 のインタンジェント
    float   valueInTangent[2];
    //! padding
    NN_PADDING4;
};


//---------------------------------------------------------------------------
//! @brief  Vector Graphics のアニメーション可能な値データです。
//---------------------------------------------------------------------------
struct ResBnvgAnimatableValue
{
    //! アニメーションキーの数です。1 の時は value に値が設定され、それ以上の場合は keys にキーフレームデータへのポインタが設定されます。
    uint16_t    keyCount;
    //! padding
    NN_PADDING6;

    //! 値を評価するためのデータです。
    union
    {
        //! 定数
        float   value;
        //! アニメーションキーへのポインタ
        nn::util::BinTPtr<ResBnvgAnimationKey>    keys;
    };
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics のベジェアニメーション可能な値データです。
//---------------------------------------------------------------------------
struct ResBnvgBezierAnimatableValue2D
{
    //! アニメーションキーの数です。1 の時は value に値が設定され、それ以上の場合は keys に値が設定されます。
    uint16_t    keyCount;
    //! padding
    NN_PADDING6;

    //! 値を評価するためのデータです。
    union
    {
        //! 定数
        float   value[2];
        //! アニメーションキーへのポインタ
        nn::util::BinTPtr<ResBnvgBezierAnimationKey>    keys;
    };
};


//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのレイヤーの姿勢データです。
//---------------------------------------------------------------------------
struct ResBnvgTransform
{
    //! 位置の情報(アニメーションの場合はベジェカーブが設定されます)
    ResBnvgBezierAnimatableValue2D    position;
    //! 位置の情報(アニメーションの場合はベジェカーブが設定されます)
    ResBnvgBezierAnimatableValue2D    anchorPoint;
    //! スケール
    ResBnvgAnimatableValue    scale[3];
    //! Z 軸回転
    ResBnvgAnimatableValue    rotation;
    //! 透明度
    ResBnvgAnimatableValue    opacity;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのレイヤー基本情報です。
//---------------------------------------------------------------------------
struct ResBnvgLayerBasicInfo
{
    //! レイヤータイプです。BnvgLayerType を指定します。
    BnvgLayerType  type;
    //! padding
    NN_PADDING4;

    //! 変換
    ResBnvgTransform  transform;

    //! index
    int32_t     index;
    //! In Point
    float       inPoint;
    //! Out Point
    float       outPoint;
    //! Start Time
    float       startTime;
    //! Streatch
    float       stretch;
    //! Parent
    int32_t     parent;
    //! Auto Oriented
    bool    autoOriented;
    //! 各種フラグ情報(旧 3D フラグ)
    uint8_t     flags;
    //! 参照するマスクデータの MaskInfoLayer 内でのインデックス。
    uint16_t    maskSetIndex;
    //! padding2
    NN_PADDING4;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのシェイプコントロールポイントデータです。
//---------------------------------------------------------------------------
struct ResBnvgShapeControlPoint
{
    //! コントロールポイントのインタンジェントです。
    ResBnvgAnimatableValue   in[2];
    //! コントロールポイントのアウトタンジェントです。
    ResBnvgAnimatableValue   out[2];
    //! コントロールポイントの値です。
    ResBnvgAnimatableValue   v[2];
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのシェイプ内パスを表すデータです。
//---------------------------------------------------------------------------
struct ResBnvgShapePathInfo
{
    //! コントロールポイントの数です。
    uint32_t    controlPointCount;
    //! フラグです。BnvgShapePathInfoType を指定します。
    uint8_t     flags;
    //! padding
    NN_PADDING3;
    //! コントロールポイントへのポインタです。
    nn::util::BinTPtr<ResBnvgShapeControlPoint>  pControlPoints;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのパスの基本データです。
//---------------------------------------------------------------------------
struct ResBnvgShapePath
{
    //! パス情報のタイプです。BnvgShapePathType を指定します。
    uint8_t     type;
    //! シェイプの状態に関するフラグです。BnvgShapePathFlag を指定します。
    uint8_t     flags;
    //! パスに適用するエフェクトの開始インデックス
    uint16_t    appliedEffectStartIndex;
    //! パスに対して BnvgShapeEffectType のどれが有効かビットフラグで指定します。
    uint16_t    appliedEffectFlags;
    //! pading
    NN_PADDING4;
    //! padding
    NN_PADDING8;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データの実際にパス情報を持っているデータです。
//---------------------------------------------------------------------------
struct ResBnvgShapePathData
{
    //! パス情報の基本情報です。
    ResBnvgShapePath    basicInfo;

    //! パス情報へのポインタです。
    ResBnvgShapePathInfo    pathData;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データの楕円形パスのデータです。
//---------------------------------------------------------------------------
struct ResBnvgShapePathEllipse
{
    //! パス情報の基本情報です。
    ResBnvgShapePath    basicInfo;

    //! After Effect's Direction
    float   direction;
    //! 位置
    ResBnvgAnimatableValue    position[2];
    //! サイズ
    ResBnvgAnimatableValue    size[2];
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データの長方形パスのデータです。
//---------------------------------------------------------------------------
struct ResBnvgShapePathRect
{
    //! パス情報の基本情報です。
    ResBnvgShapePath    basicInfo;

    //! After Effect's Direction
    float   direction;
    //! 位置
    ResBnvgAnimatableValue    position[2];
    //! サイズ
    ResBnvgAnimatableValue    size[2];
    //! 角丸の半径
    ResBnvgAnimatableValue    radius;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データの多角形パスのデータです。
//---------------------------------------------------------------------------
struct ResBnvgShapePathStar
{
    //! パス情報の基本情報です。
    ResBnvgShapePath    basicInfo;

    //! 多角形のタイプ
    BnvgShapePathStarType   starType;
    //! After Effect's Direction
    float   direction;
    //! 位置
    ResBnvgAnimatableValue    position[2];
    //! 回転
    ResBnvgAnimatableValue    rotation;
    //! 頂点数
    ResBnvgAnimatableValue    points;
    //! 外半径
    ResBnvgAnimatableValue    outerRadius;
    //! 外側の丸み
    ResBnvgAnimatableValue    outerRoundness;
    //! 内半径
    ResBnvgAnimatableValue    innerRadius;
    //! 内側の丸み
    ResBnvgAnimatableValue    innerRoundness;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのパスに適用するエフェクトの基本データです。
//---------------------------------------------------------------------------
struct ResBnvgShapeEffect
{
    //! シェイプエフェクトのタイプです。BnvgShapeEffectType を指定します。
    uint8_t     type;
    //! シェイプエフェクトに関するフラグです。
    uint8_t     flags;
    //! pading
    NN_PADDING2;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのパスを塗りつぶすエフェクトです。
//---------------------------------------------------------------------------
struct ResBnvgShapeEffectFill
{
    //! エフェクトの基本情報です。
    ResBnvgShapeEffect  basicInfo;

    //! Fill カラー
    ResBnvgAnimatableValue    fillColor[4];
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのパス上に線を描画するエフェクトです。
//---------------------------------------------------------------------------
struct ResBnvgShapeEffectStroke
{
    //! エフェクトの基本情報です。
    ResBnvgShapeEffect  basicInfo;

    //! 線の終端タイプ(非対応)
    BnvgShapeLineCap    lineCap;
    //! 線の結合タイプ
    BnvgShapeLineJoin   lineJoin;

    //! 線の太さ
    ResBnvgAnimatableValue    width;
    //! 結合タイプがマイターの時の制限値
    ResBnvgAnimatableValue    miterLimit;
    //! Stroke カラー
    ResBnvgAnimatableValue    color[4];
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのパスの一部を表示するエフェクトです。
//---------------------------------------------------------------------------
struct ResBnvgShapeEffectTrim
{
    //! エフェクトの基本情報です。
    ResBnvgShapeEffect  basicInfo;

    //! トリム開始位置
    ResBnvgAnimatableValue    start;
    //! トリム終了位置
    ResBnvgAnimatableValue    end;
    //! トリムオフセット
    ResBnvgAnimatableValue    offset;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのシェイプデータです。
//---------------------------------------------------------------------------
struct ResBnvgShapeInfo
{
    //! パス情報の数です。
    uint16_t    pathInfoCount;
    //! グループトランスフォームのインデックス
    uint16_t    groupTransformIndex;
    //! padding
    NN_PADDING4;
    //! パスデータへのポインタ。各パスデータは ResBnvgShapePath が先頭に含まれており、type から適切な型を判定してキャストして使用します。
    nn::util::BinTPtr<nn::util::BinTPtr<ResBnvgShapePath> > pPathPtrTable;

    //! エフェクト情報の数です。
    uint16_t    effectCount;
    //! padding
    NN_PADDING6;
    //! エフェクトデータへのポインタ。各エフェクトデータは ResBnvgShapeEffect が先頭に含まれており、type から適切な型を判定してキャストして使用します。
    nn::util::BinTPtr<nn::util::BinTPtr<ResBnvgShapeEffect> >   pEffectPtrTable;

    //! padding
    NN_PADDING8;
    //! padding
    NN_PADDING8;

    //! 姿勢変換
    ResBnvgTransform          transform;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データの Null レイヤーのデータです。
//---------------------------------------------------------------------------
struct ResBnvgNullLayer
{
    //! レイヤー基本情報
    ResBnvgLayerBasicInfo basicInfo;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのレイヤー内グループ階層構造を表すデータです。
//---------------------------------------------------------------------------
struct ResBnvgGroupTransform
{
    //! 親トランスフォームの ResBnvgShapeLayer::pGroupTransform でのインデックスです。
    int     parentIndex;
    //! 自分自身のトランスフォームです。
    ResBnvgTransform    transform;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのマスク情報です。
//---------------------------------------------------------------------------
struct ResBnvgMaskInfo
{
    //! マスク反転フラグ
    bool    inverse;
    //! padding
    NN_PADDING3;
    //! 半透明マスクのブレンドモード
    BnvgMaskMode        mode;

    //! マスクの形状を表すパスデータ
    ResBnvgShapePathInfo   pathData;
    //! マスクの不透明度
    ResBnvgAnimatableValue  opacity;
    //! マスクの拡張データ
    ResBnvgAnimatableValue  extend;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのマスク情報セットです。
//---------------------------------------------------------------------------
struct ResBnvgMaskInfoSet
{
    //! マスク情報
    uint16_t    maskCount;
    //! padding
    NN_PADDING6;
    //! マスク情報リスト
    nn::util::BinTPtr<ResBnvgMaskInfo>      pMaskData;
};

//---------------------------------------------------------------------------
//! @brief マスク情報用仮想レイヤー。マスク情報を LayerBasicInfo へ追加できないため、仮想的にレイヤーデータとしてデータを作成し LayerBasicInfo から参照します。
//---------------------------------------------------------------------------
struct ResBnvgMaskInfoLayer
{
    //! レイヤー基本情報
    ResBnvgLayerBasicInfo basicInfo;
    //! マスクセット数
    uint16_t    maskSetCount;
    //! padding
    NN_PADDING6;
    //! マスクセットリスト
    nn::util::BinTPtr<ResBnvgMaskInfoSet>      pMaskSet;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データの Shape レイヤーのデータです。
//---------------------------------------------------------------------------
struct ResBnvgShapeLayer
{
    //! レイヤー基本情報
    ResBnvgLayerBasicInfo basicInfo;
    //! padding
    NN_PADDING8;
    //! グループトランスフォーム情報
    nn::util::BinTPtr<ResBnvgGroupTransform>   pGroupTransform;
    //! シェイプ数
    uint32_t    shapeCount;
    //! グループトランスフォーム情報数
    uint16_t    groupTransformCount;
    //! padding2
    NN_PADDING2;
    //! シェイプデータ
    nn::util::BinTPtr<ResBnvgShapeInfo>   pShapes;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データの PreComp レイヤーのデータです。
//---------------------------------------------------------------------------
struct ResBnvgPreCompLayer
{
    //! レイヤー基本情報
    ResBnvgLayerBasicInfo basicInfo;
    //! 参照しているコンポジションのインデックス
    int32_t         refAssetIndex;
    //! padding
    NN_PADDING4;
};

//---------------------------------------------------------------------------
//! @brief  Vector Graphics データのコンポジションを表すデータです。
//---------------------------------------------------------------------------
struct ResBnvgComposition
{
    //! コンポジションに含まれるレイヤーの数。
    uint32_t    layerCount;
    //! padding
    NN_PADDING4;
    //! レイヤーデータへのポインタ。各レイヤーデータは ResBnvgLayerBasicInfo が先頭に含まれており、type から適切な型を判定してキャストして使用します。
    nn::util::BinTPtr<nn::util::BinTPtr<ResBnvgLayerBasicInfo> >  pLayerPtrTable;
};

//---------------------------------------------------------------------------
//! @brief  bnvg データのバイナリファイルヘッダです。
//---------------------------------------------------------------------------
struct BnvgFileHeader
{
    //! ファイル全体のヘッダです。
    nn::util::BinaryFileHeader header;
    //! VectorGraphics データ用のファイルヘッダです。
    ResBnvgInfoHeader    vgHeader;
};

//---------------------------------------------------------------------------
//! @brief  bnvg データのコンポジションチャンクのデータです。
//---------------------------------------------------------------------------
struct BnvgCompositionDataBlock
{
    //! ブロックヘッダ
    nn::util::BinaryBlockHeader header;
    //! コンポジションデータ
    ResBnvgComposition    composition;
};

//@}

} // namespace nn::ui2d
} // namespace nn
