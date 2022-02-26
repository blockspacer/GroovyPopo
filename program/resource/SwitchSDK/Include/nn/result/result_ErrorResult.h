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

#include <nn/result/result_ResultBase.h>
#include <nn/nn_Result.h>

namespace nn { namespace result {

namespace detail {

    template <int Module_, int Description_>
    class ErrorResultBase : public ResultBase<ErrorResultBase<Module_, Description_>>
    {
    private:
        typedef ResultBase<ErrorResultBase<Module_, Description_>> Base;
    public:
        static const int Module = Module_;
        static const int Description = Description_;
        static const typename Base::InnerType InnerValue = ResultTraits::MakeInnerValueStatic<Module, Description>::value;
        static_assert(InnerValue != Base::InnerSuccessValue, "not ErrorResult");
        NN_IMPLICIT operator Result() const NN_NOEXCEPT
        {
            return ConstructResult(InnerValue);
        }
        bool IsSuccess() const NN_NOEXCEPT
        {
            return false;
        }
        NN_IMPLICIT operator ResultSuccess() const NN_NOEXCEPT
        {
            OnUnhandledResult(*this);
        }
        typename Base::InnerType GetInnerValueForDebug() const NN_NOEXCEPT
        {
            return ResultTraits::MakeInnerValue(Module, Description);
        }
    };

    template <int Module_, int DescriptionBegin_, int DescriptionEnd_>
    class ErrorRange
    {
    public:
        static const int Module = Module_;
        static const int DescriptionBegin = DescriptionBegin_;
        static const int DescriptionEnd = DescriptionEnd_;
        static_assert(ResultTraits::ModuleBegin <= Module && Module < ResultTraits::ModuleEnd, "invalid Module number");
        static_assert(ResultTraits::DescriptionBegin <= DescriptionBegin && DescriptionBegin < ResultTraits::DescriptionEnd, "invalid Description number");
        static_assert(ResultTraits::DescriptionBegin < DescriptionEnd && DescriptionEnd <= ResultTraits::DescriptionEnd, "invalid Description number");
        static_assert(DescriptionBegin <= DescriptionEnd, "not DescriptionBegin <= DescriptionEnd");
        static bool Includes(Result result) NN_NOEXCEPT
        {
            return result.GetModule() == Module && DescriptionBegin <= result.GetDescription() && result.GetDescription() < DescriptionEnd;
        }
        friend bool operator<=(Result result, ErrorRange) NN_NOEXCEPT
        {
            return Includes(result);
        }
    };

}

}}

#if defined(NN_BUILD_FOR_DOCUMENT_GENERATION)

#define NN_DEFINE_ERROR_RANGE_RESULT(name, module, descriptionBegin, descriptionEnd) \
    class name \
    {}

#define NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(name, module, descriptionBegin, descriptionEnd) \
    class name \
    {}

#else

#define NN_DEFINE_ERROR_RANGE_RESULT(name, module, descriptionBegin, descriptionEnd) \
    class name \
        : public ::nn::result::detail::ErrorResultBase<module, descriptionBegin> \
        , public ::nn::result::detail::ErrorRange<module, descriptionBegin, descriptionEnd> \
    {}

#define NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(name, module, descriptionBegin, descriptionEnd) \
    class name \
        : public ::nn::result::detail::ErrorRange<module, descriptionBegin, descriptionEnd> \
    {}

#endif
