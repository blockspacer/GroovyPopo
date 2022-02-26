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
 *  @file
 *  @brief  追加コンテンツに関する API の宣言
 *
 */


#pragma once

/**
 *  @namespace  nn::aoc
 *  @brief      追加コンテンツライブラリの名前空間です。

 *              ### 機能概要
 *              追加コンテンツライブラリでは追加コンテンツへアクセスするための機能を提供します。
 *
 *              ### FS ライブラリとの連携
 *              追加コンテンツライブラリでは追加コンテンツの内容を読み取る機能を直接的には提供していません。
 *              追加コンテンツの内容を読み取るためには FS ライブラリを使用する必要があります。
 *
 *              追加コンテンツの内容を読み取るためには下記のような手順で API を呼び出す必要があります。
 *              - @ref nn::aoc::ListAddOnContent を呼び出しマウント対象のインデックス取得する。
 *              - @ref nn::fs::MountAddOnContent を呼び出し追加コンテンツをマウントする。
 *                  （この際、nn::aoc::ListAddOnContent で取得した情報を使用する。）
 *              - FS ライブラリを使って、マウント済みの追加コンテンツにアクセスする。
 *              - @ref nn::fs::Unmount を呼び出して追加コンテンツをアンマウントする。

 *              より詳細な情報は、サンプルプログラムを参照してください。
 */

#include <nn/aoc/aoc_Types.h>
#include <nn/aoc/aoc_Api.h>
