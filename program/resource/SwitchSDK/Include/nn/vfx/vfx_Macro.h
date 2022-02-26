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

namespace nn {
namespace vfx {
namespace detail {

//---------------------------------------------------------------------------
//! @brief  Tagを生成するマクロ
//!         VFX_ENDIAN_LITTLE の有無はこのヘッダの外部で決定されます。
//---------------------------------------------------------------------------
#if defined( VFX_ENDIAN_LITTLE )
#define VFX_MAKE_TAG( a, b, c, d ) ( a | b << 8 | c << 16 | d << 24 )
#else
#define VFX_MAKE_TAG( a, b, c, d ) ( a << 24 | b << 16 | c << 8 | d )
#endif

} // namespace detail
} // namespace vfx
} // namespace nn
