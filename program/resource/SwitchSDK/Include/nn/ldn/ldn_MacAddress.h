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

#include <cstring>
#include <nn/nn_Common.h>

namespace nn { namespace ldn
{
    /**
     * @brief         MAC アドレスです。
     */
    struct MacAddress
    {
        Bit8 raw[6];
    };

    //! @name MAC アドレス関連
    //! @{

    /**
     * @brief         MAC アドレスを生成します。
     *
     * @param[in]     byte1       MAC アドレスの 1 バイト目
     * @param[in]     byte2       MAC アドレスの 2 バイト目
     * @param[in]     byte3       MAC アドレスの 3 バイト目
     * @param[in]     byte4       MAC アドレスの 4 バイト目
     * @param[in]     byte5       MAC アドレスの 5 バイト目
     * @param[in]     byte6       MAC アドレスの 6 バイト目
     *
     * @return        生成された MAC アドレスです。
     */
    inline MacAddress MakeMacAddress(Bit8 byte1, Bit8 byte2, Bit8 byte3,
                                     Bit8 byte4, Bit8 byte5, Bit8 byte6) NN_NOEXCEPT
    {
        MacAddress address;
        address.raw[0] = byte1;
        address.raw[1] = byte2;
        address.raw[2] = byte3;
        address.raw[3] = byte4;
        address.raw[4] = byte5;
        address.raw[5] = byte6;
        return address;
    }

    /**
     * @brief         MAC アドレスを生成します。
     *
     * @param[in]     rawAddress  MAC アドレスの生の値
     *
     * @return        生成された MAC アドレスです。
     */
    inline MacAddress MakeMacAddress(const Bit8 (&rawAddress)[6]) NN_NOEXCEPT
    {
        MacAddress address;
        std::memcpy(address.raw, rawAddress, sizeof(MacAddress));
        return address;
    }

    /**
     * @brief         MAC アドレスを比較します。
     *
     * @param[in]     lhs         比較対象の MAC アドレス
     * @param[in]     rhs         比較対象の MAC アドレス
     *
     * @return        MAC アドレスが等しい場合に true です。
     */
    inline bool operator == (MacAddress lhs, MacAddress rhs) NN_NOEXCEPT
    {
        return std::memcmp(lhs.raw, rhs.raw, sizeof(MacAddress)) == 0;
    }

    /**
     * @brief         MAC アドレスを比較します。
     *
     * @param[in]     lhs         比較対象の MAC アドレス
     * @param[in]     rhs         比較対象の MAC アドレス
     *
     * @return        MAC アドレスが等しくない場合に true です。
     */
    inline bool operator != (MacAddress lhs, MacAddress rhs) NN_NOEXCEPT
    {
        return std::memcmp(lhs.raw, rhs.raw, sizeof(MacAddress)) != 0;
    }

    //! @}

    /**
     * @brief         全て 0 の MAC アドレス (00:00:00:00:00:00) です。
     */
    const MacAddress ZeroMacAddress = MakeMacAddress(0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    /**
     * @brief         全て 1 の MAC アドレス (FF:FF:FF:FF:FF:FF) です。
     */
    const MacAddress BroadcastMacAddress = MakeMacAddress(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

}} // end of namespace nn::ldn

namespace nn { namespace ldn { namespace detail
{
    // 文字列に変換します。
    size_t ConvertToString(MacAddress macAddress, char* buffer, size_t bufferSize) NN_NOEXCEPT;

    // 文字列に変換されたアドレスの文字数です。
    const size_t StringizedMacAddressLength = 17;

}}} // namespace nn::ldn::detail
