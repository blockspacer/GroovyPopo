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
    @brief CMAC を計算するための API の宣言
 */

#include <nn/nn_Common.h>
#include <nn/crypto/detail/crypto_CmacImpl.h>

namespace nn { namespace crypto {

/**
    @brief      CMAC 計算を行うクラスです。

    @tparam     BlockCipher 対象とするブロック暗号の実装クラス

    @details
    CMAC は NIST SP 800-38B で規定されるブロック暗号を利用して MAC
    (Message Authentication Code) を生成する暗号利用モードです。

    このクラスはブロック暗号の実装クラスをテンプレート引数とするテンプレートクラスです。
    現在は AES などブロックサイズが 128bit のブロック暗号のみがサポートされています。
    実際の使用には Aes128CmacGenerator などを利用してください。

    テンプレートとして指定するブロック暗号クラスは以下の条件を満たす実装になっている必要があります。
    各関数の引数など詳細な説明については AesEncryptor クラスを参照してください。

    -# BlockSize という処理するブロックサイズが定義された定数を持つ。
    -# EncryptBlock という単一ブロックを暗号化する関数を持つ。

    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 処理可能状態
    - 完了状態

 */
template <typename BlockCipher>
class CmacGenerator
{
    NN_DISALLOW_COPY(CmacGenerator<BlockCipher>);

public:
    static const size_t MacSize   = BlockCipher::BlockSize;  //!< MAC のサイズを表す定数です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    CmacGenerator() NN_NOEXCEPT {}

    /**
        @brief      MAC 計算を初期化します。

        @param[in]  pBlockCipher ブロック暗号の実装クラスのインスタンス。

        @details
        pBlockCipher には暗号化に使用するブロック暗号を実装するインスタンスを指定します。
        あらかじめ鍵の設定など必要な初期化を行い、EncryptBlock が実行できる状態になっている必要があります。

        pBlockCipher で指定したインスタンスはコピーされません。
        一連の処理が完了するまで対象 pBlockCipher に対する処理は行わないでください。
     */
    void Initialize(const BlockCipher* pBlockCipher) NN_NOEXCEPT
    {
        m_Impl.Initialize(pBlockCipher);
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
    detail::CmacImpl<BlockCipher> m_Impl;
};

NN_DEFINE_STATIC_CONSTANT(template<typename T> const size_t CmacGenerator<T>::MacSize);

}} // namespace nn::crypto
