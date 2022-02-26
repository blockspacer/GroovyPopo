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

namespace nn {
namespace atk {
namespace detail {

/* ========================================================================
        class definition
   ======================================================================== */

class CurveAdshr
{
    /* ------------------------------------------------------------------------
            typename definition
       ------------------------------------------------------------------------ */
public:
    enum Status
    {
        Status_Attack = 0,
        Status_Hold,
        Status_Decay,
        Status_Sustain,
        Status_Release
    };

    /* ------------------------------------------------------------------------
            constant declaration
       ------------------------------------------------------------------------ */
private:
    static const float VolumeInit; // -90.4;
    static const int AttackInit    = 127;
    static const int HoldInit      = 0;
    static const int DecayInit     = 127;
    static const int SustainInit   = 127;
    static const int ReleaseInit   = 127;

    static const int DecibelSquareTableSize  = 128;
    static const int CalcDecibelScaleMax     = 127;
    static const int16_t DecibelSquareTable[ DecibelSquareTableSize ];

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    CurveAdshr() NN_NOEXCEPT;
    void Initialize( float initDecibel = VolumeInit ) NN_NOEXCEPT;
    void Reset( float initDecibel = VolumeInit ) NN_NOEXCEPT;

    void Update( int msec ) NN_NOEXCEPT;
    float  GetValue() const NN_NOEXCEPT;

    // Accesser
    Status GetStatus() const NN_NOEXCEPT { return m_Status; }
    void SetStatus( Status status ) NN_NOEXCEPT { m_Status = status; }

    void SetAttack( int attack ) NN_NOEXCEPT;
    void SetHold( int hold ) NN_NOEXCEPT;
    void SetDecay( int decay ) NN_NOEXCEPT;
    void SetSustain( int sustain ) NN_NOEXCEPT;
    void SetRelease( int release ) NN_NOEXCEPT;

private:
    static float CalcRelease( int release ) NN_NOEXCEPT;
    static int16_t CalcDecibelSquare( int scale ) NN_NOEXCEPT;

    Status m_Status;
    float m_Value;
    float m_Decay;
    float m_Release;
    float m_Attack;
    uint16_t m_Hold;
    uint16_t m_HoldCounter;
    uint8_t  m_Sustain;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t  m_Padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
};


} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

