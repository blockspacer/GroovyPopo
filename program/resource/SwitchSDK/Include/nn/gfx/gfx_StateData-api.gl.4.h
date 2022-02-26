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
* @brief ステートのデータに関する API の宣言（GL4 特殊版）
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/util/util_BitPack.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.gl.h>
#include <nn/gfx/gfx_StateInfoData.h>

#include <nn/gfx/detail/gfx_Common-api.gl.h>
#include <nn/gfx/detail/gfx_Declare.h>

namespace nn {
namespace gfx {

/**
* @brief ラスタライザーステートを表す構造体です（GL4 特殊版）
*/
template<>
struct RasterizerStateImplData< ApiVariationGl4 >
{
    enum Flag
    {
        Flag_CullEnable,
        Flag_RasterDisable,
        Flag_MultisampleEnable,
        Flag_DepthClampEnable,
        Flag_PolygonOffsetEnable,
        Flag_ConservativeRasterEnable
    };
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };
    Bit8 state; //!< オブジェクトの状態です。
    char reserved; //!< 予約領域です。
    nn::util::BitPack16 flag; //!< フラグです
    detail::GlEnum polygonMode; //!< ポリゴンモードです
    detail::GlEnum frontFace; //!< 前面です
    detail::GlEnum cullFace; //!< カリングモードです
    float slopeScaledDepthBias; //!< 最大深度でスケールされる、深度値に加算される値です
    float depthBias; //!< 深度値に加算される値です
    uint32_t sampleMask; //!< サンプルマスクです
    float depthBiasClamp; //!< 深度値に加算される最大値です
};

/**
* @brief ブレンドステートを表す構造体です（GL4 特殊版）
*/
template<>
struct BlendStateImplData< ApiVariationGl4 >
{
    /**
    * ブレンドターゲットステートを表す構造体です（GL4 特殊版）
    */
    struct BlendTargetState
    {
        enum Flag
        {
            Flag_BlendEnable = BlendTargetStateInfoData::Flag_BlendEnable
        };

        nn::util::BitPack8 flag; //!< フラグです
        Bit8 colorMask; //!< カラーマスクです
        char reserved[ 2 ]; //!< 予約領域です。
        detail::GlEnum srcRGB; //!< ソースのカラー要素のブレンドファクターです
        detail::GlEnum dstRGB; //!< デスティネーションのカラー要素のブレンドファクターです
        detail::GlEnum srcAlpha; //!< ソースのアルファ要素のブレンドファクターです
        detail::GlEnum dstAlpha; //!< デスティネーションのアルファ要素のブレンドファクターです
        detail::GlEnum modeRGB; //!< カラー要素のブレンド関数です
        detail::GlEnum modeAlpha; //!< アルファ要素のブレンド関数です
    };

    enum Flag
    {
        Flag_AlphaToCoverageEnable = BlendStateInfoData::Flag_AlphaToCoverageEnable,
        Flag_IndependentBlendEnable = BlendStateInfoData::Flag_IndependentBlendEnable,
        Flag_LogicOpEnable = BlendStateInfoData::Flag_LogicOperationEnable
    };
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };
    Bit8 state; //!< オブジェクトの状態です。
    char reserved; //!< 予約領域です。
    nn::util::BitPack16 flag; //!< フラグです
    detail::GlEnum logicOp; //!< 論理オペレーションです
    float blendColor[ 4 ]; //!< ブレンド定数です
    int32_t blendTargetCount; //!< ブレンドターゲットの数です
    uint32_t memorySize; //!< バイトでの作業メモリーのサイズです
    detail::Ptr< BlendTargetState > pTargetArray; //!< ブレンドターゲットステートの配列へのポインターです
};

/**
* @brief 深度ステンシルステートを表す構造体です（GL4 特殊版）
*/
template<>
struct DepthStencilStateImplData< ApiVariationGl4 >
{
    /**
    * @brief ステンシルステートを表す構造体です（GL4 特殊版）
    */
    struct StencilState
    {
        detail::GlEnum sfail; //!< ステンシルテスト失敗時のオペレーションです
        detail::GlEnum dpfail; //!< 深度テスト失敗時のオペレーションです
        detail::GlEnum dppass; //!< 深度テスト成功時のオペレーションです
        detail::GlEnum func; //!< ステンシルテストの比較関数です
        int32_t ref; //!< ステンシル参照値です
        uint32_t mask; //!< ステンシルマスクです
    };

    enum Flag
    {
        Flag_DepthTestEnable = DepthStencilStateInfoData::Flag_DepthTestEnable,
        Flag_DepthWriteEnable = DepthStencilStateInfoData::Flag_DepthWriteEnable,
        Flag_StencilTestEnable = DepthStencilStateInfoData::Flag_StencilTestEnable,
        Flag_DepthBoundsTestEnable = DepthStencilStateInfoData::Flag_DepthBoundsTestEnable
    };
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };
    Bit8 state; //!< オブジェクトの状態です。
    char reserved; //!< 予約領域です。
    nn::util::BitPack16 flag; //!< フラグです
    detail::GlEnum depthFunc; //!< 深度テストの比較関数です
    StencilState frontStencil; //!< 前面のステンシルステートです
    StencilState backStencil; //!< 背面のステンシルステートです
};

/**
* @brief 頂点ステートを表す構造体です（GL4 特殊版）
*/
template<>
struct VertexStateImplData< ApiVariationGl4 >
{
    /**
    * @brief 頂点属性ステートを表す構造体です（GL4 特殊版）
    */
    struct AttributeState
    {
        detail::GlBool normalized; //!< 正規化されているかどうかです
        char reserved[ 3 ]; //!< 予約領域です。
        int32_t slot; //!< シェーダーにおけるスロット番号です
        int32_t size; //!< 要素数です
        detail::GlEnum type; //!< 頂点属性のタイプです
        uint32_t offset; //!< バイトでのバッファーにおけるオフセットです
        uint32_t bindingIndex; //!< バッファーインデックスです
        Bit32 typeFormat; //!< 頂点属性フォーマットです
    };
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };
    Bit8 state; //!< オブジェクトの状態です。
    char reserved[ 3 ]; //!< 予約領域です。
    int32_t attributeCount; //!< 頂点属性の数です
    int32_t bufferCount; //!< 頂点バッファーの数です
    uint32_t memorySize; //!< 作業メモリーのサイズです
    detail::Ptr< void > pWorkMemory; //!< 作業メモリーへのポインターです

    // pWorkMemory
    //    AttributeState[ attributeCount ]
    //    uint32_t[ bufferCount ]
};

/**
* @brief テッセレーションステートを表す構造体です（GL4 特殊版）
*/
template<>
struct TessellationStateImplData< ApiVariationGl4 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    Bit8 state; //!< オブジェクトの状態です
    char reserved[ 3 ]; //!< 予約領域です。
    int32_t patchControlPointCount; //!< パッチの制御点の数です
};

/**
* @brief ビューポートシザーステートを表す構造体です（GL4 特殊版）
*/
template<>
struct ViewportScissorStateImplData< ApiVariationGl4 >
{
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
    int32_t viewportCount; //!< ビューポートの数です
    uint32_t memorySize; //!< バイトでの作業メモリーのサイズです
    char reserved2[ 4 ]; //!< 予約領域です。
    float viewport[ 4 ];
    double depthRange[ 2 ];
    int32_t scissor[ 4 ];
    detail::Ptr< void > pWorkMemory; //!< 作業メモリーへのポインターです
};

}
}
