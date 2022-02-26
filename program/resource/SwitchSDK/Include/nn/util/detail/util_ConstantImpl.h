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

#include <nn/nn_Common.h>
#include <nn/util/util_MathTypes.h>

//
// 定数を定義します。
//

namespace nn { namespace util { namespace detail {

extern const float FloatPi;             //!< π
extern const float Float2Pi;            //!< 2π
extern const float FloatPiDivided2;     //!< 1/2π
extern const float Float3PiDivided2;    //!< 3/2π
extern const float Float1Divided2Pi;    //!< 1/(2π)

extern const float FloatDegree180;  //!< 180度

extern const float FloatQuaternionEpsilon;

extern const float FloatUlp;            //!< 1.f + x != 1.f を満たす最小の正の値

extern const float FloatSqrt0Point5;    //!< sqrt(0.5)

extern const AngleIndex AngleIndexHalfRound;            //!< インデックス表現での 1/2 周
extern const AngleIndex AngleIndexQuarterRound;         //!< インデックス表現での 1/4 周
extern const AngleIndex AngleIndexThreeQuarterRound;    //!< インデックス表現での 3/4 周

}}}
