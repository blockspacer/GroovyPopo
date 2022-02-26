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

#include <nn/nn_Common.h>

namespace nn { namespace oe {


/**
 * @brief   SDK が表示する UI に対するテーマカラーのプリセットを表す列挙子です。@n
 *          nn::oe::SetExpectedThemeColor() 関数に渡すことで、カラータイプの設定・変更ができます。
 */
enum ThemeColorType
{
    ThemeColorType_Default = 0, //!< 初期状態です。テーマカラーを指定せず、各 UI のデフォルトの挙動となります。
    ThemeColorType_User,        //!< テーマカラーを指定せず、ユーザが本体設定で指定した色を使用します。
    ThemeColorType_White,       //!< 本体設定で指定できる「ホワイト」テーマと同じ色になります。
    ThemeColorType_Black,       //!< 本体設定で指定できる「ブラック」テーマと同じ色になります。
    ThemeColorType_Max          //!< 列挙子の総数です。
};


/**
 * @brief   ボリューム設定値の最小値を表す定数です。
 */
const float VolumeValueMin = 0.0f;

/**
 * @brief   ボリューム設定値の最大値を表す定数です。
 */
const float VolumeValueMax = 1.0f;


}} // namespace nn::oe

