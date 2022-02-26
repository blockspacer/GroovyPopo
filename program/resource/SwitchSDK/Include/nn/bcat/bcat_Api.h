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

#include <nn/bcat/bcat_Types.h>
#include <nn/bcat/bcat_Result.h>
#include <nn/bcat/bcat_DeliveryCacheProgress.h>

namespace nn { namespace bcat {

/*!
    @name 初期化
    @{
*/

/*!
    @brief      ライブラリの初期化を行います。

    @details
                本関数の呼び出しにより、システムはアプリケーションがデータ配信機能を利用することを検知します。@n
                そのため、アプリケーション起動後すぐに本関数を呼び出すことを推奨します。

                データ配信機能を利用するための設定を行うため、本関数の完了には数十ミリ秒かかる場合があります。

                本関数の呼び出しによりバックグラウンド同期は中断され、以降、アプリケーション起動中はバックグラウンド同期は行われません。@n
                アプリケーションを終了することで、バックグラウンド同期は再開されます。

                ファイルのコミット処理は即時に中断できないため、停止するまで数秒かかる場合があります。（本関数の実行時間に影響はありません）

                本ライブラリの API はライブラリの初期化が行われていなかった場合、自動でライブラリの初期化を行います。@n
                本関数は複数回呼び出しても問題ありません。
*/
void Initialize() NN_NOEXCEPT;

/*!
    @}
    @name データ配信
    @{
*/

/*!
    @brief      データ配信キャッシュストレージをマウントします。

    @return     処理結果。
    @retval     ResultStorageNotFound       データ配信キャッシュストレージが存在していない。
    @retval     ResultLocked                データ配信キャッシュストレージはロックされている。
    @retval     ResultServiceUnavailable    データ配信機能が利用可能な状態ではない。

    @details
                本関数を呼び出したアプリケーションのバックグラウンド同期が行われている間、システムはデータ配信キャッシュストレージをロックします。@n
                この場合、本関数は ResultLocked を返します。

    @see
        - Initialize
*/
nn::Result MountDeliveryCacheStorage() NN_NOEXCEPT;

/*!
    @brief      データ配信キャッシュストレージをアンマウントします。

    @pre
        - データ配信キャッシュストレージをマウントしている。
        - DeliveryCacheFile や DeliveryCacheDirectory を 1 つも Open していない。
*/
void UnmountDeliveryCacheStorage() NN_NOEXCEPT;

/*!
    @brief      データ配信キャッシュストレージのディレクトリを列挙します。

    @param[out] outCount            取得したディレクトリ名数。
    @param[out] outDirectoryNames   ディレクトリ名リスト。
    @param[in]  count               ディレクトリ名リストの要素数。

    @return     処理結果。

    @pre
        - outCount != nullptr
        - outDirectoryNames != nullptr
        - count > 0
        - データ配信キャッシュストレージをマウントしている。

    @details
                ディレクトリは順不同です。@n
                また、ディレクトリの最大数は @ref DeliveryCacheDirectoryCountMax になります。
*/
nn::Result EnumerateDeliveryCacheDirectory(int* outCount, DirectoryName* outDirectoryNames, int count) NN_NOEXCEPT;

/*!
    @brief      データ配信キャッシュの同期を要求します。（即時同期）

    @param[out] outProgress 進捗オブジェクト。

    @return     処理結果。
    @retval     ResultTooManySyncRequests           同期要求を出しすぎている。
    @retval     ResultInternetRequestNotAccepted    インターネット接続要求が受理されていない。

    @pre
        - outProgress != nullptr
        - データ配信キャッシュストレージをマウントしていない。

    @details
                同期要求はシステム内の実行キューによって管理されています。@n
                システムは同期要求を 1 つずつ処理するため、未実行の同期要求は実行キューに蓄積されます。

                実行キューが満杯になり、同期要求が受け付けられなかった場合、 ResultTooManySyncRequests が返ります。@n
                そのため、直前の同期処理が完了するまでは次の同期要求を行わないようにすることを推奨します。

                本関数を呼ぶ前に、インターネット接続要求が受理されている必要があります。@n
                インターネット接続要求が受理されてない場合、 ResultInternetRequestNotAccepted が返ります。
*/
nn::Result RequestSyncDeliveryCache(DeliveryCacheProgress* outProgress) NN_NOEXCEPT;

/*!
    @brief      データ配信キャッシュの同期を要求します。（即時同期・ディレクトリ指定）

    @param[out] outProgress 進捗オブジェクト。
    @param[in]  dirName     ディレクトリ名。

    @return     処理結果。
    @retval     ResultTooManySyncRequests           同期要求を出しすぎている。
    @retval     ResultInternetRequestNotAccepted    インターネット接続要求が受理されていない。

    @pre
        - outProgress != nullptr
        - dirName != nullptr
        - データ配信キャッシュストレージをマウントしていない。

    @details
                本関数はディレクトリを指定して同期を要求します。@n
                指定外のディレクトリに更新がある場合でも、それらのディレクトリの同期処理は行われません。

                アプリケーションのシーケンス毎にアクセスするディレクトリが明確なのであれば、対象を絞ることで同期にかかる時間を減らすことができます。@n
                ただし、指定外のディレクトリ更新には関与しないため、データの配信状況によってはデータ配信キャッシュストレージの容量が不足する可能性があります。

                容量が不足した場合、 @ref DeliveryCacheProgress::GetResult は ResultUsableSpaceNotEnough を返します。@n
                アプリケーションはこのエラーを検知した場合、ディレクトリを指定しない即時同期 API を呼び出して復旧処理を行う必要があります。

                指定したディレクトリが存在しない場合、 @ref DeliveryCacheProgress::GetResult は ResultNotFound を返します。

                同期要求はシステム内の実行キューによって管理されています。@n
                システムは同期要求を 1 つずつ処理するため、未実行の同期要求は実行キューに蓄積されます。

                実行キューが満杯になり、同期要求が受け付けられなかった場合、 ResultTooManySyncRequests が返ります。@n
                そのため、直前の同期処理が完了するまでは次の同期要求を行わないようにすることを推奨します。

                本関数を呼ぶ前に、インターネット接続要求が受理されている必要があります。@n
                インターネット接続要求が受理されてない場合、 ResultInternetRequestNotAccepted が返ります。

    @attention
                エラーハンドリングが複雑になるため、特に理由がなければディレクトリを指定しない即時同期 API を使用してください。
*/
nn::Result RequestSyncDeliveryCache(DeliveryCacheProgress* outProgress, const char* dirName) NN_NOEXCEPT;

/*!
    @brief      データ配信キャッシュの同期要求をキャンセルします。

    @pre
        - データ配信キャッシュストレージをマウントしていない。

    @details
                @ref RequestSyncDeliveryCache によって発生した同期要求をすべてキャンセルします。

                本関数は即座に処理を返します。@n
                アプリケーションは、本関数の呼び出し後、進捗の状態が完了状態になるまで待機することを推奨します。

                キャンセルされた同期要求の処理結果は ResultCanceledByUser になります。

                コミット処理中にキャンセルした場合、そのコミット処理が完了するまで同期処理は継続されます。@n
                これは、ファイルの保存処理を強制中断することでデータ配信キャッシュストレージが不完全な状態になってしまうのを防ぐためです。

                コミット処理中以外でキャンセルした場合、同期処理は即座に完了します。@n
                ダウンロードしたファイルはコミット処理の段階でデータ配信キャッシュストレージに保存されます。@n
                ダウンロード中またはダウンロード済みのファイルは、この時点ではまだデータ配信キャッシュストレージに保存されていないことに注意してください。

    @see
        - DeliveryCacheProgress::GetResult
*/
void CancelSyncDeliveryCacheRequest() NN_NOEXCEPT;

/*!
    @}
*/

}}
