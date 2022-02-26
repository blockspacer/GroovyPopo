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
 * @brief   ハンドル関連の型や定数の宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Common.h>

namespace nn { namespace os {

#if defined(NN_BUILD_CONFIG_OS_HORIZON) || \
    defined(NN_BUILD_FOR_DOCUMENT_GENERATION)
//--------------------------------------------------------------------------
/**
 * @brief   システムが管理しているハンドルを表す型です。
 *
 * @details
 *  システムが管理しているハンドルを扱う場合に使用する型です。@n
 *  通常は一部の SDK 開発者のみが使用します。
 *
 * @platformbegin{Windows}
 *  Windows 環境では uint32_t ではなく void* に typedef されています。
 * @platformend
 *
 * 値を直接変更することは禁止です。 @n
 * 次の関数を介して利用してください。
 *
 * - @ref CloseNativeHandle()
 *
 * 上記の関数を利用するには nn/os/os_NativeHandleTypes.h をインクルードする必要があります。
 */
typedef uint32_t    NativeHandle;
#elif defined(NN_BUILD_CONFIG_OS_WIN32)
typedef void*       NativeHandle;
#endif

//--------------------------------------------------------------------------
/**
 * @brief   無効なハンドルであることを表す定数です。
 */
const NativeHandle InvalidNativeHandle = 0;

//--------------------------------------------------------------------------

}} // namespace nn::os

