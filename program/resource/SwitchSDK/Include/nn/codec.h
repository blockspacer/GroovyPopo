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

/**
 * @file
 * @brief       オーディオおよびビデオに関する圧縮コーデックライブラリの API を宣言します。
 */

/**
 * @namespace   nn::codec
 * @brief       オーディオおよびビデオに関する圧縮コーデックライブラリの名前空間です。
 */

#include <nn/codec/codec_AacCommon.h>
#include <nn/codec/codec_AacDecoder.h>
#include <nn/codec/codec_AdpcmCommon.h>
#include <nn/codec/codec_AdpcmDecoder.h>
#include <nn/codec/codec_HardwareOpusDecoder.h>
#include <nn/codec/codec_HardwareOpusMultiStreamDecoder.h>
#include <nn/codec/codec_OpusCommon.h>
#include <nn/codec/codec_OpusDecoder.h>
#include <nn/codec/codec_OpusEncoder.h>
#include <nn/codec/codec_OpusHeader.h>
#include <nn/codec/codec_OpusMultiStreamDecoder.h>
#include <nn/codec/codec_OpusPacket.h>
#include <nn/codec/codec_OpusUtility.h>
