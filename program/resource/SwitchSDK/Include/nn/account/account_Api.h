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
    @brief このファイルは一般アプリケーションや本体システムでの利用を想定した API を宣言します。
 */

#include <nn/account/account_Types.h>

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace account {

//! @name アカウントシステムの一般的な機能
//! @{

/**
    @brief アカウントシステムに登録されているユーザーの数を取得します。

    @param[out] pOutCount ユーザー数の格納先

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理に成功しました。
        }
    @endretresult

    @pre
        - ライブラリが初期化済み
        - pOutCount != nullptr
    @post
        - *pOutCount >= 0 && *pOutCount <= UserCountMax

    @details
        アカウントシステムに登録されているユーザーの数を取得します。
        アカウントシステムに登録され得るユーザー数の上限は UserCountMax で、これより大きい値が得られることはありません。

        アカウントシステムに登録されているユーザーの一覧も含めて取得するには ListAllUsers() を使用してください。
*/
Result GetUserCount(int* pOutCount) NN_NOEXCEPT;

/**
    @brief ユーザーを指定して、そのユーザーがアカウントシステムに登録済みかを取得します。

    @param[out] pOutExistence ユーザーが登録済みかの格納先
    @param[in]  user 対象のユーザーを指す Uid

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理に成功しました。
        }
    @endretresult

    @pre
        - ライブラリが初期化済み
        - static_cast<bool>(user) == true
        - pOutExistence != nullptr

    @details
        指定された Uid に対応するユーザーが、アカウントシステムに登録されているかどうかを取得します。
*/
Result GetUserExistence(bool* pOutExistence, const Uid& user) NN_NOEXCEPT;

/**
    @brief アカウントシステムに登録されているユーザーの一覧を取得します。

    @param[out] pOutActualLength アカウントシステムに登録されているユーザーの数の格納先
    @param[out] outUsers ユーザーの一覧の格納先
    @param[in]  arrayLength outUsers の要素数

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理に成功しました。
        }
    @endretresult

    @pre
        - ライブラリが初期化済み
        - pOutActualLength != nullptr
        - pOutUsers != nullptr
        - arrayLength > 0
    @post
        - *pOutActualLength >= 0 && *pOutActualLength <= UserCountMax
        - outUsers[0, min(arrayLength, *pOutActualLength)) が有効な Uid を保持
        - outUsers[min(arrayLength, *pOutActualLength), arrayLength) が無効な Uid を保持

    @details
        アカウントシステムに登録されているユーザーの一覧を取得します。
        アカウントシステムに登録され得るユーザー数の上限は UserCountMax で、これより大きい一覧が得られることはありません。

        このリストには、 Closed 状態と Open 状態に関わらず、すべてのユーザーが列挙されます。
        リスト上のユーザーの列挙順序は、システムによって定められます。

        アカウントシステムに登録されているユーザーの数のみを取得する場合は GetUserCount() の使用を推奨します。
*/
Result ListAllUsers(int* pOutActualLength, Uid* outUsers, int arrayLength) NN_NOEXCEPT;

/**
    @brief 現在 Open 状態のユーザーの一覧を取得します。

    @param[out] pOutActualLength 現在 Open 状態のユーザー数の格納先
    @param[out] outUsers ユーザーの一覧の格納先
    @param[in]  arrayLength outUsers の要素数

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理に成功しました。
        }
    @endretresult

    @pre
        - ライブラリが初期化済み
        - pOutActualLength != nullptr
        - pOutUsers != nullptr
        - arrayLength > 0
    @post
        - *pOutActualLength >= 0 && *pOutActualLength <= UserCountMax
        - outUsers[0, min(arrayLength, *pOutActualLength)) が有効な Uid を保持
        - outUsers[min(arrayLength, *pOutActualLength), arrayLength) が無効な Uid を保持

    @details
        アカウントシステム上で、現在 Open 状態のユーザーの一覧を取得します。
        リスト上のユーザーの列挙順序は、システムによって定められます。
*/
Result ListOpenUsers(int* pOutActualLength, Uid* outUsers, int arrayLength) NN_NOEXCEPT;

/**
    @brief 最後に Open 状態に設定されたユーザーの Uid を取得します。

    @param[out] pOutUser 対象のユーザーを指す Uid の格納先

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理に成功しました。
        }
    @endretresult

    @pre
        - ライブラリが初期化済み
        - pOutUser != nullptr
    @post
        - 当該ユーザーが存在する場合、 static_cast<bool>(*pOutUser) == true
        - そうでない場合、 static_cast<bool>(*pOutUser) == false

    @details
        アカウントシステム上で、最後に Open 状態を設定されたひとりのユーザーの Uid を取得します。
        当該ユーザーが既に削除されているなど、返却すべき値が存在しない場合には *pOutUser は無効な値をもちます。

        この関数は、アプリケーションが独自にユーザー選択のGUIを作成する場合の、カーソルの初期位置などの参考に使用することを想定しています。
*/
Result GetLastOpenedUser(Uid* pOutUser) NN_NOEXCEPT;

/**
    @brief ユーザーを指定して、そのユーザーのニックネームを取得します。

    @param[out] pOut ニックネームの格納先
    @param[in]  user 対象のユーザーを指す Uid

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理に成功しました。
        }
        @handleresult{
            nn::account::ResultUserNotExist,
            対象のユーザーアカウントが存在しません。
        }
    @endretresult

    @pre
        - ライブラリが初期化済み
        - static_cast<bool>(user) == true
        - pOut != nullptr
        - user が指すユーザーが存在
    @post
        - pOut->name[0, NicknameBytesMax] にひとつ以上の '\0' が存在

    @details
        ユーザーを指定して、そのユーザーに設定されたニックネームを取得します。
        ユーザーに対してこれまでプロフィールが設定されていない場合、次の有効な値を返します。
        - pOUt->name[0] == '\0'
*/
Result GetNickname(Nickname* pOut, const Uid& user) NN_NOEXCEPT;

/**
    @brief ユーザーを指定して、そのユーザのプロフィール画像を取得します。

    @param[out] pOutActualSize プロフィール画像の実際の大きさ (バイト数) の格納先
    @param[out] outImage プロフィール画像の格納先
    @param[in] bufferSize outImage に書き込み可能な大きさ (バイト数)
    @param[in] user 対象のユーザーを指す Uid

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理に成功しました。
        }
        @handleresult{
            nn::account::ResultUserNotExist,
            対象のユーザーアカウントが存在しません。
        }
    @endretresult

    @pre
        - ライブラリが初期化済み
        - static_cast<bool>(user) == true
        - pOutActualSize != nullptr
        - user が指すユーザーが存在
    @post
        - *pOutActualSize が画像データの実際の大きさ (バイト数) を保持
        - outImage != nullptr のとき、 outImage[0, min(*pOutActualSize, bufferSize)) が画像データを保持
        - outImage の保持する画像データをJPEGとして解釈可能

    @details
        指定したユーザーのプロフィール画像を取得します。
        第 2 引数に nullptr を指定することで、プロフィール画像の大きさ (バイト数) のみを取得できます。

        プロフィール画像は下記の形式に従います。
        - 画像解像度は縦横各256px
        - 色空間はsRGB
        - ベースライン方式のJPEGフォーマット
            - YCbCr サンプリング比は 4:2:0, 4:2:2, 4:4:4 のいずれか
            - Exif情報が存在する場合、その内容は不定
        - 大きさ (バイト数) は128KiB (131,072バイト) を上限とする
*/
Result LoadProfileImage(size_t* pOutActualSize, void* outImage, size_t bufferSize, const Uid& user) NN_NOEXCEPT;

//! @}

}} // ~namespace nn::account
