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
#include <nn/util/util_BinTypes.h>

namespace nn {
namespace atk {
namespace detail {

#pragma pack(push, 1)

//---------------------------------------------------------------------------
//! @brief  チャンクヘッダ
//---------------------------------------------------------------------------
struct ChunkHeader
{
    /* ctor */ ChunkHeader() NN_NOEXCEPT : id(0), size(0) { }

    /* ctor */ explicit ChunkHeader(uint32_t id) NN_NOEXCEPT : id(id), size(0) { }

    /* ctor */ ChunkHeader(uint32_t id, uint32_t size) NN_NOEXCEPT : id(id), size(size) { }

    uint32_t id;
    fnd::PcBinU32 size;
};

//---------------------------------------------------------------------------
//! @brief  RIFF チャンク
//---------------------------------------------------------------------------
struct RiffChunk
{
    static const uint32_t ValidId = NN_UTIL_CREATE_SIGNATURE_4('R', 'I', 'F', 'F');

    /* ctor */ RiffChunk() NN_NOEXCEPT : header(ValidId), formatType(0) { }

    bool IsValid() NN_NOEXCEPT
    {
        return header.id == ValidId;
    }

    ChunkHeader header;
    uint32_t formatType;
};

//---------------------------------------------------------------------------
//! @brief  fmt チャンク
//---------------------------------------------------------------------------
struct FmtChunk
{
    static const uint32_t ValidId = NN_UTIL_CREATE_SIGNATURE_4('f', 'm', 't', ' ');
    static const uint16_t FormatPcm = 1;

    /* ctor */ FmtChunk() NN_NOEXCEPT :
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
    fnd::PcBinU16 formatTag;
    fnd::PcBinU16 channels;
    fnd::PcBinU32 samplesPerSec;
    fnd::PcBinU32 avgBytesPerSec;
    fnd::PcBinU16 blockAlign;
    fnd::PcBinU16 bitsPerSample;
};

//---------------------------------------------------------------------------
//! @brief  data チャンク
//---------------------------------------------------------------------------
struct DataChunk
{
    static const uint32_t ValidId = NN_UTIL_CREATE_SIGNATURE_4('d', 'a', 't', 'a');

    /* ctor */ DataChunk() NN_NOEXCEPT : header(ValidId) { }

    bool IsValid() NN_NOEXCEPT
    {
        return header.id == ValidId;
    }

    ChunkHeader header;
};

//---------------------------------------------------------------------------
//! @brief  WAV バイナリヘッダ
//---------------------------------------------------------------------------
struct WaveBinaryHeader
{
    WaveBinaryHeader() NN_NOEXCEPT
    {
        riffChunk.formatType = NN_UTIL_CREATE_SIGNATURE_4('W', 'A', 'V', 'E');
    }

    RiffChunk riffChunk;
    FmtChunk fmtChunk;
    DataChunk dataChunk;
};

#pragma pack(pop)

} // detail
} // atk
} // nn
