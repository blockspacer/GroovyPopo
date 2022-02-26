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
 * @brief   PWM ドライバライブラリで使用するチャンネル識別子を切り替えます。
 */

#pragma once

#include <nn/TargetConfigs/build_Base.h>

// 対応プラットフォームが増えた場合は切り替えをここで行う
#if (defined(NN_BUILD_CONFIG_SPEC_GENERIC))
    #include <nn/pcm/pcm_MeasuringPoint-spec.generic.h>
#elif (defined(NN_BUILD_CONFIG_SPEC_NX))
    #include <nn/pcm/pcm_MeasuringPoint-spec.nx.h>
#else
    #error NN_BUILD_CONFIG_SPEC_ is not defined
#endif
