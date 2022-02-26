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
#include <nn/nn_SdkAssert.h>

namespace nn { namespace ldn
{
    /**
     * @brief         IPv4 アドレスです。
     */
    struct Ipv4Address
    {
        Bit32 raw;
    };

    /**
     * @brief         サブネットマスクです。
     */
    struct SubnetMask
    {
        Bit32 raw;
    };

    //! @name IP アドレス関連
    //! @{

    /**
     * @brief         IPv4 アドレスを生成します。
     *
     * @param[in]     byte1       IP アドレスの 1 バイト目
     * @param[in]     byte2       IP アドレスの 2 バイト目
     * @param[in]     byte3       IP アドレスの 3 バイト目
     * @param[in]     byte4       IP アドレスの 4 バイト目
     *
     * @return        生成された IPv4 アドレスです。
     */
    const Ipv4Address MakeIpv4Address(Bit8 byte1, Bit8 byte2, Bit8 byte3, Bit8 byte4) NN_NOEXCEPT;

    /**
     * @brief         IPv4 アドレスを生成します。
     *
     * @param[in]     rawAddress  IP アドレスの生の値
     *
     * @return        生成された IPv4 アドレスです。
     */
    const Ipv4Address MakeIpv4Address(const Bit8 (&rawAddress)[4]) NN_NOEXCEPT;

    /**
     * @brief         IPv4 アドレスを比較します。
     *
     * @param[in]     lhs         比較対象の IPv4 アドレス
     * @param[in]     rhs         比較対象の IPv4 アドレス
     *
     * @return        IPv4 アドレスが等しい場合に true です。
     */
    bool operator == (Ipv4Address lhs, Ipv4Address rhs) NN_NOEXCEPT;

    /**
     * @brief         IPv4 アドレスを比較します。
     *
     * @param[in]     lhs         比較対象の IPv4 アドレス
     * @param[in]     rhs         比較対象の IPv4 アドレス
     *
     * @return        IPv4 アドレスが等しくない場合に true です。
     */
    inline bool operator != (Ipv4Address lhs, Ipv4Address rhs) NN_NOEXCEPT
    {
        return lhs.raw != rhs.raw;
    }

    /**
     * @brief         サブネットマスクを生成します。
     *
     * @param[in]     byte1       サブネットマスクの 1 バイト目
     * @param[in]     byte2       サブネットマスクの 2 バイト目
     * @param[in]     byte3       サブネットマスクの 3 バイト目
     * @param[in]     byte4       サブネットマスクの 4 バイト目
     *
     * @return        生成されたサブネットマスクです。
     *
     * @pre
     *                - 指定されたアドレスが正しいサブネットマスクを構成すること
     */
    const SubnetMask MakeSubnetMask(Bit8 byte1, Bit8 byte2, Bit8 byte3, Bit8 byte4) NN_NOEXCEPT;

    /**
     * @brief         サブネットマスクを生成します。
     *
     * @param[in]     raw           サブネットマスクの生の値
     *
     * @return        生成されたサブネットマスクです。
     *
     * @pre
     *                - 指定されたアドレスが正しいサブネットマスクを構成すること
     */
    const SubnetMask MakeSubnetMask(const Bit8 (&raw)[4]) NN_NOEXCEPT;

    /**
     * @brief         サブネットマスクを生成します。
     *
     * @param[in]     cidr        ネットワーク部のビット数
     *
     * @return        生成されたサブネットマスクです。
     *
     * @pre
     *                - 指定されたアドレスが正しいサブネットマスクを構成すること
     */
    const SubnetMask MakeSubnetMask(int cidr) NN_NOEXCEPT;

    /**
     * @brief         ホストが属するネットワークアドレスを取得します。
     *
     * @param[in]     address     ホストの IPv4 アドレスです。
     * @param[in]     mask        サブネットマスクです。
     *
     * @return        ホストが属するネットワークアドレスです。
     *
     * @pre
     *                - @a mask が正しいサブネットマスクであること
     */
    const Ipv4Address MakeNetworkAddress(Ipv4Address address, SubnetMask mask) NN_NOEXCEPT;

    /**
     * @brief         指定されたネットワークに対するブロードキャストアドレスを生成します。
     *
     * @param[in]     address     ホストあるいはネットワークの IPv4 アドレスです。
     * @param[in]     mask        サブネットマスクです。
     *
     * @return        指定されたネットワークを対象とするブロードキャストアドレスです。
     *
     * @pre
     *                - @a mask が正しいサブネットマスクであること
     */
    const Ipv4Address MakeBroadcastAddress(Ipv4Address address, SubnetMask mask) NN_NOEXCEPT;

    //! @}

    /**
     * @brief         ローカル・ループバック・アドレス (127.0.0.1) です。
     */
    const Ipv4Address LocalLoopbackIpv4Address = MakeIpv4Address(127, 0, 0, 1);

    /**
     * @brief         ブロードキャスト・アドレス (255.255.255.255) です。
     */
    const Ipv4Address BroadcastIpv4Address = MakeIpv4Address(255, 255, 255, 255);

    /**
     * @brief         全て 0 の IPv4 アドレス (0.0.0.0) です。
     */
    const Ipv4Address ZeroIpv4Address = MakeIpv4Address(0, 0, 0, 0);

}} // end of namespace nn::ldn

namespace nn { namespace ldn { namespace detail
{
    // バイトオーダの変換です。
    const Ipv4Address ReverseByteOrderImpl(Ipv4Address intentId) NN_NOEXCEPT;

    // 文字列に変換します。
    size_t ConvertToString(Ipv4Address ipv4Address, char* buffer, size_t bufferSize) NN_NOEXCEPT;

    // 文字列に変換されたアドレスの文字数です。
    const size_t StringizedIpv4AddressLengthMax = 15;

}}} // namespace nn::ldn::detail
