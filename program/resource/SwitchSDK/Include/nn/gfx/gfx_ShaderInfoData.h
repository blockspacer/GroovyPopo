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
* @brief シェーダー初期化用の情報のデータに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

#include <nn/util/util_BitPack.h>

#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief シェーダーを初期化するための情報を表す構造体です。
*/
struct ShaderInfoData
{
    enum
    {
        Flag_SeparationEnable,
        Flag_ResShader
    };

    nn::util::BitPack8 flags; //!< フラグです
    Bit8 codeType; //!< nn::gfx::ShaderCodeType で表される、コードタイプです
    Bit8 sourceFormat; //!< nn::gfx::ShaderSourceFormat で表される、ソースフォーマットです
    char reserved2; //!< 予約領域です。
    Bit32 binaryFormat; //!< バイナリーフォーマットです
    detail::Ptr< const void > pVertexShaderCode; //!< 頂点シェーダーコードへのポインターです
    detail::Ptr< const void > pHullShaderCode; //!< ハルシェーダーコードへのポインターです
    detail::Ptr< const void > pDomainShaderCode; //!< ドメインシェーダーコードへのポインターです
    detail::Ptr< const void > pGeometryShaderCode; //!< ジオメトリーシェーダーコードへのポインターです
    detail::Ptr< const void > pPixelShaderCode; //!< ピクセルシェーダーコードへのポインターです
    detail::Ptr< const void > pComputeShaderCode; //!< 演算シェーダーコードへのポインターです

    char reserved[ 40 ]; //!< 予約領域です
};

/**
* @brief シェーダーコードを表す構造体です。
*/
struct ShaderCode
{
    uint32_t codeSize; //!< バイトでのコードサイズです
    uint32_t decompressedCodeSize; //!< nn::util::DecompressZlib による解凍後のバイトでのコードサイズです。pCode が圧縮されていない場合は 0 です。
    detail::Ptr< const void > pCode; //!< コードへのポインターです

    char reserved[ 16 ]; //!< 予約領域です
};

/**
* @brief ソース配列のコードを表す構造体です。コードタイプが nn::gfx::ShaderCodeType_SourceArray のシェーダーのために使います。
*/
struct SourceArrayCode
{
    /**
    * @brief コードの配列の最大長です。
    */
    static const int MaxCodeArrayLength = 64;

    uint16_t codeArrayLength; //!< コードの配列の長さです
    char reserved2[ 6 ]; //!< 予約領域です。
    detail::Ptr< const uint32_t > pCodeSizeArray; //!< コードサイズの配列へのポインターです
    detail::Ptr< detail::Ptr< const void > > pCodePtrArray; //!< コードへのポインターの配列へのポインターです

    char reserved[ 8 ]; //!< 予約領域です
};

}
}
