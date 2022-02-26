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
    @brief GCM モードによる認証付き暗号化を利用するための API の宣言
 */

#include <nn/nn_Common.h>
#include <nn/nn_StaticAssert.h>
#include <nn/crypto/detail/crypto_GcmModeImpl.h>

namespace nn { namespace crypto {

/**
    @brief      GCM 暗号利用モードによる認証付き暗号化を行うクラスです。

    @tparam     BlockCipher 対象とするブロック暗号の実装クラス

    @details
    GCM は NIST SP 800-38D で規定される認証付きの暗号利用モードです。

    GCM モードでは暗号化や復号化の処理を行うだけでなく、MAC (メッセージ認証コード)が同時に計算されます。
    暗号化と復号化で同じ MAC が出力されることで、データが改ざんされていないことを検出できます。

    このクラスはブロック暗号の実装クラスをテンプレート引数とするテンプレートクラスです。
    ただし GCM モードはブロック長が 128 bit のブロック暗号を必要とします。
    実際の使用には Aes128GcmEncryptor などを利用してください。

    テンプレートとして指定するブロック暗号クラスは以下の条件を満たす実装になっている必要があります。
    各関数の引数など詳細な説明については AesEncryptor クラスを参照してください。

    -# BlockSize という処理するブロックサイズが定義された定数を持つ。GCM モードではこの値が 16 であるブロック暗号のみが利用できます。
    -# EncryptBlock という単一ブロックを暗号化する関数を持つ。

    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 追加認証データ処理可能状態
    - 暗号化処理可能状態
    - 完了状態

 */
template <typename BlockCipher>
class GcmEncryptor
{
    NN_DISALLOW_COPY(GcmEncryptor<BlockCipher>);
    NN_STATIC_ASSERT(BlockCipher::BlockSize == 16);

public:
    static const size_t BlockSize = BlockCipher::BlockSize; //!< ブロックサイズを表す定数です。
    static const size_t MacSize = 16;                       //!< MAC のサイズを表す定数です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    GcmEncryptor() NN_NOEXCEPT {}

    /**
        @brief      GCM モードによる認証付き暗号化の処理を初期化します。

        @param[in]  pBlockCipher ブロック暗号の実装クラスのインスタンス。
        @param[in]  pIv          カウンタの初期値データへのポインタ。
        @param[in]  ivSize       カウンタの初期値データのバイトサイズ。

        @pre
        - pBlockCipher で指定するインスタンスは EncryptBlock を実行できる。

        @post
        - インスタンスは追加認証データ処理可能状態になる。

        @details
        pBlockCipher には暗号化に使用するブロック暗号を実装するインスタンスを指定します。
        あらかじめ鍵の設定など必要な初期化を行い、EncryptBlock が実行できる状態になっている必要があります。

        カウンタの初期値は暗号化ごとに異なるものを使用する必要があります。
        推奨サイズは 12 バイトです。

        pBlockCipher で指定したインスタンスはコピーされません。
        一連の処理が完了するまで対象 pBlockCipher に対する処理は行わないでください。

        カウンタの初期値データは内部にコピーされます。
     */
    void Initialize(const BlockCipher* pBlockCipher,
                    const void* pIv, size_t ivSize) NN_NOEXCEPT
    {
        m_Impl.Initialize(*pBlockCipher);
        m_Impl.Reset(pIv, ivSize);
    }

    /**
        @brief      認証付き暗号化の処理をリセットします。

        @pre
        - インスタンスは未初期化状態でない。

        @post
        - インスタンスは追加認証データ処理可能状態になる。

        @details
        指定されたカウンタの初期値で処理をリセットします。
        この関数を実行する前に Initialize() が行われている必要があります。

        カウンタの初期値データは内部にコピーされます。
     */
    void Reset( const void* pIv, size_t ivSize) NN_NOEXCEPT
    {
        m_Impl.Reset( pIv, ivSize );
    }

    /**
        @brief      追加認証データ(Additional Authentication Data)を入力します。

        @param[in]  pAad    追加認証データが格納されているバッファへのポインタ。
        @param[in]  aadSize 入力する追加認証データのバイトサイズ。

        @pre
        - インスタンスは追加認証データ処理可能状態である。

        @details
        追加認証データとは暗号化の対象とならず MAC の対象にのみなるデータです。
        pSrc から指定のサイズ分のデータを読み取り、MAC を計算します。

        この関数は複数回に分けて呼び出すことができます。
        ただし、入力されたデータはすべてが連結されたひとまとまりのデータとして扱われるため、入力する順序が意味を持つことに注意してください。

        すべての追加認証データの入力は Update() の処理を開始する前に完了させる必要があります。
      */
    void UpdateAad(const void* pAad, size_t aadSize) NN_NOEXCEPT
    {
        m_Impl.UpdateAad(pAad, aadSize);
    }

    /**
        @brief      暗号化を行います。

        @param[out] pDst    暗号化されたデータを受け取るバッファへのポインタ。
        @param[in]  dstSize pDst が指すバッファのバイトサイズ。
        @param[in]  pSrc    暗号化するデータが格納されているバッファへのポインタ。
        @param[in]  srcSize 暗号化するデータのバイトサイズ。

        @return     実際に暗号化されたバイト数

        @pre
        - インスタンスは追加認証データ処理可能状態または暗号化処理可能状態である。
        - dstSize >= srcSize
        - pDst, dstSize で指定する領域と pSrc, srcSize で指定する領域に重なりがない。

        @post
        - インスタンスは暗号化処理可能状態である。

        @details
        GCM モードではバイト単位で暗号化処理が行われるため 1 回の呼び出しで必要になる dstSize は srcSize と同じだけ必要です。

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
        - インスタンスは追加認証データ処理可能状態または暗号化処理可能状態または完了状態である。
        - macSize >= MacSize

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
    detail::GcmModeImpl<BlockCipher> m_Impl;
};

NN_DEFINE_STATIC_CONSTANT(template<typename T> const size_t GcmEncryptor<T>::BlockSize);
NN_DEFINE_STATIC_CONSTANT(template<typename T> const size_t GcmEncryptor<T>::MacSize);

}} // namespace nn::crypto
