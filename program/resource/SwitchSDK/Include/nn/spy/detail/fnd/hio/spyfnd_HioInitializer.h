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

#include <nn/spy/spy_Config.h>
#ifdef NN_BUILD_CONFIG_SPY_ENABLED

#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
#include <cafe/hio.h>
#endif

namespace nn {
namespace spy {
namespace detail {
namespace fnd {

#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
//---------------------------------------------------------------------------
//! @brief    HIO の初期化をサポートします。
//---------------------------------------------------------------------------
class HioInitializer
{
private:
    HioInitializer() NN_NOEXCEPT { }

public:
    //! @brief 初期化します。
    static void Initialize() NN_NOEXCEPT;

    //! @brief 終了します。
    static void Finalize() NN_NOEXCEPT;

private:
    static int m_Count;     //!< 初期化回数です。
};
#endif

} // namespace nn::spy::detail::fnd
} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn

#endif // NN_BUILD_CONFIG_SPY_ENABLED
