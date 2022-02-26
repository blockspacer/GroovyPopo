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
* @brief サンプラー初期化用の情報のデータに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

namespace nn {
namespace gfx {

/**
* @brief サンプラーを初期化するための情報を表す構造体です。
*/
struct SamplerInfoData
{
    Bit8 addressU; //!< nn::gfx::TextureAddressMode で表される、U 座標のアドレスモードです
    Bit8 addressV; //!< nn::gfx::TextureAddressMode で表される、V 座標のアドレスモードです
    Bit8 addressW; //!< nn::gfx::TextureAddressMode で表される、W 座標のアドレスモードです
    Bit8 comparisonFunction; //!< nn::gfx::ComparisonFunction で表される、比較関数です
    Bit8 borderColorType; //!< nn::gfx::BorderColorType で表される、ボーダーカラータイプです
    uint8_t maxAnisotropy; //!< 異方性フィルタリングの最大サンプリング数です
    Bit16 filterMode; //!< nn::gfx::FilterMode で表される、フィルターモードです
    float minLod; //!< ミップマップ範囲の下限値です
    float maxLod; //!< ミップマップ範囲の上限値です
    float lodBias; //!< ミップマップレベルのオフセット値です

    char reserved[ 12 ]; //!< 予約領域です
};

}
}
