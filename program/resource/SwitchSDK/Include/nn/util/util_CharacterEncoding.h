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
 * @brief    文字エンコーディング変換を行う API の宣言です。
 */

#include <nn/nn_Macro.h>
#include <nn/util/util_CharacterEncodingResult.h>
#include <stdint.h>

namespace nn { namespace util {

    //! @{
    //! @name 文字エンコーディング変換

    /*!
    @brief        文字コードを UTF-8 から UTF-16 に変換します。
    @param[out]   pDst       変換先の配列。終端文字は付加されません。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  pDst の末尾に終端文字が付加されない点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf8ToUtf16Native(
        uint16_t* pDst, int dstLength, const char* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-8 から UTF-16 に変換します。
    @param[out]   pDst       変換先の配列。終端文字は付加されません。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  pDst の末尾に終端文字が付加されない点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf8ToUtf16Native(
        char16_t* pDst, int dstLength, const char* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-8 から UTF-16 に変換します。終端文字まで変換されます。
    @param[out]   pDst       変換先の配列。終端文字が付加されます。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまで変換を行います。@n
                  pDst の末尾に終端文字が付加される点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf8ToUtf16Native(
        uint16_t* pDst, int dstLength, const char* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-8 から UTF-16 に変換します。終端文字まで変換されます。
    @param[out]   pDst       変換先の配列。終端文字が付加されます。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまで変換を行います。@n
                  pDst の末尾に終端文字が付加される点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf8ToUtf16Native(
        char16_t* pDst, int dstLength, const char* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-16 から UTF-8 に変換します。
    @param[out]   pDst       変換先の配列。終端文字は付加されません。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  pDst の末尾に終端文字が付加されない点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf16NativeToUtf8(
        char* pDst, int dstLength, const uint16_t* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-16 から UTF-8 に変換します。
    @param[out]   pDst       変換先の配列。終端文字は付加されません。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  pDst の末尾に終端文字が付加されない点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf16NativeToUtf8(
        char* pDst, int dstLength, const char16_t* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-16 から UTF-8 に変換します。終端文字まで変換されます。
    @param[out]   pDst       変換先の配列。終端文字が付加されます。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまで変換を行います。@n
                  pDst の末尾に終端文字が付加される点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf16NativeToUtf8(
        char* pDst, int dstLength, const uint16_t* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-16 から UTF-8 に変換します。終端文字まで変換されます。
    @param[out]   pDst       変換先の配列。終端文字が付加されます。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまで変換を行います。@n
                  pDst の末尾に終端文字が付加される点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf16NativeToUtf8(
        char* pDst, int dstLength, const char16_t* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief        UTF-8 から UTF-16 に変換するために必要な変換先の配列の長さを取得します。
    @param[out]   pOutLength 変換先の配列に文字列を書き込むのに必要な配列の長さ。終端文字の分は含まれません。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success          - 正常に変換されました。
    @retval       CharacterEncodingResult_InvalidFormat    - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  変換元の配列が配列末尾の終端文字を含まない場合、pOutLength は終端文字を含まない配列の長さであることに注意してください。
    */
    CharacterEncodingResult GetLengthOfConvertedStringUtf8ToUtf16Native(
        int* pOutLength, const char* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        UTF-8 から UTF-16 に変換するために必要な変換先の配列の長さを取得します。終端文字まで読み込まれます。
    @param[out]   pOutLength 変換先の配列に文字列を書き込むのに必要な配列の長さ。終端文字の分は含まれません。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success          - 正常に変換されました。
    @retval       CharacterEncodingResult_InvalidFormat    - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまでの長さを取得します。@n
                  pOutLength は終端文字を含まない配列の長さであることに注意してください。
    */
    CharacterEncodingResult GetLengthOfConvertedStringUtf8ToUtf16Native(
        int* pOutLength, const char* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief        UTF-16 から UTF-8 に変換するために必要な変換先の配列の長さを取得します。
    @param[out]   pOutLength 変換先の配列に文字列を書き込むのに必要な配列の長さ。終端文字の分は含まれません。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success          - 正常に変換されました。
    @retval       CharacterEncodingResult_InvalidFormat    - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  変換元の配列が配列末尾の終端文字を含まない場合、pOutLength は終端文字を含まない配列の長さであることに注意してください。
    */
    CharacterEncodingResult GetLengthOfConvertedStringUtf16NativeToUtf8(
        int* pOutLength, const uint16_t* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        UTF-16 から UTF-8 に変換するために必要な変換先の配列の長さを取得します。
    @param[out]   pOutLength 変換先の配列に文字列を書き込むのに必要な配列の長さ。終端文字の分は含まれません。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success          - 正常に変換されました。
    @retval       CharacterEncodingResult_InvalidFormat    - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  変換元の配列が配列末尾の終端文字を含まない場合、pOutLength は終端文字を含まない配列の長さであることに注意してください。
    */
    CharacterEncodingResult GetLengthOfConvertedStringUtf16NativeToUtf8(
        int* pOutLength, const char16_t* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        UTF-16 から UTF-8 に変換するために必要な変換先の配列の長さを取得します。終端文字まで読み込まれます。
    @param[out]   pOutLength 変換先の配列に文字列を書き込むのに必要な配列の長さ。終端文字の分は含まれません。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success          - 正常に変換されました。
    @retval       CharacterEncodingResult_InvalidFormat    - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまでの長さを取得します。@n
                  pOutLength は終端文字を含まない配列の長さであることに注意してください。
    */
    CharacterEncodingResult GetLengthOfConvertedStringUtf16NativeToUtf8(
        int* pOutLength, const uint16_t* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief        UTF-16 から UTF-8 に変換するために必要な変換先の配列の長さを取得します。終端文字まで読み込まれます。
    @param[out]   pOutLength 変換先の配列に文字列を書き込むのに必要な配列の長さ。終端文字の分は含まれません。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success          - 正常に変換されました。
    @retval       CharacterEncodingResult_InvalidFormat    - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまでの長さを取得します。@n
                  pOutLength は終端文字を含まない配列の長さであることに注意してください。
    */
    CharacterEncodingResult GetLengthOfConvertedStringUtf16NativeToUtf8(
        int* pOutLength, const char16_t* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-8 から UTF-32 に変換します。
    @param[out]   pDst       変換先の配列。終端文字は付加されません。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  pDst の末尾に終端文字が付加されない点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf8ToUtf32(
        uint32_t* pDst, int dstLength, const char* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-8 から UTF-32 に変換します。
    @param[out]   pDst       変換先の配列。終端文字は付加されません。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  pDst の末尾に終端文字が付加されない点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf8ToUtf32(
        char32_t* pDst, int dstLength, const char* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-8 から UTF-32 に変換します。終端文字まで変換されます。
    @param[out]   pDst       変換先の配列。終端文字が付加されます。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまで変換を行います。@n
                  pDst の末尾に終端文字が付加される点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf8ToUtf32(
        uint32_t* pDst, int dstLength, const char* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-8 から UTF-32 に変換します。終端文字まで変換されます。
    @param[out]   pDst       変換先の配列。終端文字が付加されます。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまで変換を行います。@n
                  pDst の末尾に終端文字が付加される点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf8ToUtf32(
        char32_t* pDst, int dstLength, const char* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-32 から UTF-8 に変換します。
    @param[out]   pDst       変換先の配列。終端文字は付加されません。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  pDst の末尾に終端文字が付加されない点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf32ToUtf8(
        char* pDst, int dstLength, const uint32_t* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-32 から UTF-8 に変換します。
    @param[out]   pDst       変換先の配列。終端文字は付加されません。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  pDst の末尾に終端文字が付加されない点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf32ToUtf8(
        char* pDst, int dstLength, const char32_t* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-32 から UTF-8 に変換します。終端文字まで変換されます。
    @param[out]   pDst       変換先の配列。終端文字が付加されます。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまで変換を行います。@n
                  pDst の末尾に終端文字が付加される点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf32ToUtf8(
        char* pDst, int dstLength, const uint32_t* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief        文字コードを UTF-32 から UTF-8 に変換します。終端文字まで変換されます。
    @param[out]   pDst       変換先の配列。終端文字が付加されます。
    @param[in]    dstLength  変換先の配列の長さ。*pDst の型換算の長さ(バッファサイズ / 型サイズ)です。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success               - 正常に変換されました。
    @retval       CharacterEncodingResult_InsufficientLength    - 変換先の配列の長さが不足しました。
    @retval       CharacterEncodingResult_InvalidFormat         - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまで変換を行います。@n
                  pDst の末尾に終端文字が付加される点に注意してください。
    */
    CharacterEncodingResult ConvertStringUtf32ToUtf8(
        char* pDst, int dstLength, const char32_t* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief        UTF-8 から UTF-32 に変換するために必要な変換先の配列の長さを取得します。
    @param[out]   pOutLength 変換先の配列に文字列を書き込むのに必要な配列の長さ。終端文字の分は含まれません。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success          - 正常に変換されました。
    @retval       CharacterEncodingResult_InvalidFormat    - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  変換元の配列が配列末尾の終端文字を含まない場合、pOutLength は終端文字を含まない配列の長さであることに注意してください。
    */
    CharacterEncodingResult GetLengthOfConvertedStringUtf8ToUtf32(
        int* pOutLength, const char* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        UTF-8 から UTF-32 に変換するために必要な変換先の配列の長さを取得します。終端文字まで読み込まれます。
    @param[out]   pOutLength 変換先の配列に文字列を書き込むのに必要な配列の長さ。終端文字の分は含まれません。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success          - 正常に変換されました。
    @retval       CharacterEncodingResult_InvalidFormat    - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまでの長さを取得します。@n
                  pOutLength は終端文字を含まない配列の長さであることに注意してください。
    */
    CharacterEncodingResult GetLengthOfConvertedStringUtf8ToUtf32(
        int* pOutLength, const char* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief        UTF-32 から UTF-8 に変換するために必要な変換先の配列の長さを取得します。
    @param[out]   pOutLength 変換先の配列に文字列を書き込むのに必要な配列の長さ。終端文字の分は含まれません。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success          - 正常に変換されました。
    @retval       CharacterEncodingResult_InvalidFormat    - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  変換元の配列が配列末尾の終端文字を含まない場合、pOutLength は終端文字を含まない配列の長さであることに注意してください。
    */
    CharacterEncodingResult GetLengthOfConvertedStringUtf32ToUtf8(
        int* pOutLength, const uint32_t* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        UTF-32 から UTF-8 に変換するために必要な変換先の配列の長さを取得します。
    @param[out]   pOutLength 変換先の配列に文字列を書き込むのに必要な配列の長さ。終端文字の分は含まれません。
    @param[in]    pSrc       変換元の配列。
    @param[in]    srcLength  変換元の配列の長さ。*pSrc の型換算の長さ(バッファサイズ / 型サイズ)です。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success          - 正常に変換されました。
    @retval       CharacterEncodingResult_InvalidFormat    - 変換できない文字が検出されました。
    @details      変換元の配列が終端文字を含むかどうかに関わらず、srcLength で指定した長さを処理します。@n
                  変換元の配列が配列末尾の終端文字を含まない場合、pOutLength は終端文字を含まない配列の長さであることに注意してください。
    */
    CharacterEncodingResult GetLengthOfConvertedStringUtf32ToUtf8(
        int* pOutLength, const char32_t* pSrc, int srcLength
    ) NN_NOEXCEPT;

    /*!
    @brief        UTF-32 から UTF-8 に変換するために必要な変換先の配列の長さを取得します。終端文字まで読み込まれます。
    @param[out]   pOutLength 変換先の配列に文字列を書き込むのに必要な配列の長さ。終端文字の分は含まれません。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success          - 正常に変換されました。
    @retval       CharacterEncodingResult_InvalidFormat    - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまでの長さを取得します。@n
                  pOutLength は終端文字を含まない配列の長さであることに注意してください。
    */
    CharacterEncodingResult GetLengthOfConvertedStringUtf32ToUtf8(
        int* pOutLength, const uint32_t* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief        UTF-32 から UTF-8 に変換するために必要な変換先の配列の長さを取得します。終端文字まで読み込まれます。
    @param[out]   pOutLength 変換先の配列に文字列を書き込むのに必要な配列の長さ。終端文字の分は含まれません。
    @param[in]    pSrc       変換元の配列。

    @return       結果を返します。
    @retval       CharacterEncodingResult_Success          - 正常に変換されました。
    @retval       CharacterEncodingResult_InvalidFormat    - 変換できない文字が検出されました。
    @details      pSrc が正常に変換可能な文字列である場合、終端文字が現れるまでの長さを取得します。@n
                  pOutLength は終端文字を含まない配列の長さであることに注意してください。
    */
    CharacterEncodingResult GetLengthOfConvertedStringUtf32ToUtf8(
        int* pOutLength, const char32_t* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief          文字コードを UTF-8 から UTF-16 に 1 文字だけ変換します。
    @param[out]     pDst        変換先の文字。配列 2 つ分のバッファを渡してください。
    @param[in]      pSrc        変換元の文字。配列 4 つ分のバッファを渡してください。

    @return         結果を返します。
    */
    CharacterEncodingResult ConvertCharacterUtf8ToUtf16Native(
        uint16_t* pDst, const char* pSrc
    ) NN_NOEXCEPT;
    /*!
    @brief          文字コードを UTF-8 から UTF-16 に 1 文字だけ変換します。
    @param[out]     pDst        変換先の文字。配列 2 つ分のバッファを渡してください。
    @param[in]      pSrc        変換元の文字。配列 4 つ分のバッファを渡してください。

    @return         結果を返します。
    */
    CharacterEncodingResult ConvertCharacterUtf8ToUtf16Native(
        char16_t* pDst, const char* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief          文字コードを UTF-16 から UTF-8 に 1 文字だけ変換します。
    @param[out]     pDst        変換先の文字。配列 4 つ分のバッファを渡してください。
    @param[in]      pSrc        変換元の文字。配列 2 つ分のバッファを渡してください。

    @return         結果を返します。
    */
    CharacterEncodingResult ConvertCharacterUtf16NativeToUtf8(
        char* pDst, const uint16_t* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief          文字コードを UTF-16 から UTF-8 に 1 文字だけ変換します。
    @param[out]     pDst        変換先の文字。配列 4 つ分のバッファを渡してください。
    @param[in]      pSrc        変換元の文字。配列 2 つ分のバッファを渡してください。

    @return         結果を返します。
    */
    CharacterEncodingResult ConvertCharacterUtf16NativeToUtf8(
        char* pDst, const char16_t* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief          文字コードを UTF-8 から UTF-32 に 1 文字だけ変換します。
    @param[out]     pDst        変換先の文字。配列 1 つ分のバッファを渡してください。
    @param[in]      pSrc        変換元の文字。配列 4 つ分のバッファを渡してください。

    @return         結果を返します。
    */
    CharacterEncodingResult ConvertCharacterUtf8ToUtf32(
        uint32_t* pDst, const char* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief          文字コードを UTF-8 から UTF-32 に 1 文字だけ変換します。
    @param[out]     pDst        変換先の文字。配列 1 つ分のバッファを渡してください。
    @param[in]      pSrc        変換元の文字。配列 4 つ分のバッファを渡してください。

    @return         結果を返します。
    */
    CharacterEncodingResult ConvertCharacterUtf8ToUtf32(
        char32_t* pDst, const char* pSrc
    ) NN_NOEXCEPT;

    /*!
    @brief          文字コードを UTF-32 から UTF-8 に 1 文字だけ変換します。
    @param[out]     pDst        変換先の文字。配列 4 つ分のバッファを渡してください。
    @param[in]      src         変換元の文字。

    @return         結果を返します。
    */
    CharacterEncodingResult ConvertCharacterUtf32ToUtf8(
        char* pDst, uint32_t src
    ) NN_NOEXCEPT;

    /*!
    @brief          文字コードを UTF-32 から UTF-8 に 1 文字だけ変換します。
    @param[out]     pDst        変換先の文字。配列 4 つ分のバッファを渡してください。
    @param[in]      src         変換元の文字。

    @return         結果を返します。
    */
    CharacterEncodingResult ConvertCharacterUtf32ToUtf8(
        char* pDst, char32_t src
    ) NN_NOEXCEPT;

    /*!
    @brief          UTF-8 の文字列から 1 文字を切り出して、ポインタを進めます。
    @param[out]     pChar       切り出した 1 文字を格納する 4byte 分のバッファ。
    @param[in,out]  pStr        切り出す文字列。切り出した後に 1 文字分ポインタを進めます。

    @return         結果を返します。
    */
    CharacterEncodingResult PickOutCharacterFromUtf8String(
        char* pChar, const char** pStr
    ) NN_NOEXCEPT;

    //! @}
}}
