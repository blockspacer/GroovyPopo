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
* @brief   Abortマクロ 関係の型定義
*/

#include <nn/nn_Common.h>
#include <nn/diag/diag_LogTypes.h>

namespace nn { namespace diag {

/**
*   @brief      アボートが発生した理由です。
*/
enum AbortReason
{
    AbortReason_SdkAssert,          //!< SDK ライブラリによるアサートの失敗
    AbortReason_SdkRequires,        //!< SDK ライブラリによる事前条件チェックの失敗
    AbortReason_UserAssert,         //!< SDK ユーザによるアサートの失敗
    AbortReason_Abort,              //!< NN_ABORT 系列のマクロの呼び出し
    AbortReason_UnexpectedDefault   //!< switch 文において、予期せず default ラベルに到達した
};

/**
*   @brief      アボートオブザーバに与えられる、アボートに関する情報です。
*/
struct AbortInfo
{
    AbortReason abortReason;        //!< アボートが発生した理由
    const LogMessage* message;      //!< マクロに与えられたメッセージ
    const char* condition;          //!< マクロに与えられた条件を文字列化したもの
    const char* functionName;       //!< マクロが記述された箇所の関数名
    const char* fileName;           //!< マクロが記述された箇所のファイル名
    int lineNumber;                 //!< マクロが記述された箇所の行番号
};

}} // nn::diag
