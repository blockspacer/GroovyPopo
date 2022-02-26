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

#include <nn/nn_Common.h>

namespace nn { namespace crypto { namespace detail {

template <size_t KeySize>
class AesImpl
{
public:
    static const size_t BlockSize = 16;
    static const int    RoundCount = (KeySize / 4) + 6;
    static const size_t RoundKeySize = BlockSize * (RoundCount + 1);

public:
    ~AesImpl() NN_NOEXCEPT;
    void Initialize(const void* pKey, size_t keySize, bool isEncryptionKey) NN_NOEXCEPT;
    void EncryptBlock(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) const NN_NOEXCEPT;
    void DecryptBlock(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) const NN_NOEXCEPT;
    const Bit8* GetRoundKey() const NN_NOEXCEPT
    {
        return reinterpret_cast<const Bit8*>(m_RoundKey);
    }

private:
    Bit32 m_RoundKey[ RoundKeySize / sizeof(Bit32) ];
};

}}} // namespace nn::crypto::detail

