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

#include <nn/util/util_BytePtr.h>
#include <nn/atk/fnd/basis/atkfnd_Config.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//! @internal
template<typename TValue, int Alignment>
class AlignedValue
{
public:
    AlignedValue() NN_NOEXCEPT
    {
        new(&GetValue()) TValue();
    }

    const TValue& GetValue() const NN_NOEXCEPT
    {
        return *static_cast<const TValue*>(util::BytePtr(m_Buffer).AlignUp(Alignment).Get());
    }

    TValue& GetValue() NN_NOEXCEPT
    {
        return *static_cast<TValue*>(util::BytePtr(m_Buffer).AlignUp(Alignment).Get());
    }

private:
    uint8_t m_Buffer[sizeof(TValue) + Alignment];
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
