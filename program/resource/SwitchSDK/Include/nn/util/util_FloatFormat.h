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
#include <cstring>
#include <nn/nn_Common.h>
#include <nn/nn_StaticAssert.h>

namespace nn { namespace util {

NN_STATIC_ASSERT( sizeof( float ) == 4 );

//! @brief 浮動小数点数フォーマット
template< int TSignBit, int TExponentialBit, int TFractionalBit >
class FloatFormat
{
public:
    //! @brief ビット演算に使用する整数定数です。
    enum
    {
        FractionalBit       = TFractionalBit,    //!< 仮数部のビットです。
        ExponentialBit      = TExponentialBit,    //!< 指数部のビットです。
        SignBit             = TSignBit,    //!< 符号部のビットです。
        ExponentialBias     = ( 1 << ( ExponentialBit - 1 ) ) - 1,    //!< 指数部のバイアスです。
        FractionalShift     = 0,    //!< 仮数部へのビットシフトです。
        ExponentialShift    = FractionalShift + FractionalBit,    //!< 指数部へのビットシフトです。
        SignShift           = ExponentialShift + ExponentialBit,    //!< 符号部へのビットシフトです。
        BitWidth            = SignShift + SignBit,    //!< ビット幅です。
        FractionalMask      = ( ( 1 << FractionalBit ) - 1 ) << FractionalShift,    //!< 仮数部のマスクです。
        ExponentialMask     = ( ( 1 << ExponentialBit ) - 1 ) << ExponentialShift,    //!< 指数部のマスクです。
        SignMask            = ( ( 1 << SignBit ) - 1 ) << SignShift    //!< 符号部のマスクです。
    };

    //! @brief float からエンコードします。
    //!
    //! 非正規化数は 0 として扱います。
    //! 符号ビットを持たない型の場合、0 から 最大の有限値でクランプします。
    //!
    static int Encode( float source );

    //! @brief float にデコードします。
    //!
    //! 非正規化数は 0 として扱います。
    //!
    static float Decode( int source );

    //! @brief 非数かどうかを取得します。
    static bool IsNan( int source )
    {
        return ( source & ( ExponentialMask | FractionalMask ) ) > ExponentialMask;
    };

    //! @brief 無限大かどうかを取得します。
    static bool IsInfinite( int source )
    {
        return ( source & ( ExponentialMask | FractionalMask ) ) == ExponentialMask;
    };

    //! @brief 有限の値かどうかを取得します。
    static bool IsFinite( int source )
    {
        return ( source & ( ExponentialMask | FractionalMask ) ) < ExponentialMask;
    };

    //! @brief 正規化数かどうかを取得します。
    static bool IsNormal( int source )
    {
        int bits = source & ( ExponentialMask | FractionalMask );
        return FractionalMask < bits && bits < ExponentialMask;
    };

private:
    NN_STATIC_ASSERT( BitWidth <= 32 );
};

//! @brief 32 ビット浮動小数点数を扱う浮動小数点数フォーマットの型定義です。
typedef FloatFormat< 1, 8, 23 > FloatFormat32;
//! @brief 16 ビット浮動小数点数を扱う浮動小数点数フォーマットの型定義です。
typedef FloatFormat< 1, 5, 10 > FloatFormat16;

//--------------------------------------------------------------------------------------------------
// クラスメンバの定義

template< int TSignBit, int TExponentialBit, int TFractionalBit >
int FloatFormat< TSignBit, TExponentialBit, TFractionalBit >::Encode( float source )
{
    int intValue = 0;
    std::memcpy( &intValue, &source, sizeof( source ) );

    int bits = 0;

    if ( NN_STATIC_CONDITION( TSignBit ) )
    {
        // 符号ビットを移植します。
        bits = ( intValue >> ( FloatFormat32::SignShift - SignShift ) & SignMask );
    }
    else if ( intValue <= ( FloatFormat32::SignMask | FloatFormat32::ExponentialMask ) )
    {
        // 符号ビットを持たない型への負数の変換は、非数を除き 0 になります。
        return 0;
    }

    int exponential = intValue & FloatFormat32::ExponentialMask;
    if ( exponential == FloatFormat32::ExponentialMask )
    {
        bits |= ExponentialMask;
        if ( intValue & FloatFormat32::FractionalMask )
        {
            // 非数を無限大に変換してしまわないように別途変換します。
            if ( ( intValue & FloatFormat32::FractionalMask ) > ( FloatFormat32::FractionalMask >> 1 ) )
            {
                bits |= FractionalMask; // quiet NaN
            }
            else
            {
                bits |= FractionalMask >> 1; // signaling NaN
            }
        }
        return static_cast< int >( bits );
    }

    exponential = ( exponential >> FloatFormat32::ExponentialShift ) - FloatFormat32::ExponentialBias + ExponentialBias;
    if ( exponential >= ( ExponentialMask >> ExponentialShift ) )
    {
        if ( NN_STATIC_CONDITION( TSignBit ) )
        {
            // 無限大に変換します。
            bits |= ExponentialMask;
        }
        else
        {
            // 符号ビットを持たない型の場合は最大の有限な値に変換します。
            bits |= ExponentialMask - 1;
        }
    }
    else if ( exponential > 0 )
    {
        bits |= ( exponential << ExponentialShift ) & ExponentialMask;
        bits |= ( intValue >> ( FloatFormat32::FractionalBit - TFractionalBit ) ) & FractionalMask;
    }

    // 非正規化数はすべて 0 として扱います。
    return static_cast< int >( bits );
}

template< int TSignBit, int TExponentialBit, int TFractionalBit >
float FloatFormat< TSignBit, TExponentialBit, TFractionalBit >::Decode( int source )
{
    int intValue = 0;

    if ( NN_STATIC_CONDITION( TSignBit ) )
    {
        // 符号ビットを移植します。
        intValue = ( source & SignMask ) << ( FloatFormat32::SignShift - SignShift );
    }

    int exponential = source & ExponentialMask;
    if ( exponential == ExponentialMask )
    {
        // 無限大と非数を変換します。
        intValue |= FloatFormat32::ExponentialMask;
        intValue |= ( source << ( FloatFormat32::FractionalBit - TFractionalBit ) ) & FloatFormat32::FractionalMask;
    }
    else if ( exponential > 0 )
    {
        exponential = ( exponential >> ExponentialShift ) - ExponentialBias + FloatFormat32::ExponentialBias;
        intValue |= ( exponential << FloatFormat32::ExponentialShift ) & FloatFormat32::ExponentialMask;
        intValue |= ( source << ( FloatFormat32::FractionalBit - TFractionalBit ) ) & FloatFormat32::FractionalMask;
    }

    // 非正規化数はすべて 0 として扱います。
    float floatValue = 0.0f;
    std::memcpy( &floatValue, &intValue, sizeof( floatValue ) );
    return floatValue;
}

}} // namespace nn::util
