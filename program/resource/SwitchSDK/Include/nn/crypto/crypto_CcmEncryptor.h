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
    @brief CCM モードによる認証付き暗号化を利用するための API の宣言
 */

#include <nn/nn_Common.h>
#include <nn/nn_StaticAssert.h>
#include <nn/crypto/detail/crypto_CcmModeImpl.h>

namespace nn { namespace crypto {

/**
    @brief      CCM 暗号利用モードによる認証付き暗号化を行うクラスです。

    @tparam     BlockCipher 対象とするブロック暗号の実装クラス

    @details
    CCM は NIST SP 800-38C で規定される認証付きの暗号利用モードです。
    このクラスによる実装では入力値の整形およびカウンタ値の生成関数として上記ドキュメント Appendix A による方法を採用しています。

    CCM モードでは暗号化や復号化の処理を行うだけでなく、MAC (メッセージ認証コード)が同時に計算されます。
    暗号化と復号化で同じ MAC が出力されることでデータが改ざんされていないことを検出できます。

    このクラスはブロック暗号の実装クラスをテンプレート引数とするテンプレートクラスです。
    ただし CCM モードはブロック長が 128 bit のブロック暗号を必要とします。
    実際の使用には Aes128CcmEncryptor などを利用してください。

    テンプレートとして指定するブロック暗号クラスは以下の条件を満たす実装になっている必要があります。
    各関数の引数など詳細な説明については AesEncryptor クラスを参照してください。

    -# BlockSize という処理するブロックサイズが定義された定数を持つ。CCM モードではこの値が 16 であるブロック暗号のみが利用できます。
    -# EncryptBlock という単一ブロックを暗号化する関数を持つ。

    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 追加認証データ処理可能状態
    - 暗号化処理可能状態
    - データ入力完了状態
    - 完了状態

 */
template <typename BlockCipher>
class CcmEncryptor
{
    NN_DISALLOW_COPY(CcmEncryptor<BlockCipher>);
    NN_STATIC_ASSERT(BlockCipher::BlockSize == 16);

public:
    static const size_t BlockSize    = BlockCipher::BlockSize; //!< ブロックサイズを表す定数です。
    static const size_t MaxNonceSize = 13; //!< 最大の Nonce のサイズを表す定数です。
    static const size_t MaxMacSize   = 16; //!< 最大の MAC のサイズを表す定数です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    CcmEncryptor() NN_NOEXCEPT {}

    /**
        @brief      CCM モードによる認証付き暗号化の処理を初期化します

        @param[in]  pBlockCipher ブロック暗号の実装クラスのインスタンス。
        @param[in]  pNonce       Nonce データへのポインタ。
        @param[in]  nonceSize    Nonce データのバイトサイズ。
        @param[in]  aadSize      追加認証データのバイトサイズ。
        @param[in]  dataSize     暗号化の対象となるデータのバイトサイズ。
        @param[in]  macSize      メッセージ認証コードのバイトサイズ。

        @pre
        - pBlockCipher で指定するインスタンスは EncryptBlock を実行できる。
        - nonceSize >= 7 && nonceSize <= 13
        - macSize >= 4 && macSize <= 16 && macSize % 2 == 0
        - aadSize >= 0
        - dataSize >= 0
        - (nonceSize == 7) ? dataSize <= INT64_MAX : dataSize < (1LL << ((15 - nonceSize) * 8))

        @post
        - aadSize が 0 の場合、インスタンスは暗号化処理可能状態になる。
        - aadSize が 0 でない場合、インスタンスは追加認証データ処理可能状態になる。

        @details
        pBlockCipher には暗号化に使用するブロック暗号を実装するインスタンスを指定します。
        あらかじめ鍵の設定など必要な初期化を行い、EncryptBlock が実行できる状態になっている必要があります。

        CCM モードではあらかじめ暗号化の対象となるデータのサイズがわかっている必要があります。

        Nonce は暗号化ごとに異なるものを使用する必要があるため Nonce のサイズは大きい値を使用することが推奨されます。
        しかし、Nonce のサイズは暗号化の対象となるデータサイズに対して以下の関係式を満たす必要があり、Nonce のサイズによって暗号化の対象となるデータサイズが制限されます。

        「Nonce のサイズ」 + 「暗号化の対象となるデータサイズを表現するためのバイトサイズ」 <= 15

        例えば、Nonce のサイズが 13 のとき、暗号化の対象となるデータサイズを表現するためのバイトサイズは 2 バイトまでになり、暗号化の対象となるデータサイズは最大で 0xFFFF バイトまでに制限されます。
        それを超えるデータを扱う場合には Nonce のサイズを減らす必要があります。

        MAC のサイズは 4 以上 16 以下の偶数を指定することができます。
        大きい値を使用するほど暗号強度が高くなるため 16 を指定することを強く推奨します。

        pBlockCipher で指定したインスタンスはコピーされません。
        一連の処理が完了するまで対象 pBlockCipher に対する処理は行わないでください。

        Nonce データは内部にコピーされます。
     */
    void Initialize(const BlockCipher* pBlockCipher,
                    const void* pNonce, size_t nonceSize,
                    int64_t aadSize, int64_t dataSize, size_t macSize) NN_NOEXCEPT
    {
        m_Impl.Initialize(pBlockCipher, pNonce, nonceSize, aadSize, dataSize, macSize);
    }

    /**
        @brief       追加認証データ(Additional Authentication Data)を入力します。

        @param[in]  pAad    追加認証データが格納されているバッファへのポインタ。
        @param[in]  aadSize 入力する追加認証データのバイトサイズ。

        @pre
        - インスタンスは追加認証データ処理可能状態である。
        - UpdateAad によって入力されるデータの合計が Initialize() で指定された aadSize を超えない。

        @post
        - 追加認証データとして入力されたサイズが aadSize に達したとき、Initialize() で指定された dataSize が 0 でない場合、インスタンスは暗号化処理可能状態になる。
        - 追加認証データとして入力されたサイズが aadSize に達したとき、Initialize() で指定された dataSize が 0 の場合、インスタンスはデータ入力完了状態になる。

        @details
        追加認証データとは暗号化の対象とならず MAC の対象にのみなるデータです。
        pSrc から指定のサイズ分のデータを読み取り MAC を計算します。

        この関数は複数回に分けて呼び出すことができます。
        ただし、入力されたデータはすべてが連結されたひとまとまりのデータとして扱われるため、入力する順序が意味を持つことに注意してください。

        すべての追加認証データの入力は Update() の処理を開始する前に完了させる必要があります。
     */
    void UpdateAad(const void* pAad, size_t aadSize) NN_NOEXCEPT
    {
        m_Impl.UpdateAad(pAad, aadSize);
    }

    /**
        @brief      CCM モードによる認証付き暗号化を行います。

        @param[out] pDst    暗号化されたデータを受け取るバッファへのポインタ。
        @param[in]  dstSize pDst が指すバッファのバイトサイズ。
        @param[in]  pSrc    暗号化するデータが格納されているバッファへのポインタ。
        @param[in]  srcSize 暗号化するデータのバイトサイズ。

        @return     実際に暗号化されたバイト数

        @pre
        - インスタンスは暗号化処理可能状態である。
        - Update によって入力されるデータの合計が Initialize() で指定された dataSize を超えない。
        - dstSize >= srcSize
        - pDst, dstSize で指定する領域と pSrc, srcSize で指定する領域に重なりがない。

        @post
        - 暗号化データとして入力されたサイズの合計が Initialize() で指定した dataSize に達した場合、インスタンスはデータ入力完了状態になる。
        - pDst に暗号化された結果が書き込まれる。

        @details
        CCM モードではバイト単位で暗号化処理が行われるため 1 回の呼び出しで必要になる dstSize は srcSize と同じだけ必要です。

        dstSize の合計として必要になるバッファサイズも srcSize の合計と同じだけ必要になります。
     */
    size_t Update(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT
    {
        return m_Impl.UpdateEncryption(pDst, dstSize, pSrc, srcSize);
    }

    /**
        @brief      最終的な MAC を取得します。

        @param[out] pMac    MAC を格納するバッファへのポインタ。
        @param[in]  macSize pMac で指定されたバッファのバイトサイズ。

        @pre
        - インスタンスはデータ入力完了状態または完了状態である。
        - macSize が Initialize() で指定された macSize 以上である。

        @post
        - pMac に MAC が書き込まれる。
        - インスタンスは完了状態になる。

        @details
        これまでに入力された追加認証データと平文データから計算された MAC を取得します。

        暗号化では計算した MAC を暗号文、追加認証データとあわせて送信してください。
        復号化では計算した MAC と受信した MAC が一致することを確認して下さい。
        MAC が一致することで受信した暗号文、追加認証データに改ざんがないことを検出できます。
     */
    void   GetMac(void* pMac, size_t macSize) NN_NOEXCEPT
    {
        m_Impl.GetMac(pMac, macSize);
    }

private:
    detail::CcmModeImpl<BlockCipher> m_Impl;
};

NN_DEFINE_STATIC_CONSTANT(template<typename T> const size_t CcmEncryptor<T>::BlockSize);
NN_DEFINE_STATIC_CONSTANT(template<typename T> const size_t CcmEncryptor<T>::MaxNonceSize);
NN_DEFINE_STATIC_CONSTANT(template<typename T> const size_t CcmEncryptor<T>::MaxMacSize);

}} // namespace nn::crypto

