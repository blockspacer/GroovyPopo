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

/**
* @file
* @brief   Assertion 失敗ハンドラ関連の API 宣言
*/

#include <nn/nn_Common.h>
#include <nn/diag/diag_AssertionTypes.h>
#include <nn/diag/diag_LogTypes.h>

namespace nn { namespace diag {

/**
*   @brief      nn::diag::AssertionFailureHandler から返った後に行われる処理を表します。通常は、Abort を選択してください。
*/
enum AssertionFailureOperation
{
    AssertionFailureOperation_Abort,    //!< 実行を停止します
    AssertionFailureOperation_Continue  //!< Assertion 失敗が発生していなかったかのように実行を続行します
};

/**
*   @brief      NN_ASSERT 系列のマクロに与えた条件が満たされない場合に呼び出されるコールバック関数の型です。
*
*   @param[in]  AssertionInfo        Assertion に関する情報
*
*   @return                          関数から返った後に指定する処理を nn::diag::AssertionFailureOperation で返します。
*
*   @details
* 戻り値で、関数から返った後の処理を指定します。通常は、Abort を選択してください。
* Continue を選択する場合、NN_ASSERT 系列のマクロに与えた条件が満たされない状態で実行が継続されるため、十分な注意が必要です。
*
* 複数のスレッドで同時に呼び出される可能性がありますので、再入可能である必要があります。
*
* 例外安全でない関数で呼び出される可能性がありますので、例外を発生させてはいけません。
*
* 関数内では、ログの出力などの処理を行うことが想定されています。
*
*/
typedef AssertionFailureOperation (*AssertionFailureHandler)(const AssertionInfo& assertionInfo);

#if !defined(NN_SDK_BUILD_LIBRARY) // SDK はコールバック関数を差し替えてはいけない
/**
*   @brief      NN_ASSERT 系列のマクロに与えた条件が満たされない場合に呼び出されるコールバック関数を設定します。
*
*   @param[in]  assertionFailureHandler     コールバック関数に設定する関数のポインタ
*
*   @details    コールバック関数の注意点については、 nn::diag::AssertionFailureHandler のリファレンスを参照してください。
*/
void SetAssertionFailureHandler(AssertionFailureHandler assertionFailureHandler) NN_NOEXCEPT;
#endif

}}
