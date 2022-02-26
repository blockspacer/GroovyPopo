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
 * @brief       NGC に関するライブラリの API を宣言します。
 */

/**
 * @namespace   nn::ngc
 * @brief       NGC に関するライブラリの名前空間です。
 *
 * @details
 *  ### 機能概要
 *   NGC とは、NG word Check の略です。
 *
 *   NGCライブラリ は、NG ワードチェックを行うためのライブラリです。
 *   ユーザーが入力した文字列が侮辱的であったり性的であったりする場合に
 *   これを問題であると判定ししかるべき措置を取るための機能を提供します。
 *   NGCライブラリでチェック可能な文字列の文字コードは UTF-8 です。
 *
 */


#include <nn/ngc/ngc_Types.h>
#include <nn/ngc/ngc_Result.h>
#include <nn/ngc/ngc_Api.h>
#include <nn/ngc/ngc_ProfanityFilterPatternList.h>
#include <nn/ngc/ngc_ProfanityFilterBase.h>
#include <nn/ngc/ngc_ProfanityFilter.h>
