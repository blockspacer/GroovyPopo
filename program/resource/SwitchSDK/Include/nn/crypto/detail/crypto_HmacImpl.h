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
#include <cstddef>
#include <nn/nn_Common.h>
#include <nn/nn_SdkAssert.h>
#include <nn/crypto/detail/crypto_Clear.h>

namespace nn { namespace crypto { namespace detail {

template <typename HashFunction>
class HmacImpl
{
public:
    static const size_t MacSize   = HashFunction::HashSize;
    static const size_t BlockSize = HashFunction::BlockSize;

public:
    HmacImpl() NN_NOEXCEPT : m_State(State_None) {};
    ~HmacImpl() NN_NOEXCEPT
    {
        NN_STATIC_ASSERT(offsetof(HmacImpl, m_HashFunction) == 0);

        /* Should not wipe out user-defined HashFunction */
        ClearMemory(reinterpret_cast<uint8_t*>(this) + sizeof(m_HashFunction),
                    sizeof(*this) - sizeof(m_HashFunction));
    }

    void Initialize(const void* pKey, size_t keySize) NN_NOEXCEPT;
    void Update(const void* pData, size_t dataSize) NN_NOEXCEPT;
    void GetMac(void* pMac, size_t macSize) NN_NOEXCEPT;

private:
    enum State
    {
        State_None,
        State_Initialized,
        State_Done,
    };

private:
    HashFunction  m_HashFunction;
    uint32_t      m_Key[BlockSize / sizeof(uint32_t)];
    uint32_t      m_Mac[MacSize / sizeof(uint32_t)];
    State         m_State;

};

template <typename HashFunction>
inline void HmacImpl<HashFunction>::Initialize(const void* pKey, size_t keySize) NN_NOEXCEPT
{
    /* 鍵全体を 0 で初期化 */
    std::memset(m_Key, 0x00, BlockSize);

    /* 鍵がブロック長よりも長い場合、ハッシュ値を鍵とする */
    if (keySize > BlockSize)
    {
        m_HashFunction.Initialize();
        m_HashFunction.Update(pKey, keySize);
        m_HashFunction.GetHash(m_Key, m_HashFunction.HashSize);
    }
    else
    {
        std::memcpy(m_Key, pKey, keySize);
    }

    /* ipad key を計算する */
    for (int i = 0; i < static_cast<int>(BlockSize / sizeof(uint32_t)); i++)
    {
        m_Key[i] ^= 0x36363636;
    }

    /* ipad key をハッシュ計算の最初の入力とする */
    m_HashFunction.Initialize();
    m_HashFunction.Update(m_Key, BlockSize);

    m_State = State_Initialized;
}

template <typename HashFunction>
inline void HmacImpl<HashFunction>::Update(const void* pData, size_t dataSize) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(m_State == State_Initialized, "Invalid state. Please restart from Initialize().");

    m_HashFunction.Update(pData, dataSize);
}

template <typename HashFunction>
inline void HmacImpl<HashFunction>::GetMac(void* pMac, size_t macSize) NN_NOEXCEPT
{
    NN_SDK_REQUIRES((m_State == State_Initialized) || (m_State == State_Done), "Invalid state. Please restart from Initialize().");
    NN_SDK_REQUIRES(macSize >= MacSize);
    NN_UNUSED(macSize);

    if (m_State == State_Initialized)
    {
        /* 中間ハッシュ値を取得 */
        m_HashFunction.GetHash(m_Mac, MacSize);

        /* opad key を計算する。opad は 0x5c なので 0x36^0x5c = 0x6a を XOR する */
        for (int i = 0; i < static_cast<int>(BlockSize / sizeof(uint32_t)); i++)
        {
            m_Key[i] ^= 0x6a6a6a6a;
        }

        /* opad key と中間のハッシュ値を結合したメッセージのハッシュ値が MAC になる */
        m_HashFunction.Initialize();
        m_HashFunction.Update(m_Key, BlockSize);
        m_HashFunction.Update(m_Mac, MacSize);
        m_HashFunction.GetHash(m_Mac, MacSize);

        m_State = State_Done;
    }

    std::memcpy(pMac, m_Mac, MacSize);
}

}}} // namespace nn::crypto::detail
