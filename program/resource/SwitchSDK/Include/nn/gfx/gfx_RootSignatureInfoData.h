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
* @brief ルートシグネチャー初期化用の情報のデータに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

#include <nn/gfx/detail/gfx_Misc.h>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS

namespace nn {
namespace gfx {

/**
* @brief デスクリプター範囲を表す構造体です。
*/
struct NN_DEPRECATED DescriptorRangeInfoData
{
    Bit8 descriptorSlotType; //!< デスクリプタースロットの種類です
    char reserved2[ 3 ]; //!< 予約領域です。
    uint32_t descriptorSlotCount; //!< デスクリプタースロットの数です
    int32_t baseShaderSlot; //!< シェーダースロットのベース値です
    char reserved3[ 4 ]; //!< 予約領域です。
    union
    {
        uint32_t bufferDescriptorSlotOffset; //!< バッファーのデスクリプターテーブルの先頭からのスロットのオフセットです
        struct
        {
            uint32_t textureDescriptorSlotOffset; //!< テクスチャーのデスクリプターテーブルの先頭からのスロットのオフセットです
            uint32_t samplerDescriptorSlotOffset; //!< サンプラーのデスクリプターテーブルの先頭からのスロットのオフセットです
        } textureSamplerDescriptorSlotOffset;
    };

    char reserved[ 8 ]; //!< 予約領域です
};

/**
* @brief デスクリプターテーブルを表す構造体です。
*/
struct NN_DEPRECATED DescriptorTableInfoData
{
    Bit8 shaderStage; //!< シェーダーステージです
    char reserved2[ 3 ]; //!< 予約領域です。
    uint32_t descriptorRangeCount; //!< デスクリプター範囲の数です

    detail::Ptr< const DescriptorRangeInfoData > pDescriptorRangeArray; //!< デスクリプター範囲の配列へのポインターです

    char reserved[ 16 ]; //!< 予約領域です
};

/**
* @brief 動的なデスクリプターを表す構造体です。
*/
struct NN_DEPRECATED DynamicDescriptorInfoData
{
    Bit8 shaderStage; //!< シェーダーステージです
    char reserved2[ 3 ]; //!< 予約領域です。
    int32_t shaderSlot; //!< シェーダーのスロット番号です

    char reserved[ 8 ]; //!< 予約領域です
};

/**
* @brief ルートシグネチャーを表す構造体です。
*/
struct NN_DEPRECATED RootSignatureInfoData
{
    uint32_t descriptorTableCount; //!< デスクリプターテーブルの数です
    uint32_t dynamicDescriptorCount; //!< 動的なデスクリプターの数です

    detail::Ptr< const DescriptorTableInfoData > pDescriptorTableArray; //!< デスクリプターテーブルの配列へのポインターです
    detail::Ptr< const DynamicDescriptorInfoData > pDynamicDescriptorArray; //!< 動的なデスクリプターの配列へのポインターです

    char reserved[ 40 ]; //!< 予約領域です
};

}
}

NN_PRAGMA_POP_WARNINGS
