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
* @brief シェーダーのデータに関する API の宣言（NVN8 特殊版）
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.nvn.h>
#include <nn/gfx/gfx_ShaderInfoData.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {

/**
* @brief シェーダーを表す構造体です（NVN8 特殊版）
*/
template<>
struct ShaderImplData< ApiVariationNvn8 >
{
    enum State
    {
        State_NotInitialized,
        State_Initialized
    };

    enum Flag
    {
        Flag_SeparationEnable = ShaderInfoData::Flag_SeparationEnable,
        Flag_ResShader = ShaderInfoData::Flag_ResShader,
        Flag_Shared
    };

    Bit8 state; //!< オブジェクトの状態です。
    nn::util::BitPack8 flags; //!< フラグです
    char reserved[ 2 ]; //!< 予約領域です。
    Bit32 nvnShaderStageBits; //!< NVNshaderStageBits で表される、シェーダーステージのセットです。

    detail::Ptr< void > pNvnProgram;
    NN_ALIGNAS( 8 ) char nvnProgram[ 192 ];

    detail::Ptr< const void > pReflection; //!< シェーダーリフレクション情報です

    detail::Ptr< void > pOnlineCompiledShader;

    detail::Ptr< void > userPtr; //!< ユーザーポインターです
};

}
}
