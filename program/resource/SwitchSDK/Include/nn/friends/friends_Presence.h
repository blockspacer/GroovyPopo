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
#include <nn/friends/friends_Macros.h>

namespace nn { namespace friends {

/*!
    @brief      ユーザーアカウントのプレゼンスを扱うクラスです。

    @details
                プレゼンスは、フレンドに自分の情報（オンライン状態や遊んでいるアプリケーション等）をリアルタイムに伝搬する機能です。

                本クラスで設定した情報は、フレンドの本体上で @ref FriendPresence を経由して取得されます。

                ユーザーはプレゼンスの公開範囲を設定することができ、フレンドに一切の情報を送信しないようにすることも選択できます。@n
                そのため、必ずフレンドにデータが伝わる保証はありません。

                プレゼンスの公開範囲は、以下の通りです。

                - 非公開
                - お気に入りフレンドまで
                - フレンド全員

                システムがプレゼンスのフィルタリングを行うため、アプリケーションがプレゼンスの公開範囲を気にすることはありません。

                自分のプレゼンスの状態の遷移に関しては、 @ref PresenceStatus を参照してください。

                オンラインプレイの開始・終了の宣言とプレゼンスの更新を同時に行いたい場合、以下の関数を使用してください。

                - UserPresence::DeclareOpenOnlinePlaySession
                - UserPresence::DeclareCloseOnlinePlaySession

                どちらの関数も呼び出さなかった場合、オンラインプレイの開始・終了の宣言は現状を維持します。
*/
class UserPresence
{
public:
    /*!
        @brief      コンストラクタです。
    */
    UserPresence() NN_NOEXCEPT;

    /*!
        @brief      初期化します。

        @param[in]  uid ユーザーアカウント。

        @return     処理結果。

        @pre
            - 指定したユーザーアカウントが Open 状態である。
    */
    nn::Result Initialize(const nn::account::Uid& uid) NN_NOEXCEPT;

    /*!
        @brief      初期化します。（ユーザーアカウント自動選択）

        @return     処理結果。

        @pre
            - Open 状態のユーザーアカウントが 1 人である。

        @details
                    本関数は、Open 状態のユーザーアカウントが 1 人の場合のみ、自動的にそのユーザーを選択します。@n
                    Open 状態のユーザーアカウントが 1 人以外の場合、ASSERT して失敗します。

                    ユーザーアカウントを指定しないこと以外は、標準の @ref UserPresence::Initialize と同じです。
    */
    nn::Result Initialize() NN_NOEXCEPT;

    /*!
        @brief      入力内容をクリアします。
    */
    void Clear() NN_NOEXCEPT;

    /*!
        @brief      ゲームモード説明文を設定します。

        @param[in]  description ゲームモード説明文。

        @return     処理結果。

        @pre
            - description != nullptr

        @details
                    ゲームモード説明文は、「マイページ」上で表示される文字列です。@n
                    フレンドが同じアプリケーションを起動していなくても、アプリケーションで何をしているかをフレンドに知らせることができます。

                    ゲームモード説明文に文字数の制限はありません。（「マイページ」は、ゲームモード説明文をスクロール表示します）

                    本関数は、アプリ固有領域に "sys_description" というキーでゲームモード説明文を登録します。@n
                    細かい制限に関しては、@ref UserPresence::SetAppValue を参照してください。

        @see
            - FriendPresence::GetDescription
    */
    nn::Result SetDescription(const char* description) NN_NOEXCEPT;

    /*!
        @brief      アプリ固有情報を設定します。【非推奨】

        @param[in]  key     キー。
        @param[in]  value   バリュー。

        @return     処理結果。

        @pre
            - key != nullptr
            - value != nullptr

        @details
                    本関数の利用は非推奨です。@n
                    リリース済みのアプリケーションを最新の SDK でリビルドする等、互換性維持の目的でのみ利用可能です。

                    アプリ固有情報を設定することで、任意のデータをフレンドに伝搬することができます。

                    アプリ固有情報の記録領域のサイズは @ref PresenceAppFieldSize です。@n
                    1 つのキーバリューに付き、キーの文字数＋バリューのバイト数＋2 バイト（NULL 終端用）のサイズを消費します。

                    設定できるキーバリューには以下の制限があります。

                    - キー@n
                      ASCII の英数字、及び、"_" が利用可能です。@n
                      "sys_" で始まるキーはシステムが予約しているため利用できません。
                    - バリュー@n
                      UTF-8 文字列のみ設定可能です。

        @see
            - @ref FriendPresence::GetAppValue
    */
    NN_DEPRECATED nn::Result SetAppValue(const char* key, const char* value) NN_NOEXCEPT;

    /*!
        @brief      オンラインプレイの開始を宣言します。

        @details
                    本関数は、オンラインプレイの開始の宣言とプレゼンスの更新を同時に行うことができます。@n
                    詳しくは、 @ref nn::friends::DeclareOpenOnlinePlaySession を参照してください。

                    宣言の反映は @ref UserPresence::Commit 呼び出し時です。@n
                    @ref UserPresence::DeclareCloseOnlinePlaySession を呼び出していた場合、宣言は上書きされます。

        @see
            - nn::friends::DeclareOpenOnlinePlaySession
    */
    void DeclareOpenOnlinePlaySession() NN_NOEXCEPT;

    /*!
        @brief      オンラインプレイの終了を宣言します。

        @details
                    本関数は、オンラインプレイの終了の宣言とプレゼンスの更新を同時に行うことができます。@n
                    詳しくは、 @ref nn::friends::DeclareCloseOnlinePlaySession を参照してください。

                    宣言の反映は @ref UserPresence::Commit 呼び出し時です。@n
                    @ref UserPresence::DeclareOpenOnlinePlaySession を呼び出していた場合、宣言は上書きされます。

        @see
            - nn::friends::DeclareCloseOnlinePlaySession
    */
    void DeclareCloseOnlinePlaySession() NN_NOEXCEPT;

    /*!
        @brief      設定を反映します。

        @return     処理結果。

        @pre
            - 初期化時に指定したユーザーアカウントが Open 状態である。

        @details
                    設定の反映後、本オブジェクトを再利用する場合は @ref UserPresence::Clear を呼び出してください。
    */
    nn::Result Commit() NN_NOEXCEPT;

private:
    //
    NN_FRIENDS_CLASS_STORAGE(224);

private:
    //
    nn::Result SetAppValueImpl(const char* key, const char* value) NN_NOEXCEPT;
};

/*!
    @brief      フレンドのプレゼンスを扱うクラスです。

    @details
                フレンドが設定したプレゼンスの情報を取得する機能を持ちます。@n
                本クラスで取得できる情報は、フレンドの本体上で @ref UserPresence を経由して設定されます。

                プレゼンスはリアルタイムな情報を伝搬する機能であるため、本体ストレージ上にキャッシュは残りません。

                以下の場合、有効なプレゼンスデータは取得できません。

                - ネットワークに接続していない。
                - フレンドサーバーに障害が発生しており、フレンドのプレゼンスが取得できない。
                - フレンドが自身のプレゼンス公開範囲を制限している。
*/
class FriendPresence
{
public:
    /*!
        @brief      コンストラクタです。
    */
    FriendPresence() NN_NOEXCEPT;

    /*!
        @brief      プレゼンスの状態を取得します。

        @return     プレゼンスの状態。

        @details
                    有効なプレゼンスデータが存在しない場合、本関数は @ref PresenceStatus_Offline を返します。
    */
    PresenceStatus GetStatus() const NN_NOEXCEPT;

    /*!
        @brief      最後に遊んだアプリケーションを取得します。

        @return     最後に遊んだアプリケーション。

        @details
                    プレゼンスの状態が @ref PresenceStatus_Online または @ref PresenceStatus_OnlinePlay の場合、現在遊んでいるアプリケーションとなります。

                    有効なプレゼンスデータが存在しない場合、本関数はゼロクリアされたアプリケーション情報を返します。
    */
    const ApplicationInfo& GetLastPlayedApplication() const NN_NOEXCEPT;

    /*!
        @deprecated
                    本関数は廃止予定です。@n
                    代わりに、 @ref FriendPresence::GetLastUpdateTime を使用してください。

        @brief      最終オンライン時刻を取得します。【廃止予定】

        @return     最終オンライン時刻。

        @details
                    有効なプレゼンスデータが存在しない場合、本関数は 0 を返します。
    */
    NN_DEPRECATED const nn::time::PosixTime& GetLastOnlineTime() const NN_NOEXCEPT;

    /*!
        @brief      プレゼンスの最終更新時刻を取得します。

        @return     プレゼンスの最終更新時刻。

        @details
                    有効なプレゼンスデータが存在しない場合、本関数は 0 を返します。
    */
    const nn::time::PosixTime& GetLastUpdateTime() const NN_NOEXCEPT;

    /*!
        @brief      ゲームモード説明文を取得します。

        @return     ゲームモード説明文。

        @see
            - UserPresence::SetDescription

        @details
                    フレンドが設定したゲームモード説明文を取得します。

                    有効なプレゼンスデータが存在しない場合、本関数は空文字列を返します。
    */
    const char* GetDescription() const NN_NOEXCEPT;

    /*!
        @brief      アプリ固有情報を取得します。【非推奨】

        @param[in]  key キー。

        @pre
            - key != nullptr

        @return     アプリ固有情報。

        @details
                    本関数の利用は非推奨です。@n
                    リリース済みのアプリケーションを最新の SDK でリビルドする等、互換性維持の目的でのみ利用可能です。

                    フレンドが設定したアプリ固有情報を取得します。

                    有効なプレゼンスデータが存在しない場合、本関数は空文字列を返します。

        @see
            - UserPresence::SetAppValue
    */
    NN_DEPRECATED const char* GetAppValue(const char* key) const NN_NOEXCEPT;

    /*!
        @brief      実行中アプリケーションと同一プレゼンスグループのアプリケーションで遊んでいるかどうかを確認します。

        @return     実行中アプリケーションと同一プレゼンスグループのアプリケーションで遊んでいるかどうか。

        @details
                    同一プレゼンスグループかどうかは、プレゼンスグループ ID が一致しているかどうかで判定します。

                    有効なプレゼンスデータが存在しない場合、本関数は false を返します。
    */
    bool IsSamePresenceGroupApplication() const NN_NOEXCEPT;

private:
    //
    NN_FRIENDS_CLASS_STORAGE(224);
};

}}
