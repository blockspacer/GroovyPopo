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

#include <nn/atk/fnd/binary/atkfnd_PrimitiveTypes.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief  チャンクヘッダ
//---------------------------------------------------------------------------
struct ChunkHeader
{
    ChunkHeader() NN_NOEXCEPT :
        id(0),
        size(0)
    { }

    explicit ChunkHeader(uint32_t id) NN_NOEXCEPT :
        id(id),
        size(0)
    { }

    ChunkHeader(uint32_t id, uint32_t size) NN_NOEXCEPT :
        id(id),
        size(size)
    { }

    uint32_t      id;
    PcBinU32 size;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  チャンクヘッダ（ビッグエンディアン版）
//---------------------------------------------------------------------------
struct ChunkHeaderBE
{
    ChunkHeaderBE() NN_NOEXCEPT :
        id(0),
        size(0)
    { }

    explicit ChunkHeaderBE(uint32_t id) NN_NOEXCEPT :
        id(id),
        size(0)
    { }

    ChunkHeaderBE(uint32_t id, uint32_t size) NN_NOEXCEPT :
        id(id),
        size(size)
    { }

    uint32_t        id;
    PcBeBinU32 size;
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
