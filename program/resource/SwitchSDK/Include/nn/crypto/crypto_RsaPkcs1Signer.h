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
    @brief RSA-PKCS1 署名を生成するための API の宣言
 */

#include <nn/crypto/crypto_RsaCalculator.h>
#include <nn/crypto/detail/crypto_RsaPkcs1Padding.h>

namespace nn { namespace crypto {

/**
    @brief      RSA-PKCS1 署名の生成を行うクラスです。

    @tparam     KeySize      使用する鍵のバイトサイズ
    @tparam     HashFunction 使用するハッシュ関数の実装クラス

    @details
    RSA-PKCS1 署名は PKCS #1 で規定される RSA 署名アルゴリズムを利用した署名方式です。

    このクラスは鍵サイズとハッシュ関数の実装クラスをテンプレート引数とするテンプレートクラスです。
    実際の使用には @ref Rsa2048Pkcs1Sha256Signer などを利用してください。

    鍵のサイズは 1 バイト (8 ビット) 単位で最大 512 バイト (4096 ビット) まで指定できます。

    テンプレートとして指定するハッシュ関数クラスは以下の条件を満たす実装になっている必要があります。
    各関数の引数など詳細な説明については Sha1Generator クラスなどを参照してください。

    -# BlockSize という処理するブロックサイズが定義された定数を持つ
    -# HashSize というハッシュ値の出力サイズが定義された定数を持つ
    -# Asn1ObjectIdentifier というオブジェクト ID を DER 符号化したバイナリ列のメンバを持つ
    -# Asn1ObjectIdentifierSize というオブジェクト ID を DER 符号化したバイナリ列のサイズが定義された定数を持つ
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
class RsaPkcs1Signer
{
    NN_DISALLOW_COPY(RsaPkcs1Signer);

public:
    static const size_t HashSize = HashFunction::HashSize; //!< ハッシュ関数の出力サイズを表す定数です。
    static const size_t SignatureSize = KeySize;           //!< 署名のサイズを表す定数です。
    static const size_t RequiredWorkBufferSize = RsaCalculator<KeySize, KeySize>::RequiredWorkBufferSize; //!< ワークバッファとして必要になるサイズです。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    RsaPkcs1Signer() NN_NOEXCEPT
        : m_State(State_None)
    {
    }

    /**
        @brief      署名の生成処理を初期化します。

        @param[in]  pModulus             RSA の法データへのポインタ。
        @param[in]  modulusSize          RSA の法データのバイトサイズ。
        @param[in]  pPrivateExponent     RSA の秘密指数データへのポインタ
        @param[in]  privateExponentSize  RSA の秘密指数データのバイトサイズ

        @return     初期化に成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられています。

        @pre
        - modulusSize <= KeySize
        - privateExponentSize <= KeySize

        @post
        - インスタンスは処理可能状態になる。

        @details
        パラメータを入力し、署名の生成処理を初期化します。
        パラメータは内部にコピーされます。

        法と指数データは常に符号なし、ビッグエンディアンとして扱います。
     */
    bool Initialize( const void* pModulus, size_t modulusSize,
                     const void* pPrivateExponent, size_t privateExponentSize ) NN_NOEXCEPT
    {
        m_Hash.Initialize();
        bool ret = m_Key.Initialize(pModulus, modulusSize, pPrivateExponent, privateExponentSize);
        if (ret)
        {
            m_State = State_Initialized;
        }
        return ret;
    }

    /**
        @brief      署名対象のデータを入力します。

        @param[in]  pData    署名対象のデータへのポインタ。
        @param[in]  dataSize 署名対象のデータのバイトサイズ。

        @pre
        - インスタンスは処理可能状態である。

        @details
        対象のデータのハッシュ計算はクラスの内部で行われます。

        この関数は複数回に分けて呼び出すことができます。
        ただし、入力されたデータはすべてが連結されたひとまとまりのデータとして扱われるため、入力する順序が意味を持つことに注意してください。
     */
    void Update(const void* pData, size_t dataSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_Initialized,
                        "Invalid state. Please restart from Initialize().");

        m_Hash.Update( pData, dataSize );
    }

    /**
        @brief      署名を生成します。作業領域としてスタックを利用します。

        @param[out] pOutSignature 署名データが格納されるバッファへのポインタ。
        @param[in]  signatureSize pOutSignature が指すバッファのバイトサイズ。

        @return     署名の生成に成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられています。

        @pre
        - インスタンスは処理可能状態である。
        - signatureSize == SignatureSize

        @post
        - pOutSignature に署名が格納される。
        - インスタンスは完了状態になる。

        @details
        これまでに入力されたデータに対して RSA-PKCS1 署名を生成します。

        ワークバッファとしてスタックから RequiredWorkBufferSize バイトが利用されます。
     */
    bool Sign(void* pOutSignature, size_t signatureSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_Initialized,
                        "Invalid state. Please restart from Initialize().");

        detail::RsaPkcs1Padding<HashFunction> impl;
        impl.BuildPad( pOutSignature, signatureSize, &m_Hash );
        if (!m_Key.ModExp(pOutSignature, signatureSize, pOutSignature, signatureSize))
        {
            std::memset( pOutSignature, 0, signatureSize );
            return false;
        }
        m_State = State_Done;
        return true;
    }

    /**
        @brief      署名を生成します。作業領域として与えられたワークバッファを利用します。

        @param[out] pOutSignature  署名データが格納されるバッファへのポインタ。
        @param[in]  signatureSize  pOutSignature が指すバッファのバイトサイズ。
        @param[in]  pWorkBuffer    ワークバッファへのポインタ
        @param[in]  workBufferSize ワークバッファのサイズ

        @return     署名の生成に成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられているか、ワークバッファが不足しています。

        @pre
        - インスタンスは処理可能状態である。
        - signatureSize == SignatureSize
        - workBufferSize >= RequiredWorkBufferSize

        @post
        - pOutSignature に署名が格納される。
        - インスタンスは完了状態になる。

        @details
        これまでに入力されたデータに対して RSA-PKCS1 署名を生成します。

        ワークバッファは RequiredWorkBufferSize バイト以上が必要です。
     */
    bool Sign(void* pOutSignature, size_t signatureSize, void* pWorkBuffer, size_t workBufferSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_Initialized,
                        "Invalid state. Please restart from Initialize().");

        detail::RsaPkcs1Padding<HashFunction> impl;
        impl.BuildPad( pOutSignature, signatureSize, &m_Hash );
        if (!m_Key.ModExp(pOutSignature, signatureSize, pOutSignature, signatureSize,
                          pWorkBuffer, workBufferSize))
        {
            std::memset( pOutSignature, 0, signatureSize );
            return false;
        }
        m_State = State_Done;
        return true;
    }

    /**
        @brief      RSA-PKCS1 署名を生成する関数です。作業領域としてスタックを利用します。

        @param[out] pOutSignature          署名データが格納されるバッファへのポインタ。
        @param[in]  signatureSize          pOutSignature が指すバッファのバイトサイズ。
        @param[in]  pModulus               RSA の法データへのポインタ。
        @param[in]  modulusSize            RSA の法データのバイトサイズ。
        @param[in]  pPrivateExponent       RSA の秘密指数データへのポインタ
        @param[in]  privateExponentSize    RSA の秘密指数データのバイトサイズ
        @param[in]  pRawMessage            署名対象のデータへのポインタ。
        @param[in]  rawMessageSize         署名対象のデータのバイトサイズ。

        @return     署名の生成に成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられています。

        @pre
        - signatureSize == SignatureSize
        - modulusSize <= KeySize
        - privateExponentSize <= KeySize

        @post
        - pOutSignature に署名が格納される。

        @details
        与えられたデータ列の RSA-PKCS1 署名を生成します。

        法と指数データは常に符号なし、ビッグエンディアンとして扱います。

        ワークバッファとしてスタックから RequiredWorkBufferSize バイトが利用されます。
     */
    static bool Sign(void* pOutSignature, size_t signatureSize,
                     const void* pModulus, size_t modulusSize,
                     const void* pPrivateExponent, size_t privateExponentSize,
                     const void* pRawMessage, size_t rawMessageSize ) NN_NOEXCEPT
    {
        RsaPkcs1Signer<KeySize, HashFunction> signer;
        if (!signer.Initialize(pModulus, modulusSize, pPrivateExponent, privateExponentSize))
        {
            return false;
        }
        signer.Update(pRawMessage, rawMessageSize);
        return signer.Sign(pOutSignature, signatureSize);
    }

    /**
        @brief      RSA-PKCS1 署名を生成する関数です。作業領域として与えられたワークバッファを利用します。

        @param[out] pOutSignature          署名データが格納されるバッファへのポインタ。
        @param[in]  signatureSize          pOutSignature が指すバッファのバイトサイズ。
        @param[in]  pModulus               RSA の法データへのポインタ。
        @param[in]  modulusSize            RSA の法データのバイトサイズ。
        @param[in]  pPrivateExponent       RSA の秘密指数データへのポインタ
        @param[in]  privateExponentSize    RSA の秘密指数データのバイトサイズ
        @param[in]  pRawMessage            署名対象のデータへのポインタ。
        @param[in]  rawMessageSize         署名対象のデータのバイトサイズ。
        @param[in]  pWorkBuffer            ワークバッファへのポインタ。
        @param[in]  workBufferSize         ワークバッファのバイトサイズ。

        @return     署名の生成に成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられているか、ワークバッファが不足しています。

        @pre
        - signatureSize == SignatureSize
        - modulusSize <= KeySize
        - privateExponentSize <= KeySize
        - workBufferSize >= RequiredWorkBufferSize

        @post
        - pOutSignature に署名が格納される。

        @details
        与えられたデータ列の RSA-PKCS1 署名を生成します。

        法と指数データは常に符号なし、ビッグエンディアンとして扱います。

        ワークバッファは RequiredWorkBufferSize バイト以上が必要です。
     */
    static bool Sign(void* pOutSignature, size_t signatureSize,
                     const void* pModulus, size_t modulusSize,
                     const void* pPrivateExponent, size_t privateExponentSize,
                     const void* pRawMessage, size_t rawMessageSize,
                     void* pWorkBuffer, size_t workBufferSize ) NN_NOEXCEPT
    {
        RsaPkcs1Signer<KeySize, HashFunction> signer;
        if (!signer.Initialize(pModulus, modulusSize, pPrivateExponent, privateExponentSize))
        {
            return false;
        }
        signer.Update(pRawMessage, rawMessageSize);
        return signer.Sign(pOutSignature, signatureSize, pWorkBuffer, workBufferSize);
    }

private:
    enum State
    {
        State_None,
        State_Initialized,
        State_Done,
    };

private:
    nn::crypto::RsaCalculator<KeySize, KeySize> m_Key;
    HashFunction m_Hash;
    State m_State;
};

template<size_t T, typename U> const size_t RsaPkcs1Signer<T, U>::HashSize;
template<size_t T, typename U> const size_t RsaPkcs1Signer<T, U>::SignatureSize;
template<size_t T, typename U> const size_t RsaPkcs1Signer<T, U>::RequiredWorkBufferSize;

}} // namespace nn::crypto
