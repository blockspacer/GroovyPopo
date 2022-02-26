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
 * @brief   ハンドルを扱う API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/os/os_NativeHandleTypes.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

//! @name ハンドル関連共通 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   指定されたハンドルをクローズします。
 *
 * @param[in] handle    クローズするハンドル
 *
 * @pre
 *  - handle が有効か、もしくは nn::os::InvalidNativeHandle である
 *
 * @details
 *  指定されたハンドルをクローズします。@n
 *  handle が nn::os::InvalidNativeHandle だった場合、何もせずにリターンします。
 *
 *  ハンドルは通常、 CreateSystemEvent() 等のオブジェクト初期化時に
 *  システムから取得し、オブジェクトの中で管理されるものです。ただし、
 *  DetachSystemEvent() 等でハンドルをオブジェクトから切り離し、その後
 *  不要になったハンドルがある場合には本 API でクローズするようにして下さい。
 *
 *  本 API は、ハンドルによるカーネルオブジェクトの参照カウントや、
 *  各 OS オブジェクトでの管理状態を正しく把握した上で使用する必要があるため、
 *  主に SDK 開発者が利用することを想定しています。
 *
 */
void CloseNativeHandle(NativeHandle handle) NN_NOEXCEPT;

//! @}


NativeHandle GetCurrentProcessHandle() NN_NOEXCEPT;

}} // namespace nn::os

