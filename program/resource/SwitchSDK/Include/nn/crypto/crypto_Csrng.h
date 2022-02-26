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
    @brief 暗号用途の乱数生成 API の宣言
 */

#include <nn/nn_Common.h>
#include <nn/crypto/detail/crypto_CtrModeImpl.h>

namespace nn { namespace crypto {

    //! @name 乱数生成
    //! @{

    /**
        @brief      暗号用途の乱数を生成します。

        @param[out] pBuffer    生成された乱数列を格納するバッファへのポインタ。
        @param[in]  bufferSize 生成する乱数列のバイトサイズ。

        @post
        - pBuffer に bufferSize ぶんの乱数が書き込まれる。

        @details
        この関数は以下の理由により暗号用途ではない乱数生成には向きません。 暗号用途の場合にのみ使用すべきです。

        @li 遅い
        @li 乱数の再現ができない

        暗号用途ではない乱数生成には C++ 標準ライブラリや @ref nn::util::TinyMt, @ref nn::os::GenerateRandomBytes を使用してください。
     */
    void GenerateCryptographicallyRandomBytes(void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    //! @}

}} // namespace nn::crypto

