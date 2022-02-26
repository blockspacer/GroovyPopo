/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

//! @cond   SECTION_selectable_DeviceDriverKit

/**
 * @file
 * @brief   SystemEvent に関する API の宣言
 * @details このヘッダに記載の API はアプリケーション開発者は直接利用することができません。
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/os/os_NativeHandleTypes.h>
#include <nn/os/os_EventCommon.h>


//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

struct  SystemEventType;

//! @name システムイベント関連 API
//! @{

Result CreateSystemEvent(SystemEventType* event, EventClearMode clearMode, bool interProcess) NN_NOEXCEPT;

void AttachSystemEvent(SystemEventType* event, NativeHandle readableHandle, bool isReadableHandleManaged, NativeHandle writableHandle, bool isWritableHandleManaged, EventClearMode clearMode) NN_NOEXCEPT;

void AttachReadableHandleToSystemEvent(SystemEventType* event, NativeHandle readableHandle, bool isReadableHandleManaged, EventClearMode clearMode) NN_NOEXCEPT;

void AttachWritableHandleToSystemEvent(SystemEventType* event, NativeHandle writableHandle, bool isWritableHandleManaged, EventClearMode clearMode) NN_NOEXCEPT;

NativeHandle DetachReadableHandleOfSystemEvent(SystemEventType* event) NN_NOEXCEPT;

NativeHandle DetachWritableHandleOfSystemEvent(SystemEventType* event) NN_NOEXCEPT;

void SignalSystemEvent(SystemEventType* event) NN_NOEXCEPT;

NativeHandle GetReadableHandleOfSystemEvent(const SystemEventType* event) NN_NOEXCEPT;

NativeHandle GetWritableHandleOfSystemEvent(const SystemEventType* event) NN_NOEXCEPT;

//! @}

}} // namespace nn::os

//! @endcond
