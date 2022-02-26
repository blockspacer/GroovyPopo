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

namespace nn { namespace crypto {

namespace detail {

class GhashImpl
{
public:
    static const size_t BlockSize = 16;

public:
    struct Block128
    {
        Bit64 hi;
        Bit64 lo;

        inline void Zero() NN_NOEXCEPT {hi = lo = 0;}
        inline void Shift1() NN_NOEXCEPT;
        inline void MakeBigEndian() NN_NOEXCEPT;
    };

    union Block
    {
        Block128 as128;
        Bit32    as32[4];
        Bit8     as8[16];
    };

public:
    GhashImpl() NN_NOEXCEPT : m_BufferedByte(0), m_State(State_None) {}
    ~GhashImpl() NN_NOEXCEPT;

    template <typename BlockCipher>
    void   Initialize(const BlockCipher* pBlockCipher) NN_NOEXCEPT
    {
        // 全ゼロのブロックを暗号化して hashkey を初期化
        m_Tmp.as128.Zero();
        pBlockCipher->EncryptBlock(&m_Tmp.as128, BlockSize, &m_Tmp.as128, BlockSize);

        InitializeHashKey(&m_Tmp);

        m_State = State_Initialized;
    }

    void   Reset() NN_NOEXCEPT;
    void   Update(const void* pData, size_t dataSize) NN_NOEXCEPT;

    template <typename BlockCipher>
    void   ProcessBlocks(const uint8_t* pData8, int numBlocks) NN_NOEXCEPT
    {
        ProcessBlocksGeneric(pData8, numBlocks);
    }

    size_t GetBufferedDataSize() const NN_NOEXCEPT
    {
        return m_BufferedByte;
    }

    size_t GetBlockSize() const NN_NOEXCEPT
    {
        return BlockSize;
    }

    uint64_t GetMessageSize() const NN_NOEXCEPT
    {
        return m_MessageSizeLo;
    }

    void   ProcessPadding() NN_NOEXCEPT;
    void   SwitchMessageSizeDigit() NN_NOEXCEPT;
    void   GetGhash(void* pHash, size_t hashSize) NN_NOEXCEPT;

    void   ProcessPartialData(const uint8_t* pData8, size_t dataSize) NN_NOEXCEPT;
    void   ProcessRemainingData(const uint8_t* pData8, size_t dataSize) NN_NOEXCEPT;
    void   ProcessBlocksGeneric(const uint8_t* pData8, int numBlocks) NN_NOEXCEPT;
    void   InitializeHashKey(Block* encryptedZeroBlock) NN_NOEXCEPT;
    void   GfMultH(Block* pGhash) NN_NOEXCEPT;

private:
    enum State
    {
        State_None,
        State_Initialized,
        State_ProcessingData,
        State_Done,
    };

private:
    Block       m_Ghash;
    Block       m_Tmp;
    Block       m_MultH[16];
    uint64_t    m_MessageSizeHi;
    uint64_t    m_MessageSizeLo;
    size_t      m_BufferedByte;
    State       m_State;
};

}}} // namespace nn::crypto::detail
