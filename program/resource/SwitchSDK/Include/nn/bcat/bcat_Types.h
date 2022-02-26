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

namespace nn { namespace bcat {

/*!
    @brief      データ配信キャッシュストレージのディレクトリの最大数です。
*/
const int DeliveryCacheDirectoryCountMax = 100;

/*!
    @brief      データ配信キャッシュストレージの 1 ディレクトリあたりの最大ファイル数です。
*/
const int DeliveryCacheFileCountMaxPerDirectory = 100;

/*!
    @brief      データ配信の進捗の状態です。

    @details
                各状態は、以下のようになります。

                - Queued
                -- 実行待ちの状態です。@n
                   システムに複数の同期要求が登録されている状況で、先に出された要求を処理している状態です。
                - Connect
                -- ネットワーク接続中です。
                - ProcessList
                -- データ配信リストの処理中です。@n
                   データ配信リストの受信・解析を行います。
                - Download
                -- ファイルのダウンロード処理中です。@n
                   現在ダウンロードしているファイルの情報（名前、サイズ等）が取得できます。
                - Commit
                -- ファイルのコミット処理中です。@n
                   ダウンロードしたファイルをアプリケーションのデータ配信キャッシュストレージに移動します。@n
                   また、配信停止されたファイルの削除を行います。
                - Done
                -- 完了状態です。@n
                   正常終了した場合、同期が完了しています。@n
                   異常終了した場合、コミット処理が行われたディレクトリのみ同期が取れた状態になります。@n
                   正常終了したかどうかは、 @ref DeliveryCacheProgress::GetResult で確認できます。
*/
enum DeliveryCacheProgressStatus
{
    DeliveryCacheProgressStatus_None        = 0, //!< 進捗なし。
    DeliveryCacheProgressStatus_Queued      = 1, //!< 実行待ち。
    DeliveryCacheProgressStatus_Connect     = 2, //!< ネットワーク接続中。
    DeliveryCacheProgressStatus_ProcessList = 3, //!< データ配信リストの処理中。
    DeliveryCacheProgressStatus_Download    = 4, //!< ダウンロード処理中。
    DeliveryCacheProgressStatus_Commit      = 5, //!< コミット処理中。
    DeliveryCacheProgressStatus_Done        = 9  //!< 完了。
};

/*!
    @brief      ファイル名です。

    @details
                ファイル名に指定できる文字は以下の通りです。

                - 英数
                - アンダースコア
                - ピリオド

                ファイル名の末尾にピリオドを指定することはできません。
*/
struct FileName
{
    char value[32]; //!< ファイル名を格納するバッファ。

public:
    /*!
        @brief      ファイル名の有効性を評価します。

        @return     ファイル名が有効かどうか。
    */
    bool IsValid() const NN_NOEXCEPT;
};

/*!
    @brief      ディレクトリ名です。

    @details
                ディレクトリ名に指定できる文字は以下の通りです。

                - 英数
                - アンダースコア
                - ハイフン
*/
struct DirectoryName
{
    char value[32]; //!< ディレクトリ名を格納するバッファ。

public:
    /*!
        @brief      ディレクトリ名の有効性を評価します。

        @return     ディレクトリ名が有効かどうか。
    */
    bool IsValid() const NN_NOEXCEPT;
};

/*!
    @brief      ダイジェスト値です。

    @details
                ダイジェスト値は関連するオブジェクトから一意に決まる 16 バイトのデータ列です。@n
                オブジェクトの同一性検証に利用することができます。
*/
struct Digest
{
    Bit64 value[2]; //!< ダイジェスト値を格納するバッファ。
};

/*!
    @brief      データ配信キャッシュディレクトリのエントリー情報です。
*/
struct DeliveryCacheDirectoryEntry
{
    FileName name; //!< ファイル名。
    int64_t size;  //!< ファイルサイズ。
    Digest digest; //!< ダイジェスト値。
};

}}
