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

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace fs {

//! @name セーブデータ関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      セーブデータの配置場所として利用するホスト PC 上のパスを設定します。
*
*   @param[in]  rootPath   セーブデータの配置場所とする、ホスト PC 上のディレクトリパス
*
*   @return     なし
*
*   @pre
*       - rootPath がホスト PC 上に存在するディレクトリへの絶対パスまたは相対パスである。
*       - rootPath のディレクトリの区切り文字が '/' または '\' である。
*       - rootPath が相対パスの場合に基点より上の階層に遡っていない。
*       - rootPath が終端の NULL 文字を含めず 0 バイト以上 nn::fs::EntryNameLengthMax バイト以下の文字列である。
*
*   @details    この API はデバッグ用途に限り使用可能です。@n
*               セーブデータのマウント処理の前に実行する必要があります。@n
*               詳細は NintendoSDK ドキュメントの @confluencelink{107320205, セーブデータ} を参照してください。@n
*               @n
*               rootPath には相対パスを指定することができます。この場合、起点となるパスは実行環境に応じて異なります。@n
*               詳細は NintendoSDK ドキュメントの @confluencelink{213598244, ホスト PC} を参照してください。@n
*               rootPath に "" を指定した場合は相対パスの起点を指定したことになります。@n
*               起点より上の階層に遡るような相対パス（例："../"、"./dir/../../"）は指定できません。
*/
void SetSaveDataRootPath(const char* rootPath) NN_NOEXCEPT;


//! @}

//! @name デバッグ用途 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      デバッグ用セーブデータをマウントします。
*
*   @param[in]  name       マウント名
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                成功しました。
*   @retval     ResultTargetLocked           セーブデータが使用中です。
*   @retval     ResultMountNameAlreadyExists name で表されるファイルシステムが既に存在しています。
*   @retval     ResultTargetNotFound         セーブデータの配置場所が見つかりません。
*
*   @pre
*       - name が @link mount_name 有効なマウント名 @endlink である。
*       - 対象のセーブデータが存在する。
*       - @link supported_filesystem サポートされているファイルシステム @endlink に記載されている同時にマウントできる数の上限を越えない。
*
*   @details    この API はデバッグ用途に限り使用可能です。@n
*               セーブデータをマウント名 name でマウントします。
*/
Result MountSaveDataForDebug(const char* name) NN_NOEXCEPT;

//! @}

}}
