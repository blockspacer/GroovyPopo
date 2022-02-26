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

namespace nn { namespace crypto {

// forward class declaration
template <size_t KeySize> class AesEncryptor;
typedef AesEncryptor<16> AesEncryptor128;


namespace detail {

class CbcMacImpl
{
public:
    static const size_t BlockSize = 16;

public:
    CbcMacImpl() NN_NOEXCEPT : m_BufferedByte(0), m_State(State_None) {}
    ~CbcMacImpl() NN_NOEXCEPT;

    template <typename BlockCipher>
    void   Initialize(const BlockCipher* pBlockCipher) NN_NOEXCEPT
    {
        m_pCipherContext = pBlockCipher;
        m_pCipherFunction = &EncryptBlockCallback<BlockCipher>;
        m_BufferedByte = 0;

        std::memset(m_Mac, 0x00, BlockSize);

        m_State = State_Initialized;
    }

    template <typename BlockCipher>
    void   Update(const void* pData, size_t dataSize) NN_NOEXCEPT
    {
        UpdateGeneric(pData, dataSize);
    }

    template <typename BlockCipher>
    void   ProcessBlocks(const void* pData, int numBlocks) NN_NOEXCEPT
    {
        ProcessBlocksGeneric(pData, numBlocks);
    }

    size_t GetBufferedDataSize() const NN_NOEXCEPT
    {
        return m_BufferedByte;
    }

    size_t GetBlockSize() const NN_NOEXCEPT
    {
        return BlockSize;
    }

    void   UpdateGeneric(const void* pData, size_t dataSize) NN_NOEXCEPT;
    void   ProcessBlocksGeneric(const void* pData, int numBlocks) NN_NOEXCEPT;
    void   ProcessPartialData(const void* pData, size_t dataSize) NN_NOEXCEPT;
    void   ProcessRemainingData(const void* pData, size_t dataSize) NN_NOEXCEPT;

    void   GetMac(void* pMac, size_t macSize) NN_NOEXCEPT;
    void   MaskBufferedData(const void* pData, size_t dataSize) NN_NOEXCEPT;

private:
    void   ProcessBlock(const void* pData) NN_NOEXCEPT;

    template <typename BlockCipher>
    static void EncryptBlockCallback(void* outBlock, const void* inBlock, const void* self) NN_NOEXCEPT
    {
        static_cast<const BlockCipher*>(self)->EncryptBlock(outBlock, BlockCipher::BlockSize,
                                                            inBlock, BlockCipher::BlockSize );
    }

private:
    enum State
    {
        State_None,
        State_Initialized,
        State_Done
    };

private:
    uint8_t     m_Mac[BlockSize];
    uint8_t     m_TemporalBlockBuffer[BlockSize];
    size_t      m_BufferedByte;
    const void* m_pCipherContext;
    void     (* m_pCipherFunction)(void* outBlock, const void* inBlock, const void* cipherContext);
    State       m_State;
};

// template specialization
template <>
void CbcMacImpl::Update<AesEncryptor128>(const void* pData, size_t dataSize) NN_NOEXCEPT;

}}} // namespace nn::crypto::detail
