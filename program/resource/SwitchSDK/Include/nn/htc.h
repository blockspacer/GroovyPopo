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
*   @brief  ホスト・ターゲット間通信に関する API の宣言
*/

#pragma once

/**
*   @namespace  nn::htc
*   @brief      ホスト・ターゲット間通信の名前空間です。
*
*               ### 機能概要
*               HTC とは、Host Target Communication の略です。@n
*               HTC ライブラリでは、ホストとやり取りを行う手段を提供します。@n
*               HTC ライブラリの機能はビルドタイプ Release の場合、標準では使用できないようになっています。@n
*               ビルドタイプ Release で使用したい場合は NN_ENABLE_HTC を定義するようにしてください。@n
*
*               #### 使用方法
*               HTC ライブラリの機能を使うためにはあらかじめ TargetManager.exe を起動しておく必要があります。@n
*               詳細な接続手段についてはマニュアルを参照してください。@n
*/

#include <nn/htc/detail/htc_BuildSwitch.h>

#if defined(NN_BUILD_CONFIG_HTC_ENABLED)
#include <nn/htc/htc_Api.h>
#include <nn/htc/htc_Result.h>
#endif
