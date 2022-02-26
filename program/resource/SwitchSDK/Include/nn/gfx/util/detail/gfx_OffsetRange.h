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

#include <nn/nn_Common.h>

namespace nn{ namespace gfx{ namespace util{ namespace detail{

    //! @brief オフセットの範囲を表します。
    //! @details
    //! 範囲は先頭と長さで表されます。
    //! 表現される範囲は [base, base + size) です。
    struct OffsetRange
    {
        //! @brief 先頭のオフセットです。
        ptrdiff_t base;
        //! @brief 範囲の長さです。
        ptrdiff_t size;
    };

}}}}
