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

/**
* @file
* @brief    文字列フォーマットを行う API の宣言です。
*/

#include <nn/nn_Common.h>
#include <cstdarg>

namespace nn { namespace util {

/*!
    @brief @ref FormatString(),  @ref VFormatString() が生成した文字列を受け取るコールバック関数の型です。

    @param[in]  arg             ユーザ定義の引数です。
    @param[in]  pCharacters     生成された文字列です。ヌル終端されていません。
    @param[in]  count           pCharacters が指す先の文字数です。
*/
typedef void (*FormatStringOutputFunction)(uintptr_t arg, const char* pCharacters, int count);

/*!
    @brief 文字列をフォーマットします。

    @param[in]  pOutputFunction     生成された文字列を順次受け取る出力コールバック関数です。
    @param[in]  outputFunctionArg   出力コールバック関数に与えられる、ユーザ定義の引数です。
    @param[in]  pFormat             printf スタイルの書式指定文字列です。
    @param[in]  ...                 書式指定文字列に与える引数です。
*/
void FormatString(
    FormatStringOutputFunction pOutputFunction,
    uintptr_t outputFunctionArg,
    const char* pFormat,
    ...) NN_NOEXCEPT;

/*!
    @brief 文字列をフォーマットします。書式指定文字列に与える引数を
           std::va_list で受け取る以外は、@ref FormatString() と同一です。

    @param[in]  pOutputFunction     出力文字列を順次受け取る出力コールバック関数です。
    @param[in]  outputFunctionArg   出力コールバック関数に与えられる、ユーザ定義の引数です。
    @param[in]  pFormat             printf スタイルの書式指定文字列です。詳細は @ref FormatString() を参照してください。
    @param[in]  formatArg           書式指定文字列に与える引数です。
*/
void VFormatString(
    FormatStringOutputFunction pOutputFunction,
    uintptr_t outputFunctionArg,
    const char* pFormat,
    std::va_list formatArg) NN_NOEXCEPT;

/*!
    @brief 文字列をフォーマットします。

    @param[in]  buffer          生成された文字列を格納するバッファです。
    @param[in]  bufferLength    バッファの要素数です。
    @param[in]  pFormat         printf スタイルの書式指定文字列です。詳細は @ref FormatString() を参照してください。
    @param[in]  ...             書式指定文字列に与える引数です。

    @return     生成された文字列の文字数（ヌル文字を含まない）を返します。
                出力が切りつめられた場合でも、切りつめられる前の文字数を返します。
*/
int SNPrintf(char* buffer, size_t bufferLength, const char* pFormat, ...) NN_NOEXCEPT;

/*!
    @brief 文字列をフォーマットします。書式指定文字列に与える引数を std::va_list で受け取る以外は、
           @ref SNPrintf() と同一です。

    @param[in]  buffer          出力文字列を格納するバッファです。
    @param[in]  bufferLength    バッファの要素数です。
    @param[in]  pFormat         printf スタイルの書式指定文字列です。詳細は @ref FormatString() を参照してください。
    @param[in]  formatArg       書式指定文字列に与える引数です。

    @return     生成された文字列の文字数（ヌル文字を含まない）を返します。
                出力が切りつめられた場合でも、切りつめられる前の文字数を返します。
*/
int VSNPrintf(char* buffer, size_t bufferLength, const char* pFormat, std::va_list formatArg) NN_NOEXCEPT;

}}
