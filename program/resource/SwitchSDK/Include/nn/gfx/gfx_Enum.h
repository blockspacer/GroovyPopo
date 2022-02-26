/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
* @file
* @brief 列挙に関する API の宣言
*/

#pragma once

#include <nn/nn_Macro.h>

namespace nn {
namespace gfx {

namespace detail {

enum FilterModeBit
{
    FilterModeBit_Point = 1,
    FilterModeBit_Linear = 2,
    FilterModeBit_MinFilterShift = 4,
    FilterModeBit_MagFilterShift = 2,
    FilterModeBit_MipFilterShift = 0,
    FilterModeBit_MinPoint = FilterModeBit_Point << FilterModeBit_MinFilterShift,
    FilterModeBit_MinLinear = FilterModeBit_Linear << FilterModeBit_MinFilterShift,
    FilterModeBit_MagPoint = FilterModeBit_Point << FilterModeBit_MagFilterShift,
    FilterModeBit_MagLinear = FilterModeBit_Linear << FilterModeBit_MagFilterShift,
    FilterModeBit_MipPoint = FilterModeBit_Point << FilterModeBit_MipFilterShift,
    FilterModeBit_MipLinear = FilterModeBit_Linear << FilterModeBit_MipFilterShift,

    FilterModeBit_MinFilterMask = 0x03 << FilterModeBit_MinFilterShift,
    FilterModeBit_MagFilterMask = 0x03 << FilterModeBit_MagFilterShift,
    FilterModeBit_MipFilterMask = 0x03 << FilterModeBit_MipFilterShift,

    FilterModeBit_Anisotropic = 0x40,
    FilterModeBit_Comparison = 0x80,
    FilterModeBit_Minimum = 0x0100,
    FilterModeBit_Maximum = 0x0200
};

}

/**
* @brief 論理オペレーションです。
*
* @details
* それぞれ列挙子の説明において、src はソースの値、dst はデスティネーションの値を表します。
*/
enum LogicOperation
{
    LogicOperation_Clear, //!< 0 を表します。
    LogicOperation_And, //!< src & dst を表します。
    LogicOperation_AndReverse, //!< src & ~dst を表します。
    LogicOperation_Copy, //!< src を表します。
    LogicOperation_AndInverted, //!< ~src & dst を表します。
    LogicOperation_NoOp, //!< dst を表します。
    LogicOperation_Xor, //!< src ^ dst を表します。
    LogicOperation_Or, //!< src | dst を表します。
    LogicOperation_Nor, //!< ~( src | dst ) を表します。
    LogicOperation_Equiv, //!< ~( src ^ dst ) を表します。
    LogicOperation_Invert, //!< ~dst を表します。
    LogicOperation_OrReverse, //!< src | ~dst を表します。
    LogicOperation_CopyInverted, //!< ~src を表します。
    LogicOperation_OrInverted, //!< ~src | dst を表します。
    LogicOperation_Nand, //!< ~( src & dst ) を表します。
    LogicOperation_Set, //!< 1 を表します。

    LogicOperation_End
};

/**
* @brief ブレンド関数です。
*
* @details
* それぞれの列挙子の説明において、src はソースの値、srcfactor はソースのブレンドファクター、dst はデスティネーションの値、dstfactor はデスティネーションファクターを表します。
*/
enum BlendFunction
{
    BlendFunction_Add, //!< src * srcfactor + dst * dstfactor を表します。
    BlendFunction_Subtract, //!< src * srcfactor - dst * dstfactor を表します。
    BlendFunction_ReverseSubtract, //!< dst * srcparam - src * dstparam を表します。
    BlendFunction_Min, //!< min( src, dst ) を表します。
    BlendFunction_Max, //!< max( src, dst ) を表します。

    BlendFunction_End
};

/**
* @brief ブレンドファクターです。
*
* @details
* それぞれの列挙子の説明において、src0 はインデックス 0 でのソースの値、src1 はインデックス 1 でのソースの値、const はブレンド定数の値、dst はデスティネーションの値を表します。
*/
enum BlendFactor
{
    BlendFactor_Zero, //!< 0 を表します。
    BlendFactor_One, //!< 1 を表します。
    BlendFactor_SourceColor, //!< src0 を表します。
    BlendFactor_OneMinusSourceColor, //!< 1 - src0 を表します。
    BlendFactor_DestinationColor, //!< dst を表します。
    BlendFactor_OneMinusDestinationColor, //!< 1 - dst を表します。
    BlendFactor_SourceAlpha, //!< src0.alpha を表します。
    BlendFactor_OneMinusSourceAlpha, //!< 1 - src0.alpha を表します。
    BlendFactor_DestinationAlpha, //!< dstA を表します。
    BlendFactor_OneMinusDestinationAlpha, //!< 1 - dst.alpha を表します。
    BlendFactor_ConstantColor, //!< const を表します。
    BlendFactor_OneMinusConstantColor, //!< 1 - const を表します。
    BlendFactor_ConstantAlpha, //!< const.alpha を表します。
    BlendFactor_OneMinusConstantAlpha, //!< 1 - const.alpha を表します。
    BlendFactor_SourceAlphaSaturate, //!< rgb については min( src0.alpha, 1 - dst.alpha )、alpha については 1 を表します。
    BlendFactor_Source1Color, //!< src1 を表します。
    BlendFactor_OneMinusSource1Color, //!< 1 - src1 を表します。
    BlendFactor_Source1Alpha, //!< src1.alpha を表します。
    BlendFactor_OneMinusSource1Alpha, //!< 1 - src1.alpha を表します。

    BlendFactor_End
};

/**
* @brief ステンシルオペレーションです。
*/
enum StencilOperation
{
    StencilOperation_Keep, //!< 現在の値を保持します。
    StencilOperation_Zero, //!< ゼロをセットします。
    StencilOperation_Replace, //!< 参照値で置き換えます。
    StencilOperation_IncrementClamp, //!< インクリメントします。最大値の場合は変更しません。
    StencilOperation_DecrementClamp, //!< デクリメントします。0 の場合は変更しません。
    StencilOperation_Invert, //!< ビットを反転させます。
    StencilOperation_IncrementWrap, //!< インクリメントします。最大値の場合は 0 をセットします。
    StencilOperation_DecrementWrap, //!< デクリメントします。0 の場合は最大値をセットします。

    StencilOperation_End
};

/**
* @brief チャンネルマスクです。
*/
enum ChannelMask
{
    ChannelMask_Red = 0x01, //!< 赤要素です。
    ChannelMask_Green = 0x02, //!< 緑要素です。
    ChannelMask_Blue = 0x04, //!< 青要素です。
    ChannelMask_Alpha = 0x08, //!< アルファ要素です。
    ChannelMask_All = ( ChannelMask_Red | ChannelMask_Green //!< 全要素です。
        | ChannelMask_Blue | ChannelMask_Alpha )
};

/**
* @brief プリミティブトポロジーです。
*/
enum PrimitiveTopology
{
    PrimitiveTopology_PointList, //!< 点のリストです。
    PrimitiveTopology_LineList, //!< 線のリストです。
    PrimitiveTopology_LineStrip, //!< 線ストリップです。
    PrimitiveTopology_TriangleList, //!< 三角形のリストです。
    PrimitiveTopology_TriangleStrip, //!< 三角形ストリップです。
    PrimitiveTopology_LineListAdjacency, //!< 隣接情報付きの線のリストです。
    PrimitiveTopology_LineStripAdjacency, //!< 隣接情報付きの線ストリップです。
    PrimitiveTopology_TriangleListAdjacency, //!< 隣接情報付きの三角形のリストです。
    PrimitiveTopology_TriangleStripAdjacency, //!< 隣接情報付きの三角形ストリップです。
    PrimitiveTopology_PatchList, //!< パッチのリストです。

    PrimitiveTopology_End
};

/**
* @brief プリミティブトポロジーの種類です。
*/
enum PrimitiveTopologyType
{
    PrimitiveTopologyType_Undefined, //!< 不明です
    PrimitiveTopologyType_Point, //!< 点です
    PrimitiveTopologyType_Line, //!< 線です
    PrimitiveTopologyType_Triangle, //!< 三角形です
    PrimitiveTopologyType_Patch, //!< パッチです

    PrimitiveTopologyType_End
};

/**
* @brief 保守的なラスタライゼーションモードです。
*/
enum ConservativeRasterizationMode
{
    ConservativeRasterizationMode_Disable, //!< 保守的なラスタライゼーションが無効です
    ConservativeRasterizationMode_Enable, //!< 保守的なラスタライゼーションが有効です

    ConservativeRasterizationMode_End
};

/**
* @brief インデックスフォーマットです。
*/
enum IndexFormat
{
    IndexFormat_Uint8, //!< 8 ビット符号なし整数です。
    IndexFormat_Uint16, //!< 16 ビット符号なし整数です。
    IndexFormat_Uint32, //!< 32 ビット符号なし整数です。

    IndexFormat_End
};

/**
* @brief 塗りつぶしモードです。
*/
enum FillMode
{
    FillMode_Wireframe = 1, //!< ワイヤーフレームです。
    FillMode_Solid, //!< ポリゴンの内側を塗りつぶします。

    FillMode_End
};

/**
* @brief カリングモードです。
*/
enum CullMode
{
    CullMode_None, //!< カリングしません。
    CullMode_Front, //!< 前面をカリングします。
    CullMode_Back, //!< 背面をカリングします。

    CullMode_End
};

/**
* @brief どの面を前面とするかの定義です。
*/
enum FrontFace
{
    FrontFace_Ccw, //!< 反時計回りを前面とします。
    FrontFace_Cw, //!< 時計回りを前面とします。

    FrontFace_End
};

/**
* @brief テクスチャーの境界外のアドレスの計算モードです。
*/
enum TextureAddressMode
{
    TextureAddressMode_Repeat, //!< 境界内のアドレスを繰り返します。
    TextureAddressMode_Mirror, //!< 境界内のアドレスを反転しながら繰り返します。
    TextureAddressMode_ClampToEdge, //!< 端にクランプします。
    TextureAddressMode_ClampToBorder, //!< ボーダーカラーにクランプします。
    TextureAddressMode_MirrorClampToEdge, //!< 負の場合は一度だけ境界内のアドレスを反転し、以降は端にクランプします。

    TextureAddressMode_End
};

/**
* @brief テクスチャーサンプリングのフィルターモードです。
*/
enum FilterMode
{
    FilterMode_MinPoint_MagPoint_MipPoint = detail::FilterModeBit_MinPoint |
        detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipPoint, //!< ポイント縮小フィルター、ポイント拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_MinPoint_MagPoint_MipLinear = detail::FilterModeBit_MinPoint |
        detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipLinear,//!< ポイント縮小フィルター、ポイント拡大フィルター、線形ミップマップフィルターです。
    FilterMode_MinPoint_MagLinear_MipPoint = detail::FilterModeBit_MinPoint |
        detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipPoint, //!< ポイント縮小フィルター、線形拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_MinPoint_MagLinear_MipLinear = detail::FilterModeBit_MinPoint |
        detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipLinear, //!< ポイント縮小フィルター、線形拡大フィルター、線形ミップマップフィルターです。
    FilterMode_MinLinear_MagPoint_MipPoint = detail::FilterModeBit_MinLinear |
        detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipPoint, //!< 線形縮小フィルター、ポイント拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_MinLinear_MagPoint_MipLinear = detail::FilterModeBit_MinLinear |
        detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipLinear, //!< 線形縮小フィルター、ポイント拡大フィルター、線形ミップマップフィルターです。
    FilterMode_MinLinear_MagLinear_MipPoint = detail::FilterModeBit_MinLinear |
        detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipPoint, //!< 線形縮小フィルター、線形拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_MinLinear_MagLinear_MipLinear = detail::FilterModeBit_MinLinear |
        detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipLinear, //!< 線形縮小フィルター、線形拡大フィルター、線形ミップマップフィルターです。
    FilterMode_Anisotropic = detail::FilterModeBit_Anisotropic | detail::FilterModeBit_MinLinear |
        detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipLinear, //!< 異方性フィルターです。
    FilterMode_Comparison_MinPoint_MagPoint_MipPoint = detail::FilterModeBit_Comparison |
        detail::FilterModeBit_MinPoint | detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipPoint, //!< 比較モード、ポイント縮小フィルター、ポイント拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_Comparison_MinPoint_MagPoint_MipLinear = detail::FilterModeBit_Comparison |
        detail::FilterModeBit_MinPoint | detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipLinear, //!< 比較モード、ポイント縮小フィルター、ポイント拡大フィルター、線形ミップマップフィルターです。
    FilterMode_Comparison_MinPoint_MagLinear_MipPoint = detail::FilterModeBit_Comparison |
        detail::FilterModeBit_MinPoint | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipPoint, //!< 比較モード、ポイント縮小フィルター、線形拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_Comparison_MinPoint_MagLinear_MipLinear = detail::FilterModeBit_Comparison |
        detail::FilterModeBit_MinPoint | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipLinear, //!< 比較モード、ポイント縮小フィルター、線形拡大フィルター、線形ミップマップフィルターです。
    FilterMode_Comparison_MinLinear_MagPoint_MipPoint = detail::FilterModeBit_Comparison |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipPoint, //!< 比較モード、線形縮小フィルター、ポイント拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_Comparison_MinLinear_MagPoint_MipLinear = detail::FilterModeBit_Comparison |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipLinear, //!< 比較モード、線形縮小フィルター、ポイント拡大フィルター、線形ミップマップフィルターです。
    FilterMode_Comparison_MinLinear_MagLinear_MipPoint = detail::FilterModeBit_Comparison |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipPoint, //!< 比較モード、線形縮小フィルター、線形拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_Comparison_MinLinear_MagLinear_MipLinear = detail::FilterModeBit_Comparison |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipLinear, //!< 比較モード、線形縮小フィルター、線形拡大フィルター、線形ミップマップフィルターです。
    FilterMode_Comparison_Anisotropic = detail::FilterModeBit_Comparison | detail::FilterModeBit_Anisotropic |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipLinear, //!< 比較モード、異方性フィルターです。
    FilterMode_Minimum_MinPoint_MagPoint_MipPoint = detail::FilterModeBit_Minimum |
        detail::FilterModeBit_MinPoint | detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipPoint, //!< 最小リダクション、ポイント縮小フィルター、ポイント拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_Minimum_MinPoint_MagPoint_MipLinear = detail::FilterModeBit_Minimum |
        detail::FilterModeBit_MinPoint | detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipLinear,//!< 最小リダクション、ポイント縮小フィルター、ポイント拡大フィルター、線形ミップマップフィルターです。
    FilterMode_Minimum_MinPoint_MagLinear_MipPoint = detail::FilterModeBit_Minimum |
        detail::FilterModeBit_MinPoint | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipPoint, //!< 最小リダクション、ポイント縮小フィルター、線形拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_Minimum_MinPoint_MagLinear_MipLinear = detail::FilterModeBit_Minimum |
        detail::FilterModeBit_MinPoint | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipLinear, //!< 最小リダクション、ポイント縮小フィルター、線形拡大フィルター、線形ミップマップフィルターです。
    FilterMode_Minimum_MinLinear_MagPoint_MipPoint = detail::FilterModeBit_Minimum |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipPoint, //!< 最小リダクション、線形縮小フィルター、ポイント拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_Minimum_MinLinear_MagPoint_MipLinear = detail::FilterModeBit_Minimum |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipLinear, //!< 最小リダクション、線形縮小フィルター、ポイント拡大フィルター、線形ミップマップフィルターです。
    FilterMode_Minimum_MinLinear_MagLinear_MipPoint = detail::FilterModeBit_Minimum |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipPoint, //!< 最小リダクション、線形縮小フィルター、線形拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_Minimum_MinLinear_MagLinear_MipLinear = detail::FilterModeBit_Minimum |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipLinear, //!< 最小リダクション、線形縮小フィルター、線形拡大フィルター、線形ミップマップフィルターです。
    FilterMode_Minimum_Anisotropic = detail::FilterModeBit_Minimum | detail::FilterModeBit_Anisotropic |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipLinear, //!< 最小リダクション、異方性フィルターです。
    FilterMode_Maximum_MinPoint_MagPoint_MipPoint = detail::FilterModeBit_Maximum |
        detail::FilterModeBit_MinPoint | detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipPoint, //!< 最大リダクション、ポイント縮小フィルター、ポイント拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_Maximum_MinPoint_MagPoint_MipLinear = detail::FilterModeBit_Maximum |
        detail::FilterModeBit_MinPoint | detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipLinear,//!< 最大リダクション、ポイント縮小フィルター、ポイント拡大フィルター、線形ミップマップフィルターです。
    FilterMode_Maximum_MinPoint_MagLinear_MipPoint = detail::FilterModeBit_Maximum |
        detail::FilterModeBit_MinPoint | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipPoint, //!< 最大リダクション、ポイント縮小フィルター、線形拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_Maximum_MinPoint_MagLinear_MipLinear = detail::FilterModeBit_Maximum |
        detail::FilterModeBit_MinPoint | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipLinear, //!< 最大リダクション、ポイント縮小フィルター、線形拡大フィルター、線形ミップマップフィルターです。
    FilterMode_Maximum_MinLinear_MagPoint_MipPoint = detail::FilterModeBit_Maximum |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipPoint, //!< 最大リダクション、線形縮小フィルター、ポイント拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_Maximum_MinLinear_MagPoint_MipLinear = detail::FilterModeBit_Maximum |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagPoint | detail::FilterModeBit_MipLinear, //!< 最大リダクション、線形縮小フィルター、ポイント拡大フィルター、線形ミップマップフィルターです。
    FilterMode_Maximum_MinLinear_MagLinear_MipPoint = detail::FilterModeBit_Maximum |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipPoint, //!< 最大リダクション、線形縮小フィルター、線形拡大フィルター、ポイントミップマップフィルターです。
    FilterMode_Maximum_MinLinear_MagLinear_MipLinear = detail::FilterModeBit_Maximum |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipLinear, //!< 最大リダクション、線形縮小フィルター、線形拡大フィルター、線形ミップマップフィルターです。
    FilterMode_Maximum_Anisotropic = detail::FilterModeBit_Maximum | detail::FilterModeBit_Anisotropic |
        detail::FilterModeBit_MinLinear | detail::FilterModeBit_MagLinear | detail::FilterModeBit_MipLinear, //!< 最大リダクション、異方性フィルターです。
};

/**
* @brief 比較関数です。
*/
enum ComparisonFunction
{
    ComparisonFunction_Never, //!< false を表します。
    ComparisonFunction_Less, //!< lhs < rhs を表します。
    ComparisonFunction_Equal, //!< lhs == rhs を表します。
    ComparisonFunction_LessEqual, //!< lhs <= rhs を表します。
    ComparisonFunction_Greater, //!< lhs > rhs を表します。
    ComparisonFunction_NotEqual, //!< lhs != rhs を表します。
    ComparisonFunction_GreaterEqual, //!< lhs >= rhs を表します。
    ComparisonFunction_Always, //!< true を表します。

    ComparisonFunction_End
};

/**
* @brief テクスチャーボーダーカラーのタイプです。
*/
enum TextureBorderColorType
{
    TextureBorderColorType_White, //!< ( 1, 1, 1, 1 ) を表します。
    TextureBorderColorType_TransparentBlack, //!< ( 0, 0, 0, 0 ) を表します。
    TextureBorderColorType_OpaqueBlack, //!< ( 0, 0, 0, 1 ) を表します。

    TextureBorderColorType_End
};

/**
* @brief イメージデータの次元です。
*/
enum ImageStorageDimension
{
    ImageStorageDimension_Undefined,
    ImageStorageDimension_1d, //!< 一次元です
    ImageStorageDimension_2d, //!< 二次元です
    ImageStorageDimension_3d //!< 三次元です
};

/**
* @brief イメージの次元です。
*/
enum ImageDimension
{
    ImageDimension_1d, //!< 一次元です。
    ImageDimension_2d, //!< 二次元です。
    ImageDimension_3d, //!< 三次元です。
    ImageDimension_CubeMap, //!< キューブマップイメージです。
    ImageDimension_1dArray, //!< 一次元配列です。
    ImageDimension_2dArray, //!< 二次元配列です。
    ImageDimension_2dMultisample, //!< マルチサンプルイメージです。
    ImageDimension_2dMultisampleArray, //!< マルチサンプルイメージ配列です。
    ImageDimension_CubeMapArray, //!< キューブマップイメージ配列です。

    ImageDimension_End
};

/**
* @brief チャンネルフォーマットです。
*
* @details
* パッキングされたフォーマットについては下位ビットから順に表記されています。
*/
enum ChannelFormat
{
    ChannelFormat_Undefined, //!< 未定義のフォーマットです。
    ChannelFormat_R4_G4, //!< 各要素 4 ビットの 2 要素 8 ビットフォーマットです。
    ChannelFormat_R8, //!< 8 ビットの単一要素フォーマットです。
    ChannelFormat_R4_G4_B4_A4, //!< 各要素 4 ビットの 4 要素 16 ビットフォーマットです。
    ChannelFormat_A4_B4_G4_R4, //!< 各要素 4 ビットの 4 要素 16 ビット反転フォーマットです。
    ChannelFormat_R5_G5_B5_A1, //!< 最初の 3 要素が 5 ビット、最後の 1 要素が 1 ビットの 4 要素 16 ビットフォーマットです。
    ChannelFormat_A1_B5_G5_R5, //!< 最初の 1 要素が 1 ビット、残りの 3 要素が 5 ビットの 4 要素 16 ビット反転フォーマットです。
    ChannelFormat_R5_G6_B5, //!< 5 ビット、6 ビット、5 ビットの 3 要素 16 ビットフォーマットです。
    ChannelFormat_B5_G6_R5, //!< 5 ビット、6 ビット、5 ビットの 3 要素 16 ビット反転フォーマットです。
    ChannelFormat_R8_G8, //!< 各要素 8 ビットの 2 要素 16 ビットフォーマットです。
    ChannelFormat_R16, //!< 16 ビットの単一要素フォーマットです。
    ChannelFormat_R8_G8_B8_A8, //!< 各要素 8 ビットの 4 要素 32 ビットフォーマットです。
    ChannelFormat_B8_G8_R8_A8, //!< 各要素 8 ビットの 4 要素 32 ビットフォーマットです。最初の 3 要素は反転フォーマットです。
    ChannelFormat_R9_G9_B9_E5, //!< 最初の 3 要素が最後の 1 要素を指数として共有するフォーマットです。
    ChannelFormat_R10_G10_B10_A2, //!< 最初の 3 要素が 10 ビット、最後の 1 要素が 2 ビットの 4 要素 32 ビットフォーマットです。
    ChannelFormat_R11_G11_B10, //!< 11 ビット、11 ビット、10 ビットの 3 要素 32 ビットフォーマットです。
    ChannelFormat_B10_G11_R11, //!< 10 ビット、11 ビット、11 ビットの 3 要素 32 ビット反転フォーマットです。
    ChannelFormat_R10_G11_B11, //!< 10 ビット、11 ビット、11 ビットの 3 要素 32 ビットフォーマットです。
    ChannelFormat_R16_G16, //!< 各要素 16 ビットの 2 要素 32 ビットフォーマットです。
    ChannelFormat_R24_G8, //!< 24 ビット、8 ビットの 2 要素 32 ビットフォーマットです。
    ChannelFormat_R32, //!< 32 ビットの単一要素フォーマットです。
    ChannelFormat_R16_G16_B16_A16, //!< 各要素 16 ビットの 4 要素 64 ビットフォーマットです。
    ChannelFormat_R32_G8_X24, //!< 32 ビット、8 ビット、パディング 24 ビットの 3 要素 64 ビットフォーマットです。
    ChannelFormat_R32_G32, //!< 各要素 32 ビットの 2 要素 64 ビットフォーマットです。
    ChannelFormat_R32_G32_B32, //!< 各要素 32 ビットの 3 要素 96 ビットフォーマットです。
    ChannelFormat_R32_G32_B32_A32, //!< 各要素 32 ビットの 4 要素 128 ビットフォーマットです。
    ChannelFormat_Bc1, //!< BC1 圧縮フォーマットです。
    ChannelFormat_Bc2, //!< BC2 圧縮フォーマットです。
    ChannelFormat_Bc3, //!< BC3 圧縮フォーマットです。
    ChannelFormat_Bc4, //!< BC4 圧縮フォーマットです。
    ChannelFormat_Bc5, //!< BC5 圧縮フォーマットです。
    ChannelFormat_Bc6, //!< BC6 圧縮フォーマットです。
    ChannelFormat_Bc7, //!< BC7 圧縮フォーマットです。
    ChannelFormat_Eac_R11, //!< EAC R11 圧縮フォーマットです。
    ChannelFormat_Eac_R11_G11, //!< EAC R11 G11 圧縮フォーマットです。
    ChannelFormat_Etc1, //!< ETC1 圧縮フォーマットです。
    ChannelFormat_Etc2, //!< ETC2 圧縮フォーマットです。
    ChannelFormat_Etc2_Mask, //!< 1bit アルファ ETC2 圧縮フォーマットです。
    ChannelFormat_Etc2_Alpha, //!< アルファあり ETC2 圧縮フォーマットです。
    ChannelFormat_Pvrtc1_2Bpp, //!< PVRTC1 2bpp 圧縮フォーマットです。
    ChannelFormat_Pvrtc1_4Bpp, //!< PVRTC1 4bpp 圧縮フォーマットです。
    ChannelFormat_Pvrtc1_Alpha_2Bpp, //!< アルファあり PVRTC1 2bpp 圧縮フォーマットです。
    ChannelFormat_Pvrtc1_Alpha_4Bpp, //!< アルファあり PVRTC1 4bpp 圧縮フォーマットです。
    ChannelFormat_Pvrtc2_Alpha_2Bpp, //!< アルファあり PVRTC2 2bpp 圧縮フォーマットです。
    ChannelFormat_Pvrtc2_Alpha_4Bpp, //!< アルファあり PVRTC2 4bpp 圧縮フォーマットです。
    ChannelFormat_Astc_4x4, //!< ASTC 4x4 圧縮フォーマットです。
    ChannelFormat_Astc_5x4, //!< ASTC 5x4 圧縮フォーマットです。
    ChannelFormat_Astc_5x5, //!< ASTC 5x5 圧縮フォーマットです。
    ChannelFormat_Astc_6x5, //!< ASTC 6x5 圧縮フォーマットです。
    ChannelFormat_Astc_6x6, //!< ASTC 6x6 圧縮フォーマットです。
    ChannelFormat_Astc_8x5, //!< ASTC 8x5 圧縮フォーマットです。
    ChannelFormat_Astc_8x6, //!< ASTC 8x6 圧縮フォーマットです。
    ChannelFormat_Astc_8x8, //!< ASTC 8x8 圧縮フォーマットです。
    ChannelFormat_Astc_10x5, //!< ASTC 10x5 圧縮フォーマットです。
    ChannelFormat_Astc_10x6, //!< ASTC 10x6 圧縮フォーマットです。
    ChannelFormat_Astc_10x8, //!< ASTC 10x8 圧縮フォーマットです。
    ChannelFormat_Astc_10x10, //!< ASTC 10x10 圧縮フォーマットです。
    ChannelFormat_Astc_12x10, //!< ASTC 12x10 圧縮フォーマットです。
    ChannelFormat_Astc_12x12, //!< ASTC 12x12 圧縮フォーマットです。
    ChannelFormat_B5_G5_R5_A1, //!< 最初の 3 要素が 5 ビット、最後の 1 要素が 1 ビットの 4 要素 16 ビットフォーマットです。

    ChannelFormat_End
};

/**
* @brief 型フォーマットです。
*/
enum TypeFormat
{
    TypeFormat_Undefined, //!< 未定義のフォーマットです。
    TypeFormat_Unorm, //!< 符号なし正規化整数です。
    TypeFormat_Snorm, //!< 符号付き正規化整数です。
    TypeFormat_Uint, //!< 符号なし整数です。
    TypeFormat_Sint, //!< 符号付き整数です。
    TypeFormat_Float, //!< 浮動小数点数です。
    TypeFormat_UnormSrgb, //!< SRGB フォーマットです。
    TypeFormat_DepthStencil, //!< 深度ステンシルフォーマットです。
    TypeFormat_UintToFloat, //!< 符号なし整数から浮動小数点数へ変換するフォーマットです。
    TypeFormat_SintToFloat, //!< 符号付き整数から浮動小数点数へ変換するフォーマットです。
    TypeFormat_Ufloat, //!< 符号なし浮動小数点数です。

    TypeFormat_End,

    TypeFormat_Bits = 8
};


/**
* @brief イメージフォーマットです。
*
* @details
* パッキングされたフォーマットについては下位ビットからの順で表記されています。
*/
enum ImageFormat
{
    ImageFormat_Undefined = 0, //!< 未定義のフォーマットです。
    ImageFormat_R8_Unorm = ( ChannelFormat_R8 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 8 ビットの単一要素符号なし正規化フォーマットです。
    ImageFormat_R8_Snorm = ( ChannelFormat_R8 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 8 ビットの単一要素符号付き正規化フォーマットです。
    ImageFormat_R8_Uint = ( ChannelFormat_R8 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 8 ビットの単一要素符号なし整数フォーマットです。
    ImageFormat_R8_Sint = ( ChannelFormat_R8 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 8 ビットの単一要素符号付き整数フォーマットです。
    ImageFormat_R4_G4_B4_A4_Unorm = ( ChannelFormat_R4_G4_B4_A4 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 各要素が 4 ビットの 4 要素 16 ビット符号なし正規化フォーマットです。
    ImageFormat_A4_B4_G4_R4_Unorm = ( ChannelFormat_A4_B4_G4_R4 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 各要素が 4 ビットの 4 要素 16 ビット符号なし正規化反転フォーマットです。
    ImageFormat_R5_G5_B5_A1_Unorm = ( ChannelFormat_R5_G5_B5_A1 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 最初の 3 要素が 5 ビット、最後の 1 要素が 1 ビットの 4 要素 16 ビット符号なし正規化フォーマットです。
    ImageFormat_A1_B5_G5_R5_Unorm = ( ChannelFormat_A1_B5_G5_R5 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 最初の 1 要素が 1 ビット、最後の 3 要素が 3 ビットの 4 要素 16 ビット符号なし正規化フォーマットです。
    ImageFormat_R5_G6_B5_Unorm = ( ChannelFormat_R5_G6_B5 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 5 ビット、6 ビット、5 ビットの 3 要素 16 ビット符号なし正規化フォーマットです。
    ImageFormat_B5_G6_R5_Unorm = ( ChannelFormat_B5_G6_R5 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 5 ビット、6 ビット、5 ビットの 3 要素 16 ビット符号なし正規化反転フォーマットです。
    ImageFormat_R8_G8_Unorm = ( ChannelFormat_R8_G8 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 各要素が 8 ビットの 2 要素 16 ビット符号なし正規化フォーマットです。
    ImageFormat_R8_G8_Snorm = ( ChannelFormat_R8_G8 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 各要素が 8 ビットの 2 要素 16 ビット符号付き正規化フォーマットです。
    ImageFormat_R8_G8_Uint = ( ChannelFormat_R8_G8 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 8 ビットの 2 要素 16 ビット符号なし整数フォーマットです。
    ImageFormat_R8_G8_Sint = ( ChannelFormat_R8_G8 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 8 ビットの 2 要素 16 ビット符号付き整数フォーマットです。
    ImageFormat_R16_Unorm = ( ChannelFormat_R16 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 16 ビットの単一要素符号なし正規化フォーマットです。
    ImageFormat_R16_Snorm = ( ChannelFormat_R16 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 16 ビットの単一要素符号付き正規化フォーマットです。
    ImageFormat_R16_Uint = ( ChannelFormat_R16 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 16 ビットの単一要素符号なし整数フォーマットです。
    ImageFormat_R16_Sint = ( ChannelFormat_R16 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 16 ビットの単一要素符号付き整数フォーマットです。
    ImageFormat_R16_Float = ( ChannelFormat_R16 << TypeFormat_Bits ) | TypeFormat_Float, //!< 16 ビットの単一要素浮動小数点フォーマットです。
    ImageFormat_D16_Unorm = ( ChannelFormat_R16 << TypeFormat_Bits ) | TypeFormat_DepthStencil, //!< 16 ビットの単一要素深度ステンシルフォーマットです。
    ImageFormat_R8_G8_B8_A8_Unorm = ( ChannelFormat_R8_G8_B8_A8 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 各要素が 8 ビットの 4 要素 32 ビット符号なし正規化フォーマットです。
    ImageFormat_R8_G8_B8_A8_Snorm = ( ChannelFormat_R8_G8_B8_A8 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 各要素が 8 ビットの 4 要素 32 ビット符号付き正規化フォーマットです。
    ImageFormat_R8_G8_B8_A8_Uint = ( ChannelFormat_R8_G8_B8_A8 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 8 ビットの 4 要素 32 ビット符号なし整数フォーマットです。
    ImageFormat_R8_G8_B8_A8_Sint = ( ChannelFormat_R8_G8_B8_A8 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 8 ビットの 4 要素 32 ビット符号付き整数フォーマットです。
    ImageFormat_R8_G8_B8_A8_UnormSrgb = ( ChannelFormat_R8_G8_B8_A8 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< 各要素が 8 ビットの 4 要素 32 ビット SRGB フォーマットです。
    ImageFormat_B8_G8_R8_A8_Unorm = ( ChannelFormat_B8_G8_R8_A8 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 各要素が 8 ビットの 4 要素 32 ビット符号なし正規化フォーマットです。最初の 3 要素は反転フォーマットです。
    ImageFormat_B8_G8_R8_A8_Snorm = ( ChannelFormat_B8_G8_R8_A8 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 各要素が 8 ビットの 4 要素 32 ビット符号付き正規化フォーマットです。最初の 3 要素は反転フォーマットです。
    ImageFormat_B8_G8_R8_A8_Uint = ( ChannelFormat_B8_G8_R8_A8 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 8 ビットの 4 要素 32 ビット符号なし整数フォーマットです。最初の 3 要素は反転フォーマットです。
    ImageFormat_B8_G8_R8_A8_Sint = ( ChannelFormat_B8_G8_R8_A8 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 8 ビットの 4 要素 32 ビット符号付き整数フォーマットです。最初の 3 要素は反転フォーマットです。
    ImageFormat_B8_G8_R8_A8_UnormSrgb = ( ChannelFormat_B8_G8_R8_A8 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< 各要素が 8 ビットの 4 要素 32 ビット SRGB フォーマットです。最初の 3 要素は反転フォーマットです。
    ImageFormat_R9_G9_B9_E5_SharedExp = ( ChannelFormat_R9_G9_B9_E5 << TypeFormat_Bits ) | TypeFormat_Float, //!< 最初の 3 要素が最後の 1 要素を指数として共有するフォーマットです。
    ImageFormat_R10_G10_B10_A2_Unorm = ( ChannelFormat_R10_G10_B10_A2 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 最初の 3 要素が 10 ビット、最後の 1 要素が 2 ビットの 4 要素 32 ビット符号なし正規化フォーマットです。
    ImageFormat_R10_G10_B10_A2_Uint = ( ChannelFormat_R10_G10_B10_A2 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 最初の 3 要素が 10 ビット、最後の 1 要素が 2 ビットの 4 要素 32 ビット符号なし整数フォーマットです。
    ImageFormat_R11_G11_B10_Float = ( ChannelFormat_R11_G11_B10 << TypeFormat_Bits ) | TypeFormat_Float, //!< 11 ビット、11 ビット、10 ビットの 3 要素 32 ビット浮動小数点フォーマットです。
    ImageFormat_B10_G11_R11_Float = ( ChannelFormat_B10_G11_R11 << TypeFormat_Bits ) | TypeFormat_Float, //!< 10 ビット、11 ビット、11 ビットの 3 要素 32 ビット浮動小数点フォーマットです。
    ImageFormat_R16_G16_Unorm = ( ChannelFormat_R16_G16 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 各要素が 16 ビットの 2 要素 32 ビット符号なし正規化フォーマットです。
    ImageFormat_R16_G16_Snorm = ( ChannelFormat_R16_G16 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 各要素が 16 ビットの 2 要素 32 ビット符号付き正規化フォーマットです。
    ImageFormat_R16_G16_Uint = ( ChannelFormat_R16_G16 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 16 ビットの 2 要素 32 ビット符号なし整数フォーマットです。
    ImageFormat_R16_G16_Sint = ( ChannelFormat_R16_G16 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 16 ビットの 2 要素 32 ビット符号付き整数フォーマットです。
    ImageFormat_R16_G16_Float = ( ChannelFormat_R16_G16 << TypeFormat_Bits ) | TypeFormat_Float, //!< 各要素が 16 ビットの 2 要素 32 ビット浮動小数点フォーマットです。
    ImageFormat_D24_Unorm_S8_Uint = ( ChannelFormat_R24_G8 << TypeFormat_Bits ) | TypeFormat_DepthStencil, //!< 24 ビット、8 ビットの 2 要素 32 ビット深度ステンシルフォーマットです。
    ImageFormat_R32_Uint = ( ChannelFormat_R32 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 32 ビットの単一要素符号なし整数フォーマットです。
    ImageFormat_R32_Sint = ( ChannelFormat_R32 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 32 ビットの単一要素符号付き整数フォーマットです。
    ImageFormat_R32_Float = ( ChannelFormat_R32 << TypeFormat_Bits ) | TypeFormat_Float, //!< 32 ビットの単一要素浮動小数点フォーマットです。
    ImageFormat_D32_Float = ( ChannelFormat_R32 << TypeFormat_Bits ) | TypeFormat_DepthStencil, //!< 32 ビットの単一要素深度ステンシルフォーマットです。
    ImageFormat_R16_G16_B16_A16_Unorm = ( ChannelFormat_R16_G16_B16_A16 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 各要素が 16 ビットの 4 要素 64 ビット符号なし正規化フォーマットです。
    ImageFormat_R16_G16_B16_A16_Snorm = ( ChannelFormat_R16_G16_B16_A16 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 各要素が 16 ビットの 4 要素 64 ビット符号付き正規化フォーマットです。
    ImageFormat_R16_G16_B16_A16_Uint = ( ChannelFormat_R16_G16_B16_A16 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 16 ビットの 4 要素 64 ビット符号なし整数フォーマットです。
    ImageFormat_R16_G16_B16_A16_Sint = ( ChannelFormat_R16_G16_B16_A16 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 16 ビットの 4 要素 64 ビット符号付き整数フォーマットです。
    ImageFormat_R16_G16_B16_A16_Float = ( ChannelFormat_R16_G16_B16_A16 << TypeFormat_Bits ) | TypeFormat_Float, //!< 各要素が 16 ビットの 4 要素 64 ビット浮動小数点フォーマットです。
    ImageFormat_D32_Float_S8_Uint_X24 = ( ChannelFormat_R32_G8_X24 << TypeFormat_Bits ) | TypeFormat_DepthStencil, //!< 32 ビット、8 ビット、パディング 24 ビットの 3 要素 64 ビット深度ステンシルフォーマットです。
    ImageFormat_R32_G32_Uint = ( ChannelFormat_R32_G32 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 32 ビットの 2 要素 64 ビット符号なし整数フォーマットです。
    ImageFormat_R32_G32_Sint = ( ChannelFormat_R32_G32<< TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 32 ビットの 2 要素 64 ビット符号付き整数フォーマットです。
    ImageFormat_R32_G32_Float = ( ChannelFormat_R32_G32 << TypeFormat_Bits ) | TypeFormat_Float, //!< 各要素が 32 ビットの 2 要素 64 ビット浮動小数点フォーマットです。
    ImageFormat_R32_G32_B32_Uint = ( ChannelFormat_R32_G32_B32 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 32 ビットの 3 要素 96 ビット符号なし整数フォーマットです。
    ImageFormat_R32_G32_B32_Sint = ( ChannelFormat_R32_G32_B32 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 32 ビットの 3 要素 96 ビット符号付き整数フォーマットです。
    ImageFormat_R32_G32_B32_Float = ( ChannelFormat_R32_G32_B32 << TypeFormat_Bits ) | TypeFormat_Float, //!< 各要素が 32 ビットの 3 要素 96 ビット浮動小数点フォーマットです。
    ImageFormat_R32_G32_B32_A32_Uint = ( ChannelFormat_R32_G32_B32_A32 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 32 ビットの 4 要素 128 ビット符号なし整数フォーマットです。
    ImageFormat_R32_G32_B32_A32_Sint = ( ChannelFormat_R32_G32_B32_A32 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 32 ビットの 4 要素 128 ビット符号付き整数フォーマットです。
    ImageFormat_R32_G32_B32_A32_Float = ( ChannelFormat_R32_G32_B32_A32 << TypeFormat_Bits ) | TypeFormat_Float, //!< 各要素が 32 ビットの 4 要素 128 ビット浮動小数点フォーマットです。

    ImageFormat_Bc1_Unorm = ( ChannelFormat_Bc1 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< BC1 圧縮符号なし正規化フォーマットです。
    ImageFormat_Bc1_UnormSrgb = ( ChannelFormat_Bc1 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< BC1 圧縮 SRGB フォーマットです。
    ImageFormat_Bc2_Unorm = ( ChannelFormat_Bc2 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< BC2 圧縮符号なし正規化フォーマットです。
    ImageFormat_Bc2_UnormSrgb = ( ChannelFormat_Bc2 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< BC2 圧縮 SRGB フォーマットです。
    ImageFormat_Bc3_Unorm = ( ChannelFormat_Bc3 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< BC3 圧縮符号なし正規化フォーマットです。
    ImageFormat_Bc3_UnormSrgb = ( ChannelFormat_Bc3 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< BC3 圧縮 SRGB フォーマットです。
    ImageFormat_Bc4_Unorm = ( ChannelFormat_Bc4 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< BC4 圧縮符号なし正規化フォーマットです。
    ImageFormat_Bc4_Snorm = ( ChannelFormat_Bc4 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< BC4 圧縮符号付き正規化フォーマットです。
    ImageFormat_Bc5_Unorm = ( ChannelFormat_Bc5 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< BC5 圧縮符号なし正規化フォーマットです。
    ImageFormat_Bc5_Snorm = ( ChannelFormat_Bc5 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< BC5 圧縮符号付き正規化フォーマットです。
    ImageFormat_Bc6_Float = ( ChannelFormat_Bc6 << TypeFormat_Bits ) | TypeFormat_Float, //!< BC6 圧縮浮動小数点フォーマットです。
    ImageFormat_Bc6_Ufloat = ( ChannelFormat_Bc6 << TypeFormat_Bits ) | TypeFormat_Ufloat, //!< BC6 圧縮符号なし浮動小数点数フォーマットです。
    ImageFormat_Bc7_Unorm = ( ChannelFormat_Bc7 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< BC7 圧縮符号なし正規化フォーマットです。
    ImageFormat_Bc7_UnormSrgb = ( ChannelFormat_Bc7 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< BC7 圧縮 SRGB フォーマットです。
    ImageFormat_Eac_R11_Unorm = ( ChannelFormat_Eac_R11 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< EAC R11 圧縮符号なし正規化フォーマットです
    ImageFormat_Eac_R11_Snorm = ( ChannelFormat_Eac_R11 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< EAC R11 圧縮符号付き正規化フォーマットです
    ImageFormat_Eac_R11_G11_Unorm = ( ChannelFormat_Eac_R11_G11 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< EAC R11 G11 圧縮符号なし正規化フォーマットです
    ImageFormat_Eac_R11_G11_Snorm = ( ChannelFormat_Eac_R11_G11 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< EAC R11 G11 圧縮符号付き正規化フォーマットです
    ImageFormat_Etc1_Unorm = ( ChannelFormat_Etc1 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ETC1 圧縮符号なし正規化フォーマットです
    ImageFormat_Etc2_Unorm = ( ChannelFormat_Etc2 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ETC2 圧縮符号なし正規化フォーマットです
    ImageFormat_Etc2_UnormSrgb = ( ChannelFormat_Etc2 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ETC2 圧縮 SRGB フォーマットです
    ImageFormat_Etc2_Mask_Unorm = ( ChannelFormat_Etc2_Mask << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 1bit アルファ ETC2 圧縮符号なし正規化フォーマットです
    ImageFormat_Etc2_Mask_UnormSrgb = ( ChannelFormat_Etc2_Mask << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< 1bit アルファ ETC2 圧縮 SRGB フォーマットです
    ImageFormat_Etc2_Alpha_Unorm = ( ChannelFormat_Etc2_Alpha << TypeFormat_Bits ) | TypeFormat_Unorm, //!< アルファあり ETC2 圧縮符号なし正規化フォーマットです
    ImageFormat_Etc2_Alpha_UnormSrgb = ( ChannelFormat_Etc2_Alpha << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< アルファあり ETC2 圧縮 SRGB フォーマットです
    ImageFormat_Pvrtc1_2Bpp_Unorm = ( ChannelFormat_Pvrtc1_2Bpp << TypeFormat_Bits ) | TypeFormat_Unorm, //!< PVRTC1 2bpp 圧縮符号なし正規化フォーマットです
    ImageFormat_Pvrtc1_2Bpp_UnormSrgb = ( ChannelFormat_Pvrtc1_2Bpp << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< PVRTC1 2bpp 圧縮 SRGB フォーマットです
    ImageFormat_Pvrtc1_4Bpp_Unorm = ( ChannelFormat_Pvrtc1_4Bpp << TypeFormat_Bits ) | TypeFormat_Unorm, //!< PVRTC1 4bpp 圧縮正規化フォーマットです
    ImageFormat_Pvrtc1_4Bpp_UnormSrgb = ( ChannelFormat_Pvrtc1_4Bpp << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< PVRTC1 4bpp 圧縮 SRGB フォーマットです
    ImageFormat_Pvrtc1_Alpha_2Bpp_Unorm = ( ChannelFormat_Pvrtc1_Alpha_2Bpp << TypeFormat_Bits ) | TypeFormat_Unorm, //!< アルファあり PVRTC1 2bpp 圧縮符号なし正規化フォーマットです
    ImageFormat_Pvrtc1_Alpha_2Bpp_UnormSrgb = ( ChannelFormat_Pvrtc1_Alpha_2Bpp << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< アルファあり PVRTC1 2bpp 圧縮 SRGB フォーマットです
    ImageFormat_Pvrtc1_Alpha_4Bpp_Unorm = ( ChannelFormat_Pvrtc1_Alpha_4Bpp << TypeFormat_Bits ) | TypeFormat_Unorm, //!< アルファあり PVRTC1 4bpp 圧縮符号なし正規化フォーマットです
    ImageFormat_Pvrtc1_Alpha_4Bpp_UnormSrgb = ( ChannelFormat_Pvrtc1_Alpha_4Bpp << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< アルファあり PVTC1 4bpp 圧縮 SRGB フォーマットです
    ImageFormat_Pvrtc2_Alpha_2Bpp_Unorm = ( ChannelFormat_Pvrtc2_Alpha_2Bpp << TypeFormat_Bits ) | TypeFormat_Unorm, //!< アルファあり PVRTC2 2bpp 圧縮符号なし正規化フォーマットです
    ImageFormat_Pvrtc2_Alpha_2Bpp_UnormSrgb = ( ChannelFormat_Pvrtc2_Alpha_2Bpp << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< アルファあり PVRTC2 2bpp 圧縮 SRGB フォーマットです
    ImageFormat_Pvrtc2_Alpha_4Bpp_Unorm = ( ChannelFormat_Pvrtc2_Alpha_4Bpp << TypeFormat_Bits ) | TypeFormat_Unorm, //!< アルファあり PVRTC2 4bpp 圧縮符号なし正規化フォーマットです
    ImageFormat_Pvrtc2_Alpha_4Bpp_UnormSrgb = ( ChannelFormat_Pvrtc2_Alpha_4Bpp << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< アルファあり PVRTC2 4bpp 圧縮 SRGB フォーマットです
    ImageFormat_Astc_4x4_Unorm = ( ChannelFormat_Astc_4x4 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 4x4 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_4x4_UnormSrgb = ( ChannelFormat_Astc_4x4 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 4x4 圧縮 SRGB フォーマットです
    ImageFormat_Astc_5x4_Unorm = ( ChannelFormat_Astc_5x4 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 5x4 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_5x4_UnormSrgb = ( ChannelFormat_Astc_5x4 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 5x4 圧縮 SRGB フォーマットです
    ImageFormat_Astc_5x5_Unorm = ( ChannelFormat_Astc_5x5 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 5x5 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_5x5_UnormSrgb = ( ChannelFormat_Astc_5x5 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 5x5 圧縮 SRGB フォーマットです
    ImageFormat_Astc_6x5_Unorm = ( ChannelFormat_Astc_6x5 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 6x5 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_6x5_UnormSrgb = ( ChannelFormat_Astc_6x5 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 6x5 圧縮 SRGB フォーマットです
    ImageFormat_Astc_6x6_Unorm = ( ChannelFormat_Astc_6x6 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 6x6 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_6x6_UnormSrgb = ( ChannelFormat_Astc_6x6 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 6x6 圧縮 SRGB フォーマットです
    ImageFormat_Astc_8x5_Unorm = ( ChannelFormat_Astc_8x5 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 8x5 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_8x5_UnormSrgb = ( ChannelFormat_Astc_8x5 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 8x5 圧縮 SRGB フォーマットです
    ImageFormat_Astc_8x6_Unorm = ( ChannelFormat_Astc_8x6 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 8x6 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_8x6_UnormSrgb = ( ChannelFormat_Astc_8x6 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 8x6 圧縮 SRGB フォーマットです
    ImageFormat_Astc_8x8_Unorm = ( ChannelFormat_Astc_8x8 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 8x8 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_8x8_UnormSrgb = ( ChannelFormat_Astc_8x8 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 8x8 圧縮 SRGB フォーマットです
    ImageFormat_Astc_10x5_Unorm = ( ChannelFormat_Astc_10x5 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 10x5 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_10x5_UnormSrgb = ( ChannelFormat_Astc_10x5 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 10x5 圧縮 SRGB フォーマットです
    ImageFormat_Astc_10x6_Unorm = ( ChannelFormat_Astc_10x6 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 10x6 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_10x6_UnormSrgb = ( ChannelFormat_Astc_10x6 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 10x6 圧縮 SRGB フォーマットです
    ImageFormat_Astc_10x8_Unorm = ( ChannelFormat_Astc_10x8 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 10x8 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_10x8_UnormSrgb = ( ChannelFormat_Astc_10x8 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 10x8 圧縮 SRGB フォーマットです
    ImageFormat_Astc_10x10_Unorm = ( ChannelFormat_Astc_10x10 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 10x10 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_10x10_UnormSrgb = ( ChannelFormat_Astc_10x10 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 10x10 圧縮 SRGB フォーマットです
    ImageFormat_Astc_12x10_Unorm = ( ChannelFormat_Astc_12x10 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 12x10 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_12x10_UnormSrgb = ( ChannelFormat_Astc_12x10 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 12x10 圧縮 SRGB フォーマットです
    ImageFormat_Astc_12x12_Unorm = ( ChannelFormat_Astc_12x12 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< ASTC 12x12 圧縮符号なし正規化フォーマットです
    ImageFormat_Astc_12x12_UnormSrgb = ( ChannelFormat_Astc_12x12 << TypeFormat_Bits ) | TypeFormat_UnormSrgb, //!< ASTC 12x12 圧縮 SRGB フォーマットです

    ImageFormat_B5_G5_R5_A1_Unorm = ( ChannelFormat_B5_G5_R5_A1 << TypeFormat_Bits ) | TypeFormat_Unorm //!< 最初の 3 要素が 5 ビット、最後の 1 要素が 1 ビットの 4 要素 16 ビット符号なし正規化フォーマットです。
};

/**
* @brief 頂点属性フォーマットです。
*/
enum AttributeFormat
{
    AttributeFormat_Undefined = 0, //!< 未定義のフォーマットです。
    AttributeFormat_4_4_Unorm = ( ChannelFormat_R4_G4 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 各要素が 4 ビットの 2 要素 8 ビット符号なし正規化フォーマットです。
    AttributeFormat_8_Unorm = ( ChannelFormat_R8 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 8 ビットの単一要素符号なし正規化フォーマットです。
    AttributeFormat_8_Snorm = ( ChannelFormat_R8 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 8 ビットの単一要素符号付き正規化フォーマットです。
    AttributeFormat_8_Uint = ( ChannelFormat_R8 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 8 ビットの単一要素符号なし整数フォーマットです。
    AttributeFormat_8_Sint = ( ChannelFormat_R8 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 8 ビットの単一要素符号付き整数フォーマットです。
    AttributeFormat_8_UintToFloat = ( ChannelFormat_R8 << TypeFormat_Bits ) | TypeFormat_UintToFloat, //!< 8 ビットの単一要素符号なし整数から浮動小数点への変換フォーマットです。
    AttributeFormat_8_SintToFloat = ( ChannelFormat_R8 << TypeFormat_Bits ) | TypeFormat_SintToFloat, //!< 8 ビットの単一要素符号付き整数から浮動小数点への変換フォーマットです。
    AttributeFormat_8_8_Unorm = ( ChannelFormat_R8_G8 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 各要素が 8 ビットの 2 要素 16 ビット符号なし正規化フォーマットです。
    AttributeFormat_8_8_Snorm = ( ChannelFormat_R8_G8 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 各要素が 8 ビットの 2 要素 16 ビット符号付き正規化フォーマットです。
    AttributeFormat_8_8_Uint = ( ChannelFormat_R8_G8 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 8 ビットの 2 要素 16 ビット符号なし整数フォーマットです。
    AttributeFormat_8_8_Sint = ( ChannelFormat_R8_G8 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 8 ビットの 2 要素 16 ビット符号付き整数フォーマットです。
    AttributeFormat_8_8_UintToFloat = ( ChannelFormat_R8_G8 << TypeFormat_Bits ) | TypeFormat_UintToFloat, //!< 各要素が 8 ビットの 2 要素 16 ビット符号なし整数から浮動小数点への変換フォーマットです。
    AttributeFormat_8_8_SintToFloat = ( ChannelFormat_R8_G8 << TypeFormat_Bits ) | TypeFormat_SintToFloat, //!< 各要素が 8 ビットの 2 要素 16 ビット符号付き整数から浮動小数点への変換フォーマットです。
    AttributeFormat_16_Unorm = ( ChannelFormat_R16 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 16 ビットの単一要素符号なし正規化フォーマットです。
    AttributeFormat_16_Snorm = ( ChannelFormat_R16 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 16 ビットの単一要素符号付き正規化フォーマットです。
    AttributeFormat_16_Uint = ( ChannelFormat_R16 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 16 ビットの単一要素符号なし整数フォーマットです。
    AttributeFormat_16_Sint = ( ChannelFormat_R16 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 16 ビットの単一要素符号付き整数フォーマットです。
    AttributeFormat_16_Float = ( ChannelFormat_R16 << TypeFormat_Bits ) | TypeFormat_Float, //!< 16 ビットの単一要素浮動小数点フォーマットです。
    AttributeFormat_16_UintToFloat = ( ChannelFormat_R16 << TypeFormat_Bits ) | TypeFormat_UintToFloat, //!< 16 ビットの単一要素符号なし整数から浮動小数点への変換フォーマットです。
    AttributeFormat_16_SintToFloat = ( ChannelFormat_R16 << TypeFormat_Bits ) | TypeFormat_SintToFloat, //!< 16 ビットの単一要素符号付き整数から浮動小数点への変換フォーマットです。
    AttributeFormat_8_8_8_8_Unorm = ( ChannelFormat_R8_G8_B8_A8 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 各要素が 8 ビットの 4 要素 32 ビット符号なし正規化フォーマットです。
    AttributeFormat_8_8_8_8_Snorm = ( ChannelFormat_R8_G8_B8_A8 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 各要素が 8 ビットの 4 要素 32 ビット符号付き正規化フォーマットです。
    AttributeFormat_8_8_8_8_Uint = ( ChannelFormat_R8_G8_B8_A8 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 8 ビットの 4 要素 32 ビット符号なし整数フォーマットです。
    AttributeFormat_8_8_8_8_Sint = ( ChannelFormat_R8_G8_B8_A8 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 8 ビットの 4 要素 32 ビット符号付き整数フォーマットです。
    AttributeFormat_8_8_8_8_UintToFloat = ( ChannelFormat_R8_G8_B8_A8 << TypeFormat_Bits ) | TypeFormat_UintToFloat, //!< 各要素が 8 ビットの 4 要素 32 ビット符号なし整数から浮動小数点への変換フォーマットです。
    AttributeFormat_8_8_8_8_SintToFloat = ( ChannelFormat_R8_G8_B8_A8 << TypeFormat_Bits ) | TypeFormat_SintToFloat, //!< 各要素が 8 ビットの 4 要素 32 ビット符号付き整数から浮動小数点への変換フォーマットです。
    AttributeFormat_10_10_10_2_Unorm = ( ChannelFormat_R10_G10_B10_A2 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 最初の 3 要素が 10 ビット、最後の 1 要素が 2 ビットの 4 要素 32 ビット符号なし正規化フォーマットです。
    AttributeFormat_10_10_10_2_Snorm = ( ChannelFormat_R10_G10_B10_A2 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 最初の 3 要素が 10 ビット、最後の 1 要素が 2 ビットの 4 要素 32 ビット符号付き正規化フォーマットです。
    AttributeFormat_10_10_10_2_Uint = ( ChannelFormat_R10_G10_B10_A2 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 最初の 3 要素が 10 ビット、最後の 1 要素が 2 ビットの 4 要素 32 ビット符号なし整数フォーマットです。
    AttributeFormat_10_10_10_2_Sint = ( ChannelFormat_R10_G10_B10_A2 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 最初の 3 要素が 10 ビット、最後の 1 要素が 2 ビットの 4 要素 32 ビット符号付き整数フォーマットです。
    AttributeFormat_16_16_Unorm = ( ChannelFormat_R16_G16 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 各要素が 16 ビットの 2 要素 32 ビット符号なし正規化フォーマットです。
    AttributeFormat_16_16_Snorm = ( ChannelFormat_R16_G16 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 各要素が 16 ビットの 2 要素 32 ビット符号付き正規化フォーマットです。
    AttributeFormat_16_16_Uint = ( ChannelFormat_R16_G16 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 16 ビットの 2 要素 32 ビット符号なし整数フォーマットです。
    AttributeFormat_16_16_Sint = ( ChannelFormat_R16_G16 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 16 ビットの 2 要素 32 ビット符号付き整数フォーマットです。
    AttributeFormat_16_16_Float = ( ChannelFormat_R16_G16 << TypeFormat_Bits ) | TypeFormat_Float, //!< 各要素が 16 ビットの 2 要素 32 ビット浮動小数点フォーマットです。
    AttributeFormat_16_16_UintToFloat = ( ChannelFormat_R16_G16 << TypeFormat_Bits ) | TypeFormat_UintToFloat, //!< 各要素が 16 ビットの 2 要素 32 ビット符号なし整数から浮動小数点への変換フォーマットです。
    AttributeFormat_16_16_SintToFloat = ( ChannelFormat_R16_G16 << TypeFormat_Bits ) | TypeFormat_SintToFloat, //!< 各要素が 16 ビットの 2 要素 32 ビット符号付き整数から浮動小数点への変換フォーマットです。
    AttributeFormat_32_Uint = ( ChannelFormat_R32 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 32 ビットの単一要素符号なし整数フォーマットです。
    AttributeFormat_32_Sint = ( ChannelFormat_R32 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 32 ビットの単一要素符号付き整数フォーマットです。
    AttributeFormat_32_Float = ( ChannelFormat_R32 << TypeFormat_Bits ) | TypeFormat_Float, //!< 32 ビットの単一要素浮動小数点フォーマットです。
    AttributeFormat_16_16_16_16_Unorm = ( ChannelFormat_R16_G16_B16_A16 << TypeFormat_Bits ) | TypeFormat_Unorm, //!< 各要素が 16 ビットの 4 要素 64 ビット符号なし正規化フォーマットです。
    AttributeFormat_16_16_16_16_Snorm = ( ChannelFormat_R16_G16_B16_A16 << TypeFormat_Bits ) | TypeFormat_Snorm, //!< 各要素が 16 ビットの 4 要素 64 ビット符号付き正規化フォーマットです。
    AttributeFormat_16_16_16_16_Uint = ( ChannelFormat_R16_G16_B16_A16 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 16 ビットの 4 要素 64 ビット符号なし整数フォーマットです。
    AttributeFormat_16_16_16_16_Sint = ( ChannelFormat_R16_G16_B16_A16 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 16 ビットの 4 要素 64 ビット符号付き整数フォーマットです。
    AttributeFormat_16_16_16_16_Float = ( ChannelFormat_R16_G16_B16_A16 << TypeFormat_Bits ) | TypeFormat_Float, //!< 各要素が 16 ビットの 4 要素 64 ビット浮動小数点フォーマットです。
    AttributeFormat_16_16_16_16_UintToFloat = ( ChannelFormat_R16_G16_B16_A16 << TypeFormat_Bits ) | TypeFormat_UintToFloat, //!< 各要素が 16 ビットの 4 要素 64 ビット符号なし整数から浮動小数点への変換フォーマットです。
    AttributeFormat_16_16_16_16_SintToFloat = ( ChannelFormat_R16_G16_B16_A16 << TypeFormat_Bits ) | TypeFormat_SintToFloat, //!< 各要素が 16 ビットの 4 要素 64 ビット符号付き整数から浮動小数点への変換フォーマットです。
    AttributeFormat_32_32_Uint = ( ChannelFormat_R32_G32 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 32 ビットの 2 要素 64 ビット符号なし整数フォーマットです。
    AttributeFormat_32_32_Sint = ( ChannelFormat_R32_G32 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 32 ビットの 2 要素 64 ビット符号付き整数フォーマットです。
    AttributeFormat_32_32_Float = ( ChannelFormat_R32_G32 << TypeFormat_Bits ) | TypeFormat_Float, //!< 各要素が 32 ビットの 2 要素 64 ビット浮動小数点フォーマットです。
    AttributeFormat_32_32_32_Uint = ( ChannelFormat_R32_G32_B32 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 32 ビットの 3 要素 96 ビット符号なし整数フォーマットです。
    AttributeFormat_32_32_32_Sint = ( ChannelFormat_R32_G32_B32 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 32 ビットの 3 要素 96 ビット符号付き整数フォーマットです。
    AttributeFormat_32_32_32_Float = ( ChannelFormat_R32_G32_B32 << TypeFormat_Bits ) | TypeFormat_Float, //!< 各要素が 32 ビットの 3 要素 96 ビット浮動小数点フォーマットです。
    AttributeFormat_32_32_32_32_Uint = ( ChannelFormat_R32_G32_B32_A32 << TypeFormat_Bits ) | TypeFormat_Uint, //!< 各要素が 32 ビットの 4 要素 128 ビット符号なし整数フォーマットです。
    AttributeFormat_32_32_32_32_Sint = ( ChannelFormat_R32_G32_B32_A32 << TypeFormat_Bits ) | TypeFormat_Sint, //!< 各要素が 32 ビットの 4 要素 128 ビット符号付き整数フォーマットです。
    AttributeFormat_32_32_32_32_Float = ( ChannelFormat_R32_G32_B32_A32 << TypeFormat_Bits ) | TypeFormat_Float //!< 各要素が 32 ビットの 4 要素 128 ビット浮動小数点フォーマットです。
};

/**
* @brief GPU がアクセスする方法です。
*/
enum GpuAccess
{
    GpuAccess_Read = 0x01, //!< 読み込みアクセスします。
    GpuAccess_Write = 0x02, //!< 書き込みアクセスします。
    GpuAccess_VertexBuffer = 0x04, //!< 頂点バッファーとしてアクセスします。
    GpuAccess_IndexBuffer = 0x08, //!< インデックスバッファーとしてアクセスします。
    GpuAccess_ConstantBuffer = 0x10, //!< 定数バッファーとしてアクセスします。
    GpuAccess_Texture = 0x20, //!< テクスチャーとしてアクセスします。
    GpuAccess_UnorderedAccessBuffer = 0x40, //!< アンオーダードアクセスバッファーとしてアクセスします。
    GpuAccess_ColorBuffer = 0x80, //!< カラーバッファーとしてアクセスします。
    GpuAccess_DepthStencil = 0x0100, //!< 深度ステンシルバッファーとしてアクセスします。
    GpuAccess_IndirectBuffer = 0x0200, //!< 間接描画または間接ディスパッチのパラメーターバッファーとしてアクセスします。
    GpuAccess_ScanBuffer = 0x0400, //!< スキャンバッファーとしてアクセスします。
    GpuAccess_QueryBuffer = 0x0800, //!< クエリーの書き込み先としてアクセスします。
    GpuAccess_Descriptor = 0x1000, //!< デスクリプターとしてアクセスします。
    GpuAccess_ShaderCode = 0x2000, //!< シェーダーコードとしてアクセスします。
    GpuAccess_Image = 0x4000 //!< イメージとしてアクセスします。
};

/**
* @brief CPU がアクセスする方法です。
*/
enum CpuAccess
{
    CpuAccess_Read = 0x01, //!< 読み込みアクセスします。
    CpuAccess_Write = 0x02 //!< 書き込みアクセスします。
};

/**
* @brief メモリー重要度です。
*/
enum MemoryPriority
{
    MemoryPriority_Low, //!< 低重要度です。
    MemoryPirority_Middle, //!< 中重要度です。
    MemoryPriority_High, //!< 高重要度です。

    MemoryPriority_End
};

/**
* @brief タイルモードです。
*/
enum TileMode
{
    TileMode_Optimal, //!< GPU に最適なタイリングです。
    TileMode_Linear, //!< 線形タイリングです。

    TileMode_End
};

/**
* @brief シェーダーステージです。
*/
enum ShaderStage
{
    ShaderStage_Vertex, //!< 頂点シェーダーです。
    ShaderStage_Hull, //!< ハルシェーダーです。
    ShaderStage_Domain, //!< ドメインシェーダーです。
    ShaderStage_Geometry, //!< ジオメトリーシェーダーです。
    ShaderStage_Pixel, //!< ピクセルシェーダーです。
    ShaderStage_Compute, //!< 演算シェーダーです。

    ShaderStage_End
};

/**
* @brief シェーダーコードのタイプです。
*/
enum ShaderCodeType
{
    ShaderCodeType_Binary, //!< コンパイル済みバイナリーです。
    ShaderCodeType_Ir, //!< 中間言語表現です。
    ShaderCodeType_Source, //!< コンパイル前ソースです。
    ShaderCodeType_SourceArray, //!< コンパイル前ソースの配列です。

    ShaderCodeType_End
};

/**
* @brief シェーダーソースのフォーマットです。
*/
enum ShaderSourceFormat
{
    ShaderSourceFormat_Glsl, //!< GLSL シェーダー言語です。
    ShaderSourceFormat_Hlsl, //!< HLSL シェーダー言語です。

    ShaderSourceFormat_End
};

/**
* @brief チャンネルマッピングです。
*/
enum ChannelMapping
{
    ChannelMapping_Zero, //!< 0 をマッピングします。
    ChannelMapping_One, //!< 1 をマッピングします。
    ChannelMapping_Red, //!< 赤要素をマッピングします。
    ChannelMapping_Green, //!< 緑要素をマッピングします。
    ChannelMapping_Blue, //!< 青要素をマッピングします。
    ChannelMapping_Alpha, //!< アルファ要素をマッピングします。

    ChannelMapping_End
};

/**
* @brief 深度ステンシルテクスチャーのフェッチモードです。
*/
enum DepthStencilFetchMode
{
    DepthStencilFetchMode_DepthComponent, //!< 深度要素をフェッチします。
    DepthStencilFetchMode_StencilIndex, //!< ステンシル要素をフェッチします。

    DepthStencilFetchMode_End
};

/**
* @brief 深度ステンシルテクスチャーのクリアモードです。
*/
enum DepthStencilClearMode
{
    DepthStencilClearMode_Depth = 0x01, //!< 深度要素をクリアします。
    DepthStencilClearMode_Stencil = 0x02, //!< ステンシル要素をクリアします。
    DepthStencilClearMode_DepthStencil = //!< 深度要素とステンシル要素をクリアします。
        DepthStencilClearMode_Depth | DepthStencilClearMode_Stencil
};

/**
* @brief 頂点属性入力モードです。
*/
enum AttributeInput
{
    AttributeInput_PerVertex, //!< 頂点属性の入力は頂点単位です。
    AttributeInput_PerInstance, //!< 頂点属性の入力はインスタンス単位です。

    AttributeInput_End
};

/**
* @brief シェーダーインターフェイスのタイプです。
*/
enum ShaderInterfaceType
{
    ShaderInterfaceType_Input, //!< 入力インターフェイスです。
    ShaderInterfaceType_Output, //!< 出力インターフェイスです。
    ShaderInterfaceType_Sampler, //!< サンプラーインターフェイスです。
    ShaderInterfaceType_ConstantBuffer, //!< 定数バッファーインターフェイスです。
    ShaderInterfaceType_UnorderedAccessBuffer, //!< アンオーダードアクセスバッファーインターフェイスです。
    ShaderInterfaceType_Image, //!< イメージインターフェイスです。
    ShaderInterfaceType_SeparateTexture, //!< サンプラーを含まないテクスチャーインターフェイスです。
    ShaderInterfaceType_SeparateSampler, //!< テクスチャーを含まないサンプラーインターフェイスです。

    ShaderInterfaceType_End
};

/**
* @brief キューの機能性です。
*/
enum QueueCapability
{
    QueueCapability_Graphics = 0x01, //!< グラフィックス機能をもちます。
    QueueCapability_Compute = 0x02, //!< 演算機能をもちます。
    QueueCapability_Copy = 0x04 //!< コピー機能をもちます。
};

/**
* @brief カラー要素です。
*/
enum ColorChannel
{
    ColorChannel_Red, //!< 赤要素です。
    ColorChannel_Green, //!< 緑要素です。
    ColorChannel_Blue, //!< 青要素です。
    ColorChannel_Alpha, //!< アルファ要素です。

    ColorChannel_End
};

/**
* @brief デスクリプタープールの種類です。
*/
enum DescriptorPoolType
{
    DescriptorPoolType_BufferView, //!< バッファービューです
    DescriptorPoolType_TextureView, //!< テクスチャービューです
    DescriptorPoolType_Sampler, //!< サンプラーです

    DescriptorPoolType_End
};

/**
* @brief デスクリプタースロットの種類です。
*/
enum DescriptorSlotType
{
    DescriptorSlotType_ConstantBuffer, //!< 定数バッファーです
    DescriptorSlotType_UnorderedAccessBuffer, //!< アンオーダードアクセスバッファーです
    DescriptorSlotType_TextureSampler, //!< テクスチャーサンプラーです

    DescriptorSlotType_End
};

/**
* @brief パイプラインの種類です。
*/
enum PipelineType
{
    PipelineType_Graphics, //!< グラフィックスパイプラインです
    PipelineType_Compute, //!< 演算パイプラインです

    PipelineType_End
};

/**
* @brief メモリープールの特性です。
*/
enum MemoryPoolProperty
{
    MemoryPoolProperty_CpuInvisible = 0x01, //!< CPU から不可視です
    MemoryPoolProperty_CpuUncached = 0x02, //!< CPU キャッシュされません
    MemoryPoolProperty_CpuCached = 0x04, //!< CPU キャッシュされます
    MemoryPoolProperty_GpuInvisible = 0x08, //!< GPU から不可視です
    MemoryPoolProperty_GpuUncached = 0x10, //!< GPU キャッシュされません
    MemoryPoolProperty_GpuCached = 0x20, //!< GPU キャッシュされます
    MemoryPoolProperty_ShaderCode = 0x40, //!< シェーダーコードを含みます
    MemoryPoolProperty_Compressible = 0x80 //!< 圧縮可能リソースをサポートします
};

/**
* @brief コマンドバッファーの種類です。
*/
enum CommandBufferType
{
    CommandBufferType_Direct, //!< キューに直接提出されるコマンドバッファーです
    CommandBufferType_Nested, //!< コマンドバッファーからネストして呼び出されるコマンドバッファーです

    CommandBufferType_End
};

/**
* @brief バッファーステートです。
*/
enum BufferState
{
    BufferState_Undefined = 0x00, //!< 未定義のステートです
    BufferState_DataTransfer = 0x01, //!< データ転送ステートです
    BufferState_CopySource = 0x02, //!< コピー元ステートです
    BufferState_CopyDestination = 0x04, //!< コピー先ステートです
    BufferState_VertexBuffer = 0x08, //!< 頂点バッファーステートです
    BufferState_IndexBuffer = 0x10, //!< インデックスバッファーステートです
    BufferState_ConstantBuffer = 0x20, //!< 定数バッファーステートです
    BufferState_UnorderedAccessBuffer = 0x40, //!< アンオーダードアクセスバッファーステートです
    BufferState_IndirectArgument = 0x80, //!< 間接バッファーステートです
    BufferState_QueryBuffer = 0x0100 //!< クエリーバッファーステートです
};

/**
* @brief テクスチャーステートです。
*/
enum TextureState
{
    TextureState_Undefined = 0x00, //!< 未定義のステートです
    TextureState_DataTransfer = 0x01, //!< データ転送ステートです
    TextureState_CopySource = 0x02, //!< コピー元ステートです
    TextureState_CopyDestination = 0x04, //!< コピー先ステートです
    TextureState_ShaderRead = 0x08, //!< グラフィックスシェーダーの読み込みステートです
    TextureState_ShaderWrite = 0x10, //!< グラフィックスシェーダーの書き込みステートです
    TextureState_ColorTarget = 0x20, //!< カラーターゲットステートです
    TextureState_DepthRead = 0x40, //!< 深度読み込みステートです
    TextureState_DepthWrite = 0x80, //!< 深度書き込みステートです
    TextureState_Clear = 0x0100, //!< クリアステートです
    TextureState_ResolveSource = 0x0200, //!< マルチサンプリングの解決元ステートです
    TextureState_ResolveDestination = 0x0400, //!< マルチサンプリングの解決先ステートです
    TextureState_Present = 0x800 //!< 表示ステートです
};

/**
* @brief シェーダーステージビットです。
*/
enum ShaderStageBit
{
    ShaderStageBit_Vertex = 0x01, //!< 頂点シェーダーステージです
    ShaderStageBit_Hull = 0x02, //!< ハルシェーダーステージです
    ShaderStageBit_Domain = 0x04, //!< ドメインシェーダーステージです
    ShaderStageBit_Geometry = 0x08, //!< ジオメトリシェーダーステージです
    ShaderStageBit_Pixel = 0x10, //!< ピクセルシェーダーステージです
    ShaderStageBit_Compute = 0x20, //!< 演算シェーダーステージです
    ShaderStageBit_All = ShaderStageBit_Vertex | ShaderStageBit_Hull | ShaderStageBit_Domain
        | ShaderStageBit_Geometry | ShaderStageBit_Pixel | ShaderStageBit_Compute //!< 全シェーダーステージです
};

/**
* @brief パイプラインステージビットです。
*/
enum PipelineStageBit
{
    PipelineStageBit_VertexInput = 0x01, //!< 頂点入力ステージです
    PipelineStageBit_VertexShader = 0x02, //!< 頂点シェーダーステージです
    PipelineStageBit_HullShader = 0x04, //!< ハルシェーダーステージです
    PipelineStageBit_DomainShader = 0x08, //!< ドメインシェーダーステージです
    PipelineStageBit_GeometryShader = 0x10, //!< ジオメトリーシェーダーステージです
    PipelineStageBit_PixelShader = 0x20, //!< ピクセルシェーダーステージです
    PipelineStageBit_RenderTarget = 0x40, //!< レンダーターゲットへの書き込みのステージです
    PipelineStageBit_ComputeShader = 0x80 //!< 演算シェーダーステージです
};

/**
* @brief デバッグモードです。
*/
enum DebugMode
{
    DebugMode_Disable, //!< 無効です
    DebugMode_Enable, //!< 有効です
    DebugMode_Full, //!< 全デバッグ機能が有効です

    DebugMode_End
};

/**
* @brief クエリーターゲットです。
*/
enum QueryTarget
{
    QueryTarget_Timestamp, //!< タイムスタンプです
    QueryTarget_SamplesPassed, //!< フラグメントのテストを通過するサンプルの数です
    QueryTarget_InputVertices, //!< 入力頂点の数です
    QueryTarget_InputPrimitives, //!< 入力プリミティブの数です
    QueryTarget_VertexShaderInvocations, //!< 頂点シェーダー呼び出しの数です
    QueryTarget_GeometryShaderInvocations, //!< ジオメトリーシェーダー呼び出しの数です
    QueryTarget_GeometryShaderPrimitives, //!< ジオメトリーシェーダーによって生成されるプリミティブの数です
    QueryTarget_ClippingInputPrimitives, //!< クリッピングステージによって処理されるプリミティブの数です
    QueryTarget_ClippingOutputPrimitives, //!< クリッピングステージによって生成されるプリミティブの数です
    QueryTarget_PixelShaderInvocations, //!< ピクセルシェーダー呼び出しの数です
    QueryTarget_HullShaderInvocations, //!< ハルシェーダー呼び出しの数です
    QueryTarget_DomainShaderInvocations, //!< ドメインシェーダー呼び出しの数です
    QueryTarget_ComputeShaderInvocations, //!< 演算シェーダー呼び出しの数です

    QueryTarget_End
};

/**
* @brief イメージコピーに用いるフラグです。
*/
enum ImageCopyFlag
{
    ImageCopyFlag_LinearFilter = 0x01 //!< コピー時の伸縮処理に線形フィルタリングを使います
};

/**
* @brief シェーダーの初期化の結果です。
*/
enum ShaderInitializeResult
{
    ShaderInitializeResult_Success = 0, //!< シェーダーの初期化に成功しました。
    ShaderInitializeResult_InvalidType, //!< シェーダーコードのタイプが無効です。
    ShaderInitializeResult_InvalidFormat, //!< シェーダーコードのフォーマットが無効です。
    ShaderInitializeResult_SetupFailed //!< シェーダーのセットアップに失敗しました。
};

/**
* @brief 同期処理の結果です。
*/
enum SyncResult
{
    SyncResult_Success = 0, //!< 同期に成功しました。
    SyncResult_TimeoutExpired //!< 処理がタイムアウトしました。
};

/**
* @brief スキャンバッファーの取得処理の結果です。
*/
enum AcquireScanBufferResult
{
    AcquireScanBufferResult_Success = 0, //!< スキャンバッファーの取得に成功しました。
    AcquireScanBufferResult_Failed //!< スキャンバッファーの取得に失敗しました
};

/**
* @brief イメージフォーマットプロパティーのフラグです。
*/
enum ImageFormatPropertyFlag
{
    ImageFormatPropertyFlag_Texture = 0x01, //!< テクスチャーとして利用可能です
    ImageFormatPropertyFlag_ColorTarget = 0x02, //!< カラーターゲットとして利用可能です
    ImageFormatPropertyFlag_Image = 0x04, //!< イメージとして利用可能です
};

}
}
