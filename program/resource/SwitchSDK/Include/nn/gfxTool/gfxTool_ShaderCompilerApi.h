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
* @brief シェーダーコンパイラーに関する API の宣言
*/

#pragma once

#include "gfxTool_ApiCommon.h"

#ifdef _WIN32
    #ifdef NN_GFXTOOL_SHADERCOMPILER_DLL_EXPORTS
        #define NN_GFXTOOL_SHADERCOMPILER_API extern "C" __declspec( dllexport )
    #else
        #define NN_GFXTOOL_SHADERCOMPILER_API extern "C" __declspec( dllimport )
    #endif
#else
    #define NN_GFXTOOL_SHADERCOMPILER_API extern "C"
#endif

#define NN_GFXTOOL_SHADERCOMPILER_VERSION_MAJOR 2 // NOLINT(preprocessor/const)
#define NN_GFXTOOL_SHADERCOMPILER_VERSION_MINOR 8 // NOLINT(preprocessor/const)
#define NN_GFXTOOL_SHADERCOMPILER_VERSION_MICRO 1 // NOLINT(preprocessor/const)

/**
* @brief シェーダーコンパイラーのバージョンです。
*/
enum nngfxToolShaderCompilerVersion
{
    nngfxToolShaderCompilerVersion_Major = NN_GFXTOOL_SHADERCOMPILER_VERSION_MAJOR, //!< メジャーバージョンです
    nngfxToolShaderCompilerVersion_Minor = NN_GFXTOOL_SHADERCOMPILER_VERSION_MINOR, //!< マイナーバージョンです
    nngfxToolShaderCompilerVersion_Micro = NN_GFXTOOL_SHADERCOMPILER_VERSION_MICRO //!< マイクロバージョンです
};

/**
* @brief シェーダーソースのフォーマットです。
*/
enum nngfxToolShaderCompilerShaderSourceFormat
{
    nngfxToolShaderCompilerShaderSourceFormat_Glsl, //!< GLSL シェーダ言語です。
    nngfxToolShaderCompilerShaderSourceFormat_SpirV, //!< SPIR-V 中間表現です。
    nngfxToolShaderCompilerShaderSourceFormat_Hlsl, //!< HLSL シェーダ言語です。

    nngfxToolShaderCompilerShaderSourceFormat_End
};

/**
* @brief シェーダーコードの種類です。
*/
enum nngfxToolShaderCompilerCodeType
{
    nngfxToolShaderCompilerCodeType_Undefined, //!< 不明なコードです。
    nngfxToolShaderCompilerCodeType_Binary, //!< バイナリーです。
    nngfxToolShaderCompilerCodeType_Source, //!< ソースです。
    nngfxToolShaderCompilerCodeType_Binary_Source, //!< バイナリーとソースです。
    nngfxToolShaderCompilerCodeType_Binary_Ir, //!< バイナリーと中間表現です。
    nngfxToolShaderCompilerCodeType_Ir, //!< 中間表現です。
    nngfxToolShaderCompilerCodeType_Ir_Source //!< 中間表現とソースです。
};

/**
* @brief 低レベルグラフィックス API の種類です。
*/
enum nngfxToolShaderCompilerLowLevelApiType
{
    nngfxToolShaderCompilerLowLevelApiType_Gl = 0 //!< GL です。
};

/**
* @brief オプションの種類です。
*/
enum nngfxToolShaderCompilerOptionType
{
    nngfxToolShaderCompilerOptionType_Common = 0, //!< 共通のオプションです。
    nngfxToolShaderCompilerOptionType_Glsl = 1, //!< GLSL 専用のオプションです。
};

/**
* @brief 変数の種類です。
*/
enum nngfxToolShaderCompilerVariableType
{
    nngfxToolShaderCompilerVariableType_Bool32, //!< 32 ビット真偽値です。
    nngfxToolShaderCompilerVariableType_Int32, //!< 32 ビット符号付き整数です。
    nngfxToolShaderCompilerVariableType_Uint32, //!< 32 ビット符号なし整数です。
    nngfxToolShaderCompilerVariableType_Float32, //!< 32 ビット浮動小数点数です。
    nngfxToolShaderCompilerVariableType_Float64, //!< 64 ビット浮動小数点数です。

    nngfxToolShaderCompilerVariableType_End
};

/**
* @brief ダンプの種類です。
*/
enum nngfxToolShaderCompilerDump
{
    nngfxToolShaderCompilerDump_NoDump, //!< ダンプしません。
    nngfxToolShaderCompilerDump_FullDump, //!< 完全にダンプします。

    nngfxToolShaderCompilerDump_End
};

/**
* @brief ログ出力のレベルです。
*/
enum nngfxToolShaderCompilerLogOutputLevel
{
    nngfxToolShaderCompilerLogOutputLevel_Default, //!< 標準的なログ出力を行います。
    nngfxToolShaderCompilerLogOutputLevel_Silent, //!< ログ出力を行いません。

    nngfxToolShaderCompilerLogOutputLevel_End
};

/**
* @brief デバッグ情報レベルです。
*/
enum nngfxToolShaderCompilerDebugInfoLevel
{
    nngfxToolShaderCompilerDebugInfoLevel_None, //!< デバッグ情報なしです。
    nngfxToolShaderCompilerDebugInfoLevel_Debug, //!< 標準的なデバッグ情報レベルです。
    nngfxToolShaderCompilerDebugInfoLevel_High, //!< 高いデバッグ情報レベルです。
    nngfxToolShaderCompilerDebugInfoLevel_Full, //!< 完全なデバッグ情報レベルです。

    nngfxToolShaderCompilerDebugInfoLevel_Medium = nngfxToolShaderCompilerDebugInfoLevel_Debug //!< 標準的なデバッグ情報レベルです。
};

/**
* @brief オプション出力の種類です。
*/
enum nngfxToolShaderCompilerOptionOutputType
{
    nngfxToolShaderCompilerOptionOutputType_ProgramCommon = 0, //!< 共通のプログラム単位の出力です。
};

/**
* @brief どのステージが参照しているかのビット値です。リフレクション情報に使われます。
*/
enum nngfxToolShaderCompilerReflectionStageReference
{
    nngfxToolShaderCompilerReflectionStageReference_Vertex = 0x01, //!< 頂点ステージです
    nngfxToolShaderCompilerReflectionStageReference_Hull = 0x02, //!< ハルステージです
    nngfxToolShaderCompilerReflectionStageReference_Domain = 0x04, //!< ドメインステージです
    nngfxToolShaderCompilerReflectionStageReference_Geometry = 0x08, //!< ジオメトリーステージです
    nngfxToolShaderCompilerReflectionStageReference_Pixel = 0x10, //!< ピクセルステージです
    nngfxToolShaderCompilerReflectionStageReference_Compute = 0x20 //!< 演算ステージです
};

/**
* @brief 変数の型です。リフレクション情報に使われます。
*/
enum nngfxToolShaderCompilerReflectionVariableType
{
    nngfxToolShaderCompilerReflectionVariableType_Unknown, //!< 不明な型です

    nngfxToolShaderCompilerReflectionVariableType_Bool32, //!< 32-bit bool スカラー型です
    nngfxToolShaderCompilerReflectionVariableType_Int8, //!< 8-bit int スカラー型です
    nngfxToolShaderCompilerReflectionVariableType_Int16, //!< 16-bit int スカラー型です
    nngfxToolShaderCompilerReflectionVariableType_Int32, //!< 32-bit int スカラー型です
    nngfxToolShaderCompilerReflectionVariableType_Int64, //!< 64-bit int スカラー型です
    nngfxToolShaderCompilerReflectionVariableType_Uint8, //!< 8-bit uint スカラー型です
    nngfxToolShaderCompilerReflectionVariableType_Uint16, //!< 16-bit uint スカラー型です
    nngfxToolShaderCompilerReflectionVariableType_Uint32, //!< 32-bit uint スカラー型です
    nngfxToolShaderCompilerReflectionVariableType_Uint64, //!< 64-bit uint スカラー型です
    nngfxToolShaderCompilerReflectionVariableType_Float16, //!< 16-bit float スカラー型です
    nngfxToolShaderCompilerReflectionVariableType_Float32, //!< 32-bit float スカラー型です
    nngfxToolShaderCompilerReflectionVariableType_Float64, //!< 64-bit float スカラー型です

    nngfxToolShaderCompilerReflectionVariableType_Bool32x2, //!< 2 要素 32-bit bool ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Bool32x3, //!< 3 要素 32-bit bool ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Bool32x4, //!< 4 要素 32-bit bool ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Int8x2, //!< 2 要素 8-bit int ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Int8x3, //!< 3 要素 8-bit int ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Int8x4, //!< 4 要素 8-bit int ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Int16x2, //!< 2 要素 16-bit int ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Int16x3, //!< 3 要素 16-bit int ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Int16x4, //!< 4 要素 16-bit int ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Int32x2, //!< 2 要素 32-bit int ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Int32x3, //!< 3 要素 32-bit int ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Int32x4, //!< 4 要素 32-bit int ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Int64x2, //!< 2 要素 64-bit int ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Int64x3, //!< 3 要素 64-bit int ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Int64x4, //!< 4 要素 64-bit int ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Uint8x2, //!< 2 要素 8-bit uint ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Uint8x3, //!< 3 要素 8-bit uint ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Uint8x4, //!< 4 要素 8-bit uint ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Uint16x2, //!< 2 要素 16-bit uint ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Uint16x3, //!< 3 要素 16-bit uint ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Uint16x4, //!< 4 要素 16-bit uint ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Uint32x2, //!< 2 要素 32-bit uint ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Uint32x3, //!< 3 要素 32-bit uint ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Uint32x4, //!< 4 要素 32-bit uint ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Uint64x2, //!< 2 要素 64-bit uint ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Uint64x3, //!< 3 要素 64-bit uint ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Uint64x4, //!< 4 要素 64-bit uint ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Float16x2, //!< 2 要素 16-bit float ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Float16x3, //!< 3 要素 16-bit float ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Float16x4, //!< 4 要素 16-bit float ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Float32x2, //!< 2 要素 32-bit float ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Float32x3, //!< 3 要素 32-bit float ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Float32x4, //!< 4 要素 32-bit float ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Float64x2, //!< 2 要素 64-bit float ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Float64x3, //!< 3 要素 64-bit float ベクトル型です
    nngfxToolShaderCompilerReflectionVariableType_Float64x4, //!< 4 要素 64-bit float ベクトル型です

    nngfxToolShaderCompilerReflectionVariableType_Bool32x2x2, //!< 2x2 要素 32-bit bool 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Bool32x2x3, //!< 2x3 要素 32-bit bool 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Bool32x2x4, //!< 2x4 要素 32-bit bool 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Bool32x3x2, //!< 3x2 要素 32-bit bool 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Bool32x3x3, //!< 3x3 要素 32-bit bool 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Bool32x3x4, //!< 3x4 要素 32-bit bool 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Bool32x4x2, //!< 4x2 要素 32-bit bool 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Bool32x4x3, //!< 4x3 要素 32-bit bool 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Bool32x4x4, //!< 4x4 要素 32-bit bool 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Int32x2x2, //!< 2x2 要素 32-bit int 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Int32x2x3, //!< 2x3 要素 32-bit int 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Int32x2x4, //!< 2x4 要素 32-bit int 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Int32x3x2, //!< 3x2 要素 32-bit int 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Int32x3x3, //!< 3x3 要素 32-bit int 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Int32x3x4, //!< 3x4 要素 32-bit int 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Int32x4x2, //!< 4x2 要素 32-bit int 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Int32x4x3, //!< 4x3 要素 32-bit int 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Int32x4x4, //!< 4x4 要素 32-bit int 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Uint32x2x2, //!< 2x2 要素 32-bit uint 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Uint32x2x3, //!< 2x3 要素 32-bit uint 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Uint32x2x4, //!< 2x4 要素 32-bit uint 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Uint32x3x2, //!< 3x2 要素 32-bit uint 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Uint32x3x3, //!< 3x3 要素 32-bit uint 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Uint32x3x4, //!< 3x4 要素 32-bit uint 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Uint32x4x2, //!< 4x2 要素 32-bit uint 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Uint32x4x3, //!< 4x3 要素 32-bit uint 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Uint32x4x4, //!< 4x4 要素 32-bit uint 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float32x2x2, //!< 2x2 要素 32-bit float 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float32x2x3, //!< 2x3 要素 32-bit float 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float32x2x4, //!< 2x4 要素 32-bit float 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float32x3x2, //!< 3x2 要素 32-bit float 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float32x3x3, //!< 3x3 要素 32-bit float 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float32x3x4, //!< 3x4 要素 32-bit float 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float32x4x2, //!< 4x2 要素 32-bit float 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float32x4x3, //!< 4x3 要素 32-bit float 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float32x4x4, //!< 4x4 要素 32-bit float 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float64x2x2, //!< 2x2 要素 64-bit 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float64x2x3, //!< 2x3 要素 64-bit 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float64x2x4, //!< 2x4 要素 64-bit 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float64x3x2, //!< 3x2 要素 64-bit 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float64x3x3, //!< 3x3 要素 64-bit 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float64x3x4, //!< 3x4 要素 64-bit 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float64x4x2, //!< 4x2 要素 64-bit 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float64x4x3, //!< 4x3 要素 64-bit 行列型です
    nngfxToolShaderCompilerReflectionVariableType_Float64x4x4, //!< 4x4 要素 64-bit 行列型です

    nngfxToolShaderCompilerReflectionVariableType_Sampler1D, //!< 一次元サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_Sampler2D, //!< 二次元サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_Sampler3D, //!< 三次元サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_SamplerCube, //!< キューブマップサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_Sampler1DShadow, //!< 一次元シャドウサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_Sampler2DShadow, //!< 二次元シャドウサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_Sampler1DArray, //!< 一次元配列サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_Sampler2DArray, //!< 二次元配列サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_Sampler1DArrayShadow, //!< 一次元配列シャドウサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_Sampler2DArrayShadow, //!< 二次元配列シャドウサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_Sampler2DMultisample, //!< 二次元マルチサンプルサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_Sampler2DMultisampleArray, //!< 二次元マルチサンプル配列サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_SamplerCubeShadow, //!< キューブマップシャドウサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_SamplerBuffer, //!< バッファーサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_Sampler2DRect, //!< 二次元矩形サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_Sampler2DRectShadow, //!< 二次元矩形シャドウサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_IntSampler1D, //!< 符号付き整数一次元サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_IntSampler2D, //!< 符号付き整数二次元サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_IntSampler3D, //!< 符号付き整数三次元サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_IntSamplerCube, //!< 符号付き整数キューブマップサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_IntSampler1DArray, //!< 符号付き整数一次元配列サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_IntSampler2DArray, //!< 符号付き整数二次元配列サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_IntSampler2DMultisample, //!< 符号付き整数二次元マルチサンプルサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_IntSampler2DMultisampleArray, //!< 符号付き整数二次元マルチサンプル配列サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_IntSamplerBuffer, //!< 符号付き整数バッファーサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_IntSampler2DRect, //!< 符号付き整数二次元矩形サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_UintSampler1D, //!< 符号なし整数一次元サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_UintSampler2D, //!< 符号なし整数二次元サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_UintSampler3D, //!< 符号なし整数三次元サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_UintSamplerCube, //!< 符号なし整数キューブマップサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_UintSampler1DArray, //!< 符号なし整数一次元配列サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_UintSampler2DArray, //!< 符号なし整数二次元配列サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_UintSampler2DMultisample, //!< 符号なし整数二次元マルチサンプルサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_UintSampler2DMultisampleArray, //!< 符号なし整数二次元マルチサンプル配列サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_UintSamplerBuffer, //!< 符号なし整数バッファーサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_UintSampler2DRect, //!< 符号なし整数二次元矩形サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_SamplerCubeMapArray, //!< キューブマップ配列サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_SamplerCubeMapArrayShadow, //!< キューブマップ配列シャドウサンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_IntSamplerCubeMapArray, //!< 符号付き整数キューブマップ配列サンプラー型です
    nngfxToolShaderCompilerReflectionVariableType_UintSamplerCubeMapArray, //!< 符号なし整数キューブマップ配列サンプラー型です

    nngfxToolShaderCompilerReflectionVariableType_Image1D, //!< 一次元イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_Image2D, //!< 二次元イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_Image3D, //!< 三次元イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_ImageCube, //!< キューブマップイメージ型です
    nngfxToolShaderCompilerReflectionVariableType_Image1DArray, //!< 一次元配列イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_Image2DArray, //!< 二次元配列イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_Image2DMultisample, //!< 二次元マルチサンプルイメージ型です
    nngfxToolShaderCompilerReflectionVariableType_Image2DMultisampleArray, //!< 二次元マルチサンプル配列イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_ImageCubeMapArray, //!< キューブマップ配列イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_ImageBuffer, //!< バッファーイメージ型です
    nngfxToolShaderCompilerReflectionVariableType_Image2DRect, //!< 二次元矩形イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_IntImage1D, //!< 符号付き整数一次元イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_IntImage2D, //!< 符号付き整数二次元イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_IntImage3D, //!< 符号付き整数三次元イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_IntImageCube, //!< 符号付き整数キューブマップイメージ型です
    nngfxToolShaderCompilerReflectionVariableType_IntImage1DArray, //!< 符号付き整数一次元配列イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_IntImage2DArray, //!< 符号付き整数二次元配列イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_IntImage2DMultisample, //!< 符号付き整数二次元マルチサンプルイメージ型です
    nngfxToolShaderCompilerReflectionVariableType_IntImage2DMultisampleArray, //!< 符号付き整数二次元マルチサンプル配列イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_IntImageCubeMapArray, //!< 符号付き整数キューブマップ配列イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_IntImageBuffer, //!< 符号付き整数バッファーイメージ型です
    nngfxToolShaderCompilerReflectionVariableType_IntImage2DRect, //!< 符号付き整数二次元矩形イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_UintImage1D, //!< 符号なし整数一次元イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_UintImage2D, //!< 符号なし整数二次元イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_UintImage3D, //!< 符号なし整数三次元イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_UintImageCube, //!< 符号なし整数キューブマップイメージ型です
    nngfxToolShaderCompilerReflectionVariableType_UintImage1DArray, //!< 符号なし整数一次元配列イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_UintImage2DArray, //!< 符号なし整数二次元配列イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_UintImage2DMultisample, //!< 符号なし整数二次元マルチサンプルイメージ型です
    nngfxToolShaderCompilerReflectionVariableType_UintImage2DMultisampleArray, //!< 符号なし整数二次元マルチサンプル配列イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_UintImageCubeMapArray, //!< 符号なし整数キューブマップ配列イメージ型です
    nngfxToolShaderCompilerReflectionVariableType_UintImageBuffer, //!< 符号なし整数バッファイメージ型です
    nngfxToolShaderCompilerReflectionVariableType_UintImage2DRect, //!< 符号なし整数二次元矩形イメージ型です

    nngfxToolShaderCompilerReflectionVariableType_Sampler, //!< サンプラー型です

    nngfxToolShaderCompilerReflectionVariableType_Texture1D, //!< 一次元テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_Texture2D, //!< 二次元テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_Texture3D, //!< 三次元テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_TextureCube, //!< キューブマップテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_Texture1DShadow, //!< 一次元シャドウテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_Texture2DShadow, //!< 二次元シャドウテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_Texture1DArray, //!< 一次元配列テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_Texture2DArray, //!< 二次元配列テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_Texture1DArrayShadow, //!< 一次元配列シャドウテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_Texture2DArrayShadow, //!< 二次元配列シャドウテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_Texture2DMultisample, //!< 二次元マルチサンプルテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_Texture2DMultisampleArray, //!< 二次元マルチサンプル配列テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_TextureCubeShadow, //!< キューブマップシャドウテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_TextureBuffer, //!< バッファーテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_Texture2DRect, //!< 二次元矩形テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_Texture2DRectShadow, //!< 二次元矩形シャドウテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_TextureCubeMapArray, //!< キューブマップ配列テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_TextureCubeMapArrayShadow, //!< キューブマップ配列シャドウテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_IntTexture1D, //!< 符号付き整数一次元テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_IntTexture2D, //!< 符号付き整数二次元テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_IntTexture3D, //!< 符号付き整数三次元テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_IntTextureCube, //!< 符号付き整数キューブマップテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_IntTexture1DArray, //!< 符号付き整数一次元配列テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_IntTexture2DArray, //!< 符号付き整数二次元配列テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_IntTexture2DMultisample, //!< 符号付き整数二次元マルチサンプルテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_IntTexture2DMultisampleArray, //!< 符号付き整数二次元マルチサンプル配列テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_IntTextureBuffer, //!< 符号付き整数バッファーテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_IntTexture2DRect, //!< 符号付き整数二次元矩形テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_IntTextureCubeMapArray, //!< 符号付き整数キューブマップ配列テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_UintTexture1D, //!< 符号なし整数一次元テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_UintTexture2D, //!< 符号なし整数二次元テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_UintTexture3D, //!< 符号なし整数三次元テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_UintTextureCube, //!< 符号なし整数キューブマップテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_UintTexture1DArray, //!< 符号なし整数一次元配列テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_UintTexture2DArray, //!< 符号なし整数二次元配列テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_UintTexture2DMultisample, //!< 符号なし整数二次元マルチサンプルテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_UintTexture2DMultisampleArray, //!< 符号なし整数二次元マルチサンプル配列テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_UintTextureBuffer, //!< 符号なし整数バッファーテクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_UintTexture2DRect, //!< 符号なし整数二次元矩形テクスチャー型です
    nngfxToolShaderCompilerReflectionVariableType_UintTextureCubeMapArray //!< 符号なし整数キューブマップ配列テクスチャー型です
};

/**
* @brief 最適化レベルです。
*/
enum nngfxToolShaderCompilerOptimizationLevel
{
    nngfxToolShaderCompilerOptimizationLevel_None, //!< 最適化なしです。
    nngfxToolShaderCompilerOptimizationLevel_Default, //!< 標準的な最適化レベルです。
    nngfxToolShaderCompilerOptimizationLevel_High, //!< 高い最適化レベルです。
    nngfxToolShaderCompilerOptimizationLevel_Full, //!< 完全な最適化レベルです。
    nngfxToolShaderCompilerOptimizationLevel_Size //!< コードサイズの最適化をします。
};

/**
* @brief ハンドル作成の引数です。
*/
struct nngfxToolShaderCompilerCreateHandleArg
{
    char reserved[ 32 ]; //!< 予約領域です。
};

/**
* @brief バリエーション定数の値です。
*/
struct nngfxToolShaderCompilerVariationConstantValue
{
    uint32_t valueSizeIn32Bit; //!< 32 ビット単位での値のサイズです
    char reserved2[ 4 ]; //!< 予約領域です。
    NN_GFXTOOL_PTR64( const void* pValue ); //!< 値へのポインターです。

    char reserved[ 16 ]; //!< 予約領域です。
};

/**
* @brief バリエーションの値です。
*/
struct nngfxToolShaderCompilerVariationValue
{
    uint32_t variationConstantValueCount; //!< バリエーション定数の値の数です。
    uint32_t preprocessorDefinitionValueCount; //!< プリプロセッサー定義の値の数です。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationConstantValue*
        pVariationConstantValueArray ); //!< バリエーション定数の値の配列へのポインターです。
    NN_GFXTOOL_PTR64( const nngfxToolString* pPreprocessorDefinitionValueArray ); //!< プリプロセッサー定義の値の文字列の配列へのポインターです。

    char reserved[ 32 ]; //!< 予約領域です。
};

/**
* @brief バリエーション定数の定義です。
*/
struct nngfxToolShaderCompilerVariationConstantDefinition
{
    int16_t type; //!< nngfxToolShaderCompilerVariableType で表される変数の種類です。
    union
    {
        uint8_t vectorComponents; //!< ベクトルの要素数です。スカラーの場合は 0 を指定します。
        struct
        {
            uint8_t column; //!< 行列の列数です。スカラーの場合は 0 を指定します。
            uint8_t row; //!< 行列の行数です。スカラーまたはベクトルの場合は 0 を指定します。
        } matrix;
    };
    uint32_t arrayLength; //!< 配列長です。配列でない場合は 0 を指定します。使用する低レベル API によっては 0 以外に対応していないことがあります。
    int32_t indexInVariationConstantValueArray; //!< バリエーション定数の値の配列におけるインデックスです。他のバリエーション定数のレイアウトにおける値と重なってはいけません。
    char reserved2[ 4 ]; //!< 予約領域です。
    nngfxToolString name; //!< 変数名です。

    char reserved[ 32 ]; //!< 予約領域です。
};

/**
* @brief プリプロセッサー定義の定義です。
*/
struct nngfxToolShaderCompilerPreprocessorDefinitionDefinition
{
    int32_t indexInPreprocessorDefinitionValueArray; //!< プリプロセッサー定義の値の配列におけるインデックスです。他のプリプロセッサー定義のレイアウトにおける値と重なってはいけません。
    char reserved2[ 4 ];
    nngfxToolString name; //!< プリプロセッサー定義の名前です。

    char reserved[ 16 ]; //!< 予約領域です。
};

/**
* @brief バリエーションの定義です。
*/
struct nngfxToolShaderCompilerVariationDefinition
{
    uint32_t variationConstantDefinitionCount; //!< 定数バリエーションの定義の数です。
    uint32_t preprocessorDefinitionDefinitionCount; //!< プリプロセッサー定義の定義の数です。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationConstantDefinition*
        pVariationConstantDefinitionArray ); //!< バリエーション定数の定義の配列へのポインターです。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerPreprocessorDefinitionDefinition*
        pPreprocessorDefinitionDefinitionArray ); //!< プリプロセッサ定義の定義の配列へのポインターです。
    nngfxToolString variationConstantBufferName; //!< バリエーション定数のバッファー名です。

    char reserved[ 32 ]; //!< 予約領域です。
};

/**
* @brief コンパイルオプションです。
*/
struct nngfxToolShaderCompilerCompileOption
{
    uint32_t optionType; //!< nngfxToolShaderCompilerOptionType で表されるオプションの種類です。
    char reserved2[ 4 ]; //!< 予約領域です。
    NN_GFXTOOL_PTR64( const void* pOption ); //!< オプションへのポインターです。

    char reserved[ 16 ]; //!< 予約領域です。
};

/**
* @brief 共通のコンパイルオプションです。
*/
struct nngfxToolShaderCompilerCompileOptionCommon
{
    int8_t dump; //!< nngfxToolShaderCompilerDump で表される、コンパイル前ソースのダンプ度合いです。
    int8_t isSeparationEnabled; //!< 0 以外であればシェーダーをステージごとに別にします。
    int8_t isShaderStatisticsEnabled; //!< 0 以外であればシェーダーの統計情報を出力します。
    int8_t isReflectionEnabled; //!< 0 以外であればシェーダーのリフレクション情報を出力します。
    float printProgress; //!< 全体の進捗を 1.0 として、どれだけの進捗ごとにコンパイルの進捗状況を表示するかの値です。0 以下または 1.0 より大きい値を指定した場合は進捗を表示しません。

    uint32_t maxThreads; //!< 並列処理の最大スレッド数です。

    uint32_t preprocessorDefinitionCount; //!< プリプロセッサー定義の数です。
    NN_GFXTOOL_PTR64( const nngfxToolString* pPreprocessorDefinitionArray ); //!< プリプロセッサー定義の文字列の配列へのポインターです。"#define " 以降の文字列を指定します。

    uint32_t codePage; //!< ソースコードのコードページです。
    uint8_t isCompressionEnabled; //!< 0 以外であればシェーダーデータの圧縮を試みます。圧縮は nn::util::CompressZlib() により行われます。
    uint8_t isPreprocessEnabled; //!< 0 以外であれば低レベルコンパイラーを呼ぶ前に事前にシェーダーコードをプリプロセスします
    uint8_t logOutputLevel; //!< nngfxToolShaderCompilerLogOutputLevel で表される、ログ出力のレベルです。
    uint8_t debugInfoLevel; //!< nngfxToolShaderCompilerDebugInfoLevel で表される、付加するデバッグ情報のレベルです。

    nngfxToolString uniformRegisterBlockName; //!< ユニフォームレジスターをブロックにまとめる場合のブロック名です。isPreprocessEnabled が 0 以外の場合のみ効果をもちます。NULL または空文字が指定された場合はユニフォームレジスターをブロックにまとめません。

    uint8_t isInvertYEnabled; //!< 0 以外であれば頂点シェーダに出力位置の y 成分を反転させる処理を追加します。isPreprocessEnabled が 0 以外の場合のみ効果をもちます。
    uint8_t isRemapZEnabled; //!< 0 以外であれば頂点シェーダに出力位置の z 成分を [-1, 1] から [0, 1] にマップする処理を追加します。isPreprocessEnabled が 0 以外の場合のみ効果をもちます。

    uint8_t optimizationLevel; //!< nngfxToolShaderCompilerOptimizationLevel で表される、最適化のレベルです。

    char reserved[ 37 ]; //!< 予約領域です。
};

/**
* @brief GLSL 特有のコンパイルオプションです。
*/
struct nngfxToolShaderCompilerCompileOptionGlsl
{
    nngfxToolString glslVersion; //!< GLSL のバージョン文字列です。
    uint32_t glslExtensionCount; //!< GLSL 拡張文字列の数です。
    char reserved2[ 4 ]; //!< 予約領域です。
    NN_GFXTOOL_PTR64( const nngfxToolString* pGlslExtensionArray ); //!< GLSL 拡張文字列の配列へのポインターです。

    char reserved[ 48 ]; //!< 予約領域です。
};

/**
* @brief バリエーション定義の引数です。
*/
struct nngfxToolShaderCompilerVariationDefinitionArg
{
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationDefinition*
        pVertexShaderVariationDefinition ); //!< 頂点シェーダーのバリエーションの定義です。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationDefinition*
        pHullShaderVariationDefinition ); //!< ハルシェーダーのバリエーションの定義です。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationDefinition*
        pDomainShaderVariationDefinition ); //!< ドメインシェーダーのバリエーションの定義です。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationDefinition*
        pGeometryShaderVariationDefinition ); //!< ジオメトリーシェーダーのバリエーションの定義です。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationDefinition*
        pPixelShaderVariationDefinition ); //!< ピクセルシェーダーのバリエーションの定義です。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationDefinition*
        pComputeShaderVariationDefinition ); //!< 演算シェーダーのバリエーションの定義です。

    char reserved[ 64 ]; //!< 予約領域です。
};

/**
* @brief バリエーション値の引数です。
*/
struct nngfxToolShaderCompilerVariationValueArg
{
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationValue*
        pVertexShaderVariationValue ); //!< 頂点シェーダーのバリエーションの値です。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationValue*
        pHullShaderVariationValue ); //!< ハルシェーダーのバリエーションの値です。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationValue*
        pDomainShaderVariationValue ); //!< ドメインシェーダーのバリエーションの値です。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationValue*
        pGeometryShaderVariationValue ); //!< ジオメトリーシェーダーのバリエーションの値です。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationValue*
        pPixelShaderVariationValue ); //!< ピクセルシェーダーのバリエーションの値です。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationValue*
        pComputeShaderVariationValue ); //!< 演算シェーダーのバリエーションの値です。

    char reserved[ 64 ]; //!< 予約領域です。
};

/**
* @brief コンパイル引数です。
*/
struct nngfxToolShaderCompilerCompileArg
{
    nngfxToolString vertexShaderSource; //!< 頂点シェーダーのソースです。
    nngfxToolString hullShaderSource; //!< ハルシェーダーのソースです。
    nngfxToolString domainShaderSource; //!< ドメインシェーダーのソースです。
    nngfxToolString geometryShaderSource; //!< ジオメトリーシェーダーのソースです。
    nngfxToolString pixelShaderSource; //!< ピクセルシェーダーのソースです。
    nngfxToolString computeShaderSource; //!< 演算シェーダーのソースです。

    NN_GFXTOOL_PTR64( nngfxToolReadFileCallback pReadIncludeFileCallback ); //!< インクルードファイルを読み込むためのコールバックです。
    NN_GFXTOOL_PTR64( void* pReadIncludeFileCallbackParam ); //!< インクルードファイルを読み込むためのコールバックに渡されるパラメーターです。

    int32_t shaderSourceFormat; //!< nngfxToolShaderCompilerShaderSourceFormat で表されるシェーダーソースのフォーマットです。
    char reserved2[ 4 ]; //!< 予約領域です。
    int32_t targetLowLevelApiType; //!< nngfxToolShaderCompilerLowLevelApiType で表される対象の低レベルグラフィックス API の種類です。
    int32_t targetLowLevelApiVersion; //!< 対象の低レベルグラフィックス API のバージョンです。0 を指定した場合はデフォルト値として考慮されます。
    char reserved3[ 4 ]; //!< 予約領域です。

    uint32_t variationCount; //!< バリエーションの数です。1 以上である必要があります。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationValueArg* pVariationValueArray ); //!< バリエーション値の引数の配列へのポインターです。NULL を指定した場合はバリエーションなしとして考慮されます。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationDefinitionArg* pVariationDefinitionArg ); //!< バリエーション定義の引数へのポインターです。NULL を指定した場合はバリエーションなしとして考慮されます。

    uint32_t optionCount; //!< オプションの数です。
    int32_t targetCodeType; //!< nngfxToolShaderCompilerCodeType で表される対象のコードのタイプです。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerCompileOption* pOptionArray ); //!< optionCount 個のオプションの配列へのポインターです。同じ種類のオプションを複数含めることはできません。

    NN_GFXTOOL_PTR64( nngfxToolWriteFileCallback pWriteDebugInfoFileCallback ); //!< デバッグ情報ファイルを書き込むためのコールバックです。
    NN_GFXTOOL_PTR64( void* pWriteDebugInfoFileCallbackParam ); //!< デバッグ情報ファイルを書き込むためのコールバックに渡されるパラメーターです。

    NN_GFXTOOL_PTR64( nngfxToolReadFileCallback pReadShaderCacheCallback ); //!< シェーダーキャッシュファイルを読み込むためのコールバックです。
    NN_GFXTOOL_PTR64( void* pReadShaderCacheCallbackParam ); //!< シェーダキャッシュファイルを読み込むためのコールバックに渡されるパラメーターです。
    NN_GFXTOOL_PTR64( nngfxToolWriteFileCallback pWriteShaderCacheCallback ); //!< シェーダーキャッシュファイルを書き込むためのコールバックです。
    NN_GFXTOOL_PTR64( void* pWriteShaderCacheCallbackParam ); //!< シェーダキャッシュファイルを書き込むためのコールバックに渡されるパラメーターです。

    char reserved[ 16 ]; //!< 予約領域です。
};

/**
* @brief シェーダースロット情報です。
*/
struct nngfxToolShaderCompilerShaderSlot
{
    int32_t vertexShaderSlot; //!< 頂点シェーダーにおけるスロット番号です
    int32_t hullShaderSlot; //!< ハルシェーダーにおけるスロット番号です
    int32_t domainShaderSlot; //!< ドメインシェーダーにおけるスロット番号です
    int32_t geometryShaderSlot; //!< ジオメトリーシェーダーにおけるスロット番号です
    int32_t pixelShaderSlot; //!< ピクセルシェーダーにおけるスロット番号です
    int32_t computeShaderSlot; //!< 演算シェーダーにおけるスロット番号です

    char reserved[ 8 ];
};

/**
* @brief 定数バッファーです。
*/
struct nngfxToolShaderCompilerConstantBuffer
{
    nngfxToolString name; //!< 名前です
    nngfxToolShaderCompilerShaderSlot shaderSlot; //!< シェーダースロットです
    uint32_t size; //!< バイトでのサイズです
    uint32_t activeVariableCount; //!< この定数バッファーに含まれる変数の数です
    int32_t stages; //!< nngfxToolShaderCompilerReflectionStageReference で表されるこの定数バッファーを参照しているステージです

    char reserved[ 28 ]; //!< 予約領域です
};

/**
* @brief アンオーダードアクセスバッファーです。
*/
struct nngfxToolShaderCompilerUnorderedAccessBuffer
{
    nngfxToolString name; //!< 名前です
    nngfxToolShaderCompilerShaderSlot shaderSlot; //!< シェーダースロットです
    uint32_t size; //!< バイトでのサイズです
    uint32_t activeVariableCount; //!< このアンオーダードアクセスバッファーに含まれる変数の数です
    int32_t stages; //!< nngfxToolShaderCompilerReflectionStageReference で表されるこのアンオーダードアクセスバッファーを参照しているステージです

    char reserved[ 28 ]; //!< 予約領域です
};

/**
* @brief シェーダー入力です。
*/
struct nngfxToolShaderCompilerShaderInput
{
    nngfxToolString name; //!< 名前です
    int32_t type; //!< 型です
    int32_t shaderSlot; //!< シェーダースロットです
    uint32_t arrayCount; //!< 配列数です
    int32_t stages; //!< nngfxToolShaderCompilerReflectionStageReference で表されるこのシェーダー入力を参照しているステージです

    char reserved[ 32 ]; //!< 予約領域です
};

/**
* @brief シェーダー出力です。
*/
struct nngfxToolShaderCompilerShaderOutput
{
    nngfxToolString name; //!< 名前です
    int32_t type; //!< 型です
    int32_t shaderSlot; //!< シェーダースロットです
    uint32_t arrayCount; //!< 配列数です
    int32_t stages; //!< nngfxToolShaderCompilerReflectionStageReference で表されるこのシェーダー出力を参照しているステージです

    char reserved[ 32 ]; //!< 予約領域です
};

/**
* @brief サンプラーです。
*/
struct nngfxToolShaderCompilerSampler
{
    nngfxToolString name; //!< 名前です
    int32_t type; //!< 型です
    nngfxToolShaderCompilerShaderSlot shaderSlot; //!< シェーダースロットです
    int32_t stages; //!< nngfxToolShaderCompilerReflectionStageReference で表されるこのサンプラーを参照しているステージです
    uint32_t arrayCount; //!< 配列数です

    char reserved[ 28 ]; //!< 予約領域です
};

/**
* @brief イメージです。
*/
struct nngfxToolShaderCompilerImage
{
    nngfxToolString name; //!< 名前です
    int32_t type; //!< 型です
    nngfxToolShaderCompilerShaderSlot shaderSlot; //!< シェーダースロットです
    int32_t stages; //!< nngfxToolShaderCompilerReflectionStageReference で表されるこのイメージを参照しているステージです
    uint32_t arrayCount; //!< 配列数です

    char reserved[ 28 ]; //!< 予約領域です
};

/**
* @brief セパレートテクスチャーです。
*/
struct nngfxToolShaderCompilerSeparateTexture
{
    nngfxToolString name; //!< 名前です
    int32_t type; //!< 型です
    nngfxToolShaderCompilerShaderSlot shaderSlot; //!< シェーダースロットです
    int32_t stages; //!< nngfxToolShaderCompilerReflectionStageReference で表されるこのテクスチャーを参照しているステージです
    uint32_t arrayCount; //!< 配列数です

    char reserved[ 28 ]; //!< 予約領域です
};

/**
* @brief セパレートサンプラーです。
*/
struct nngfxToolShaderCompilerSeparateSampler
{
    nngfxToolString name; //!< 名前です
    int32_t type; //!< 型です
    nngfxToolShaderCompilerShaderSlot shaderSlot; //!< シェーダースロットです
    int32_t stages; //!< nngfxToolShaderCompilerReflectionStageReference で表されるこのテクスチャーを参照しているステージです
    uint32_t arrayCount; //!< 配列数です

    char reserved[ 28 ]; //!< 予約領域です
};

/**
* @brief  定数バッファー内の変数です。
*/
struct nngfxToolShaderCompilerConstantBufferVariable
{
    nngfxToolString name; //!< 名前です
    int32_t type; //!< 型です
    int32_t blockIndex; //!< この変数を含んでいる定数バッファーのインデックスです
    int32_t offset; //!< バイトでの定数バッファーにおけるオフセットです
    uint32_t arrayCount; //!< 配列数です
    int32_t stages; //!< nngfxToolShaderCompilerReflectionStageReference で表されるこの変数を参照しているステージです

    char reserved[ 28 ]; //!< 予約領域です
};

/**
* @brief アンオーダードアクセスバッファー内の変数です。
*/
struct nngfxToolShaderCompilerUnorderedAccessBufferVariable
{
    nngfxToolString name; //!< 名前です
    int32_t type; //!< 型です
    int32_t blockIndex; //!< この変数を含んでいるアンオーダードアクセスバッファーのインデックスです
    int32_t offset; //!< バイトでのアンオーダードアクセスバッファーにおけるオフセットです
    uint32_t arrayCount; //!< 配列数です
    int32_t stages; //!< nngfxToolShaderCompilerReflectionStageReference で表されるこの変数を参照しているステージです

    char reserved[ 28 ]; //!< 予約領域です
};

/**
* @brief シェーダーのリフレクション情報です。
*/
struct nngfxToolShaderCompilerShaderReflection
{
    uint32_t constantBufferCount; //!< 定数バッファーの数です
    uint32_t unorderedAccessBufferCount; //!< アンオーダードアクセスバッファーの数です
    uint32_t shaderInputCount; //!< シェーダー入力の数です
    uint32_t shaderOutputCount; //!< シェーダー出力の数です
    uint32_t samplerCount; //!< サンプラーの数です
    uint32_t constantBufferVariableCount; //!< 定数バッファーの数です
    uint32_t unorderedAccessBufferVariableCount; //!< アンオーダードアクセスバッファーの数です
    uint32_t imageCount; //!< イメージの数です

    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerConstantBuffer* pConstantBufferArray ); //!< 定数バッファーの配列へのポインターです
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerUnorderedAccessBuffer* pUnorderedAccessBufferArray ); //!< アンオーダードアクセスバッファーの配列へのポインターです
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerShaderInput* pShaderInputArray ); //!< シェーダー入力の配列へのポインターです
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerShaderOutput* pShaderOutputArray ); //!< シェーダー出力の配列へのポインターです
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerSampler* pSamplerArray ); //!< サンプラーの配列へのポインターです
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerConstantBufferVariable* pConstantBufferVariableArray ); //!< 定数バッファー内の変数の配列へのポインターです
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerUnorderedAccessBufferVariable* pUnorderedAccessBufferVariableArray ); //!< アンオーダードアクセスバッファー内の変数の配列へのポインターです

    uint32_t computeWorkGroupSizeX; //!< 演算シェーダーのワークグループの X 次元のサイズです。
    uint32_t computeWorkGroupSizeY; //!< 演算シェーダーのワークグループの Y 次元のサイズです。
    uint32_t computeWorkGroupSizeZ; //!< 演算シェーダーのワークグループの Z 次元のサイズです。
    char reserved2[ 4 ]; //!< 予約領域です。

    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerImage* pImageArray ); //!< イメージの配列へのポインターです

    uint32_t separateTextureCount; //!< セパレートテクスチャーの数です
    uint32_t separateSamplerCount; //!< セパレートサンプラーの数です

    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerSeparateTexture* pSeparateTextureArray ); //!< セパレートテクスチャーの配列へのポインターです
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerSeparateSampler* pSeparateSamplerArray ); //!< セパレートサンプラーの配列へのポインターです

    char reserved[ 16 ]; //!< 予約領域です
};

/**
* @brief コンパイルオプションによって追加される出力です。
*/
struct nngfxToolShaderCompilerOptionOutput
{
    uint32_t optionOutputType; //!< nngfxToolShaderCompilerOptionOutputType で表されるオプションの種類です。
    char reserved2[ 4 ]; //!< 予約領域です。
    NN_GFXTOOL_PTR64( void* pOutput ); //!< 出力内容へのポインターです。

    char reserved[ 16 ]; //!< 予約領域です。
};

/**
* @brief ステージごとに追加される出力です。
*/
struct nngfxToolShaderCompilerOptionOutputStageCommon
{
    nngfxToolString infoLog; //!< コンパイル情報ログです。
    nngfxToolString dump; //!< ダンプ情報です。
    NN_GFXTOOL_PTR64( void* pShaderStatistics ); //!< 統計情報です。詳細は低レベルシェーダコンパイラーを参照してください。

    char reserved[ 32 ]; //!< 予約領域です。
};

/**
* @brief nngfxToolShaderCompilerOptionOutputType_ProgramCommon に対応するプログラムごとに追加される出力です。
*/
struct nngfxToolShaderCompilerOptionOutputProgramCommon
{
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerOptionOutputStageCommon* pVertexStageOutput ); //!< 頂点シェーダーの追加出力です
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerOptionOutputStageCommon* pHullStageOutput ); //!< ハルシェーダーの追加出力です
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerOptionOutputStageCommon* pDomainStageOutput ); //!< ドメインシェーダーの追加出力です
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerOptionOutputStageCommon* pGeometryStageOutput ); //!< ジオメトリーシェーダーの追加出力です
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerOptionOutputStageCommon* pPixelStageOutput ); //!< ピクセルシェーダーの追加出力です
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerOptionOutputStageCommon* pComputeStageOutput ); //!< 演算シェーダーの追加出力です

    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerShaderReflection* pReflection ); //!< シェーダーリフレクション情報です

    char reserved[ 32 ]; //!< 予約領域です。
};

/**
* @brief シェーダープログラムの出力です。
*/
struct nngfxToolShaderCompilerShaderProgramOutput
{
    NN_GFXTOOL_PTR64( void* nngfxShaderInfoData ); //!< nn::gfx::ShaderInfo::DataType 型のシェーダー情報へのポインターです。

    uint32_t optionOutputCount; //!< コンパイルオプションによって追加される出力の数です。
    char reserved2[ 4 ]; //!< 予約領域です。
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerOptionOutput* pOptionOutputArray ); //!< コンパイルオプションによって追加される出力の配列へのポインターです。

    char reserved[ 32 ]; //!< 予約領域です。
};

/**
* @brief バリエーションの出力です。
*/
struct nngfxToolShaderCompilerVariationOutput
{
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerShaderProgramOutput* pSourceOutput ); //!< ソースタイプのシェーダープログラムの出力です。
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerShaderProgramOutput* pIntermediateLanguageOutput ); //!< 中間言語タイプのシェーダープログラムの出力です。
    NN_GFXTOOL_PTR64( nngfxToolShaderCompilerShaderProgramOutput* pBinaryOutput ); //!< バイナリータイプのシェーダープログラムの出力です。

    char reserved[ 32 ]; //!< 予約領域です。
};

/**
* @brief コンパイル結果の出力です。
*/
struct nngfxToolShaderCompilerCompileOutput
{
    uint32_t compilerVersion; //!< コンパイラーのバージョンです。
    uint32_t variationCount; //!< バリエーションの出力の数です。
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerVariationOutput* pVariationOutputArray ); //!< バリエーションの出力の配列へのポインターです。
    uint64_t lowLevelCompilerVerison; //!< 低レベルコンパイラーのバージョンです。

    char reserved[ 64 ]; //!< 予約領域です。
};

/**
* @brief シェーダーコンパイラーのハンドルを作成します。
*
* @param[out] pOutHandle 作成されるハンドルを格納するための変数へのポインター
* @param[in] pArg ハンドル作成の引数へのポインター
*
* @pre
* - pOutHandle != NULL
* - pArg != NULL
*
* @post
* - *pOutHandle に作成されたハンドルが格納されている
*
* @return 結果コードを返します。
*/
NN_GFXTOOL_SHADERCOMPILER_API nngfxToolResultCode nngfxToolShaderCompilerCreateHandle(
    nngfxToolHandle* pOutHandle, const nngfxToolShaderCompilerCreateHandleArg* pArg );

/**
* @brief シェーダコンパイラーのハンドルをリセットし再利用できる状態にします。
*
* @param[in] handle シェーダコンパイラーのハンドル
*
* @pre
* - handle の値が作成されたハンドルである
* - handle が削除されていない
*
* @post
* - handle がリセットされている
*
* @return 結果コードを返します。
*/
NN_GFXTOOL_SHADERCOMPILER_API nngfxToolResultCode nngfxToolShaderCompilerResetHandle(
    nngfxToolHandle handle );

/**
* @brief シェーダーをコンパイルします。
*
* @param[out] ppOutput コンパイル結果の出力へのポインターを格納するためのポインターへのポインター
* @param[in] handle シェーダーコンパイラーのハンドル
* @param[in] pArg コンパイル引数へのポインター
*
* @pre
* - ppOutput != NULL
* - pArg != NULL
* - handle の値が作成されたハンドルである
* - handle が削除されていない
* - handle に対してコンパイルしていない
*
* @post
* - *ppOutput にコンパイル結果の出力へのポインターが格納されている
*
* @return 結果コードを返します。
*/
NN_GFXTOOL_SHADERCOMPILER_API nngfxToolResultCode nngfxToolShaderCompilerCompile(
    nngfxToolShaderCompilerCompileOutput** ppOutput, nngfxToolHandle handle,
    const nngfxToolShaderCompilerCompileArg* pArg );

/**
* @brief ハンドルを削除します。
*
* @param[in] handle 削除するシェーダーコンパイラーのハンドル
*
* @pre
* - handle の値が作成されたハンドルである
* - handle が削除されていない
*
* @post
* - handle が削除されている
*
* @return 結果コードを返します。
*/
NN_GFXTOOL_SHADERCOMPILER_API nngfxToolResultCode nngfxToolShaderCompilerDeleteHandle(
    nngfxToolHandle handle );

/**
* @brief コンパイル結果の出力を削除します。
*
* @param[in] pOutput 削除するコンパイル結果へのポインター
*
* @pre
* - pOutput != NULL
* - pOutput がコンパイル結果の出力である
* - pOutput が削除されていない
*
* @post
* - pOutput が削除されている
*
* @return 結果コードを返します。
*/
NN_GFXTOOL_SHADERCOMPILER_API nngfxToolResultCode nngfxToolShaderCompilerDeleteCompileOutput(
    nngfxToolShaderCompilerCompileOutput* pOutput );

/**
* @brief シェーダーコンパイラーのログ出力ストリームを設定します。
*
* @param[in] pArg ログ出力ストリーム設定引数へのポインター
*
* @pre
* - pArg != NULL
*
* @post
* - ログ出力ストリームが設定されている
*
* @return 結果コードを返します。
*
* @details
* NULL のストリームを設定すると、ログ出力されなくなります。
*/
NN_GFXTOOL_SHADERCOMPILER_API nngfxToolResultCode nngfxToolShaderCompilerSetLogStream(
    const nngfxToolSetLogStreamArg* pArg );

/**
* @brief シェーダーコンパイラーの API バージョンを取得します。
*
* @param[out] pOutApiVersion 出力される API バージョンを格納するための変数へのポインタ
*
* @return 結果コードを返します。
*
* @details
* 結果コードが nngfxToolResultCode_Succeeded のとき、pOutApiVersion の major, minor, micro メンバーは、API バージョンにかかわらず正しい値が格納されていることが保証されます。
*/
NN_GFXTOOL_SHADERCOMPILER_API nngfxToolResultCode nngfxToolShaderCompilerGetApiVersion(
    nngfxToolApiVersion* pOutApiVersion );
