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
* @brief zlib を使用したストリーミング deflate 伸長を行うための API の宣言
*/
namespace nn {
namespace util {


/** @brief      zlib 形式の圧縮データのストリーミング伸長用コンテキストです。
    @details    zlib 形式の圧縮データのストリーミング伸長用コンテキストです。メンバに直接アクセスしないでください。@n
                サイズが大きいため、スタック上に置く場合にはスタックサイズにご注意ください。@n
                本構造体は 8 Byte アラインされている必要があります。
 */
struct StreamingDecompressZlibContext
{
    NN_ALIGNAS(8) nn::Bit8 _context[40064];
};

/** @brief      gzip 形式の圧縮データのストリーミング伸長用コンテキストです。
    @details    gzip 形式の圧縮データのストリーミング伸長用コンテキストです。メンバに直接アクセスしないでください。@n
                サイズが大きいため、スタック上に置く場合にはスタックサイズにご注意ください。
                本構造体は 8 Byte アラインされている必要があります。
 */
typedef StreamingDecompressZlibContext StreamingDecompressGzipContext;

/** @brief      raw deflate 形式の圧縮データのストリーミング伸長用コンテキストです。
    @details    raw deflate 形式の圧縮データのストリーミング伸長用コンテキストです。メンバに直接アクセスしないでください。@n
                サイズが大きいため、スタック上に置く場合にはスタックサイズにご注意ください。
                本構造体は 8 Byte アラインされている必要があります。
 */
typedef StreamingDecompressZlibContext StreamingDecompressDeflateContext;


//! @name ストリーミング伸長
//! @{

/**
    @brief      zlib 形式のストリーミング伸長用コンテキストを初期化します。
    @param[in]  pContext         ストリーミング伸長用のコンテキスト

    @post       コンテキスト領域が初期化され、 StreamingDecompressZlib() が使用可能になります。

    @details    ストリーミング伸長用のコンテキスト領域の初期化を行い、 StreamingDecompressZlib() を使用可能な状態にします。
*/
void InitializeStreamingDecompressZlibContext(StreamingDecompressZlibContext* pContext) NN_NOEXCEPT;


/**
    @brief      Gzip 形式のストリーミング伸長用コンテキストを初期化します。
    @param[in]  pContext         ストリーミング伸長用のコンテキスト

    @post       コンテキスト領域が初期化され、 StreamingDecompressGzip() が使用可能になります。

　　　　@details    ストリーミング伸長用のコンテキスト領域の初期化を行い、 StreamingDecompressGzip() を使用可能な状態にします。
*/
void InitializeStreamingDecompressGzipContext(StreamingDecompressGzipContext* pContext) NN_NOEXCEPT;


/**
    @brief      raw deflate 形式のストリーミング伸長用コンテキストを初期化します。
    @param[in]  pContext         ストリーミング伸長用のコンテキスト

    @post       コンテキスト領域が初期化され、 StreamingDecompressDeflate() が使用可能になります。

    @details    ストリーミング伸長用のコンテキスト領域の初期化を行い、 StreamingDecompressDeflate() を使用可能な状態にします。
*/
void InitializeStreamingDecompressDeflateContext(StreamingDecompressDeflateContext* pContext) NN_NOEXCEPT;


/** @brief    zlib 形式のストリーミング deflate 伸長を行います。

    @param[out]      pOutDstSize            出力用バッファに書き込んだサイズ
    @param[out]      pOutConsumedSrcSize    入力バッファから読み込んだサイズ
    @param[out]      pDst                   伸長したデータを書き込むバッファへのポインタ
    @param[in]       dstSize                pDst が指すバッファのサイズ
    @param[in]       pSrc                   伸長するデータが格納されたバッファへのポインタ
    @param[in]       srcSize                pSrc が指すバッファのサイズ
    @param[in]       pContext               ストリーミング伸長用のコンテキスト領域へのポインタ

    @return     ストリーミング伸長に成功したかどうかを返します。@n
                失敗した場合はデータが壊れているかサポートされていない圧縮データが入力されています。

    @pre
                - InitializeStreamingZlibContext() でストリーミング伸長用のコンテキストが初期化されている必要があります。

    @details    zlib 形式の ストリーミング deflate 伸長を行います。@n
                サイズの大きな圧縮済みデータや出力バッファが十分に用意できないときにもストリーミングして読み出すことが可能です。@n
                本関数は指定された入力バッファを読みきるか出力先バッファがいっぱいになると true を返します。@n
                pOutDstSize と pOutConsumedSize を確認し、出力バッファと入力データを再設定し、繰り返し本関数を実行してください。@n
                全ての圧縮データの伸長処理が完了すると、 pOutDstSize と pOutConsumedSize は共に 0 となり、すべての伸長処理の完了を表します。@n
                本関数で扱えるデータサイズは 4GB までです。4GB 以上のデータを扱いたい場合は分けて読み込ませてください。
*/
bool StreamingDecompressZlib(size_t* pOutDstSize, size_t* pOutConsumedSrcSize,
                             void* pDst,          size_t dstSize,
                             const void* pSrc,    size_t srcSize,
                             StreamingDecompressZlibContext* pContext) NN_NOEXCEPT;


/** @brief    Gzip 形式のストリーミング deflate 伸長を行います。

    @param[out]      pOutDstSize            出力用バッファに書き込んだサイズ
    @param[out]      pOutConsumedSrcSize    入力バッファから読み込んだサイズ
    @param[out]      pDst                   伸長したデータを書き込むバッファへのポインタ
    @param[in]       dstSize                pDst が指すバッファのサイズ
    @param[in]       pSrc                   伸長するデータが格納されたバッファへのポインタ
    @param[in]       srcSize                pSrc が指すバッファのサイズ
    @param[in]       pContext               ストリーミング伸長用のコンテキスト領域へのポインタ

    @return     ストリーミング伸長に成功したかどうかを返します。@n
                失敗した場合はデータが壊れているかサポートされていない圧縮データが入力されています。

    @pre
                - InitializeStreamingGzipContext()　でストリーミング伸長用のコンテキスト用領域が初期化されている必要があります。

    @details    Gzip 形式の ストリーミング deflate 伸長を行います。@n
                サイズの大きな圧縮済みデータや出力バッファが十分に用意できないときにもストリーミングして読み出すことが可能です。@n
                本関数は指定された入力バッファを読みきるか出力先バッファがいっぱいになると true を返します。@n
                pOutDstSize と pOutConsumedSize を確認し、出力バッファと入力データを再設定し、繰り返し本関数を実行してください。@n
                全ての圧縮データの伸長処理が完了すると、 pOutDstSize と pOutConsumedSize は共に 0 となり、すべての伸長処理の完了を表します。@n
                本関数で扱えるデータサイズは 4GB までです。4GB 以上のデータを扱いたい場合は分けて読み込ませてください。
*/
bool StreamingDecompressGzip(size_t* pOutDstSize, size_t* pOutConsumedSrcSize,
                             void* pDst,          size_t dstSize,
                             const void* pSrc,    size_t srcSize,
                             StreamingDecompressGzipContext* pContext) NN_NOEXCEPT;


/** @brief    raw deflate 形式のストリーミング deflate 伸長を行います。

    @param[out]      pOutDstSize            出力用バッファに書き込んだサイズ
    @param[out]      pOutConsumedSrcSize    入力バッファから読み込んだサイズ
    @param[out]      pDst                   伸長したデータを書き込むバッファへのポインタ
    @param[in]       dstSize                pDst が指すバッファのサイズ
    @param[in]       pSrc                   伸長するデータが格納されたバッファへのポインタ
    @param[in]       srcSize                pSrc が指すバッファのサイズ
    @param[in]       pContext               ストリーミング伸長用のコンテキスト領域へのポインタ

    @return     ストリーミング伸長に成功したかどうかを返します。@n
                失敗した場合はデータが壊れているかサポートされていない圧縮データが入力されています。

    @pre
                - InitializeStreamingDeflateContext()　でストリーミング伸長用のコンテキスト用領域が初期化されている必要があります。

    @details    raw deflate 形式の ストリーミング deflate 伸長を行います。@n
                サイズの大きな圧縮済みデータや出力バッファが十分に用意できないときにもストリーミングして読み出すことが可能です。@n
                本関数は指定された入力バッファを読みきるか出力先バッファがいっぱいになると true を返します。@n
                pOutDstSize と pOutConsumedSize を確認し、出力バッファと入力データを再設定し、繰り返し本関数を実行してください。@n
                全ての圧縮データの伸長処理が完了すると、 pOutDstSize と pOutConsumedSize は共に 0 となり、すべての伸長処理の完了を表します。@n
                本関数で扱えるデータサイズは 4GB までです。4GB 以上のデータを扱いたい場合は分けて読み込ませてください。
*/
bool StreamingDecompressDeflate(size_t* pOutDstSize, size_t* pOutConsumedSrcSize,
                                void* pDst,          size_t dstSize,
                                const void* pSrc,    size_t srcSize,
                                StreamingDecompressDeflateContext* pContext) NN_NOEXCEPT;

//! @}
// ~ストリーミング伸長

} //util
} //nn
