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
* @brief シェーダーリソースファイルのデータに関する API の宣言
*/

#pragma once

#include <nn/util/util_BinaryFormat.h>
#include <nn/util/util_ResDic.h>

#include <nn/gfx/gfx_ShaderInfo.h>

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/gfx_ResShaderData-api.nvn.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/gfx_ResShaderData-api.vk.1.h>
#endif

struct nngfxToolShaderCompilerShaderReflection;

namespace nn {
namespace gfx {

struct ResShaderProgramData;
struct ResShaderVariationData;
struct ResShaderContainerData;
struct ResShaderFileData;
struct ResShaderReflectionStageData;
struct ResShaderReflectionStageData2;
struct ResShaderReflectionData;

/**
* @brief シェーダープログラムリソースを表す構造体です。
*/
struct ResShaderProgramData
{
    ShaderInfoData info; //!< シェーダーを初期化するための情報です。
    uint32_t objSize; //!< シェーダーオブジェクトを保持するメモリーのバイトでの最大サイズです。
    char reserved2[ 4 ]; //!< 予約領域です。
    nn::util::BinTPtr< void > pObj; //!< シェーダーオブジェクトへのポインターです。

    nn::util::BinTPtr< ResShaderVariationData > pParent; //!< このシェーダープログラムを保持しているシェーダーステージへのポインターです。

    nn::util::BinTPtr< ResShaderReflectionData > pShaderReflection; //!< シェーダーリフレクションリソースへのポインターです。
    nn::util::BinTPtr< nngfxToolShaderCompilerShaderReflection > pShaderCompilerReflection; //!< シェーダーコンパイラーから出力された全リフレクション情報へのポインターです。

    nn::util::BinTPtr< void > pShaderLayout; //!< Pointer to the shader layout

    char reserved[ 16 ]; //!< 予約領域です。
};

/**
* @brief シェーダーバリエーションリソースを表す構造体です。
*/
struct ResShaderVariationData
{
    nn::util::BinTPtr< ResShaderProgramData > pSourceProgram; //!< ソースのシェーダープログラムへのポインターです。
    nn::util::BinTPtr< ResShaderProgramData > pIntermediateLanguageProgram; //!< 中間言語表現のシェーダープログラムへのポインターです。
    nn::util::BinTPtr< ResShaderProgramData > pBinaryProgram; //!< バイナリのシェーダープログラムへのポインターです。

    nn::util::BinTPtr< ResShaderContainerData > pParent; //!< このシェーダーステージを保持しているシェーダーコンテナーへのポインターです。

    char reserved[ 32 ]; //!< 予約領域です。
};

/**
* @brief シェーダーコンテナーリソースを表す構造体です。
*/
struct ResShaderContainerData
{
    nn::util::BinaryBlockHeader blockHeader; //!< ブロックヘッダーです。

    uint16_t targetApiType; //!< nn::gfx::LowLevelApi で表される、対象の API の種類です。
    uint16_t targetApiVersion; //!< 対象の API のバージョンです。
    uint8_t targetCodeType; //!< nn::gfx::ShaderCodeType で表される、対象のコードタイプです。
    char reserved2[ 3 ]; //!< 予約領域です。
    uint32_t compilerVersion; //!< コンパイラーのバージョンです。

    uint32_t shaderVariationCount; //!< シェーダーバリエーションの数です。
    nn::util::BinTPtr< ResShaderVariationData > pShaderVariationArray; //!< シェーダーバリエーションの配列へのポインターです。

    nn::util::BinTPtr< void > pShaderBinaryPool; //!< シェーダーバイナリープールへのポインターです。

    uint64_t lowLevelCompilerVersion; //!< 低レベルコンパイラーのバージョンです。

    char reserved[ 40 ]; //!< 予約領域です。
};

/**
* @brief シェーダーファイルリソースを表す構造体です。
*/
struct ResShaderFileData
{
    nn::util::BinaryFileHeader fileHeader; //!< ファイルヘッダーです。

    char reserved[ 64 ]; //!< 予約領域です。
};

/**
* @brief ステージ単位のシェーダーリフレクションリソースを表す構造体です。
*/
struct ResShaderReflectionStageData
{
    nn::util::BinTPtr< nn::util::ResDic > pShaderInputDic; //!< シェーダー入力の辞書へのポインターです
    nn::util::BinTPtr< nn::util::ResDic > pShaderOutputDic; //!< シェーダー出力の辞書へのポインターです
    nn::util::BinTPtr< nn::util::ResDic > pSamplerDic; //!< サンプラーの辞書へのポインターです
    nn::util::BinTPtr< nn::util::ResDic > pConstantBufferDic; //!< 定数バッファーの辞書へのポインターです
    nn::util::BinTPtr< nn::util::ResDic > pUnorderedAccessBufferDic; //!< アンオーダードアクセスバッファーの辞書へのポインターです

    int32_t offsetShaderOutput; //!< 配列におけるシェーダー出力情報の開始位置です
    int32_t offsetSampler; //!< 配列におけるサンプラー情報の開始位置です
    int32_t offsetConstantBuffer; //!< 配列における定数バッファー情報の開始位置です
    int32_t offsetUnorderedAccessBuffer; //!< 配列におけるアンオーダードアクセスバッファー情報の開始位置です
    nn::util::BinTPtr< int32_t > pShaderSlotArray; //!< シェーダーのスロット配列へのポインターです

    uint32_t computeWorkGroupSizeX; //!< 演算シェーダーの場合のワークグループの X 次元のサイズです。
    uint32_t computeWorkGroupSizeY; //!< 演算シェーダーの場合のワークグループの Y 次元のサイズです。
    uint32_t computeWorkGroupSizeZ; //!< 演算シェーダーの場合のワークグループの Z 次元のサイズです。

    int32_t offsetImage; //!< 配列におけるイメージ情報の開始位置です

    nn::util::BinTPtr< nn::util::ResDic > pImageDic; //!< イメージの辞書へのポインターです

    nn::util::BinTPtr< ResShaderReflectionStageData2 > pReflectionStageData2; //!< リフレクション情報の続きへのポインターです
};

/**
* @brief ResShaderReflectionStageData に続くステージ単位のシェーダーリフレクションリソースを表す構造体です。
*/
struct ResShaderReflectionStageData2
{
    nn::util::BinTPtr< nn::util::ResDic > pSeparateTextureDic; //!< セパレートテクスチャーの辞書へのポインターです。
    nn::util::BinTPtr< nn::util::ResDic > pSeparateSamplerDic; //!< セパレートサンプラーの辞書へのポインターです。
    int32_t offsetSeparateTexture; //!< 配列におけるテクスチャー情報の開始位置です。
    int32_t offsetSeparateSampler; //!< 配列におけるサンプラー情報の開始位置です。

    char reserved[ 24 ]; //!< 予約領域です。
};

/**
* @brief シェーダーリフレクションリソースを表す構造体です。
*/
struct ResShaderReflectionData
{
    nn::util::BinTPtr< ResShaderReflectionStageData > pVertexReflection; //!< 頂点シェーダーのリフレクションリソースへのポインターです
    nn::util::BinTPtr< ResShaderReflectionStageData > pHullReflection; //!< ハルシェーダーのリフレクションリソースへのポインターです
    nn::util::BinTPtr< ResShaderReflectionStageData > pDomainReflection; //!< ドメインシェーダーのリフレクションリソースへのポインターです
    nn::util::BinTPtr< ResShaderReflectionStageData > pGeometryReflection; //!< ジオメトリーシェーダーのリフレクションリソースへのポインターです
    nn::util::BinTPtr< ResShaderReflectionStageData > pPixelReflection; //!< ピクセルシェーダーのリフレクションリソースへのポインターです
    nn::util::BinTPtr< ResShaderReflectionStageData > pComputeReflection; //!< 演算シェーダーのリフレクションリソースへのポインターです

    char reserved[ 16 ]; //!< 予約領域です
};

}
}
