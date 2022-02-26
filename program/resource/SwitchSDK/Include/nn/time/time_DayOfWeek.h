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
*   @brief  曜日を表す列挙型の宣言
*/

#pragma once

#include <nn/time/time_Common.h>

namespace nn
{
namespace time
{

/**
* @brief    曜日を表す列挙型です。
* @details
*/
enum DayOfWeek
{
    DayOfWeek_Sunday = 0,    //!< 日曜日
    DayOfWeek_Monday,        //!< 月曜日
    DayOfWeek_Tuesday,       //!< 火曜日
    DayOfWeek_Wednesday,     //!< 水曜日
    DayOfWeek_Thursday,      //!< 木曜日
    DayOfWeek_Friday,        //!< 金曜日
    DayOfWeek_Saturday       //!< 土曜日
};

}
}
