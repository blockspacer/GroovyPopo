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
* @brief ステートのデータに関する API の宣言（NVN8 特殊版）
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.nvn.h>
#include <nn/gfx/gfx_StateInfoData.h>

#include <nn/gfx/detail/gfx_Declare.h>

namespace nn {
namespace gfx {

/**
* @brief ラスタライザーステートを表す構造体です（NVN8 特殊版）
*/
template<>
struct RasterizerStateImplData< ApiVariationNvn8 >
{
    // NVN State object containers
    Bit32 nvnPolygonState;
    char nvnMultisampleState[ 24 ];

    Bit32 nvnSampleMask;
    float nvnDepthBias;
    float nvnDepthBiasClamp;
    float nvnSlopeScaledDepthBias;

    enum Flag
    {
        Flag_MultisampleEnabled,
        Flag_DepthClipEnabled,
        Flag_RasterEnabled,
        Flag_ConservativeRasterEnabled
    };

    enum State
    {
        State_NotInitialized,
        State_Initialized
    };
    Bit8 state; //!< オブジェクトの状態です。
    nn::util::BitPack8 flags;
    char reserved[ 2 ]; //!< 予約領域です。
};

/**
* @brief ブレンドステートを表す構造体です（NVN8 特殊版）
*/
template<>
struct BlendStateImplData< ApiVariationNvn8 >
{
    detail::Ptr< void > pNvnBlendStateData;

    Bit32 nvnColorState;
    Bit32 nvnChannelMaskState;

    float nvnBlendConstant[ 4 ];
    Bit32 memorySize;

    enum State
    {
        State_NotInitialized,
        State_Initialized
    };
    Bit8 state; //!< オブジェクトの状態です。
    Bit8 targetCount;
    char reserved[ 2 ]; //!< 予約領域です。
};

/**
* @brief 深度ステンシルステートを表す構造体です（NVN8 特殊版）
*/
template<>
struct DepthStencilStateImplData< ApiVariationNvn8 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    enum Flag
    {
        Flag_DepthBoundsTestEnable = DepthStencilStateInfoData::Flag_DepthBoundsTestEnable
    };

    char nvnDepthStencilState[ 8 ];
    Bit32 nvnStencilValueMask;
    Bit32 nvnStencilMask;
    Bit32 nvnStencilBackRef;
    Bit32 nvnStencilFrontRef;

    Bit8 state; //!< オブジェクトの状態です。
    char reserved; //!< 予約領域です。
    nn::util::BitPack16 flag;

};

/**
* @brief 頂点ステートを表す構造体です（NVN8 特殊版）
*/
template<>
struct VertexStateImplData< ApiVariationNvn8 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    Bit8 state; //!< オブジェクトの状態です。
    char reserved[ 3 ]; //!< 予約領域です。
    int32_t vertexStreamStateCount;
    int32_t vertexAttributeStateCount;
    uint32_t memorySize;

    detail::Ptr< void > pNvnVertexStreamState;
    detail::Ptr< void > pNvnVertexAttribState;
};

template<>
struct TessellationStateImplData< ApiVariationNvn8 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    Bit8 state;
    char reserved[ 3 ]; //!< 予約領域です。
    uint32_t patchSize;
};

/**
* @brief ビューポートシザーステートを表す構造体です（NVN8 特殊版）
*/
template<>
struct ViewportScissorStateImplData< ApiVariationNvn8 >
{
    detail::Ptr< void > pWorkMemory; //!< 作業メモリーへのポインターです
    // pWorkMemory
    //    float viewportArray[ viewportCount * 4 ];
    //    double depthRangeArray[ viewportCount * 2 ];
    //    int32_t scissorArray[ viewportCount * 4 ];

    int32_t viewportCount; //!< ビューポートの数です
    uint32_t memorySize; //!< バイトでの作業メモリーのサイズです

    float viewport[ 4 ];
    double depthRange[ 2 ];
    int32_t scissor[ 4 ];

    enum Flag
    {
        Flag_ScissorEnable = ViewportScissorStateInfoData::Flag_ScissorEnable
    };
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };
    Bit8 state; //!< オブジェクトの状態です。
    nn::util::BitPack8 flag; //!< フラグです
    char reserved[ 2 ]; //!< 予約領域です。
};

}
}
