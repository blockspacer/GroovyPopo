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

#include <cstdint>
#include <nn/nn_Macro.h>
#include <nn/nn_SdkAssert.h>

namespace nn {
namespace atk {
namespace detail {

/* ========================================================================
        class definition
   ======================================================================== */

class MidiStreamParser
{
public:
    typedef void (*MidiCallback)( uint8_t status, uint8_t data1, uint8_t data2, void* arg );

    MidiStreamParser() NN_NOEXCEPT;
    void SetCallback( MidiCallback callback, void* arg ) NN_NOEXCEPT;
    void Parse( const void* buffer, unsigned long size ) NN_NOEXCEPT;
    void Reset() NN_NOEXCEPT;

private:
    struct MidiMsgBuffer
    {
        const uint8_t* ptr;
        uint32_t len;
        uint32_t readPos;
        uint32_t donePos;
    };

    bool ReadByte( uint8_t* data ) NN_NOEXCEPT;
    bool PeakByte( uint8_t* data ) NN_NOEXCEPT;
    void BackByte( uint8_t byte ) NN_NOEXCEPT;
    void EatByte() NN_NOEXCEPT;
    void SetMsgBuffer( const void* buffer, uint32_t len ) NN_NOEXCEPT;
    void ParseBuffer() NN_NOEXCEPT;
    void RestBuffer() NN_NOEXCEPT;
    bool SeekStatusByte() NN_NOEXCEPT;

    static bool IsDataByte( uint8_t b ) NN_NOEXCEPT { return ( b & 0x80 ) ? false : true; }
    static bool IsStatusByte( uint8_t b ) NN_NOEXCEPT { return ( b & 0x80 ) ? true : false; }
    static bool IsRealtimeMesg( uint8_t b ) NN_NOEXCEPT { return ( b >= 0xf8 ) ? true : false; }
    static bool IsSystemResetMesg( uint8_t b ) NN_NOEXCEPT { return ( b == 0xff ) ? true : false; }

    MidiCallback m_CallbackFunc;
    void* m_pCallbackArg;

    MidiMsgBuffer m_MsgBuffer;
    MidiMsgBuffer m_RestBuffer;

    uint8_t m_RestBuf[256];
    bool m_IsReset;
    bool m_IsSysEx;
    uint8_t m_RunningStatus;
    uint8_t m_BackByte;
    bool m_IsBackByteAvailable;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

