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

#include <cstdint>
#include <climits>
#include <type_traits>
#include <nn/nn_Common.h>

namespace nn { namespace result {

namespace detail {

    class ResultTraits
    {
    public:
        typedef std::uint32_t InnerType;
        static const InnerType InnerSuccessValue = InnerType();
        static const int ModuleBitsOffset = 0;
        static const int ModuleBitsCount = 9;
        static const int ModuleBegin = 1;
        static const int ModuleEnd = 1 << ModuleBitsCount;
        static const int DescriptionBitsOffset = ModuleBitsOffset + ModuleBitsCount;
        static const int DescriptionBitsCount = 13;
        static const int DescriptionBegin = 0;
        static const int DescriptionEnd = 1 << DescriptionBitsCount;
        static const int ReservedBitsOffset = DescriptionBitsOffset + DescriptionBitsCount;
        static const int ReservedBitsCount = 10;
    private:
        static const int EndOffset = ReservedBitsOffset + ReservedBitsCount;
        static_assert(EndOffset == sizeof(InnerType) * CHAR_BIT, "");
        static InnerType GetBitsValue(InnerType value, int offset, int count) NN_NOEXCEPT
        {
            return (value >> offset) & ~(~InnerType() << count);
        }
    public:
        template <int Module, int Description>
        struct MakeInnerValueStatic : public std::integral_constant<InnerType,
            (Module << ModuleBitsOffset) | (Description << DescriptionBitsOffset)
        >
        {};
        static InnerType MakeInnerValue(int module, int description) NN_NOEXCEPT
        {
            return (static_cast<InnerType>(module) << ModuleBitsOffset) | (static_cast<InnerType>(description) << DescriptionBitsOffset);
        }
        static InnerType GetModuleFromValue(InnerType value) NN_NOEXCEPT
        {
            return GetBitsValue(value, ModuleBitsOffset, ModuleBitsCount);
        }
        static InnerType GetDescriptionFromValue(InnerType value) NN_NOEXCEPT
        {
            return GetBitsValue(value, DescriptionBitsOffset, DescriptionBitsCount);
        }
    };

    template <typename Self>
    class ResultBase
    {
    public:
        /**
            @brief Result の内部表現を表す型で std::int32_t です。(デバッグ用)
        */
        typedef ResultTraits::InnerType InnerType;

        /**
            @brief Result の成功時の内部表現の値です。(デバッグ用)
        */
        static const InnerType InnerSuccessValue = ResultTraits::InnerSuccessValue;

        int GetModule() const NN_NOEXCEPT
        {
            return static_cast<int>(ResultTraits::GetModuleFromValue(static_cast<const Self&>(*this).GetInnerValueForDebug()));
        }

        int GetDescription() const NN_NOEXCEPT
        {
            return static_cast<int>(ResultTraits::GetDescriptionFromValue(static_cast<const Self&>(*this).GetInnerValueForDebug()));
        }
    };

}

}}
