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
*   @brief  NintendoSDK のシステムクロックが扱う型を定義しているクラスの型宣言
*/

#pragma once

#include <nn/time/time_Common.h>

#include <chrono>


namespace nn
{
namespace time
{

/**
 * @brief   NintendoSDK のシステムクロックが扱う型を定義しているクラスです。
 *
 * @details
 *  C++ 標準の TrivialClock の要件を満たし、 std::chrono::system_clock に準ずる機能を提供するクロックが扱う型は、このクラスの型の別名として定義されています。
 *  このため、それらのクロックが返す time_point はすべて同一の型として扱うことができます。
 */
class SystemClockTraits
{
public:
    /**
     * @brief   NintendoSDK のシステムクロックから取得・計算される時刻・時間の内部表現となる算術型を表します。
     *
     * @details
     */
    typedef int64_t rep;

    /**
     * @brief   NintendoSDK のシステムクロックの持つ分解能を std::ratio 型で表したものです。
     *
     * @details
     *  1単位当たりの秒数で表されています。
     */
    typedef std::ratio<1> period;

    /**
     * @brief   NintendoSDK のシステムクロックが扱う、経過時間を表す型です。
     *
     * @details
     */
    typedef std::chrono::duration<rep, period> duration;

    /**
     * @brief   NintendoSDK のシステムクロックから取得される、時間の流れの中の一点を指す型です。
     *
     * @details
     */
    typedef std::chrono::time_point<SystemClockTraits> time_point;
};

}
}
