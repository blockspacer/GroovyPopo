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
    @brief 暗号機能 に関する API の宣言
 */

/**
    @namespace  nn::crypto
    @brief      暗号機能ライブラリの名前空間です。

    @details
    crypto ライブラリでは以下のクラスが提供されます。
    各クラスの説明はリンク先を参照してください。

    ### ブロック暗号

    - (AES) AesEncryptor / AesDecryptor

    ### 暗号利用モード(ブロック暗号を引数とするテンプレート)

    - (CBC モード) CbcEncryptor / CbcDecryptor
    - (CTR モード) CtrEncryptor / CtrDecryptor
    - (CCM モード) CcmEncryptor / CcmDecryptor
    - (GCM モード) GcmEncryptor / GcmDecryptor
    - (CMAC モード) CmacGenerator

    ### 暗号利用モード(AES 128bit 鍵)

    - (AES-128-CBC) Aes128CbcEncryptor / Aes128CbcDecryptor
    - (AES-128-CTR) Aes128CtrEncryptor / Aes128CtrDecryptor
    - (AES-128-CCM) Aes128CcmEncryptor / Aes128CcmDecryptor
    - (AES-128-GCM) Aes128GcmEncryptor / Aes128GcmDecryptor
    - (AES-128-CMAC) Aes128CmacGenerator

    ### ハッシュ関数

    - (SHA-1) Sha1Generator
    - (SHA-256) Sha256Generator
    - (MD5) Md5Generator

    ###  MAC

    - (HMAC) HmacGenerator
    - (CMAC) CmacGenerator

    ### RSA 暗号プリミティブ

    - RsaCalculator

    ### RSA 暗号

    - (RSA-OAEP) RsaOaepEncryptor / RsaOaepDecryptor

    ### RSA 署名

    - (RSA-PKCS1) RsaPkcs1Signer / RsaPkcs1Verifier
    - (RSA-PSS) RsaPssSigner / RsaPssVerifier

 */
#include <nn/crypto/crypto_CbcEncryptor.h>
#include <nn/crypto/crypto_CbcDecryptor.h>
#include <nn/crypto/crypto_CcmEncryptor.h>
#include <nn/crypto/crypto_CcmDecryptor.h>
#include <nn/crypto/crypto_CtrEncryptor.h>
#include <nn/crypto/crypto_CtrDecryptor.h>
#include <nn/crypto/crypto_GcmEncryptor.h>
#include <nn/crypto/crypto_GcmDecryptor.h>
#include <nn/crypto/crypto_CmacGenerator.h>

#include <nn/crypto/crypto_AesEncryptor.h>
#include <nn/crypto/crypto_AesDecryptor.h>
#include <nn/crypto/crypto_Aes128CbcEncryptor.h>
#include <nn/crypto/crypto_Aes128CbcDecryptor.h>
#include <nn/crypto/crypto_Aes128CcmEncryptor.h>
#include <nn/crypto/crypto_Aes128CcmDecryptor.h>
#include <nn/crypto/crypto_Aes128CtrEncryptor.h>
#include <nn/crypto/crypto_Aes128CtrDecryptor.h>
#include <nn/crypto/crypto_Aes128GcmEncryptor.h>
#include <nn/crypto/crypto_Aes128GcmDecryptor.h>
#include <nn/crypto/crypto_Aes128CmacGenerator.h>

#include <nn/crypto/crypto_Sha1Generator.h>
#include <nn/crypto/crypto_Sha256Generator.h>
#include <nn/crypto/crypto_Md5Generator.h>

#include <nn/crypto/crypto_HmacGenerator.h>
#include <nn/crypto/crypto_HmacSha1Generator.h>
#include <nn/crypto/crypto_HmacSha256Generator.h>

#include <nn/crypto/crypto_RsaCalculator.h>
#include <nn/crypto/crypto_RsaOaepEncryptor.h>
#include <nn/crypto/crypto_RsaOaepDecryptor.h>
#include <nn/crypto/crypto_RsaOaepSha256Encryptor.h>
#include <nn/crypto/crypto_RsaOaepSha256Decryptor.h>
#include <nn/crypto/crypto_RsaPkcs1Signer.h>
#include <nn/crypto/crypto_RsaPkcs1Verifier.h>
#include <nn/crypto/crypto_RsaPkcs1Sha256Signer.h>
#include <nn/crypto/crypto_RsaPkcs1Sha256Verifier.h>
#include <nn/crypto/crypto_RsaPssSigner.h>
#include <nn/crypto/crypto_RsaPssVerifier.h>
#include <nn/crypto/crypto_RsaPssSha256Signer.h>
#include <nn/crypto/crypto_RsaPssSha256Verifier.h>

#include <nn/crypto/crypto_Csrng.h>
