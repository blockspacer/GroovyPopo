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
 * :include nn/atk/atk_Adpcm.h
 *
 * @file atk_Adpcm.h
 */

#pragma once

#include <nn/audio.h>

namespace nn {
namespace atk {

typedef nn::audio::AdpcmParameter AdpcmParam;

// 配列で持った時にアライメントが揃うように構造体をラップ
#if defined(NN_BUILD_CONFIG_COMPILER_VC)
NN_AUDIO_ALIGNAS_BUFFER_ALIGN struct AdpcmContext
{
    nn::audio::AdpcmContext audioAdpcmContext;
};
#else
struct AdpcmContext
{
    nn::audio::AdpcmContext audioAdpcmContext;
} NN_AUDIO_ALIGNAS_BUFFER_ALIGN;
#endif
struct AdpcmContextNotAligned
{
    nn::audio::AdpcmContext audioAdpcmContext;
};

} // namespace nn::atk
} // namespace nn
