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

#include <nn/vfx/vfx_Enum.h>
#include <nn/vfx/vfx_Heap.h>

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

//---------------------------------------------------------------------------
//! @brief  ファイルの読み込み・書き出し
//---------------------------------------------------------------------------
class FileSystem
{
    NN_DISALLOW_COPY( FileSystem );
public:
    //---------------------------------------------------------------------------
    //! @brief  ファイル読み込み処理です。
    //! @param[out] pOutReturnBuffer     TBD
    //! @param[out] pOutReturnBufferSize TBD
    //! @param[in] fileName             TBD
    //! @param[in] pHeap                TBD
    //! @param[in] alignment            TBD
    //! @return TBD
    //---------------------------------------------------------------------------
    static bool Load( void** pOutReturnBuffer,
                      size_t* pOutReturnBufferSize,
                      const char* fileName, nn::vfx::Heap* pHeap,
                      size_t alignment = nn::vfx::SystemParameters_ParticleBinaryDataAlignment ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ファイル書き出し処理です。
    //! @param[in] fileName     TBD
    //! @param[in] buffer       TBD
    //! @param[in] size         TBD
    //! @param[in] count        TBD
    //! @param[in] isBinary     TBD
    //---------------------------------------------------------------------------
    static void Write( const char* fileName, void* buffer, size_t size, int count, bool isBinary ) NN_NOEXCEPT;
};

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn
