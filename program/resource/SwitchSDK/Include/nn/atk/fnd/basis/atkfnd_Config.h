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

//---------------------------------------------------------------------------
//! @internal
//!
//! @namespace nn::atk::fnd
//!
//! @brief     サウンドランタイム用の基本クラスライブラリです。
//---------------------------------------------------------------------------

#include <cstddef>
#include <nn/nn_Macro.h>
#include <nn/atk/fnd/basis/atkfnd_Target.h>
#include <nn/atk/fnd/basis/atkfnd_Platform.h>

#if defined(NN_SDK_BUILD_DEBUG) || defined(NN_SDK_BUILD_DEVELOP)
#if defined(NN_BUILD_CONFIG_OS_HORIZON)
#define NN_ATK_FND_CONFIG_ENABLE_THREAD_CORE_NUMBER_OBSERVATION
#endif
#endif

namespace nn { namespace atk { namespace detail { namespace fnd {

#ifndef _POSITION_T_FND_DEFINED
    typedef ptrdiff_t position_t;
    #define _POSITION_T_FND_DEFINED
#endif

}}}} // namespace nn::atk::detail::fnd
