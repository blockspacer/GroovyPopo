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
*   @brief  タイムゾーンルールを扱う構造体の宣言
*/

#pragma once

#include <nn/time/time_Common.h>

#include <nn/util/util_TypedStorage.h>

namespace nn
{
namespace time
{

/**
 * @brief タイムゾーンルールを表す型です。
 *
 * @details
 *  @ref LoadTimeZoneRule 関数で読み込んでから、絶対時刻と現地時刻の相互変換に使います。
 *
 *  内部仕様は将来変更される可能性があるので、
 *  セーブデータ等に保存して次回起動時に使いまわすようなことはしないでください。
 *
 * @see LoadTimeZoneRule(), ToCalendarTime(), ToPosixTime()
 *
 */
struct TimeZoneRule
{
    nn::util::TypedStorage<char, 1024u * 16u, 8u> storage;
};

}
}
