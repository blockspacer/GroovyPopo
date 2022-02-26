/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/*!
    @file
    @brief   フレンド情報の表示及び操作に関連する UI を呼び出す処理の API を宣言します。
*/

#pragma once

#include <nn/nn_Result.h>
#include <nn/friends/friends_Types.h>

namespace nn { namespace friends {

/*!
    @name アプレット
    @{
*/

/*!
    @brief      フレンド一覧画面を表示します。

    @param[in]  uid     フレンド一覧を表示する対象のユーザーアカウント。

    @return     処理結果。

    @details
                uid で指定したユーザーアカウントのフレンド一覧画面を表示します。
                本関数は内部で UI を表示するため、UI 操作が完了するまで処理をブロックします。
*/
nn::Result ShowFriendList(const nn::account::Uid& uid) NN_NOEXCEPT;

/*!
    @brief      ユーザーの詳細画面を表示します。

    @param[in]  uid                 UI 上で自身として扱うユーザーアカウント。
    @param[in]  accountId           詳細情報を表示する対象のネットワークサービスアカウント ID。
    @param[in]  myInAppScreenName   UI 上で自身として扱うユーザーアカウントのアプリ内表示名。
    @param[in]  inAppScreenName     詳細情報を表示する対象のアプリ内表示名。

    @return     処理結果。

    @details
                uid で指定したユーザーアカウントから見た場合の accountId で指定したユーザーの情報を表示します。
                詳細情報を表示する対象のユーザーがフレンドである場合、フレンドでない場合の両方に対応しています。@n
                また、uid に紐づくネットワークサービスアカウント ID が accountId と一致する場合は uid の情報を表示します。@n
                本関数は内部で UI を表示するため、UI 操作が完了するまで処理をブロックします。
*/
nn::Result ShowUserDetailInfo(const nn::account::Uid& uid, const nn::account::NetworkServiceAccountId& accountId,
    const InAppScreenName& myInAppScreenName, const InAppScreenName& inAppScreenName) NN_NOEXCEPT;

/*!
    @brief      フレンドリクエスト送信処理を行います。

    @param[in]  uid                 フレンドリクエスト送信元となるユーザーアカウント。
    @param[in]  accountId           フレンドリクエスト送信対象のネットワークサービスアカウント ID。
    @param[in]  myInAppScreenName   フレンドリクエスト送信元となるユーザーアカウントのアプリ内表示名。
    @param[in]  inAppScreenName     フレンドリクエスト送信対象のアプリ内表示名。

    @return     処理結果。
    @retval     nn::ResultSuccess   フレンドリクエスト送信処理に成功しました。

    @details
                uid で指定したユーザーアカウントから accountId で指定したユーザーに対してフレンドリクエストを送信します。
                本関数は内部で UI を表示するため、UI 操作が完了するまで処理をブロックします。
*/
nn::Result StartSendingFriendRequest(const nn::account::Uid& uid, const nn::account::NetworkServiceAccountId& accountId,
    const InAppScreenName& myInAppScreenName, const InAppScreenName& inAppScreenName) NN_NOEXCEPT;

/*!
    @brief      フレンドリクエストの送信手段一覧画面を表示します。

    @param[in]  uid     フレンドリクエストの送信手段を表示する対象のユーザーアカウント。

    @return     処理結果。

    @details
                uid で指定したユーザーアカウントからフレンドリクエストを送信する手段の一覧を表示します。
                本関数は内部で UI を表示するため、UI 操作が完了するまで処理をブロックします。
*/
nn::Result ShowMethodsOfSendingFriendRequest(const nn::account::Uid& uid) NN_NOEXCEPT;

/*!
    @brief      ローカル通信でフレンドリクエストを予約する画面を表示します。

    @param[in]  uid     フレンドリクエストを予約する画面を表示する対象のユーザーアカウント。

    @return     処理結果。

    @details
                uid で指定したユーザーアカウントがローカル通信でフレンドリクエストを予約する画面を表示します。
                本関数は内部で UI を表示するため、UI 操作が完了するまで処理をブロックします。
*/
nn::Result StartFacedFriendRequest(const nn::account::Uid& uid) NN_NOEXCEPT;

/*!
    @brief      受信したフレンドリクエストの一覧画面を表示します。

    @param[in]  uid     受信したフレンドリクエストの一覧を表示する対象のユーザーアカウント。

    @return     処理結果。

    @details
                uid で指定したユーザーアカウントが受信しているフレンドリクエストの一覧画面を表示します。
                本関数は内部で UI を表示するため、UI 操作が完了するまで処理をブロックします。
*/
nn::Result ShowReceivedFriendRequestList(const nn::account::Uid& uid) NN_NOEXCEPT;

/*!
    @brief      ブロックリストの管理画面を表示します。

    @param[in]  uid     ブロックリストの管理画面を表示する対象のユーザーアカウント。

    @return     処理結果。

    @details
                uid で指定したユーザーアカウントのブロックリストの管理画面を表示します。
                本関数は内部で UI を表示するため、UI 操作が完了するまで処理をブロックします。
*/
nn::Result ShowBlockedUserList(const nn::account::Uid& uid) NN_NOEXCEPT;

/*!
    @}
*/

}} // namespace nn::friends
