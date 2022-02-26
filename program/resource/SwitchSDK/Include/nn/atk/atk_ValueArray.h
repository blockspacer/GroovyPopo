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
#include <nn/nn_SdkAssert.h>
#include <algorithm>

namespace nn { namespace atk { namespace detail {

template <class T>
class ValueArray
{
public:
    static size_t GetRequiredMemSize(int count) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_GREATER_EQUAL(count, 0);
        return sizeof(T) * count;
    }

    ValueArray() NN_NOEXCEPT : m_pValue( nullptr ), m_Count( 0 )
    {
    }

    bool Initialize(void* buffer, size_t bufferSize, int count) NN_NOEXCEPT
    {
        NN_UNUSED(bufferSize);
        NN_SDK_ASSERT_NOT_NULL(buffer);
        NN_SDK_ASSERT_GREATER_EQUAL(count, 0);
        NN_SDK_ASSERT_GREATER_EQUAL(bufferSize, GetRequiredMemSize(count));
        NN_SDK_ASSERT_ALIGNED(buffer, NN_ALIGNOF(T));
        m_pValue = static_cast<T*>(buffer);
        m_Count = count;

        Reset();

        return true;
    }

    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_pValue != nullptr;
    }

    void* GetBufferAddr() NN_NOEXCEPT
    {
        return m_pValue;
    }

    void Finalize() NN_NOEXCEPT
    {
        m_pValue = nullptr;
        m_Count = 0;
    }

    void Reset() NN_NOEXCEPT
    {
        for(int i = 0; i < m_Count; ++i)
        {
            m_pValue[i] = 0;
        }

    }

    int GetCount() const NN_NOEXCEPT
    {
        return m_Count;
    }

    void SetValue(int index, T value) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE(index, 0, m_Count);
        m_pValue[index] = value;
    }

    T GetValue(int index) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE(index, 0, m_Count);
        return m_pValue[index];
    }

    bool TrySetValue(int index, T value) NN_NOEXCEPT
    {
        if(index < 0 || index >= m_Count)
        {
            return false;
        }
        m_pValue[index] = value;
        return true;
    }

    T TryGetValue(int index) const NN_NOEXCEPT
    {
        if(index < 0 || index >= m_Count)
        {
            return 0;
        }
        return m_pValue[index];
    }

    ValueArray<T>& operator=(const ValueArray<T>& rhs) NN_NOEXCEPT
    {
        const int CopyCount = std::min(m_Count, rhs.m_Count);
        for (int i = 0; i < CopyCount; ++i)
        {
            SetValue(i, rhs.GetValue(i));
        }
        if (CopyCount < m_Count)
        {
            for (int i = CopyCount; i < m_Count; ++i)
            {
                SetValue(i, 0);
            }
        }

        return *this;
    }

private:
    T* m_pValue;
    int m_Count;
};

}}}
