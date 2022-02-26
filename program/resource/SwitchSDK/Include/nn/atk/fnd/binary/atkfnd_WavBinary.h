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
#include <nn/atk/fnd/binary/atkfnd_Binary.h>
#include <nn/atk/fnd/binary/atkfnd_IffBinary.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

#pragma pack(push, 1)

//---------------------------------------------------------------------------
//! @internal
//! @brief  RIFF チャンク
//---------------------------------------------------------------------------
struct RiffChunk
{
    static const uint32_t ValidId = NN_ATK_BINARY_MAKE_SIGNATURE('R', 'I', 'F', 'F');

    RiffChunk() NN_NOEXCEPT :
        header(ValidId),
        formatType(0)
    { }

    bool IsValid() NN_NOEXCEPT
    {
        return header.id == ValidId;
    }

    ChunkHeader header;
    uint32_t formatType;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  fmt チャンク
//---------------------------------------------------------------------------
struct FmtChunk
{
    static const uint32_t ValidId = NN_ATK_BINARY_MAKE_SIGNATURE('f', 'm', 't', ' ');
    static const uint16_t FormatPcm = 1;

    FmtChunk() NN_NOEXCEPT :
        header(ValidId, GetValidBodySize()),
        formatTag(0),
        channels(0),
        samplesPerSec(0),
        avgBytesPerSec(0),
        blockAlign(0),
        bitsPerSample(0)
    { }

    static uint32_t GetValidBodySize() NN_NOEXCEPT
    {
        return sizeof(FmtChunk) - sizeof(ChunkHeader);
    }

    bool IsValid() NN_NOEXCEPT
    {
        return header.id == ValidId;
    }

    ChunkHeader header;
    PcBinU16 formatTag;
    PcBinU16 channels;
    PcBinU32 samplesPerSec;
    PcBinU32 avgBytesPerSec;
    PcBinU16 blockAlign;
    PcBinU16 bitsPerSample;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  data チャンク
//---------------------------------------------------------------------------
struct DataChunk
{
    static const uint32_t ValidId = NN_ATK_BINARY_MAKE_SIGNATURE('d', 'a', 't', 'a');

    DataChunk() NN_NOEXCEPT : header(ValidId) { }

    bool IsValid() NN_NOEXCEPT
    {
        return header.id == ValidId;
    }

    ChunkHeader header;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  WAV バイナリヘッダ
//---------------------------------------------------------------------------
struct WaveBinaryHeader
{
    WaveBinaryHeader() NN_NOEXCEPT
    {
        riffChunk.formatType = NN_ATK_BINARY_MAKE_SIGNATURE('W', 'A', 'V', 'E');
    }

    RiffChunk riffChunk;
    FmtChunk fmtChunk;
    DataChunk dataChunk;
};

#pragma pack(pop)

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
