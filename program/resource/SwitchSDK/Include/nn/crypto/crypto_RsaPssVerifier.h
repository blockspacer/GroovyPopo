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
    @brief RSA-PSS 署名を検証するための API の宣言
 */

#include <nn/crypto/crypto_RsaCalculator.h>
#include <nn/crypto/detail/crypto_RsaPssPadding.h>

namespace nn { namespace crypto {

/**
    @brief      RSA-PSS 署名の検証を行うクラスです。

    @tparam     KeySize      使用する鍵のバイトサイズ
    @tparam     HashFunction 使用するハッシュ関数の実装クラス

    @details
    RSA-PSS 署名は PKCS #1 で規定される RSA 署名アルゴリズムを利用した署名方式です。

    このクラスは鍵サイズとハッシュ関数の実装クラスをテンプレート引数とするテンプレートクラスです。
    実際の使用には @ref Rsa2048PssSha256Verifier などを利用してください。

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
class RsaPssVerifier
{
    NN_DISALLOW_COPY(RsaPssVerifier);

public:
    static const size_t HashSize = HashFunction::HashSize;        //!< ハッシュ関数の出力サイズを表す定数です。
    static const size_t DefaultSaltSize = HashFunction::HashSize; //!< 推奨される Salt のサイズを表す定数です。
    static const size_t SignatureSize = KeySize;                  //!< 署名のサイズを表す定数です。
    static const size_t RequiredWorkBufferSize = RsaCalculator<KeySize, 3>::RequiredWorkBufferSize; //!< ワークバッファとして必要になるサイズです。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    RsaPssVerifier() NN_NOEXCEPT
        : m_State(State_None)
    {
    }

    /**
        @brief      署名の検証処理を初期化します。

        @param[in]  pModulus           RSA の法データへのポインタ。
        @param[in]  modulusSize        RSA の法データのバイトサイズ。
        @param[in]  pPublicExponent    RSA の公開指数データへのポインタ
        @param[in]  publicExponentSize RSA の公開指数データのバイトサイズ

        @return     初期化に成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられています。

        @pre
        - modulusSize <= KeySize
        - publicExponentSize <= 3

        @post
        - インスタンスは処理可能状態になる。

        @details
        パラメータを入力し、署名の検証処理を初期化します。
        パラメータは内部にコピーされます。

        法と指数データは常に符号なし、ビッグエンディアンとして扱います。

        RSA の公開指数は 65537 など最大で 3 バイトの値が指定できます。
     */
    bool Initialize( const void* pModulus, size_t modulusSize,
                     const void* pPublicExponent, size_t publicExponentSize ) NN_NOEXCEPT
    {
        m_Hash.Initialize();
        bool ret = m_Key.Initialize(pModulus, modulusSize, pPublicExponent, publicExponentSize);
        if (ret)
        {
            m_State = State_Initialized;
        }
        return ret;
    }

    /**
        @brief      署名の検証対象のデータを入力します。

        @param[in]  pData    検証対象のデータへのポインタ。
        @param[in]  dataSize 検証対象のデータのバイトサイズ。

        @pre
        - インスタンスは処理可能状態である。

        @details
        検証対象のデータのハッシュ計算はクラスの内部で行われます。

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
        @brief      署名を検証します。作業領域としてスタックを利用します。

        @param[in]  pSignature    署名データへのポインタ。
        @param[in]  signatureSize 署名データのバイトサイズ。

        @return     署名の検証に成功したかを返します。@n
                    失敗した場合は正しい署名データではないか不正なパラメータが与えられています。

        @pre
        - インスタンスは処理可能状態である。
        - signatureSize == SignatureSize

        @post
        - インスタンスは完了状態になる。

        @details
        指定された署名データがこれまで入力された検証対象のデータに対して正しい RSA-PSS 署名であることを検証します。

        ワークバッファとしてスタックから RequiredWorkBufferSize バイトが利用されます。
     */
    bool Verify(const void* pSignature, size_t signatureSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_Initialized,
                        "Invalid state. Please restart from Initialize().");

        detail::RsaPssPadding<HashFunction> impl;
        uint8_t plain[SignatureSize];
        bool ret = m_Key.ModExp(plain, sizeof(plain), pSignature, signatureSize)
                   && impl.CheckPad( plain, sizeof(plain), &m_Hash );

        m_State = State_Done;

        return ret;
    }

    /**
        @brief      署名を検証します。作業領域として与えられたワークバッファを利用します。

        @param[in]  pSignature     署名データへのポインタ。
        @param[in]  signatureSize  署名データのバイトサイズ。
        @param[in]  pWorkBuffer    ワークバッファへのポインタ
        @param[in]  workBufferSize ワークバッファのサイズ

        @return     署名の検証に成功したかを返します。@n
                    失敗した場合は正しい署名データではないか不正なパラメータが与えられている、あるいはワークバッファが不足しています。

        @pre
        - インスタンスは処理可能状態である。
        - signatureSize == SignatureSize
        - workBufferSize >= RequiredWorkBufferSize

        @post
        - インスタンスは完了状態になる。

        @details
        指定された署名データがこれまで入力された検証対象のデータに対して正しい RSA-PSS 署名であることを検証します。

        ワークバッファは RequiredWorkBufferSize バイト以上が必要です。
     */
    bool Verify(const void* pSignature, size_t signatureSize, void* pWorkBuffer, size_t workBufferSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_Initialized,
                        "Invalid state. Please restart from Initialize().");

        detail::RsaPssPadding<HashFunction> impl;
        uint8_t plain[SignatureSize];
        bool ret = m_Key.ModExp(plain, sizeof(plain), pSignature, signatureSize, pWorkBuffer, workBufferSize)
                   && impl.CheckPad( plain, sizeof(plain), &m_Hash );

        m_State = State_Done;

        return ret;
    }

    /**
        @brief      RSA-PSS 署名を検証する関数です。作業領域としてスタックを利用します。

        @param[in]  pSignature           署名データへのポインタ。
        @param[in]  signatureSize        署名データのバイトサイズ。
        @param[in]  pModulus             RSA の法データへのポインタ。
        @param[in]  modulusSize          RSA の法データのバイトサイズ。
        @param[in]  pPublicExponent      RSA の公開指数データへのポインタ
        @param[in]  publicExponentSize   RSA の公開指数データのバイトサイズ
        @param[in]  pRawMessage          検証対象のデータへのポインタ。
        @param[in]  rawMessageSize       検証対象のデータのバイトサイズ。

        @return     署名の検証に成功したかを返します。@n
                    失敗した場合は正しい署名データではないか不正なパラメータが与えられています。

        @pre
        - signatureSize == SignatureSize
        - modulusSize <= KeySize
        - publicExponentSize <= 3

        @details
        指定された署名データが指定された検証対象のデータに対して正しい RSA-PSS 署名であることを検証します。

        法と指数データは常に符号なし、ビッグエンディアンとして扱います。

        検証対象のデータのハッシュ値は内部で計算されます。

        ワークバッファとしてスタックから RequiredWorkBufferSize バイトが利用されます。
     */
    static bool Verify( const void* pSignature, size_t signatureSize,
                        const void* pModulus, size_t modulusSize,
                        const void* pPublicExponent, size_t publicExponentSize,
                        const void* pRawMessage, size_t rawMessageSize ) NN_NOEXCEPT
    {
        RsaPssVerifier<KeySize,HashFunction> pss;
        if (!pss.Initialize(pModulus, modulusSize, pPublicExponent, publicExponentSize))
        {
            return false;
        }
        pss.Update(pRawMessage, rawMessageSize);
        return pss.Verify(pSignature, signatureSize);
    }

    /**
        @brief      RSA-PSS 署名を検証する関数です。作業領域として与えられたワークバッファを利用します。

        @param[in]  pSignature           署名データへのポインタ。
        @param[in]  signatureSize        署名データのバイトサイズ。
        @param[in]  pModulus             RSA の法データへのポインタ。
        @param[in]  modulusSize          RSA の法データのバイトサイズ。
        @param[in]  pPublicExponent      RSA の公開指数データへのポインタ
        @param[in]  publicExponentSize   RSA の公開指数データのバイトサイズ
        @param[in]  pRawMessage          検証対象のデータへのポインタ。
        @param[in]  rawMessageSize       検証対象のデータのバイトサイズ。
        @param[in]  pWorkBuffer          ワークバッファへのポインタ。
        @param[in]  workBufferSize       ワークバッファのバイトサイズ。

        @return     署名の検証に成功したかを返します。@n
                    失敗した場合は正しい署名データではないか不正なパラメータが与えられている、あるいはワークバッファが不足しています。

        @pre
        - signatureSize == SignatureSize
        - modulusSize <= KeySize
        - publicExponentSize <= 3
        - workBufferSize >= RequiredWorkBufferSize

        @details
        指定された署名データが指定された検証対象のデータに対して正しい RSA-PSS 署名であることを検証します。

        法と指数データは常に符号なし、ビッグエンディアンとして扱います。

        検証対象のデータのハッシュ値は内部で計算されます。

        ワークバッファは RequiredWorkBufferSize バイト以上が必要です。
     */
    static bool Verify( const void* pSignature, size_t signatureSize,
                        const void* pModulus, size_t modulusSize,
                        const void* pPublicExponent, size_t publicExponentSize,
                        const void* pRawMessage, size_t rawMessageSize,
                        void* pWorkBuffer, size_t workBufferSize ) NN_NOEXCEPT
    {
        RsaPssVerifier<KeySize,HashFunction> pss;
        if (!pss.Initialize(pModulus, modulusSize, pPublicExponent, publicExponentSize))
        {
            return false;
        }
        pss.Update(pRawMessage, rawMessageSize);
        return pss.Verify(pSignature, signatureSize, pWorkBuffer, workBufferSize);
    }

private:
    enum State
    {
        State_None,
        State_Initialized,
        State_Done,
    };

private:
    RsaCalculator<KeySize,3> m_Key; // Supported public exponent is 65537 at most
    HashFunction m_Hash;
    State m_State;
};

template<size_t T, typename U> const size_t RsaPssVerifier<T, U>::HashSize;
template<size_t T, typename U> const size_t RsaPssVerifier<T, U>::DefaultSaltSize;
template<size_t T, typename U> const size_t RsaPssVerifier<T, U>::SignatureSize;
template<size_t T, typename U> const size_t RsaPssVerifier<T, U>::RequiredWorkBufferSize;

}} // namespace nn::crypto
