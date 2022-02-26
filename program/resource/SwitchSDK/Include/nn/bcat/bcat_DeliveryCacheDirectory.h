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

namespace nn { namespace bcat {

namespace detail { namespace ipc {

class IDeliveryCacheDirectoryService;

}}

/*!
    @brief      データ配信キャッシュのディレクトリを扱うクラスです。
*/
class DeliveryCacheDirectory
{
private:
    NN_DISALLOW_COPY(DeliveryCacheDirectory);
    NN_DISALLOW_MOVE(DeliveryCacheDirectory);

public:
    /*!
        @brief      同時に開くことができるハンドルの最大数です。
    */
    static const int HandleCountMax = 8;

public:
    /*!
        @brief      コンストラクタです。
    */
    DeliveryCacheDirectory() NN_NOEXCEPT;

    /*!
        @brief      デストラクタです。

        @details
                    オブジェクトの破棄時に、 @ref DeliveryCacheDirectory::Close を呼び出します。
    */
    ~DeliveryCacheDirectory() NN_NOEXCEPT;

    /*!
        @brief      ディレクトリを開きます。

        @param[in]  dirName ディレクトリ名。

        @return     処理結果。
        @retval     ResultTooManyHandles    ディレクトリを開きすぎている。

        @pre
            - dirName != nullptr
            - データ配信キャッシュストレージをマウントしている。
    */
    nn::Result Open(const char* dirName) NN_NOEXCEPT;

    /*!
        @brief      ディレクトリを開きます。

        @param[in]  dirName ディレクトリ名。

        @return     処理結果。
        @retval     ResultTooManyHandles    ディレクトリを開きすぎている。

        @pre
            - データ配信キャッシュストレージをマウントしている。
    */
    nn::Result Open(const DirectoryName& dirName) NN_NOEXCEPT;

    /*!
        @brief      ディレクトリを閉じます。

        @details
                    本関数は複数回呼び出しても問題ありません。
    */
    void Close() NN_NOEXCEPT;

    /*!
        @brief      ディレクトリの子エントリーを列挙します。

        @param[out] outCount    取得したエントリー数。
        @param[out] outEntries  エントリーリスト。
        @param[in]  count       エントリーリストの要素数。

        @return     処理結果。

        @pre
            - outCount != nullptr
            - outEntries != nullptr
            - count > 0
            - ディレクトリを開いている状態である。

        @details
                    子エントリーは順不同です。@n
                    子エントリーの最大数は @ref DeliveryCacheFileCountMaxPerDirectory になります。
    */
    nn::Result Read(int* outCount, DeliveryCacheDirectoryEntry* outEntries, int count) NN_NOEXCEPT;

    /*!
        @brief      ディレクトリの子エントリー数を取得します。

        @return     子エントリー数。

        @pre
            - ディレクトリを開いている状態である。
    */
    int GetCount() NN_NOEXCEPT;

private:
    //
    detail::ipc::IDeliveryCacheDirectoryService* m_Handle;
};

}}
