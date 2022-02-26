/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * :include nn/atk/atk_Global.h
 *
 * @file atk_Global.h
 */

#pragma once

#include <nn/atk/atk_Adpcm.h>
#include <nn/atk/atk_Config.h>
#include <nn/atk/detail/atk_Log.h>
#include <nn/os/os_Tick.h>
#include <nn/atk/atk_OutputReceiver.h>

namespace nn {
namespace atk {

/* ========================================================================
        atk ライブラリ共通のグローバル定義
   ======================================================================== */

//---------------------------------------------------------------------------
//! @brief    サウンドフレーム処理から呼ばれるユーザーコールバック関数の型定義です。
//!
//!           @ref SoundSystem::SetSoundFrameUserCallback で登録することができます。
//!
//! @param[in] arg    コールバック関数のユーザー引数です。
//!
//! @see SoundSystem::SetSoundFrameUserCallback
//---------------------------------------------------------------------------
typedef void(*SoundFrameUserCallback)( uintptr_t arg );

//---------------------------------------------------------------------------
//! @briefprivate     サウンドスレッド処理から呼ばれるユーザーコールバック関数の型定義です。
//!
//!                   @ref SoundSystem::SetSoundThreadBeginUserCallback および、
//!                   @ref SoundSystem::SetSoundThreadEndUserCallback で登録することができます。
//!
//! @param[in] arg    コールバック関数のユーザー引数です。
//!
//! @see SoundSystem::SetSoundThreadUserCallback
//---------------------------------------------------------------------------
typedef void(*SoundThreadUserCallback)(uintptr_t arg);

//---------------------------------------------------------------------------
//! @brief    サウンドの再生を停止するときに呼ばれるコールバック関数の型定義です。
//!
//!           @ref SoundStartable::StartInfo で登録することができます。
//!
//! @see SoundStartable::StartInfo
//!
//---------------------------------------------------------------------------
typedef void(*SoundStopCallback)();

//---------------------------------------------------------------------------
//! @brief    サウンドの出力モードの定義です。
//!
//! @see      SoundSystem::SetOutputMode
//! @see      SoundSystem::GetOutputMode
//!
//---------------------------------------------------------------------------
enum OutputMode
{
    OutputMode_Monaural,       //!< モノラルモードです。パンは効果がありません。
    OutputMode_Stereo,     //!< ステレオモードです。
    OutputMode_Surround,   //!< サラウンドモードです。
    OutputMode_Dpl2,       //!< @internal
    OutputMode_Count       //!< 出力モードの総数です。
};

//---------------------------------------------------------------------------
//! @brief  無効なサウンド ID であることを表す値です。
//---------------------------------------------------------------------------
const uint32_t InvalidSoundId = 0xffffffff;

//---------------------------------------------------------------------------
//! @brief  無効なストリームジャンプのリージョンインデックスです。
//---------------------------------------------------------------------------
const int InvalidStreamJumpRegionIndex = -1;

//---------------------------------------------------------------------------
//! @brief  リージョン名の最大長です。
//---------------------------------------------------------------------------
const int RegionNameLengthMax = 63;

//---------------------------------------------------------------------------
//! @brief  マーカー名の最大長です。
//---------------------------------------------------------------------------
const int MarkerNameLengthMax = 63;

//---------------------------------------------------------------------------
//! @brief  プレイヤープライオリティの最小値です。
//---------------------------------------------------------------------------
const int PlayerPriorityMin = 0;

//---------------------------------------------------------------------------
//! @brief  プレイヤープライオリティの最大値です。
//---------------------------------------------------------------------------
const int PlayerPriorityMax = 127;

//---------------------------------------------------------------------------
//! @brief  チャンネルプライオリティの最小値です。
//---------------------------------------------------------------------------
const int ChannelPriorityMin = 0;

//---------------------------------------------------------------------------
//! @brief  チャンネルプライオリティの最大値です。
//---------------------------------------------------------------------------
const int ChannelPriorityMax = 127;

// ------------------------------------------------------------------------
// スピーカー

//---------------------------------------------------------------------------
//! @brief  サウンドの出力先デバイスを表す定数です。
//!
//! @see    SoundSystem::SetOutputDeviceFlag
//! @see    SoundSystem::GetOutputDeviceFlag
//!
//---------------------------------------------------------------------------
enum OutputDeviceIndex
{
    OutputDeviceIndex_Main,    //!< メイン出力 (TV 出力) です。
    OutputDeviceIndex_Count    //!< 出力先デバイスの数です。
};

//---------------------------------------------------------------------------
//! @brief  サウンドの出力先を表すインデックスです。
//!
//---------------------------------------------------------------------------
enum OutputLineIndex
{
    OutputLineIndex_Main,         //!< メイン出力 (TV への出力) を表します。
    OutputLineIndex_ReservedMax, //!< 予約済みの出力先の最大数です。これより前の出力先は設定変更ができません。
    OutputLineIndex_User0 = 16,  //!< ユーザ定義の出力 0 を表します。
    OutputLineIndex_User1,       //!< ユーザ定義の出力 1 を表します。
    OutputLineIndex_User2,       //!< ユーザ定義の出力 2 を表します。
    OutputLineIndex_User3,       //!< ユーザ定義の出力 3 を表します。
    OutputLineIndex_Max = 32      //!< 出力先の最大数です。
};

//---------------------------------------------------------------------------
//! @brief  サウンドの出力先を表すビットフラグです。
//!
//! @see SoundHandle::SetOutputLine
//! @see SoundPlayer::SetDefaultOutputLine
//! @see SequenceSoundHandle::SetTrackOutputLine
//! @see StreamSoundHandle::SetTrackOutputLine
//!
//---------------------------------------------------------------------------
enum OutputLine
{
    OutputLine_Main        = 1 << OutputLineIndex_Main,         //!< メイン出力 (TV への出力) を表します。
    OutputLine_ReservedMax = 1 << OutputLineIndex_ReservedMax, //!< 予約済みの出力先の最大数です。これより前の出力先は設定変更ができません。
    OutputLine_User0       = 1 << OutputLineIndex_User0,       //!< ユーザ定義の出力 0 を表します。
    OutputLine_User1       = 1 << OutputLineIndex_User1,       //!< ユーザ定義の出力 1 を表します。
    OutputLine_User2       = 1 << OutputLineIndex_User2,       //!< ユーザ定義の出力 2 を表します。
    OutputLine_User3       = 1 << OutputLineIndex_User3        //!< ユーザ定義の出力 3 を表します。
};

// ------------------------------------------------------------------------
// エフェクトバス

//---------------------------------------------------------------------------
//! @briefprivate
//! @brief    AUX バス ID の定義です。
//!
//! @see SoundSystem::AppendEffect
//! @see SoundHandle::SetEffectSend
//! @see SequenceSoundHandle::SetEffectSend
//! @see StreamSoundHandle::SetEffectSend
//! @see WaveSoundHandle::SetEffectSend
//! @see SoundPlayer::SetEffectSend
//! @see OutputAmbientParam
//!
//---------------------------------------------------------------------------
enum AuxBus
{
    AuxBus_A,      //!< AUX バス A です。
    AuxBus_B,      //!< AUX バス B です。
    AuxBus_C,      //!< AUX バス C です。
    AuxBus_Count   //!< AUX バスの本数です。
};

// ------------------------------------------------------------------------
// サンプルフォーマット

//---------------------------------------------------------------------------
//! @brief    サンプルフォーマットです。
//!
//!           @ref EffectBase::UpdateBuffer では、
//!           常に SampleFormat_PcmS32 が入ります。
//!
//! @see EffectBase::UpdateBuffer
//!
//---------------------------------------------------------------------------
enum SampleFormat
{
    SampleFormat_PcmS8,       //!< 符号付き 8 ビットフォーマットです。
    SampleFormat_PcmS16,      //!< 符号付き 16 ビットフォーマットです。
    SampleFormat_DspAdpcm,    //!< DSP ADPCM フォーマットです。
    SampleFormat_PcmS32       //!< 符号付き 32 ビットフォーマットです。
};

// ------------------------------------------------------------------------
// シーケンスミュート

//---------------------------------------------------------------------------
//! @brief    シーケンストラックのミュート、及びミュート解除の定義です。
//!
//! @see SequenceSoundHandle::SetTrackMute
//!
//---------------------------------------------------------------------------
enum SequenceMute
{
    //---------------------------------------------------------------------------
    //! @brief    ミュート状態を解除します。
    //---------------------------------------------------------------------------
    SequenceMute_Off,

    //---------------------------------------------------------------------------
    //! @brief    シーケンストラックをミュート状態にします。
    //!           そのトラックで発音中の音は変化しません。
    //---------------------------------------------------------------------------
    SequenceMute_NoStop,

    //---------------------------------------------------------------------------
    //! @brief    シーケンストラックをミュート状態にします。
    //!           同時に、そのトラックで発音中の音をエンベロープのリリースを発音してから、
    //!           緩やかに停止させます。
    //---------------------------------------------------------------------------
    SequenceMute_Release,

    //---------------------------------------------------------------------------
    //! @brief    シーケンストラックをミュート状態にします。
    //!           同時に、そのトラックで発音中の音を直ちに停止します。
    //---------------------------------------------------------------------------
    SequenceMute_Stop
};

//! @brief 出力先デバイスの定義です
enum OutputDevice
{
    OutputDevice_Main,                     //!< メイン出力 (テレビ出力) です。
    OutputDevice_Count                     //!< デバイスの数を表します。
};

/*!
  @brief    チャンネルの位置を表す列挙型です。
*/
enum ChannelIndex
{
    ChannelIndex_FrontLeft  = 0,  //!< 左前チャンネルを表します。
    ChannelIndex_FrontRight = 1,  //!< 右前チャンネルを表します。
    ChannelIndex_RearLeft   = 2,  //!< 左後チャンネルを表します。
    ChannelIndex_RearRight  = 3,  //!< 右後チャンネルを表します。
    ChannelIndex_FrontCenter,     //!< 中央前チャンネルを表します。
    ChannelIndex_Lfe,             //!< 低音チャンネルを表します。
    ChannelIndex_Count            //!< チャンネルの数です。
};

//! @internal
enum DrcOutChannelIndex
{
    DrcOutChannelIndex_Left = 0,
    DrcOutChannelIndex_Right = 1,
    DrcOutChannelIndex_Surround = 2, // 対応されるかわからないが念のため
    DrcOutChannelIndex_Count
};

// ------------------------------------------------------------------------
// 波形データ
// 2ch [ステレオ] 波形までサポートします。
//! @internal
static const uint32_t WaveChannelMax = 2;

//! @internal
static const uint32_t VoiceOutMax = 4;

//! @internal
static const uint32_t SeqBankMax = 4;

// ------------------------------------------------------------------------
// ステレオ時のパン動作モード
//! @internal
static const int PanModeCount = 2;

//---------------------------------------------------------------------------
//! @brief    パンの動作モードの定義です。
//!
//! @see      SoundArchive::SoundInfo
//!
//---------------------------------------------------------------------------
enum PanMode
{
    PanMode_Dual,      //!< ステレオを２本のモノラルとしてそれぞれに定位処理を行います。
    PanMode_Balance,   //!< 左右チャンネルの音量バランスを処理します。
    PanMode_Invalid    //!< 不正な種類のパンモードです。
};

// ------------------------------------------------------------------------
// パン計算の曲線

//! @internal
static const int PanCurveCount = 9;

//---------------------------------------------------------------------------
//! @brief    パンカーブの定義です。
//!
//!           パンのカーブは以下の項目の組み合わせで定義されています。
//!
//!           - 曲線の種類（平方根、三角関数、線形）
//!           - 曲線のスケール（パンが中央のときに 0dB とするか、
//!             パンが両端に振るられたときに 0dB とするか）
//!           - パンの曲線が 0dB を超えたときに、 0dB にクランプするかどうか
//!
//! @see      SoundArchive::SoundInfo
//!
//---------------------------------------------------------------------------
enum PanCurve
{
    PanCurve_Sqrt,           //!< 平方根のカーブ。中央で-3dB、両端に振り切ると0dBとなります。
    PanCurve_Sqrt0Db,        //!< 平方根のカーブ。中央で0dB、両端に振り切ると+3dBとなります。
    PanCurve_Sqrt0DbClamp,   //!< 平方根のカーブ。中央で0dB、両端に振り切ると0dBとなります。
    PanCurve_Sincos,         //!< 三角関数のカーブ。中央で-3dB、両端に振り切ると0dBとなります。
    PanCurve_Sincos0Db,      //!< 三角関数のカーブ。中央で0dB、両端に振り切ると+3dBとなります。
    PanCurve_Sincos0DbClamp, //!< 三角関数のカーブ。中央で0dB、両端に振り切ると0dBとなります。
    PanCurve_Linear,         //!< 線形のカーブ。中央で-6dB、両端に振り切ると0dBとなります。
    PanCurve_Linear0Db,      //!< 線形のカーブ。中央で0dB、両端に振り切ると+6dBとなります。
    PanCurve_Linear0DbClamp, //!< 線形のカーブ。中央で0dB、両端に振り切ると0dBとなります。
    PanCurve_Invalid         //!< 不正な種類のパンカーブです。
};

//---------------------------------------------------------------------------
//! @brief    単一再生機能の種類の定義です。
//!
//! @see      SoundArchive::SoundInfo
//!
//---------------------------------------------------------------------------
enum SinglePlayType
{
    //< 単一再生機能は無効です。
    SinglePlayType_None,
    //! 先着優先です。サウンドが再生中ならば、再生要求を中断します。
    SinglePlayType_PrioritizeOldest,
    //! 時間内先着優先です。指定された時間内でサウンドが再生中ならば、再生要求を中断します。
    SinglePlayType_PrioritizeOldestWithDuration,
    //! @deprecated
    //! こちらの API は削除予定です。代わりに @ref SinglePlayType_PrioritizeOldestWithDuration を使用してください。
    //! 時間内先着優先です。指定された時間内でサウンドが再生中ならば、再生要求を中断します。
    SinglePlayType_PrioritizeOldestEffectiveDuration = SinglePlayType_PrioritizeOldestWithDuration,
    //! 後着優先です。再生中のサウンドを止めて再生されます。
    SinglePlayType_PrioritizeNewest,
    //! 時間内後着優先です。指定された時間内で再生中のサウンドを止めて再生されます。
    SinglePlayType_PrioritizeNewestWithDuration,
    //! @deprecated
    //! こちらの API は削除予定です。代わりに @ref SinglePlayType_PrioritizeNewestWithDuration を使用してください。
    //! 時間内後着優先です。指定された時間内で再生中のサウンドを止めて再生されます。
    SinglePlayType_PrioritizeNewestEffectiveDuration = SinglePlayType_PrioritizeNewestWithDuration,
};

//---------------------------------------------------------------------------
//! @brief  ウェーブサウンドが参照する波形の種類を示す列挙型です。
//!
//---------------------------------------------------------------------------
enum WaveType
{
    WaveType_Nwwav,        //!< bfwav 形式の波形です。ループに対応しています。
    WaveType_Dspadpcm,     //!< SDK の dspadpcm.exe で出力した波形です。ループには対応していません。
    WaveType_Invalid = -1  //!< 無効な種類の波形です。
};

//---------------------------------------------------------------------------
//! @brief    音量減衰カーブの列挙型です。
//!
//!           音量減衰のカーブは 3D サウンドで使用されています。
//!           @ref Sound3DParam  を参照してください。
//!
//! @see Sound3DParam 構造体
//!
//---------------------------------------------------------------------------
enum DecayCurve
{
    DecayCurve_Log    = 1, //!< 対数のカーブです。
    DecayCurve_Linear = 2  //!< 線形のカーブです。
};

//! @internal
struct DspAdpcmParam
{
    uint16_t coef[8][2];
    uint16_t predScale;
    uint16_t yn1;
    uint16_t yn2;
};

//! @internal
const int AdshrMin = 0;

//! @internal
const int AdshrMax = 127;

// ADSHR カーブ
//! @internal
class AdshrCurve
{
public:
    AdshrCurve( uint8_t a = 0, uint8_t d = 0, uint8_t s = 0, uint8_t h = 0, uint8_t r = 0 ) NN_NOEXCEPT
    : m_Attack( a ),
      m_Decay( d ),
      m_Sustain( s ),
      m_Hold( h ),
      m_Release( r )
    {}

    //! @internal
    uint8_t GetAttack() const NN_NOEXCEPT
    {
        return m_Attack;
    }

    //! @internal
    void SetAttack(uint8_t attack) NN_NOEXCEPT
    {
        m_Attack = attack;
    }

    //! @internal
    uint8_t GetDecay() const NN_NOEXCEPT
    {
        return m_Decay;
    }

    //! @internal
    void SetDecay(uint8_t decay) NN_NOEXCEPT
    {
        m_Decay = decay;
    }

    //! @internal
    uint8_t GetSustain() const NN_NOEXCEPT
    {
        return m_Sustain;
    }

    //! @internal
    void SetSustain(uint8_t sustain) NN_NOEXCEPT
    {
        m_Sustain = sustain;
    }

    //! @internal
    uint8_t GetHold() const NN_NOEXCEPT
    {
        return m_Hold;
    }

    //! @internal
    void SetHold(uint8_t hold) NN_NOEXCEPT
    {
        m_Hold = hold;
    }

    //! @internal
    uint8_t GetRelease() const NN_NOEXCEPT
    {
        return m_Release;
    }

    //! @internal
    void SetRelease(uint8_t release) NN_NOEXCEPT
    {
        m_Release = release;
    }

private:
    uint8_t m_Attack;
    uint8_t m_Decay;
    uint8_t m_Sustain;
    uint8_t m_Hold;
    uint8_t m_Release;
    // uint8_t m_Padding[3];

};

// ------------------------------------------------------------------------
// Biquadフィルタの種類

//---------------------------------------------------------------------------
//! @brief  Biquad フィルタの種類を示す列挙体です。
//!
//!         Biquad フィルタでは、フィルタの係数を変更することにより
//!         様々な種類のフィルタを使用することができます。
//!
//!         ライブラリで用意されているプリセットの係数を利用するには、
//!         BiquadFilterType_LowPassFilter ～ BiquadFilterType_BandPassFilter2048 を指定してください。
//!
//!         これらプリセットのフィルタ係数はオーディオレンダラのレンダリングサンプルレートを 32kHz に設定した場合の値になります。
//!         オーディオレンダラのサンプルレートによってはフィルタ係数を変える必要があることにご注意ください。
//!
//!         32kHz 以外のレンダリングサンプルレートを使用する場合は、
//!         プリセットの係数ではなくユーザーで定義したフィルタ係数をご使用ください。
//!         ユーザー定義の BiquadFilter の使用方法については @ref SoundSystem::SetBiquadFilterCallback を
//!         レンダリングサンプルレートの変更につきましては @ref SoundSystem::SoundSystemParam::rendererSampleRate を参照ください。
//!
//!         Biquad フィルタを使用しない場合は BiquadFilterType_None を指定してください。
//!
//!         また、ユーザーが設計したフィルタ係数を登録する際のタイプ番号として
//!         BiquadFilterType_UserMin ～ BiquadFilterType_UserMax
//!         までの値をご利用ください。
//!
//!         Biquad フィルタは @ref SoundHandle::SetBiquadFilter
//!         関数から利用することができます。
//!
//! @see SoundHandle::SetBiquadFilter
//! @see SoundSystem::SetBiquadFilterCallback
//!
//---------------------------------------------------------------------------
enum BiquadFilterType
{
    BiquadFilterType_Inherit = -1, //!< バイナリデータの値を使用します。

    BiquadFilterType_None           = 0,  //!< Biquad フィルタを使用しません。
    BiquadFilterType_LowPassFilter  = 1,  //!< プリセットのローパスフィルタを使用します。
    BiquadFilterType_HighPassFilter = 2,  //!< プリセットのハイパスフィルタを使用します。

    //! プリセットのバンドパスフィルタ (中心周波数 512Hz) を使用します。
    BiquadFilterType_BandPassFilter512  = 3,

    //! プリセットのバンドパスフィルタ (中心周波数 1024Hz) を使用します。
    BiquadFilterType_BandPassFilter1024 = 4,

    //! プリセットのバンドパスフィルタ (中心周波数 2048Hz) を使用します。
    BiquadFilterType_BandPassFilter2048 = 5,

    BiquadFilterType_LowPassFilterNw4fCompatible48k  = 6,       //!< @briefprivate プリセットのローパスフィルタを使用します。オーディオレンダラのサンプルレートが48kHzの場合に NW4Fと同様のかかり方をするフィルタです。
    BiquadFilterType_HighPassFilterNw4fCompatible48k = 7,       //!< @briefprivate プリセットのハイパスフィルタを使用します。オーディオレンダラのサンプルレートが48kHzの場合に NW4Fと同様のかかり方をするフィルタです。
    BiquadFilterType_BandPassFilter512Nw4fCompatible48k = 8,    //!< @briefprivate プリセットのバンドパスフィルタ (中心周波数 512Hz) を使用します。オーディオレンダラのサンプルレートが48kHzの場合に NW4Fと同様のかかり方をするフィルタです。
    BiquadFilterType_BandPassFilter1024Nw4fCompatible48k = 9,   //!< @briefprivate プリセットのバンドパスフィルタ (中心周波数 1024Hz) を使用します。オーディオレンダラのサンプルレートが48kHzの場合に NW4Fと同様のかかり方をするフィルタです。
    BiquadFilterType_BandPassFilter2048Nw4fCompatible48k  = 10, //!< @briefprivate プリセットのバンドパスフィルタ (中心周波数 2048Hz) を使用します。オーディオレンダラのサンプルレートが48kHzの場合に NW4Fと同様のかかり方をするフィルタです。

    //! ユーザーが登録できるフィルタ係数の種類の最小値です。
    BiquadFilterType_UserMin = 64,

    BiquadFilterType_User0 = BiquadFilterType_UserMin,    //!< ユーザー定義 0 のフィルタを使用します。
    BiquadFilterType_User1,                                  //!< ユーザー定義 1 のフィルタを使用します。
    BiquadFilterType_User2,                                  //!< ユーザー定義 2 のフィルタを使用します。
    BiquadFilterType_User3,                                  //!< ユーザー定義 3 のフィルタを使用します。
    BiquadFilterType_User4,                                  //!< ユーザー定義 4 のフィルタを使用します。
    BiquadFilterType_User5,                                  //!< ユーザー定義 5 のフィルタを使用します。
    BiquadFilterType_User6,                                  //!< ユーザー定義 6 のフィルタを使用します。
    BiquadFilterType_User7,                                  //!< ユーザー定義 7 のフィルタを使用します。
    BiquadFilterType_User8,                                  //!< ユーザー定義 8 のフィルタを使用します。
    BiquadFilterType_User9,                                  //!< ユーザー定義 9 のフィルタを使用します。
    BiquadFilterType_User10,                                 //!< ユーザー定義 10 のフィルタを使用します。
    BiquadFilterType_User11,                                 //!< ユーザー定義 11 のフィルタを使用します。
    BiquadFilterType_User12,                                 //!< ユーザー定義 12 のフィルタを使用します。
    BiquadFilterType_User13,                                 //!< ユーザー定義 13 のフィルタを使用します。
    BiquadFilterType_User14,                                 //!< ユーザー定義 14 のフィルタを使用します。
    BiquadFilterType_User15,                                 //!< ユーザー定義 15 のフィルタを使用します。
    BiquadFilterType_User16,                                 //!< ユーザー定義 16 のフィルタを使用します。
    BiquadFilterType_User17,                                 //!< ユーザー定義 17 のフィルタを使用します。
    BiquadFilterType_User18,                                 //!< ユーザー定義 18 のフィルタを使用します。
    BiquadFilterType_User19,                                 //!< ユーザー定義 19 のフィルタを使用します。
    BiquadFilterType_User20,                                 //!< ユーザー定義 20 のフィルタを使用します。
    BiquadFilterType_User21,                                 //!< ユーザー定義 21 のフィルタを使用します。
    BiquadFilterType_User22,                                 //!< ユーザー定義 22 のフィルタを使用します。
    BiquadFilterType_User23,                                 //!< ユーザー定義 23 のフィルタを使用します。
    BiquadFilterType_User24,                                 //!< ユーザー定義 24 のフィルタを使用します。
    BiquadFilterType_User25,                                 //!< ユーザー定義 25 のフィルタを使用します。
    BiquadFilterType_User26,                                 //!< ユーザー定義 26 のフィルタを使用します。
    BiquadFilterType_User27,                                 //!< ユーザー定義 27 のフィルタを使用します。
    BiquadFilterType_User28,                                 //!< ユーザー定義 28 のフィルタを使用します。
    BiquadFilterType_User29,                                 //!< ユーザー定義 29 のフィルタを使用します。
    BiquadFilterType_User30,                                 //!< ユーザー定義 30 のフィルタを使用します。
    BiquadFilterType_User31,                                 //!< ユーザー定義 31 のフィルタを使用します。
    BiquadFilterType_User32,                                 //!< ユーザー定義 32 のフィルタを使用します。
    BiquadFilterType_User33,                                 //!< ユーザー定義 33 のフィルタを使用します。
    BiquadFilterType_User34,                                 //!< ユーザー定義 34 のフィルタを使用します。
    BiquadFilterType_User35,                                 //!< ユーザー定義 35 のフィルタを使用します。
    BiquadFilterType_User36,                                 //!< ユーザー定義 36 のフィルタを使用します。
    BiquadFilterType_User37,                                 //!< ユーザー定義 37 のフィルタを使用します。
    BiquadFilterType_User38,                                 //!< ユーザー定義 38 のフィルタを使用します。
    BiquadFilterType_User39,                                 //!< ユーザー定義 39 のフィルタを使用します。
    BiquadFilterType_User40,                                 //!< ユーザー定義 40 のフィルタを使用します。
    BiquadFilterType_User41,                                 //!< ユーザー定義 41 のフィルタを使用します。
    BiquadFilterType_User42,                                 //!< ユーザー定義 42 のフィルタを使用します。
    BiquadFilterType_User43,                                 //!< ユーザー定義 43 のフィルタを使用します。
    BiquadFilterType_User44,                                 //!< ユーザー定義 44 のフィルタを使用します。
    BiquadFilterType_User45,                                 //!< ユーザー定義 45 のフィルタを使用します。
    BiquadFilterType_User46,                                 //!< ユーザー定義 46 のフィルタを使用します。
    BiquadFilterType_User47,                                 //!< ユーザー定義 47 のフィルタを使用します。
    BiquadFilterType_User48,                                 //!< ユーザー定義 48 のフィルタを使用します。
    BiquadFilterType_User49,                                 //!< ユーザー定義 49 のフィルタを使用します。
    BiquadFilterType_User50,                                 //!< ユーザー定義 50 のフィルタを使用します。
    BiquadFilterType_User51,                                 //!< ユーザー定義 51 のフィルタを使用します。
    BiquadFilterType_User52,                                 //!< ユーザー定義 52 のフィルタを使用します。
    BiquadFilterType_User53,                                 //!< ユーザー定義 53 のフィルタを使用します。
    BiquadFilterType_User54,                                 //!< ユーザー定義 54 のフィルタを使用します。
    BiquadFilterType_User55,                                 //!< ユーザー定義 55 のフィルタを使用します。
    BiquadFilterType_User56,                                 //!< ユーザー定義 56 のフィルタを使用します。
    BiquadFilterType_User57,                                 //!< ユーザー定義 57 のフィルタを使用します。
    BiquadFilterType_User58,                                 //!< ユーザー定義 58 のフィルタを使用します。
    BiquadFilterType_User59,                                 //!< ユーザー定義 59 のフィルタを使用します。
    BiquadFilterType_User60,                                 //!< ユーザー定義 60 のフィルタを使用します。
    BiquadFilterType_User61,                                 //!< ユーザー定義 61 のフィルタを使用します。
    BiquadFilterType_User62,                                 //!< ユーザー定義 62 のフィルタを使用します。
    BiquadFilterType_User63,                                 //!< ユーザー定義 63 のフィルタを使用します。

    //! ユーザーが登録できるフィルタ係数の種類の最大値です。
    BiquadFilterType_UserMax = BiquadFilterType_User63,

    //! フィルタ係数の種類のデータ上の最小値です。
    BiquadFilterType_DataMin = BiquadFilterType_None,

    //! フィルタ係数の種類の最小値です。
    BiquadFilterType_Min = BiquadFilterType_Inherit,

    //! フィルタ係数の種類の最大値です。
    BiquadFilterType_Max = BiquadFilterType_UserMax
};

//---------------------------------------------------------------------------
//! @briefprivate
//!
//! @brief    SRC タイプ (サンプルレートコンバータの補間方法) の定義です。
//!
//!           サンプルレート変換時の補間方法を示します。
//!
//! @see      SoundSystem::SetSampleRateConverterType
//! @see      SoundSystem::GetSampleRateConverterType
//!
//---------------------------------------------------------------------------
enum SampleRateConverterType
{
    SampleRateConverterType_None,      //!< 補間しません。
    SampleRateConverterType_Linear,    //!< 線形補間します。
    SampleRateConverterType_4Tap       //!< ４点補間します。
};

/*!
    @internal
    @brief  各チャンネルのゲインを格納する構造体です。
*/
struct OutputMix
{
    //! @briefprivate
    //! 最大チャンネル数です。
    static const int ChannelCountMax = 24;

    //! 各チャンネルのゲイン（1.0 が等倍）
    float channelGain[ChannelCountMax];

    void Initialize()
    {
        for(int i = 0; i < ChannelCountMax; i++)
        {
            channelGain[i] = 0.0f;
        }
    }
};

// (TODO: Voice の公開時に、これも公開する)
/*!
    @internal
    @brief  サンプルデータが格納されているバッファの情報を格納する構造体です。
*/
struct WaveBuffer
{
    /*!
      @brief    バッファの状態を表す列挙型です。
    */
    enum Status
    {
        Status_Free, //!< 初期状態（一度もAppendWaveBuffer で追加されていない状態）を表します。ボイスの管理外にあります。
        Status_Wait, //!< ボイスのバッファリストにリンクされ、再生待ちの状態を表します。ボイスが管理しています。
        Status_Play, //!< 現在再生中の状態を表します。ボイスが管理しています。
        Status_Done  //!< 再生完了状態を表します。ボイスの管理外にあります。
    };

    // ユーザ用
    const void * bufferAddress;   //!< サンプルデータ格納先アドレス。必須です。ボイスに登録後は変更しないで下さい。
    size_t bufferSize;
    size_t sampleLength;             //!< バッファ内のサンプル長。必須です。ボイスに登録後は変更しないで下さい。
    detail::position_t sampleOffset;

    const AdpcmContext * pAdpcmContext; //!< Adpcm コンテクストのアドレス。ボイスに登録後、バッファの再生が終了するまで変更しないで下さい。

    void* userParam;               //!< ユーザーパラメータ。任意です。

    bool loopFlag;                //!< ループフラグ。任意です。
    Status status;                //!< バッファの状態。ライブラリが使用するため、変更しないで下さい。
//    uint16_t bufferId;                 //!< バッファ管理用 ID。ライブラリが使用するため、変更しないでください。

    struct WaveBuffer * next;     //!< 次のバッファ構造体のアドレス。ライブラリが使用するため、変更しないで下さい。

    /*!
        @brief      コンストラクタです。
     */
    WaveBuffer() NN_NOEXCEPT : status( Status_Free )
    {
        Initialize();
    }
    ~WaveBuffer() NN_NOEXCEPT
    {
        NN_SDK_ASSERT( status == Status_Free || status == Status_Done );
    }

    void Initialize() NN_NOEXCEPT
    {
        bufferAddress = NULL;
        bufferSize = 0;
        sampleLength = 0;
        sampleOffset = 0;
        pAdpcmContext = NULL;
        userParam = NULL;
        loopFlag = false;
        status = Status_Free;
        next = NULL;
    }

    void Dump() NN_NOEXCEPT
    {
        NN_DETAIL_ATK_INFO("WaveBuffer(%p) addr(%p) size(%zu) len(%zu) ofs(%lld) loop(%d) status(%d) next(%p)\n",
            this, bufferAddress, bufferSize, sampleLength, sampleOffset, loopFlag, status, next);
    }
}; // WaveBuffer

//---------------------------------------------------------------------------
//! @brief  Biquad フィルタ係数の構造体です。
//!
//!         各係数については、Nintendo SDK リファレンスマニュアルの
//!         BiquadFilterParameter の説明をご参照ください。
//!         各係数がどの係数に対応しているかは、それぞれの係数の説明に記載されています。
//!
//---------------------------------------------------------------------------
struct BiquadFilterCoefficients
{
    int16_t b0; //!< numerator[0] の係数を表します。
    int16_t b1; //!< numerator[1] の係数を表します。
    int16_t b2; //!< numerator[2] の係数を表します。
    int16_t a1; //!< denominator[0] の係数を表します。
    int16_t a2; //!< denominator[1] の係数を表します。
};


//---------------------------------------------------------------------------
//! @internal
//! @brief  波形情報を格納した構造体です。
//!
//---------------------------------------------------------------------------
struct FinalMixData
{
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //!
    //---------------------------------------------------------------------------
    FinalMixData() NN_NOEXCEPT
    : frontLeft( NULL )
    , frontRight( NULL )
    , rearLeft( NULL )
    , rearRight( NULL )
    , frontCenter( NULL )
    , lowFrequencyEffect( NULL )
    , sampleCount( 0 )
    , channelCount( 0 )
    {
    }

    int32_t* frontLeft;     //!< フロント左チャンネルの波形です。
    int32_t* frontRight;    //!< フロント右チャンネルの波形です。
    int32_t* rearLeft;      //!< リア左チャンネルの波形です。
    int32_t* rearRight;     //!< リア右チャンネルの波形です。
    int32_t* frontCenter;   //!< フロントセンターチャンネルの波形です。
    int32_t* lowFrequencyEffect; //!< LFE（低域効果音）チャンネルの波形です。

    uint16_t sampleCount;    //!< サンプル数です。
    uint16_t channelCount;   //!< チャンネル数です。
};

//! @briefprivate
//! @deprecated SoundSystemParam::rendererSampleRate で 48kHz を指定することで制約もなく 48kHz レンダリングできるためそちらを使用してください。
//!             互換性のために残されていますが、これらの API はすでに使用できない状態になっていて、今後削除される予定です。
enum VoiceRendererType
{
    VoiceRendererType_Sdk,     //!< AXSetDefaultRenderer で設定したレンダラです。
    VoiceRendererType_SdkDsp, //!< DSP レンダラです。
    VoiceRendererType_SdkPpc, //!< PPC レンダラです。
    VoiceRendererType_Nw,      //!< NW レンダラです。48KHz でレンダリングされます。
    VoiceRendererType_Count    //!< レンダラの数です。
};

//! @brief 各スピーカーに対する音量配分の設定方法を示す列挙型です。
enum MixMode
{
    MixMode_Pan,                              //!< Pan と SurroundPan を指定する音量配分の設定です。
    MixMode_Mixparameter,                     //!< 内部用機能のため使用しないでください。
    MixMode_MixVolume = MixMode_Mixparameter, //!< 全てのチャンネルの音量を指定する音量配分の設定です。
    MixMode_Count                             //!< 指定可能な音量配分の設定の数です。
};

#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
#pragma warning(push)
#pragma warning(disable:4201)
#endif
//! @brief  5.1chの各スピーカーの音量を示す構造体です。
struct MixVolume
{
    union
    {
        struct
        {
            float frontLeft;          //!< 前方左チャンネルの音量です。
            float frontRight;         //!< 前方右チャンネルの音量です。
            float rearLeft;           //!< 後方左チャンネルの音量です。
            float rearRight;          //!< 後方右チャンネルの音量です。
            float frontCenter;        //!< 前方中央チャンネルの音量です。
            float lowFrequencyEffect; //!< LFE(低域効果音)チャンネルの音量です。
        };
        float channel[ChannelIndex_Count]; //!< 各チャンネルの音量です。
    };
};

//---------------------------------------------------------------------------
//! @briefprivate
//!
//! @brief  サラウンドボリューム(5.1chの各スピーカーの音量)の構造体です。
//!
//---------------------------------------------------------------------------
struct MixParameter
{
    union
    {
        struct
        {
            float fL;        //!< フロント左チャンネルのパラメータです。
            float fR;        //!< フロント右チャンネルのパラメータです。
            float rL;        //!< リア左チャンネルのパラメータです。
            float rR;        //!< リア右チャンネルのパラメータです。
            float fC;        //!< フロントセンターチャンネルのパラメータです。
            float lfe;       //!< LFE（低域効果音）チャンネルのパラメータです。
        };

        float ch[ChannelIndex_Count];
    };

    /* ctor */ MixParameter() NN_NOEXCEPT
    {
        fL = fR = rL = rR = fC = lfe = 1.0f;
    }

    /* ctor */ MixParameter( float _fL, float _fR, float _rL, float _rR, float _fC, float _lfe ) NN_NOEXCEPT
     : fL ( _fL ), fR ( _fR ), rL ( _rL ), rR ( _rR ), fC ( _fC ), lfe( _lfe )
    {}
};
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
#pragma warning(pop)
#endif

//! @briefprivate
//! @brief 更新の頻度を表します
enum UpdateType
{
    UpdateType_AudioFrame, //!< オーディオフレームの粒度で更新を行います
    UpdateType_GameFrame   //!< ゲームフレームの粒度で更新を行います
};

//! @briefprivate
//! @brief SoundHandle などにおけるポーズの設定を表します
enum PauseMode
{
    //! NW4F の nw::snd との互換性を保ったポーズの設定を表します。
    //!
    //! 一時停止の際、 SoundArchivePlayer::Update を呼んだ回数が fadeFrames 回でフェードアウトのボリュームが 0 になり、
    //! fadeFrames + 1 回でポーズが完了します。
    //! 再開時は fadeFrames 回でフェードインが完了します。
    //! また、 fadeFrames が 0 の場合は fadeFrames に 1 を設定した場合と同じ挙動になります。
    //!
    //! フェードアウトのボリュームが 0 に設定されるタイミングと一時停止が完了するタイミングが異なるため、
    //! fadeFrames に 0 または 1 を指定した場合でも、 2 回 SoundArchivePlayer::Update を呼び出すまで一時停止が完了せず、
    //! 無音状態で再生位置が進んでしまう点にご注意ください。
    //!
    PauseMode_Nw4fSndCompatible,

    //! フェードアウトの完了と同時にポーズが完了する設定を表します。
    //!
    //! 一時停止の際、 SoundArchivePlayer::Update を呼んだ回数が fadeFrames 回になった時点でポーズが完了します。
    //! 再開時は fadeFrames 回でフェードインが完了します。
    //! また、 fadeFrames が 0 の場合は fadeFrames に 1 を設定した場合と同じ挙動になります。
    //!
    //! PauseMode_Nw4fSndCompatible を利用したポーズは最低でも SoundArchivePlayer::Update の更新間隔分再生位置が進んでしまうため、
    //! Pause を利用した短い音源の頭出しなどをする場合に、こちらを利用してください。
    //!
    PauseMode_PauseImmediately,

    //! デフォルトのポーズの設定です
    PauseMode_Default = PauseMode_Nw4fSndCompatible
};

//---------------------------------------------------------------------------
//! @brief CircularBufferSink の動作状態を表します。
//!
//! @see SoundSystem::SoundSystemParam::enableCircularBufferSink
//! @see SoundSystem::StopCircularBufferSink
//! @see SoundSystem::StartCircularBufferSink
//---------------------------------------------------------------------------
enum CircularBufferSinkState
{
    //! CircularBufferSink の動作が無効である状態を表します
    //! SoundSystem の初期化前や、 CircularBufferSink を有効にせずに SoundSystem を初期化した場合にこの状態となります
    CircularBufferSinkState_Invalid,
    //! CircularBufferSink の動作中を表します
    CircularBufferSinkState_Started,
    //! CircularBufferSink の動作が停止していることを表します
    CircularBufferSinkState_Stopped
};

//---------------------------------------------------------------------------
//! @briefprivate
//! @brief 特定のボリューム設定を無視するかどうかを表すビットフラグです。
//!
//! @see SoundHandle::SetVolumeThroughMode
//---------------------------------------------------------------------------
enum VolumeThroughModeBitFlag
{
    VolumeThroughMode_Binary = 1 << 0 //!< バイナリで指定されたボリューム設定を無視します
};

//---------------------------------------------------------------------------
//! @brief ファイルロードを行う際の fs アクセス優先度です。
//!
//! @see SoundSystem::SoundSystemParam::taskThreadFsPriority
//---------------------------------------------------------------------------
enum FsPriority
{
    FsPriority_RealTime, //!< 通常より高い fs アクセス優先度を表します。
    FsPriority_Normal, //!< 通常の fs アクセス優先度を表します。
    FsPriority_Low //!< 通常より低い fs アクセス優先度を表します。
};

/* ========================================================================
        非公開
   ======================================================================== */

namespace detail {

// ------------------------------------------------------------------------
// 波形データ
struct DspAdpcmLoopParam
{
    uint16_t loopPredScale;
    uint16_t loopYn1;
    uint16_t loopYn2;
};

struct WaveInfo
{
    SampleFormat sampleFormat;
    bool loopFlag;
    int channelCount;
    int sampleRate;
    position_t loopStartFrame;
    position_t loopEndFrame;
    position_t originalLoopStartFrame;
    size_t dataSize;

    struct ChannelParam
    {
        const void* dataAddress;
        int dataSize;
        DspAdpcmParam adpcmParam;
        DspAdpcmLoopParam adpcmLoopParam;
    } channelParam[ WaveChannelMax ];

    void Dump() NN_NOEXCEPT
    {
    #if defined(NN_SDK_BUILD_DEBUG) || defined(NN_SDK_BUILD_DEVELOP) // Dump 用途
        NN_DETAIL_ATK_INFO("[WaveInfo] fmt(%d) loop(%d) ch(%d) rate(%d) LS(%d) LE(%d) OLS(%d)\n",
                sampleFormat, loopFlag, channelCount, sampleRate,
                loopStartFrame, loopEndFrame, originalLoopStartFrame );
        for ( int i = 0; i < channelCount; i++ )
        {
            const ChannelParam& p = channelParam[i];
            const DspAdpcmParam& adpcm = p.adpcmParam;
            const DspAdpcmLoopParam& loop = p.adpcmLoopParam;
            NN_DETAIL_ATK_INFO("  %d/%d addr(%p)\n       ps(%5d)  yn1(%5d)  yn2(%5d)\n      lps(%5d) lyn1(%5d) lyn2(%5d)\n",
                    i, channelCount - 1, p.dataAddress, adpcm.predScale, adpcm.yn1, adpcm.yn2,
                    loop.loopPredScale, loop.loopYn1, loop.loopYn2 );
            NN_DETAIL_ATK_INFO("      coef %5d %5d %5d %5d %5d %5d %5d %5d\n",
                    adpcm.coef[0][0], adpcm.coef[0][1], adpcm.coef[1][0], adpcm.coef[1][1],
                    adpcm.coef[2][0], adpcm.coef[2][1], adpcm.coef[3][0], adpcm.coef[3][1] );
            NN_DETAIL_ATK_INFO("           %5d %5d %5d %5d %5d %5d %5d %5d\n",
                    adpcm.coef[4][0], adpcm.coef[4][1], adpcm.coef[5][0], adpcm.coef[5][1],
                    adpcm.coef[6][0], adpcm.coef[6][1], adpcm.coef[7][0], adpcm.coef[7][1] );
        }
    #endif // NN_SDK_BUILD_DEBUG || NN_SDK_BUILD_DEVELOP
    }
};

const int DefaultBusCount = AuxBus_Count + 1; // Main + AuxA, AuxB, AuxC

struct OutputParam
{
    float volume;
    uint32_t mixMode;
    MixParameter mixParameter[ WaveChannelMax ];
    float pan;
    float span;
    float send[ DefaultBusCount ];

    void Initialize() NN_NOEXCEPT
    {
        volume = 1.0f;
        mixMode = 0;
        pan = 0.0f;
        span = 0.0f;
        mixMode = 0;

        for ( int i = 0; i < DefaultBusCount; ++i )
        {
            send[i] = 0.0f;
        }
    }
};

struct OutputBusMixVolume
{
    float volume[ WaveChannelMax ][ OutputMix::ChannelCountMax ];

    void Initialize() NN_NOEXCEPT
    {
        for ( int i = 0; i < WaveChannelMax; i++ )
        {
            for ( int j = 0; j < OutputMix::ChannelCountMax; j++)
            {
                volume[i][j] = 1.0f;
            }
        }
    }
};

enum StreamFileType
{
    StreamFileType_Bfstm,
    StreamFileType_Opus
};

enum DecodeMode
{
    DecodeMode_Invalid = -1,
    DecodeMode_Default = 0,
    DecodeMode_Cpu,
    DecodeMode_Accelerator
};

//---------------------------------------------------------------------------
//! @briefprivate
//! @brief  オーディオライブラリ内部で保持しているパフォーマンス情報の最大数です。
//!
//!         @ref ProfileReader::Read の maxCount 引数の最大値としても利用可能です。
//!
//! @see ProfileReader::Read
//!
//---------------------------------------------------------------------------
static const int MaxPerformanceInfoCount = 32;

//---------------------------------------------------------------------------
//! @briefprivate
//! @brief  オーディオライブラリ内部で保持しているパフォーマンスフレーム用バッファの最大数です。
//!         ライブラリ内部では合計 MaxPerformanceInfoCount * MaxPerformanceBufferCount 個のパフォーマンス情報バッファを保持することになります。
//---------------------------------------------------------------------------
static const int MaxPerformanceBufferCount = 3;

enum VoiceState
{
    VoiceState_Play,  //!< 再生指令状態を表します。
    VoiceState_Stop,  //!< 停止状態を表します。
    VoiceState_Pause  //!< 一時停止状態を表します。再生を再開する際は、SetState により再生状態を設定して下さい。
};

struct VoiceParam
{
    float                 m_Volume;               // ボリューム
    float                 m_Pitch;                // ピッチ（SampleRate に対する比）
    OutputMix           m_TvMix;

    bool                m_MonoFilterFlag;
    bool                m_BiquadFilterFlag;

    BiquadFilterCoefficients m_BiquadFilterCoefficients;
    uint16_t                 m_MonoFilterCutoff;     // LPF のカットオフ周波数
    uint8_t                  m_InterpolationType;    // 補間タイプ

    void Initialize() NN_NOEXCEPT;
};

struct VoiceInfo
{
    VoiceState voiceState;
    WaveBuffer::Status waveBufferStatus;
    void* waveBufferTag;
    uint32_t playPosition;
    void* userId;
};

struct SoundInstanceConfig
{
    bool isBusMixVolumeEnabled;
    bool isVolumeThroughModeEnabled;
    int busCount;
};

    static const int FilePathMax  = 511 + 128;
    static const int PpcIoBufferAlign = nn::audio::BufferAlignSize;
    static const int CacheBlockSize   = 32;

    static const int StreamBlockCountMin = 2; // ストリームバッファのブロック数として設定できる最小の数

} // namespace nn::atk::detail

} // namespace nn::atk
} // namespace nn
