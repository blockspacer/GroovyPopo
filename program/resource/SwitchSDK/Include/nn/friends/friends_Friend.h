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
#include <nn/friends/friends_Presence.h>

namespace nn { namespace friends {

/*!
    @brief      フレンドを扱うクラスです。

    @details
                あるユーザーアカウントに関するフレンドの情報を参照することができます。@n
                複数のユーザーアカウントが同一ユーザーをフレンドにしていたとしても、ユーザーアカウントごとに参照できる内容は異なります。
*/
class Friend
{
public:
    /*!
        @brief      コンストラクタです。
    */
    Friend() NN_NOEXCEPT;

    /*!
        @brief      ネットワークサービスアカウント ID を取得します。

        @return     ネットワークサービスアカウント ID。
    */
    nn::account::NetworkServiceAccountId GetAccountId() const NN_NOEXCEPT;

    /*!
        @brief      ニックネームを取得します。

        @return     ニックネーム。
    */
    const nn::account::Nickname& GetNickname() const NN_NOEXCEPT;

    /*!
        @brief      本体ストレージ上にキャッシュしたユーザーのプロフィール画像を取得します。

        @param[out] outSize 取得したサイズ。
        @param[in]  buffer  バッファ。
        @param[in]  size    バッファのサイズ。

        @return     処理結果。

        @retval     ResultProfileImageCacheNotFound フレンドのプロフィール画像のキャッシュが存在しません。@n
                                                    デフォルトのユーザーアイコンを表示するか、別途 URL を取得してダウンロードしてください。

        @pre
            - outSize != nullptr
            - buffer != nullptr
            - size > 0
            - 初期化時に指定したユーザーアカウントが Open 状態である。

        @details
                    プロフィール画像は、256 × 256 の JPEG 画像で保存されています。@n
                    ファイルの最大サイズは 100KB 以内に収まります。

                    別サイズのプロフィール画像を取得したい場合、 @ref GetProfileList で URL を取得し、ダウンロードしてください。
    */
    nn::Result GetProfileImage(size_t* outSize, void* buffer, size_t size) const NN_NOEXCEPT;

    /*!
        @brief      フレンドプレゼンスを取得します。

        @return     フレンドプレゼンス。
    */
    const FriendPresence& GetPresence() const NN_NOEXCEPT;

    /*!
        @brief      お気に入りのフレンドかどうかを確認します。

        @return     お気に入りのフレンドかどうか。
    */
    bool IsFavorite() const NN_NOEXCEPT;

    /*!
        @brief      新規に成立したフレンドかどうかを確認します。

        @return     新規に成立したフレンドかどうか。

        @details
                    新規に成立したフレンドには、「マイページ」上で新着マークが付きます。@n
                    このフラグは、ユーザーが「マイページ」上で新着マークを見た時に false になります。

                    新着マークは「マイページ」上での表示用であるため、アプリケーション内で表示してはいけません。
    */
    bool IsNewly() const NN_NOEXCEPT;

    /*!
        @brief      フレンド情報を更新します。

        @return     処理結果。

        @pre
            - 本オブジェクトの取得時に指定したユーザーアカウントが Open 状態である。

        @details
                    本関数は、 @ref UpdateFriendInfo を使用して自身の情報を更新します。@n
                    詳細は、 @ref UpdateFriendInfo を参照してください。
    */
    nn::Result Update() NN_NOEXCEPT;

    /*!
        @brief      有効なデータを保持しているかどうかを確認します。

        @return     有効なデータを保持しているかどうか。
    */
    bool IsValid() const NN_NOEXCEPT;

private:
    //
    NN_FRIENDS_CLASS_STORAGE(512);
};

}}
