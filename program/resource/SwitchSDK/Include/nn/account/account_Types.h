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

/** @file
    @brief  アカウントシステムが使用する基礎的な型を定義します。
 */

#include <nn/account/account_Config.h>

#include <nn/nn_Common.h>

namespace nn { namespace account {

//! @name アカウントシステムの基礎的な型や値、演算子の定義
//! @{

/**
    @brief アカウントシステムに登録されたユーザーの識別子
    @details
        アカウントシステムに登録されたユーザーを一意に識別するための識別子です。

        InvalidUid はユーザーの識別子として無効であることが保証されています。
        また bool 演算子により、ある Uid 型の値が無効かどうかを評価できます。
*/
struct Uid
{
    Bit64 _data[2];

    /**
        @brief Uid の値の有効性を評価します。

        @return *this != InvalidUid

        @details
            Uid の値の有効性を評価します。
            ここでの有効性とは Uid 型の値が InvalidUid でないことを意味します。
            仮に Uid の値が有効な場合でも、アカウントシステムに登録されたユーザーを指すことは保証されません。
    */
    NN_EXPLICIT_OPERATOR bool() const NN_NOEXCEPT;
};

/**
    @brief Uid の同値比較演算子
    @details
        与えられたふたつの Uid が等しいことを検証します。
        ふたつの Uid が互いに等値であるとき、この演算子は true を返します。
        それ以外の場合、この演算子は false を返します。

        有効な Uid と無効な Uid の比較では常に false が返ります。
        無効な Uid 同士の比較では常に true が返ります。
*/
inline bool operator ==(const Uid& lhs, const Uid& rhs) NN_NOEXCEPT;

/**
    @brief Uid の非同値比較演算子
    @details
        与えられたふたつの Uid が異なることを検証します。
        ふたつの Uid が互いに異なるとき、この演算子は true を返します。
        それ以外の場合、この演算子は false を返します。

        有効な Uid と無効な Uid の比較では常に true が返ります。
        無効な Uid 同士の比較では常に false が返ります。
*/
inline bool operator !=(const Uid& lhs, const Uid& rhs) NN_NOEXCEPT;

/**
    @brief 無効であることが保証された Uid の値
    @details
        この値はユーザーの識別子として取り得ない Uid 型の値を表します。
        すなわち、アカウントシステムはその有効なユーザーとして、 InvalidUid をもつユーザーを作成しないことを意味します。
*/
const Uid InvalidUid = {{0x00ull, 0x00ull}};

/**
    @brief Open 状態にあるユーザーを扱うためのハンドル
    @details
        OpenUser() によってユーザーを Open 状態に変更すると、そのユーザーを指す UserHandle が作成されます。
        この UserHandle を破棄するとハンドルの指すユーザーは Closed 状態に戻ります。
        UserHandle は CloseUser() によって破棄できます。
*/
struct UserHandle
{
    Bit64 _data[2];
    uintptr_t _context;
};

//! @}


//! @name ユーザーのプロフィールに関連する型の定義
//! @{

/** @brief ユーザーのニックネーム
    @details
        ユーザーのニックネームを持ちます。
 */
struct Nickname
{
    char name[NicknameBytesMax + 1]; //!< UTF-8 でエンコードされたニックネーム。終端文字を除くバイト数が NicknameBytesMax 以下であることが保証されます。
};

//! @}

//! @name ネットワークサービスに関する型や演算子の定義
//! @{

/**
    @brief ネットワークサービスアカウントの ID
    @details
        ユーザーがネットワークサービスを利用する際に必要となる「ネットワークサービスアカウント」を指す ID です。
        ネットワークサービスアカウントはフレンドシステム (nn::friends) や NEX の機能を使用する際に必要となります。

        それぞれのユーザーは最大1つのネットワークサービスアカウントを保有しますが、このアカウントは外部要因で置き換えられたり削除されることがあり、そのIDは一定ではありません。
        このためアプリケーションは、ネットワークサービス利用時に都度 GetNetworkServiceAccountId() を使用して、最新の NetworkServiceAccountId を取得および使用してください。
        また、アプリケーションはユーザーの NetworkServiceAccountId が一定であることを期待した設計をもたないでください。
*/
struct NetworkServiceAccountId
{
    uint64_t id; //!< ネットワークサービスアカウントの ID である 64bit の値
};

/**
    @brief ニンテンドーアカウントサーバーとのOAuth 2.0/Open ID Connectの手続きにおいて使用するパラメータ
    @details
        ニンテンドーアカウントサーバーから認可を取得する際に、サーバーに送信するパラメータです。

        これらのパラメータはアプリケーションおよび独自サーバーの任意に設定することができます。
        それぞれのパラメータは長さ1以上で'\0'で終端されている必要があります。

        ニンテンドーアカウントサーバーとのOAuth 2.0/Open ID Connectの手続きに関して、詳細は「独自サーバー構築マニュアル」を参照してください。
*/
struct NintendoAccountAuthorizationRequestParameters
{
    char scope[256]; //!< ニンテンドーアカウントサーバーから認可を取得する際に要請する権限の範囲(scope)。 (RFC 6749 準拠)
    char state[128]; //!< CSRFを防ぐために使用するstate値。 (RFC 6749 準拠)
    char nonce[128]; //!< リプレイ攻撃を防ぐために使用するnonce値。 先頭から終端までの範囲に 0x20-0x7E 以外の値を含まないでください。
};

//! @}


/* --------------------------------------------------------------------------------------------
    実装
 */
#if !defined(NN_BUILD_FOR_DOCUMENT_GENERATION)
inline Uid::operator bool() const NN_NOEXCEPT
{
    return *this != InvalidUid;
}
#endif

inline bool operator ==(const Uid& lhs, const Uid& rhs) NN_NOEXCEPT
{
    return true
        && lhs._data[0] == rhs._data[0]
        && lhs._data[1] == rhs._data[1];
}

inline bool operator !=(const Uid& lhs, const Uid& rhs) NN_NOEXCEPT
{
    return !(lhs == rhs);
}

inline bool operator ==(const NetworkServiceAccountId& lhs, const NetworkServiceAccountId& rhs) NN_NOEXCEPT
{
    return lhs.id == rhs.id;
}
inline bool operator !=(const NetworkServiceAccountId& lhs, const NetworkServiceAccountId& rhs) NN_NOEXCEPT
{
    return !(lhs == rhs);
}

}}
