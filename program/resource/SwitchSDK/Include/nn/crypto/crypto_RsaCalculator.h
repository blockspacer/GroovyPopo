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
    @brief RSA 暗号プリミティブの計算を行うための API の宣言
 */

#include <algorithm>
#include <nn/nn_Common.h>
#include <nn/nn_SdkAssert.h>
#include <nn/crypto/detail/crypto_BigNumOnStack.h>

namespace nn { namespace crypto {

/**
    @brief      RSA 暗号プリミティブの機能を実装するクラスです。

    @tparam     ModulusSize   使用する RSA の法のバイトサイズ
    @tparam     ExponentSize  使用する RSA の指数のバイトサイズ

    @details
    このクラスでは RSA 暗号プリミティブの機能である大きな法と指数に対するべき剰余の計算機能が提供されます。
    実際に暗号化や署名で利用する際にはメッセージのパディングが考慮されたアルゴリズムと組み合わせて使用してください。

    このクラスは RSA の法と指数のサイズをテンプレート引数とするテンプレートクラスです。
    RSA の法と指数は 1 バイト (8 ビット) 単位で最大 512 バイト(4096 ビット) まで指定できます。

    このクラスは以下の状態を持ちます。
    インスタンスの生成直後は未初期化状態です。
    - 未初期化状態
    - 処理可能状態

 */
template <size_t ModulusSize, size_t ExponentSize>
class RsaCalculator
{
    NN_DISALLOW_COPY(RsaCalculator);

public:
    static const size_t RequiredWorkBufferSize = ModulusSize * 16; //!< ワークバッファとして必要になるサイズです。

public:
    /**
        @brief    デフォルトコンストラクタです。

        @details
        生成されるインスタンスは未初期化状態です。
     */
    RsaCalculator() NN_NOEXCEPT {}

    /**
        @brief    デストラクタです。

        @details
        コピーした指数データをクリアします。
     */
    ~RsaCalculator() NN_NOEXCEPT
    {
        m_Exponent.Cleanse();
    }

    /**
        @brief    RSA のべき剰余の計算を初期化します。

        @param[in]  pModulus     RSA の法データへのポインタ。
        @param[in]  modulusSize  RSA の法データのバイトサイズ。
        @param[in]  pExponent    RSA の指数データへのポインタ。
        @param[in]  exponentSize RSA の指数データのバイトサイズ。

        @return     初期化に成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられています。

        @pre
        - modulusSize <= ModulusSize
        - exponentSize <= ExponentSize

        @post
        - インスタンスは処理可能状態になる。

        @details
        パラメータを指定し、RSA のべき剰余の計算を初期化します。

        パラメータは内部にコピーされます。
     */
    bool Initialize( const void* pModulus, size_t modulusSize, const void* pExponent,
                     size_t exponentSize ) NN_NOEXCEPT
    {
        if ((m_Modulus.Set(pModulus, modulusSize) == false) || m_Modulus.IsZero() )
        {
            return false;
        }
        // Will assert if exp (e or d) stripped of leading 0's exceeds the allocated size
        if ((m_Exponent.Set(pExponent, exponentSize) == false) || m_Exponent.IsZero() )
        {
            return false;
        }
        return true;
    }

    /**
        @brief    RSA のべき剰余計算を行います。作業領域としてスタックを利用します。

        @param[out] pOutBlock     計算結果を格納するバッファへのポインタ。
        @param[in]  outBlockSize  pOutBlockSize が指すバッファのバイトサイズ。
        @param[in]  pInBlock      計算対象のデータへのポインタ。
        @param[in]  inBlockSize   計算対象のデータのバイトサイズ。

        @return     計算に成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられています。

        @pre
        - インスタンスは処理可能状態である。
        - outBlockSize == inBlockSize

        @post
        - pOutBlock に計算結果が格納される。

        @details
        与えられたデータに対してべき剰余計算を行います。

        ワークバッファとしてスタックから RequiredWorkBufferSize バイトが利用されます。
     */
    bool ModExp( void* pOutBlock, size_t outBlockSize,
                 const void* pInBlock, size_t inBlockSize ) const NN_NOEXCEPT
    {
        uint32_t workBuffer[RequiredWorkBufferSize / sizeof(uint32_t)];

        return ModExp(pOutBlock, outBlockSize, pInBlock, inBlockSize,
                      workBuffer, RequiredWorkBufferSize);
    }

    /**
        @brief    RSA のべき剰余計算を行います。作業領域として与えられたワークバッファを利用します。

        @param[out] pOutBlock      計算結果を格納するバッファへのポインタ。
        @param[in]  outBlockSize   pOutBlockSize が指すバッファのバイトサイズ。
        @param[in]  pInBlock       計算対象のデータへのポインタ。
        @param[in]  inBlockSize    計算対象のデータのバイトサイズ。
        @param[in]  pWorkBuffer    ワークバッファへのポインタ。
        @param[in]  workBufferSize ワークバッファのバイトサイズ。

        @return     計算に成功したかを返します。@n
                    失敗した場合は不正なパラメータが与えられているか、ワークバッファが不足しています。

        @pre
        - インスタンスは処理可能状態である。
        - outBlockSize == inBlockSize
        - workBufferSize >= RequiredWorkBufferSize

        @post
        - pOutBlock に計算結果が格納される。

        @details
        与えられたデータに対してべき剰余計算を行います。

        ワークバッファは RequiredWorkBufferSize バイト以上が必要です。
     */
    bool ModExp( void* pOutBlock, size_t outBlockSize,
                 const void* pInBlock, size_t inBlockSize,
                 void* pWorkBuffer, size_t workBufferSize) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( outBlockSize == inBlockSize );
        NN_SDK_REQUIRES( workBufferSize >= RequiredWorkBufferSize );
        NN_UNUSED( inBlockSize );

        return m_Modulus.ModExp( pOutBlock, pInBlock, m_Exponent, outBlockSize,
                                 static_cast<uint32_t*>(pWorkBuffer), workBufferSize );
    }

private:
    detail::BigNumOnStack<ModulusSize << 3> m_Modulus;
    detail::BigNumOnStack<ExponentSize << 3> m_Exponent;
};

template<size_t T, size_t U> const size_t RsaCalculator<T, U>::RequiredWorkBufferSize;

}} // namespace nn::crypto
