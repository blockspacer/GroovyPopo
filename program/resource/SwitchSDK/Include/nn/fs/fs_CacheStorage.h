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
*   @brief キャッシュストレージ関連 API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace fs {

//! @name キャッシュストレージ関連 API
//! @{


//--------------------------------------------------------------------------
/**
*   @brief      キャッシュストレージをマウントします。
*
*   @param[in]  name       マウント名
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess               成功しました。
*
*   @pre
*       - name が @link mount_name 有効なマウント名 @endlink である。
*       - @link supported_filesystem サポートされているファイルシステム @endlink に記載されている同時にマウントできる数の上限を越えない。
*
*   @details    キャッシュストレージを利用するには事前の申請が必要です。@n
*               詳細は NintendoSDK ドキュメントの @confluencelink{224957652, キャッシュストレージ} を参照してください。
*/
Result MountCacheStorage(const char* name) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      インデックスを指定してキャッシュストレージをマウントします。
*
*   @param[in]  name       マウント名
*   @param[in]  index      マウントしたいキャッシュストレージのインデックス
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess               成功しました。
*   @retval     ResultTargetNotFound        指定したインデックスのキャッシュストレージが存在していない
*
*   @pre
*       - name が @link mount_name 有効なマウント名 @endlink である。
*       - 指定したインデックスのキャッシュストレージが存在している。
*       - @link supported_filesystem サポートされているファイルシステム @endlink に記載されている同時にマウントできる数の上限を越えない。
*
*   @details    キャッシュストレージを利用するには事前の申請が必要です。@n
*               詳細は NintendoSDK ドキュメントの @confluencelink{276903214, インデックス付きキャッシュストレージ} を参照してください。@n
*/
Result MountCacheStorage(const char* name, int index) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs

