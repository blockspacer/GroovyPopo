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
* @brief ステート初期化用の情報のデータに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

#include <nn/util/util_BitPack.h>

#include <nn/gfx/gfx_Enum.h>

#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief マルチサンプルステートを初期化するための情報を表す構造体です。
*/
struct MultisampleStateInfoData
{
    enum Flag
    {
        Flag_AlphaToCoverageEnable,
    };
    uint8_t sampleCount; //!< サンプル数です
    char reserved2; //!< 予約領域です。
    nn::util::BitPack16 flag; //!< フラグです
    uint32_t sampleMask; //!< サンプルマスクです

    char reserved[ 8 ]; //!< 予約領域です

    // SampleCoverage
};

/**
* @brief ラスタライザーステートを初期化するための情報を表す構造体です。
*/
struct RasterizerStateInfoData
{
    enum Flag
    {
        Flag_RasterDisable,
        Flag_MultisampleEnable,
        Flag_DepthClipDisable,
        Flag_ScissorEnable
    };

    Bit8 fillMode; //!< nn::gfx::FillMode で表される、塗りつぶしモードです
    Bit8 frontFace; //!< nn::gfx::FrontFace で表される、前面とする面です
    Bit8 cullMode; //!< nn::gfx::CullMode で表される、カリングモードです
    Bit8 primitiveTopologyType; //!< nn::gfx::PrimitiveTopologyType で表される、プリミティブトポロジータイプです
    nn::util::BitPack16 flag; //!< フラグです
    Bit8 conservativeRasterizationMode; //!< nn::gfx::ConservativeRasterizationMode で表される、保守的なラスタライゼーションモードです
    char reserved2; //!< 予約領域です。
    float slopeScaledDepthBias; //!< 最大深度スロープでスケールされる、ピクセルの深度値に加算する値です
    int32_t depthBias; //!< ピクセルの深度値に加算される値です
    float depthBiasClamp; //!< ピクセルの深度値に加算される最大値です
    char reserved3[ 4 ]; //!< 予約領域です。

    MultisampleStateInfoData multisample; //!< マルチサンプルステートです

    char reserved[ 24 ]; //!< 予約領域です

    // PointSize
    // LineWidth
    // VertexReuse
};

/**
* @brief ブレンドターゲットステートを初期化するための情報を表す構造体です。
*/
struct BlendTargetStateInfoData
{
    enum Flag
    {
        Flag_BlendEnable
    };

    nn::util::BitPack8 flag; //!< フラグです
    Bit8 sourceColorBlendFactor; //!< nn::gfx::BlendFactor で表される、ソースのカラー要素のブレンドファクターです
    Bit8 destinationColorBlendFactor; //!< nn::gfx::BlendFactor で表される、デスティネーションのカラー要素のブレンドファクターです
    Bit8 colorBlendFunction; //!< nn::gfx::BlendFunction で表される、カラー要素のブレンド関数です
    Bit8 sourceAlphaBlendFactor; //!< nn::gfx::BlendFactor で表される、ソースのアルファ要素のブレンドファクターです
    Bit8 destinationAlphaBlendFactor; //!< nn::gfx::BlendFactor で表される、デスティネーションのアルファ要素のブレンドファクターです
    Bit8 alphaBlendFunction; //!< nn::gfx::BlendFunction で表される、アルファ要素のブレンド関数です
    Bit8 channelMask; //!< nn::gfx::ChannelMask で表される、チャンネルマスクです

    char reserved[ 24 ]; //!< 予約領域です
};

/**
* @brief ブレンドステートを初期化するための情報を表す構造体です。
*/
struct BlendStateInfoData
{
    enum Flag
    {
        Flag_AlphaToCoverageEnable,
        Flag_DualSourceBlendEnable,
        Flag_IndependentBlendEnable,
        Flag_LogicOperationEnable,
    };

    uint8_t blendTargetCount; //!< ブレンドターゲットステートの数です
    Bit8 logicOperation; //!< nn::gfx::LogicOperation で表される、論理オペレーションです
    nn::util::BitPack16 flag; //!< フラグです
    float blendConstant[ 4 ]; //!< ブレンド定数です
    char reserved2[ 4 ]; //!< 予約領域です。
    detail::Ptr< const BlendTargetStateInfoData > pBlendTargetArray; //!< ブレンドターゲットステートの配列へのポインターです

    char reserved[ 16 ]; //!< 予約領域です
};

/**
* @brief ステンシルステートを初期化するための情報を表す構造体です。
*/
struct StencilStateInfoData
{
    Bit8 stencilFailOperation; //!< nn::gfx::StencilOperation で表される、ステンシルテスト失敗時のオペレーションです
    Bit8 depthFailOperation; //!< nn::gfx::StencilOperation で表される、深度テスト失敗時のオペレーションです
    Bit8 depthPassOperation; //!< nn::gfx::StencilOperation で表される、深度テスト成功時のオペレーションです
    Bit8 comparisonFunction; //!< nn::gfx::ComparisonFunction で表される、ステンシルテストの比較関数です
    uint8_t stencilRef; //!< ステンシル参照値です

    char reserved[ 11 ]; //!< 予約領域です
};

/**
* @brief 深度ステンシルステートを初期化するための情報を表す構造体です。
*/
struct DepthStencilStateInfoData
{
    enum Flag
    {
        Flag_DepthTestEnable,
        Flag_DepthWriteEnable,
        Flag_StencilTestEnable,
        Flag_DepthBoundsTestEnable
    };

    Bit8 depthComparisonFunction; //!< nn::gfx::ComparisonFunction で表される、深度テストの比較関数です
    uint8_t stencilReadMask; //!< ステンシル読み込みに適用されるマスクです
    uint8_t stencilWriteMask; //!< ステンシル書き込みに適用されるマスクです
    char reserved2; //!< 予約領域です。
    nn::util::BitPack16 flag; //!< フラグです
    char reserved3[ 2 ]; //!< 予約領域です。
    StencilStateInfoData frontStencil; //!< 前面のステンシルステートです
    StencilStateInfoData backStencil; //!< 背面のステンシルステートです

    char reserved[ 24 ]; //!< 予約領域です
};

/**
* @brief カラーターゲットステートを初期化するための情報を表す構造体です。
*/
struct ColorTargetStateInfoData
{
    Bit32 format; //!< nn::gfx::ImageFormat で表される、カラーフォーマットです

    char reserved[ 28 ];
};

/**
* @brief レンダーターゲットステートを初期化するための情報を表す構造体です。
*/
struct RenderTargetStateInfoData
{
    uint8_t colorTargetCount; //!< カラーターゲットの数です
    char reserved2[ 3 ]; //!< 予約領域です。
    Bit32 depthStencilFormat; //!< nn::gfx::ImageFormat で表される、深度ステンシルフォーマットです
    detail::Ptr< const ColorTargetStateInfoData > pColorTargetStateArray; //!< カラーターゲットステートの配列へのポインターです

    char reserved[ 16 ];
};

/**
* @brief 頂点属性ステートを初期化するための情報を表す構造体です。
*/
struct VertexAttributeStateInfoData
{
    uint8_t semanticIndex; //!< セマンティックインデックスです
    char reserved2; //!< 予約領域です。
    int16_t shaderSlot; //!< シェーダーにおけるスロット番号です
    uint16_t bufferIndex; //!< 頂点バッファーインデックスです
    char reserved3[ 2 ]; //!< 予約領域です。
    uint32_t offset; //!< バイトでのバッファーにおけるオフセットです
    Bit32 format; //!< nn::gfx::AttributeFormat で表される、頂点属性フォーマットです
    detail::Ptr< const char > pName; //!< 名前へのポインターです

    char reserved[ 16 ]; //!< 予約領域です
};

/**
* @brief 頂点バッファーステートを初期化するための情報を表す構造体です。
*/
struct VertexBufferStateInfoData
{
    int32_t stride; //!< ストライドです
    uint32_t divisor; //!< インスタンスの除数です

    char reserved[ 8 ]; //!< 予約領域です
};

/**
* @brief 頂点ステートを初期化するための情報を表す構造体です。
*/
struct VertexStateInfoData
{
    uint16_t attributeCount; //!< 頂点属性の数です
    uint16_t bufferCount; //!< 頂点バッファーの数です
    char reserved2[ 4 ]; //!< 予約領域です。
    detail::Ptr< const VertexAttributeStateInfoData > pAttributeArray; //!< 頂点属性ステートの配列へのポインターです
    detail::Ptr< const VertexBufferStateInfoData > pBufferArray; //!< 頂点バッファーステートの配列へのポインターです

    char reserved[ 24 ]; //!< 予約領域です
};

/**
* @brief テッセレーションステートを初期化するための情報を表す構造体です。
*/
struct TessellationStateInfoData
{
    uint16_t patchControlPointCount; //!< パッチの制御点の数です

    char reserved[ 30 ];
};

/**
* @brief 深度範囲を初期化するための情報を表す構造体です。
*/
struct DepthRangeInfoData
{
    float minDepth; //!< 最小深度です
    float maxDepth; //!< 最大深度です

    char reserved[ 8 ]; //!< 予約領域です
};

/**
* @brief ビューポートステートを初期化するための情報を表す構造体です。
*/
struct ViewportStateInfoData
{
    float originX; //!< X 座標の原点です
    float originY; //!< Y 座標の原点です
    float width; //!< 幅です
    float height; //!< 高さです

    DepthRangeInfoData depthRange; //!< 深度範囲です

    char reserved[ 16 ];
};

/**
* @brief シザーステートを初期化するための情報を表す構造体です。
*/
struct ScissorStateInfoData
{
    uint32_t originX; //!< X 座標の原点です
    uint32_t originY; //!< Y 座標の原点です
    uint32_t width; //!< 幅です
    uint32_t height; //!< 高さです

    char reserved[ 16 ]; //!< 予約領域です
};

/**
* @brief ビューポートシザーステートを初期化するための情報を表す構造体です。
*/
struct ViewportScissorStateInfoData
{
    enum Flag
    {
        Flag_ScissorEnable
    };

    nn::util::BitPack8 flag; //!< フラグです
    char reserved2; //!< 予約領域です。
    uint16_t viewportCount; //!< ビューポートの数です
    uint16_t scissorCount; //!< シザーの数です
    char reserved3[ 2 ]; //!< 予約領域です。
    detail::Ptr< const ViewportStateInfoData > pViewportArray; //!< ビューポートステート配列へのポインターです
    detail::Ptr< const ScissorStateInfoData > pScissorArray; //!< シザーステート配列へのポインターです

    char reserved[ 8 ]; //!< 予約領域です
};

}
}
