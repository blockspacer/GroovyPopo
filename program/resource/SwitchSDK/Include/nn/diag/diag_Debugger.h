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

#include <nn/nn_Common.h>

/**
*   @file
*   @brief  デバッガ関連の API 宣言
*/

#if !defined(NN_BUILD_CONFIG_OS_WIN32) && !defined(NN_BUILD_CONFIG_OS_HORIZON)
    #error "未サポートの OS 種別が指定されています。"
#endif

namespace nn { namespace diag {

/**
*   @brief  デバッガにアタッチされているか判定します。
*
*   @return      判定結果を返します。
*   @retval      true    デバッガにアタッチされています。
*   @retval      false   デバッガにアタッチされていません。
*/
bool IsDebuggerAttached() NN_NOEXCEPT;

}} // nn::diag
