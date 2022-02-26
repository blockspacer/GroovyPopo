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

#include <nn/atk/atk_Config.h>
#ifdef NN_ATK_CONFIG_ENABLE_DEV

#include <nn/atk/fnd/basis/atkfnd_Memory.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

//---------------------------------------------------------------------------
//! @internal
//! @brief  リソースサイズに応じてヒープの割り当て先（前後）を切り替えるアロケータです。
//---------------------------------------------------------------------------
class ResourceAllocator : public atk::detail::fnd::IAllocator
{
public:
    ResourceAllocator() NN_NOEXCEPT : m_ThresholdSize(0), m_SourceAllocator(NULL), m_IsInitialized(false) { }
    virtual ~ResourceAllocator() NN_NOEXCEPT NN_OVERRIDE { }

public:
    void Initialize(atk::detail::fnd::IAllocator* allocator, size_t thresholdSize) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(allocator);
        m_ThresholdSize = thresholdSize;
        m_SourceAllocator = allocator;
        m_IsInitialized = true;
    }

    bool IsInitialized() NN_NOEXCEPT
    {
        return m_IsInitialized;
    }

    void Finalize() NN_NOEXCEPT
    {
        m_ThresholdSize = 0;
        m_SourceAllocator = NULL;
        m_IsInitialized = false;
    }

    virtual void* Alloc(size_t size, int alignment = atk::detail::fnd::MemoryTraits::DefaultAlignment) NN_NOEXCEPT NN_OVERRIDE
    {
        NN_SDK_ASSERT_NOT_NULL(m_SourceAllocator);
        return m_SourceAllocator->Alloc(
            size,
            m_ThresholdSize < size ? alignment : -alignment);
    }

    virtual void Free(void* ptr) NN_NOEXCEPT NN_OVERRIDE
    {
        NN_SDK_ASSERT_NOT_NULL(m_SourceAllocator);
        return m_SourceAllocator->Free(ptr);
    }

private:
    size_t                        m_ThresholdSize;
    atk::detail::fnd::IAllocator* m_SourceAllocator;
    bool                          m_IsInitialized;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
