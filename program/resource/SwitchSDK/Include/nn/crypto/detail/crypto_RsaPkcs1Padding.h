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

#include <cstring>
#include <algorithm>

#include <nn/nn_Common.h>
#include <nn/nn_SdkAssert.h>

namespace nn { namespace crypto { namespace detail {

template <typename HashFunction>
class RsaPkcs1Padding
{
public:
    static const size_t HashSize = HashFunction::HashSize;

    void BuildPad(void* outBlock, size_t blockSize, HashFunction* hash) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(blockSize >= 2 + 1 + sizeof(HashFunction::Asn1ObjectIdentifier) + HashSize);

        uint8_t* pDst = static_cast<uint8_t*>(outBlock);
        *pDst++ = 0x00;
        *pDst++ = 0x01;
        size_t padLen = blockSize - HashSize - sizeof(HashFunction::Asn1ObjectIdentifier) - 3;
        std::memset( pDst, 0xFF, padLen );
        pDst += padLen;
        *pDst++ = 0x00;
        std::memcpy( pDst, HashFunction::Asn1ObjectIdentifier, sizeof(HashFunction::Asn1ObjectIdentifier) );
        pDst += sizeof(HashFunction::Asn1ObjectIdentifier);
        hash->GetHash( pDst, HashSize );
    }

    bool CheckPad(const uint8_t* pSrc, size_t blockSize, HashFunction* hash ) NN_NOEXCEPT
    {
        if (blockSize < sizeof(HashFunction::Asn1ObjectIdentifier) + 11)
        {
            return false;
        }

        if (*pSrc++ != 0x00)
        {
            return false;
        }

        if (*pSrc++ != 0x01)
        {
            return false;
        }

        size_t padLen = blockSize - HashSize - sizeof(HashFunction::Asn1ObjectIdentifier) - 3;
        for (size_t i = 0; i < padLen; i++)
        {
            if (*pSrc++ != 0xFF)
            {
                return false;
            }
        }

        if (*pSrc++ != 0x00)
        {
            return false;
        }

        if ( std::memcmp( pSrc, HashFunction::Asn1ObjectIdentifier,
                          sizeof(HashFunction::Asn1ObjectIdentifier) ) != 0 )
        {
            return false;
        }
        pSrc += sizeof(HashFunction::Asn1ObjectIdentifier);

        char digest[HashSize];
        hash->GetHash( digest, sizeof(digest) );

        return std::memcmp(digest, pSrc, HashSize) == 0;
    }
};

}}} // namespace nn::crypto::detail
