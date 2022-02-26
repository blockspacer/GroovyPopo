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
 * @file
 * @brief       汎用ゲームパッドのデジタルボタン定義
 */

#pragma once

namespace nn { namespace settings {

/**
 * @brief       汎用ゲームパッドのデジタルボタン定義です。
 */
enum GenericPadButton
{
    GenericPadButton_1 = 1,     //!< 汎用ゲームパッドのボタン 1
    GenericPadButton_2,         //!< 汎用ゲームパッドのボタン 2
    GenericPadButton_3,         //!< 汎用ゲームパッドのボタン 3
    GenericPadButton_4,         //!< 汎用ゲームパッドのボタン 4
    GenericPadButton_5,         //!< 汎用ゲームパッドのボタン 5
    GenericPadButton_6,         //!< 汎用ゲームパッドのボタン 6
    GenericPadButton_7,         //!< 汎用ゲームパッドのボタン 7
    GenericPadButton_8,         //!< 汎用ゲームパッドのボタン 8
    GenericPadButton_9,         //!< 汎用ゲームパッドのボタン 9
    GenericPadButton_10,        //!< 汎用ゲームパッドのボタン 10
    GenericPadButton_11,        //!< 汎用ゲームパッドのボタン 11
    GenericPadButton_12,        //!< 汎用ゲームパッドのボタン 12
    GenericPadButton_13,        //!< 汎用ゲームパッドのボタン 13
    GenericPadButton_14,        //!< 汎用ゲームパッドのボタン 14
    GenericPadButton_15,        //!< 汎用ゲームパッドのボタン 15
    GenericPadButton_16,        //!< 汎用ゲームパッドのボタン 16
    GenericPadButton_Left = 64, //!< 汎用ゲームパッドのハットスイッチ 左
    GenericPadButton_Up,        //!< 汎用ゲームパッドのハットスイッチ 上
    GenericPadButton_Right,     //!< 汎用ゲームパッドのハットスイッチ 右
    GenericPadButton_Down,      //!< 汎用ゲームパッドのハットスイッチ 下
    GenericPadButton_TriggerL,  //!< 汎用ゲームパッドのトリガー 左
    GenericPadButton_TriggerR,  //!< 汎用ゲームパッドのトリガー 右
};

}} // namespace nn::settings
