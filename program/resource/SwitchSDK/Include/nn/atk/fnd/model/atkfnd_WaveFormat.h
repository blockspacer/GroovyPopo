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

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief  波形フォーマット情報を格納します。
//---------------------------------------------------------------------------
struct WaveFormat
{
    WaveFormat() NN_NOEXCEPT :
        channels(0),
        bitsPerSample(0),
        samplesPerSec(0)
    { }

    //---------------------------------------------------------------------------

    uint8_t channels;
    uint8_t bitsPerSample;
    uint8_t padding[2];
    uint32_t samplesPerSec;
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
