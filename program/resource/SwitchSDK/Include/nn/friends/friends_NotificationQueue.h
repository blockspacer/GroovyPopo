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

#include <nn/friends/friends_Types.h>
#include <nn/os.h>

namespace nn { namespace friends {

namespace detail { namespace ipc {

class INotificationService;

}}

/*!
    @brief      通知キューを扱うクラスです。

    @details
                本クラスは、ユーザーアカウントごとに作成する通知キューの管理を行います。

                通知キューは最大 @ref NotificationCountMax 個の通知を保持します。@n
                通知キューが満杯の時、 @ref NotificationType_FriendPresenceUpdated の通知はすべて削除され、代わりに @ref NotificationType_FriendListUpdated の通知 1 つに置き換わります。

                同一内容の通知は 1 つにまとめられます。@n
                例えば、あるフレンドが複数回プレゼンスを更新しても、通知キューに貯まる通知は 1 つです。@n
                アプリケーションが @ref NotificationQueue::Pop で通知を取得すると、それと同一内容の通知が再び追加されるようになります。
*/
class NotificationQueue
{
private:
    NN_DISALLOW_COPY(NotificationQueue);
    NN_DISALLOW_MOVE(NotificationQueue);

public:
    /*!
        @brief      コンストラクタです。
    */
    NotificationQueue() NN_NOEXCEPT;

    /*!
        @brief      デストラクタです。
    */
    ~NotificationQueue() NN_NOEXCEPT;

    /*!
        @brief      初期化します。

        @param[in]  uid ユーザーアカウント。

        @return     処理結果。

        @pre
            - 未初期化である。
            - 指定したユーザーアカウントが Open 状態である。
    */
    nn::Result Initialize(const nn::account::Uid& uid) NN_NOEXCEPT;

    /*!
        @brief      初期化します。（ユーザーアカウント自動選択）

        @return     処理結果。

        @pre
            - 未初期化である。
            - Open 状態のユーザーアカウントが 1 人である。

        @details
                    本関数は、Open 状態のユーザーアカウントが 1 人の場合のみ、自動的にそのユーザーを選択します。@n
                    Open 状態のユーザーアカウントが 1 人以外の場合、ASSERT して失敗します。

                    ユーザーアカウントを指定しないこと以外は、標準の @ref NotificationQueue::Initialize と同じです。
    */
    nn::Result Initialize() NN_NOEXCEPT;

    /*!
        @brief      終了します。

        @details
                    本関数は、システムイベントの破棄を行います。@n
                    本関数の呼び出し以降、 @ref NotificationQueue::GetSystemEvent で取得したシステムイベントを使用してはいけません。

                    終了処理後、本オブジェクトは未初期化状態になります。
    */
    void Finalize() NN_NOEXCEPT;

    /*!
        @brief      ユーザーアカウントを取得します。

        @return     ユーザーアカウント。
    */
    const nn::account::Uid& GetUid() const NN_NOEXCEPT;

    /*!
        @brief      通知の追加を検知するシステムイベントを取得します。

        @return     システムイベント。
    */
    nn::os::SystemEvent* GetSystemEvent() NN_NOEXCEPT;

    /*!
        @brief      通知キューをクリアします。

        @return     処理結果。
    */
    nn::Result Clear() NN_NOEXCEPT;

    /*!
        @brief      通知を取得します。

        @param[out] outInfo 通知。

        @return     処理結果。
        @retval     ResultNotificationNotFound  指定したユーザー宛ての通知はありません。

        @pre
            - outInfo != nullptr

        @details
                    通知キューに貯め込まれた通知を 1 つ取得します。

                    1 回のシステムイベントの Signal で複数の通知が追加されている場合があります。@n
                    そのため、 @ref ResultNotificationNotFound が返るまで本関数を繰り返し呼び出すことを推奨します。
    */
    nn::Result Pop(NotificationInfo* outInfo) NN_NOEXCEPT;

private:
    //
    mutable nn::os::Mutex m_Mutex;
    //
    nn::account::Uid m_Uid;
    //
    detail::ipc::INotificationService* m_Handle;
    nn::os::SystemEvent m_Event;
};

}}
