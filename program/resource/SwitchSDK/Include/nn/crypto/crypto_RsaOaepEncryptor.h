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
    @brief RSA-OAEP による暗号化を利用するための API の宣言
 */

#include <nn/crypto/crypto_RsaCalculator.h>
#include <nn/crypto/detail/crypto_RsaOaepPadding.h>

namespace nn { namespace crypto {

/**
    @brief      RSA-OAEP による暗号化を行うクラスです。

    @tparam     KeySize      使用する鍵のバイトサイズ
    @tparam     HashFunction 使用するハッシュ関数の実装クラス

    @details
    RSA-OAEP は PKCS #1 で規定される RSA 暗号アルゴリズムを利用した暗号化方式です。

    このクラスは鍵サイズとハッシュ関数の実装クラスをテンプレート引数とするテンプレートクラスです。

    鍵のサイズは 1 バイト (8 ビット) 単位で最大 512 バイト (4096 ビット) まで指定できます。

    テンプレートとして指定するハッシュ関数クラスは以下の条件を満たす実装になっている必要があります。
    各関数の引数など詳細な説明については Sha1Generator クラスなどを参照してください。

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
template <size_t KeySize, typename HashFunction>
class RsaOaepEncryptor
{
    NN_DISALLOW_COPY(RsaOaepEncryptor);

public:
    static const size_t HashSize = HashFunction::HashSize; //!< ハッシュ関数の出力サイズを表す定数です。
    static const size_t BlockSize = KeySize;               //!< 出力サイズを表す定数です。
    static const size_t RequiredWorkBufferSize = RsaCalculator<KeySize, 3>::RequiredWorkBufferSize; //!< ワークバッファとして必要になるサイズです。

public:

    /**
        @brief      デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    RsaOaepEncryptor() NN_NOEXCEPT
    {
        m_State = State_None;
        m_IsLabelDigestSet = false;
        std::memset(m_LabelDigest, 0, sizeof(m_LabelDigest));
    }

    /**
        @brief      暗号化処理を初期化します。

        @param[in]  pModulus             RSA の法データへのポインタ。
        @param[in]  modulusSize          RSA の法データのバイトサイズ。
        @param[in]  pPublicExponent      RSA の公開指数データへのポインタ
        @param[in]  publicExponentSize   RSA の公開指数データのバイトサイズ

        @return     初期化が成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられています。

        @pre
        - modulusSize <= KeySize
        - publicExponentSize <= 3

        @post
        - インスタンスは処理可能状態になる。

        @details
        パラメータを入力し、暗号化処理を初期化します。
        パラメータは内部にコピーされます。

        法と指数データは常に符号なし、ビッグエンディアンとして扱います。

        RSA の公開指数は 65537 など最大で 3 バイトの値が指定できます。
     */
    bool Initialize( const void* pModulus, size_t modulusSize,
                     const void* pPublicExponent, size_t publicExponentSize) NN_NOEXCEPT
    {
        m_Hash.Initialize();
        m_IsLabelDigestSet = false;

        bool ret = m_Key.Initialize(pModulus, modulusSize, pPublicExponent, publicExponentSize);
        if (ret)
        {
            m_State = State_Initialized;
        }

        return ret;
    }

    /**
        @brief      ラベルを入力します。

        @param[in]  pLabel               ラベルのデータへのポインタ。
        @param[in]  labelSize            ラベルのバイトサイズ。

        @pre
        - インスタンスは処理可能状態である。

        @details
        ラベルは暗号化対象のメッセージとは別に付加することができるデータです。

        この関数は複数回に分けて呼び出すことができます。
        ただし、入力されたデータはすべてが連結されたひとまとまりのデータとして扱われるため、入力する順序が意味を持つことに注意してください。

        すべてのラベルの入力は Encrypt() の処理をする前に完了させる必要があります。

        ラベルのハッシュ値があらかじめわかっている場合はこの関数のかわりに SetLabelDigest() で入力することができます。
     */
    void UpdateLabel(const void* pLabel, size_t labelSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_Initialized,
                        "Invalid state. Please restart from Initialize().");

        m_Hash.Update(pLabel, labelSize);
    }

    /**
        @brief      ラベルのハッシュ値を入力します。

        @param[in]  pLabelDigest         ラベルのハッシュ値へのポインタ。
        @param[in]  labelDigestSize      ラベルのハッシュ値のバイトサイズ。

        @pre
        - インスタンスは処理可能状態である。

        @details
        ラベルのハッシュ値があらかじめわかっている場合は UpdateLabel() 関数のかわりにこの関数で指定することができます。

        ラベルのハッシュ値の入力は Encrypt() の処理をする前に完了させる必要があります。
     */
    void SetLabelDigest(const void* pLabelDigest, size_t labelDigestSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_Initialized,
                        "Invalid state. Please restart from Initialize().");
        NN_SDK_REQUIRES(labelDigestSize == HashSize);
        NN_UNUSED(labelDigestSize);
        std::memcpy( m_LabelDigest, pLabelDigest, HashSize );
        m_IsLabelDigestSet = true;
    }

    /**
        @brief      暗号化を行います。作業領域としてスタックを利用します。

        @param[out] pOutCipher           暗号化されたデータを受け取るバッファへのポインタ。
        @param[in]  cipherSize           pOutCipher が指すバッファのバイトサイズ。
        @param[in]  pRawMessage          暗号化するデータが格納されているバッファへのポインタ。
        @param[in]  rawMessageSize       暗号化するデータのバイトサイズ。
        @param[in]  pSeed                Seed データへのポインタ。
        @param[in]  seedSize             Seed データのサイズ。

        @return     暗号化に成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられています。

        @pre
        - インスタンスは処理可能状態である。
        - cipherSize == KeySize
        - rawMessageSize <= KeySize - 2 * HashSize - 2

        @post
        - インスタンスは完了状態になる。
        - pOutCipher に暗号化された結果が書き込まれる。

        @details
        与えられたデータ列を暗号化します。

        Seed データには暗号化を行うごとに異なる HashSize バイトの乱数を指定してください。

        ワークバッファとしてスタックから RequiredWorkBufferSize バイトが利用されます。
     */
    bool Encrypt(void* pOutCipher, size_t cipherSize,
                 const void* pRawMessage, size_t rawMessageSize,
                 const void* pSeed, size_t seedSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_Initialized,
                        "Invalid state. Please restart from Initialize().");

        detail::RsaOaepPadding<HashFunction> impl;
        if(m_IsLabelDigestSet == false)
        {
            m_Hash.GetHash(m_LabelDigest, sizeof(m_LabelDigest));
        }

        impl.BuildPad(pOutCipher, cipherSize, m_LabelDigest, sizeof(m_LabelDigest), pRawMessage, rawMessageSize,
                      pSeed, seedSize);

        if (!m_Key.ModExp(pOutCipher, cipherSize, pOutCipher, cipherSize))
        {
            std::memset(pOutCipher, 0, cipherSize);
            return false;
        }

        m_State = State_Done;

        return true;
    }

    /**
        @brief      暗号化を行います。作業領域として与えられたワークバッファを利用します。

        @param[out] pOutCipher           暗号化されたデータを受け取るバッファへのポインタ。
        @param[in]  cipherSize           pOutCipher が指すバッファのバイトサイズ。
        @param[in]  pRawMessage          暗号化するデータが格納されているバッファへのポインタ。
        @param[in]  rawMessageSize       暗号化するデータのバイトサイズ。
        @param[in]  pSeed                Seed データへのポインタ。
        @param[in]  seedSize             Seed データのサイズ。
        @param[in]  pWorkBuffer          ワークバッファへのポインタ
        @param[in]  workBufferSize       ワークバッファのサイズ

        @return     署名の生成に成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられているか、ワークバッファが不足しています。

        @pre
        - インスタンスは処理可能状態である。
        - cipherSize == KeySize
        - rawMessageSize <= KeySize - 2 * HashSize - 2
        - workBufferSize >= RequiredWorkBufferSize

        @post
        - インスタンスは完了状態になる。
        - pOutCipher に暗号化された結果が書き込まれる。

        @details
        与えられたデータ列を暗号化します。

        Seed データには暗号化を行うごとに異なる HashSize バイトの乱数を指定してください。

        ワークバッファは RequiredWorkBufferSize バイト以上が必要です。
     */
    bool Encrypt(void* pOutCipher, size_t cipherSize,
                 const void* pRawMessage, size_t rawMessageSize,
                 const void* pSeed, size_t seedSize,
                 void* pWorkBuffer, size_t workBufferSize ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_Initialized,
                        "Invalid state. Please restart from Initialize().");

        detail::RsaOaepPadding<HashFunction> impl;
        if(m_IsLabelDigestSet == false)
        {
            m_Hash.GetHash(m_LabelDigest, sizeof(m_LabelDigest));
        }

        impl.BuildPad(pOutCipher, cipherSize, m_LabelDigest, sizeof(m_LabelDigest), pRawMessage, rawMessageSize,
                      pSeed, seedSize);

        if (!m_Key.ModExp(pOutCipher, cipherSize, pOutCipher, cipherSize, pWorkBuffer, workBufferSize))
        {
            std::memset(pOutCipher, 0, cipherSize);
            return false;
        }

        m_State = State_Done;

        return true;
    }

    /**
        @brief      RSA-OAEP による暗号化を行う関数です。作業領域としてスタックを利用します。

        @param[out] pOutCipher           暗号化されたデータを受け取るバッファへのポインタ。
        @param[in]  cipherSize           pOutCipher が指すバッファのバイトサイズ。
        @param[in]  pModulus             RSA の法データへのポインタ。
        @param[in]  modulusSize          RSA の法データのバイトサイズ。
        @param[in]  pPublicExponent      RSA の公開指数データへのポインタ
        @param[in]  publicExponentSize   RSA の公開指数データのバイトサイズ
        @param[in]  pRawMessage          暗号化するデータが格納されているバッファへのポインタ。
        @param[in]  rawMessageSize       暗号化するデータのバイトサイズ。
        @param[in]  pSeed                Seed データへのポインタ。
        @param[in]  seedSize             Seed データのサイズ。
        @param[in]  pLabel               ラベルのデータへのポインタ。
        @param[in]  labelSize            ラベルのバイトサイズ。

        @return     暗号化に成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられています。

        @pre
        - modulusSize <= KeySize
        - publicExponentSize <= 3
        - cipherSize == KeySize
        - rawMessageSize <= KeySize - 2 * HashSize - 2

        @post
        - pOutCipher に暗号化された結果が書き込まれる。

        @details
        与えられたデータ列を暗号化します。

        法と指数データは常に符号なし、ビッグエンディアンとして扱います。

        RSA の公開指数は 65537 など最大で 3 バイトの値が指定できます。

        Seed データには暗号化を行うごとに異なる HashSize バイトの乱数を指定してください。

        ワークバッファとしてスタックから RequiredWorkBufferSize バイトが利用されます。
     */
    static bool Encrypt(void* pOutCipher, size_t cipherSize,
                        const void* pModulus, size_t modulusSize,
                        const void* pPublicExponent, size_t publicExponentSize,
                        const void* pRawMessage, size_t rawMessageSize,
                        const void* pSeed, size_t seedSize,
                        const void* pLabel, size_t labelSize ) NN_NOEXCEPT
    {
        RsaOaepEncryptor<KeySize, HashFunction> oaep;
        if (!oaep.Initialize(pModulus, modulusSize, pPublicExponent, publicExponentSize))
        {
            return false;
        }
        oaep.UpdateLabel(pLabel, labelSize);
        return oaep.Encrypt(pOutCipher, cipherSize, pRawMessage, rawMessageSize, pSeed, seedSize);
    }

    /**
        @brief      RSA-OAEP による暗号化を行う関数です。作業領域として与えられたワークバッファを利用します。

        @param[out] pOutCipher           暗号化されたデータを受け取るバッファへのポインタ。
        @param[in]  cipherSize           pOutCipher が指すバッファのバイトサイズ。
        @param[in]  pModulus             RSA の法データへのポインタ。
        @param[in]  modulusSize          RSA の法データのバイトサイズ。
        @param[in]  pPublicExponent      RSA の公開指数データへのポインタ
        @param[in]  publicExponentSize   RSA の公開指数データのバイトサイズ
        @param[in]  pRawMessage          暗号化するデータが格納されているバッファへのポインタ。
        @param[in]  rawMessageSize       暗号化するデータのバイトサイズ。
        @param[in]  pSeed                Seed データへのポインタ。
        @param[in]  seedSize             Seed データのサイズ。
        @param[in]  pLabel               ラベルのデータへのポインタ。
        @param[in]  labelSize            ラベルのバイトサイズ。
        @param[in]  pWorkBuffer          ワークバッファへのポインタ。
        @param[in]  workBufferSize       ワークバッファのバイトサイズ。

        @return     署名の生成に成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられているか、ワークバッファが不足しています。

        @pre
        - modulusSize <= KeySize
        - publicExponentSize <= 3
        - cipherSize == KeySize
        - rawMessageSize <= KeySize - 2 * HashSize - 2
        - workBufferSize >= RequiredWorkBufferSize

        @post
        - pOutCipher に暗号化された結果が書き込まれる。

        @details
        与えられたデータ列を暗号化します。

        法と指数データは常に符号なし、ビッグエンディアンとして扱います。

        RSA の公開指数は 65537 など最大で 3 バイトの値が指定できます。

        Seed データには暗号化を行うごとに異なる HashSize バイトの乱数を指定してください。

        ワークバッファは RequiredWorkBufferSize バイト以上が必要です。
     */
    static bool Encrypt(void* pOutCipher, size_t cipherSize,
                        const void* pModulus, size_t modulusSize,
                        const void* pPublicExponent, size_t publicExponentSize,
                        const void* pRawMessage, size_t rawMessageSize,
                        const void* pSeed, size_t seedSize,
                        const void* pLabel, size_t labelSize,
                        void* pWorkBuffer, size_t workBufferSize ) NN_NOEXCEPT
    {
        RsaOaepEncryptor<KeySize, HashFunction> oaep;
        if (!oaep.Initialize(pModulus, modulusSize, pPublicExponent, publicExponentSize))
        {
            return false;
        }
        oaep.UpdateLabel(pLabel, labelSize);
        return oaep.Encrypt(pOutCipher, cipherSize, pRawMessage, rawMessageSize, pSeed, seedSize,
                            pWorkBuffer, workBufferSize);
    }

private:
    enum State
    {
        State_None,
        State_Initialized,
        State_Done,
    };

private:
    nn::crypto::RsaCalculator<KeySize, 3> m_Key; // Supported public exponent is 65537 at most
    HashFunction m_Hash;
    bool         m_IsLabelDigestSet;
    uint8_t      m_LabelDigest[HashSize];
    State        m_State;
};

template<size_t T, typename U> const size_t RsaOaepEncryptor<T, U>::HashSize;
template<size_t T, typename U> const size_t RsaOaepEncryptor<T, U>::BlockSize;
template<size_t T, typename U> const size_t RsaOaepEncryptor<T, U>::RequiredWorkBufferSize;

}} // namespace nn::crypto
