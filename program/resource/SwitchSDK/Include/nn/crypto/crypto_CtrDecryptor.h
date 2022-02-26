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
    @brief CTR モードによる復号化を利用するための API の宣言
 */

#include <nn/nn_Common.h>
#include <nn/crypto/detail/crypto_CtrModeImpl.h>

namespace nn { namespace crypto {

/**
    @brief      CTR 暗号利用モードによる復号化を行うクラスです。

    @tparam     BlockCipher 対象とするブロック暗号の実装クラス

    @details
    CTR は NIST SP 800-38A で規定される暗号利用モードです。
    このクラスによる実装ではカウンタ値のインクリメント関数として単純なインクリメントを採用しています。

    このクラスはブロック暗号の実装クラスをテンプレート引数とするテンプレートクラスです。
    実際の使用には Aes128CtrDecryptor などを利用してください。

    テンプレートとして指定するブロック暗号クラスは以下の条件を満たす実装になっている必要があります。
    各関数の引数など詳細な説明については AesEncryptor クラスを参照してください。

    -# BlockSize という処理するブロックサイズが定義された定数を持つ。
    -# EncryptBlock という単一ブロックを暗号化する関数を持つ。

    CTR モードの復号化ではブロック暗号の復号化処理ではなく、暗号化処理が用いられることに注意してください。

    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 処理可能状態
 */
template <typename BlockCipher>
class CtrDecryptor
{
    NN_DISALLOW_COPY(CtrDecryptor<BlockCipher>);

public:
    static const size_t BlockSize = BlockCipher::BlockSize; //!< ブロックサイズを表す定数です。
    static const size_t IvSize    = BlockCipher::BlockSize; //!< IV のサイズを表す定数です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    CtrDecryptor() NN_NOEXCEPT {}

    /**
        @brief      CTR モードで復号化の処理を初期化します。

        @param[in]  pBlockCipher ブロック暗号の実装クラスのインスタンス。
        @param[in]  pIv          カウンタの初期値データへのポインタ。
        @param[in]  ivSize       カウンタの初期値データのバイトサイズ。

        @pre
        - pBlockCipher で指定するインスタンスは EncryptBlock を実行できる。
        - ivSize == IvSize

        @post
        - インスタンスは処理可能状態になる。

        @details
        pBlockCipher には暗号化に使用するブロック暗号を実装するインスタンスを指定します。
        あらかじめ鍵の設定など必要な初期化を行い、EncryptBlock が実行できる状態になっている必要があります。

        pBlockCipher で指定したインスタンスはコピーされません。
        一連の処理が完了するまで対象 pBlockCipher に対する処理は行わないでください。

        CTR モードでは IV はカウンタの初期値として扱われます。
        カウンタの初期値は内部にコピーされます。
     */
    void Initialize(const BlockCipher* pBlockCipher, const void* pIv, size_t ivSize) NN_NOEXCEPT
    {
        m_Impl.Initialize(pBlockCipher, pIv, ivSize);
    }

    /**
        @brief      CTR モードで復号化を行います。

        @param[out] pDst    復号化されたデータを受け取るバッファへのポインタ。
        @param[in]  dstSize pDst が指すバッファのバイトサイズ。
        @param[in]  pSrc    復号化するデータが格納されているバッファへのポインタ。
        @param[in]  srcSize 復号化するデータのバイトサイズ。

        @return     実際に復号化されたバイト数

        @pre
        - インスタンスは処理可能状態である。
        - dstSize >= srcSize
        - pDst, dstSize で指定する領域と pSrc, srcSize で指定する領域に重なりがない。

        @post
        - pDst に復号化された結果が書き込まれる。

        @details
        CTR モードではバイト単位で復号化処理が行われるため 1 回の呼び出しで必要になる dstSize は srcSize と同じだけ必要です。

        dstSize の合計として必要になるバッファサイズも srcSize の合計と同じだけ必要になります。
     */
    size_t Update(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT
    {
        return m_Impl.Update(pDst, dstSize, pSrc, srcSize);
    }

private:
    detail::CtrModeImpl<BlockCipher> m_Impl;
};

NN_DEFINE_STATIC_CONSTANT(template<typename T> const size_t CtrDecryptor<T>::BlockSize);
NN_DEFINE_STATIC_CONSTANT(template<typename T> const size_t CtrDecryptor<T>::IvSize);

}} // namespace nn::crypto

