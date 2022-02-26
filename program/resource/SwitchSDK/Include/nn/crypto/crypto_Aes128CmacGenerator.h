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
    @brief 鍵サイズが 128 bit の AES による CMAC 計算を行う API の宣言
 */

#include <nn/nn_SdkAssert.h>
#include <nn/crypto/crypto_AesEncryptor.h>
#include <nn/crypto/crypto_CmacGenerator.h>

namespace nn { namespace crypto {

/**
    @brief      鍵サイズが 128bit の AES による CMAC を計算するためのクラスです。

    @details
    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 処理可能状態
    - 完了状態

 */
class Aes128CmacGenerator
{
    NN_DISALLOW_COPY(Aes128CmacGenerator);

public:
    static const size_t MacSize = AesEncryptor128::BlockSize; //!< MAC のサイズを表す定数です。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    Aes128CmacGenerator() NN_NOEXCEPT {}

    /**
        @brief      鍵サイズが 128 bit の AES による CMAC の計算を初期化します。

        @param[in]  pKey     鍵データへのポインタ。
        @param[in]  keySize  鍵データのバイトサイズ。

        @pre
        - keySize == 16

        @post
        - インスタンスは処理可能状態になる。

        @details
        鍵データを入力し、MAC 計算を初期化します。
        鍵データは内部にコピーされます。
     */
    void Initialize(const void* pKey, size_t keySize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(keySize == 16);

        m_Aes.Initialize(pKey, keySize);
        m_CmacGenerator.Initialize(&m_Aes);
    }

    /**
        @brief     MAC を与えられたデータで更新します。

        @copydetails CmacGenerator::Update
     */
    void Update(const void* pData, size_t dataSize) NN_NOEXCEPT
    {
        m_CmacGenerator.Update(pData, dataSize);
    }

    /**
        @brief    MAC 計算を完了し、最終的な MAC を取得します。

        @copydetails CmacGenerator::GetMac
     */
    void GetMac(void* pMac, size_t macSize) NN_NOEXCEPT
    {
        m_CmacGenerator.GetMac(pMac, macSize);
    }

private:
    AesEncryptor128                m_Aes;
    CmacGenerator<AesEncryptor128> m_CmacGenerator;
};

//! @name MAC
//! @{

/**
    @brief    鍵サイズが 128 bit の AES-CMAC 計算を行うユーティリティ関数です。

    @param[out] pMac     MAC を格納するバッファへのポインタ。
    @param[in]  macSize  pMac が指すバッファのバイトサイズ。
    @param[in]  pKey     鍵データへのポインタ。
    @param[in]  keySize  鍵データのバイトサイズ。
    @param[in]  pData    入力データへのポインタ。
    @param[in]  dataSize 入力データのバイトサイズ。

    @pre
    - keySize == 16
    - macSize >= 16

    @post
    - pMac に MAC が書き込まれる。

    @details
    与えられたデータ列の AES-CMAC を計算します。
 */
inline void GenerateAes128Cmac(void* pMac, size_t macSize,
                               const void* pData, size_t dataSize,
                               const void* pKey, size_t keySize) NN_NOEXCEPT
{
    Aes128CmacGenerator aes128CmacGenerator;

    aes128CmacGenerator.Initialize(pKey, keySize);
    aes128CmacGenerator.Update(pData, dataSize);
    aes128CmacGenerator.GetMac(pMac, macSize);
}

//! @}

}} // namespace nn::crypto

