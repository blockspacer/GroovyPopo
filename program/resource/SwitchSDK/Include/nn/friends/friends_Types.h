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
#include <nn/nn_Result.h>
#include <nn/nn_ApplicationId.h>
#include <nn/account/account_Types.h>
#include <nn/settings/settings_Language.h>
#include <nn/time/time_PosixTime.h>

namespace nn { namespace friends {

/*!
    @brief      フレンドリストの最大人数です。
*/
const int FriendCountMax = 300;

/*!
    @brief      ブロックリストの最大人数です。
*/
const int BlockedUserCountMax = 100;

/*!
    @brief      プレゼンスに設定できるアプリ固有情報フィールドのサイズです。
*/
const size_t PresenceAppFieldSize = 192;

/*!
    @brief      アプリ内表示名の最大文字数です。
*/
const int InAppScreenNameLengthMax = 20;

/*!
    @brief      1 度に取得できるプロフィールの最大人数です。
*/
const int GetProfileCountMax = 100;

/*!
    @brief      プロフィール画像の最大サイズです。
*/
const size_t ProfileImageSizeMax = 128 * 1024;

/*!
    @brief      通知キューに貯めることができる通知の最大数です。
*/
const int NotificationCountMax = 100;

/*!
    @brief      プレゼンスの状態を表します。

    @details
                プレゼンスの状態は、以下のように遷移します。

                - ユーザーアカウント Open 時@n
                  選択したユーザーアカウントのプレゼンスの状態が、オンラインに遷移します。
                - ユーザーアカウント Close 時@n
                  選択したユーザーアカウントのプレゼンスの状態が、オフラインに遷移します。
                - オンラインプレイ開始の宣言時@n
                  選択したユーザーアカウントのプレゼンスの状態が、オンラインプレイ中に遷移します。
                - オンラインプレイ終了の宣言時@n
                  選択したユーザーアカウントのプレゼンスの状態が、オンラインに遷移します。
                - ネットワーク接続時@n
                  Open 中のユーザーアカウントのプレゼンスの状態が、オフラインであればオンラインに遷移します。
                - ネットワーク切断時@n
                  全ユーザーアカウントのプレゼンスの状態が、オフラインに遷移します。@n
                  オンラインプレイの開始宣言は、自動的に取り下げられます。（＝終了を宣言）
*/
enum PresenceStatus
{
    PresenceStatus_Offline    = 0, //!< オフライン。@nフレンドからは、ゲームで遊んでいないように見えます。
    PresenceStatus_Online     = 1, //!< オンライン。@nフレンドからは、ゲームで遊んでいるに見えます。
    PresenceStatus_OnlinePlay = 2  //!< オンラインプレイ中。@nフレンドからは、ネットワーク対戦や協力プレイ等、オンラインで遊んでいるように見えます。
};

/*!
    @brief      プレゼンスの状態を利用したフィルターです。

    @see
        - FriendFilter
        - PresenceStatus
*/
enum PresenceStatusFilter
{
    PresenceStatusFilter_None               = 0, //!< プレゼンスの状態でフィルターしない。
    PresenceStatusFilter_Online             = 1, //!< オンラインのフレンドでフィルターする。
    PresenceStatusFilter_OnlinePlay         = 2, //!< オンラインプレイ中のフレンドでフィルターする。
    PresenceStatusFilter_OnlineOrOnlinePlay = 3  //!< オンライン、または、オンラインプレイ中のフレンドでフィルターする。
};

/*!
    @brief      画像サイズです。

    @details
                プロフィール画像 URL を取得する際に、取得したい画像サイズを選択することができます。@n
                標準の画像サイズは、256 × 256 です。

    @see
        - Profile
*/
enum ImageSize
{
    ImageSize_64x64   = 64,  //!< 幅 64 高さ 64 の画像。
    ImageSize_128x128 = 128, //!< 幅 128 高さ 128 の画像。
    ImageSize_256x256 = 256, //!< 幅 256 高さ 256 の画像。

    ImageSize_Standard = ImageSize_256x256 //!< 標準の画像サイズです。
};

/*!
    @brief      通知種別です。

    @details
                各通知は、以下のようにハンドリングすることを推奨します。

                - NotificationType_FriendListUpdated@n
                  @ref GetFriendList でフレンドリスト全体を更新します。
                - NotificationType_FriendPresenceUpdated@n
                  @ref Friend::Update や @ref UpdateFriendInfo でフレンド情報を更新します。
*/
enum NotificationType
{
    NotificationType_Unkown                 = 0, //!< 不明な通知です。
    NotificationType_FriendListUpdated      = 1, //!< フレンドリストが更新されました。
    NotificationType_FriendPresenceUpdated  = 2, //!< フレンドプレゼンスが更新されました。

    // for system
    NotificationType_FriendRequestReceived  = 101
};

/*!
    @brief      アプリ内表示名です。

    @details
                アプリ内表示名は UTF-8 でエンコードする必要があります。

                システムに記録できる文字数は @ref InAppScreenNameLengthMax です。@n
                アプリ内表示名が長すぎる場合、「マイページ」は最後の文字を "…" に置き換えて表示します。

                アプリ内表示名を登録する際、文字数が @ref InAppScreenNameLengthMax をオーバーしても問題ありません。@n
                文字数がオーバーしていた場合、システムが自動的に切り捨てます。

                言語コードはアプリケーションで選択中の言語のものを指定してください。@n
                本体設定に依存する場合、 @ref nn::oe::GetDesiredLanguage で取得した言語コードを指定してください。

                アプリ内表示名にユーザーアカウントのニックネームを指定する場合、アプリケーションの言語設定によらず @ref nn::oe::GetDesiredLanguage で取得した言語コードを指定しても問題ありません。

                言語コードはアプリ内表示名を表示する際のフォント選択のヒントとなります。@n
                ターゲットプラットフォーム外のサービスにおけるアプリ内表示名も扱うため、未知の言語コードが設定されている場合があります。

                未知の言語コードが設定されていた場合、 @ref nn::oe::GetDesiredLanguage で取得した言語コードを参照してください。
*/
struct InAppScreenName
{
    char name[64];                       //!< アプリ内表示名を格納するバッファ。
    nn::settings::LanguageCode language; //!< 言語コード。
};

/*!
    @brief      フレンドリストフィルターです。

    @details
                フレンドリストの取得時に、様々なフィルターを掛けて取得することができます。@n
                フィルターをゼロ初期化した場合、フレンド全員が取得されます。

                例：「今いっしょに遊べそうなお気に入りのフレンド」でフィルターする場合、以下のパラメータを指定します。

                - presenceStatus@n
                  PresenceStatusFilter_OnlineOrOnlinePlay
                - isFavoriteOnly@n
                  true
                - isSameAppPresenceOnly@n
                  true
                - isSameAppPlayedOnly@n
                  false
                - isArbitraryAppPlayedOnly@n
                  false
                - presenceGroupId@n
                  0

                isSameAppPlayedOnly や isArbitraryAppPlayedOnly に true を指定した場合、プレイログとフレンドプレゼンスの最後に遊んだアプリケーションを元に判定します。@n
                プレイログはユーザーが最近遊んだアプリケーションのログであり、古いものは削除される場合があります。@n
                また、ユーザーが任意のタイミングで非公開にしたり、削除したりすることができます。@n
                そのため、あるアプリケーションで遊んだことがあるかどうかを確実に判定することはできません。

                isArbitraryAppPlayedOnly を true に指定した場合、 presenceGroupId に任意のアプリケーションのプレゼンスグループ ID を指定する必要があります。
*/
struct FriendFilter
{
    PresenceStatusFilter presenceStatus; //!< プレゼンス状態フィルター。
    bool isFavoriteOnly;                 //!< お気に入りフレンドでフィルターするかどうか。
    bool isSameAppPresenceOnly;          //!< 実行中アプリケーションとフレンドプレゼンスの最後に遊んだアプリケーションのプレゼンスグループ ID が同じフレンドでフィルターするかどうか。
    bool isSameAppPlayedOnly;            //!< 実行中アプリケーションのプレゼンスグループ ID を持つアプリケーションで遊んだことのあるフレンドでフィルターするかどうか。
    bool isArbitraryAppPlayedOnly;       //!< 任意のアプリケーションのプレゼンスグループ ID を持つアプリケーションで遊んだことのあるフレンドでフィルターするかどうか。
    Bit64 presenceGroupId;               //!< 任意のアプリケーションのプレゼンスグループ ID。
};

/*!
    @brief      URL です。

    @details
                ユーザーのプロフィール画像の URL を取得するために利用します。@n
                文字列は NULL 終端されます。

    @see
        - Profile
*/
struct Url
{
    char value[160]; //!< URL を格納するバッファ。
};

/*!
    @brief      アプリケーション情報です。

    @details
                プレゼンスやいっしょに遊んだユーザーに記録される情報です。
*/
struct ApplicationInfo
{
    nn::ApplicationId appId; //!< アプリケーション ID。
    Bit64 presenceGroupId;   //!< プレゼンスグループ ID。
};

/*!
    @brief      いっしょに遊んだユーザーの登録キーです。

    @see
        - GetPlayHistoryRegistrationKey
        - AddPlayHistory
*/
struct PlayHistoryRegistrationKey
{
    Bit8 value[64];
};

/*!
    @brief      通知情報です。
*/
struct NotificationInfo
{
    NotificationType type;                          //!< 通知種別。
    nn::account::NetworkServiceAccountId accountId; //!< 対象ユーザー。
};

}}
