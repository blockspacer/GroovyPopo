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
* @brief テクスチャー初期化用の情報のデータに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

#include <nn/util/util_BitPack.h>

#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief テクスチャーのミップマップ範囲を表す構造体です。
*/
struct TextureMipRangeData
{
    uint16_t minMipLevel; //!< 最小のミップマップレベルです
    uint16_t mipCount; //!< ミップマップの数です

    char reserved[ 12 ]; //!< 予約領域です
};

/**
* @brief テクスチャーの配列範囲を表す構造体です。
*/
struct TextureArrayRangeData
{
    uint32_t baseArrayIndex; //!< 配列インデックスのベース値です
    uint32_t arrayLength; //!< 配列の長さです

    char reserved[ 8 ]; //!< 予約領域です
};

/**
* @brief テクスチャーにおけるイメージ範囲を表す構造体です。
*/
struct TextureSubresourceRangeData
{
    TextureMipRangeData mipRange; //!< ミップマップ範囲です
    TextureArrayRangeData arrayRange; //!< 配列範囲です
};

/**
* @brief テクスチャーを初期化するための情報を表す構造体です。
*/
struct TextureInfoData
{
    enum Flag
    {
        Flag_SpecifyTextureLayout,
        Flag_SparseBinding,
        Flag_SparseResidency
    };

    nn::util::BitPack8 flags; //!< フラグです
    Bit8 imageStorageDimension; //!< nn::gfx::ImageStorageDimension で表される、イメージデータの次元です
    Bit16 tileMode; //!< nn::gfx::TileMode で表される、タイルモードです
    uint16_t swizzle; //!< スウィズリングの初期値です
    uint16_t mipCount; //!< ミップマップの数です
    uint16_t multisampleCount; //!< マルチサンプルの数です
    char reserved2[ 2 ]; //!< 予約領域です。
    Bit32 imageFormat; //!< nn::gfx::ImageFormat で表される、イメージフォーマットです
    Bit32 gpuAccessFlags; //!< nn::gfx::GpuAccess の組み合わせで表される、GPU がアクセスする方法です
    uint32_t width; //!< 幅です
    uint32_t height; //!< 高さです
    uint32_t depth; //!< 深さです
    uint32_t arrayLength; //!< 配列の長さです
    char textureLayout[ 8 ]; //!< テクスチャーレイアウトです

    char reserved[ 20 ]; //!< 予約領域です
};

/**
* @brief テクスチャービューを初期化するための情報を表す構造体です。
*/
struct TextureViewInfoData
{
    Bit8 imageDimension; //!< nn::gfx::ImageDimension で表される、イメージの次元です
    Bit8 depthStencilTextureMode; //!< nn::gfx::DepthStencilFetchMode で表される、テクスチャーフェッチモードです
    char reserved2[ 2 ]; //!< 予約領域です。
    Bit32 imageFormat; //!< nn::gfx::ImageFormat で表される、イメージフォーマットです
    Bit8 channelMapping[ 4 ]; //!< nn::gfx::ChannelMapping で表される、R、G、B、A の順での各要素のマッピングです
    char reserved3[ 4 ]; //!< 予約領域です。
    TextureSubresourceRangeData subresourceRange; //!< 参照するイメージ範囲です
    detail::Ptr< const void > pTexture; //!< 参照するテクスチャーです

    char reserved[ 8 ]; //!< 予約領域です
};

/**
* @brief カラーターゲットビューを初期化するための情報を表す構造体です。
*/
struct ColorTargetViewInfoData
{
    Bit8 imageDimension; //!< nn::gfx::ImageDimension で表される、イメージの次元です
    char reserved2; //!< 予約領域です。
    uint16_t mipLevel; //!< 参照するミップマップレベルです
    Bit32 imageFormat; //!< nn::gfx::ImageFormat で表される、イメージフォーマットです
    TextureArrayRangeData arrayRange; //!< 参照する配列範囲です
    detail::Ptr< const void > pTexture; //!< 参照するテクスチャーです

    char reserved[ 16 ];
};

/**
* @brief 深度ステンシルビューを初期化するための情報を表す構造体です。
*/
struct DepthStencilViewInfoData
{
    Bit8 imageDimension; //!< nn::gfx::ImageDimension で表される、イメージの次元です
    char reserved2; //!< 予約領域です。
    uint16_t mipLevel; //!< 参照するミップマップレベルです
    char reserved3[ 4 ]; //!< 予約領域です。
    TextureArrayRangeData arrayRange; //!< 参照する配列範囲です
    detail::Ptr< const void > pTexture; //!< 参照するテクスチャーです

    char reserved[ 16 ]; //!< 予約領域です
    // format
};

/**
* @brief テクスチャーにおける特定のイメージを表す構造体です。
*/
struct TextureSubresourceData
{
    uint16_t mipLevel; //!< ミップマップレベルです
    char reserved[ 2 ]; //!< 予約領域です。
    uint32_t arrayIndex; //!< 配列インデックスです
};

/**
* @brief テクスチャーコピーオペレーションの範囲を表す構造体です。
*/
struct TextureCopyRegionData
{
    uint32_t offsetU; //!< U 座標のオフセットです
    uint32_t offsetV; //!< V 座標のオフセットです
    uint32_t offsetW; //!< W 座標のオフセットです
    uint32_t width; //!< 幅です
    uint32_t height; //!< 高さです
    uint32_t depth; //!< 深さです
    TextureSubresourceData subresource; //!< イメージを特定するパラメーターです
    uint32_t arrayLength; //!< 配列長です
};

/**
* @brief バッファー・テクスチャー間のコピーオペレーションの範囲を表す構造体です。
*/
struct BufferTextureCopyRegionData
{
    uint32_t bufferOffset; //!< バイトでのバッファーのオフセットです。
    uint32_t bufferImageWidth; //!< バッファーにおけるイメージの幅です。
    uint32_t bufferImageHeight; //!< バッファーにおけるイメージの高さです。
    TextureCopyRegionData textureCopyRegion; //!< テクスチャーのコピー領域です。
};

/**
* @brief イメージフォーマットのプロパティーを表す構造体です。
*/
struct ImageFormatProperty
{
    Bit32 propertyFlags; //!< nn::gfx::ImageFormatPropertyFlag の組み合わせによって表される、プロパティーフラグです
};

}
}
