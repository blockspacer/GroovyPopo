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
#include <nn/util/util_Endian.h>

/*
For reference:
- http://www.emc.com/emc-plus/rsa-labs/historical/raising-standard-rsa-signatures-rsa-pss.htm
- ftp://ftp.rfc-editor.org/in-notes/rfc3447.txt
- http://crypto.stackexchange.com/questions/1217/rsa-pss-salt-size
*/

namespace nn { namespace crypto { namespace detail {

template <typename HashFunction>
class RsaPssPadding
{
public:
    static const size_t HashSize = HashFunction::HashSize;

    void BuildPad(void* outBlock, size_t blockSize,
                  HashFunction* hash, const void* pSalt, size_t saltSize) NN_NOEXCEPT
    {
        NN_SDK_ASSERT(saltSize > 0);
        NN_SDK_ASSERT(blockSize >= HashSize + saltSize + 2);
        NN_SDK_ASSERT(blockSize >= HashSize + 2);

        const size_t dbLen = blockSize - HashSize - 1;
        const size_t dbPaddingLen = dbLen - saltSize - 1;
        NN_SDK_ASSERT(dbPaddingLen < blockSize);    // Should be OK from above ASSERTs

        uint8_t* emBuf = static_cast<uint8_t*>(outBlock);
        uint8_t* db = emBuf;
        uint8_t* saltPtr = db + dbPaddingLen + 1;
        uint8_t* hashPtr = db + dbLen;

        std::memset(db, 0, dbPaddingLen);
        db[dbPaddingLen] = 0x01;

        std::memcpy(saltPtr, pSalt, saltSize);

        ComputeEncodedMessageHash(hashPtr, hash, saltPtr, saltSize);
        db[0] = 0x80;

        ApplyMGF1Mask(db, dbLen, hashPtr, HashSize);

        db[0] &= 0x7f; // TODO: zero only required bits
        emBuf[blockSize - 1] = 0xBC;
    }

    bool CheckPad(uint8_t* emBuf, size_t blockSize, HashFunction* hash ) NN_NOEXCEPT
    {
        if (emBuf[blockSize - 1] != 0xBC)
        {
            return false;
        }

        const size_t dbLen = blockSize - HashSize - 1;
        uint8_t* db = emBuf;

        const uint8_t* hashPtr = emBuf + dbLen;

        ApplyMGF1Mask(db, dbLen, hashPtr, HashSize);
        db[0] &= 0x7f;

        int saltStart = -1;
        for (int i = 0; i < static_cast<int>(dbLen); ++i)
        {
            if (db[i] != 0)
            {
                saltStart = i + 1;
                break;
            }
        }

        if (saltStart == -1)
        {
            return false;
        }
        if (db[saltStart - 1] != 0x01)
        {
            return false;
        }
        const uint8_t* saltPtr = emBuf + saltStart;
        size_t saltSize = dbLen - saltStart;

        if (saltSize == 0)
        {
            return false;
        }

        uint8_t hashBuf[HashSize];
        ComputeEncodedMessageHash(hashBuf, hash, saltPtr, saltSize);
        return std::memcmp(hashBuf, hashPtr, HashSize) ? false : true;
    }

private:
    void ApplyMGF1Mask(uint8_t* pOutputData, size_t maskLen,
                       const void* pInputData,
                       size_t dataSize ) NN_NOEXCEPT
    {
        uint8_t hashBuf[HashSize];

        size_t numRounds = (maskLen + HashSize - 1) / HashSize;
        for (size_t i = 0; i < numRounds; ++i)
        {
            HashFunction hash;

            hash.Initialize();
            hash.Update(pInputData, dataSize);

            uint32_t tmp;
            nn::util::StoreBigEndian( &tmp, static_cast<uint32_t>(i) );
            hash.Update(&tmp, 4);
            hash.GetHash(hashBuf, HashSize);

            size_t lowPos = i * HashSize;
            size_t highPos = std::min(maskLen, lowPos + HashSize);
            for (size_t j = lowPos; j < highPos; ++j)
            {
                pOutputData[j] ^= hashBuf[j - lowPos];
            }
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
