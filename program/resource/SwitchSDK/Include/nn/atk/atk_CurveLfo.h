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
        structure definition
   ======================================================================== */

struct CurveLfoParam
{
    float depth;
    float speed;   // [Hz]
    uint32_t delay;
    uint8_t  range;
    uint8_t  curve; // enum Curve
    uint8_t  phase;
    uint8_t  padding[1];

    enum CurveType
    {
        CurveType_Sine,
        CurveType_Min = CurveType_Sine,
        CurveType_Triangle,
        CurveType_Saw,
        CurveType_Square,
        CurveType_Random,
        CurveType_UserMin = 64,
        CurveType_UserMax = 127,
        CurveType_Max = CurveType_UserMax,
        CurveType_Count
    };
    CurveLfoParam() NN_NOEXCEPT { Initialize(); }
    void Initialize() NN_NOEXCEPT;
};

/* ========================================================================
        class definition
   ======================================================================== */

class CurveLfo
{
public:
    CurveLfo() NN_NOEXCEPT : m_DelayCounter(0), m_Counter( 0.0f ), m_RandomValue(1.0f),
                 m_IsStart(false), m_IsNext(false) {}
    void Reset() NN_NOEXCEPT;

    void Update( int msec ) NN_NOEXCEPT;
    float  GetValue() const NN_NOEXCEPT;

    void SetParam( const CurveLfoParam& lfoParam ) NN_NOEXCEPT { m_Param = lfoParam; /* copy */ }
    CurveLfoParam& GetParam() NN_NOEXCEPT { return m_Param; }
    const CurveLfoParam& GetParam() const NN_NOEXCEPT { return m_Param; }

    typedef float (*CurveFunc)(float arg);

    static void InitializeCurveTable() NN_NOEXCEPT;
    static CurveFunc RegisterUserCurve(CurveFunc userCurve, uint32_t index) NN_NOEXCEPT; // index は 0～63
    static CurveFunc UnregisterUserCurve(uint32_t index) NN_NOEXCEPT; // index は 0-63

private:
    CurveLfoParam m_Param;
    uint32_t      m_DelayCounter;
    float      m_Counter;
    mutable float m_RandomValue;
    bool     m_IsStart;
    bool     m_IsNext;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t  m_Padding[2];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

