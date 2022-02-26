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
*   @file
*   @brief  SDEV を利用したデバッグ機能に関する API
*/

#pragma once

#include<nn/nn_Common.h>
#include<nn/nn_Result.h>
#include<nn/os/os_SystemEvent.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------
namespace nn { namespace pl {

/**
*   @brief      USB インターフェースの使用の可否を指定します。
*
*   @param[in]  bDetached       USB 使用の可否
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess            成功しました。
*   @retval     ResultNotSupportedDevice この機能がサポートされていない機材です。
*
*   @details    bDetached に true を指定し、USB インターフェースの使用を許可しない設定にすると、USB インターフェースに何か接続されていても接続されていないものとして動作します。
*               SDEV の電源投入後は bDetached に false が指定された設定で動作します。この設定は SDEV の電源が切断されたり、SDEV の初期化が行われたりしない限り持続します。
*/
Result SetUsbDetachedForDebug(bool bDetached) NN_NOEXCEPT;

}}

