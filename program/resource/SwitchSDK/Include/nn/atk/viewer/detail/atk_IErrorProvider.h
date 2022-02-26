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

#include <nn/atk/viewer/atk_Result.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

//! @brief  エラー情報を提供するインターフェイスです。
class IErrorProvider
{
public:
    virtual ~IErrorProvider() NN_NOEXCEPT {}
public:
    virtual Result GetLastError() const NN_NOEXCEPT = 0;
    virtual void   SetLastError(ResultType value) NN_NOEXCEPT = 0;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
