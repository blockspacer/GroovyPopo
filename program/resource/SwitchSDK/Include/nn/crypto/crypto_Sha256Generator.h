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
    @brief SHA-256 ハッシュ値の計算を行う API の宣言
 */

#include <nn/nn_Common.h>
#include <nn/crypto/detail/crypto_Sha256Impl.h>

namespace nn { namespace crypto {

/**
    @brief  SHA-256 のハッシュコンテキストを保持するための構造体です。

    @details
    ハッシュ計算の中間状態であるハッシュコンテキストを保持するための構造体です。

    Sha256Generator::GetContext() で取得したハッシュコンテキストを保存し、Sha256Generator::InitializeWithContext() に与えて初期化することでハッシュ計算の中断と再開を行うことができます。
 */
struct Sha256Context
{
    Bit32  _intermediateHash[8];
    Bit64  _inputBitCount;
};

/**
    @brief  SHA-256 ハッシュを計算するクラスです。

    @details
    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 処理可能状態
    - 完了状態

 */
class Sha256Generator
{
    NN_DISALLOW_COPY(Sha256Generator);

public:
    static const size_t HashSize = 32;  //!< ハッシュ値のサイズを表す定数です。
    static const size_t BlockSize = 64; //!< ハッシュ値の計算処理単位のサイズを表す定数です。

    static const size_t Asn1ObjectIdentifierSize = 19; //!< SHA-256 のオブジェクト ID を DER 符号化したバイナリ列のサイズを表す定数です。
    static const uint8_t Asn1ObjectIdentifier[Asn1ObjectIdentifierSize]; //!< SHA-256 のオブジェクト ID を DER 符号化したバイナリ列です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    Sha256Generator() NN_NOEXCEPT {}

    /**
        @brief    ハッシュ計算を初期化します。

        @post
        - インスタンスは処理可能状態になる。

        @details
        ハッシュ値が規定の初期値で初期化され、ハッシュ計算が開始されます。
     */
    void Initialize() NN_NOEXCEPT
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
    void Update(const void* pData, size_t dataSize) NN_NOEXCEPT
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
    void GetHash(void* pHash, size_t hashSize) NN_NOEXCEPT
    {
        m_Impl.GetHash(pHash, hashSize);
    }

    /**
        @brief    ハッシュコンテキストを指定してハッシュ計算を開始します。

        @param[in]  pContext    ハッシュコンテキストへのポインタ。

        @post
        - インスタンスは処理可能状態になる。

        @details
        指定されたハッシュコンテキストの状態から、ハッシュ計算が開始されます。
        ハッシュコンテキストは GetContext() で取得することができます。
     */
    void InitializeWithContext(const Sha256Context* pContext) NN_NOEXCEPT
    {
        m_Impl.InitializeWithContext(pContext);
    }

    /**
        @brief    ハッシュ計算の中間状態であるハッシュコンテキストを取得します。

        @param[out] pContext    ハッシュコンテキストを格納するバッファへのポインタ。

        @return     入力済みデータのうち、ハッシュ計算が行われていないデータのバイト数。

        @pre
        - インスタンスは処理可能状態である。

        @post
        - pContext にハッシュコンテキストが書き込まれる。

        @details
        取得したハッシュコンテキストを InitializeWithContext() で初期化することでハッシュ計算を再開することができます。

        Update() で入力されたデータのうちブロックサイズに満たなかったデータはブロックサイズに達するまで未処理の状態でバッファされます。
        未処理のデータについては計算結果はコンテキストには反映されていないため、この関数の返り値が 0 でない場合、正しく計算を再開するためには以下のどちらかの処理が必要になります。

        -# ハッシュ計算の再開時に返り値のサイズだけ巻き戻した位置からデータを入力する
        -# GetBufferedData() で取得できるデータをコンテキストとあわせて保存しておき、ハッシュ計算の再開後に最初に入力する

     */
    size_t GetContext(Sha256Context* pContext) const NN_NOEXCEPT
    {
        return m_Impl.GetContext(pContext);
    }

    /**
        @brief      入力済みの未処理データのサイズを取得します。

        @return     入力済みの未処理データのバイト数。

        @details
        Update() で入力されたデータのうちブロックサイズに満たなかったデータはブロックサイズに達するまで未処理の状態でバッファされます。

        この関数ではそのデータのサイズを取得できます。
     */
    size_t GetBufferedDataSize() const NN_NOEXCEPT
    {
        return m_Impl.GetBufferedDataSize();
    }

    /**
        @brief      入力済みの未処理データを取得します。

        @param[out] pData    データを取得するバッファへのポインタ。
        @param[in]  dataSize pData が指すバッファのバイトサイズ。

        @pre
        - dataSize >= GetBufferedDataSize()

        @post
        - pData にデータが書き込まれる。

        @details
        Update() で入力されたデータのうちブロックサイズに満たなかったデータはブロックサイズに達するまで未処理の状態でバッファされます。

        この関数ではそのデータを取得できます。

        データの取得に必要になるバッファのサイズは最大で BlockSize - 1 バイトです。
     */
    void GetBufferedData(void* pData, size_t dataSize) const NN_NOEXCEPT
    {
        m_Impl.GetBufferedData(pData, dataSize);
    }

private:
    detail::Sha256Impl m_Impl;
};

//! @name ハッシュ
//! @{

/**
    @brief    SHA-256 を計算するユーティリティ関数です。

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
    複数に分割されたデータ列のハッシュ値を計算したい場合は Sha256Generator クラスを利用してください。
 */
void   GenerateSha256Hash(void* pHash, size_t hashSize, const void* pData,
                          size_t dataSize) NN_NOEXCEPT;

//! @}

}} // namespace nn::crypto
