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

 /**
* @file
*
* @brief zlib を使用した deflate 伸長を行うための API の宣言
*/
namespace nn {
namespace util {


/** @brief     DecompressZlib() で必要となるワークバッファのサイズ (バイト数) を定義した定数です。
*/
const size_t DecompressZlibWorkBufferSize = 7160;

/** @brief     DecompressGzip() で必要となるワークバッファのサイズ (バイト数) を定義した定数です。
*/
const size_t DecompressGzipWorkBufferSize = DecompressZlibWorkBufferSize;

/** @brief     DecompressDeflate() で必要となるワークバッファのサイズ (バイト数) を定義した定数です。
*/
const size_t DecompressDeflateWorkBufferSize = DecompressZlibWorkBufferSize;


//! @name 伸長
//! @{

/**
* @brief      gzip 形式の圧縮データから圧縮前のデータサイズを取得します。
*
* @param[in]  pSrc        圧縮データが格納されたバッファへのポインタ
* @param[in]  srcSize     圧縮データのサイズ
*
* @return     圧縮前のデータサイズを返します。
*
* @pre        pSrc には gzip 形式の圧縮データを指定する必要があります。
*
* @details    gzip 形式の圧縮データから圧縮前のデータサイズを取得します。 srcSize にはバッファのサイズではなく、圧縮データのサイズを入力してください。
*/
size_t GetGzipDecompressedSize(const void* pSrc, size_t srcSize) NN_NOEXCEPT;


/** @brief      zlib 形式の deflate 伸長を行います。

        @param[out] pDst       伸長したデータを書き込むバッファへのポインタ
        @param[in]  dstSize    pDst が指すバッファのサイズ
        @param[in]  pSrc       伸長するデータが格納されたバッファへのポインタ
        @param[in]  srcSize    伸長するデータのサイズ
        @param[in]  pWork      ワークバッファへのポインタ
        @param[in]  workSize   ワークバッファのサイズ

        @return     伸長に成功したかどうかを返します。@n
                    失敗した場合はサポートされていないフォーマットのデータか壊れたデータが入力されています。

        @pre
                    - pWork には DecompressZlibWorkBufferSize 以上の領域が必要です。@n
                    - dstSize == (圧縮前のデータサイズ)

        @details    zlib を使用した zlib 形式の deflate 伸長を行います。@n
                    zlib は deflate アルゴリズムを用いてデータ圧縮/伸長を行うオープンソースライブラリです。 http://www.zlib.net/ で配布されています。@n
                    本関数は入力データや出力バッファを分割せずに一括で伸長を行います。@n
                    本関数は dstSize を超えて pDst に書き込むことはありません。@n
                    本関数で扱えるデータサイズは 4GB までです。@n
                    分割したストリーミング伸長を行いたい場合は StreamingDecompressZlib() を使用してください。
*/
bool DecompressZlib(void* pDst,       size_t dstSize,
                    const void* pSrc, size_t srcSize,
                    void* pWork,      size_t workSize) NN_NOEXCEPT;


/** @brief      Gzip 形式の deflate 伸長を行います。

        @param[out] pDst       伸長したデータを書き込むバッファへのポインタ
        @param[in]  dstSize    pDst が指すバッファのサイズ
        @param[in]  pSrc       伸長するデータが格納されたバッファへのポインタ
        @param[in]  srcSize    伸長するデータのサイズ
        @param[in]  pWork      ワークバッファへのポインタ
        @param[in]  workSize   ワークバッファのサイズ

        @return     伸長に成功したかどうかを返します。@n
                    失敗した場合はサポートされていないフォーマットのデータか壊れたデータが入力されています。

        @pre
                    - pWork には DecompressGzipWorkBufferSize 以上の領域が必要です。@n
                    - dstSize == (圧縮前のデータサイズ)

        @details    zlib を使用した Gzip 形式の deflate 伸長を行います。@n
                    zlib は deflate アルゴリズムを用いてデータ圧縮/伸長を行うオープンソースライブラリです。 http://www.zlib.net/ で配布されています。@n
                    本関数は入力データや出力バッファを分割せずに一括で伸長を行います。@n
                    本関数は dstSize を超えて pDst に書き込むことはありません。@n
                    本関数で扱えるデータサイズは 4GB までです。@n
                    分割したストリーミング伸長を行いたい場合は StreamingDecompressGzip() を使用してください。
*/
bool DecompressGzip(void* pDst,       size_t dstSize,
                    const void* pSrc, size_t srcSize,
                    void* pWork,      size_t workSize) NN_NOEXCEPT;


/** @brief      raw deflate 形式の deflate 伸長を行います。

        @param[out] pDst       伸長したデータを書き込むバッファへのポインタ
        @param[in]  dstSize    pDst が指すバッファのサイズ
        @param[in]  pSrc       伸長するデータが格納されたバッファへのポインタ
        @param[in]  srcSize    伸長するデータのサイズ
        @param[in]  pWork      ワークバッファへのポインタ
        @param[in]  workSize   ワークバッファのサイズ

        @return     伸長に成功したかどうかを返します。@n
                    失敗した場合はサポートされていないフォーマットのデータか壊れたデータが入力されています。

        @pre
                    - pWork には DecompressDeflateWorkBufferSize 以上の領域が必要です。@n
                    - dstSize == (圧縮前のデータサイズ)

        @details    zlib を使用した raw deflate 形式の deflate 伸長を行います。@n
                    zlib は deflate アルゴリズムを用いてデータ圧縮/伸長を行うオープンソースライブラリです。 http://www.zlib.net/ で配布されています。@n
                    本関数は入力データや出力バッファを分割せずに一括で伸長を行います。@n
                    本関数は dstSize を超えて pDst に書き込むことはありません。@n
                    本関数で扱えるデータサイズは 4GB までです。@n
                    分割したストリーミング伸長を行いたい場合は StreamingDecompressDeflate() を使用してください。
*/
bool DecompressDeflate(void* pDst,       size_t dstSize,
                       const void* pSrc, size_t srcSize,
                       void* pWork,      size_t workSize) NN_NOEXCEPT;

//! @}
// ~伸長

} //util
} //nn
