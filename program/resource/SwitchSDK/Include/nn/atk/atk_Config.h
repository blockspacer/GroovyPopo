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

#include <nn/nn_SdkLog.h>
#include <nn/nn_SdkAssert.h>
#include <nn/atk/fnd/basis/atkfnd_Config.h>

#define NN_ATK_CONFIG_ENABLE_APPFRAMECALLBACK
#define NN_ATK_CONFIG_ENABLE_MULTICHEFT
#define NN_ATK_CONFIG_ENABLE_6CHAAC
//#define NN_ATK_CONFIG_ENABLE_SOUND2

#if defined(NN_SDK_BUILD_DEBUG) || defined(NN_SDK_BUILD_DEVELOP)
#define NN_ATK_CONFIG_ENABLE_DEV
#endif

#if defined(NN_ATK_FND_CONFIG_ENABLE_THREAD_CORE_NUMBER_OBSERVATION)
#define NN_ATK_CONFIG_ENABLE_THREAD_CORE_NUMBER_OBSERVATION
#endif

namespace nn {
namespace atk {
namespace detail {

#ifndef _POSITION_T_DEFINED
#ifdef _POSITION_T_FND_DEFINED
    typedef fnd::position_t position_t;
    #define _POSITION_T_DEFINED
#endif
#endif

static const int StreamChannelCount = 16;
static const uint32_t StreamTrackCount = 8;
static const uint32_t StreamChannelCountPerTrack = 2;
static const int StreamDataLoadTaskMax = 32;

// 1 ブロックのマージン [サンプル数]
// (48KHz の波形が 8 倍のピッチで 3ms 以内に再生できるサンプル数)
//   48KHz : ユーザーが利用するであろう最大サンプリングレート。
//   8倍   : AX のボイスに設定できる最大ピッチ。
//   3ms   : AX 割り込み周期。
static const size_t DataBlockSizeMarginSamples = 48000 * 8 * 3 / 1000;

} // namespace detail
} // namespace atk
} // namespace nn
