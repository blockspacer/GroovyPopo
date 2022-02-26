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

#include "gfxTool_ApiCommon.h"

enum
{
    nngfxToolShaderCompilerLowLevelApiType_Nvn = 2 //!< NVN です
};

enum
{
    nngfxToolShaderCompilerPlatform_Nx = 2 //!< NX です
};

enum
{
    nngfxToolShaderCompilerOptionType_Nvn = 3 //!< NVN のオプションです
};

enum
{
    nngfxToolShaderCompilerOptionOutputType_ProgramNvn = 2, //!< NVN のプログラム単位の出力です。
};

/**
* @brief NVN 特有のコンパイルオプションです。詳細は glslc を参照してください。
*/
struct nngfxToolShaderCompilerCompileOptionNvn
{
    uint8_t spillControl; //!< SpillControlEnum で表されるレジスターのスピル制御です。
    uint8_t tessellationAndPassthroughGS; //!< 0 以外の場合、テセレーションとパススルージオメトリーシェーダーをペアとして動くようコンパイルします。
    uint8_t prioritizeConsecutiveTextureInstructions; //!< 0 以外の場合、演算シェーダーにおいてテクスチャーのバッチ化を優先して最適化を行います。
    uint8_t enableFastMathMask; //!< NVNshaderStageBits の組み合わせで表される、fast math 最適化を有効化するステージです。
    uint8_t optLevel; //!< GLSLCoptLevelEnum で表される、シェーダーをコンパイルする最適化レベルです。--optimization-levelの設定より優先して設定されます。
    uint8_t unrollControl; //!< GLSLCunrollControlEnum で表される、ループのアンロールの振る舞いの制御です。
    uint8_t errorOnScratchMemUsage; //!< 0 以外の場合、データの格納にスクラッチメモリーを使うことをエラーとして扱います。
    uint8_t enableCBFOptimization; //!< 0 以外の場合、cull-before-fetch の最適化を有効化します。
    uint8_t enableWarpCulling; //!< 0 以外の場合、ワープカリングの最適化を有効化します。
    uint8_t warnUninitControl; // !< GLSLCwarnUninitControlEnum で表される、未初期化変数に対する警告の制御です。

    char reserved[ 62 ]; //!< 予約領域です
};

/**
* @brief NVN 特有のステージごとに追加される出力です。
*/
struct nngfxToolShaderCompilerOptionOutputStageNvn
{
    uint32_t requiredScratchMemorySizePerWarp; //!< バイトでの最低限要求されるワープあたりのスクラッチメモリーのサイズです
    uint32_t recommendedScratchMemorySizePerWarp; //!< バイトでの推奨されるワープあたりのスクラッチメモリーのサイズです

    char reserved[ 32 ]; //!< 予約領域です
};

/**
* @brief NVN 特有のプログラムごとに追加される出力です。
*/
struct nngfxToolShaderCompilerOptionOutputProgramNvn
{
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerOptionOutputStageNvn* pVertexStageOutput ); //!< 頂点シェーダーの追加出力です
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerOptionOutputStageNvn* pHullStageOutput ); //!< ハルシェーダーの追加出力です
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerOptionOutputStageNvn* pDomainStageOutput ); //!< ドメインシェーダーの追加出力です
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerOptionOutputStageNvn* pGeometryStageOutput ); //!< ジオメトリーシェーダーの追加出力です
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerOptionOutputStageNvn* pPixelStageOutput ); //!< ピクセルシェーダーの追加出力です
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerOptionOutputStageNvn* pComputeStageOutput ); //!< 演算シェーダーの追加出力です

    char reserved[ 32 ]; //!< 予約領域です
};
