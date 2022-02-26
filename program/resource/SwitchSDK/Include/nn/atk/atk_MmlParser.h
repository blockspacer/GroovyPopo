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

#include <nn/atk/atk_SequenceTrack.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

class MmlSequenceTrack;
class SequenceSoundPlayer;

/* ========================================================================
        class definition
   ======================================================================== */

class MmlParser
{
public:
    static const int PanCenter = 64;
    static const int SurroundPanCenter = 64;
    static const int TempoMin = 0;
    static const int TempoMax = 1023;

private:
    enum SeqArgType
    {
        SeqArgType_None,
        SeqArgType_U8,
        SeqArgType_S16,
        SeqArgType_Vmidi,
        SeqArgType_Random,
        SeqArgType_Variable
    };

public:
    virtual ~MmlParser() NN_NOEXCEPT {}

    SequenceTrack::ParseResult Parse(
        MmlSequenceTrack* track,
        bool doNoteOn
    ) const NN_NOEXCEPT;

    static uint32_t ParseAllocTrack( const void* baseAddress, uint32_t seqOffset, uint32_t* allocTrack ) NN_NOEXCEPT;

    static void EnablePrintVar( bool enable ) NN_NOEXCEPT { mPrintVarEnabledFlag = enable; }
    static bool IsEnabledPrintVar() NN_NOEXCEPT { return mPrintVarEnabledFlag; }

protected:
    virtual void CommandProc(
        MmlSequenceTrack* track,
        uint32_t command,
        int32_t commandArg1,
        int32_t commandArg2
    ) const NN_NOEXCEPT;

    virtual void NoteOnCommandProc(
        MmlSequenceTrack* track,
        int key,
        int velocity,
        int32_t length,
        bool tieFlag
    ) const NN_NOEXCEPT;

private:
    static bool mPrintVarEnabledFlag;

    uint8_t   ReadByte( const uint8_t** ptr ) const NN_NOEXCEPT { return *(*ptr)++; }
    void UnreadByte( const uint8_t** ptr ) const NN_NOEXCEPT { --(*ptr); }
    uint16_t  Read16( const uint8_t** ptr ) const NN_NOEXCEPT;
    uint32_t  Read24( const uint8_t** ptr ) const NN_NOEXCEPT;
    int32_t  ReadVar( const uint8_t** ptr ) const NN_NOEXCEPT;
    int32_t  ReadArg( const uint8_t** ptr, SequenceSoundPlayer* player, SequenceTrack* track, SeqArgType argType ) const NN_NOEXCEPT;
    volatile int16_t* GetVariablePtr( SequenceSoundPlayer* player, SequenceTrack* track, int varNo ) const NN_NOEXCEPT;
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

