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

#include <nn/nn_Common.h>
#include <nn/nn_SdkAssert.h>
#include <nn/util/util_Endian.h>

namespace nn { namespace crypto { namespace detail {

template <typename HashFunction>
class RsaOaepPadding
{
public:
    static const size_t HashSize = HashFunction::HashSize;

    void BuildPad(void* outBlock, size_t blockSize,
                  HashFunction* hash,
                  const void* pRawMessage, size_t rawMessageSize,
                  const void* pSeed, size_t seedSize) NN_NOEXCEPT
    {
        uint8_t labelDigest[HashSize];
        hash->GetHash(labelDigest, HashSize);
        BuildPad(outBlock, blockSize, labelDigest, sizeof(labelDigest), pRawMessage, rawMessageSize, pSeed, seedSize);
    }

    void BuildPad(void* outBlock, size_t blockSize,
                  const void* pLabelDigest, size_t labelDigestSize,
                  const void* pRawMessage, size_t rawMessageSize,
                  const void* pSeed, size_t seedSize) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(blockSize >= 2 * HashSize + 2 + rawMessageSize);
        NN_SDK_REQUIRES(seedSize > 0);
        NN_SDK_REQUIRES(seedSize == HashSize);
        NN_SDK_REQUIRES(labelDigestSize == HashSize);
        NN_UNUSED(seedSize);
        NN_UNUSED(labelDigestSize);

        uint8_t* em = static_cast<uint8_t*>(outBlock);
        em[0] = 0x00;
        uint8_t* seed = em + 1;
        std::memcpy(seed, pSeed, HashSize );
        uint8_t* db = seed + HashSize;
        std::memcpy(db, pLabelDigest, HashSize);
        std::memset(db + HashSize, 0, blockSize - 2 * HashSize - 2 - rawMessageSize);

        uint8_t* msg = em + blockSize - rawMessageSize - 1;
        *msg++ = 0x01;
        std::memcpy(msg, pRawMessage, rawMessageSize);

        ApplyMgf1Mask(db, blockSize - (1 + HashSize), seed, HashSize);
        ApplyMgf1Mask(seed, HashSize, db, blockSize - (1 + HashSize));
    }

    size_t RecoverFromPad(void* pOutPlain, size_t outSize, HashFunction* hash, uint8_t* em,
                          size_t blockSize ) NN_NOEXCEPT
    {
        uint8_t labelDigest[HashSize];
        hash->GetHash(labelDigest, HashSize);
        return RecoverFromPad(pOutPlain, outSize, labelDigest, sizeof(labelDigest), em, blockSize);
    }
    size_t RecoverFromPad(void* pOutPlain, size_t outSize, const void* pLabelDigest, size_t labelDigestSize,
                          uint8_t* em, size_t blockSize ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(outSize > 0);
        NN_SDK_REQUIRES(blockSize >= 2 * HashSize + 2 + 1);
        NN_SDK_REQUIRES(labelDigestSize == HashSize);
        NN_UNUSED(labelDigestSize);

        // To avoid timing attacks, make all checks in constant time
        int bad = em[0];   // First byte should be 0
        uint8_t* seed = em + 1;
        uint8_t* db = seed + HashSize;

        ApplyMgf1Mask(seed, HashSize, db, blockSize - (1 + HashSize));
        ApplyMgf1Mask(db, blockSize - (1 + HashSize), seed, HashSize);

        const uint8_t* labelDigest = reinterpret_cast<const uint8_t*>(pLabelDigest);

        for (size_t i = 0; i < HashSize; ++i)
        {
            bad |= db[i] ^ labelDigest[i];  // Hashes should match
        }

        const uint8_t* rest = db + HashSize;
        size_t restSize = blockSize - 2 * HashSize - 1;
        size_t index = 0;
        // Should find 01 marker after optional 00 bytes
        int lookingForIndex = 1;
        int invalid = 0;
        for (size_t i = 0; i < restSize; )
        {
            int is0 = (rest[i] == 0x00);
            int is1 = (rest[i] == 0x01);
            index += (lookingForIndex & is1) * static_cast<int>(++i);
            lookingForIndex &= ~is1;
            invalid |= (lookingForIndex & ~is0);
        }
        if ( bad | invalid | lookingForIndex )
        {
            return 0;
        }
        size_t rawMessageSize = restSize - index;
        if (rawMessageSize > outSize)
        {
            return 0;
        }
        std::memcpy( pOutPlain, rest + index, rawMessageSize);
        return rawMessageSize;
    }

private:
    void ApplyMgf1Mask(uint8_t* pDst, size_t dstSize, const void* pSrc, size_t srcSize) NN_NOEXCEPT
    {
        uint32_t counter = 0;
        while (dstSize > 0)
        {
            size_t roundSize = HashSize;
            if (roundSize > dstSize)
            {
                roundSize = dstSize;
            }
            dstSize -= roundSize;
            uint32_t counterBe;
            nn::util::StoreBigEndian(&counterBe, counter);

            HashFunction hash;
            hash.Initialize();
            hash.Update(pSrc, srcSize);
            hash.Update(&counterBe, 4);

            uint8_t mask[HashSize];
            hash.GetHash(mask, HashSize);
            const uint8_t* pMask = mask;

            while (roundSize-- > 0)
            {
                *pDst++ ^= *pMask++;
            }
            ++counter;
        }
    }

    void ComputeEncodedMessageHash(void* outHash,
                                   HashFunction* hash,
                                   const void* pSalt,
                                   size_t saltSize ) NN_NOEXCEPT
    {
        uint8_t mPrime[8 + HashSize];
        std::memset(mPrime, 0, 8);
        hash->GetHash(mPrime + 8, HashSize);

        hash->Initialize();
        hash->Update(mPrime, sizeof(mPrime));
        hash->Update(pSalt, saltSize);
        hash->GetHash(outHash, HashSize);
    }
};

}}} // namespace nn::crypto::detail
