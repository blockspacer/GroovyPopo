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
#include <nn/os.h>

namespace nn { namespace bcat {

namespace detail { namespace ipc {

class IDeliveryCacheProgressService;

}}

/*!
    @brief      データ配信の進捗を扱うクラスです。
*/
class DeliveryCacheProgress
{
private:
    NN_DISALLOW_COPY(DeliveryCacheProgress);
    NN_DISALLOW_MOVE(DeliveryCacheProgress);

public:
    /*!
        @brief      コンストラクタです。
    */
    DeliveryCacheProgress() NN_NOEXCEPT;

    /*!
        @brief      デストラクタです。
    */
    ~DeliveryCacheProgress() NN_NOEXCEPT;

    /*!
        @brief      進捗を更新します。
    */
    void Update() NN_NOEXCEPT;

    /*!
        @brief      進捗の状態を取得します。

        @return     進捗の状態。

        @details
                    @ref DeliveryCacheProgress::Update を呼び出すことで、状態が更新されます。
    */
    DeliveryCacheProgressStatus GetStatus() const NN_NOEXCEPT;

    /*!
        @brief      ダウンロード中ファイルの親ディレクトリ名を取得します。

        @return     ダウンロード中ファイルの親ディレクトリ名。

        @details
                    進捗の状態が以下の場合、有効な値が返ります。

                    - DeliveryCacheProgressStatus_Download
                    - DeliveryCacheProgressStatus_Commit

                    @ref DeliveryCacheProgress::Update を呼び出すことで、状態と値が更新されます。
    */
    DirectoryName GetCurrentDirectoryName() const NN_NOEXCEPT;

    /*!
        @brief      ダウンロード中ファイルのファイル名を取得します。

        @return     ダウンロード中ファイルのファイル名。

        @details
                    進捗の状態が以下の場合、有効な値が返ります。

                    - DeliveryCacheProgressStatus_Download

                    @ref DeliveryCacheProgress::Update を呼び出すことで、状態と値が更新されます。
    */
    FileName GetCurrentFileName() const NN_NOEXCEPT;

    /*!
        @brief      ダウンロード中ファイルのダウンロード済みサイズを取得します。

        @return     ダウンロード中ファイルのダウンロード済みサイズ。

        @details
                    進捗の状態が以下の場合、有効な値が返ります。

                    - DeliveryCacheProgressStatus_Download

                    @ref DeliveryCacheProgress::Update を呼び出すことで、状態と値が更新されます。
    */
    int64_t GetCurrentDownloaded() const NN_NOEXCEPT;

    /*!
        @brief      ダウンロード中ファイルのサイズを取得します。

        @return     ダウンロード中ファイルのサイズ。

        @details
                    進捗の状態が以下の場合、有効な値が返ります。

                    - DeliveryCacheProgressStatus_Download

                    @ref DeliveryCacheProgress::Update を呼び出すことで、状態と値が更新されます。
    */
    int64_t GetCurrentTotal() const NN_NOEXCEPT;

    /*!
        @brief      ダウンロード処理全体のダウンロード済みサイズを取得します。

        @return     ダウンロード処理全体のダウンロード済みサイズ。

        @details
                    進捗の状態が以下の場合、有効な値が返ります。

                    - DeliveryCacheProgressStatus_Download
                    - DeliveryCacheProgressStatus_Commit
                    - DeliveryCacheProgressStatus_Done

                    @ref DeliveryCacheProgress::Update を呼び出すことで、状態と値が更新されます。
    */
    int64_t GetWholeDownloaded() const NN_NOEXCEPT;

    /*!
        @brief      ダウンロード処理全体の合計サイズを取得します。

        @return     ダウンロード処理全体の合計サイズ。

        @details
                    本関数で取得できる値は配信中ファイルすべての合計サイズではなく、新規追加ファイル・更新ファイルの合計サイズになります。

                    進捗の状態が以下の場合、有効な値が返ります。

                    - DeliveryCacheProgressStatus_Download
                    - DeliveryCacheProgressStatus_Commit
                    - DeliveryCacheProgressStatus_Done

                    @ref DeliveryCacheProgress::Update を呼び出すことで、状態と値が更新されます。
    */
    int64_t GetWholeTotal() const NN_NOEXCEPT;

    /*!
        @brief      処理結果を取得します。

        @return     処理結果。

        @details
                    進捗の状態が以下の場合、有効な値が返ります。

                    - DeliveryCacheProgressStatus_Done

                    @ref DeliveryCacheProgress::Update を呼び出すことで、状態と値が更新されます。
    */
    nn::Result GetResult() const NN_NOEXCEPT;

public:
    // internal use only
    void Attach(detail::ipc::IDeliveryCacheProgressService* handle) NN_NOEXCEPT;

private:
    //
    mutable nn::os::Mutex m_Mutex;
    //
    detail::ipc::IDeliveryCacheProgressService* m_Handle;
    nn::os::SystemEvent m_Event;
    //
    Bit64 m_Storage[512 / sizeof (Bit64)];

private:
    //
    void Detach() NN_NOEXCEPT;
};

}}
