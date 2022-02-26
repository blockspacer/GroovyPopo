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
 * @brief   アプリケーションの動作環境に関するヘッダファイル
 */

#pragma once

/**
 * @namespace   nn::oe
 * @brief   アプリケーションの動作環境操作ライブラリに関する名前空間です。
 *
 * @details
 *  ### 機能概要
 *  oe とは、Operating Environment の略です。
 *  oe ライブラリはアプリケーションが自身のプログラムの動作環境について、
 *  プログラムの状態遷移を制御したり、
 *  性能のコンフィグレーションを行なったりする機能を提供します。
 *
 */

#include <nn/oe/oe_Common.h>
#include <nn/oe/oe_DisplayResolutionApis.h>
#include <nn/oe/oe_NotificationMessage.h>
#include <nn/oe/oe_OperationModeApis.h>
#include <nn/oe/oe_ApplicationControlApis.h>
#include <nn/oe/oe_LibraryAppletControlApis.h>
#include <nn/oe/oe_SelfControlApis.h>
#include <nn/oe/oe_Performance.h>
#include <nn/oe/oe_IdlePolicyApi.h>
#include <nn/oe/oe_PseudoDeviceIdApis.h>
#include <nn/oe/oe_Result.h>
#include <nn/oe/oe_CrashReportPolicyApi.h>
#include <nn/oe/oe_InStoreDemoApis.h>
#include <nn/oe/oe_MultiProgramApis.h>
