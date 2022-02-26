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
*   @brief  ホスト・ターゲット間ソケット通信に関する API の宣言
*/

#pragma once

/**
*   @namespace  nn::htcs
*   @brief      ホスト・ターゲット間ソケット通信の名前空間です。
*
*               ### 機能概要
*               HTCS とは、Host Target Communication Socket の略です。@n
*               HTCS ライブラリでは、ソケットインターフェイスによるホストとターゲット間の通信手段を提供します。@n
*               HTCS ライブラリの機能はビルドタイプ Release の場合、標準では使用できないようになっています。@n
*               ビルドタイプ Release で使用したい場合は NN_ENABLE_HTC を定義するようにしてください。@n
*
*               #### 使用方法
*               ターゲット側での基本的な使用手順はソケットと同じです。@n
*               ホスト側では Winsock などのソケットインターフェイスを使用してください。@n
*               またホストとの接続を行うためにはあらかじめ TargetManager.exe を起動しておく必要があります。@n
*               詳細な接続手段についてはマニュアルの接続シーケンスの項を参照してください@n
*/

#include <nn/htc/detail/htc_BuildSwitch.h>

#if defined(NN_BUILD_CONFIG_HTC_ENABLED)
#include <nn/htcs/htcs_Types.h>
#include <nn/htcs/htcs_Result.h>
#include <nn/htcs/htcs_Library.h>
#include <nn/htcs/htcs_Socket.h>
#endif
