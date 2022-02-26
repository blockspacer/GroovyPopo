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

#include <nn/vfx/vfx_TargetDef.h>

namespace nn {
namespace vfx {
namespace detail {

static const float PiX2 = nn::util::FloatPi * 2.0f;                                                 //!< 円周率x2
static const nn::util::Vector4fType BaseAmplifier = NN_UTIL_VECTOR_4F_INITIALIZER( 4, 3, 2, 1.5f ); //!< 振幅

//! 基本周期に対する比率
static const nn::util::Vector4fType WaveCycleRatio = NN_UTIL_VECTOR_4F_INITIALIZER(
    ( 1.0f / 0.60f ),
    ( 1.0f / 0.42f ),
    ( 1.0f / 0.23f ),
    ( 1.0f / 0.15f )
);

//------------------------------------------------------------------------------
//! @brief          GPUノイズの値を計算
//! @param[in,out]  pOutPos             パーティクル位置
//! @param[in]      pEmitter            エミッタへのポインタ
//! @param[in]      pParticleProperty   パーティクルアトリビュートへのポインタ
//! @param[in]      particleIndex       パーティクルのインデックス
//! @param[in]      randomVel           ランダム量
//! @param[in]      time                パーティクル時間
//------------------------------------------------------------------------------
void CalculateGpuNoise( nn::util::Vector3fType* pOutPos, Emitter* pEmitter, const ParticleProperty* pParticleProperty, int particleIndex, const nn::util::Vector3fType& randomVel, float time ) NN_NOEXCEPT;

#if 0
//------------------------------------------------------------------------------
// 疑似乱数生成器（積算可能）（標準機能）
// x時点での乱数の積算値を取得
//------------------------------------------------------------------------------
inline float RandFuncDefault( const float x, const float interval ) NN_NOEXCEPT
{
    // 波形合成
    const float t = x * interval;

    float aggregate = 0;
    aggregate += nn::util::SinEst( t * details::CYCLE_RATE[ 0 ] ) * details::AMP_BASE[ 0 ];
    aggregate += nn::util::SinEst( t * details::CYCLE_RATE[ 1 ] ) * details::AMP_BASE[ 1 ];
    aggregate += nn::util::SinEst( t * details::CYCLE_RATE[ 2 ] ) * details::AMP_BASE[ 2 ];
    aggregate += nn::util::SinEst( t * details::CYCLE_RATE[ 3 ] ) * details::AMP_BASE[ 3 ];

    return aggregate;
}
#endif

//------------------------------------------------------------------------------
//! @briefprivate       4つの波形を合成するユーティリティ
//! @param[in] t1       時刻t1
//! @param[in] t2       時刻t2
//! @param[in] t3       時刻t3
//! @param[in] t4       時刻t4
//! @param[in] cycle    周期に乗算する値
//! @param[in] amp      波の大きさ
//! @return             合成結果
//------------------------------------------------------------------------------
inline float GetAggregate4( float t1, float t2, float t3, float t4, float cycle, float amp ) NN_NOEXCEPT
{
    float add = -nn::util::SinEst( t1 * cycle );
    add += nn::util::SinEst( t2 * cycle );
    add += nn::util::SinEst( t3 * cycle );
    add -= nn::util::SinEst( t4 * cycle );
    return add * amp;
}

//------------------------------------------------------------------------------
//! @briefprivate       2つの波形を合成するユーティリティ
//! @param[in] t1       時刻t1
//! @param[in] t2       時刻t2
//! @param[in] cycle    周期に乗算する値
//! @param[in] amp      波の大きさ
//! @return             合成結果
//------------------------------------------------------------------------------
inline float GetAggregate2( float t1, float t2, float cycle, float amp ) NN_NOEXCEPT
{
    float add = -nn::util::SinEst( t1 * cycle );
    add += nn::util::SinEst( t2 * cycle );
    return add * amp;
}

//------------------------------------------------------------------------------
//! @briefprivate   疑似乱数生成器（積算可能）（標準機能）
//!                 x時点での乱数の積算値差分を取得
//! @param[in] t1   時刻t1
//! @param[in] t2   時刻t2
//! @return         疑似乱数値
//------------------------------------------------------------------------------
inline float RandFuncSubDefault( float t1, float t2 ) NN_NOEXCEPT
{
    // 波形合成
    float aggregate = GetAggregate2( t1, t2, nn::util::VectorGetX( detail::WaveCycleRatio ), nn::util::VectorGetX( detail::BaseAmplifier ) );
    aggregate += GetAggregate2( t1, t2, nn::util::VectorGetY( detail::WaveCycleRatio ), nn::util::VectorGetY( detail::BaseAmplifier ) );
    aggregate += GetAggregate2( t1, t2, nn::util::VectorGetZ( detail::WaveCycleRatio ), nn::util::VectorGetZ( detail::BaseAmplifier ) );
    aggregate += GetAggregate2( t1, t2, nn::util::VectorGetW( detail::WaveCycleRatio ), nn::util::VectorGetW( detail::BaseAmplifier ) );
    return aggregate;
}

//------------------------------------------------------------------------------
//! @briefprivate   疑似乱数生成器（積算可能）（標準機能）
//!                 x時点での乱数の積算値差分を取得
//!                 ( F2 - F1 ) - ( F4 - F3 )
//! @param[in] t1   時刻t1
//! @param[in] t2   時刻t2
//! @param[in] t3   時刻t3
//! @param[in] t4   時刻t4
//! @return         疑似乱数値
//------------------------------------------------------------------------------
inline float RandFuncSubSubDefault( float t1, float t2, float t3, float t4 ) NN_NOEXCEPT
{
    // 波形合成
    float aggregate = GetAggregate4( t1, t2, t3, t4, nn::util::VectorGetX( detail::WaveCycleRatio ), nn::util::VectorGetX( detail::BaseAmplifier ) );
    aggregate += GetAggregate4( t1, t2, t3, t4, nn::util::VectorGetY( detail::WaveCycleRatio ), nn::util::VectorGetY( detail::BaseAmplifier ) );
    aggregate += GetAggregate4( t1, t2, t3, t4, nn::util::VectorGetY( detail::WaveCycleRatio ), nn::util::VectorGetZ( detail::BaseAmplifier ) );
    aggregate += GetAggregate4( t1, t2, t3, t4, nn::util::VectorGetY( detail::WaveCycleRatio ), nn::util::VectorGetW( detail::BaseAmplifier ) );
    return aggregate;
}

//------------------------------------------------------------------------------
//! @briefprivate  疑似乱数生成器（積算可能）（詳細オプション使用）
//!         x時点での乱数の積算値を取得
//! @param[in] x                現在時刻x
//! @param[in] interval         フレーム数からRadianへ変換する定数
//! @param[in] pGpuNoiseData   GPUノイズのパラメータへのポインタ
//! @return 疑似乱数値
//------------------------------------------------------------------------------
inline float RandFunc( float x, float interval, const ResFieldRandom* pGpuNoiseData ) NN_NOEXCEPT
{
    // 振幅
    const nn::util::Float4 ampBase =
    NN_UTIL_FLOAT_4_INITIALIZER(
        pGpuNoiseData->waveParam0,
        pGpuNoiseData->waveParam1,
        pGpuNoiseData->waveParam2,
        pGpuNoiseData->waveParam3
    );

    // 基本周期に対する比率
    const float t = x * interval;
    nn::util::Float4 cycleRate = NN_UTIL_FLOAT_4_INITIALIZER(
        ( t / pGpuNoiseData->waveParamHzRate0 ),
        ( t / pGpuNoiseData->waveParamHzRate1 ),
        ( t / pGpuNoiseData->waveParamHzRate2 ),
        ( t / pGpuNoiseData->waveParamHzRate3 )
    );

    // 波形合成
    float aggregate = nn::util::SinEst( cycleRate.x ) * ampBase.x;
    aggregate += nn::util::SinEst( cycleRate.y ) * ampBase.y;
    aggregate += nn::util::SinEst( cycleRate.z ) * ampBase.z;
    aggregate += nn::util::SinEst( cycleRate.w ) * ampBase.w;

    return aggregate;
}

#if 0
//------------------------------------------------------------------------------
// [x1, x2]の区間の乱数の積算を取得
//------------------------------------------------------------------------------
inline float GetRandSum(
    const float x1,
    const float x2,
    const ResFieldRandom* resFieldRandom ) NN_NOEXCEPT
{
    return RandFunc( x2, resFieldRandom ) - RandFunc( x1, resFieldRandom );
}
#endif

//------------------------------------------------------------------------------
//! @briefprivate               現在の疑似乱数を取得（詳細オプション）（位相変化あり）
//! @param[in] seed             乱数シード
//! @param[in] time             時刻
//! @param[in] deltaS           乱数シード差分
//! @param[in] deltaV           時刻差分
//! @param[in] interval         フレーム数からRadianへ変換する定数
//! @param[in] pGpuNoiseData   GPUノイズのパラメータへのポインタ
//! @return                     疑似乱数値
//------------------------------------------------------------------------------
inline float GetRand(
    float seed,
    float time,
    float deltaS,
    float deltaV,
    float interval,
    const ResFieldRandom* pGpuNoiseData ) NN_NOEXCEPT
{
    const float x = seed + time;
    return RandFunc( x + deltaV + deltaS, interval, pGpuNoiseData ) - RandFunc( x, interval, pGpuNoiseData )
        - ( RandFunc( seed + deltaS, interval, pGpuNoiseData ) - RandFunc( seed, interval, pGpuNoiseData ) );
}

//------------------------------------------------------------------------------
//! @briefprivate               現在の疑似乱数を取得（詳細オプション）（位相変化なし）
//! @param[in] seed             乱数シード
//! @param[in] time             時刻
//! @param[in] deltaV           時刻差分
//! @param[in] interval         フレーム数からRadianへ変換する定数
//! @param[in] pGpuNoiseData   GPUノイズのパラメータへのポインタ
//! @return                     疑似乱数値
//------------------------------------------------------------------------------
inline float GetRandWithNoPhaseShift(
    float seed,
    float time,
    float deltaV,
    float interval,
    const ResFieldRandom* pGpuNoiseData ) NN_NOEXCEPT
{
    const float x = seed + time;
    return RandFunc( x + deltaV, interval, pGpuNoiseData ) - RandFunc( x, interval, pGpuNoiseData );
}

//------------------------------------------------------------------------------
//! @briefprivate       現在の疑似乱数を取得（標準）（位相変化あり）
//! @param[in] seed     乱数シード
//! @param[in] time     時刻
//! @param[in] deltaS   乱数シード差分
//! @param[in] deltaV   時刻差分
//! @param[in] interval フレーム数からRadianへ変換する定数
//! @return             疑似乱数値
//------------------------------------------------------------------------------
inline float GetRandDefault(
    float seed,
    float time,
    float deltaS,
    float deltaV,
    float interval ) NN_NOEXCEPT
{
    const float x = seed + time;
    const float sTemp = deltaS * interval;
    const float t1 = x * interval;
    const float t2 = t1 + ( deltaV * interval ) + sTemp;
    const float t3 = seed * interval;
    const float t4 = t3 + sTemp;
    return RandFuncSubSubDefault( t1, t2, t3, t4 );
}

//------------------------------------------------------------------------------
//! @briefprivate 現在の疑似乱数を取得（標準）（位相変化なし）
//! @param[in] seed     乱数シード
//! @param[in] time     パーティクル時間
//! @param[in] deltaV   時間差分
//! @param[in] interval フレーム数からRadianへ変換する定数
//! @return             疑似乱数値
//------------------------------------------------------------------------------
inline float GetRandWithNoPhaseShiftDefault(
    float seed,
    float time,
    float deltaV,
    float interval ) NN_NOEXCEPT
{
    const float x = seed + time;
    const float t1 = x * interval;
    const float t2 = t1 + deltaV * interval;
    return RandFuncSubDefault( t1, t2 );
}

} // namespace detail
} // namespace vfx
} // namespace nn
