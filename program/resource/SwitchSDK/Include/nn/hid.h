/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/*
 * @file
 * @brief       ヒューマンインターフェイスデバイスに関する API の宣言
 */

#pragma once

/**
 * @namespace   nn::hid
 * @brief       ヒューマンインターフェイスデバイスの名前空間です。
 */

#include <nn/hid/hid_Result.h>
#include <nn/hid/hid_DebugPad.h>
#include <nn/hid/hid_Gesture.h>
#include <nn/hid/hid_Keyboard.h>
#include <nn/hid/hid_Mouse.h>
#include <nn/hid/hid_Npad.h>
#include <nn/hid/hid_TouchScreen.h>

#if defined(NN_BUILD_CONFIG_SPEC_NX)
#include <nn/hid/hid_Nx.h>
#endif
