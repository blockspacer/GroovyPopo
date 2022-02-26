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

#include <nn/atk/atk_Config.h>
#ifdef NN_ATK_CONFIG_ENABLE_DEV

#include <nn/atk/viewer/atk_Config.h>
#include <nn/atk/viewer/detail/atk_SequenceVariable.h>

namespace nn {
namespace atk {
namespace viewer {

//---------------------------------------------------------------------------
//! @brief  Win 向けサウンドのパラメータ構造体です。
//---------------------------------------------------------------------------
struct SoundParameters
{
    float  volume;                //!< 音量です。
    float  pitch;                 //!< ピッチです。

    uint32_t  startOffset;           //!< 再生開始位置です。

    float  pan;                   //!< パンです。
    float  mainPan;               //!< メイン出力のパンです。

    float  surroundPan;           //!< サラウンドパンです。
    float  mainSurroundPan;       //!< メイン出力のサラウンドパンです。

    MixMode mixMode;              //!< 音量配分設定です。
    MixVolume mixVolume;          //!< 5.1chの各スピーカーの音量です。

    float  lowPassFilterFrequency;  //!< LPF 周波数です。
    int32_t  biquadFilterType;      //!< Biquad フィルタの種類です。
    float  biquadFilterValue;       //!< Biquad フィルタのかかり具合です。

    float  mainSend;                                          //!< メインバスのセンド量です。
    float  effectSend[Limits::EffectBusCountMax];             //!< 各エフェクトバスのセンド量です。
    float  mainMainSend;                                      //!< メイン出力 メインバスのセンド量です。
    float  mainEffectSend[Limits::EffectBusCountMax];         //!< メイン出力 各エフェクトバスのセンド量です。

    float  mainOutVolume;                                     //!< メイン出力の音量です。

    bool isMainOutEnabled;                                  //!< メイン出力の有効化フラグです。
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t _padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif

    bool tracksMute[Limits::SequenceTrackCountMax];      //!< シーケンストラックのミュートフラグです。
    float  tracksVolume[Limits::SequenceTrackCountMax];    //!< シーケンストラックの音量です。

    // 再生開始時にシーケンス変数を設定するためのコンテナ
    //! @internal
    detail::SequenceVariableContainer localVariables;
    //! @internal
    detail::SequenceVariableContainer trackVariables[ detail::SequenceVariableTraits::TrackCount ];

    SoundParameters() NN_NOEXCEPT :
      volume(1.f),
      pitch(1.f),
      startOffset(0),
      pan(0.f),
      mainPan(0.f),
      surroundPan(0.f),
      mainSurroundPan(0.f),
      mixMode(MixMode_Pan),
      lowPassFilterFrequency(0.f),
      biquadFilterType(BiquadFilterType_Inherit),
      biquadFilterValue(0.5f),
      mainSend(0.f),
      mainMainSend(0.f),
      mainOutVolume(1.f),
      isMainOutEnabled(true)
      {
          for (uint32_t index = 0; index < Limits::EffectBusCountMax; ++index)
          {
              effectSend[index] = 0.f;
              mainEffectSend[index] = 0.f;
          }

          for (uint32_t index = 0; index < Limits::SequenceTrackCountMax; ++index)
          {
              tracksMute[index] = false;
              tracksVolume[index] = 1.0f;
          }

          for(int index = 0; index < ChannelIndex_Count; index++)
          {
              mixVolume.channel[index] = 0.0f;
          }
      }
};

} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
