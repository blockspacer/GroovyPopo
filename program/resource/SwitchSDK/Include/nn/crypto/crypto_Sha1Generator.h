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
    @brief SHA-1 ハッシュ値の計算を行う API の宣言
 */

#include <nn/nn_Common.h>
#include <nn/crypto/crypto_Config.h>
#include <nn/crypto/detail/crypto_Sha1Impl.h>

namespace nn { namespace crypto {

/**
    @brief  SHA-1 ハッシュを計算するクラスです。

    @details
    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 処理可能状態
    - 完了状態

    @note
    SHA-1 は脆弱性が指摘されており、過去システムとの互換性以外の用途では非推奨です。

 */
class Sha1Generator
{
    NN_DISALLOW_COPY(Sha1Generator);

public:
    static const size_t HashSize = 20;  //!< ハッシュ値のサイズを表す定数です。
    static const size_t BlockSize = 64; //!< ハッシュ値の計算処理単位のサイズを表す定数です。

    static const size_t Asn1ObjectIdentifierSize = 15; //!< SHA-1 のオブジェクト ID を DER 符号化したバイナリ列のサイズを表す定数です。
    static const uint8_t Asn1ObjectIdentifier[Asn1ObjectIdentifierSize]; //!< SHA-1 のオブジェクト ID を DER 符号化したバイナリ列です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    Sha1Generator() NN_NOEXCEPT {}

    /**
        @brief    ハッシュ計算を初期化します。

        @post
        - インスタンスは処理可能状態になる。

        @details
        ハッシュ値が規定の初期値で初期化され、ハッシュ計算が開始されます。
     */
    void   Initialize() NN_NOEXCEPT
    {
        m_Impl.Initialize();
    }

    /**
        @brief    ハッシュ値を与えられたデータで更新します。

        @param[in]  pData    入力データへのポインタ。
        @param[in]  dataSize 入力データのバイトサイズ。

        @pre
        - インスタンスは処理可能状態である。

        @details
        この関数は複数回に分けて呼び出すことができます。
        ただし、入力されたデータはすべてが連結されたひとまとまりのデータとして扱われるため、入力する順序が意味を持つことに注意してください。
     */
    void   Update(const void* pData, size_t dataSize) NN_NOEXCEPT
    {
        m_Impl.Update(pData, dataSize);
    }

    /**
        @brief    ハッシュ計算を完了し、最終的なハッシュ値を取得します。

        @param[out] pHash    ハッシュ値を格納するバッファへのポインタ。
        @param[in]  hashSize pHash が指すバッファのバイトサイズ。

        @pre
        - hashSize >= HashSize
        - インスタンスは処理可能状態または完了状態である。

        @post
        - pHash に計算されたハッシュ値が書き込まれる。
        - インスタンスは完了状態になる。

        @details
        ハッシュ計算が完了されます。
        別のハッシュ計算を行う場合は Initialize() を呼んで新たに計算を開始してください。
     */
    void   GetHash(void* pHash, size_t hashSize) NN_NOEXCEPT
    {
        m_Impl.GetHash(pHash, hashSize);
    }

private:
    detail::Sha1Impl m_Impl;
};

//! @name ハッシュ
//! @{

/**
    @brief    SHA-1 を計算するユーティリティ関数です。

    @param[out] pHash    ハッシュ値を格納するバッファへのポインタ。
    @param[in]  hashSize pHash が指すバッファのバイトサイズ。
    @param[in]  pData    入力データへのポインタ。
    @param[in]  dataSize 入力データのバイトサイズ。

    @pre
    - hashSize >= HashSize

    @post
    - pHash に計算されたハッシュ値が書き込まれる

    @details
    与えられたデータ列のハッシュ値を計算します。
    複数に分割されたデータ列のハッシュ値を計算したい場合は Sha1Generator クラスを利用してください。

    @note
    SHA-1 は脆弱性が指摘されており、過去システムとの互換性以外の用途では非推奨です。
 */
void   GenerateSha1Hash(void* pHash, size_t hashSize, const void* pData,
                        size_t dataSize) NN_NOEXCEPT;

//! @}

}} // namespace nn::crypto
