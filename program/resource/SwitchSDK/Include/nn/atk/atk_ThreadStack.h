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

#include <cstddef>
#include <cstdint>
#include <nn/nn_Macro.h>

namespace nn {
namespace atk {
namespace detail {

class ThreadStack
{
public:
    ThreadStack() NN_NOEXCEPT : m_Addr( static_cast<uintptr_t>(NULL) ), m_Size( 0 ) {}
    ~ThreadStack() NN_NOEXCEPT { Finalize(); }

    void Initialize( uintptr_t ptr, size_t size ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    uintptr_t GetStackBottom() const NN_NOEXCEPT { return m_Addr + m_Size; }
    uintptr_t GetBaseAddress() const NN_NOEXCEPT { return m_Addr; }

private:
    uintptr_t m_Addr;
    size_t    m_Size;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

