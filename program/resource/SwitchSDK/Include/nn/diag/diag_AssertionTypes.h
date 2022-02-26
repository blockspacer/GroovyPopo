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
* @brief   Assertマクロ 関係の型定義
*/

#include <nn/nn_Common.h>
#include <nn/diag/diag_LogTypes.h>

namespace nn { namespace diag {

/**
*   @brief      Assert マクロの種類です。
*/
enum AssertionType
{
    AssertionType_SdkAssert,    //!< SDK ライブラリによるアサート
    AssertionType_SdkRequires,  //!< SDK ライブラリによる事前条件チェック
    AssertionType_UserAssert    //!< SDK ユーザによるアサート
};

/**
*   @brief      NN_ASSERT 系列のマクロで発生した Assertion に関する情報です。
*
*   @attention  正式な仕様が決まっていないので、メンバは予告なく変更されることがあります。
*/
struct AssertionInfo
{
    AssertionType assertionType;    //!< Assert マクロの種類
    const LogMessage* message;      //!< マクロに与えられたメッセージ
    const char* condition;          //!< マクロに与えられた条件を文字列化したもの
    const char* functionName;       //!< マクロが記述された箇所の関数名
    const char* fileName;           //!< マクロが記述された箇所のファイル名
    int lineNumber;                 //!< マクロが記述された箇所の行番号
};

}}
