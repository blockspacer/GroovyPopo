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

#include <algorithm>
#include <nn/g3d/g3d_Configuration.h>

#define NN_G3D_FLAG_DECLARE(mbit, mname) \
    FLAG_##mname##_SHIFT = (mbit), \
    FLAG_##mname = 0x1 << (mbit)

#define NN_G3D_FLAG_VALUE_DECLARE(mbit, msize, mname) \
    FLAG_##mname##_VALUE_SHIFT = (mbit), \
    FLAG_##mname##_VALUE_MASK = (((0x1ul << (msize)) - 1) << (mbit))

#define NN_G3D_GET_FLAG_VALUE(mflags, mname, mtype) \
    GetFlagValue<mtype>((mflags), (FLAG_##mname##_VALUE_SHIFT), (FLAG_##mname##_VALUE_MASK))

#define NN_G3D_SET_FLAG_VALUE(mflags, mname, mval) \
    ((mflags) = SetFlagValue( \
        (mflags), (FLAG_##mname##_VALUE_SHIFT), (FLAG_##mname##_VALUE_MASK), (mval)))

namespace nn { namespace g3d { namespace detail {

//----------------------------------------
//! @name フラグ操作関連
//@{

//---------------------------------------------------------------------------
//! @brief        ビットフィールドの指定のマスクのビットが全て真かチェックします。
//!
//! @tparam       flags   現在のビットフィールドです。
//! @tparam       mask    チェックに用いるマスクです。
//---------------------------------------------------------------------------
template<typename TFlags, typename TMask, typename TCheck>
inline
bool CheckFlag(TFlags flags, TMask mask, TCheck check) NN_NOEXCEPT
{
    return (flags & static_cast<TFlags>(mask)) == static_cast<TFlags>(check);
}
//---------------------------------------------------------------------------
//! @brief        ビットフィールドの指定のマスクのビットが全て真かチェックします。
//!
//! @tparam       flags   現在のビットフィールドです。
//! @tparam       mask    チェックに用いるマスクです。
//---------------------------------------------------------------------------
template<typename TFlags, typename TMask>
inline
bool CheckFlag(TFlags flags, TMask mask) NN_NOEXCEPT
{
    return CheckFlag(flags, mask, mask);
}

//---------------------------------------------------------------------------
//! @brief        ビットフィールドの指定のマスクのビット値を作成します。
//!
//! @param[in]   shift   作成に用いるビットシフトです。
//! @param[in]   mask    作成に用いるマスクです。
//! @param[in]   value   設定したいビット値です。
//!
//! @return 指定のマスクのビット値を返します。
//---------------------------------------------------------------------------
template<typename TFlags, typename TMask, typename TValue>
inline
TFlags CreateFlagValue(TValue value, int shift, TMask mask) NN_NOEXCEPT
{
    return (value << shift) & static_cast<TFlags>(mask);
}

//---------------------------------------------------------------------------
//! @brief        ビットフィールドの指定のマスクのビット値を取得します。
//!
//! @param[in]   flags   現在のビットフィールドです。
//! @param[in]   shift   取得に用いるビットシフトです。
//! @param[in]   mask    取得に用いるマスクです。
//!
//! @return 指定のマスクのビット値を返します。
//---------------------------------------------------------------------------
template<typename TResult, typename TFlags, typename TMask>
inline
TResult GetFlagValue(TFlags flags, int shift, TMask mask) NN_NOEXCEPT
{
    return static_cast<TResult>((flags & static_cast<TFlags>(mask)) >> shift);
}

//---------------------------------------------------------------------------
//! @brief        ビットフィールドの指定のマスクのビット値を設定します。
//!
//! @param[in]  flags   現在のビットフィールドです。
//! @param[in]  shift   設定に用いるビットシフトです。
//! @param[in]  mask    設定に用いるマスクです。
//! @param[in]  value   設定したいビット値です。
//!
//! @return ビット値をセットしたフラグの値を返します。
//---------------------------------------------------------------------------
template<typename TFlags, typename TMask, typename TValue>
inline
TFlags SetFlagValue(TFlags flags, int shift, TMask mask, TValue value) NN_NOEXCEPT
{
    return (flags & ~(static_cast<TFlags>(mask))) | ((value << shift) & static_cast<TFlags>(mask));
}

template<typename TFlags, typename TMask>
inline
TFlags InvertFlag(TFlags flags, TMask mask) NN_NOEXCEPT
{
    return (flags & ~(static_cast<TFlags>(mask))) | ((~flags) & static_cast<TFlags>(mask));
}

template<typename TValue>
inline
void FillBits(Bit32* pBitArray, int count, TValue value) NN_NOEXCEPT
{
    NN_SDK_REQUIRES(count == 0 || pBitArray);
    NN_SDK_REQUIRES((static_cast<Bit32>(value) & (~Bit32(0x1))) == 0);
    int numWord = ((count + 0x1F) >> 5);
    if (value)
    {
        for (int idxWord = 0; idxWord < numWord; ++idxWord)
        {
            pBitArray[idxWord] = ~Bit32(0);
        }
    }
    else
    {
        for (int idxWord = 0; idxWord < numWord; ++idxWord)
        {
            pBitArray[idxWord] = 0;
        }
    }
}

template<typename TResult>
inline
TResult IsBitOn(const Bit32* pBitArray, int wordIndex, int bitIndex) NN_NOEXCEPT
{
    NN_SDK_REQUIRES((bitIndex & ~0x1F) == 0);
    return static_cast<TResult>((pBitArray[wordIndex] >> bitIndex) & 0x1u);
}

template<typename TResult>
inline
TResult IsBitOn(const Bit32* pBitArray, int index) NN_NOEXCEPT
{
    return IsBitOn<TResult>(pBitArray, index >> 5, index & 0x1F);
}

template<typename TValue>
inline
void SetBit(Bit32* pBitArray, int wordIndex, int bitIndex, TValue value) NN_NOEXCEPT
{
    NN_SDK_REQUIRES((bitIndex & ~0x1F) == 0);
    NN_SDK_REQUIRES((static_cast<Bit32>(value) & (~Bit32(0x1))) == 0);
    Bit32& word = pBitArray[wordIndex];
    word &= ~(0x1u << bitIndex);
    word |= static_cast<Bit32>(value) << bitIndex;
}

template<typename TValue>
inline
void SetBit(Bit32* pBitArray, int index, TValue value) NN_NOEXCEPT
{
    SetBit(pBitArray, index >> 5, index & 0x1F, value);
}

//--------------------------------------------------------------------------------------------------

class FlagSet
{
public:
    FlagSet() NN_NOEXCEPT :
        m_pMainFlagArray(NULL), m_pSubFlagArray(NULL),
        m_FlagSetCount(0), m_Flag32Count(0), m_FlagCount(0), m_MainFlag(0), m_FlagPerSet(0)
    {
    }

    static size_t CalcBufferSize(int flagCount, int flagSetCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(flagSetCount, 1, 9);

        if (flagSetCount > 1)
        {
            return (nn::util::align_up(flagCount, 32) >> 3) * (flagSetCount + 1); // 1 パラメータあたり 1 ビット。
        }
        else
        {
            return nn::util::align_up(flagCount, 32) >> 3;
        }
    }

    void Initialize(int flagCount, int flagSetCount, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;
    void Caclulate() NN_NOEXCEPT;
    void Set(int bitIndex) NN_NOEXCEPT
    {
        m_pMainFlagArray[bitIndex >> 5] |= 0x1 << (bitIndex & 0x1F);
        m_MainFlag |= Flag_Dirty;
    }

    void Clear() NN_NOEXCEPT
    {
        if (m_Flag32Count > 0)
        {
            if (m_FlagSetCount > 1)
            {
                std::fill_n(m_pMainFlagArray, m_Flag32Count * (m_FlagSetCount + 1), 0);
            }
            else
            {
                std::fill_n(m_pMainFlagArray, m_Flag32Count, 0);
            }
        }
        m_MainFlag &= ~Flag_Dirty;
    }

    void ClearMainFlag() NN_NOEXCEPT
    {
        if (m_Flag32Count > 0)
        {
            std::fill_n(m_pMainFlagArray, m_Flag32Count, 0);
        }
        m_MainFlag &= ~Flag_Dirty;
    }

    void ClearAllSubFlag() NN_NOEXCEPT
    {
        if (m_FlagSetCount > 1)
        {
            for (int index = 0; index < m_FlagSetCount; ++index)
            {
                ClearSubFlag(index);
            }
        }
    }

    void ClearSubFlag(int index) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, m_FlagSetCount);
        if (m_Flag32Count > 0)
        {
            std::fill_n(m_pSubFlagArray + (m_Flag32Count * index), m_Flag32Count, 0);
        }
    }

    Bit32* GetFlagSet(int index) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, m_FlagSetCount);

        int bufferBit = 0x1 << index;
        m_FlagPerSet &= ~bufferBit;
        // m_FlagSetCount == 1のときはm_pSubDirtyFlagArray == m_pMainDirtyFlagArray
        return &m_pSubFlagArray[m_Flag32Count * index];
    }

    bool IsFlagDirty() const NN_NOEXCEPT
    {
        return (m_MainFlag & Flag_Dirty);
    }

    bool IsSubSetDirty(int index) const NN_NOEXCEPT
    {
        int bufferBit = 0x1 << index;
        return (m_FlagPerSet & bufferBit) != 0;
    }

    bool IsFlagDirty(int bitIndex) const NN_NOEXCEPT
    {
        return IsBitOn<bool>(m_pMainFlagArray, bitIndex);
    }

private:
    enum Flag
    {
        Flag_Dirty        = 0x1//!< @brief ビットが変更されたされたことを表すフラグです。
    };

    Bit32* m_pMainFlagArray;
    Bit32* m_pSubFlagArray;
    int    m_FlagSetCount;
    int    m_Flag32Count;
    int    m_FlagCount;
    Bit8   m_MainFlag;
    Bit8   m_FlagPerSet;
    NN_PADDING2;
};

//@}

}}} // namespace nn::g3d::detail

