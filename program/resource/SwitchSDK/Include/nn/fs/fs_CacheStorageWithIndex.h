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
*   @brief  インデックス付きキャッシュストレージ関連 API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace fs {

/**
* @brief キャッシュストレージリストを取得するためのハンドルです。
*/
struct CacheStorageListHandle
{
    void* handle;
};

/**
* @brief キャッシュストレージの情報を表す構造体です。
*/
struct CacheStorageInfo
{
    int                    index;                //!< キャッシュストレージのインデックス
    uint8_t                reserved[28];
};
static_assert(sizeof(CacheStorageInfo) == 32, "sizeof(nn::fs::CacheStorageInfo) must be 32.");
NN_STATIC_ASSERT(std::is_pod<CacheStorageInfo>::value);

//! @name インデックス付きキャッシュストレージ関連 API
//! @{

//--------------------------------------------------------------------------
/**
*   @brief      キャッシュストレージを作成します。
*
*   @details    指定されたサイズのキャッシュストレージを作成します。@n
*               空き容量が不足している場合、システムが容量確保のためのUIやエラービューアを自動で表示します。@n
*               詳細は NintendoSDK ドキュメントの @confluencelink{276903214, インデックス付きキャッシュストレージ} を参照してください。@n
*
*   @param[in]  index                         キャッシュストレージのインデックス
*   @param[in]  cacheStorageSize              作成するキャッシュストレージのデータ保存領域のサイズ
*   @param[in]  cacheStorageJournalSize       作成するキャッシュストレージのジャーナリング領域のサイズ
*
*   @pre
*       - nmeta ファイルの NintendoSdkMeta/Application/CacheStorageDataAndJournalSizeMax にキャッシュストレージの最大サイズが記載されている
*       - nmeta ファイルの NintendoSdkMeta/Application/CacheStorageIndexMax  にインデックスの最大値が記載されている
*       - cacheStorageSize + cacheStorageJournalSize <= CacheStorageDataAndJournalSizeMax
*       - index <= CacheStorageIndexMax
*       - 同インデックスのキャッシュストレージが存在しない
*       - 同インデックスのキャッシュストレージをマウントしていない
*
*   @post
*       - データ保存領域 >= cacheStorageSize かつ ジャーナリング領域 >= cacheStorageJournalSize で指定したインデックスのキャッシュストレージが存在する
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                                成功しました。
*   @retval     ResultUsableSpaceNotEnoughForCacheStorage    空き容量が不足しています。
*/
Result CreateCacheStorage(int index, int64_t cacheStorageSize, int64_t cacheStorageJournalSize) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      指定されたインデックスのキャッシュストレージのサイズを取得します。
*
*   @param[out]  outCacheStorageSize            キャッシュストレージのサイズ
*   @param[out]  outCacheStorageJournalSize     キャッシュストレージのジャーナリング領域のサイズ
*   @param[in]   index                          サイズを取得したいキャッシュストレージのインデックス
*
*   @pre
*       - 指定されたインデックスのキャッシュストレージが存在する
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess               成功しました。
*/
Result GetCacheStorageSize(int64_t* outCacheStorageSize, int64_t* outCacheStorageJournalSize, int index) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      指定されたインデックスのキャッシュストレージを削除します。
*
*   @param[in]  index                      削除したいキャッシュストレージのインデックス
*
*   @pre
*       - 指定されたインデックスのキャッシュストレージが存在する
*       - 指定されたインデックスのキャッシュストレージをマウントしていない
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess               成功しました。
*/
Result DeleteCacheStorage(int index) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
*   @brief      キャッシュストレージリストをオープンします。
*
*   @param[out] outValue    キャッシュストレージリストのハンドル
*
*   @pre
*       - outValue が有効なメモリを指している。
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                   成功しました。
*/
Result OpenCacheStorageList(CacheStorageListHandle* outValue) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
*   @brief      キャッシュストレージを列挙します。
*
*   @param[out] outValue         キャッシュストレージの個数
*   @param[in]  infoBuffer       CacheStorageInfo の格納先バッファ
*   @param[in]  handle           キャッシュストレージリストのハンドル
*   @param[in]  infoBufferCount  infoBuffer の要素数
*
*   @pre
*       - infoBuffer の長さが infoBufferCount 以上である。
*       - infoBufferCount >= 0
*       - handle が有効なハンドルである。
*       - outValue が有効なメモリを指している。
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess                       成功しました。
*/
Result ReadCacheStorageList(int* outValue, CacheStorageInfo *infoBuffer, CacheStorageListHandle handle, int infoBufferCount) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
*   @brief      キャッシュストレージリストをクローズします。
*
*   @param[in]  handle           キャッシュストレージリストのハンドル
*
*   @retval     なし
*/
void CloseCacheStorageList(CacheStorageListHandle handle) NN_NOEXCEPT;

//! @}

}} // namespace nn::fs
