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
    @file
    @brief AES によるブロック復号化を行うための API の宣言
 */

#include <nn/nn_Common.h>
#include <nn/crypto/detail/crypto_AesImpl.h>

namespace nn { namespace crypto {

/**
    @brief      AES 暗号による復号化を実装するクラスです。

    @tparam     KeySize     バイト単位の鍵サイズ

    @details
    AES (Rijndael) 暗号による復号化を実装するクラスです。

    このクラスは鍵サイズをテンプレート引数とするテンプレートクラスです。
    実際の使用には @ref AesDecryptor128, @ref AesDecryptor192, @ref AesDecryptor256 型を使用してください。

    暗号に対する十分な知識があり、何をしているかを十分に理解しているのでない限り、このクラスの DecryptBlock(), GetRoundKey() を直接使用すべきではありません。

    このクラスは以下の状態を持ちます。
    インスタンスの生成時は未初期化状態です。
    - 未初期化状態
    - 処理可能状態

 */
template <size_t KeySize>
class AesDecryptor
{
public:
    static const size_t BlockSize = 16; //!< ブロックサイズを表す定数です。
    static const size_t RoundKeySize = detail::AesImpl<KeySize>::RoundKeySize; //!< ラウンド鍵のサイズを表す定数です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    AesDecryptor() NN_NOEXCEPT {}

    /**
        @brief      復号化に使用する鍵を設定し、処理を初期化します。

        @param[in]  pKey    鍵データへのポインタ。
        @param[in]  keySize 鍵データのバイトサイズ。

        @pre
        - keySize == KeySize

        @post
        - インスタンスは処理可能状態になる。

        @details
        鍵データは内部にコピーされます。
     */
    void Initialize(const void* pKey, size_t keySize) NN_NOEXCEPT
    {
        m_Impl.Initialize(pKey, keySize, false);
    }

    /**
        @brief      1ブロックの復号化を行います。

        @param[out] pDst    復号化されたデータを受け取るバッファへのポインタ。
        @param[in]  dstSize pDst が指すバッファのバイトサイズ。
        @param[in]  pSrc    復号化するデータが格納されているバッファへのポインタ。
        @param[in]  srcSize 復号化するデータのバイトサイズ。

        @pre
        - dstSize == BlockSize && srcSize == BlockSize
        - インスタンスは処理可能状態である

        @post
        - pDst に BlockSize の復号化された結果が書き込まれる。

        @details
        復号化ではブロックサイズ分の暗号文を同サイズの平文に変換します。
        pSrc からブロックサイズ分のデータを読み取り、復号化して pDst にその結果を書き込みます。
     */
    void DecryptBlock(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) const NN_NOEXCEPT
    {
        m_Impl.DecryptBlock(pDst, dstSize, pSrc, srcSize);
    }

    /**
        @brief      ラウンド鍵を取得します。

        @return     ラウンド鍵のデータ列へのポインタ。

        @pre
        - インスタンスは処理可能状態である

        @details
        Initialize() 時に鍵拡張処理によってブロックサイズのラウンド鍵が「鍵のサイズごとの規定のラウンド数 + 1」個計算されます。
        この関数ではそのラウンド鍵のデータ列の先頭へのポインタを取得できます。
        ラウンド鍵の合計サイズは RoundKeySize で定義されています。
     */
    const Bit8* GetRoundKey() const NN_NOEXCEPT
    {
        return m_Impl.GetRoundKey();
    }

private:
    detail::AesImpl<KeySize> m_Impl;

};

NN_DEFINE_STATIC_CONSTANT(template<size_t T> const size_t AesDecryptor<T>::BlockSize);
NN_DEFINE_STATIC_CONSTANT(template<size_t T> const size_t AesDecryptor<T>::RoundKeySize);

/**
    @brief 鍵長 128 bit の AES 暗号処理クラスです。
 */
typedef AesDecryptor<16>    AesDecryptor128;

/**
    @brief 鍵長 192 bit の AES 暗号処理クラスです。
 */
typedef AesDecryptor<24>    AesDecryptor192;

/**
    @brief 鍵長 256 bit の AES 暗号処理クラスです。
 */
typedef AesDecryptor<32>    AesDecryptor256;

}} // namespace nn::crypto

