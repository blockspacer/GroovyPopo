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
* @brief    文字列操作を行う API の宣言です。
*/

#include <nn/nn_Common.h>
#include <nn/nn_SdkAssert.h>
#include <nn/nn_StaticAssert.h>
#include <nn/nn_Abort.h>

namespace nn { namespace util {

NN_STATIC_ASSERT('Z' - 'A' == 26 - 1);
NN_STATIC_ASSERT('z' - 'a' == 26 - 1);

//! @name 文字列操作関数
//! @{

/*!
    @brief      大文字を小文字に変換します。

    @tparam     T   文字の型です。

    @param[in]  c   変換する文字を指定します。

    @return     変換後の文字を返します。

    @details
                本関数は C ロケールのシステムであることを前提として、アルファベットのみを変換します。@n
                大文字以外を指定した場合、入力した文字をそのまま返します。
*/
template<typename T>
T ToLower(T c) NN_NOEXCEPT
{
    return (c >= 'A' && c <= 'Z') ? (c - 'A' + 'a') : c;
}

/*!
    @brief      小文字を大文字に変換します。

    @tparam     T   文字の型です。

    @param[in]  c   変換する文字を指定します。

    @return     変換後の文字を返します。

    @details
                本関数は C ロケールのシステムであることを前提として、アルファベットのみを変換します。@n
                小文字以外を指定した場合、入力した文字をそのまま返します。
*/
template<typename T>
T ToUpper(T c) NN_NOEXCEPT
{
    return (c >= 'a' && c <= 'z') ? (c - 'a' + 'A') : c;
}

/*!
    @brief      コピー先のバッファの要素数や終端に配慮した文字列のコピーをおこないます。

    @tparam     T       文字の型です。

    @param[out] pOutDst コピー先のバッファの先頭へのポインタを指定します。
    @param[in]  pSrc    コピー元の文字列の先頭へのポインタを指定します。
    @param[in]  count   コピー先のバッファの要素数を指定します。

    @return     pSrc に与えられた文字列の長さ（終端のヌル文字を含まない要素数）を返します。

    @pre
                - pOutDst != nullptr
                - pSrc != nullptr

    @details
                count の値が文字列 pSrc の長さより大きい場合、文字列 pSrc を、終端のヌル文字も含めて pOutDst へコピーします。

                count の値が 1 以上かつ文字列 pSrc の長さが count 以上の場合、 count - 1 文字分のコピーをおこない、ヌル文字で終端します。@n
                count の値が 0 以下の場合、ヌル文字での終端を含め、コピー先のバッファへは一切の書き込みをおこないません。

                関数の返り値が count 未満であれば、 pSrc の文字列は切り詰めなく pOutDst にコピーされています。
*/
template<typename T>
int Strlcpy(T* pOutDst, const T* pSrc, int count) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL(pOutDst);
    NN_SDK_REQUIRES_NOT_NULL(pSrc);

    int length = 0;

    if (count > 0)
    {
        while (--count && *pSrc)
        {
            *pOutDst++ = *pSrc++;
            ++length;
        }
        *pOutDst = 0;
    }

    while (*pSrc++)
    {
        ++length;
    }

    return length;
}

/*!
    @brief      バッファの要素数や終端に配慮した文字数のカウントをおこないます。

    @tparam     T       文字の型です。
    @param[in]  pStr    文字列の先頭へのポインタを指定します。
    @param[in]  count   文字列が格納されたバッファの要素数を指定します。

    @return     pStr に与えられた文字列の長さ（終端のヌル文字を含まない要素数）を返します。

    @pre
                - pStr != nullptr
                - count >= 0

    @details
                count の値が文字列 pStr の長さより大きい場合、文字列 pStr の、終端のヌル文字を含まない要素数を返します。

                count の値が文字列 pStr の長さ以下の場合は count の値を返し、 count 番目以降の要素にはアクセスしません。

                関数の返り値が count と等しい場合、 pStr がヌル終端されていることは保証されません。@n
                複数要素で成り立つ文字があった場合もそれを 1 文字と数えることはなく、要素数をもって文字数とします。
*/
template<typename T>
int Strnlen(const T* pStr, int count) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL(pStr);
    NN_SDK_REQUIRES(count >= 0);

    int length = 0;

    while (count-- && *pStr++)
    {
        ++length;
    }

    return length;
}

/*!
    @brief      バッファの要素数や終端に配慮して文字列を比較します。

    @tparam     T       文字の型です。
    @param[in]  pStr1   比較する文字列の一方の先頭へのポインタを指定します。
    @param[in]  pStr2   比較する文字列の他方の先頭へのポインタを指定します。
    @param[in]  count   文字列の比較をおこなう最大の要素数を指定します。

    @return     文字列が一致すれば 0 、一致しなければ非 0 の値を返します。

    @pre
                - pStr1 != nullptr
                - pStr2 != nullptr
                - count >= 0

    @details
                与えられた二つの文字列に対し、最大で count - 1 番目までの文字を比較します。

                最初に発見した異なる文字の値が、 pStr1 の指す文字列のほうが小さかった場合は負の値が、大きかった場合は正の値が返ります。

                非 0 の値が返った場合に双方の文字列が異なることは保証されますが、 0 が返った場合に双方の文字列が一致することは保証されません。@n
                双方の文字列が一致していることを保証するには、返り値が 0 であることに加え、いずれかの文字列の長さが count 未満であることを確認する必要があります。
*/
template<typename T>
int Strncmp(const T* pStr1, const T* pStr2, int count) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL(pStr1);
    NN_SDK_REQUIRES_NOT_NULL(pStr2);
    NN_ABORT_UNLESS(count >= 0);

    if (count == 0)
    {
        return 0;
    }

    T c1, c2;

    do
    {
        c1 = *pStr1++;
        c2 = *pStr2++;
    }
    while (c1 && c1 == c2 && --count);

    return c1 - c2;
}

/*!
    @brief      バッファの要素数や終端に配慮して文字列を英字の大小を区別せずに比較します。

    @tparam     T       文字の型です。

    @param[in]  pStr1   比較する文字列の一方の先頭へのポインタを指定します。
    @param[in]  pStr2   比較する文字列の他方の先頭へのポインタを指定します。
    @param[in]  count   文字列の比較をおこなう最大の要素数を指定します。

    @return     文字列が一致すれば 0 、一致しなければ非 0 の値を返します。

    @pre
                - pStr1 != nullptr
                - pStr2 != nullptr
                - count >= 0

    @details
                与えられた二つの文字列に対し、最大で count - 1 番目までの文字を英字の大小を区別せずに比較します。

                最初に発見した異なる文字の値が、 pStr1 の指す文字列のほうが小さかった場合は負の値が、大きかった場合は正の値が返ります。@n
                本関数は各文字を小文字に変換してから比較するため、ASCII コードの 'Z' と 'a' の間の記号文字と大文字の比較が行われる文字列を指定した場合、 Strncmp() と返り値の大小が逆転します。

                非 0 の値が返った場合に双方の文字列が異なることは保証されますが、 0 が返った場合に双方の文字列が一致することは保証されません。@n
                双方の文字列が一致していることを保証するには、返り値が 0 であることに加え、いずれかの文字列の長さが count 未満であることを確認する必要があります。
*/
template<typename T>
int Strnicmp(const T* pStr1, const T* pStr2, int count) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL(pStr1);
    NN_SDK_REQUIRES_NOT_NULL(pStr2);
    NN_ABORT_UNLESS(count >= 0);

    if (count == 0)
    {
        return 0;
    }

    T c1, c2;

    do
    {
        c1 = ToLower(*pStr1++);
        c2 = ToLower(*pStr2++);
    }
    while (c1 && c1 == c2 && --count);

    return c1 - c2;
}

/*!
    @brief      部分文字列の位置を検索します。

    @tparam     T       文字の型です。

    @param[in]  pStr1   検索対象文字列を指定します。
    @param[in]  pStr2   部分文字列を指定します。
    @param[in]  count   文字列の比較をおこなう最大の要素数を指定します。

    @return     部分文字列が見つかった場合は最初の出現位置、見つからなかった場合は nullptr を返します。

    @pre
                - pStr1 != nullptr
                - pStr2 != nullptr
                - count >= 0

    @details
                pStr1 の先頭から最大 count - 1 番目までの文字列内から pStr2 が最初に現れる位置を見つけます。@n
                終端のヌル文字は比較されません。

                pStr2 が空文字列（"\0"）の場合、常に pStr1 の先頭を返します。
*/
template<typename T>
const T* Strnstr(const T* pStr1, const T* pStr2, int count) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL(pStr1);
    NN_SDK_REQUIRES_NOT_NULL(pStr2);
    NN_ABORT_UNLESS(count >= 0);

    if (*pStr2 == '\0')
    {
        return pStr1;
    }
    if (count == 0)
    {
        return nullptr;
    }

    const T* pStr1End = pStr1 + count;

    while (*pStr1 != '\0' && pStr1 < pStr1End)
    {
        const T* p1 = pStr1;
        const T* p2 = pStr2;

        while (p1 < pStr1End && *p1 == *p2)
        {
            if (*++p2 == '\0')
            {
                return pStr1;
            }

            ++p1;
        }

        ++pStr1;
    }

    return nullptr;
}

/*!
    @brief      部分文字列の位置を検索します。

    @param[in]  pHaystack       検索対象データを指定します。
    @param[in]  haystackLength  pHaystack のデータ長を指定します。
    @param[in]  pNeedle         部分文字列データを指定します。
    @param[in]  needleLength    pNeedle のデータ長を指定します。

    @return     部分文字列が見つかった場合は最初の出現位置、見つからなかった場合は nullptr を返します。

    @pre
                - pHaystack != nullptr
                - pNeedle != nullptr

    @details
                pHaystack から pNeedle が最初に現れる位置を見つけます。@n
                Strnstr() とは違い、ヌル文字の確認は行われません。

                needleLength が 0 の場合、常に pHaystack の先頭を返します。
*/
const void* Memmem(const void* pHaystack, size_t haystackLength, const void* pNeedle, size_t needleLength) NN_NOEXCEPT;

//! @}


}} // namespace nn::util
