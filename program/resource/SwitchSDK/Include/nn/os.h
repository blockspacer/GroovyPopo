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
 * @brief   オペレーティングシステム に関する API の宣言
 */


#pragma once


/**
 * @namespace   nn::os
 * @brief       オペレーティングシステムライブラリの名前空間です。
 */

#include <nn/os/os_Result.h>

#include <nn/os/os_Argument.h>
#include <nn/os/os_Barrier.h>
#include <nn/os/os_Base.h>
#include <nn/os/os_Cache.h>
#include <nn/os/os_ConditionVariable.h>
#include <nn/os/os_Debug.h>
#include <nn/os/os_DefaultUserExceptionHandler.h>
#include <nn/os/os_DyingMessage.h>
#include <nn/os/os_Event.h>
#include <nn/os/os_Fiber.h>
#include <nn/os/os_InvestigationApiForDebug.h>
#include <nn/os/os_LightEvent.h>
#include <nn/os/os_MemoryAllocatorForThreadLocal.h>
#include <nn/os/os_MemoryFence.h>
#include <nn/os/os_MemoryHeap.h>
#include <nn/os/os_MemoryPermission.h>
#include <nn/os/os_MessageQueue.h>
#include <nn/os/os_MultipleWait.h>
#include <nn/os/os_Mutex.h>
#include <nn/os/os_NativeHandle.h>
#include <nn/os/os_Random.h>
#include <nn/os/os_ReaderWriterLock.h>
#include <nn/os/os_Semaphore.h>
#include <nn/os/os_SystemEvent.h>
#include <nn/os/os_Thread.h>
#include <nn/os/os_ThreadLocalStorage.h>
#include <nn/os/os_Tick.h>
#include <nn/os/os_TimerEvent.h>
#include <nn/os/os_UserExceptionHandler.h>
#include <nn/os/os_VirtualAddressMemory.h>
