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

namespace nn { namespace crypto {

struct Sha256Context;

namespace detail {

class Sha256Impl
{
public:
    static const size_t HashSize  = 32;
    static const size_t BlockSize = 64;

public:
    Sha256Impl() NN_NOEXCEPT : m_State(State_None) {}
    ~Sha256Impl() NN_NOEXCEPT;

    void   Initialize() NN_NOEXCEPT;
    void   Update(const void* pData, size_t dataSize) NN_NOEXCEPT;
    void   GetHash(void* pHash, size_t hashSize) NN_NOEXCEPT;

    void   InitializeWithContext(const Sha256Context* pContext) NN_NOEXCEPT;
    size_t GetContext(Sha256Context* pContext) const NN_NOEXCEPT;

    size_t GetBufferedDataSize() const NN_NOEXCEPT { return m_BufferedByte; }

    void   GetBufferedData(void* pData, size_t dataSize) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(dataSize >= GetBufferedDataSize());
        NN_UNUSED(dataSize);

        std::memcpy(pData, m_TemporalBlockBuffer, m_BufferedByte);
    }

private:
    void   ProcessBlock(const void* pData) NN_NOEXCEPT;
    void   ProcessBlocks(const uint8_t* pData, size_t blockCount) NN_NOEXCEPT;
    void   ProcessLastBlock() NN_NOEXCEPT;

private:
    enum State
    {
        State_None,
        State_Initialized,
        State_Done,
    };

private:
    Bit32  m_IntermediateHash[HashSize / sizeof(Bit32)];
    Bit8   m_TemporalBlockBuffer[BlockSize];
    size_t m_BufferedByte;
    Bit64  m_InputBitCount;
    State  m_State;
};

}}} // namespace nn::crypto::detail

