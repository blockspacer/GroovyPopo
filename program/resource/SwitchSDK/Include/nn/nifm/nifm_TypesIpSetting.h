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

#include <nn/nifm/nifm_TypesIpAddress.h>
#include <nn/nifm/nifm_TypesProxySetting.h>

namespace nn
{
namespace nifm
{

/**
 * @brief IP アドレス設定を表す構造体です。
 */
struct IpAddressSetting
{
    bool isAuto;                 //!< IP アドレスを自動取得するか否かを表すフラグ
    IpV4Address ipAddress;       //!< IP アドレス
    IpV4Address subnetMask;      //!< サブネットマスク
    IpV4Address defaultGateway;  //!< デフォルトゲートウェイ
};

/**
 * @brief DNS 設定を表す構造体です。
 */
struct DnsSetting
{
    bool isAuto;               //!< DNS を自動取得するか否かを表すフラグ
    IpV4Address preferredDns;  //!< 優先 DNS
    IpV4Address alternateDns;  //!< 代替 DNS
};

/**
 * @brief IP 設定情報を表す構造体です。
 */
struct IpSettingData
{
    IpAddressSetting ip;  //!< IP アドレス設定
    DnsSetting dns;       //!< DNS 設定
    ProxySetting proxy;   //!< プロキシ情報
    uint16_t mtu;         //!< MTU
};

inline bool operator==(const IpAddressSetting& lhs, const IpAddressSetting& rhs)
{
    return
        lhs.isAuto == rhs.isAuto &&
        lhs.ipAddress == rhs.ipAddress &&
        lhs.subnetMask == rhs.subnetMask &&
        lhs.defaultGateway == rhs.defaultGateway;
}

inline bool operator!=(const IpAddressSetting& lhs, const IpAddressSetting& rhs)
{
    return !(lhs == rhs);
}

inline bool operator==(const DnsSetting& lhs, const DnsSetting& rhs)
{
    return
        lhs.isAuto == rhs.isAuto &&
        lhs.preferredDns == rhs.preferredDns &&
        lhs.alternateDns == rhs.alternateDns;
}

inline bool operator!=(const DnsSetting& lhs, const DnsSetting& rhs)
{
    return !(lhs == rhs);
}

inline bool operator==(const IpSettingData& lhs, const IpSettingData rhs)
{
    return
        lhs.ip == rhs.ip &&
        lhs.dns == rhs.dns &&
        lhs.proxy == rhs.proxy &&
        lhs.mtu == rhs.mtu;
}

}
}
