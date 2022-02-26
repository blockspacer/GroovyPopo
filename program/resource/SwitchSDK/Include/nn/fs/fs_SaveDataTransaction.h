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
*   @brief  セーブデータトランザクション管理 API の宣言
*/

#pragma once

namespace nn { namespace fs {

//! @name セーブデータトランザクション管理 API
//! @{

//--------------------------------------------------------------------------
/**
*   @deprecated
*
*   @brief      セーブデータの更新をコミットします。
*
*   @param[in]  name       対象のセーブデータをマウントするときに指定したマウント名
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess               成功しました。
*
*   @pre
*       - name がマウント済みのセーブデータのマウント名である。
*       - name マウント内のファイルが OpenMode_Write を含むモードで開かれていない。
*
*   @details    セーブデータへの更新内容をコミットします。@n
*               この関数を呼ばないままアンマウントしたり、プログラムが停止した場合、@n
*               セーブデータは前回コミットが成功した時点の内容に巻き戻ります。@n
*               この関数の実行中にプログラムが停止した場合のセーブデータの内容は、@n
*               「前回コミットが成功した時点の内容」か「全ての更新が反映された内容」のいずれかになり、@n
*               破損したり中途半端な内容になることはありません。@n
*               この関数は非推奨です。 nn::fs::Commit() を使用してください。
*/
Result CommitSaveData(const char* name) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs
