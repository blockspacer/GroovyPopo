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
    @brief CBC モードによる暗号化を利用するための API の宣言
 */

#include <nn/nn_Common.h>
#include <nn/crypto/detail/crypto_CbcModeImpl.h>

namespace nn { namespace crypto {

/**
    @brief      CBC 暗号利用モードによる暗号化を行うクラスです。

    @tparam     BlockCipher 対象とするブロック暗号の実装クラス

    @details
    CBC は NIST SP 800-38A で規定される暗号利用モードです。

    このクラスはブロック暗号の実装クラスをテンプレート引数とするテンプレートクラスです。
    実際の使用には Aes128CbcEncryptor などを利用してください。

    テンプレートとして指定するブロック暗号クラスは以下の条件を満たす実装になっている必要があります。
    各関数の引数など詳細な説明については AesEncryptor クラスを参照してください。

    -# BlockSize という処理するブロックサイズが定義された定数を持つ。
    -# EncryptBlock という単一ブロックを暗号化する関数を持つ。

    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 処理可能状態

    本クラスではパディングはサポートされません。
    データフォーマットとして特定のパディングが必要な場合は、データ側にパディングを付与して使用してください。
 */
template <typename BlockCipher>
class CbcEncryptor
{
    NN_DISALLOW_COPY(CbcEncryptor<BlockCipher>);

public:
    static const size_t BlockSize = BlockCipher::BlockSize; //!< ブロックサイズを表す定数です。
    static const size_t IvSize    = BlockCipher::BlockSize; //!< IV のサイズを表す定数です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    CbcEncryptor() NN_NOEXCEPT {}

    /**
        @brief      CBC モードで暗号化の処理を初期化します。

        @param[in]  pBlockCipher ブロック暗号の実装クラスのインスタンス。
        @param[in]  pIv          IV データへのポインタ。
        @param[in]  ivSize       IV データのバイトサイズ。

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

        IV データは内部にコピーされます。
     */
    void Initialize(const BlockCipher* pBlockCipher, const void* pIv, size_t ivSize) NN_NOEXCEPT
    {
        m_Impl.Initialize(pBlockCipher, pIv, ivSize);
    }

    /**
        @brief      CBC モードで暗号化を行います。

        @param[out] pDst    暗号化されたデータを受け取るバッファへのポインタ。
        @param[in]  dstSize pDst が指すバッファのバイトサイズ。
        @param[in]  pSrc    暗号化するデータが格納されているバッファへのポインタ。
        @param[in]  srcSize 暗号化するデータのバイトサイズ。

        @return     実際に暗号化されたバイト数。

        @pre
        - インスタンスは処理可能状態である。
        - dstSize >= ((srcSize + GetBufferedDataSize()) / BlockSize) * BlockSize
        - pDst, dstSize で指定する領域と pSrc, srcSize で指定する領域に重なりがない。

        @post
        - pDst に暗号化された結果が書き込まれる。

        @details
        CBC モードでは暗号化はブロック単位で処理されるため、1 回の呼び出しで dstSize として必要になるサイズは最大で (srcSize + ブロックサイズ - 1) になります。
        例えば 1 バイトずつ Update を呼び出したとすると積算サイズがブロックサイズになるまでは処理は行われず、その間は返り値として 0 が返されます。
        積算サイズがブロックサイズに到達した呼び出しで 1 ブロック分が処理され、ブロックサイズ分が返り値として返されることになります。

        CBC モードでは srcSize の合計がブロックサイズの倍数である必要があります。

        dstSize の合計として必要になるバッファサイズは srcSize の合計と同じだけ必要になります。
     */
    size_t Update(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT
    {
        return m_Impl.UpdateEncryption(pDst, dstSize, pSrc, srcSize);
    }

    /**
        @brief      入力済みの未処理データのサイズを取得します。

        @return     入力済みの未処理データのバイト数。

        @details
        Update で入力されたデータのうちブロックサイズに満たなかったデータはブロックサイズに達するまで未処理の状態でバッファされます。

        この関数ではそのデータのサイズを取得できます。
     */
    size_t GetBufferedDataSize() const NN_NOEXCEPT
    {
        return m_Impl.GetBufferedDataSize();
    }

private:
    detail::CbcModeImpl<BlockCipher> m_Impl;
};

NN_DEFINE_STATIC_CONSTANT(template<typename T> const size_t CbcEncryptor<T>::BlockSize);
NN_DEFINE_STATIC_CONSTANT(template<typename T> const size_t CbcEncryptor<T>::IvSize);

}} // namespace nn::crypto

