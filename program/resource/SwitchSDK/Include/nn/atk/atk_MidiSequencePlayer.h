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

#include <nn/atk/atk_SequenceSoundPlayer.h>
#include <nn/atk/atk_Channel.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

/* ========================================================================
        class definition
   ======================================================================== */

class MidiSequencePlayer : public SequenceSoundPlayer
{
    /* ------------------------------------------------------------------------
            type definition
       ------------------------------------------------------------------------ */
public:
    enum {
        MidiMessage_NoteOff = 0x80,
        MidiMessage_NoteOn = 0x90,
        MidiMessage_PolyKeyPressure = 0xa0,
        MidiMessage_ControlChange = 0xb0,
        MidiMessage_ProgramChange = 0xc0,
        MidiMessage_ChannelPressure = 0xd0,
        MidiMessage_PitchBend = 0xe0,
        MidiMessage_System = 0xf0
    };

    enum {
        MidiControlChange_BankSelect = 0,
        MidiControlChange_ModDepth = 1,
        MidiControlChange_InitPan = 3,
        MidiControlChange_PortaTime = 5,
        MidiControlChange_DataEntry = 6,
        MidiControlChange_Volume = 7,
        MidiControlChange_Span = 9,
        MidiControlChange_Pan = 10,
        MidiControlChange_Expression = 11,
        MidiControlChange_MainVolume = 12,
        MidiControlChange_Transpose = 13,
        MidiControlChange_Priority = 14,
        MidiControlChange_Setvar0 = 16,
        MidiControlChange_Setvar1 = 17,
        MidiControlChange_Setvar2 = 18,
        MidiControlChange_Setvar3 = 19,
        MidiControlChange_BendRange = 20,
        MidiControlChange_ModSpeed = 21,
        MidiControlChange_ModType = 22,
        MidiControlChange_ModRange = 23,
        MidiControlChange_ModDelay = 26,
        MidiControlChange_ModDelay10 = 27,
        MidiControlChange_SweepPitch = 28,
        MidiControlChange_SweepPitch24 = 29,
        MidiControlChange_BiquadType = 30,
        MidiControlChange_BiquadValue = 31,
        MidiControlChange_Damper = 64,
        MidiControlChange_Porta = 65,
        MidiControlChange_Monophonic = 68,
        MidiControlChange_Cutoff = 74,
        MidiControlChange_EnvHold = 79,
        MidiControlChange_PortaControl = 84,
        MidiControlChange_Attack = 85,
        MidiControlChange_Decay = 86,
        MidiControlChange_Sustain = 87,
        MidiControlChange_Release = 88,
        MidiControlChange_FxsendA = 91,
        MidiControlChange_FxsendB = 92,
        MidiControlChange_FxsendC = 93,
        MidiControlChange_Mainsend = 95,
        MidiControlChange_NrpnLsb = 98,
        MidiControlChange_NrpnMsb = 99,
        MidiControlChange_RpnLsb = 100,
        MidiControlChange_RpnMsb = 101,
        MidiControlChange_Frontbypass =119
    };

    enum {
        MidiModeMessage_AllSoundOff = 120,
        MidiModeMessage_ResetAllController = 121,
        MidiModeMessage_LocalControl = 122,
        MidiModeMessage_AllNoteOff = 123,
        MidiModeMessage_OmniOff = 124,
        MidiModeMessage_OmniOn = 125,
        MidiModeMessage_MonoMode = 126,
        MidiModeMessage_PolyMode = 127
    };

    enum {
        // ( msb << 8 ) + lsb
        MidiRpn_PitchbendSensitivity = ( 0 << 8 ) + 0,
        MidiRpn_Null = ( 127 << 8 ) + 127
    };

    enum {
        // ( msb << 8 ) + lsb
        MidiNrpn_EnvReset = ( 0 << 8 ) + 0,
        MidiNrpn_Null = ( 127 << 8 ) + 127
    };

    static const int NoteInfoCount = Channel::ChannelCount;

    struct NoteInfo
    {
        Channel* channel;
        int channelIndex;
        int key;
    };

    struct ParameterControlInfo
    {
        enum Mode
        {
            Mode_Rpn,
            Mode_Nrpn
        };

        Mode mode;
        uint8_t rpnLsb;
        uint8_t rpnMsb;
        uint8_t nrpnLsb;
        uint8_t nrpnMsb;

        ParameterControlInfo() NN_NOEXCEPT
        : mode(Mode_Rpn),
          rpnLsb(127),
          rpnMsb(127),
          nrpnLsb(127),
          nrpnMsb(127)
        {}
    };

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    MidiSequencePlayer() NN_NOEXCEPT;
    virtual ~MidiSequencePlayer() NN_NOEXCEPT NN_OVERRIDE;

    void Prepare(
        const void* banks[],    // バンクファイル (4つ)
        const void* warcs[],    // 波形アーカイブファイル (4つ)
        bool warcIsIndividuals[] ) NN_NOEXCEPT;

    virtual void Start() NN_NOEXCEPT NN_OVERRIDE;

    void SendMessage( uint8_t status, uint8_t data1, uint8_t data2 ) NN_NOEXCEPT;
    void Reset() NN_NOEXCEPT;

    void SetProgramNumber( int channelIndex, int prgNo ) NN_NOEXCEPT;

protected:
    virtual void ChannelCallback( Channel* channel ) NN_NOEXCEPT NN_OVERRIDE;

private:
    void NoteOn( uint8_t channelIndex, uint8_t key, uint8_t velocity ) NN_NOEXCEPT;
    void NoteOff( uint8_t channelIndex, uint8_t key, uint8_t velocity ) NN_NOEXCEPT;

    void HandleControlChangeMessage( uint8_t channelIndex, uint8_t control, uint8_t value ) NN_NOEXCEPT;
    void HandleRpnMessage( uint8_t channelIndex, uint8_t value ) NN_NOEXCEPT;
    void HandleNrpnMessage( uint8_t channelIndex, uint8_t value ) NN_NOEXCEPT;
    void HandleChannelModeMessage( uint8_t control, uint8_t value ) NN_NOEXCEPT;
    void HandleProgramChangeMessage( uint8_t channelIndex, uint8_t program ) NN_NOEXCEPT;
    void HandlePitchBendMessage( uint8_t channelIndex, uint8_t lsb, uint8_t msb ) NN_NOEXCEPT;

    void NoteOffAll() NN_NOEXCEPT;
    void StopAllSound() NN_NOEXCEPT;
    void ResetAllController() NN_NOEXCEPT;

    NoteInfo* FindFreeNoteInfo() NN_NOEXCEPT;
    NoteInfo* FindNoteInfo( int channelIndex, int key ) NN_NOEXCEPT;
    NoteInfo* FindNoteInfo( int channelIndex ) NN_NOEXCEPT;

    NoteInfo m_NoteInfo[ NoteInfoCount ];
    ParameterControlInfo m_ParameterControlInfo[ TrackCountPerPlayer ];
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

