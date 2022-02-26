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
*   @brief  地域名を扱う構造体の宣言
*/

#pragma once

#include <nn/time/time_Common.h>
#include <nn/util/util_StringUtil.h>

namespace nn
{
namespace time
{


/**
 * @brief    地域名を扱う構造体です。
 * @details
 *  LoadTimeZoneRule() 関数に指定します。
 *
 *  アプリケーション起動中に LocationName を固定しておきたい場合には、必要に応じてキャッシュしても構いません。@n
 *  また、その値を次回起動に引き継ぎたい場合には、その時点での LocationName をセーブデータ等に保存しても構いません。@n
 *  ただし、キャッシュした LocationName から計算される値や判定（現在時刻や日付またぎなどの判定など) は、
 *  システム側のものとずれが出る可能性がある点に注意してください。
 *
 *  @link about_time_zone_api タイムゾーンを扱う API について @endlink も併せて参照してください。
 *
 * @see GetDeviceLocationName()
 */
struct LocationName
{
    static const size_t Size = 36; // 地域名のサイズ
    char _value[Size]; // 地域名を表す文字列

    /**
     * @brief   nn::time::LocationName 同士の等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する一方を指定します。
     * @param[in]   rhs     比較する他方を指定します。
     *
     * @return  両者が同じなら true を、異なるなら false を返します。
     *
     * @details
    */
    friend bool operator == (const LocationName& lhs, const LocationName& rhs) NN_NOEXCEPT
    {
        return nn::util::Strncmp(lhs._value, rhs._value, Size) == 0;
    }

    /**
     * @brief   nn::time::LocationName 同士の不等価演算子のオーバーロードです。
     *
     * @param[in]   lhs     比較する一方を指定します。
     * @param[in]   rhs     比較する他方を指定します。
     *
     * @return  両者が異なるなら true を、同じなら false を返します。
     *
     * @details
     */
    friend bool operator != (const LocationName& lhs, const LocationName& rhs) NN_NOEXCEPT
    {
        return !(lhs == rhs);
    }

};

}
}
