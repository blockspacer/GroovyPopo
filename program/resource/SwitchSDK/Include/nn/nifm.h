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
 *  @file
 *  @brief  ネットワーク接続管理に関する API の宣言
 *
 *  :include nn/nifm.h
 */


#pragma once


/**
 *  @namespace  nn::nifm
 *  @brief      ネットワーク接続管理ライブラリの名前空間です。
 *
 *              ### 機能概要
 *              ネットワーク接続管理ライブラリは、アプリケーションやシステムの要求を調停し、使用するネットワークインターフェースの選択やネットワーク接続の確立、利用の可否判断などを提供します。
 *              アプリケーションがネットワーク接続を利用する場合は、このライブラリを通じて利用要求を提出することにより、アプリケーションの利用に適した接続を確立したり、利用ができなかった場合はその理由を取得したりすることができます。@n
 *
 *              ### 共通の事前条件
 *              ネットワーク接続管理ライブラリが提供する API を利用するには、個別に明示されていないかぎり、利用前に @ref nn::nifm::Initialize 関数によってライブラリを初期化する必要があります。@n
 *
 *              ### ネットワーク接続設定の登録
 *              デバイスをネットワークに接続するためには、事前に設定マネージャによりネットワーク接続設定を登録する必要があります。@n
 *              詳しくは設定マネージャのマニュアルを参照してください。@n
 *
 *              ### エラーハンドリングについて
 *              ネットワーク利用要求を提出後にエラーハンドリング関数を呼び出すことによって、要求提出の結果に応じた処理を行うシーケンスに遷移します。
 *              詳細は nn::nifm::HandleNetworkRequestErrorResult の項を参照してください。
 *
 *              ### Windows 上での挙動と制限
 *              Windows 上で動作させた場合、ライブラリを通じたネットワーク利用要求の提出によって Windows のネットワーク接続や設定を切り替えることはありません。@n
 *              また、このライブラリは内部で fs ライブラリの機能を使用します。
 *              nn::fs::SetAllocator() で設定したアロケータか malloc() によって fs ライブラリがメモリを確保できる状態で利用してください。
 *              詳細は fs ライブラリのマニュアルを参照してください。
 */

#include <nn/nifm/nifm_Common.h>

#include <nn/nifm/nifm_Api.h>
#include <nn/nifm/nifm_ApiNetworkConnection.h>
#include <nn/nifm/nifm_ApiProxy.h>
#include <nn/nifm/nifm_ApiCommunicationControlForTest.h>
