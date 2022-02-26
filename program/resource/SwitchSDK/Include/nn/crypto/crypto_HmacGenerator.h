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
    @brief HMAC を計算するための API の宣言
 */

#include <nn/nn_Common.h>
#include <nn/crypto/detail/crypto_HmacImpl.h>

namespace nn { namespace crypto {

/**
    @brief      HMAC 計算を行うクラスです。

    @tparam     HashFunction 対象とするハッシュ関数の実装クラス

    @details
    HMAC はハッシュ関数を利用して MAC
    (Message Authentication Code) を生成します。

    このクラスはハッシュ関数の実装クラスをテンプレート引数とするテンプレートクラスです。
    実際の使用には @ref HmacSha1Generator, @ref HmacSha256Generator などを利用してください。

    テンプレートとして指定するハッシュ関数クラスは以下の条件を満たす実装になっている必要があります。
    各関数の引数など詳細な説明については Sha256Generator クラスなどを参照してください。

    -# BlockSize という処理するブロックサイズが定義された定数を持つ
    -# HashSize というハッシュ値の出力サイズが定義された定数を持つ
    -# Initialize という処理を開始する関数を持つ
    -# Update という入力でハッシュ値を更新する関数を持つ
    -# GetHash という最終的なハッシュ値を取得する関数を持つ

    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 処理可能状態
    - 完了状態

 */
template <typename HashFunction>
class HmacGenerator
{
    NN_DISALLOW_COPY(HmacGenerator<HashFunction>);

public:
    static const size_t MacSize   = HashFunction::HashSize;  //!< MAC のサイズを表す定数です。
    static const size_t BlockSize = HashFunction::BlockSize; //!< MAC の計算処理単位のサイズを表す定数です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    HmacGenerator() NN_NOEXCEPT {}

    /**
        @brief      MAC 計算を初期化します。

        @param[in]  pKey     鍵データへのポインタ。
        @param[in]  keySize  鍵データのバイトサイズ。

        @post
        - インスタンスは処理可能状態になる。

        @details
        鍵データを入力し、MAC 計算を初期化します。
        鍵データは内部にコピーされます。

        HMAC では任意のサイズの鍵を使用することができますが、暗号強度の観点から HashSize 以上のサイズの鍵を使用することを推奨します。
        BlockSize 以上のサイズの鍵は指定されたハッシュ関数によってハッシュ値が計算され、HashSize の鍵として使用されます。
     */
    void Initialize(const void* pKey, size_t keySize) NN_NOEXCEPT
    {
        m_Impl.Initialize(pKey, keySize);
    }

    /**
        @brief    MAC を与えられたデータで更新します。

        @param[in]  pData    入力データへのポインタ。
        @param[in]  dataSize 入力データのバイトサイズ。

        @pre
        - インスタンスは処理可能状態である。

        @details
        この関数は複数回に分けて呼び出すことができます。
        ただし、入力されたデータはすべてが連結されたひとまとまりのデータとして扱われるため、入力する順序が意味を持つことに注意してください。
     */
    void Update(const void* pData, size_t dataSize) NN_NOEXCEPT
    {
        m_Impl.Update(pData, dataSize);
    }

    /**
        @brief    MAC 計算を完了し、最終的な MAC を取得します。

        @param[out] pMac    MAC を格納するバッファへのポインタ。
        @param[in]  macSize pMac が指すバッファのバイトサイズ。

        @pre
        - macSize >= MacSize
        - インスタンスは処理可能状態または完了状態である。

        @post
        - pMac に MAC が書き込まれる。
        - インスタンスは完了状態になる。

        @details
        MAC 計算が完了されます。
        別の MAC 計算を行う場合は Initialize() を呼んで新たに計算を開始してください。
     */
    void GetMac(void* pMac, size_t macSize) NN_NOEXCEPT
    {
        m_Impl.GetMac(pMac, macSize);
    }

private:
    detail::HmacImpl<HashFunction> m_Impl;
};

NN_DEFINE_STATIC_CONSTANT(template<typename T> const size_t HmacGenerator<T>::MacSize);
NN_DEFINE_STATIC_CONSTANT(template<typename T> const size_t HmacGenerator<T>::BlockSize);

}} // namespace nn::crypto
