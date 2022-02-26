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
    @brief      ユーザープロフィールを扱うクラスです。

    @details
                本クラスは、ユーザーのニックネームとプロフィール画像 URL を取得するために利用します。@n
                プロフィール画像のダウンロードはアプリケーション自身が行う必要があります。

                プロフィール画像 URL は HTTPS スキームです。@n
                HTTPS スキームに対応したダウンローダー（例：libcurl）を利用してください。
*/
class Profile
{
public:
    /*!
        @brief      コンストラクタです。
    */
    Profile() NN_NOEXCEPT;

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
        @brief      ユーザーのプロフィール画像 URL を取得します。

        @param[out] outUrl      プロフィール画像 URL。
        @param[in]  imageSize   画像サイズ。

        @return     処理結果。

        @pre
            - outUrl != nullptr
    */
    nn::Result GetProfileImageUrl(Url* outUrl, ImageSize imageSize) const NN_NOEXCEPT;

    /*!
        @brief      有効なデータを保持しているかどうかを確認します。

        @return     有効なデータを保持しているかどうか。
    */
    bool IsValid() const NN_NOEXCEPT;

private:
    //
    NN_FRIENDS_CLASS_STORAGE(256);
};

}}
