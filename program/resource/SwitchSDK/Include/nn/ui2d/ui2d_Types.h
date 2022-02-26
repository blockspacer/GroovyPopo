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

#include <cstddef>

#include <nn/gfx/gfx_ResTexture.h>
#include <nn/gfx/gfx_ResTextureData.h>

#include <nn/util/util_MathTypes.h>
#include <nn/util/util_IntrusiveList.h>
#include <nn/font/font_DrawerTypes.h>
#include <nn/font/font_Util.h>

#include <nn/gfx.h>
#include <nn/hws.h>

namespace nn
{
namespace ui2d
{

class Layout;
class TextureInfo;

namespace detail
{
class DynamicRenderingTexture;
}

//! ペイン名などのリソースの名前文字列の最大文字数です。
const   int     ResourceNameStrMax  = 24;
//! マテリアルの名前文字列の最大文字数です。
const   int     MaterialNameStrMax  = ResourceNameStrMax + 4;
//! アニメーションタグの最大文字数です。
const   int     AnimTagNameStrMax   = 64;
//! ユーザーデータの名前文字列の最大文字数です。
const   int     UserDataStrMax      = 8;
//! テクスチャの最大数です。
const   int     TexMapMax           = 3;
//! TEVステージの最大数です。
const   int     TevStageMax         = 6;
//! マテリアルカラーの最大数です。
const   int     MatColorMax         = 2;
//! フォントリソース名の最大(NULLを含む)です。
const   int     FontNameMax         = 128;
//! テクスチャリソース名の最大(NULLを含む)です。
const   int     TexImageNameMax     = 128;
//! シェーダリソース名の最大(NULLを含む)です。
const   int     ShaderNameMax       = 8;
//! アーカイブシェーダ名の最大(NULLを含む)です。
const   int     ArchiveShaderNameMax = 6;
//! レイアウト名の最大文字数です。
const   int     LayoutNameStrMax    = 64;
//! グループ名の最大文字数です。
const   int     GroupNameStrMax     = 32;
//! コンバイナユーザーシェーダのテクスチャの最大数（TexMapMax(3) + フレームバッファ(1)）です。
const   int     CombinerUserShaderTexMapMax = 4;

//! ConstantBuffer のバッファ数です。
const   int     ConstantBufferCount = 2;
//! 詳細なコンスタントカラーの最大数です。
const   int     DetailedCombinerConstantColor = 5;

//! ブレンドあたりのシェーダバリエーション数です。(頂点カラーあり／なし、メッシュ用で 3 つ。)
const   int     ShaderVariationCountPerBlend = 3;

//! TextBox の行ごとの幅とオフセットの数です。
const   int     TextBoxLineWidthOffsetCount = 16;
//! コンバイナユーザーシェーダのコンスタントカラー最大数です。
const   int     CombinerUserShaderConstantColor = 5;

//!< 動的に作成されるキャプチャテクスチャの名前の最大長
const   size_t  CaptureTexturePathMax = 64;
//!< キャプチャテクスチャのパーツ階層構造の最大数
const   size_t  CaptureTexturePartsLayerMax = 8;
//!< キャプチャテクスチャのパス区切り文字
const   char    CaptureTexturePathSeparator = '%';

//!< ドロップシャドウのぼかし半径(pixel)の最大値です
const   int     MaxDropShadowBlurRadius = 32;


//! アーカイブリソースの先頭アドレスの必要アライメント数です。
//!
//! アーカイブデータ内に含まれているテクスチャデータを、別の場所にコピーせずに
//! そのままGPUで扱うことができるようにするためにこのアライメントが必要です。
//! アーカイブ内のファイルのデータアラインメントは、レイアウトアーカイバが
//! 自動的に行います。
//!
//! この数値は、要求される最も大きいアライメントに合わせて定義しています。
//!
static const int ArchiveResourceAlignment = 4 * 1024;

//! 統合済みアーカイブシェーダのファイル名です。
static const char* ArchiveShaderFileName = "__ArchiveShader.bnsh";

//! 統合済みアーカイブシェーダの登録名です。
static const char* ArchiveShaderRegistrationName = "__arcsh";

//! 統合済みアーカイブシェーダのバリエーションテーブルのファイル名です。
static const char* ArchiveShaderVariationTableFileName = "__ArchiveShader.bushvt";

//! 個別バリエーションテーブルの詳細コンバイナステージで利用する要素数
static const int DetailedCombinerStageBitsCountWithVariationTable = 24;

// コンバイナユーザーシェーダの拡張ユーザー情報用 vec3 固定名です。
static const char* pCombinerUserShaderExtUserData_Vec3[] = {
    "__CUS_Vec3_0", "__CUS_Vec3_1", "__CUS_Vec3_2", "__CUS_Vec3_3",
};

// コンバイナユーザーシェーダの拡張ユーザー情報用 vec2 固定名です。
static const char* pCombinerUserShaderExtUserData_Vec2[] = {
    "__CUS_Vec2_0", "__CUS_Vec2_1", "__CUS_Vec2_2", "__CUS_Vec2_3",
};

// コンバイナユーザーシェーダの拡張ユーザー情報用 rgba 固定名です。
static const char* pCombinerUserShaderExtUserData_Rgba[] = {
    "__CUS_Rgba_0", "__CUS_Rgba_1", "__CUS_Rgba_2", "__CUS_Rgba_3",
};

// コンバイナユーザーシェーダの拡張ユーザー情報用 float 固定名です。
static const char* pCombinerUserShaderExtUserData_Float[] = {
    "__CUS_Float_0", "__CUS_Float_1", "__CUS_Float_2", "__CUS_Float_3",
};

namespace detail
{

//! テクスチャユニットの数です。
const   int     TexUnitMax          = 4;
//! TexEnvユニットの数です。
const   int     TexEnvUnitMax       = 6;

template <typename T>
NN_FORCEINLINE
T
SetBit(
    T       bits,
    int     pos,
    bool    val
)
{
    const T mask = T(~(1 << pos));

    bits &= mask;
    bits |= (val ? 1: 0) << pos;

    return bits;
}

template <typename T>
NN_FORCEINLINE
void
SetBit(
    T*      pBits,
    int     pos,
    bool    val
)
{
    const T mask = T(~(1 << pos));

    *pBits &= mask;
    *pBits |= (val ? 1: 0) << pos;
}

template <typename T>
NN_FORCEINLINE
    bool
    TestBit(
    T       bits,
    int     pos
    )
{
    const T mask = T(1 << pos);

    return 0 != (bits & mask);
}

template <typename T>
NN_FORCEINLINE
T
SetBits(
    T       bits,
    int     pos,
    int     len,
    T       val
)
{
    NN_SDK_ASSERT(len <= 32);
    const uint32_t  MaxValue = 0xFFFFFFFFU >> (32 - len);
    NN_SDK_ASSERT(val <= MaxValue);

    const T mask = T(~(MaxValue << pos));

    bits &= mask;
    bits |= val << pos;

    return bits;
}

template <typename T>
NN_FORCEINLINE
void
SetBits(
    T*      pBits,
    int     pos,
    int     len,
    T       val
)
{
    NN_SDK_ASSERT(len <= 32);
    const uint32_t  MaxValue = 0xFFFFFFFFU >> (32 - len);
    NN_SDK_ASSERT(val <= MaxValue);

    const T mask = T(~(MaxValue << pos));

    *pBits &= mask;
    *pBits |= val << pos;
}

template <typename T>
NN_FORCEINLINE
    T
    GetBits(
    T       bits,
    int     pos,
    int     len
    )
{
    NN_SDK_ASSERT(len <= 32);
    const uint32_t mask = ~(0xFFFFFFFFU << len);

    return T((bits >> pos) & mask);
}

//! @brief  マスク系のシェーダーバリエーションです。CombineType2 に対応しています。
//!
enum MaskShaderVariation
{
    MaskShaderVariation_Mask = 0,
    MaskShaderVariation_Copy,
};

//! @brief  ドロップシャドウ系のシェーダーバリエーションです。CombineType2 に対応しています。
//!
enum DropShadowShaderVariation
{
    DropShadowShaderVariation_HorizontalBlur1 = 0,
    DropShadowShaderVariation_HorizontalBlur2,
    DropShadowShaderVariation_HorizontalBlur3,
    DropShadowShaderVariation_HorizontalBlur4,

    DropShadowShaderVariation_VerticalBlur1,
    DropShadowShaderVariation_VerticalBlur2,
    DropShadowShaderVariation_VerticalBlur3,
    DropShadowShaderVariation_VerticalBlur4,

    DropShadowShaderVariation_VerticalBlur1WithColorMul,
    DropShadowShaderVariation_VerticalBlur2WithColorMul,
    DropShadowShaderVariation_VerticalBlur3WithColorMul,
    DropShadowShaderVariation_VerticalBlur4WithColorMul,

    DropShadowShaderVariation_VerticalBlur1WithKnockout,
    DropShadowShaderVariation_VerticalBlur2WithKnockout,
    DropShadowShaderVariation_VerticalBlur3WithKnockout,
    DropShadowShaderVariation_VerticalBlur4WithKnockout,

    DropShadowShaderVariation_VerticalBlur1WithKnockoutColorMul,
    DropShadowShaderVariation_VerticalBlur2WithKnockoutColorMul,
    DropShadowShaderVariation_VerticalBlur3WithKnockoutColorMul,
    DropShadowShaderVariation_VerticalBlur4WithKnockoutColorMul,

    DropShadowShaderVariation_Copy,
    DropShadowShaderVariation_CopyColorMul,

    DropShadowShaderVariation_Max
};

//! @brief  ベクターグラフィックスシェーダーのバリエーションです。
enum VectorGraphicsShaderVariation
{
    VectorGraphicsShaderVariation_Normal = 0,
    VectorGraphicsShaderVariation_MaskedTex1,
    VectorGraphicsShaderVariation_MaskedTex2,
    VectorGraphicsShaderVariation_MaskedTex3,

    VectorGraphicsShaderVariation_Max
};

//! @brief  ドロップシャドウ系エフェクトのブレンドタイプです。
enum DropShadowBlendMode
{
    DropShadowBlendMode_Normal,
    DropShadowBlendMode_Mul,
    DropShadowBlendMode_Add,
    DropShadowBlendMode_Sub,

    DropShadowBlendMode_Max
};

//! @brief  角丸系エフェクトのブレンドタイプです。
enum ProceduralShapeEffectBlendMode
{
    ProceduralShapeEffectBlendMode_Normal,
    ProceduralShapeEffectBlendMode_Mul,
    ProceduralShapeEffectBlendMode_Add,
    ProceduralShapeEffectBlendMode_Sub,

    ProceduralShapeEffectBlendMode_Max
};



} // namespace nn::ui2d::detail

//----------------------------------------
//! @name 実行時型情報
//@{

//--------------------------------------------------------------------
//! @brief   ベースクラスの型情報定義マクロです。
//!
//! @details ダイナミックキャストを可能にする為に埋め込むべき実行時型情報を
//!          メンバに含めるマクロです。クラスの宣言中に記述してください。
//!
//!          nn::ui2d::DynamicCast 等の機能が使用できるようになります。
//--------------------------------------------------------------------
#define NN_UI2D_RUNTIME_TYPEINFO_ROOT  NN_FONT_RUNTIME_TYPEINFO_ROOT

//--------------------------------------------------------------------
//! @brief   サブクラスの型情報定義マクロです。
//!
//! @details ダイナミックキャストを可能にする為に埋め込むべき実行時型情報を
//!          メンバに含めるマクロです。クラスの宣言中に記述してください。
//!
//!          nn::ui2d::DynamicCast 等の機能が使用できるようになります。
//!
//! @param   base 親クラスのクラス名を指定します。
//--------------------------------------------------------------------
#define NN_UI2D_RUNTIME_TYPEINFO  NN_FONT_RUNTIME_TYPEINFO

//---------------------------------------------------------------------------
//! @brief        ダウンキャスト用テンプレート関数です。
//!
//! @tparam       TToPtr  キャスト先のポインタ型を指定します。
//! @param[out]   obj     キャストをおこなうインスタンスです。
//!
//! @return       ダウンキャストに成功した場合はキャスト先クラスのポインタ、失敗した場合は NULL を返します。
//---------------------------------------------------------------------------
template<typename TToPtr, typename TFrom> inline TToPtr DynamicCast(TFrom* obj)
{
    return nn::font::DynamicCast<TToPtr, TFrom>(obj);
}

//---------------------------------------------------------------------------
//! @brief        ダウンキャスト可能かどうかの型チェックの ASSERT 付きスタティックキャストです。
//!               入力が NULL の場合は、ASSERT せず NULL を返します。
//!
//! @tparam       TToPtr  キャスト先のポインタ型を指定します。
//! @param[out]   obj     キャストをおこなうインスタンスです。
//!
//! @return       キャスト結果を返します。
//---------------------------------------------------------------------------
template<typename TToPtr, typename TFrom> inline TToPtr StaticCast(TFrom* obj)
{
    return nn::font::StaticCast<TToPtr, TFrom>(obj);
}

//--------------------------------------------------------------------------
//! @brief        型の比較をおこないます。
//!
//! @tparam       To       比較する型です。
//! @param[in]    instance 型を比較するインスタンスです。
//!
//! @return       テンプレート引数に指定した型と同一のインスタンスであれば真を返します。
//---------------------------------------------------------------------------
template <typename To, typename From> inline bool IsTypeOf(const From* instance)
{
    return nn::font::IsTypeOf<To, From>(instance);
}

//---------------------------------------------------------------------------
//! @brief        オブジェクトが指定した型にキャスト可能かどうかを取得します。
//!
//! @tparam       To       比較対象の型です。
//! @param[in]    instance 型をチェックするインスタンスです。
//!
//! @return       テンプレート引数に指定した型にキャスト可能であれば真を返します。
//---------------------------------------------------------------------------
template <typename To, typename From> inline bool IsDerivedFrom(const From* instance)
{
    return nn::font::IsDerivedFrom<To, From>(instance);
}

//---------------------------------------------------------------------------
//! @brief        インスタンスが同じ型かどうかを比較します。
//!               NULL の場合には結果は false になります。
//!
//! @param[in]    ptrA   比較するインスタンス１です。
//! @param[in]    ptrB   比較するインスタンス２です。
//!
//! @return       同一のクラスのインスタンスであれば true、そうでなければ false を返します。
//---------------------------------------------------------------------------
template< typename TypeA, typename TypeB > inline bool IsSameType( const TypeA* ptrA, const TypeB* ptrB )
{
    return nn::font::IsSameType<TypeA, TypeB>(ptrA, ptrB);
}


//---------------------------------------------------------------------------
//! @brief        インスタンスが同じ型かどうかを比較します。
//!               両方が NULL の場合にも結果は true になります。
//!
//! @param[in]    ptrA   比較するインスタンス１です。
//! @param[in]    ptrB   比較するインスタンス２です。
//!
//! @return       同一のクラスのインスタンスであれば true、そうでなければ false を返します。
//---------------------------------------------------------------------------
template< typename TypeA, typename TypeB > inline bool IsSameTypeOrNull( const TypeA* ptrA, const TypeB* ptrB )
{
    return nn::font::IsSameTypeOrNull<TypeA, TypeB>(ptrA, ptrB);
}

//@}

/**
* @brief ディスクリプタスロット確保関数のインタフェースです。
*
*
* @details
* 引数の textureView を登録したディスクリプタスロットを pOutSlot に返します。
*
* @param[out] pOutSlot     確保するスロット
* @param[in] textureView  テクスチャビューです。
* @param[in] pUserData    ユーザーデーターです。
*
* @return                   確保できなかった場合は、 false を返します。
*/
typedef bool (*RegisterTextureViewSlot)(nn::gfx::DescriptorSlot* pOutSlot, const nn::gfx::TextureView& textureView, void* pUserData);

/**
* @brief ディスクリプタスロット確保関数のインタフェースです。
*
* @details
* 引数の sampler を登録したディスクリプタスロットを pOutSlot に返します。
*
* @param[out] pOutSlot     確保するスロット
* @param[in] sampler      サンプラーです。
* @param[in] pUserData    ユーザーデーターです。
*
* @return                   確保できなかった場合は、 false を返します。
*/
typedef bool (*RegisterSamplerSlot)(nn::gfx::DescriptorSlot* pOutSlot, const nn::gfx::Sampler& sampler, void* pUserData);

/**
* @brief ディスクリプタスロット取得関数のインタフェースです。
*
* @details
* 引数の samplerInfo に対応する登録済みディスクリプタスロットを pOutSlot に返します。
* RegisterSamplerSlot と比較して、本関数は、Sampler オブジェクトの生成や管理をユーザープログラム側で行いたい場合に利用します。
*
* @param[out] pOutSlot     取得するスロット
* @param[in] samplerInfo   サンプラー情報です。
* @param[in] pUserData     ユーザーデーターです。
*
* @return                   取得できなかった場合は、 false を返します。
*
* @sa ReleaseSamplerSlot
*/
typedef bool(*AcquireSamplerSlot)(nn::gfx::DescriptorSlot* pOutSlot, const nn::gfx::SamplerInfo& samplerInfo, void* pUserData);

/**
* @brief ディスクリプタスロット解放関数のインタフェースです。
*
* @param[out] pSlot     解放するスロットです。
* @param[in] textureView  テクスチャビューです。
* @param[in] pUserData    ユーザーデーターです。
*
*/
typedef void (*UnregisterTextureViewSlot)(nn::gfx::DescriptorSlot* pSlot, const nn::gfx::TextureView& textureView, void* pUserData);

/**
* @brief ディスクリプタスロット解放関数のインタフェースです。
*
* @param[out] pSlot     解放するスロットです。
* @param[in] sampler      サンプラーです。
* @param[in] pUserData    ユーザーデーターです。
*
*/
typedef void (*UnregisterSamplerSlot)(nn::gfx::DescriptorSlot* pSlot, const nn::gfx::Sampler& sampler, void* pUserData);

/**
* @brief ディスクリプタスロット解放関数のインタフェースです。
*
* @details
* UnregisterSamplerSlot と比較して、本関数は、Sampler オブジェクトの生成や管理をユーザープログラム側で行いたい場合に利用します。
*
* @param[out] pSlot    解放するスロットです。
* @param[in] samplerInfo  サンプラー情報です。
* @param[in] pUserData    ユーザーデーターです。
*
* @sa AcquireSamplerSlot
*
*/
typedef void (*ReleaseSamplerSlot)(nn::gfx::DescriptorSlot* pSlot, const nn::gfx::SamplerInfo& samplerInfo, void* pUserData);

//---------------------------------------------------------------------------
// (削除予定)
// ペインの種類です。
//---------------------------------------------------------------------------

//! @brief 内部用機能のため使用禁止です。
enum PaneType
{
    // その他のペインです。
    PaneType_Other,
    // ヌルペインです。
    PaneType_Null,
    // ピクチャペインです。
    PaneType_Picture,
    // ウィンドウペインです。
    PaneType_Window,
    // テキストボックスペインです。
    PaneType_TextBox,
    // バウンディングペインです。
    PaneType_Bounding,
    // パーツペインです。
    PaneType_Parts,

    // 列挙子の総数です。
    PaneType_MaxPaneType
};

//---------------------------------------------------------------------------
//! @brief ペインの表示設定に使用します。
//!
//! @date 2010/04/09 PANEFLAG_MTXCONDITION, PaneFlag_UserMatrix を追加しました。
//! @date 2010/03/26 PaneFlag_UserAllocated を追加しました。
//---------------------------------------------------------------------------
enum PaneFlag
{
    //! ペインを表示するかどうかのフラグです。
    PaneFlag_Visible,
    //! 子のペインに親ペインのアルファ値が影響するかどうかのフラグです。
    PaneFlag_InfluencedAlpha,
    //! ペインが位置調整処理の対象となるかどうかのフラグです。
    PaneFlag_LocationAdjust,
    //! ユーザが作成したペインをあらわすフラグです。
    PaneFlag_UserAllocated,
    //! グローバルマトリックスがDirtyか否かをあらわすフラグです。
    PaneFlag_IsGlobalMatrixDirty,
    //! Srt 行列はユーザによって指定された値です（再計算禁止）。
    PaneFlag_UserMatrix,
    //! グローバル行列はユーザによって指定された値です（再計算禁止）。
    PaneFlag_UserGlobalMatrix,
    //! 配下の子供ペインも含め、描画用コンスタントバッファが準備できているかどうかをあらわすフラグです(nn::Pane::Caluclate()で設定されます)。
    PaneFlag_IsConstantBufferReady,
    //! 使用するビット数の合計です。
    PaneFlag_MaxPaneFlag
};

//---------------------------------------------------------------------------
//! @brief ペインの追加の属性です。
//!
//---------------------------------------------------------------------------
enum PaneFlagEx
{
    //! 部品倍率を無視するか否かのフラグです。
    PaneFlagEx_IgnorePartsMagnify,

    //! 部品倍率をかける際に、部品の境界に合わせて大きくなるか否かのフラグです。
    PaneFlagEx_PartsMagnifyAdjustToPartsBound,

    //! 拡張ユーザーデータアニメーションを使用するかどうかのフラグです。
    PaneFlagEx_ExtUserDataAnimationEnabled,

    //! ビューアで非表示状態になっているか否かです。
    PaneFlagEx_ViewerInvisible,

    //! 配下の子供ペインを含まず、自身のみの描画用コンスタントバッファが準備できているかどうかをあらわすフラグです(nn::Pane::Caluclate()で設定されます)。
    PaneFlagEx_IsConstantBufferReadySelf,

    //! Calculate が完了し、Draw 待ちの状態であるか否かです。Release ビルドでは使われません。
    PaneFlagEx_IsCalculationFinishedSelf,

    //! 動的な拡張ユーザーデータが作成されているかどうかのフラグです。
    PaneFlagEx_DynamicExtUserDataEnabled,

    //! 使用するビット数の合計です。
    PaneFlagEx_MaxPaneFlagEx,
};

//---------------------------------------------------------------------------
//! @brief ペインのシステム用拡張ユーザー情報の型です。
//!
//! @details
//! 下位 16bit がデータコンバート時に埋め込まれる静的なシステム用拡張ユーザーデータを
//! 上位 16bit がランタイムで動的に作成されるシステム用拡張ユーザーデータを表します。
//!
//---------------------------------------------------------------------------
enum PaneSystemDataType
{
    //! AABB メッシュのデータです。
    PaneSystemDataType_SimpleAABBMesh,

    //! OBB メッシュのデータです。
    PaneSystemDataType_SimpleOBBMesh,

    //! 整列ペインの追加情報
    PaneSystemDataType_AlignmentExInfo,

    //! マスク機能のためのデータです。
    PaneSystemDataType_MaskTexture,

    //! ドロップシャドウ機能のためのデータです。
    PaneSystemDataType_DropShadow,

    //! レイアウトアニメーションタグのためのデータです。
    PaneSystemDataType_LayoutAnimTag,

    //! 角丸機能のためのデータです。
    PaneSystemDataType_ProceduralShape,

    //! スクリーンのためのデータです。
    PaneSystemDataType_Screen,

    //! キャプチャテクスチャのコピー情報ランタイムデータです。
    PaneSystemDataType_CaptureTextureRuntimeCopyInfo = 16,

    //! ペインエフェクト用のランタイムデータです。
    PaneSystemDataType_PaneEffect,

    //! 角丸用のランタイムデータです。
    PaneSystemDataType_ProceduralShapeRuntimeInfo,

    //! ステート用のランタイムデータです。
    PaneSystemDataType_StateRuntimeInfo,

    //! システム用拡張ユーザーデータ高速参照用のテーブルデータです。
    PaneSystemDataType_SystemDataReferenceTable,

    //! 使用するビット数の合計です。
    PaneSystemDataType_Max,
};

//---------------------------------------------------------------------------
//! @brief レイアウトのシステム用拡張ユーザー情報の型です。
//!
//! @details
//! 下位 16bit がデータコンバート時に埋め込まれる静的なシステム用拡張ユーザーデータを
//! 上位 16bit がランタイムで動的に作成されるシステム用拡張ユーザーデータを表します。
//!
//---------------------------------------------------------------------------
enum LayoutSystemDataType
{
    //! アニメーション区間タグの名前です。
    LayoutSystemDataType_AnimTagName,

    //! 使用するビット数の合計です。
    LayoutSystemDataType_Max,
};

//---------------------------------------------------------------------------
//! @brief 表示の水平方向の基準位置に関する列挙子です。
//!
//---------------------------------------------------------------------------
enum HorizontalPosition
{
    //! 中央を基準にします。
    HorizontalPosition_Center,
    //! 左を基準にします。
    HorizontalPosition_Left,
    //! 右を基準にします。
    HorizontalPosition_Right,

    //! 列挙子の総数です。
    HorizontalPosition_MaxHorizontalPosition
};

//---------------------------------------------------------------------------
//! @brief 表示の垂直方向の基準位置に関する列挙子です。
//!
//---------------------------------------------------------------------------
enum VerticalPosition
{
    //! 中央を基準にします。
    VerticalPosition_Center,
    //! 上を基準にします。
    VerticalPosition_Top,
    //! 下を基準にします。
    VerticalPosition_Bottom,

    //! 列挙子の総数です。
    VerticalPosition_MaxVerticalPosition
};

//---------------------------------------------------------------------------
//! @brief テクスチャフォーマットの列挙子です。
//!
//---------------------------------------------------------------------------
typedef nn::gfx::ImageFormat TexFormat;

//---------------------------------------------------------------------------
//! @brief テクスチャのラップモードを表す列挙子です。
//!
//---------------------------------------------------------------------------
enum TexWrap
{
    //! クランプします。
    TexWrap_Clamp,
    //! 繰り返します。
    TexWrap_Repeat,
    //! ミラーします。
    TexWrap_Mirror,

    //! 列挙子の総数です。
    TexWrap_MaxTexWrap
};

//---------------------------------------------------------------------------
//! @brief テクスチャフィルターのモードを表す列挙子です。
//!
//---------------------------------------------------------------------------
enum TexFilter
{
    //! NEARフィルターモードです。
    TexFilter_Near,
    //! LINEARフィルターモードです。
    TexFilter_Linear,

    //! 列挙子の総数です。
    TexFilter_MaxTexFilter
};

//---------------------------------------------------------------------------
//! @brief 投影テクスチャの表示設定に使用します。
//!
//---------------------------------------------------------------------------
enum TexProjectionFlag
{
    //! 投影テクスチャの自動サイズ調整をするかどうかのフラグです。
    TexProjectionFlag_FittingLayoutSize,
    //! ペイン全体に投影するかどうかのフラグです。
    TexProjectionFlag_FittingPaneSizeEnabled,
    //! ペインの回転スケールを影響させるかどうかのフラグです。
    TexProjectionFlag_AdjustProjectionScaleRotateEnabled,

    //! 使用するビット数の合計です。
    TexProjectionFlag_MaxTexProjectionFlag,
};

//---------------------------------------------------------------------------
//! @brief Tevの演算モードを表す列挙子です。
//!
//---------------------------------------------------------------------------
enum TevMode
{
    //! REPLACEモードです。
    TevMode_Replace,
    //! MODULATEモードです。
    TevMode_Modulate,
    //! ADDモードです。
    TevMode_Add,
    //! ADD_SIGNEDモードです。
    TevMode_AddSigned,
    //! INTERPOLATEモードです。
    TevMode_Interpolate,
    //! SUBTRACTモードです。
    TevMode_Subtract,
    //! ADD_MULTモードです。
    TevMode_AddMultiply,
    //! MULT_ADDモードです。
    TevMode_MultiplyAdd,
    //! OVERLAYモードです。
    TevMode_Overlay,
    //! Lightenモードです。
    TevMode_Lighten,
    //! Darkenモードです。
    TevMode_Darken,

    //! INDIRECTモードです。
    TevMode_Indirect,
    //! BLEND_INDIRECTモードです。
    TevMode_BlendIndirect,
    //! EACH_INDIRECTモードです。
    TevMode_EachIndirect,

    //! 列挙子の総数です。
    TevMode_MaxTevMode
};

//---------------------------------------------------------------------------
//! @brief アルファテストのモードを指定する列挙子です。
//!
//---------------------------------------------------------------------------
enum AlphaTest
{
    //! フラグメントは常に受け入れません。
    AlphaTest_Never,
    //! フラグメント・アルファ＜参照アルファのとき受け入れます。
    AlphaTest_Less,
    //! フラグメント・アルファ≦参照アルファのとき受け入れます。
    AlphaTest_LessEqual,
    //! フラグメント・アルファ＝参照アルファのとき受け入れます。
    AlphaTest_Equal,
    //! フラグメント・アルファ≠参照アルファのとき受け入れます。
    AlphaTest_NotEqual,
    //! フラグメント・アルファ≧参照アルファのとき受け入れます。
    AlphaTest_GreaterEqual,
    //! フラグメント・アルファ＞参照アルファのとき受け入れます。
    AlphaTest_Greater,
    //! フラグメントは無条件に受け入れます。
    AlphaTest_Always,

    //! 列挙子の総数です。
    AlphaTest_MaxAlphaTest
};

//---------------------------------------------------------------------------
//! @brief ブレンド処理の係数です。
//!
//---------------------------------------------------------------------------
enum BlendFactor
{
    //! (0.0) です。
    BlendFactor_0,
    //! (1.0) です。
    BlendFactor_1,
    //! (ターゲット・カラー) です。
    BlendFactor_DstColor,
    //! (1.0 - ターゲット・カラー) です。
    BlendFactor_InvDstColor,
    //! (ソース・アルファ) です。
    BlendFactor_SrcAlpha,
    //! (1.0 - ソース・アルファ) です。
    BlendFactor_InvSrcAlpha,
    //! (ターゲット・アルファ) です。
    BlendFactor_DstAlpha,
    //! (1.0 - ターゲットアルファ) です。
    BlendFactor_InvDstAlpha,
    //! (ソース・カラー) です。
    BlendFactor_SrcColor,
    //! (1.0 - ソース・カラー) です。
    BlendFactor_InvSrcColor,

    //! 列挙子の総数です。
    BlendFactor_MaxBlendFactor
};

//---------------------------------------------------------------------------
//! @brief ブレンドの演算方法を指定します。
//!
//---------------------------------------------------------------------------
enum BlendOp
{
    //! ブレンド処理は行いません。
    BlendOp_Disable,
    //! 加算します。
    BlendOp_Add,
    //! 減算します。
    BlendOp_Subtract,
    //! 逆方向に減算します。
    BlendOp_ReverseSubtract,
    //! 小さい値を採用します。
    BlendOp_SelectMin,
    //! 大きい値を採用します。
    BlendOp_SelectMax,

    //! 列挙子の総数です。
    BlendOp_MaxBlendOp
};

//---------------------------------------------------------------------------
//! @brief 論理演算の演算方法を指定します。
//!
//---------------------------------------------------------------------------
enum LogicOp
{
    //! 論理演算は行いません。
    LogicOp_Disable,
    //! 何もしません。
    LogicOp_Noop,
    //! 0x00 です。
    LogicOp_Clear,
    //! 0xff です。
    LogicOp_Set,
    //! (ソース) です。
    LogicOp_Copy,
    //! (~ソース) です。
    LogicOp_InvCopy,
    //! (~ターゲット) です。
    LogicOp_Inv,
    //! (ソース & ターゲット) です。
    LogicOp_And,
    //! ~(ソース & ターゲット) です。
    LogicOp_Nand,
    //! (ソース | ターゲット です。
    LogicOp_Or,
    //! ~(ソース | ターゲット) です。
    LogicOp_Nor,
    //! (ソース ^ ターゲット です。
    LogicOp_Xor,
    //! ~(ソース ^ ターゲット) です。
    LogicOp_Equiv,
    //! (ソース & ~ターゲット) です。
    LogicOp_RevAnd,
    //! (~ソース & ターゲット) です。
    LogicOp_InvAnd,
    //! (ソース | ~ターゲット) です。
    LogicOp_RevOr,
    //! (~ソース | ターゲット) です。
    LogicOp_InvOr,

    //! 列挙子の総数です。
    LogicOp_MaxLogicOp
};

//---------------------------------------------------------------------------
//! @brief テクスチャ座標の生成方法を指定します。
//!
//---------------------------------------------------------------------------
enum TexGenType
{
    //! 入力属性に関して 2X4 行列の乗算を行い、S, T テクスチャ座標を生成します。
    TexGenType_Mtx2x4,

    //! 列挙子の総数です。
    TexGenType_MaxTexGenType
};

//---------------------------------------------------------------------------
//! @brief テクスチャ座標のソースを指定します。
//!
//---------------------------------------------------------------------------
enum TexGenSrc
{
    //! テクスチャ座標 0 を使います。
    TexGenSrc_Tex0,
    //! テクスチャ座標 1 を使います。
    TexGenSrc_Tex1,
    //! テクスチャ座標 2 を使います。
    TexGenSrc_Tex2,
    //! 平行投影を使います。
    TexGenSrc_OrthoProjection,
    //! ペイン基準の平行投影を使います。
    TexGenSrc_PaneBaseOrthoProjection,
    //! 透視投影 を使います。
    TexGenSrc_PerspectiveProjection,
    //! ペイン基準の透視投影 を使います。
    TexGenSrc_PaneBasePerspectiveProjection,

    //! 列挙子の総数です。
    TexGenSrc_MaxTexGenSrc
};

//---------------------------------------------------------------------------
//! @brief テクスチャの反転表示に関する列挙子です。
//!
//---------------------------------------------------------------------------
enum TextureFlip
{
    //! そのまま表示します。
    TextureFlip_None,
    //! 左右（水平方向）に反転して表示します。
    TextureFlip_FlipU,
    //! 上下（垂直方向）に反転して表示します。
    TextureFlip_FlipV,
    //! 90度回転して表示します。
    TextureFlip_Rotate90,
    //! 180度回転して表示します。
    TextureFlip_Rotate180,
    //! 270度回転して表示します。
    TextureFlip_Rotate270,

    //! 列挙子の総数です。
    TextureFlip_MaxTextureFlip
};

//---------------------------------------------------------------------------
//! @brief 原点位置を指定します。
//!
//---------------------------------------------------------------------------
enum ScreenOriginType
{
    //! レイアウトの左上隅を原点とします。
    ScreenOriginType_Classic,
    //! レイアウトの中心を原点とします。
    ScreenOriginType_Normal,

    //! 列挙子の総数です。
    ScreenOriginType_MaxScreenOriginType
};

//---------------------------------------------------------------------------
//! @brief テキストボックスフラグです。
//!
//---------------------------------------------------------------------------
enum TextBoxFlag
{
    //! カゲを表示するかどうかのフラグです。
    TextBoxFlag_ShadowEnabled,

    //! 「文字列の長さを強制的に指定する」のチェックが入っているか否かのフラグです。
    TextBoxFlag_ForceAssignTextLength,

    //! フチ付きフォントのフチを非表示にするか否かのフラグです。
    TextBoxFlag_InvisibleBorderEnabled,

    //! フチを二度書きするか否かのフラグです。
    TextBoxFlag_DoubleDrawnBorderEnabled,

    //! 文字単位のアニメーションをするかどうかのフラグです。
    TextBoxFlag_PerCharacterTransformEnabled,

    //! 中央揃えのにじみ防止機能を有効にするか否かのフラグです。
    TextBoxFlag_CenterCeilingEnabled,

    //! 行ごとの幅とオフセットを有効にするか否かのフラグです。
    TextBoxFlag_LineWidthOffsetEnabled,

    //! 拡張タグの解釈を有効にするか否かのフラグです。
    TextBoxFlag_ExtendedTagEnabled,

    //! 文字毎の変換で文字幅に応じて分割するかどうかのフラグです。
    TextBoxFlag_PerCharacterTransformSplitByCharWidth,

    //! 文字毎の変換で影のアルファを文字のアルファに自動追従させるかどうかのフラグです。
    TextBoxFlag_PerCharacterTransformAutoShadowAlpha,

    //! 右側の文字から描画するかどうかのフラグです。
    TextBoxFlag_DrawFromRightToLeft,

    //! 文字毎の変換で各文字の原点をテキストボックスの中心にするかどうかのフラグです。
    TextBoxFlag_PerCharacterTransformOriginToCenter,

    //! フォントが差し変わっていたときにテキストボックスの初期化時にフォントのスケールを維持するフラグです。
    TextBoxFlag_KeepingFontScaleEnabled,

    //! 文字毎のアニメーションの文字幅固定機能の有効・無効のフラグです。
    TextBoxFlag_PerCharacterTransformFixSpace,

    //! 文字毎のアニメーションの左右に空白を挿入する機能の有効・無効のフラグです。
    TextBoxFlag_PerCharacterTransformSplitByCharWidthInsertSpaceEnabled,

    //! 使用するビット数の合計です。
    TextBoxFlag_MaxTextBoxFlag,
};

//---------------------------------------------------------------------------
//! @brief テキストボックスの部分上書きフラグです。
//!
//---------------------------------------------------------------------------
enum TextBoxOverrideUsageFlag
{
    //! テキスト及びテキストIDを部分的に上書きするか否かのフラグです。
    TextBoxOverrideUsageFlag_TextEnabled,

    //! 使用するビット数の合計です。
    TextBoxOverrideUsageFlag_MaxTextBoxOverrideUsageFlag,
};

//---------------------------------------------------------------------------
//! @brief ペイン基本の部分上書きフラグです。
//!
//---------------------------------------------------------------------------
enum BasicOverrideUsageFlag
{
    //! 表示/非表示を上書きするか否かです。
    BasicOverrideUsageFlag_VisibleEnabled,

    //! 表示/非表示を上書きする場合の値です。
    BasicOverrideUsageFlag_VisibleValue,

    //! ユーザーデータを上書きするか否かです。
    BasicOverrideUsageFlag_BasicUserDataEnabled,

    //! 位置を上書きするか否かです。
    BasicOverrideUsageFlag_TranslateEnabled,

    //! サイズを上書きするか否かです。
    BasicOverrideUsageFlag_SizeEnabled,

    //! スケールを上書きするか否かです。
    BasicOverrideUsageFlag_ScaleEnabled,

    //! 回転を上書きするか否かです。
    BasicOverrideUsageFlag_RotateEnabled,

    //! 透明度を上書きするか否かです。
    BasicOverrideUsageFlag_AlphaEnabled,

    //! 使用するビット数の合計です。
    BasicOverrideUsageFlag_MaxBasicOverrideUsageFlag,
};

//---------------------------------------------------------------------------
//! @brief マテリアルの部分上書きフラグです。
//!
//---------------------------------------------------------------------------
enum MaterialOverrideUsageFlag
{
    //! カラー補完を部分的に上書きするか否かのフラグです。
    MaterialOverrideUsageFlag_InterpolateColorEnabled,

    //! テクスチャを部分的に上書きするか否かのフラグです。
    MaterialOverrideUsageFlag_TextureEnabled,

    //! キャプチャテクスチャリソースを上書きするかどうかのフラグです。ピクチャペインのテクスチャの上書き状態によって自動的に設定されます。
    MaterialOverrideUsageFlag_OverrideCaptureTextureResources,

    //! 使用するビット数の合計です。
    MaterialOverrideUsageFlag_MaxMaterialOverrideUsageFlag,
};

//---------------------------------------------------------------------------
//! @brief 拡張ユーザーデータを上書きする際のオフセットの特別な値です。
//!
//---------------------------------------------------------------------------
enum ExtUserDataOverrideOffset
{
    //! 上書きを行いません。
    ExtUserDataOverrideOffset_NoOverride = 0,

    //! 上書きしますが、データがありません。
    ExtUserDataOverrideOffset_NoData = 1
};

//---------------------------------------------------------------------------
//! @brief TextBox の行そろえ指定です。
//!
//---------------------------------------------------------------------------
enum TextAlignment
{
    //! 水平方向の位置にあわせます。
    TextAlignment_Synchronous,
    //! 左にそろえます。
    TextAlignment_Left,
    //! 中央にそろえます。
    TextAlignment_Center,
    //! 右にそろえます。
    TextAlignment_Right,

    //! 列挙子の総数です。
    TextAlignment_MaxTextAlignment
};

//---------------------------------------------------------------------------
//! @brief 頂点属性番号です。
//!
//---------------------------------------------------------------------------
enum VertexAttr
{
    //! 頂点座標です。
    VertexAttr_Pos,
    //! 頂点カラーです。
    VertexAttr_Color,
    //! テクスチャ座標 0 です。
    VertexAttr_Texcoord0,
    //! テクスチャ座標 1 です。
    VertexAttr_Texcoord1,
    //! テクスチャ座標 2 です。
    VertexAttr_Texcoord2,

    //! 頂点インデックスです。
    VertexAttr_VertexIndex = 0,

    //! 列挙子の総数です。
    VertexAttr_MaxVertexAttr
};

//---------------------------------------------------------------------------
//! @brief 頂点属性に要求されるサイズです。
//!
//---------------------------------------------------------------------------
enum VertexAttrSize
{
    //! 頂点座標に要求されるサイズです。
    VertexAttrSize_Pos = 4,
    //! 頂点カラーに要求されるサイズです。
    VertexAttrSize_Color = 4,
    //! テクスチャ座標に要求されるサイズです。
    VertexAttrSize_Texcoord = 2,
    //! 頂点インデックスに要求されるサイズです。
    VertexAttrSize_Index = 2
};

using nn::font::PaneVertex;
using nn::font::PaneVertex_LeftTop;
using nn::font::PaneVertex_RightTop;
using nn::font::PaneVertex_LeftBottom;
using nn::font::PaneVertex_RightBottom;
using nn::font::PaneVertex_MaxPaneVertex;

//---------------------------------------------------------------------------
//! @brief ペインの四隅に設定された頂点カラーにアクセスするときに使用します。
//!
//---------------------------------------------------------------------------
enum VertexColor
{
    //! 左上隅の頂点カラーです。
    VertexColor_LeftTop,
    //! 右上隅の頂点カラーです。
    VertexColor_RightTop,
    //! 左下隅の頂点カラーです。
    VertexColor_LeftBottom,
    //! 右下隅の頂点カラーです。
    VertexColor_RightBottom,

    //! 列挙子の総数です。
    VertexColor_MaxVertexColor
};

//---------------------------------------------------------------------------
//! @brief ウィンドウフラグです。
//!
//---------------------------------------------------------------------------
enum WindowFlag
{
    //! 一つのマテリアルをウィンドウ全体に使用するかどうかのフラグです。
    WindowFlag_UseOneMaterialForAll,

    //! 頂点カラーをウィンドウ全体に使用するかどうかのフラグです。
    WindowFlag_UseVertexColorAll,

    //! ウィンドウ種類のフラグです（1ビット目）。
    WindowFlag_WindowKind0,
    //! ウィンドウ種類のフラグです（2ビット目）。
    WindowFlag_WindowKind1,

    //! コンテンツ領域を描画しないかのフラグです。
    WindowFlag_NotDrawContent,

    //! 使用するビット数の合計です。
    WindowFlag_MaxWindowFlag,

    //! WindowFlag_WindowKindのビット数です。
    WindowFlag_WindowKindLength = 2
};

//---------------------------------------------------------------------------
//! @brief ウィンドウの種類を指定するときに使用します。
//!
//---------------------------------------------------------------------------
enum WindowKind
{
    //! 通常のモード（従来挙動）です。
    WindowKind_Around,
    //! 水平モードです。
    WindowKind_Horizontal,
    //! 水平モード、コンテンツ無しです。
    WindowKind_HorizontalNoContents,

    //! 列挙子の総数です。
    WindowKind_MaxWindowKind
};

//---------------------------------------------------------------------------
//! @brief ウィンドウフレームを指定するときに使用します。
//!
//---------------------------------------------------------------------------
enum WindowFrame
{
    //! 左上フレームです。
    WindowFrame_LeftTop,
    //! 右上フレームです。
    WindowFrame_RightTop,
    //! 左下フレームです。
    WindowFrame_LeftBottom,
    //! 右下フレームです。
    WindowFrame_RightBottom,
    //! 左フレームです。
    WindowFrame_Left,
    //! 右フレームです。
    WindowFrame_Right,
    //! 上フレームです。
    WindowFrame_Top,
    //! 下フレームです。
    WindowFrame_Bottom,

    //! 列挙子の総数です。
    WindowFrame_MaxWindowFrame
};

//---------------------------------------------------------------------------
//! @brief ウィンドウ拡大率の固定少数の定義です。
//!
//---------------------------------------------------------------------------
enum WindowInflationFixedPoint
{
    WindowInflationFixedPoint_FractionalBit = 4,
    WindowInflationFixedPoint_IntegerBit = 11,
    WindowInflationFixedPoint_SignBit = 1,

    WindowInflationFixedPoint_ScalingFactor = 1 << WindowInflationFixedPoint_FractionalBit
};

//---------------------------------------------------------------------------
//! @brief 文字色の設定にアクセスするときに使用します。
//!
//---------------------------------------------------------------------------
enum TextColor
{
    //! 上端の文字色です。
    TextColor_Top,
    //! 下端の文字色です。
    TextColor_Bottom,

    //! 列挙子の総数です。
    TextColor_MaxTextColor
};

//---------------------------------------------------------------------------
//! @brief 文字色の設定にアクセスするときに使用します。
//!
//---------------------------------------------------------------------------
enum InterpolateColor
{
    //! 黒カラー補間色です。
    InterpolateColor_Black,
    //! 白カラー補間色です。
    InterpolateColor_White,

    //! 列挙子の総数です。
    InterpolateColor_MaxInterpolateColor
};

NN_STATIC_ASSERT(InterpolateColor_MaxInterpolateColor <= MatColorMax);

//---------------------------------------------------------------------------
//! @brief ピクチャペインの追加情報フラグです。
//!
//---------------------------------------------------------------------------
enum PicturePaneFlag
{
    //! シェイプバイナリインデックスを持っているか。(uint32_t)
    PicturePaneFlag_ShapeBinaryIndex,

    //! 列挙子の総数です。
    PicturePaneFlag_Max
};

//---------------------------------------------------------------------------
//! @brief 拡張ユーザデータのデータ種別です。
//!
//---------------------------------------------------------------------------
enum ExtUserDataType
{
    //! 文字列です。
    ExtUserDataType_String,
    //! 整数配列です。
    ExtUserDataType_Int,
    //! 浮動小数点数配列です。
    ExtUserDataType_Float,
    //! システム用拡張ユーザーデータです。
    ExtUserDataType_SystemData,

    //! 列挙子の総数です。
    ExtUserDataType_MaxExtUserDataType
};

//---------------------------------------------------------------------------
//! @brief アニメーションの対象を表します。
//!
//! @sa AnimationContent
//!
//---------------------------------------------------------------------------
enum AnimContentType
{
    //! ペインを対象としたアニメーションです。
    AnimContentType_Pane,
    //! マテリアルを対象としたアニメーションです。
    AnimContentType_Material,
    //! 拡張ユーザーデータを対象としたアニメーションです。
    AnimContentType_ExtUserData,
    //! ステートマシンを対象としたアニメーションです。
    AnimContentType_StateMachine,
    //! 部品ペインのステートマシンを対象としたアニメーションです。
    AnimContentType_PartsStateMachine,

    //! 列挙子の総数です。
    AnimContentType_MaxAnimContentType
};

//---------------------------------------------------------------------------
//! @brief アニメーションタグのフラグです。
//!
//---------------------------------------------------------------------------
enum AnimTagFlag
{
    //! ペインの子孫ペインもバインドの対象にします。
    AnimTagFlag_DescendingBind,

    //! 列挙子の総数です。
    AnimTagFlag_MaxAnimTagFlag
};

//---------------------------------------------------------------------------
//! @brief ペインアニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetPane
{
    //! 表示位置の X 座標です。
    AnimTargetPane_TranslateX = 0,
    //! 表示位置の Y 座標です。
    AnimTargetPane_TranslateY,
    //! 表示位置の Z 座標です。
    AnimTargetPane_TranslateZ,
    //! X 軸の回転量です。
    AnimTargetPane_RotateX,
    //! Y 軸の回転量です。
    AnimTargetPane_RotateY,
    //! Z 軸の回転量です。
    AnimTargetPane_RotateZ,
    //! X 座標方向の拡大率です。
    AnimTargetPane_ScaleX,
    //! Y 座標方向の拡大率です。
    AnimTargetPane_ScaleY,
    //! 幅です。
    AnimTargetPane_SizeWidth,
    //! 高さです。
    AnimTargetPane_SizeHeight,

    //! 列挙子の総数です。
    AnimTargetPane_MaxAnimTargetPane
};

//---------------------------------------------------------------------------
//! @brief ビジビリティアニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetVisibility
{
    //! ビジビリティです。
    AnimTargetVisibility_Visibility = 0,

    //! 列挙子の総数です。
    AnimTargetVisibility_MaxAnimTargetVisibility
};

//---------------------------------------------------------------------------
//! @brief ペインカラーアニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetPaneColor
{
    //! 左上隅の頂点カラーの R 成分値です。
    AnimTargetPaneColor_VertexLtR = 0,
    //! 左上隅の頂点カラーの G 成分値です。
    AnimTargetPaneColor_VertexLtG,
    //! 左上隅の頂点カラーの B 成分値です。
    AnimTargetPaneColor_VertexLtB,
    //! 左上隅の頂点カラーの A 成分値です。
    AnimTargetPaneColor_VertexLtA,
    //! 右上隅の頂点カラーの R 成分値です。
    AnimTargetPaneColor_VertexRtR,
    //! 右上隅の頂点カラーの G 成分値です。
    AnimTargetPaneColor_VertexRtG,
    //! 右上隅の頂点カラーの B 成分値です。
    AnimTargetPaneColor_VertexRtB,
    //! 右上隅の頂点カラーの A 成分値です。
    AnimTargetPaneColor_VertexRtA,
    //! 左下隅の頂点カラーの R 成分値です。
    AnimTargetPaneColor_VertexLbR,
    //! 左下隅の頂点カラーの G 成分値です。
    AnimTargetPaneColor_VertexLbG,
    //! 左下隅の頂点カラーの B 成分値です。
    AnimTargetPaneColor_VertexLbB,
    //! 左下隅の頂点カラーの A 成分値です。
    AnimTargetPaneColor_VertexLbA,
    //! 右下隅の頂点カラーの R 成分値です。
    AnimTargetPaneColor_VertexRbR,
    //! 右下隅の頂点カラーの G 成分値です。
    AnimTargetPaneColor_VertexRbG,
    //! 右下隅の頂点カラーの B 成分値です。
    AnimTargetPaneColor_VertexRbB,
    //! 右下隅の頂点カラーの A 成分値です。
    AnimTargetPaneColor_VertexRbA,

    //! 列挙子の総数です。
    AnimTargetPaneColor_MaxVertex,

    //! ペインのアルファ値です。
    AnimTargetPaneColor_PaneAlpha = AnimTargetPaneColor_MaxVertex,

    //! 列挙子の総数です。
    AnimTargetPaneColor_MaxAnimTargetPaneColor
};

//---------------------------------------------------------------------------
//! @brief マテリアルカラーアニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetMatColor
{
    //! コンバイナバッファの R 成分です。
    AnimTargetMatColor_BufferR = 0,
    //! コンバイナバッファの G 成分です。
    AnimTargetMatColor_BufferG,
    //! コンバイナバッファの B 成分です。
    AnimTargetMatColor_BufferB,
    //! コンバイナバッファの A 成分です。
    AnimTargetMatColor_BufferA,
    //! コンスタントレジスタ 0 の R 成分です。
    AnimTargetMatColor_Konst0R,
    //! コンスタントレジスタ 0 の G 成分です。
    AnimTargetMatColor_Konst0G,
    //! コンスタントレジスタ 0 の B 成分です。
    AnimTargetMatColor_Konst0B,
    //! コンスタントレジスタ 0 の A 成分です。
    AnimTargetMatColor_Konst0A,

    //! コンスタントカラー 0 の R 成分です。
    AnimTargetMatColor_C0_R,
    //! コンスタントカラー 0 の G 成分です。
    AnimTargetMatColor_C0_G,
    //! コンスタントカラー 0 の B 成分です。
    AnimTargetMatColor_C0_B,
    //! コンスタントカラー 0 の A 成分です。
    AnimTargetMatColor_C0_A,

    //! コンスタントカラー 1 の R 成分です。
    AnimTargetMatColor_C1_R,
    //! コンスタントカラー 1 の G 成分です。
    AnimTargetMatColor_C1_G,
    //! コンスタントカラー 1 の B 成分です。
    AnimTargetMatColor_C1_B,
    //! コンスタントカラー 1 の A 成分です。
    AnimTargetMatColor_C1_A,

    //! コンスタントカラー 2 の R 成分です。
    AnimTargetMatColor_C2_R,
    //! コンスタントカラー 2 の G 成分です。
    AnimTargetMatColor_C2_G,
    //! コンスタントカラー 2 の B 成分です。
    AnimTargetMatColor_C2_B,
    //! コンスタントカラー 2 の A 成分です。
    AnimTargetMatColor_C2_A,

    //! コンスタントカラー 3 の R 成分です。
    AnimTargetMatColor_C3_R,
    //! コンスタントカラー 3 の G 成分です。
    AnimTargetMatColor_C3_G,
    //! コンスタントカラー 3 の B 成分です。
    AnimTargetMatColor_C3_B,
    //! コンスタントカラー 3 の A 成分です。
    AnimTargetMatColor_C3_A,

    //! コンスタントカラー 4 の R 成分です。
    AnimTargetMatColor_C4_R,
    //! コンスタントカラー 4 の G 成分です。
    AnimTargetMatColor_C4_G,
    //! コンスタントカラー 4 の B 成分です。
    AnimTargetMatColor_C4_B,
    //! コンスタントカラー 4 の A 成分です。
    AnimTargetMatColor_C4_A,


    //! 列挙子の総数です。
    AnimTargetMatColor_MaxAnimTargetMatColor
};

NN_STATIC_ASSERT(AnimTargetMatColor_MaxAnimTargetMatColor / 4 == (MatColorMax + DetailedCombinerConstantColor));

//---------------------------------------------------------------------------
//! @brief マテリアルカラーアニメーションの対象(float 版)を指定します。
//!
//! @details
//! カーブが float カラーに接続されているかどうかの判断に使用されています。
//! AnimTargetMatColor の中に取り込むと最大値が変わり、/ 4 してインデックスでアクセスしている
//! コードに悪影響があるため初期値を変えて別途定義している。
//---------------------------------------------------------------------------
enum AnimTargetMatColorFloat
{
    // float 版
    //! コンバイナバッファの R 成分です。
    AnimTargetMatColorFloat_BufferR = AnimTargetMatColor_MaxAnimTargetMatColor,
    //! コンバイナバッファの G 成分です。
    AnimTargetMatColorFloat_BufferG,
    //! コンバイナバッファの B 成分です。
    AnimTargetMatColorFloat_BufferB,
    //! コンバイナバッファの A 成分です。
    AnimTargetMatColorFloat_BufferA,
    //! コンスタントレジスタ 0 の R 成分です。
    AnimTargetMatColorFloat_Konst0R,
    //! コンスタントレジスタ 0 の G 成分です。
    AnimTargetMatColorFloat_Konst0G,
    //! コンスタントレジスタ 0 の B 成分です。
    AnimTargetMatColorFloat_Konst0B,
    //! コンスタントレジスタ 0 の A 成分です。
    AnimTargetMatColorFloat_Konst0A,

    //! コンスタントカラー 0 の R 成分です。
    AnimTargetMatColorFloat_C0_R,
    //! コンスタントカラー 0 の G 成分です。
    AnimTargetMatColorFloat_C0_G,
    //! コンスタントカラー 0 の B 成分です。
    AnimTargetMatColorFloat_C0_B,
    //! コンスタントカラー 0 の A 成分です。
    AnimTargetMatColorFloat_C0_A,

    //! コンスタントカラー 1 の R 成分です。
    AnimTargetMatColorFloat_C1_R,
    //! コンスタントカラー 1 の G 成分です。
    AnimTargetMatColorFloat_C1_G,
    //! コンスタントカラー 1 の B 成分です。
    AnimTargetMatColorFloat_C1_B,
    //! コンスタントカラー 1 の A 成分です。
    AnimTargetMatColorFloat_C1_A,

    //! コンスタントカラー 2 の R 成分です。
    AnimTargetMatColorFloat_C2_R,
    //! コンスタントカラー 2 の G 成分です。
    AnimTargetMatColorFloat_C2_G,
    //! コンスタントカラー 2 の B 成分です。
    AnimTargetMatColorFloat_C2_B,
    //! コンスタントカラー 2 の A 成分です。
    AnimTargetMatColorFloat_C2_A,

    //! コンスタントカラー 3 の R 成分です。
    AnimTargetMatColorFloat_C3_R,
    //! コンスタントカラー 3 の G 成分です。
    AnimTargetMatColorFloat_C3_G,
    //! コンスタントカラー 3 の B 成分です。
    AnimTargetMatColorFloat_C3_B,
    //! コンスタントカラー 3 の A 成分です。
    AnimTargetMatColorFloat_C3_A,

    //! コンスタントカラー 4 の R 成分です。
    AnimTargetMatColorFloat_C4_R,
    //! コンスタントカラー 4 の G 成分です。
    AnimTargetMatColorFloat_C4_G,
    //! コンスタントカラー 4 の B 成分です。
    AnimTargetMatColorFloat_C4_B,
    //! コンスタントカラー 4 の A 成分です。
    AnimTargetMatColorFloat_C4_A,

    //! 列挙子の総数です。
    AnimTargetMatColorFloat_MaxAnimTargetMatColor
};

//---------------------------------------------------------------------------
//! @brief テクスチャ Srt アニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetTexSrt
{
    //! S 座標の移動量です。
    AnimTargetTexSrt_TranslateS = 0,
    //! T 座標の移動量です。
    AnimTargetTexSrt_TranslateT,
    //! 回転量です。
    AnimTargetTexSrt_Rotate,
    //! S 座標方向の拡大率です。
    AnimTargetTexSrt_ScaleS,
    //! T 座標方向の拡大率です。
    AnimTargetTexSrt_ScaleT,

    //! 列挙子の総数です。
    AnimTargetTexSrt_MaxAnimTargetTexSrt
};

//---------------------------------------------------------------------------
//! @brief テクスチャパターンアニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetTexPattern
{
    //! イメージによるアニメーションです。
    AnimTargetTexPattern_Image = 0,

    //! 列挙子の総数です。
    AnimTargetTexPattern_MaxAnimTargetTexPattern
};

//---------------------------------------------------------------------------
//! @brief インダイレクト Srt アニメーションの対象を指定します。(現状、Transには未対応です。)
//!
//---------------------------------------------------------------------------
enum AnimTargetIndirectSrt
{
    //! 回転量です。
    AnimTargetIndirectSrt_Rotate = 0,
    //! S 座標方向の拡大率です。
    AnimTargetIndirectSrt_ScaleS,
    //! T 座標方向の拡大率です。
    AnimTargetIndirectSrt_ScaleT,

    //! 列挙子の総数です。
    AnimTargetIndirectSrt_MaxAnimTargetIndirectSrt
};

//---------------------------------------------------------------------------
//! @brief フォント影アニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetFontShadow
{
    //! 黒補間カラーの R 成分です。
    AnimTargetFontShadow_BlackInterporateColorR = 0,
    //! 黒補間カラーの G 成分です。
    AnimTargetFontShadow_BlackInterporateColorG,
    //! 黒補間カラーの B 成分です。
    AnimTargetFontShadow_BlackInterporateColorB,
    //! 白補間カラーの R 成分です。
    AnimTargetFontShadow_WhiteInterporateColorR,
    //! 白補間カラーの G 成分です。
    AnimTargetFontShadow_WhiteInterporateColorG,
    //! 白補間カラーの B 成分です。
    AnimTargetFontShadow_WhiteInterporateColorB,
    //! 白補間カラーの A 成分です。
    AnimTargetFontShadow_WhiteInterporateColorA,

    //! 列挙子の総数です。
    AnimTargetFontShadow_MaxAnimTargetFontShadow
};

//---------------------------------------------------------------------------
//! @brief 文字単位アニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetPerCharacterTransform
{
    //! オフセットです。
    AnimTargetPerCharacterTransform_EvalTypeOffset = 0,

    //! 時間幅です。
    AnimTargetPerCharacterTransform_EvalTypeWidth = 1,

    //! 列挙子の総数です。
    AnimTargetPerCharacterTransform_MaxAnimTargetPerCharacterTransform
};

//---------------------------------------------------------------------------
//! @brief 文字単位アニメーションカーブの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetPerCharacterTransformCurve
{
    //! 平行移動Xです。
    AnimTargetPerCharacterTransformCurve_TranslateX = 0,
    //! 平行移動Yです。
    AnimTargetPerCharacterTransformCurve_TranslateY,
    //! 平行移動Zです。
    AnimTargetPerCharacterTransformCurve_TranslateZ,
    //! 回転Xです。
    AnimTargetPerCharacterTransformCurve_RotateX,
    //! 回転Yです。
    AnimTargetPerCharacterTransformCurve_RotateY,
    //! 回転Zです。
    AnimTargetPerCharacterTransformCurve_RotateZ,
    //! 上端カラーRです。
    AnimTargetPerCharacterTransformCurve_LeftTopR,
    //! 上端カラーGです。
    AnimTargetPerCharacterTransformCurve_LeftTopG,
    //! 上端カラーBです。
    AnimTargetPerCharacterTransformCurve_LeftTopB,
    //! 上端カラーAです。
    AnimTargetPerCharacterTransformCurve_LeftTopA,
    //! 下端カラーRです。
    AnimTargetPerCharacterTransformCurve_LeftBottomR,
    //! 下端カラーGです。
    AnimTargetPerCharacterTransformCurve_LeftBottomG,
    //! 下端カラーBです。
    AnimTargetPerCharacterTransformCurve_LeftBottomB,
    //! 下端カラーAです。
    AnimTargetPerCharacterTransformCurve_LeftBottomA,
    //! スケールXです。
    AnimTargetPerCharacterTransformCurve_ScaleX,
    //! スケールYです。
    AnimTargetPerCharacterTransformCurve_ScaleY,
    //! 列挙子の総数です。
    AnimTargetPerCharacterTransformCurve_MaxAnimTargetPerCharacterTransformCurve
};

//---------------------------------------------------------------------------
//! @brief ウィンドウペインカーブの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetWindow
{
    //! ウィンドウペインフレーム上です。
    AnimTargetWindow_FrameTop,
    //! ウィンドウペインフレーム下です。
    AnimTargetWindow_FrameBottom,
    //! ウィンドウペインフレーム左です。
    AnimTargetWindow_FrameLeft,
    //! ウィンドウペインフレーム右です。
    AnimTargetWindow_FrameRight,

    //! 列挙子の総数です。
    AnimTargetWindow_MaxAnimTargetWindow
};

//---------------------------------------------------------------------------
//! @brief イベントアニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetStateMachineEvent
{
    //! 子供へのイベント通知。
    AnimTargetStateMachineEvent_PostToChild,
    AnimTargetStateMachineEvent_MaxAnimTargetStateMachineEvent
};

//---------------------------------------------------------------------------
//! @brief 文字単位アニメーションのループ種類を指定します。
//!
//---------------------------------------------------------------------------
enum PerCharacterTransformLoopType
{
    //! 一度のみの再生です。
    PerCharacterTransformLoopType_OneTime,
    //! ループ再生です。
    PerCharacterTransformLoopType_Loop,

    //! 列挙子の総数です。
    PerCharacterTransformLoopType_MaxPerCharacterTransformLoopType
};

//---------------------------------------------------------------------------
//! @brief 文字単位アニメーションの回転中心を指定します。
//!
//---------------------------------------------------------------------------
enum PerCharacterTransformOriginV
{
    //! 中央です。
    PerCharacterTransformOriginV_Center,
    //! 下端です。
    PerCharacterTransformOriginV_Bottom,

    //! 列挙子の総数です。
    PerCharacterTransformOriginV_MaxPerCharacterTransformOriginV
};

//---------------------------------------------------------------------------
//! @brief アルファコンペアアニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetAlphaCompare
{
    //! アルファコンペアの参照値です。
    AnimTargetAlphaCompare_Ref = 0,

    //! 列挙子の総数です。
    AnimTargetAlphaCompare_MaxAnimTargetAlphaCompare
};

//---------------------------------------------------------------------------
//! @brief マスクテクスチャ行列アニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetMaskTexMtxSrt
{
    //! 平行移動Xです。
    AnimTargetMaskTexSrt_TranslateX = 0,
    //! 平行移動Yです。
    AnimTargetMaskTexSrt_TranslateY,
    //! 回転です。
    AnimTargetMaskTexSrt_Rotate,
    //! スケールXです。
    AnimTargetMaskTexSrt_ScaleX,
    //! スケールYです。
    AnimTargetMaskTexSrt_ScaleY,

    //! 列挙子の総数です。
    AnimTargetMaskTexSrt_MaxAnimTargetMask
};


//---------------------------------------------------------------------------
//! @brief ドロップシャドウ系パラメータアニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetDropShadow
{
    AnimTargetDropShadow_StrokeSize,
    AnimTargetDropShadow_StrokeColor_r,
    AnimTargetDropShadow_StrokeColor_g,
    AnimTargetDropShadow_StrokeColor_b,
    AnimTargetDropShadow_StrokeColor_a,
    AnimTargetDropShadow_OuterGlowColor_r,
    AnimTargetDropShadow_OuterGlowColor_g,
    AnimTargetDropShadow_OuterGlowColor_b,
    AnimTargetDropShadow_OuterGlowColor_a,
    AnimTargetDropShadow_OuterGlowSpread,
    AnimTargetDropShadow_OuterGlowSize,
    AnimTargetDropShadow_DropShadowColor_r,
    AnimTargetDropShadow_DropShadowColor_g,
    AnimTargetDropShadow_DropShadowColor_b,
    AnimTargetDropShadow_DropShadowColor_a,
    AnimTargetDropShadow_DropShadowAngle,
    AnimTargetDropShadow_DropShadowDistance,
    AnimTargetDropShadow_DropShadowSpread,
    AnimTargetDropShadow_DropShadowSize,

    //! 列挙子の総数です。
    AnimTargetDropShadow_MaxAnimTargetDropShadow
};

//---------------------------------------------------------------------------
//! @brief 角丸系パラメータアニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetProceduralShape
{
    AnimTargetProceduralShape_ExpLeftTop,
    AnimTargetProceduralShape_ExpRightTop,
    AnimTargetProceduralShape_ExpLeftBottom,
    AnimTargetProceduralShape_ExpRightBottom,
    AnimTargetProceduralShape_RadiusLeftTop,
    AnimTargetProceduralShape_RadiusRightTop,
    AnimTargetProceduralShape_RadiusLeftBottom,
    AnimTargetProceduralShape_RadiusRightBottom,
    AnimTargetProceduralShape_InnerStrokeSize,
    AnimTargetProceduralShape_InnerStrokeColor_r,
    AnimTargetProceduralShape_InnerStrokeColor_g,
    AnimTargetProceduralShape_InnerStrokeColor_b,
    AnimTargetProceduralShape_InnerStrokeColor_a,
    AnimTargetProceduralShape_InnerShadowColor_r,
    AnimTargetProceduralShape_InnerShadowColor_g,
    AnimTargetProceduralShape_InnerShadowColor_b,
    AnimTargetProceduralShape_InnerShadowColor_a,
    AnimTargetProceduralShape_InnerShadowAngle,
    AnimTargetProceduralShape_InnerShadowDistance,
    AnimTargetProceduralShape_InnerShadowSize,
    AnimTargetProceduralShape_ColorOverlayColor_r,
    AnimTargetProceduralShape_ColorOverlayColor_g,
    AnimTargetProceduralShape_ColorOverlayColor_b,
    AnimTargetProceduralShape_ColorOverlayColor_a,
    AnimTargetProceduralShape_GradationOverlayControl0,
    AnimTargetProceduralShape_GradationOverlayControl1,
    AnimTargetProceduralShape_GradationOverlayControl2,
    AnimTargetProceduralShape_GradationOverlayControl3,
    AnimTargetProceduralShape_GradationOverlayColor0_r,
    AnimTargetProceduralShape_GradationOverlayColor0_g,
    AnimTargetProceduralShape_GradationOverlayColor0_b,
    AnimTargetProceduralShape_GradationOverlayColor0_a,
    AnimTargetProceduralShape_GradationOverlayColor1_r,
    AnimTargetProceduralShape_GradationOverlayColor1_g,
    AnimTargetProceduralShape_GradationOverlayColor1_b,
    AnimTargetProceduralShape_GradationOverlayColor1_a,
    AnimTargetProceduralShape_GradationOverlayColor2_r,
    AnimTargetProceduralShape_GradationOverlayColor2_g,
    AnimTargetProceduralShape_GradationOverlayColor2_b,
    AnimTargetProceduralShape_GradationOverlayColor2_a,
    AnimTargetProceduralShape_GradationOverlayColor3_r,
    AnimTargetProceduralShape_GradationOverlayColor3_g,
    AnimTargetProceduralShape_GradationOverlayColor3_b,
    AnimTargetProceduralShape_GradationOverlayColor3_a,
    AnimTargetProceduralShape_GradationOverlayAngle,
    AnimTargetProceduralShape_OuterShadowColor_r,
    AnimTargetProceduralShape_OuterShadowColor_g,
    AnimTargetProceduralShape_OuterShadowColor_b,
    AnimTargetProceduralShape_OuterShadowColor_a,
    AnimTargetProceduralShape_OuterShadowAngle,
    AnimTargetProceduralShape_OuterShadowDistance,
    AnimTargetProceduralShape_OuterShadowSize,

    //! 列挙子の総数です。
    AnimTargetProceduralShape_MaxAnimTargetProceduralShape
};

//---------------------------------------------------------------------------
//! @brief ベクターグラフィックス情報アニメーションの対象を指定します。
//!
//---------------------------------------------------------------------------
enum AnimTargetVectorGraphicsTextureInfo
{
    AnimTargetVectorGraphicsTextureInfo_Time,

    //! 列挙子の総数です。
    AnimTargetVectorGraphicsTextureInfo_Max
};

//---------------------------------------------------------------------------
//! @brief アニメーションの計算形式です。
//!
//---------------------------------------------------------------------------
enum AnimCurve
{
    //! コンスタント形式です。
    AnimCurve_Constant,
    //! ステップ形式です。
    AnimCurve_Step,
    //! エルミート補間形式です。
    AnimCurve_Hermite,
    //! パラメタライズドアニメーションです。
    AnimCurve_ParameterizedAnim,

    //! 列挙子の総数です。
    AnimCurve_MaxAnimCurve
};

//---------------------------------------------------------------------------
//! @brief パラメタライズドアニメーションの種類です。
//!
//---------------------------------------------------------------------------
enum ParameterizedAnimType
{
    //! リニア
    ParameterizedAnimType_Linear,

    //! EaseIn - Sine
    ParameterizedAnimType_EaseInSine,
    //! EaseOut - Sine
    ParameterizedAnimType_EaseOutSine,
    //! EaseInOut - Sine
    ParameterizedAnimType_EaseInOutSine,

    //! EaseIn - Cubic
    ParameterizedAnimType_EaseInCubic,
    //! EaseOut - Cubic
    ParameterizedAnimType_EaseOutCubic,
    //! EaseInOut - Cubic
    ParameterizedAnimType_EaseInOutCubic,

    //! EaseIn - Quint
    ParameterizedAnimType_EaseInQuint,
    //! EaseOut - Quint
    ParameterizedAnimType_EaseOutQuint,
    //! EaseInOut - Quint
    ParameterizedAnimType_EaseInOutQuint,

    //! EaseIn - Back
    ParameterizedAnimType_EaseInBack,
    //! EaseOut - Back
    ParameterizedAnimType_EaseOutBack,
    //! EaseInOut - Back
    ParameterizedAnimType_EaseInOutBack,

    //! EaseIn - Elastic
    ParameterizedAnimType_EaseInElastic,
    //! EaseOut - Elastic
    ParameterizedAnimType_EaseOutElastic,
    //! EaseInOut - Elastic
    ParameterizedAnimType_EaseInOutElastic,

    //! イベント発火
    ParameterizedAnimType_EventForStateChange,
    ParameterizedAnimType_EventForVariablesSet,
    ParameterizedAnimType_EventForVariablesAdd,

    //! 列挙子の総数です。
    ParameterizedAnimType_MaxType
};

//---------------------------------------------------------------------------
//! @brief アニメーションの計算を制御するオプションです。
//!
//! @sa Layout::Animate
//! @sa Pane::Animate
//!
//---------------------------------------------------------------------------
enum AnimateOpt
{
    AnimateOpt_NoAnimateInvisible   = 0x0001 //!< 非表示のペインの計算を省略します。
};

//---------------------------------------------------------------------------
//! @brief シェーダのバリエーションを識別するための定義です。
//!
//---------------------------------------------------------------------------
enum ShaderVariation
{
    ShaderVariation_Standard,
    ShaderVariation_WithoutVertexColor,
    ShaderVariation_GfxPrimitive,
    ShaderVariation_MaxShaderVariation
};

//---------------------------------------------------------------------------
//! @brief ユーザーが作成した RenderTargetTexture の寿命。
//!
//---------------------------------------------------------------------------
enum RenderTargetTextureLifetime
{
    //! レイアウトの終了処理時にリソースの破棄が行われることを想定した寿命です。
    RenderTargetTextureLifetime_Layout,
    //! 毎フレーム更新処理のたびに初期化と終了処理を繰り返すことを想定した寿命です。
    RenderTargetTextureLifetime_OneFrame,
    RenderTargetTextureLifetime_Max,

    RenderTargetTextureLifetime_Invalid = 0xFFFFFFFF
};

//---------------------------------------------------------------------------
//! @brief 描画するキャプチャテクスチャのタイプです。
//!
//---------------------------------------------------------------------------
enum CaptureTextureDrawType
{
    //! 初回フレームキャプチャテクスチャ
    CaptureTextureDrawType_FirstFrameOnly = (1 << 0),
    //! 動的更新キャプチャ
    CaptureTextureDrawType_Dynamic = (1 << 1),

    //! 全て
    CaptureTextureDrawType_All = (CaptureTextureDrawType_FirstFrameOnly | CaptureTextureDrawType_Dynamic)
};

//---------------------------------------------------------------------------
//! @brief 角丸パラメータ分割設定データの並びタイプです。
//!
//---------------------------------------------------------------------------
enum ProceduralShapeSeparateParamType
{
    ProceduralShapeSeparateParamType_LeftTop,
    ProceduralShapeSeparateParamType_RightTop,
    ProceduralShapeSeparateParamType_LeftBottom,
    ProceduralShapeSeparateParamType_RightBottom,

    ProceduralShapeSeparateParamType_Max
};

//@}

//---------------------------------------------------------------------------
//! @brief ウィンドウペインのフレーム（枠）のサイズを持つ構造体です。
//!
//---------------------------------------------------------------------------
struct WindowFrameSize
{
    float          left; //!< 左辺の枠の幅です。
    float          right; //!< 右辺の枠の幅です。
    float          top; //!< 上辺の枠の高さです。
    float          bottom; //!< 下辺の枠の高さです。
};

//---------------------------------------------------------------------------
//! @brief 幅と高さを持つ構造体です。
//!
//---------------------------------------------------------------------------
struct Size
{
    //! @brief 生成します。
    //!
    //! @param[in] aWidth   幅です。
    //! @param[in] aHeight  高さです。
    //!
    //! @return インスタンスです。
    //!
    static Size Create(float aWidth, float aHeight)
    {
        Size size;

        size.width = aWidth;
        size.height = aHeight;

        return size;
    }

    //! @brief 値を設定します。
    //!
    //! @param[in] aWidth   幅です。
    //! @param[in] aHeight  高さです。
    //!
    void Set(float aWidth, float aHeight)
    {
        this->width = aWidth;
        this->height = aHeight;
    }

    //! 幅です。
    float width;

    //! 高さです。
    float height;
};

//---------------------------------------------------------------------------
//! @brief 二つの Size が等しいかどうかを判定します。
//!
//! @param[in] a    比較する Size 構造体のリファレンスです。
//! @param[in] b    比較する Size 構造体のリファレンスです。
//!
//! @return
//! 幅と高さが等しければ true を、等しくなければ false を返します。
//!
//---------------------------------------------------------------------------
inline bool
operator == (const Size& a, const Size& b)
{
    return a.width == b.width && a.height == b.height;
}

//---------------------------------------------------------------------------
//! @brief テクスチャの幅と高さを持つ構造体です。
//!
//---------------------------------------------------------------------------
class TexSize
{
public:// TODO:アクセサ
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 幅と高さを 0 で初期化します。
    //!
    TexSize()
        : width(0), height(0)
    {}

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 指定の幅と高さで初期化します。
    //!
    //! @param[in] aWidth   幅です。
    //! @param[in] aHeight  高さです。
    //!
    TexSize(uint16_t  aWidth, uint16_t  aHeight)
        : width(aWidth), height(aHeight)
    {}

    //! @brief コピーコンストラクタです。
    //!
    //! @param[in] other    コピー元です。
    //!
    NN_IMPLICIT TexSize(const TexSize& other)
        : width(other.width), height(other.height)
    {}

    //@}

    //! 幅です。
    uint16_t  width;

    //! 高さです。
    uint16_t  height;
};

//---------------------------------------------------------------------------
//! @brief テクスチャ情報を保持するクラスです。
//!
//! @details
//! ユーザーが直接このクラスを利用することはなく、本クラスの派生クラスを利用します。
//!
//! @sa PlacementTextureInfo
//! @sa ResourceTextureInfo
//!
//---------------------------------------------------------------------------
class TextureInfo
{
    NN_DISALLOW_COPY(TextureInfo);

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO_ROOT();

    //! 無効なテクスチャを表します。
    static const uint64_t  InvalidSlot = 0xFFFFFFFFFFFFFFFF;

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 無効なテクスチャオブジェクトへのハンドル (0) とサイズ (0, 0) で初期化します。
    //!
    TextureInfo()
    {
        ResetTextureDescriptorSlot();
    }

    //! @brief デストラクタです。
    virtual ~TextureInfo(){}

    //! @brief テクスチャを解放します。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice) = 0;

    //! @brief スロットをリセットします。
    //!
    void ResetTextureDescriptorSlot()
    {
        m_DescriptorSlot.ToData()->value = InvalidSlot;
    }

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief テクスチャの情報を設定します。
    //!
    //! @details
    //! すべての設定をコピーします。
    //!
    //! @param[in] src  コピー元です。
    //!
    void Set(const TextureInfo& src)
    {
        ResetTextureDescriptorSlot();
        m_DescriptorSlot = *src.GetTextureDescriptorSlot();
    }

    //! @brief テクスチャの利用領域のサイズ (幅, 高さ) を取得します。
    //!
    //! @return テクスチャの利用領域のサイズ (幅, 高さ) を返します。(デフォルトでは、空のサイズを返します。)
    //!
    virtual const TexSize GetSize() const = 0;

    //! @brief テクスチャフォーマットを取得します。
    //!
    //! @return テクスチャフォーマットを返します。
    //!
    virtual TexFormat GetFormat() const
    {
        return nn::gfx::ImageFormat_Undefined;
    }

    //! @brief テクスチャが有効か調べます。
    //!
    //! @return テクスチャが有効な場合には true を返します。
    //!
    virtual bool IsValid() const
    {
        return false;
    }

    //! @brief テクスチャスロットが有効か調べます。
    //!
    //! @return テクスチャスロットが有効な場合には true を返します。
    //!
    bool IsTextureDescriptorSlotReady() const
    {
        return m_DescriptorSlot.ToData()->value != InvalidSlot;
    }

    //! @brief テクスチャビューを取得します。
    //!
    //! @return テクスチャビューを返します。
    //!
    virtual const nn::gfx::TextureView* GetTextureView() const = 0;

    //! @brief テクスチャビューを取得します。
    //!
    //! @return テクスチャビューを返します。
    //!
    virtual nn::gfx::TextureView* GetTextureView() = 0;

    //! @brief テクスチャディスクリプタースロットを取得します。
    //!
    //! @return テクスチャディスクリプタースロットを返します。
    //!
    const nn::gfx::DescriptorSlot* GetTextureDescriptorSlot() const { return &m_DescriptorSlot; }

    //! @brief テクスチャディスクリプタースロットを取得します。
    //!
    //! @return テクスチャディスクリプタースロットを返します。
    //!
    nn::gfx::DescriptorSlot* GetTextureDescriptorSlot() { return &m_DescriptorSlot; }

    // TextureInfo へ関連付けられた汎用のポインタを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return 汎用データポインタです。
    //!
    virtual detail::DynamicRenderingTexture* GetPrivateTextureInstancePtr() const
    {
        return NULL;
    };

    //@}

private:
    // value が 2 のべき数であれば true を返す。
    static bool IsPowerOfTwo(int  value)
    {
        return (value & ~(value - 1)) == value;
    }

    nn::gfx::DescriptorSlot m_DescriptorSlot;
};

//---------------------------------------------------------------------------
//! @brief テクスチャ情報を保持するクラスです。
//!
//! @details
//! ユーザーが用意したテクスチャをもとに、テクスチャ情報を構築します。
//! マテリアルに設定する前に、あらかじめ GetTextureDescriptorSlot() で取得できる、テクスチャディスクリプタースロット にテクスチャを割り当てておくようにしてください。
//!
//! @sa ResourceTextureInfo
//! @sa ResourceAccessor
//!
//---------------------------------------------------------------------------
class PlacementTextureInfo : public TextureInfo
{
    NN_DISALLOW_COPY(PlacementTextureInfo);

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(TextureInfo);

    //! @brief コンストラクタです。
    //!
    PlacementTextureInfo()
    {
    }

    //! @brief デストラクタです。
    //!
    virtual ~PlacementTextureInfo()
    {
    }

    //! @brief テクスチャを解放します。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice) NN_OVERRIDE
    {
        NN_UNUSED(pDevice);
    }

    //! @brief テクスチャのサイズを設定します。
    //!
    //! @param[in] width 幅です。
    //! @param[in] height 高さです。
    //!
    void SetSize(int width, int height)
    {
        m_Size.width = static_cast<uint16_t>(width);
        m_Size.height = static_cast<uint16_t>(height);
    }

    //! @brief テクスチャの利用領域のサイズ (幅, 高さ) を取得します。
    //!
    //! @return テクスチャの利用領域のサイズ (幅, 高さ) を返します。(デフォルトでは、空のサイズを返します。)
    //!
    virtual const TexSize GetSize() const NN_OVERRIDE
    {
        return m_Size;
    }

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief テクスチャが有効か調べます。
    //!
    //! @return テクスチャが有効な場合には true を返します。
    //!
    virtual bool IsValid() const NN_OVERRIDE
    {
        return IsTextureDescriptorSlotReady();
    }

    //! @brief テクスチャビューを取得します。
    //!
    //! @return テクスチャビューを返します。
    //!
    virtual const nn::gfx::TextureView* GetTextureView() const NN_OVERRIDE { return NULL; }

    //! @brief テクスチャビューを取得します。
    //!
    //! @return テクスチャビューを返します。
    //!
    virtual nn::gfx::TextureView* GetTextureView() NN_OVERRIDE { return NULL; }

    //@}

private:
    TexSize m_Size;
};

//---------------------------------------------------------------------------
//! @brief テクスチャ情報を保持するクラスです。
//!
//! @details
//! nn::gfx::ResTexture をもとにテクスチャ情報を生成します。
//!
//! @sa PlacementTextureInfo
//! @sa ResourceAccessor
//!
//---------------------------------------------------------------------------
class ResourceTextureInfo : public TextureInfo
{
    NN_DISALLOW_COPY(ResourceTextureInfo);

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(TextureInfo);

    //! @brief コンストラクタです。
    //!
    ResourceTextureInfo()
        : m_pResTexture(NULL)
    {
    }

    //! @brief デストラクタです。
    //!
    virtual ~ResourceTextureInfo()
    {
    }

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief テクスチャが有効か調べます。
    //!
    //! @return テクスチャが有効な場合には true を返します。
    //!
    virtual bool IsValid() const NN_OVERRIDE
    {
        const TexSize size = GetSize();
        return size.width != 0 && size.height != 0;
    }

    //! @brief テクスチャの利用領域のサイズ (幅, 高さ) を取得します。
    //!
    //! @return テクスチャの利用領域のサイズ (幅, 高さ) を返します。
    //!
    virtual const TexSize GetSize() const NN_OVERRIDE
    {
        nn::gfx::TextureInfo* pTextureInfo = m_pResTexture->GetTextureInfo();
        return TexSize(static_cast<uint16_t>(pTextureInfo->GetWidth()), static_cast<uint16_t>(m_pResTexture->GetTextureInfo()->GetHeight()));
    }

    //! @brief テクスチャを取得します。
    //!
    //! @return テクスチャを返します。
    //!
    const nn::gfx::Texture* GetTexture() const { return m_pResTexture->GetTexture(); }

    //! @brief テクスチャを取得します。
    //!
    //! @return テクスチャを返します。
    //!
    nn::gfx::Texture* GetTexture() { return m_pResTexture->GetTexture(); }

    //! @brief テクスチャビューを取得します。
    //!
    //! @return テクスチャビューを返します。
    //!
    virtual const nn::gfx::TextureView* GetTextureView() const NN_OVERRIDE { return m_pResTexture->GetTextureView(); }

    //! @brief テクスチャビューを取得します。
    //!
    //! @return テクスチャビューを返します。
    //!
    virtual nn::gfx::TextureView* GetTextureView() NN_OVERRIDE { return m_pResTexture->GetTextureView(); }

    //! @brief ResTexture を取得します。
    //!
    //! @return ResTexture を返します。
    //!
    const nn::gfx::ResTexture* GetResTexture() const { return m_pResTexture; }

    //! @brief テクスチャフォーマットを取得します。
    //!
    //! @return テクスチャフォーマットを返します。
    //!
    virtual TexFormat GetFormat() const NN_OVERRIDE
    {
        return m_pResTexture->GetTextureInfo()->GetImageFormat();
    }

    //@}

    //----------------------------------------
    //! @name 初期化／終了処理
    //@{

    //! @brief 初期化します。
    //!
    //! @param[in] pDevice          デバイスです。
    //! @param[in] pResTexture      テクスチャリソースです。
    //!
    void InitializeFromResource(nn::gfx::Device* pDevice, nn::gfx::ResTexture* pResTexture)
    {
        m_pResTexture = pResTexture;

        nn::gfx::Texture* pTexture = m_pResTexture->GetTexture();
        if(pTexture == NULL || !nn::gfx::IsInitialized(*pTexture))
        {
            m_pResTexture->Initialize(pDevice);
        }
    }

    //! @brief テクスチャを解放します。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice) NN_OVERRIDE
    {
        if(m_pResTexture != NULL)
        {
            nn::gfx::Texture* pTexture = m_pResTexture->GetTexture();
            if(pTexture != NULL && nn::gfx::IsInitialized(*pTexture))
            {
                m_pResTexture->Finalize(pDevice);
                m_pResTexture = NULL;
            }
        }
    }

    //@}

private:

    nn::gfx::ResTexture* m_pResTexture;
};

//---------------------------------------------------------------------------
//! @brief RenderTarget テクスチャ情報を保持するクラスです。
//!
//! @sa PlacementTextureInfo
//! @sa ResourceAccessor
//!
//---------------------------------------------------------------------------
class RenderTargetTextureInfo : public PlacementTextureInfo
{
    NN_DISALLOW_COPY(RenderTargetTextureInfo);

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(PlacementTextureInfo);

    //! @brief コンストラクタです。
    //!
    RenderTargetTextureInfo()
        : m_pTexture(NULL)
        , m_pTextureView(NULL)
        , m_pTextureSlot(NULL)
        , m_pOwnerLayout(NULL)
        , m_pPrivateTexturePtr(NULL)
        , m_Format(nn::gfx::ImageFormat_Undefined)
        , m_LifeTime(RenderTargetTextureLifetime_Invalid)
    {
    }

    //! @brief デストラクタです。
    //!
    virtual ~RenderTargetTextureInfo()
    {
        NN_SDK_ASSERT(m_pTexture == NULL);
        NN_SDK_ASSERT(m_pTextureView == NULL);
        NN_SDK_ASSERT(m_pTextureSlot == NULL);
    }

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief テクスチャが有効か調べます。
    //!
    //! @return テクスチャが有効な場合には true を返します。
    //!
    virtual bool IsValid() const NN_OVERRIDE
    {
        return m_Format != nn::gfx::ImageFormat_Undefined;
    }

    //! @brief テクスチャを取得します。
    //!
    //! @return テクスチャを返します。
    //!
    const nn::gfx::Texture* GetTexture() const { return m_pTexture; }

    //! @brief テクスチャを取得します。
    //!
    //! @return テクスチャを返します。
    //!
    nn::gfx::Texture* GetTexture() { return m_pTexture; }

    //! @brief テクスチャビューを取得します。
    //!
    //! @return テクスチャビューを返します。
    //!
    virtual const nn::gfx::TextureView* GetTextureView() const NN_OVERRIDE { return m_pTextureView; }

    //! @brief テクスチャビューを取得します。
    //!
    //! @return テクスチャビューを返します。
    //!
    virtual nn::gfx::TextureView* GetTextureView() NN_OVERRIDE
    {
        return m_pTextureView;
    }

    //! @brief カラーターゲットビューを取得します。
    //!
    //! @return カラーターゲットビューを返します。
    //!
    nn::gfx::ColorTargetView* GetColorTargetView(){ return &m_ColorTargetView; }

    //! @brief カラーターゲットビューを取得します。
    //!
    //! @return カラーターゲットビューを返します。
    //!
    const nn::gfx::ColorTargetView* GetColorTargetView() const { return &m_ColorTargetView; }

    //! @brief ViewportScissorState を取得します。
    //!
    //! @return ViewportScissorState を返します。
    //!
    nn::gfx::ViewportScissorState* GetViewportScissorState(){ return &m_ViewportScissorState; }

    //! @brief ViewportScissorState を取得します。
    //!
    //! @return ViewportScissorState を返します。
    //!
    const nn::gfx::ViewportScissorState* GetViewportScissorState() const { return &m_ViewportScissorState; }


    //! @brief テクスチャフォーマットを取得します。
    //!
    //! @return テクスチャフォーマットを返します。
    //!
    virtual TexFormat GetFormat() const NN_OVERRIDE
    {
        return m_Format;
    }

    //! @brief 初期化結果の RenderTargetTextureLifetime を取得します。
    //!
    //! @return 初期化結果の RenderTargetTextureLifetime を返します。
    //!
    RenderTargetTextureLifetime GetLifetime() const
    {
        return m_LifeTime;
    }

    //@}

    //----------------------------------------
    //! @name 初期化／終了処理
    //@{

    //! @brief 初期化します。
    //!
    //! @param[in]  pDevice     デバイスです。
    //! @param[in]  pLayout     このテクスチャを使用するレイアウトインスタンスへのポインタ。
    //! @param[in]  infoType    作成するテクスチャの情報。
    //! @param[in]  lifetimeHint    このテクスチャの寿命管理に関するヒント。
    //!
    void Initialize(nn::gfx::Device* pDevice, const Layout* pLayout, const nn::gfx::TextureInfo& infoType, RenderTargetTextureLifetime lifetimeHint);

    //! @brief テクスチャを解放します。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice) NN_OVERRIDE;

    // TextureInfo へ関連付けられたテクスチャポインタを設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  pTexturePtr   テクスチャポインタです。
    //!
    void SetPrivateTextureInstancePtr(detail::DynamicRenderingTexture* pTexturePtr)
    {
        m_pPrivateTexturePtr = pTexturePtr;
    }

    // TextureInfo へ関連付けられた汎用のポインタを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return テクスチャポインタです。
    //!
    virtual detail::DynamicRenderingTexture* GetPrivateTextureInstancePtr() const NN_OVERRIDE
    {
        return m_pPrivateTexturePtr;
    }

    //@}

private:
    nn::gfx::Texture*       m_pTexture;
    nn::gfx::TextureView*   m_pTextureView;
    nn::gfx::DescriptorSlot*    m_pTextureSlot;

    const Layout*   m_pOwnerLayout;
    detail::DynamicRenderingTexture*    m_pPrivateTexturePtr;

    nn::gfx::ColorTargetView m_ColorTargetView;
    nn::gfx::ViewportScissorState   m_ViewportScissorState;

    TexFormat       m_Format;
    RenderTargetTextureLifetime m_LifeTime;
};

//---------------------------------------------------------------------------
//! @brief ダミーの動的レンダリングテクスチャ情報を保持するクラスです。
//!
//! @sa PlacementTextureInfo
//! @sa ResourceAccessor
//!
//---------------------------------------------------------------------------
class DummyRenderTargetTextureInfo : public TextureInfo
{
    NN_DISALLOW_COPY(DummyRenderTargetTextureInfo);

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(TextureInfo);

    //! @brief  コンストラクタです。
    //!
    //! @param[in] pTextureName テクスチャ名です。
    //!
    explicit DummyRenderTargetTextureInfo(const char* pTextureName);

    //! @brief  デストラクタです。
    //!
    virtual ~DummyRenderTargetTextureInfo()
    {
        NN_SDK_ASSERT(m_pTextureName == NULL);
    }

    //! @brief テクスチャを解放します。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice) NN_OVERRIDE;

    //! @brief テクスチャの利用領域のサイズ (幅, 高さ) を取得します。
    //!
    //! @return テクスチャの利用領域のサイズ (幅, 高さ) を返します。(デフォルトでは、空のサイズを返します。)
    //!
    virtual const TexSize GetSize() const NN_OVERRIDE
    {
        return TexSize();
    }

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief テクスチャが有効か調べます。
    //!
    //! @return テクスチャが有効な場合には true を返します。
    //!
    virtual bool IsValid() const NN_OVERRIDE
    {
        return true;
    }

    //! @brief テクスチャビューを取得します。
    //!
    //! @return テクスチャビューを返します。
    //!
    virtual const nn::gfx::TextureView* GetTextureView() const NN_OVERRIDE { return NULL; }

    //! @brief テクスチャビューを取得します。
    //!
    //! @return テクスチャビューを返します。
    //!
    virtual nn::gfx::TextureView* GetTextureView() NN_OVERRIDE { return NULL; }

    //! @brief テクスチャ名を取得します。
    //!
    //! @return テクスチャ名を返します。
    //!
    const char* GetName() const
    {
        return m_pTextureName;
    }

    //@}
private:
    char*   m_pTextureName;
};



//! @brief ペインの四隅のテクスチャ座標を保持する配列の定義です。
typedef nn::util::Float2 TexCoordQuad[PaneVertex_MaxPaneVertex];

//! 登録したフォントを表します。
typedef const void* FontKey;

//! 登録したテクスチャを表します。
typedef const void* TextureKey;

//---------------------------------------------------------------------------
//! @brief リソースのタイプを表します。
//!
//! @sa ResourceTypeLayout
//! @sa ResourceTypeAnimation
//! @sa ResourceTypeTexture
//! @sa ResourceTypeFont
//! @sa ResourceTypeArchiveFont
//! @sa ResourceTypeArchiveShader
//!
//---------------------------------------------------------------------------
typedef uint32_t  ResType;

//---------------------------------------------------------------------------
//! @brief 整列ペインの水平方向の配置を表す列挙子です。
//!
//---------------------------------------------------------------------------
enum HorizontalAlignment
{
    HorizontalAlignment_Left,
    HorizontalAlignment_Center,
    HorizontalAlignment_Right,

    HorizontalAlignment_Max
};

enum VerticalAlignment
{
    VerticalAlignment_Top,
    VerticalAlignment_Center,
    VerticalAlignment_Bottom,

    VerticalAlignment_Max
};

} // namespace nn::ui2d
} // namespace nn
