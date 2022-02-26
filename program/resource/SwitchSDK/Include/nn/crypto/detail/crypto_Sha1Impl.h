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

class Sha1Impl
{
public:
    static const size_t HashSize  = 20;
    static const size_t BlockSize = 64;

public:
    Sha1Impl() NN_NOEXCEPT : m_State(State_None) {}
    ~Sha1Impl() NN_NOEXCEPT;

    void   Initialize() NN_NOEXCEPT;
    void   Update(const void* pData, size_t dataSize) NN_NOEXCEPT;
    void   GetHash(void* pHash, size_t hashSize) NN_NOEXCEPT;

private:
    void   ProcessBlock(const void* pData) NN_NOEXCEPT;
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

