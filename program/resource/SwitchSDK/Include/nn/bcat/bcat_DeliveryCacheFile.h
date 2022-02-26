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

class IDeliveryCacheFileService;

}}

/*!
    @brief      データ配信キャッシュのファイルを扱うクラスです。
*/
class DeliveryCacheFile
{
private:
    NN_DISALLOW_COPY(DeliveryCacheFile);
    NN_DISALLOW_MOVE(DeliveryCacheFile);

public:
    /*!
        @brief      同時に開くことができるハンドルの最大数です。
    */
    static const int HandleCountMax = 8;

public:
    /*!
        @brief      コンストラクタです。
    */
    DeliveryCacheFile() NN_NOEXCEPT;

    /*!
        @brief      デストラクタです。

        @details
                    オブジェクトの破棄時に、 @ref DeliveryCacheFile::Close を呼び出します。
    */
    ~DeliveryCacheFile() NN_NOEXCEPT;

    /*!
        @brief      ファイルを開きます。

        @param[in]  path    パス。

        @return     処理結果。
        @retval     ResultTooManyHandles    ファイルを開きすぎている。

        @pre
            - path != nullptr
            - データ配信キャッシュストレージをマウントしている。

        @details
                    パスは (dirName)/(fileName) 形式で記述してください。
    */
    nn::Result Open(const char* path) NN_NOEXCEPT;

    /*!
        @brief      ファイルを開きます。

        @param[in]  dirName     ディレクトリ名。
        @param[in]  fileName    ファイル名。

        @return     処理結果。
        @retval     ResultTooManyHandles    ファイルを開きすぎている。

        @pre
            - データ配信キャッシュストレージをマウントしている。
    */
    nn::Result Open(const DirectoryName& dirName, const FileName& fileName) NN_NOEXCEPT;

    /*!
        @brief      ファイルを閉じます。

        @details
                    本関数は複数回呼び出しても問題ありません。
    */
    void Close() NN_NOEXCEPT;

    /*!
        @brief      ファイルを読み込みます。

        @param[out] outSize 読み込んだサイズ。
        @param[in]  offset  先頭からのオフセット。
        @param[in]  buffer  読み込みバッファ。
        @param[in]  size    読み込みバッファのサイズ。

        @return     処理結果。

        @pre
            - outSize != nullptr
            - offset >= 0
            - buffer != nullptr
            - size > 0
            - ファイルを開いている状態である。
    */
    nn::Result Read(size_t* outSize, int64_t offset, void* buffer, size_t size) NN_NOEXCEPT;

    /*!
        @brief      ファイルサイズを取得します。

        @return     ファイルサイズ。

        @pre
            - ファイルを開いている状態である。
    */
    int64_t GetSize() NN_NOEXCEPT;

    /*!
        @brief      ダイジェスト値を取得します。

        @return     ダイジェスト値。

        @pre
            - ファイルを開いている状態である。
    */
    Digest GetDigest() NN_NOEXCEPT;

private:
    //
    detail::ipc::IDeliveryCacheFileService* m_Handle;
};

}}
