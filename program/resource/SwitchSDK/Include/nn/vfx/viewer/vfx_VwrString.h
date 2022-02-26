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

#include <nn/vfx/vfx_TargetDef.h>
#if defined( NN_BUILD_CONFIG_OS_WIN )
#include <nn/nn_Windows.h>
#endif

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

//---------------------------------------------------------------------------
//! @brief  エフェクトビューア用の簡易文字列
//---------------------------------------------------------------------------
struct ViewerString
{
#if defined( NN_BUILD_CONFIG_OS_WIN )
    static const int MAX_STRING = _MAX_PATH; //!< TBD
#else
    static const int MAX_STRING = 260;      //!< TBD
#endif

    //---------------------------------------------------------------------------
    //! @brief  文字列へのアドレスを取得します。
    //!
    //! @return 文字列へのアドレスを返します。
    //---------------------------------------------------------------------------
    const char* CStr() const NN_NOEXCEPT
    {
        return value;
    }

    //---------------------------------------------------------------------------
    //! @brief                  文字列をコピーします。
    //!
    //! @param[in] SrcString    コピー元文字列の先頭アドレス
    //---------------------------------------------------------------------------
    void Copy( const char* SrcString ) NN_NOEXCEPT
    {
#if defined( NN_BUILD_CONFIG_OS_WIN )
        strcpy_s( value, MAX_STRING, SrcString );
#else
        NN_SDK_ASSERT( strlen( SrcString ) < MAX_STRING );
        strcpy( value, SrcString );
#endif
    }

    mutable char value[ MAX_STRING ];   //!< 文字列配列
};

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn
