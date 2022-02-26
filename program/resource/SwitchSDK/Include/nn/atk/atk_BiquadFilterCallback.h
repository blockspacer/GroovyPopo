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

#include <nn/atk/atk_Global.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief  Biquad フィルタを表す抽象クラスです。
//!
//!         BiquadFilterCallback クラスから継承し、
//!         ユーザー側で Biquad フィルタを実装することができます。
//!
//!         BiquadFilterCallback の使用方法については、
//!         @ref SoundSystem::SetBiquadFilterCallback 関数を参照してください。
//!
//! @see SoundSystem::SetBiquadFilterCallback
//!
//---------------------------------------------------------------------------
class BiquadFilterCallback
{
public:
    //---------------------------------------------------------------------------
    //! @brief  Biquad フィルタの計算に使用する係数の構造体です。
    //!
    //! @see GetCoefficients
    //---------------------------------------------------------------------------
    typedef BiquadFilterCoefficients Coefficients;

    //---------------------------------------------------------------------------
    //! @brief  デストラクタです。
    //!
    //---------------------------------------------------------------------------
    virtual ~BiquadFilterCallback() NN_NOEXCEPT {}

    //---------------------------------------------------------------------------
    //! @brief  Biquad フィルタの係数を返します。
    //!
    //!         nn::atk ライブラリ内部の Biquad フィルタ係数が必要なタイミングで呼び出されます。
    //!         BiquadFilterCallback を継承したクラスで、
    //!         この関数にフィルタ係数を実装する必要があります。
    //!
    //!         type は、コールバッククラスを @ref SoundSystem::SetBiquadFilterCallback
    //!         で登録したときのタイプ番号です。
    //!         @ref BiquadFilterType に記述されている BIQUAD_FILTER_TYPE_USER_MIN ～
    //!         BIQUAD_FILTER_TYPE_USER_MAX までの値が入ります。
    //!
    //!         value はフィルタのかかり具合を表す 0.0f ～ 1.0f の値です。
    //!         この値に応じたフィルタ係数を coef に格納するように実装します。
    //!
    //! @param[out] pOutValue    計算したフィルタ係数を格納するための構造体です。
    //! @param[in] type     フィルタの種類です
    //!                     (BIQUAD_FILTER_TYPE_USER_MIN ～BIQUAD_FILTER_TYPE_USER_MAX)。
    //! @param[in] value    フィルタのかかり具合です (0.0f ～ 1.0f)
    //!
    //! @see SoundSystem::SetBiquadFilterCallback
    //! @see BiquadFilterType
    //!
    //---------------------------------------------------------------------------
    virtual void GetCoefficients( Coefficients* pOutValue, int type, float value ) const NN_NOEXCEPT = 0;
};

} // namespace nn::atk
} // namespace nn

