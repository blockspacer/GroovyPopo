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
*   @brief  トランザクション管理 API の宣言
*/

#pragma once

namespace nn { namespace fs {

//! @name トランザクション管理 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      ジャーナリング機能が有効なファイルシステムに対する更新内容をコミットします。
*
*   @param[in]  name       対象のファイルシステムをマウントするときに指定したマウント名
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess               成功しました。
*
*   @pre
*       - name がマウント済みのマウント名である。
*       - name マウント内のファイルが OpenMode_Write を含むモードで開かれていない。
*
*   @details    マウント名で指定されたファイルシステムに対する更新内容をコミットします。@n
*               ジャーナリング機能が有効なファイルシステムに対してこの関数を呼ばないままアンマウントしたり、プログラムが停止した場合、@n
*               データの内容は前回コミットが成功した時点に巻き戻ります。@n
*               この関数の実行中にプログラムが停止した場合のデータの内容は、@n
*               「前回コミットが成功した時点の内容」か「全ての更新が反映された内容」のいずれかになり、@n
*               破損したり中途半端な内容になることはありません。@n
*               ジャーナリング機能が有効でないファイルシステムに対してこの関数を呼ぶ必要はありません。@n
*               各ファイルシステムにおいてジャーナリング機能が有効かどうかは @link supported_filesystem サポートされているファイルシステム @endlink を参照してください。
*/
Result Commit(const char* name) NN_NOEXCEPT;

/**
*   @brief      マルチコミット機能が有効な複数のファイルシステムに対する更新内容を同時にコミットします。
*
*   @param[in]  nameArray    ファイルシステムをマウントするときに指定したマウント名の配列
*   @param[in]  nameCount    マウント名の数
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess               成功しました。
*
*   @pre
*       - nameArray の各要素がマウント済みのマウント名である。
*       - nameArray の各要素がマルチコミット機能が有効なファイルシステムのマウント名である。
*       - nameArray の各要素が書き込み可能なファイルシステムのマウント名である。
*       - nameArray のいずれかのマウント内のファイルが OpenMode_Write を含むモードで開かれていない。
*       - nameArray に重複するマウント名が含まれない。
*       - nameCount >= 0
*       - nameCount <= 10
*
*   @details    マウント名で指定された複数のファイルシステムに対する更新内容を同時にコミットします。@n
*               この関数の実行中にプログラムが停止した場合のデータの内容は、@n
*               「指定したすべてのファイルシステムで、前回コミットが成功した時点の内容」か
*               「指定したすべてのファイルシステムで、全ての更新が反映された内容」のいずれかになり、@n
*               いずれかのファイルシステムだけが反映されるなど中途半端な内容になることはありません。@n
*               各ファイルシステムにおいてマルチコミット機能が有効かどうかは @link supported_filesystem サポートされているファイルシステム @endlink を参照してください。
*/
Result Commit(const char* const* nameArray, int nameCount) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs
