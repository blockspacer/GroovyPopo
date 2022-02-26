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

namespace nn { namespace fs {

/**
* @brief セーブデータ拡張で指定できる単位サイズです。
*/
const size_t SaveDataExtensionUnitSize = 1 * 1024 * 1024;

//! @name セーブデータ関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      セーブデータを拡張します。
*
*   @details    user に紐づくユーザーアカウントセーブデータのサイズを拡張します。@n
*               拡張の必要がない場合は何もせず ResultSuccess を返します。@n
*               空き容量が不足している場合、システムが容量確保のためのUIやエラービューアを自動で表示します。@n
*               詳細は NintendoSDK ドキュメントの @confluencelink{107320205, セーブデータ} を参照してください。
*
*   @param[in]  user                対象のユーザーを指す Uid
*   @param[in]  saveDataSize        拡張後のセーブデータのデータ保存領域のサイズ
*   @param[in]  saveDataJournalSize 拡張後のセーブデータのジャーナリング領域のサイズ
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess               成功しました。
*   @retval     ResultTargetLocked          セーブデータが使用中です。
*   @retval     ResultUsableSpaceNotEnough  空き容量が不足しています。
*
*   @pre
*       - static_cast<bool>(user) == true
*       - 対象のセーブデータが存在する。
*       - 対象セーブデータがマウントされていない。
*       - 対象セーブデータが拡張回数限度に達していない。
*       - saveDataSize, saveDataJournalSize がそれぞれメタデータで指定した拡張上限値以下である。
*       - saveDataSize, saveDataJournalSize がそれぞれ nn::fs::SaveDataExtensionUnitSize の倍数である。
*/
Result ExtendSaveData(const nn::account::Uid& user, int64_t saveDataSize, int64_t saveDataJournalSize) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      セーブデータの現在のサイズを取得します。
*
*   @details    user に紐づくユーザーアカウントセーブデータのデータ保存領域およびジャーナリング領域のサイズを取得します。
*
*   @param[out] outSaveDataSize        セーブデータのデータ保存領域のサイズ
*   @param[out] outSaveDataJournalSize セーブデータのジャーナリング領域のサイズ
*   @param[in]  user                   対象のユーザーを指す Uid
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess               成功しました。
*
*   @pre
*       - outSaveDataSize が有効なメモリを指している。
*       - outSaveDataJournalSize が有効なメモリを指している。
*       - static_cast<bool>(user) == true
*       - 対象のセーブデータが存在する。
*/
Result GetSaveDataSize(int64_t* outSaveDataSize, int64_t* outSaveDataJournalSize, const nn::account::Uid& user) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs
