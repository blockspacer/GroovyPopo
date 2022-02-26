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

#include <nn/nn_Allocator.h>
#include <nn/friends/friends_Types.h>
#include <nn/friends/friends_Result.h>
#include <nn/friends/friends_Friend.h>
#include <nn/friends/friends_Profile.h>
#include <nn/friends/friends_AsyncContext.h>

namespace nn { namespace friends {

/*!
    @name 初期化
    @{
*/

/*!
    @brief      ライブラリの初期化を行います。

    @details
                本ライブラリの API はライブラリの初期化が行われていなかった場合、ライブラリの初期化を行います。@n
                ライブラリの初期化を事前に行うことで、アプリケーションの設定に由来する初期化エラーを即座に検知できるようになります。

                本関数は複数回呼び出しても問題ありません。
*/
void Initialize() NN_NOEXCEPT;

/*!
    @}
    @name ライブラリ設定
    @{
*/

/*!
    @brief      アロケータを設定します。

    @param[in]  allocateFunction    メモリ確保関数。
    @param[in]  freeFunction        メモリ解放関数。

    @pre
        - allocateFunction != nullptr
        - freeFunction != nullptr

    @details
                本関数は @ref Initialize 直後に呼び出してください。@n
                アロケータを設定しない場合、デフォルトで malloc, free を使用します。
*/
void SetAllocator(nn::AllocateFunction allocateFunction, nn::DeallocateFunction freeFunction) NN_NOEXCEPT;

/*!
    @}
    @name フレンド
    @{
*/

/*!
    @brief      フレンドリストを取得します。（ネットワークサービスアカウント ID のみ）

    @param[out] outCount        取得した数。
    @param[out] outAccountIds   フレンドリスト。
    @param[in]  uid             ユーザーアカウント。
    @param[in]  offset          オフセット。
    @param[in]  count           フレンドリストの要素数。
    @param[in]  filter          フレンドリスト取得のフィルター。

    @return     処理結果。

    @pre
        - outCount != nullptr
        - outAccountIds != nullptr
        - offset >= 0
        - count > 0
        - 指定したユーザーアカウントが Open 状態である。

    @details
                指定したオフセットから指定した数のフレンドのネットワークサービスアカウント ID を取得します。@n
                フィルターを指定することで、条件に合致したフレンドのみ取得することができます。

                本関数は本体ストレージ上のキャッシュからフレンドリストを取得します。@n
                そのため、ネットワーク未接続時でもフレンドリストを取得することができます。

                フレンドサーバーと通信可能な状態であれば、本体ストレージ上のキャッシュは定期的に更新されます。
*/
nn::Result GetFriendList(int* outCount, nn::account::NetworkServiceAccountId* outAccountIds,
    const nn::account::Uid& uid, int offset, int count, const FriendFilter& filter) NN_NOEXCEPT;

/*!
    @brief      フレンドリストを取得します。（ネットワークサービスアカウント ID のみ、ユーザーアカウント自動選択）

    @param[out] outCount        取得した数。
    @param[out] outAccountIds   フレンドリスト。
    @param[in]  offset          オフセット。
    @param[in]  count           フレンドリストの要素数。
    @param[in]  filter          フレンドリスト取得のフィルター。

    @return     処理結果。

    @pre
        - outCount != nullptr
        - outAccountIds != nullptr
        - offset >= 0
        - count > 0
        - Open 状態のユーザーアカウントが 1 人である。

    @details
                本関数は、Open 状態のユーザーアカウントが 1 人の場合のみ、自動的にそのユーザーを選択します。@n
                Open 状態のユーザーアカウントが 1 人以外の場合、ASSERT して失敗します。

                ユーザーアカウントを指定しないこと以外は、標準の @ref GetFriendList と同じです。
*/
nn::Result GetFriendList(int* outCount, nn::account::NetworkServiceAccountId* outAccountIds,
    int offset, int count, const FriendFilter& filter) NN_NOEXCEPT;

/*!
    @brief      フレンドリストを取得します。

    @param[out] outCount    取得した数。
    @param[out] outFriends  フレンドリスト。
    @param[in]  uid         ユーザーアカウント。
    @param[in]  offset      オフセット。
    @param[in]  count       フレンドリストの要素数。
    @param[in]  filter      フレンドリスト取得のフィルター。

    @return     処理結果。

    @pre
        - outCount != nullptr
        - outFriends != nullptr
        - offset >= 0
        - count > 0
        - 指定したユーザーアカウントが Open 状態である。

    @details
                指定したオフセットから指定した数のフレンド情報を取得します。@n
                フィルターを指定することで、条件に合致したフレンドのみ取得することができます。

                本関数は本体ストレージ上のキャッシュからフレンドリストを取得します。@n
                そのため、ネットワーク未接続時でもフレンドリストを取得することができます。

                フレンドサーバーと通信可能な状態であれば、本体ストレージ上のキャッシュは定期的に更新されます。
*/
nn::Result GetFriendList(int* outCount, Friend* outFriends,
    const nn::account::Uid& uid, int offset, int count, const FriendFilter& filter) NN_NOEXCEPT;

/*!
    @brief      フレンドリストを取得します。（ユーザーアカウント自動選択）

    @param[out] outCount    取得した数。
    @param[out] outFriends  フレンドリスト。
    @param[in]  offset      オフセット。
    @param[in]  count       フレンドリストの要素数。
    @param[in]  filter      フレンドリスト取得のフィルター。

    @return     処理結果。

    @pre
        - outCount != nullptr
        - outFriends != nullptr
        - offset >= 0
        - count > 0
        - Open 状態のユーザーアカウントが 1 人である。

    @details
                本関数は、Open 状態のユーザーアカウントが 1 人の場合のみ、自動的にそのユーザーを選択します。@n
                Open 状態のユーザーアカウントが 1 人以外の場合、ASSERT して失敗します。

                ユーザーアカウントを指定しないこと以外は、標準の @ref GetFriendList と同じです。
*/
nn::Result GetFriendList(int* outCount, Friend* outFriends,
    int offset, int count, const FriendFilter& filter) NN_NOEXCEPT;

/*!
    @brief      指定したフレンドのフレンド情報を更新します。

    @param[out] outFriends  フレンドリスト。
    @param[in]  uid         ユーザーアカウント。
    @param[in]  accountIds  フレンドのネットワークサービスアカウント ID リスト。
    @param[in]  count       フレンドのネットワークサービスアカウント ID リストの要素数。

    @return     処理結果。

    @pre
        - outCount != nullptr
        - accountIds != nullptr
        - count > 0
        - 指定したユーザーアカウントが Open 状態である。

    @details
                指定したネットワークサービスアカウント ID に対応するフレンドの情報を更新します。@n
                指定したネットワークサービスアカウント ID に対応するフレンドが存在しない場合、該当する @ref Friend オブジェクトはクリアされます。

                取得済みのフレンドリストを順序や数を変更しないまま情報のみを更新する場合、本関数を利用します。

                本関数は本体ストレージ上のキャッシュからフレンドリストを取得します。@n
                そのため、ネットワーク未接続時は最後にフレンドサーバーから取得した情報が返ります。

                フレンドサーバーと通信可能な状態であれば、本体ストレージ上のキャッシュは定期的に更新されます。
*/
nn::Result UpdateFriendInfo(Friend* outFriends,
    const nn::account::Uid& uid, const nn::account::NetworkServiceAccountId* accountIds, int count) NN_NOEXCEPT;

/*!
    @brief      指定したフレンドのフレンド情報を更新します。（ユーザーアカウント自動選択）

    @param[out] outFriends  フレンドリスト。
    @param[in]  accountIds  フレンドのネットワークサービスアカウント ID リスト。
    @param[in]  count       フレンドのネットワークサービスアカウント ID リストの要素数。

    @return     処理結果。

    @pre
        - outCount != nullptr
        - accountIds != nullptr
        - count > 0
        - Open 状態のユーザーアカウントが 1 人である。

    @details
                本関数は、Open 状態のユーザーアカウントが 1 人の場合のみ、自動的にそのユーザーを選択します。@n
                Open 状態のユーザーアカウントが 1 人以外の場合、ASSERT して失敗します。

                ユーザーアカウントを指定しないこと以外は、標準の @ref UpdateFriendInfo と同じです。
*/
nn::Result UpdateFriendInfo(Friend* outFriends,
    const nn::account::NetworkServiceAccountId* accountIds, int count) NN_NOEXCEPT;

/*!
    @}
    @name ブロック
    @{
*/

/*!
    @brief      ブロックリストを取得します。（ネットワークサービスアカウント ID のみ）

    @param[out] outCount        取得した数。
    @param[out] outAccountIds   ブロックリスト。
    @param[in]  uid             ユーザーアカウント。
    @param[in]  offset          オフセット。
    @param[in]  count           ブロックリストの要素数。

    @return     処理結果。

    @pre
        - outCount != nullptr
        - outAccountIds != nullptr
        - offset >= 0
        - count > 0
        - 指定したユーザーアカウントが Open 状態である。

    @details
                指定したオフセットから指定した数のブロックしたユーザーのネットワークサービスアカウント ID を取得します。

                本関数は本体ストレージ上のキャッシュからブロックリストを取得します。@n
                そのため、ネットワーク未接続時でもブロックリストを取得することができます。

                フレンドサーバーと通信可能な状態であれば、本体ストレージ上のキャッシュは定期的に更新されます。
*/
nn::Result GetBlockedUserList(int* outCount, nn::account::NetworkServiceAccountId* outAccountIds,
    const nn::account::Uid& uid, int offset, int count) NN_NOEXCEPT;

/*!
    @brief      ブロックリストを取得します。（ネットワークサービスアカウント ID のみ、ユーザーアカウント自動選択）

    @param[out] outCount        取得した数。
    @param[out] outAccountIds   ブロックリスト。
    @param[in]  offset          オフセット。
    @param[in]  count           ブロックリストの要素数。

    @return     処理結果。

    @pre
        - outCount != nullptr
        - outAccountIds != nullptr
        - offset >= 0
        - count > 0
        - Open 状態のユーザーアカウントが 1 人である。

    @details
                本関数は、Open 状態のユーザーアカウントが 1 人の場合のみ、自動的にそのユーザーを選択します。@n
                Open 状態のユーザーアカウントが 1 人以外の場合、ASSERT して失敗します。

                ユーザーアカウントを指定しないこと以外は、標準の @ref GetBlockedUserList と同じです。
*/
nn::Result GetBlockedUserList(int* outCount, nn::account::NetworkServiceAccountId* outAccountIds,
    int offset, int count) NN_NOEXCEPT;

/*!
    @}
    @name プロフィール
    @{
*/

/*!
    @brief      プロフィールリストを取得します。

    @param[out] outAsync    非同期コンテキスト。
    @param[out] outProfiles プロフィールリスト。
    @param[in]  uid         ユーザーアカウント。
    @param[in]  accountIds  任意のユーザーのネットワークサービスアカウント ID リスト。
    @param[in]  count       プロフィールリストの要素数。

    @return     処理結果。

    @pre
        - outAsync != nullptr
        - outProfiles != nullptr
        - accountIds != nullptr
        - 0 < count <= GetProfileCountMax
        - 指定したユーザーアカウントが Open 状態である。
        - NIFM ライブラリを初期化している。

    @details
                指定したネットワークサービスアカウント ID に対応するユーザーのプロフィール情報を取得します。@n
                指定したネットワークサービスアカウント ID に対応するユーザーが存在しない場合、該当する @ref Profile オブジェクトはクリアされます。

                本関数は通信処理を行うため、事前にインターネット利用要求を行い、それが受理されている必要があります。@n
                通信処理のエラーハンドリングは、 @ref AsyncContext::GetResult を参照してください。
*/
nn::Result GetProfileList(AsyncContext* outAsync, Profile* outProfiles,
    const nn::account::Uid& uid, const nn::account::NetworkServiceAccountId* accountIds, int count) NN_NOEXCEPT;

/*!
    @brief      プロフィールリストを取得します。（ユーザーアカウント自動選択）

    @param[out] outAsync    非同期コンテキスト。
    @param[out] outProfiles プロフィールリスト。
    @param[in]  accountIds  任意のユーザーのネットワークサービスアカウント ID リスト。
    @param[in]  count       プロフィールリストの要素数。

    @return     処理結果。

    @pre
        - outAsync != nullptr
        - outProfiles != nullptr
        - accountIds != nullptr
        - 0 < count <= GetProfileCountMax
        - Open 状態のユーザーアカウントが 1 人である。

    @details
                本関数は、Open 状態のユーザーアカウントが 1 人の場合のみ、自動的にそのユーザーを選択します。@n
                Open 状態のユーザーアカウントが 1 人以外の場合、ASSERT して失敗します。

                ユーザーアカウントを指定しないこと以外は、標準の @ref GetProfileList と同じです。
*/
nn::Result GetProfileList(AsyncContext* outAsync, Profile* outProfiles,
    const nn::account::NetworkServiceAccountId* accountIds, int count) NN_NOEXCEPT;

/*!
    @}
    @name プレゼンス
    @{
*/

/*!
    @brief      オンラインプレイの開始を宣言します。

    @param[in]  uid ユーザーアカウント。

    @return     処理結果。

    @pre
        - 指定したユーザーアカウントが Open 状態である。

    @details
                オンラインプレイの開始を宣言した場合、プレゼンスの状態が @ref PresenceStatus_Online から @ref PresenceStatus_OnlinePlay に遷移します。@n
                プレゼンスの状態が @ref PresenceStatus_Offline の場合、 @ref PresenceStatus_OnlinePlay には遷移しません。

                プレゼンスの状態が @ref PresenceStatus_Online になるのは、ユーザーアカウントを Open したタイミングです。@n
                ただし、プレイ状況を非公開にしている場合、 @ref PresenceStatus_Online にはなりません。

                オンラインプレイの終了を宣言したい場合、 @ref DeclareCloseOnlinePlaySession を使用してください。

                オンラインプレイの開始の宣言とプレゼンスの更新を確実に同時に行いたい場合、 @ref UserPresence::DeclareOpenOnlinePlaySession を使用してください。

    @see
        - PresenceStatus
        - UserPresence::DeclareOpenOnlinePlaySession
*/
nn::Result DeclareOpenOnlinePlaySession(const nn::account::Uid& uid) NN_NOEXCEPT;

/*!
    @brief      オンラインプレイの開始を宣言します。（ユーザーアカウント自動選択）

    @return     処理結果。

    @pre
        - Open 状態のユーザーアカウントが 1 人である。

    @details
                本関数は、Open 状態のユーザーアカウントが 1 人の場合のみ、自動的にそのユーザーを選択します。@n
                Open 状態のユーザーアカウントが 1 人以外の場合、ASSERT して失敗します。

                ユーザーアカウントを指定しないこと以外は、標準の @ref DeclareOpenOnlinePlaySession と同じです。

    @see
        - PresenceStatus
        - UserPresence::DeclareOpenOnlinePlaySession
*/
nn::Result DeclareOpenOnlinePlaySession() NN_NOEXCEPT;

/*!
    @brief      オンラインプレイの終了を宣言します。

    @param[in]  uid ユーザーアカウント。

    @return     処理結果。

    @pre
        - 指定したユーザーアカウントが Open 状態である。

    @details
                オンラインプレイの終了を宣言した場合、プレゼンスの状態が @ref PresenceStatus_OnlinePlay から @ref PresenceStatus_Online に遷移します。@n
                プレゼンスの状態が @ref PresenceStatus_OnlinePlay でない場合、遷移は行われません。

                オンラインプレイの開始を宣言したい場合、 @ref DeclareOpenOnlinePlaySession を使用してください。

                オンラインプレイの終了の宣言とプレゼンスの更新を確実に同時に行いたい場合、 @ref UserPresence::DeclareCloseOnlinePlaySession を使用してください。

    @see
        - PresenceStatus
        - UserPresence::DeclareCloseOnlinePlaySession
*/
nn::Result DeclareCloseOnlinePlaySession(const nn::account::Uid& uid) NN_NOEXCEPT;

/*!
    @brief      オンラインプレイの終了を宣言します。（ユーザーアカウント自動選択）

    @return     処理結果。

    @pre
        - Open 状態のユーザーアカウントが 1 人である。

    @details
                本関数は、Open 状態のユーザーアカウントが 1 人の場合のみ、自動的にそのユーザーを選択します。@n
                Open 状態のユーザーアカウントが 1 人以外の場合、ASSERT して失敗します。

                ユーザーアカウントを指定しないこと以外は、標準の @ref DeclareCloseOnlinePlaySession と同じです。

    @see
        - PresenceStatus
        - UserPresence::DeclareCloseOnlinePlaySession
*/
nn::Result DeclareCloseOnlinePlaySession() NN_NOEXCEPT;

/*!
    @}
    @name いっしょに遊んだユーザー
    @{
*/

/*!
    @brief      相手のいっしょに遊んだユーザーの記録に自身を登録するための登録キーを取得します。

    @param[out] outKey      登録キー。
    @param[in]  uid         ユーザーアカウント。
    @param[in]  isLocalPlay ローカル通信でいっしょに遊ぶかどうか。

    @return     処理結果。

    @pre
        - outKey != nullptr
        - 指定したユーザーアカウントが Open 状態である。

    @see
        - AddPlayHistory
*/
nn::Result GetPlayHistoryRegistrationKey(PlayHistoryRegistrationKey* outKey,
    const nn::account::Uid& uid, bool isLocalPlay) NN_NOEXCEPT;

/*!
    @brief      相手のいっしょに遊んだユーザーの記録に自身を登録するための登録キーを取得します。（ユーザーアカウント自動選択）

    @param[out] outKey      登録キー。
    @param[in]  isLocalPlay ローカル通信でいっしょに遊ぶかどうか。

    @return     処理結果。

    @pre
        - outKey != nullptr
        - Open 状態のユーザーアカウントが 1 人である。

    @details
                本関数は、Open 状態のユーザーアカウントが 1 人の場合のみ、自動的にそのユーザーを選択します。@n
                Open 状態のユーザーアカウントが 1 人以外の場合、ASSERT して失敗します。

                ユーザーアカウントを指定しないこと以外は、標準の @ref GetPlayHistoryRegistrationKey と同じです。
*/
nn::Result GetPlayHistoryRegistrationKey(PlayHistoryRegistrationKey* outKey,
    bool isLocalPlay) NN_NOEXCEPT;

/*!
    @brief      いっしょに遊んだユーザーを登録します。

    @param[in]  uid                 ユーザーアカウント。
    @param[in]  key                 相手の登録キー。
    @param[in]  inAppScreenName     相手のアプリ内表示名。
    @param[in]  myInAppScreenName   自分のアプリ内表示名。

    @return     処理結果。

    @pre
        - 指定したユーザーアカウントが Open 状態である。

    @details
                アプリケーションは、以下の手順でいっしょに遊んだユーザーの登録を行います。

                -# 登録キーの取得@n
                   @ref GetPlayHistoryRegistrationKey で自身の登録キーを取得します。
                -# セッションの構築
                -# 登録キーとアプリ内表示名の配布@n
                   最初に取得した登録キーとアプリ内表示名をセッション内の全ユーザーに配布します。
                -# 登録キーとアプリ内表示名の登録@n
                   本関数を利用し、セッション内の全ユーザーの登録キーとアプリ内表示名を登録します。

                オンラインプレイの場合、セッションとは "P2P" セッションを指します。（マッチメイクセッションではありません）@n
                これは、実際に遊べるのは P2P セッションを構築したユーザー同士であり、マッチメイクセッションを構築した段階では遊べるかどうか分からないためです。
*/
nn::Result AddPlayHistory(const nn::account::Uid& uid,
    const PlayHistoryRegistrationKey& key,
    const InAppScreenName& inAppScreenName, const InAppScreenName& myInAppScreenName) NN_NOEXCEPT;

/*!
    @brief      いっしょに遊んだユーザーを登録します。（ユーザーアカウント自動選択）

    @param[in]  key                 相手の登録キー。
    @param[in]  inAppScreenName     相手のアプリ内表示名。
    @param[in]  myInAppScreenName   自分のアプリ内表示名。

    @return     処理結果。

    @pre
        - Open 状態のユーザーアカウントが 1 人である。

    @details
                本関数は、Open 状態のユーザーアカウントが 1 人の場合のみ、自動的にそのユーザーを選択します。@n
                Open 状態のユーザーアカウントが 1 人以外の場合、ASSERT して失敗します。

                ユーザーアカウントを指定しないこと以外は、標準の @ref AddPlayHistory と同じです。
*/
nn::Result AddPlayHistory(const PlayHistoryRegistrationKey& key,
    const InAppScreenName& inAppScreenName, const InAppScreenName& myInAppScreenName) NN_NOEXCEPT;

/*!
    @}
*/

}}
