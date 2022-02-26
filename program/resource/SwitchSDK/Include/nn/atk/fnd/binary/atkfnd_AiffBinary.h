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

#include <cfloat>
#include <cmath>
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
//! @brief  IEEE Standard 754 floating point number（ビッグエンディアン版）
//---------------------------------------------------------------------------
struct IEEE754BE
{
    IEEE754BE() NN_NOEXCEPT:
        head(0),
        hiMant(0),
        loMant(0)
    { }

    NN_IMPLICIT operator double() const NN_NOEXCEPT
    {
        double result;
        int expon;

        expon = head & 0x7fff;

        if (expon == 0 && hiMant == 0 && loMant == 0)
        {
            result = 0;
        }
        else
        {
            if (expon == 0x7FFF)
            {    /* Infinity or NaN */
                result = DBL_MAX;
            }
            else
            {
                expon -= 0x3fff;

                result = hiMant * pow(2.0, (expon -= 31));
                result += loMant * pow(2.0, (expon -= 32));
            }
        }

        return (head & 0x8000) != 0 ? -result : result;
    }

    PcBeBinU16 head;
    PcBeBinU32 hiMant;
    PcBeBinU32 loMant;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  Aiff COMM チャンク
//---------------------------------------------------------------------------
struct AiffCommChunk
{
    static const uint32_t ValidId = NN_ATK_BINARY_MAKE_SIGNATURE('C', 'O', 'M', 'M');

    AiffCommChunk() NN_NOEXCEPT :
        header(ValidId),
        channelCount(0),
        sampleFrameCount(0),
        sampleSize(0)
    { }

    static uint32_t GetValidBodySize() NN_NOEXCEPT
    {
        return sizeof(AiffCommChunk) - sizeof(ChunkHeaderBE);
    }

    bool IsValid() NN_NOEXCEPT
    {
        return header.id == ValidId;
    }

    ChunkHeaderBE header;
    PcBeBinU16    channelCount;
    PcBeBinU32    sampleFrameCount;
    PcBeBinU16    sampleSize;
    IEEE754BE     sampleRate;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  Aiff SSND チャンク
//---------------------------------------------------------------------------
struct AiffSoundDataChunk
{
    static const uint32_t ValidId = NN_ATK_BINARY_MAKE_SIGNATURE('S', 'S', 'N', 'D');

    AiffSoundDataChunk() NN_NOEXCEPT :
        header(ValidId),
        offset(0),
        blockSize(0)
    { }

    static uint32_t GetValidBodySize() NN_NOEXCEPT
    {
        return sizeof(AiffSoundDataChunk) - sizeof(ChunkHeaderBE);
    }

    bool IsValid() NN_NOEXCEPT
    {
        return header.id == ValidId;
    }

    ChunkHeaderBE header;
    PcBeBinU32    offset;
    PcBeBinU32    blockSize;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  Aiff FORM チャンクヘッダ
//---------------------------------------------------------------------------
struct AiffFormChunkHeader
{
    static const uint32_t ValidId = NN_ATK_BINARY_MAKE_SIGNATURE('F', 'O', 'R', 'M');
    static const uint32_t ValidFormatType = NN_ATK_BINARY_MAKE_SIGNATURE('A', 'I', 'F', 'F');

    AiffFormChunkHeader() NN_NOEXCEPT :
        header(ValidId)
    { }

    bool IsValid() NN_NOEXCEPT
    {
        return header.id == ValidId && formType == ValidFormatType;
    }

    ChunkHeaderBE header;
    uint32_t           formType;
};

#pragma pack(pop)

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
