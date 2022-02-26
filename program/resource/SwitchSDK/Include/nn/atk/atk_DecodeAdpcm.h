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

#include <nn/atk/atk_Adpcm.h>
#include <nn/atk/atk_Config.h>

namespace nn {
namespace atk {
namespace detail {

void DecodeDspAdpcm(
        position_t playPosition,                   // adpcmData から数えたデコード開始位置
        nn::atk::AdpcmContext& context,     // デコード用 ADPCM コンテキスト
        const nn::atk::AdpcmParam& param,   // デコード用 ADPCM パラメータ
        const void* adpcmData,              // ADPCM エンコードデータ
        size_t decodeSamples,                  // デコードサンプル数
        int16_t* dest                           // デコードデータ出力先
) NN_NOEXCEPT;

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

