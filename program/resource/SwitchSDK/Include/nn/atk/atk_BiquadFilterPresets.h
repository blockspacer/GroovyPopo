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

#include <nn/nn_Macro.h>
#include <nn/atk/atk_BiquadFilterCallback.h>

namespace nn {
namespace atk {
namespace detail {

/* ------------------------------------------------------------------------
        BiquadFilterLpf - ローパスフィルタ
        （1/16オクターブ間隔、チェビシェフ近似）
   ------------------------------------------------------------------------ */
class BiquadFilterLpf : public BiquadFilterCallback
{
public:
    BiquadFilterLpf() NN_NOEXCEPT {}
    virtual void GetCoefficients( Coefficients* pOutValue, int type, float value ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    static const int CoefficientsTableSize = 112;
    static const Coefficients CoefficientsTable32000[ CoefficientsTableSize ];
};

/* ------------------------------------------------------------------------
        BiquadFilterHpf - ハイパスフィルタ
        （1/16オクターブ間隔、チェビシェフ近似）
   ------------------------------------------------------------------------ */
class BiquadFilterHpf : public BiquadFilterCallback
{
public:
    BiquadFilterHpf() NN_NOEXCEPT {}
    virtual void GetCoefficients( Coefficients* pOutValue, int type, float value ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    static const int CoefficientsTableSize = 97;
    static const Coefficients CoefficientsTable32000[ CoefficientsTableSize ];
};

/* ------------------------------------------------------------------------
        BiquadFilterBpf512 - バンドパスフィルタ
        （中心周波数 512Hz、チェビシェフ近似）
   ------------------------------------------------------------------------ */
class BiquadFilterBpf512 : public BiquadFilterCallback
{
public:
    BiquadFilterBpf512() NN_NOEXCEPT {}
    virtual void GetCoefficients( Coefficients* pOutValue, int type, float value ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    static const int CoefficientsTableSize = 122;
    static const Coefficients CoefficientsTable32000[ CoefficientsTableSize ];
};

/* ------------------------------------------------------------------------
        BiquadFilterBpf1024 - バンドパスフィルタ
        （中心周波数 1024Hz、チェビシェフ近似）
   ------------------------------------------------------------------------ */
class BiquadFilterBpf1024 : public BiquadFilterCallback
{
public:
    BiquadFilterBpf1024() NN_NOEXCEPT {}
    virtual void GetCoefficients( Coefficients* pOutValue, int type, float value ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    static const int CoefficientsTableSize = 93;
    static const Coefficients CoefficientsTable32000[ CoefficientsTableSize ];
};

/* ------------------------------------------------------------------------
        BiquadFilterBpf2048 - バンドパスフィルタ
        （中心周波数 2048Hz、チェビシェフ近似）
   ------------------------------------------------------------------------ */
class BiquadFilterBpf2048 : public BiquadFilterCallback
{
public:
    BiquadFilterBpf2048() NN_NOEXCEPT {}
    virtual void GetCoefficients( Coefficients* pOutValue, int type, float value ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    static const int CoefficientsTableSize = 93;
    static const Coefficients CoefficientsTable32000[ CoefficientsTableSize ];
};

/* ------------------------------------------------------------------------
        BiquadFilterLpfNw4fCompatible48k - ローパスフィルタ
        （オーディオレンダラのサンプルレートが48kHzの場合に、NW4Fと同様のかかり方をするローパスフィルタ）
   ------------------------------------------------------------------------ */
class BiquadFilterLpfNw4fCompatible48k : public BiquadFilterCallback
{
public:
    BiquadFilterLpfNw4fCompatible48k() NN_NOEXCEPT {}
    virtual void GetCoefficients( Coefficients* pOutValue, int type, float value ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    static const int CoefficientsTableSize = 112;
    static const Coefficients CoefficientsTable48000[ CoefficientsTableSize ];
};

/* ------------------------------------------------------------------------
        BiquadFilterHpfNw4fCompatible48k - ハイパスフィルタ
        （オーディオレンダラのサンプルレートが48kHzの場合に、NW4Fと同様のかかり方をするハイパスフィルタ）
   ------------------------------------------------------------------------ */
class BiquadFilterHpfNw4fCompatible48k : public BiquadFilterCallback
{
public:
    BiquadFilterHpfNw4fCompatible48k() NN_NOEXCEPT {}
    virtual void GetCoefficients( Coefficients* pOutValue, int type, float value ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    static const int CoefficientsTableSize = 97;
    static const Coefficients CoefficientsTable48000[ CoefficientsTableSize ];
};

/* ------------------------------------------------------------------------
        BiquadFilterBpf512Nw4fCompatible48k - バンドパスフィルタ
        （オーディオレンダラのサンプルレートが48kHzの場合に、NW4Fと同様のかかり方をするハイパスフィルタ）
   ------------------------------------------------------------------------ */
class BiquadFilterBpf512Nw4fCompatible48k : public BiquadFilterCallback
{
public:
    BiquadFilterBpf512Nw4fCompatible48k() NN_NOEXCEPT {}
    virtual void GetCoefficients( Coefficients* pOutValue, int type, float value ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    static const int CoefficientsTableSize = 122;
    static const Coefficients CoefficientsTable48000[ CoefficientsTableSize ];
};

/* ------------------------------------------------------------------------
        BiquadFilterBpf1024Nw4fCompatible48k - バンドパスフィルタ
        （オーディオレンダラのサンプルレートが48kHzの場合に、NW4Fと同様のかかり方をするハイパスフィルタ）
   ------------------------------------------------------------------------ */
class BiquadFilterBpf1024Nw4fCompatible48k : public BiquadFilterCallback
{
public:
    BiquadFilterBpf1024Nw4fCompatible48k() NN_NOEXCEPT {}
    virtual void GetCoefficients( Coefficients* pOutValue, int type, float value ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    static const int CoefficientsTableSize = 93;
    static const Coefficients CoefficientsTable48000[ CoefficientsTableSize ];
};

/* ------------------------------------------------------------------------
        BiquadFilterBpf2048Nw4fCompatible48k - バンドパスフィルタ
        （オーディオレンダラのサンプルレートが48kHzの場合に、NW4Fと同様のかかり方をするハイパスフィルタ）
   ------------------------------------------------------------------------ */
class BiquadFilterBpf2048Nw4fCompatible48k : public BiquadFilterCallback
{
public:
    BiquadFilterBpf2048Nw4fCompatible48k() NN_NOEXCEPT {}
    virtual void GetCoefficients( Coefficients* pOutValue, int type, float value ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    static const int CoefficientsTableSize = 93;
    static const Coefficients CoefficientsTable48000[ CoefficientsTableSize ];
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

