/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
*   @file
*   @brief  セーブデータ関連 API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/account/account_Types.h>
#include <nn/fs/fs_SaveDataExtension.h>
#include <nn/fs/fs_SaveDataTransaction.h>
#include <nn/nn_ApplicationId.h>

namespace nn { namespace fs {

//! @name セーブデータ関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      セーブデータを作成します。
*
*   @details    user に紐づくユーザーアカウントセーブデータ、およびアプリケーションに必要な他のセーブデータを作成します。@n
*               既にセーブデータが存在する場合、何もせず ResultSuccess を返します。@n
*               user 以外のユーザーアカウントに紐づくユーザーアカウントセーブデータは作成しません。@n
*               空き容量が不足している場合、システムが容量確保のためのUIやエラービューアを自動で表示します。@n
*               詳細は NintendoSDK ドキュメントの @confluencelink{107320205, セーブデータ} を参照してください。
*
*   @param[in]  user       対象のユーザーを指す Uid
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess               成功しました。
*   @retval     ResultUsableSpaceNotEnough  空き容量が不足しています。
*
*   @pre
*       - nn::account::Initialize() でアカウントシステムのライブラリが初期化済みである。
*       - static_cast<bool>(user) == true
*       - user が指すユーザーアカウントが存在する。
*/
Result EnsureSaveData(const nn::account::Uid& user) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      ユーザーを指定してセーブデータをマウントします。
*
*   @details    指定したユーザーのセーブデータをマウント名 name でマウントします。
*
*   @param[in]  name       マウント名
*   @param[in]  user       対象のユーザーを指す Uid
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                成功しました。
*   @retval     ResultTargetLocked           セーブデータが使用中です。
*   @retval     ResultMountNameAlreadyExists name で表されるファイルシステムが既に存在しています。
*
*   @pre
*       - name が @link mount_name 有効なマウント名 @endlink である。
*       - static_cast<bool>(user) == true
*       - 対象のセーブデータが存在する。
*       - @link supported_filesystem サポートされているファイルシステム @endlink に記載されている同時にマウントできる数の上限を越えない。
*/
Result MountSaveData(const char* name, const nn::account::Uid& user) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
*   @brief      ユーザーを指定してアプリケーション ID に対応するアプリケーションのセーブデータを読み込み専用でマウントします。
*
*   @details    指定したアプリケーション ID およびユーザーに対応するセーブデータをマウント名 name で読み込み専用としてマウントします。
*               マウントしたセーブデータに対して書き込み操作を行うと abort します。
*               書き込み可能な状態でマウントする必要がある場合は、任天堂の窓口までご相談ください。
*
*   @param[in]  name           マウント名
*   @param[in]  applicationId  対象のアプリケーション ID
*   @param[in]  user           対象のユーザーを指す Uid
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                成功しました。
*   @retval     ResultTargetLocked           セーブデータが使用中です。
*   @retval     ResultMountNameAlreadyExists name で表されるファイルシステムが既に存在しています。
*
*   @pre
*       - name が @link mount_name 有効なマウント名 @endlink である。
*       - static_cast<bool>(user) == true
*       - 対象のセーブデータが存在する。
*       - @link supported_filesystem サポートされているファイルシステム @endlink に記載されている同時にマウントできる数の上限を越えない。
*/
Result MountSaveDataReadOnly(const char* name, const nn::ApplicationId applicationId, const nn::account::Uid& user) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      指定したユーザーアカウントセーブデータが存在するかどうかを取得します。
*
*   @param[in]  user       対象のユーザーを指す Uid
*
*   @return     存在すれば true が返ります。
*
*   @pre
*       - マウントされている数が @link supported_filesystem サポートされているファイルシステム @endlink に記載されている同時にマウントできる数の上限未満である。
*/
bool IsSaveDataExisting(const nn::account::Uid& user) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      指定したアプリケーション ID に対応するユーザーアカウントセーブデータが存在するかどうかを取得します。
*
*   @param[in]  applicationId  対象のアプリケーション ID
*   @param[in]  user           対象のユーザーを指す Uid
*
*   @return     存在すれば true が返ります。
*
*   @pre
*       - マウントされている数が @link supported_filesystem サポートされているファイルシステム @endlink に記載されている同時にマウントできる数の上限未満である。
*/
bool IsSaveDataExisting(const nn::ApplicationId applicationId, const nn::account::Uid& user) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs
