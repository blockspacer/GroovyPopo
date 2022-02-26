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

#include <nn/util/util_StringUtil.h>

namespace nn
{
namespace nifm
{
/**
 * @brief 認証設定を表す構造体です。
 */
struct AuthenticationSetting
{
    static const size_t UsernameSize = 32;  //!< ユーザー名の最大サイズ
    static const size_t PasswordSize = 32;  //!< パスワードの最大サイズ
    bool isEnabled;                         //!< 認証を行うか否かを表すフラグ
    char username[UsernameSize];            //!< ユーザー名
    char password[PasswordSize];            //!< パスワード
};

/**
 * @brief プロキシ情報を表す構造体です。
 */
struct ProxySetting
{
    static const size_t ProxyNameSize = 100;  //!< プロキシ名の最大サイズ
    bool isEnabled;                           //!< プロキシを利用するか否かを表すフラグ
    uint16_t port;                            //!< ポート番号
    char proxy[ProxyNameSize];                //!< プロキシ名
    AuthenticationSetting authentication;     //!< 認証設定
};

inline bool operator==(const AuthenticationSetting& lhs, const AuthenticationSetting& rhs)
{
    return
        lhs.isEnabled == rhs.isEnabled &&
        nn::util::Strncmp(lhs.username, rhs.username, AuthenticationSetting::UsernameSize) == 0 &&
        nn::util::Strncmp(lhs.password, rhs.password, AuthenticationSetting::PasswordSize) == 0;
}

inline bool operator!=(const AuthenticationSetting& lhs, const AuthenticationSetting& rhs)
{
    return !(lhs == rhs);
}

inline bool operator==(const ProxySetting& lhs, const ProxySetting& rhs)
{
    return
        lhs.isEnabled == rhs.isEnabled &&
        lhs.port == rhs.port &&
        nn::util::Strncmp(lhs.proxy, rhs.proxy, ProxySetting::ProxyNameSize) == 0 &&
        lhs.authentication == rhs.authentication;
}

inline bool operator!=(const ProxySetting& lhs, const ProxySetting& rhs)
{
    return !(lhs == rhs);
}

}
}
