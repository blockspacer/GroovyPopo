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

/** @brief     CompressZlib() で必要となるワークバッファのサイズ (バイト数) を計算するマクロです。
    @details   メモリ使用量を指定する場合はこちらのマクロを使って計算したワークバッファサイズのバッファを用意して CompressZlib() に渡してください。
*/
#define NN_UTIL_CALCULATE_COMPRESS_ZLIB_WORKBUFFER_SIZE(memLevel) (137024 + (1 << ((memLevel) + 9)))

/** @brief     CompressGzip() で必要となるワークバッファのサイズ (バイト数) を計算するマクロです。
    @details   メモリ使用量を指定する場合はこちらのマクロを使って計算したワークバッファサイズのバッファを用意して CompressGzip() に渡してください。
*/
#define NN_UTIL_CALCULATE_COMPRESS_GZIP_WORKBUFFER_SIZE(memLevel)  NN_UTIL_CALCULATE_COMPRESS_ZLIB_WORKBUFFER_SIZE(memLevel)


/** @brief     CompressDeflate() で必要となるワークバッファのサイズ (バイト数) を計算するマクロです。
    @details   メモリ使用量を指定する場合はこちらのマクロを使って計算したワークバッファサイズのバッファを用意して CompressDeflate() に渡してください。
*/
#define NN_UTIL_CALCULATE_COMPRESS_DEFLATE_WORKBUFFER_SIZE(memLevel)  NN_UTIL_CALCULATE_COMPRESS_ZLIB_WORKBUFFER_SIZE(memLevel)

/**
* @file
*
* @brief zlib を用いた deflate 圧縮を行うための API の宣言
*/
namespace nn {
namespace util {

/** @brief    圧縮レベルと使用メモリレベルを指定しない CompressZlib() で必要となるワークバッファのサイズ (バイト数) を定義した定数です。
*/
const size_t CompressZlibWorkBufferSizeDefault = 268096;

/** @brief    圧縮レベルと使用メモリレベルを指定しない CompressGzip() で必要となるワークバッファのサイズ (バイト数) を定義した定数です。
*/
const size_t CompressGzipWorkBufferSizeDefault = CompressZlibWorkBufferSizeDefault;

/** @brief    圧縮レベルと使用メモリレベルを指定しない CompressDeflate() で必要となるワークバッファのサイズ (バイト数) を定義した定数です。
*/
const size_t CompressDeflateWorkBufferSizeDefault = CompressZlibWorkBufferSizeDefault;



//! @name 圧縮
//! @{


/** @brief    zlib 形式の deflate 圧縮を行う関数です。

        @param[out] pOutSize    圧縮後のデータサイズ
        @param[out] pDst        圧縮したデータを書き込むバッファへのポインタ
        @param[in]  dstSize     pDst が指すバッファのサイズ
        @param[in]  pSrc        圧縮するデータが格納されたバッファへのポインタ
        @param[in]  srcSize     圧縮するデータのサイズ
        @param[in]  pWork       ワークバッファへのポインタ
        @param[in]  workSize    ワークバッファのサイズ

        @return     圧縮に成功したかどうかを返します。@n
                    失敗した場合は出力用バッファのサイズを大きくし、再度実行してください。

        @pre        pWork には CompressZlibWorkBufferSizeDefault 以上の領域が必要です。@n

        @details    zlib を使用した zlib 形式の deflate 圧縮を行います。@n
                    zlib は deflate アルゴリズムを用いてデータ圧縮/伸長を行うオープンソースライブラリです。 http://www.zlib.net/ で配布されています。@n
                    本関数では zlib 内部の圧縮レベルは 6 (Z_DEFAULT_COMPRESSION)、メモリの消費量(memLevel)は 8 に設定されています。@n
                    本関数は入力データや出力バッファを分割せずに一括で圧縮を行います。@n
                    本関数で圧縮したデータには先頭と末尾に zlib 形式のヘッダとフッタが付与され、 @ref DecompressZlib() で展開できます。@n
                    本関数で圧縮した圧縮データから圧縮前のサイズは取得できません。@n
                    圧縮前のデータサイズは別途保持するようにしてください。@n
                    本関数で扱えるデータサイズは 4GB までです。
*/
bool CompressZlib(size_t* pOutSize,
                  void* pDst,       size_t dstSize,
                  const void* pSrc, size_t srcSize,
                  void* pWork,      size_t workSize) NN_NOEXCEPT;


/** @brief     圧縮レベルと使用メモリレベルを指定し、zlib 形式の deflate 圧縮を行う関数です。

        @param[out] pOutSize            圧縮後のデータサイズ
        @param[out] pDst                圧縮したデータを書き込むバッファへのポインタ
        @param[in]  dstSize             pDst が指すバッファのサイズ
        @param[in]  pSrc                圧縮するデータが格納されたバッファへのポインタ
        @param[in]  srcSize             圧縮するデータのサイズ
        @param[in]  pWork               ワークバッファへのポインタ
        @param[in]  workSize            ワークバッファのサイズ
        @param[in]  memLevel            メモリの消費量
        @param[in]  compressionLevel    圧縮レベル

        @return     圧縮に成功したかどうかを返します。@n
                    失敗した場合は出力用バッファのサイズを大きくし、再度実行してください。

        @pre        pWork には NN_UTIL_CALCULATE_COMPRESS_ZLIB_WORKBUFFER_SIZE(memLevel) 以上の領域が必要です。@n
                    1 <= memLevel && memLevel <= 9 @n
                    0 <= compressLevel && compressLevel <= 9

        @details    本関数はデフォルト値で圧縮する CompressZlib() に圧縮レベルと使用メモリレベルを指定できるようにオーバーロードした関数です。@n
                    本関数ではメモリの消費量と圧縮レベルを指定できます。@n
                    メモリの消費量は 1 以上 9 以下の値を指定することが可能であり、数値を大きくするほど、より多くのメモリを使用し、圧縮率と圧縮速度が向上します。@n
                    圧縮レベルは 0 以上 9 以下の値を指定することが可能であり、一般的に数値を小さくすると圧縮時間が短くなり、大きくすると圧縮後のサイズが小さくなります。@n
                    0 を指定すると無圧縮となります。
*/
bool CompressZlib(size_t* pOutSize,
                  void* pDst,       size_t dstSize,
                  const void* pSrc, size_t srcSize,
                  void* pWork,      size_t workSize,
                  int memLevel,     int compressionLevel) NN_NOEXCEPT;


/** @brief    Gzip 形式の deflate 圧縮を行う関数です。

        @param[out] pOutSize    圧縮後のデータサイズ
        @param[out] pDst        圧縮したデータを書き込むバッファへのポインタ
        @param[in]  dstSize     pDst が指すバッファのサイズ
        @param[in]  pSrc        圧縮するデータが格納されたバッファへのポインタ
        @param[in]  srcSize     圧縮するデータのサイズ
        @param[in]  pWork       ワークバッファへのポインタ
        @param[in]  workSize    ワークバッファのサイズ

        @return     圧縮に成功したかどうかを返します。@n
                    失敗した場合は出力用バッファのサイズを大きくし、再度実行してください。

        @pre        pWork には CompressGzipWorkBufferSizeDefault 以上の領域が必要です。@n

        @details    zlib を使用した Gzip 形式の deflate 圧縮を行います。@n
                    zlib は deflate アルゴリズムを用いてデータ圧縮/伸長を行うオープンソースライブラリです。 http://www.zlib.net/ で配布されています。@n
                    本関数では zlib 内部の圧縮レベルは 6 (Z_DEFAULT_COMPRESSION)、メモリの消費量(memLevel)は 8 に設定されています。@n
                    本関数は入力データや出力バッファを分割せずに一括で圧縮を行います。@n
                    本関数で圧縮したデータには先頭と末尾に gzip 形式のヘッダとフッタが付与され、 @ref DecompressGzip() で展開できます。@n
                    また、gzip 形式の圧縮データの圧縮前のサイズは GetGzipDecompressedSize() で取得できます。@n
                    本関数で扱えるデータサイズは 4GB までです。
*/
bool CompressGzip(size_t* pOutSize,
                  void* pDst,       size_t dstSize,
                  const void* pSrc, size_t srcSize,
                  void* pWork,      size_t workSize) NN_NOEXCEPT;


/** @brief     圧縮レベルと使用メモリレベルを指定し、gzip 形式の deflate 圧縮を行う関数です。

        @param[out] pOutSize            圧縮後のデータサイズ
        @param[out] pDst                圧縮したデータを書き込むバッファへのポインタ
        @param[in]  dstSize             pDst が指すバッファのサイズ
        @param[in]  pSrc                圧縮するデータが格納されたバッファへのポインタ
        @param[in]  srcSize             圧縮するデータのサイズ
        @param[in]  pWork               ワークバッファへのポインタ
        @param[in]  workSize            ワークバッファのサイズ
        @param[in]  memLevel            メモリの消費量
        @param[in]  compressionLevel    圧縮レベル

        @return     圧縮に成功したかどうかを返します。@n
                    失敗した場合は出力用バッファのサイズを大きくし、再度実行してください。

        @pre        pWork には NN_UTIL_CALCULATE_COMPRESS_GZIP_WORKBUFFER_SIZE(memLevel) 以上の領域が必要です。@n
                    1 <= memLevel && memLevel <= 9 @n
                    0 <= compressLevel && compressLevel <= 9

        @details    本関数はデフォルト値で圧縮する CompressGzip() に圧縮レベルと使用メモリレベルを指定できるようにオーバーロードした関数です。@n
                    本関数ではメモリの消費量と圧縮レベルを指定できます。@n
                    メモリの消費量は 1 以上 9 以下の値を指定することが可能であり、数値を大きくするほど、より多くのメモリを使用し、圧縮率と圧縮速度が向上します。@n
                    圧縮レベルは 0 以上 9 以下の値を指定することが可能であり、一般的に数値を小さくすると圧縮時間が短くなり、大きくすると圧縮後のサイズが小さくなります。@n
                    0 を指定すると無圧縮となります。
*/
bool CompressGzip(size_t* pOutSize,
                  void* pDst,       size_t dstSize,
                  const void* pSrc, size_t srcSize,
                  void* pWork,      size_t workSize,
                  int memLevel,     int compressionLevel) NN_NOEXCEPT;


/** @brief    raw deflate 形式の deflate 圧縮を行う関数です。

        @param[out] pOutSize    圧縮後のデータサイズ
        @param[out] pDst        圧縮したデータを書き込むバッファへのポインタ
        @param[in]  dstSize     pDst が指すバッファのサイズ
        @param[in]  pSrc        圧縮するデータが格納されたバッファへのポインタ
        @param[in]  srcSize     圧縮するデータのサイズ
        @param[in]  pWork       ワークバッファへのポインタ
        @param[in]  workSize    ワークバッファのサイズ

        @return     圧縮に成功したかどうかを返します。@n
                    失敗した場合は出力用バッファのサイズを大きくし、再度実行してください。

        @pre        pWork には CompressDeflateWorkBufferSizeDefault 以上の領域が必要です。@n

        @details    zlib を使用した raw deflate 形式の deflate 圧縮を行います。@n
                    zlib は deflate アルゴリズムを用いてデータ圧縮/伸長を行うオープンソースライブラリです。 http://www.zlib.net/ で配布されています。@n
                    本関数では zlib 内部の圧縮レベルは 6 (Z_DEFAULT_COMPRESSION)、メモリの消費量(memLevel)は 8 に設定されています。@n
                    本関数は入力データや出力バッファを分割せずに一括で圧縮を行います。@n
                    この関数で圧縮したデータは @ref DecompressDeflate() で展開できます。@n
                    本関数で圧縮した圧縮データから圧縮前のサイズは取得できません。@n
                    圧縮前のデータサイズは別途保持するようにしてください。@n
                    本関数で扱えるデータサイズは 4GB までです。
*/
bool CompressDeflate(size_t* pOutSize,
                     void* pDst,       size_t dstSize,
                     const void* pSrc, size_t srcSize,
                     void* pWork,      size_t workSize) NN_NOEXCEPT;


/** @brief     圧縮レベルと使用メモリレベルを指定し、raw deflate 形式の deflate 圧縮を行う関数です。

        @param[out] pOutSize            圧縮後のデータサイズ
        @param[out] pDst                圧縮したデータを書き込むバッファへのポインタ
        @param[in]  dstSize             pDst が指すバッファのサイズ
        @param[in]  pSrc                圧縮するデータが格納されたバッファへのポインタ
        @param[in]  srcSize             圧縮するデータのサイズ
        @param[in]  pWork               ワークバッファへのポインタ
        @param[in]  workSize            ワークバッファのサイズ
        @param[in]  memLevel            メモリの消費量
        @param[in]  compressionLevel    圧縮レベル

        @return     圧縮に成功したかどうかを返します。@n
                    失敗した場合は出力用バッファのサイズを大きくし、再度実行してください。

        @pre        pWork には NN_UTIL_CALCULATE_COMPRESS_DEFLATE_WORKBUFFER_SIZE(memLevel) 以上の領域が必要です。@n
                    1 <= memLevel && memLevel <= 9 @n
                    0 <= compressLevel && compressLevel <= 9

        @details    本関数はデフォルト値で圧縮する CompressGzip() に圧縮レベルと使用メモリレベルを指定できるようにオーバーロードした関数です。@n
                    本関数ではメモリの消費量と圧縮レベルを指定できます。@n
                    メモリの消費量は 1 以上 9 以下の値を指定することが可能であり、数値を大きくするほど、より多くのメモリを使用し、圧縮率と圧縮速度が向上します。@n
                    圧縮レベルは 0 以上 9 以下の値を指定することが可能であり、一般的に数値を小さくすると圧縮時間が短くなり、大きくすると圧縮後のサイズが小さくなります。@n
                    0 を指定すると無圧縮となります。
*/
bool CompressDeflate(size_t* pOutSize,
                     void* pDst,       size_t dstSize,
                     const void* pSrc, size_t srcSize,
                     void* pWork,      size_t workSize,
                     int memLevel,     int compressionLevel) NN_NOEXCEPT;

//! @}
// ~圧縮

} //util
} //nn
