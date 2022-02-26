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
 * @brief       オーディオライブラリの API を宣言します。
 */

#pragma once

/**
 * @namespace   nn::audio
 * @brief       オーディオライブラリの名前空間です。
 */

#include <nn/audio/audio_Adpcm.h>
#include <nn/audio/audio_AudioIn.h>
#include <nn/audio/audio_AudioOut.h>
#include <nn/audio/audio_AudioRenderer.h>
#include <nn/audio/audio_Common.h>
#include <nn/audio/audio_Device.h>
#include <nn/audio/audio_Effect.h>
#include <nn/audio/audio_FinalMix.h>
#include <nn/audio/audio_MemoryPool.h>
#include <nn/audio/audio_PerformanceMetrics.h>
#include <nn/audio/audio_PolyphaseResampler.h>
#include <nn/audio/audio_Resampler.h>
#include <nn/audio/audio_Result.h>
#include <nn/audio/audio_SampleFormat.h>
#include <nn/audio/audio_Sink.h>
#include <nn/audio/audio_Splitter.h>
#include <nn/audio/audio_SubMix.h>
#include <nn/audio/audio_Voice.h>
#include <nn/audio/audio_WaveBuffer.h>
