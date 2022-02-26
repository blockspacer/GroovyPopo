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
    @brief RSA-OAEP による復号化を利用するための API の宣言
 */

#include <nn/crypto/crypto_RsaCalculator.h>
#include <nn/crypto/detail/crypto_RsaOaepPadding.h>

namespace nn { namespace crypto {

/**
    @brief      RSA-OAEP による復号化を行うクラスです。

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
class RsaOaepDecryptor
{
    NN_DISALLOW_COPY(RsaOaepDecryptor);

public:
    static const size_t HashSize = HashFunction::HashSize; //!< ハッシュ関数の出力サイズを表す定数です。
    static const size_t BlockSize = KeySize;               //!< 出力サイズを表す定数です。
    static const size_t RequiredWorkBufferSize = RsaCalculator<KeySize, KeySize>::RequiredWorkBufferSize; //!< ワークバッファとして必要になるサイズです。

public:
    /**
        @brief      デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    RsaOaepDecryptor() NN_NOEXCEPT
    {
        m_State = State_None;
        m_IsLabelDigestSet = false;
        std::memset(m_LabelDigest, 0, sizeof(m_LabelDigest));
    }

    /**
        @brief      復号化処理を初期化します。

        @param[in]  pModulus             RSA の法データへのポインタ。
        @param[in]  modulusSize          RSA の法データのバイトサイズ。
        @param[in]  pPrivateExponent     RSA の秘密指数データへのポインタ
        @param[in]  privateExponentSize  RSA の秘密指数データのバイトサイズ

        @return     初期化が成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられています。

        @pre
        - modulusSize <= KeySize
        - privateExponentSize <= KeySize

        @post
        - インスタンスは処理可能状態になる。

        @details
        パラメータを入力し、復号化処理を初期化します。
        パラメータは内部にコピーされます。

        法と指数データは常に符号なし、ビッグエンディアンとして扱います。
     */
    bool Initialize( const void* pModulus, size_t modulusSize,
                     const void* pPrivateExponent, size_t privateExponentSize) NN_NOEXCEPT
    {
        m_Hash.Initialize();
        m_IsLabelDigestSet = false;

        bool ret = m_Key.Initialize(pModulus, modulusSize, pPrivateExponent, privateExponentSize);
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
        復号化では暗号化時に指定されたラベルと同一のデータであるか検証されます。

        この関数は複数回に分けて呼び出すことができます。
        ただし、入力されたデータはすべてが連結されたひとまとまりのデータとして扱われるため、入力する順序が意味を持つことに注意してください。

        すべてのラベルの入力は Decrypt() の処理をする前に完了させる必要があります。

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

        ラベルの入力は Decrypt() の処理をする前に完了させる必要があります。
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
        @brief      復号化を行います。作業領域としてスタックを利用します。

        @param[out] pOutPlain            復号化されたデータを受け取るバッファへのポインタ。
        @param[in]  plainSize            pOutPlain が指すバッファのバイトサイズ。
        @param[in]  pCipher              復号化するデータが格納されているバッファへのポインタ。
        @param[in]  cipherSize           復号化するデータのバイトサイズ。

        @return     実際に復号化されたバイトサイズ。0 バイトのときは復号化に失敗しています。@n
                    失敗した場合は正しい暗号化データまたはラベルではないか不正なパラメータが与えられています。

        @pre
        - インスタンスは処理可能状態である。
        - cipherSize == KeySize

        @post
        - インスタンスは完了状態になる。
        - pOutPlain に復号化された結果が書き込まれる。

        @details
        与えられたデータ列を復号化します。

        ワークバッファとしてスタックから RequiredWorkBufferSize バイトが利用されます。
     */
    size_t Decrypt(void* pOutPlain, size_t plainSize,
                   const void* pCipher, size_t cipherSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_Initialized,
                        "Invalid state. Please restart from Initialize().");

        detail::RsaOaepPadding<HashFunction> impl;
        uint8_t plain[KeySize];

        if (!m_Key.ModExp(plain, sizeof(plain), pCipher, cipherSize))
        {
            std::memset(pOutPlain, 0, plainSize);
            return 0;
        }

        if(m_IsLabelDigestSet == false)
        {
            m_Hash.GetHash(m_LabelDigest, sizeof(m_LabelDigest));
        }
        size_t result = impl.RecoverFromPad(pOutPlain, plainSize, m_LabelDigest, sizeof(m_LabelDigest), plain, sizeof(plain));

        // Wipe out secret from stack
        std::memset( plain, 0, sizeof(plain) );

        m_State = State_Done;

        return result;
    }

    /**
        @brief      復号化を行います。作業領域として与えられたワークバッファを利用します。

        @param[out] pOutPlain            復号化されたデータを受け取るバッファへのポインタ。
        @param[in]  plainSize            pOutPlain が指すバッファのバイトサイズ。
        @param[in]  pCipher              復号化するデータが格納されているバッファへのポインタ。
        @param[in]  cipherSize           復号化するデータのバイトサイズ。
        @param[in]  pWorkBuffer          ワークバッファへのポインタ
        @param[in]  workBufferSize       ワークバッファのサイズ

        @return     実際に復号化されたバイトサイズ。0 バイトのときは復号化に失敗しています。@n
                    失敗した場合は正しい暗号化データまたはラベルではないか不正なパラメータが与えられている、あるいはワークバッファが不足しています。

        @pre
        - インスタンスは処理可能状態である。
        - cipherSize == KeySize
        - workBufferSize >= RequiredWorkBufferSize

        @post
        - インスタンスは完了状態になる。
        - pOutPlain に復号化された結果が書き込まれる。

        @details
        与えられたデータ列を復号化します。

        ワークバッファは RequiredWorkBufferSize バイト以上が必要です。
     */
    size_t Decrypt(void* pOutPlain, size_t plainSize,
                   const void* pCipher, size_t cipherSize,
                   void* pWorkBuffer, size_t workBufferSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(m_State == State_Initialized,
                        "Invalid state. Please restart from Initialize().");

        detail::RsaOaepPadding<HashFunction> impl;
        uint8_t plain[KeySize];

        if (!m_Key.ModExp(plain, sizeof(plain), pCipher, cipherSize, pWorkBuffer, workBufferSize))
        {
            std::memset(pOutPlain, 0, plainSize);
            return 0;
        }

        if(m_IsLabelDigestSet == false)
        {
            m_Hash.GetHash(m_LabelDigest, sizeof(m_LabelDigest));
        }
        size_t result = impl.RecoverFromPad(pOutPlain, plainSize, m_LabelDigest, sizeof(m_LabelDigest), plain, sizeof(plain));

        // Wipe out secret from stack
        std::memset( plain, 0, sizeof(plain) );

        m_State = State_Done;

        return result;
    }

    /**
        @brief      RSA-OAEP による復号化を行う関数です。作業領域としてスタックを利用します。

        @param[out] pOutPlain            復号化されたデータを受け取るバッファへのポインタ。
        @param[in]  plainSize            pOutPlain が指すバッファのバイトサイズ。
        @param[in]  pModulus             RSA の法データへのポインタ。
        @param[in]  modulusSize          RSA の法データのバイトサイズ。
        @param[in]  pPrivateExponent     RSA の秘密指数データへのポインタ
        @param[in]  privateExponentSize  RSA の秘密指数データのバイトサイズ
        @param[in]  pCipher              復号化するデータが格納されているバッファへのポインタ。
        @param[in]  cipherSize           復号化するデータのバイトサイズ。
        @param[in]  pLabel               ラベルのデータへのポインタ。
        @param[in]  labelSize            ラベルのバイトサイズ。

        @return     実際に復号化されたバイトサイズ。0 バイトのときは復号化に失敗しています。@n
                    失敗した場合は正しい暗号化データまたはラベルではないか不正なパラメータが与えられています。

        @pre
        - modulusSize <= KeySize
        - privateExponentSize <= KeySize
        - cipherSize == KeySize

        @post
        - pOutPlain に復号化された結果が書き込まれる。

        @details
        与えられたデータ列を復号化します。

        法と指数データは常に符号なし、ビッグエンディアンとして扱います。

        ワークバッファとしてスタックから RequiredWorkBufferSize バイトが利用されます。
     */
    static size_t Decrypt( void* pOutPlain, size_t plainSize,
                           const void* pModulus, size_t modulusSize,
                           const void* pPrivateExponent, size_t privateExponentSize,
                           const void* pCipher, size_t cipherSize,
                           const void* pLabel, size_t labelSize) NN_NOEXCEPT
    {
        RsaOaepDecryptor<KeySize, HashFunction> oaep;
        if (!oaep.Initialize(pModulus, modulusSize, pPrivateExponent, privateExponentSize))
        {
            return 0;
        }
        oaep.UpdateLabel(pLabel, labelSize);
        return oaep.Decrypt(pOutPlain, plainSize, pCipher, cipherSize);
    }

    /**
        @brief      RSA-OAEP による復号化を行う関数です。作業領域として与えられたワークバッファを利用します。

        @param[out] pOutPlain            復号化されたデータを受け取るバッファへのポインタ。
        @param[in]  plainSize            pOutPlain が指すバッファのバイトサイズ。
        @param[in]  pModulus             RSA の法データへのポインタ。
        @param[in]  modulusSize          RSA の法データのバイトサイズ。
        @param[in]  pPrivateExponent     RSA の秘密指数データへのポインタ
        @param[in]  privateExponentSize  RSA の秘密指数データのバイトサイズ
        @param[in]  pCipher              復号化するデータが格納されているバッファへのポインタ。
        @param[in]  cipherSize           復号化するデータのバイトサイズ。
        @param[in]  pLabel               ラベルのデータへのポインタ。
        @param[in]  labelSize            ラベルのバイトサイズ。
        @param[in]  pWorkBuffer          ワークバッファへのポインタ
        @param[in]  workBufferSize       ワークバッファのサイズ

        @return     実際に復号化されたバイトサイズ。0 バイトのときは復号化に失敗しています。@n
                    失敗した場合は正しい暗号化データまたはラベルではないか不正なパラメータが与えられている、あるいはワークバッファが不足しています。

        @pre
        - modulusSize <= KeySize
        - privateExponentSize <= KeySize
        - cipherSize == KeySize
        - workBufferSize >= RequiredWorkBufferSize

        @post
        - pOutPlain に復号化された結果が書き込まれる。

        @details
        与えられたデータ列を復号化します。

        法と指数データは常に符号なし、ビッグエンディアンとして扱います。

        ワークバッファは RequiredWorkBufferSize バイト以上が必要です。
     */
    static size_t Decrypt( void* pOutPlain, size_t plainSize,
                           const void* pModulus, size_t modulusSize,
                           const void* pPrivateExponent, size_t privateExponentSize,
                           const void* pCipher, size_t cipherSize,
                           const void* pLabel, size_t labelSize,
                           void* pWorkBuffer, size_t workBufferSize ) NN_NOEXCEPT
    {
        RsaOaepDecryptor<KeySize, HashFunction> oaep;
        if (!oaep.Initialize(pModulus, modulusSize, pPrivateExponent, privateExponentSize))
        {
            return 0;
        }
        oaep.UpdateLabel(pLabel, labelSize);
        return oaep.Decrypt(pOutPlain, plainSize, pCipher, cipherSize, pWorkBuffer, workBufferSize);
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
    bool         m_IsLabelDigestSet;
    uint8_t      m_LabelDigest[HashSize];
    State        m_State;
};

template<size_t T, typename U> const size_t RsaOaepDecryptor<T, U>::HashSize;
template<size_t T, typename U> const size_t RsaOaepDecryptor<T, U>::BlockSize;
template<size_t T, typename U> const size_t RsaOaepDecryptor<T, U>::RequiredWorkBufferSize;

}} // namespace nn::crypto
