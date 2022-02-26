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

#include <nn/atk/atk_EffectBase.h>
#include <nn/nn_TimeSpan.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief  指定されたチャンネル数に対する Interactive 3D Audio Level 2 (I3DL2) リバーブエフェクトです。
//!
//!         EffectI3dl2Reverb は nn::audio::I3dl2ReverbType に関連する API をラッピングしたクラスです。
//!         EffectI3dl2Reverb は SoundSystem::AppendEffect で追加されたのち、
//!         SoundSystem::ClearEffect か SoundSystem::RemoveEffect によって
//!         エフェクトが削除されるまでクラスを保持しておく必要があります。
//!
//!         この型は std::atomic 型のメンバを持っているため
//!         メモリ上に確保する場合にはアライメントを満たす必要があります。
//!         @ref nn::atk::EffectBase::EffectAlignment バイト境界に配置するようにしてください。
//!
//! @see EffectBase クラス
//---------------------------------------------------------------------------
class EffectI3dl2Reverb : public EffectBase
{
    /* ------------------------------------------------------------------------
            constant definition
       ------------------------------------------------------------------------ */
public:
    static const float RoomGainMin;               //!< GetRoomGain の最小値 (-10000.0f) です。
    static const float RoomGainMax;               //!< GetRoomGain の最大値 (0.0f) です。
    static const float RoomHfGainMin;             //!< GetRoomHfGain の最小値 (-10000.0f) です。
    static const float RoomHfGainMax;             //!< GetRoomHfGain の最大値 (0.0f) です。
    static const float LateReverbHfDecayRatioMin; //!< GetLateReverbHfDecayRatio の最小値 (0.1f) です。
    static const float LateReverbHfDecayRatioMax; //!< GetLateReverbHfDecayRatio の最大値 (2.0f) です。
    static const float ReflectionsGainMin;        //!< GetReflectionsGain の最小値 (-10000.0f) です。
    static const float ReflectionsGainMax;        //!< GetReflectionsGain の最大値 (1000.0f) です。
    static const float ReverbGainMin;             //!< GetReverbGain の最小値 (-10000.0f) です。
    static const float ReverbGainMax;             //!< GetReverbGain の最大値 (2000.0f) です。
    static const float LateReverbDiffusionMin;    //!< GetLateReverbDiffusion の最小値 (0.0f) です。
    static const float LateReverbDiffusionMax;    //!< GetLateReverbDiffusion の最大値 (100.0f) です。
    static const float LateReverbDensityMin;      //!< GetLateReverbDensity の最小値 (0.0f) です。
    static const float LateReverbDensityMax;      //!< GetLateReverbDensity の最大値 (100.0f) です。
    static const float HfReferenceMin;            //!< GetHfReference の最小値 (20.0f) です。
    static const float HfReferenceMax;            //!< GetHfReference の最大値 (20000.0f) です。
    static const float DryGainMin;                //!< GetDryGain の最小値 (0.0f) です。
    static const float DryGainMax;                //!< GetDryGain の最大値 (1.0f) です。

    static const float DefaultRoomGain;                //!< ルームゲインのデフォルト値 (-1000.0f) です。
    static const float DefaultRoomHfGain;              //!< 低周波成分を基準とした高周波成分の減衰量のデフォルト値 (-1000.0f) です。
    static const float DefaultLateReverbHfDecayRatio;  //!< 低周波の残響時間に対する高周波の残響時間の比のデフォルト値 (0.8f) です。
    static const float DefaultReflectionsGain;         //!< ルームゲインを基準とした初期反射音のレベルのデフォルト値 (-700.0f) です。
    static const float DefaultReverbGain;              //!< ルームゲインを基準とした後期残響音のレベルのデフォルト値 (-500.0f) です。
    static const float DefaultLateReverbDiffusion;     //!< 後期残響音のエコー密度のデフォルト値 (50.0f) です。
    static const float DefaultLateReverbDensity;       //!< 後期残響音のモーダル密度のデフォルト値 (3.0f) です。
    static const float DefaultHfReference;             //!< 高周波の参照周波数のデフォルト値 (6000.0f) です。
    static const float DefaultDryGain;                 //!< ドライ成分のゲインのデフォルト値 (0.25f) です。

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    //! @name コンストラクタ/デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //!
    //!         エフェクトパラメータは、下記のように初期化されます。
    //!
    //!         - GetRoomGain                = DefaultRoomGain
    //!         - GetRoomHfGain              = DefaultRoomHfGain
    //!         - GetLateReverbDecayTime     = GetDefaultLateReverbDecayTime
    //!         - GetLateReverbHfDecayRatio  = DefaultLateReverbHfDecayRatio
    //!         - GetReflectionsGain         = DefaultReflectionsGain
    //!         - GetReflectionDelayTime     = GetDefaultReflectionDelayTime
    //!         - GetReverbGain              = DefaultReverbGain
    //!         - GetLateReverbDelayTime     = GetDefaultLateReverbDelayTime
    //!         - GetLateReverbDiffusion     = DefaultLateReverbDiffusion
    //!         - GetLateReverbDensity       = DefaultLateReverbDensity
    //!         - GetHfReference             = DefaultHfReference
    //!         - GetDryGain                 = DefaultDryGain
    //!         - IsEnabled                  = true
    //!
    //!         また、デフォルトのチャンネルモードは ChannelMode_6ch で、
    //!         nn::atk::ChannelIndex の小さな値から順にエフェクトが適用されます。
    //---------------------------------------------------------------------------
    EffectI3dl2Reverb() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~EffectI3dl2Reverb() NN_NOEXCEPT;

    //@}

    //! @name メモリ割り当て
    //@{

    //---------------------------------------------------------------------------
    //! @brief    EffectI3dl2Reverb が必要とするメモリのサイズを取得します。
    //!
    //!           EffectI3dl2Reverb が現在のパラメータ設定でエフェクト処理を行うために必要とする
    //!           メモリのサイズを取得します。
    //!
    //! @return   EffectI3dl2Reverb が必要とするメモリのサイズを返します。
    //---------------------------------------------------------------------------
    virtual size_t GetRequiredMemSize() const NN_NOEXCEPT NN_OVERRIDE;
    //@}

    //! @name I3DL2 リバーブエフェクト定数取得
    //@{

    //---------------------------------------------------------------------------
    //! @brief    残響時間の最小値 (100 msec) を取得します。
    //!
    //! @return   残響時間の最小値 (100 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetLateReverbDecayTimeMin() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    残響時間の最大値 (20 sec) を取得します。
    //!
    //! @return   残響時間の最大値 (20 sec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetLateReverbDecayTimeMax() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    直接音を基準とする初期反射音の遅延時間の最小値 (0 msec) を取得します。
    //!
    //! @return   直接音を基準とする初期反射音の遅延時間の最小値 (0 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetReflectionDelayTimeMin() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    直接音を基準とする初期反射音の遅延時間の最大値 (300 msec) を取得します。
    //!
    //! @return   直接音を基準とする初期反射音の遅延時間の最大値 (300 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetReflectionDelayTimeMax() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    初期反射音を基準とする後期残響音の遅延時間の最小値 (0 msec) を取得します。
    //!
    //! @return   初期反射音を基準とする後期残響音の遅延時間の最小値 (0 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetLateReverbDelayTimeMin() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    初期反射音を基準とする後期残響音の遅延時間の最大値 (100 msec) を取得します。
    //!
    //! @return   初期反射音を基準とする後期残響音の遅延時間の最大値 (100 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetLateReverbDelayTimeMax() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    残響時間のデフォルト値 (2000 msec) を取得します。
    //!
    //! @return   残響時間のデフォルト値 (2000 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetDefaultLateReverbDecayTime() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    直接音を基準とする初期反射音の遅延時間のデフォルト値 (5 msec) を取得します。
    //!
    //! @return   直接音を基準とする初期反射音の遅延時間のデフォルト値 (5 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetDefaultReflectionDelayTime() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    初期反射音を基準とする後期残響音の遅延時間のデフォルト値 (40 msec) を取得します。
    //!
    //! @return   初期反射音を基準とする後期残響音の遅延時間のデフォルト値 (40 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetDefaultLateReverbDelayTime() NN_NOEXCEPT;

    //@}


    //! @internal
    //! @param[in] pConfig :private
    //! @param[in] pOutputMixer :private
    //! @param[in] bus :private
    //! @return :private
    virtual bool AddEffect(nn::audio::AudioRendererConfig* pConfig, nn::atk::OutputMixer* pOutputMixer) NN_NOEXCEPT NN_OVERRIDE;

    //! @internal
    //! @param[in] input :private
    //! @param[in] output :private
    //! @param[in] inputCount :private
    //! @param[in] outputCount :private
    virtual void SetEffectInputOutput(const int8_t* input, const int8_t* output, int inputCount, int outputCount) NN_NOEXCEPT NN_OVERRIDE;

    //! @internal
    //! @param[in] pConfig :private
    //! @param[in] pOutputMixer :private
    virtual void RemoveEffect(nn::audio::AudioRendererConfig* pConfig, nn::atk::OutputMixer* pOutputMixer) NN_NOEXCEPT NN_OVERRIDE;

    //! @name I3DL2 リバーブエフェクト操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief    エフェクトが削除可能かどうかを取得します。
    //!
    //!           @ref SoundSystem::ClearEffect または @ref SoundSystem::RemoveEffect を呼び出す前に、
    //!           まず @ref SetEnabled で I3DL2 リバーブを無効に設定した後
    //!           本関数で削除可能であることを確認してください。
    //!           @ref SetEnabled で無効にした後すぐに削除可能になるわけではない点に注意してください。
    //!
    //! @return   エフェクトが削除可能であれば true を返します。
    //!           エフェクトがサウンドシステムに追加されていない場合は false を返します。
    //!
    //! @see      SetEnabled
    //---------------------------------------------------------------------------
    virtual bool IsRemovable() const NN_NOEXCEPT NN_OVERRIDE;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    チャンネル設定の最大値を取得します。
    //!
    //! @return   チャンネル設定の最大値です。
    //---------------------------------------------------------------------------
    virtual int GetChannelSettingCountMax() const NN_NOEXCEPT NN_OVERRIDE
    {
        return ChannelSettingCountMax;
    }

    //---------------------------------------------------------------------------
    //! @brief    I3DL2 リバーブエフェクトの最大チャンネル数を取得します。
    //!
    //! @return   I3DL2 リバーブエフェクトの最大チャンネル数です。
    //!
    //! @see  SetChannelMode
    //---------------------------------------------------------------------------
    ChannelMode GetChannelMode() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    I3DL2 リバーブエフェクトの最大チャンネル数を設定します。
    //!
    //!           この関数でチャンネル数を指定した場合、
    //!           nn::atk::ChannelIndex の小さな値から順に、設定したチャンネル数分の I3DL2 リバーブが適用されます。
    //!           I3DL2 リバーブを適用するチャンネルも併せて設定したい場合は、
    //!           この関数ではなく @ref SetChannelIndex を利用してください。
    //!           入力チャンネル数は 1/2/4/6 チャンネルのいずれかを利用可能です。
    //!           初期値は ChannelMode_6ch です。
    //!
    //! @param[in] mode チャンネル数を指定する列挙型です。
    //!
    //! @return   最大チャンネル数の設定に成功すれば true を返します。
    //!
    //! @see  GetRequiredMemSize
    //! @see  GetChannelMode
    //! @see  SoundSystem::AppendEffect
    //! @see  SetChannelIndex
    //!
    //! @details  nn::atk::SoundSystem::AppendEffect 前はチャンネルの設定に成功しますが、
    //!           nn::atk::SoundSystem::AppendEffect 後は設定に失敗し、
    //!           nn::atk::SoundSystem::RemoveEffect によってエフェクトが削除されるまで失敗します。
    //!           また、入力チャンネル数が 1/2/4/6 チャンネルいずれかでない場合も失敗します。
    //!           @ref SetChannelIndex と併用した場合は、後から呼んだ関数の設定が適用されます。
    //---------------------------------------------------------------------------
    bool SetChannelMode( ChannelMode mode ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    I3DL2 リバーブエフェクトの現在のチャンネル設定を取得します。
    //!
    //!           この関数でチャンネル数を取得する場合には、
    //!           事前に @ref GetChannelMode 関数によりチャンネル数を取得し、
    //!           その大きさ以上の nn::atk::ChannelIndex の配列を用意してください。
    //!
    //! @param[out] pChannel エフェクトが適用されているチャンネルです
    //! @param[in]  channelCount pChannel の要素数です
    //!
    //! @see GetChannelMode
    //! @see SetChannelIndex
    //!
    //! @pre
    //!   - pChannel != nullptr
    //!   - channelCount >= (GetChannelMode で取得したチャンネル数)
    //---------------------------------------------------------------------------
    virtual void GetChannelIndex(ChannelIndex* pChannel, int channelCount) const NN_NOEXCEPT NN_OVERRIDE;

    //---------------------------------------------------------------------------
    //! @brief    I3DL2 リバーブエフェクトのチャンネル設定を行います。
    //!
    //!           エフェクトを適用するチャンネル、およびエフェクトのチャンネル数を指定することができます。
    //!           この関数を利用する場合は、 @ref SetChannelMode によるチャンネル数の設定は必要ありません。
    //!           入力チャンネル数は 1/2/4/6 チャンネルのいずれかを利用可能です。
    //!
    //! @param[in] pChannel エフェクトを適用するチャンネルです。 ChannelIndex 型の配列で指定します。
    //!                     pChannel の要素数は channelMode で設定したチャンネル数以上の要素数が必要となります。
    //! @param[in] channelMode エフェクトを適用するチャンネル数を表す列挙型です。
    //!
    //! @return   チャンネルの設定に成功すれば true を返します
    //!
    //! @pre
    //!   - pChannel != nullptr
    //!   - (pChannel で指定する値) < nn::atk::ChannelIndex_Count
    //!   - pChannel で指定する値に 2 つ以上同じチャンネル番号が存在しない
    //!
    //! @see     GetChannelIndex
    //! @see     SetChannelMode
    //!
    //! @details  nn::atk::SoundSystem::AppendEffect 前はチャンネルの設定に成功しますが、
    //!           nn::atk::SoundSystem::AppendEffect 後は設定に失敗し、
    //!           nn::atk::SoundSystem::RemoveEffect によってエフェクトが削除されるまで失敗します。
    //!           また、入力チャンネル数が 1/2/4/6 チャンネルいずれかでない場合も失敗します。
    //!           @ref SetChannelMode と併用した場合は、後から呼んだ関数の設定が適用されます。
    //---------------------------------------------------------------------------
    bool SetChannelIndex(const ChannelIndex* pChannel, ChannelMode channelMode) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ルームゲインを取得します。
    //!
    //! @return   ルームゲインの値 [mB] です。
    //!
    //! @see  SetRoomGain
    //---------------------------------------------------------------------------
    float GetRoomGain() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     ルームゲインを設定します。
    //!            値の範囲は @ref RoomGainMin ～ @ref RoomGainMax です。
    //!
    //! @param[in] roomGain ルームゲインの値 [mB] です。
    //!
    //! @see  GetRoomGain
    //---------------------------------------------------------------------------
    void SetRoomGain( float roomGain ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    低周波成分を基準とした高周波成分の減衰量を取得します。
    //!
    //! @return   低周波成分を基準とした高周波成分の減衰量の値 [mB] です。
    //!
    //! @see  SetRoomHfGain
    //---------------------------------------------------------------------------
    float GetRoomHfGain() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     低周波成分を基準とした高周波成分の減衰量を設定します。
    //!            値の範囲は @ref RoomHfGainMin ～ @ref RoomHfGainMax です。
    //!
    //! @param[in] roomHfGain 低周波成分を基準とした高周波成分の減衰量の値 [mB] です。
    //!
    //! @see  GetRoomHfGain
    //---------------------------------------------------------------------------
    void SetRoomHfGain( float roomHfGain ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    残響時間を取得します。
    //!
    //! @return   残響時間の値です。
    //!
    //! @see  SetLateReverbDecayTime
    //---------------------------------------------------------------------------
    nn::TimeSpan GetLateReverbDecayTime() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     残響時間を設定します。
    //!            値の範囲は @ref GetLateReverbDecayTimeMin ～ @ref GetLateReverbDecayTimeMax です。
    //!
    //! @param[in] decayTime 残響時間の値です。
    //!
    //! @see  GetLateReverbDecayTime
    //---------------------------------------------------------------------------
    void SetLateReverbDecayTime( nn::TimeSpan decayTime ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    低周波の残響時間に対する高周波の残響時間の比を取得します。
    //!
    //! @return   低周波の残響時間に対する高周波の残響時間の比の値です。
    //!
    //! @see  SetLateReverbHfDecayRatio
    //---------------------------------------------------------------------------
    float GetLateReverbHfDecayRatio() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     低周波の残響時間に対する高周波の残響時間の比を設定します。
    //!            値の範囲は @ref LateReverbHfDecayRatioMin ～ @ref LateReverbHfDecayRatioMax です
    //!
    //! @param[in] hfDecayRatio 低周波の残響時間に対する高周波の残響時間の比の値です。
    //!
    //! @see  GetLateReverbHfDecayRatio
    //---------------------------------------------------------------------------
    void SetLateReverbHfDecayRatio( float hfDecayRatio ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ルームゲインを基準とした初期反射音のレベルを取得します。
    //!
    //! @return   ルームゲインを基準とした初期反射音のレベルの値です。
    //!
    //! @see  SetReflectionsGain
    //! @see  GetRoomGain
    //---------------------------------------------------------------------------
    float GetReflectionsGain() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     ルームゲインを基準とした初期反射音のレベルを設定します。
    //!            値の範囲は @ref ReflectionsGainMin ～ @ref ReflectionsGainMax です。
    //!
    //! @param[in] reflectionsGain ルームゲインを基準とした初期反射音のレベル [mB] です。
    //!
    //! @see  GetReflectionsGain
    //! @see  SetRoomGain
    //---------------------------------------------------------------------------
    void SetReflectionsGain( float reflectionsGain ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    直接音を基準とする初期反射音の遅延時間を取得します。
    //!
    //! @return   直接音を基準とする初期反射音の遅延時間の値です。
    //!
    //! @see  SetReflectionDelayTime
    //---------------------------------------------------------------------------
    nn::TimeSpan GetReflectionDelayTime() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     直接音を基準とする初期反射音の遅延時間を設定します。
    //!            値の範囲は @ref GetReflectionDelayTimeMin ～ @ref GetReflectionDelayTimeMax です。
    //!
    //! @param[in] delayTime 直接音を基準とする初期反射音の遅延時間の値です。
    //!
    //! @see  GetReflectionDelayTime
    //---------------------------------------------------------------------------
    void SetReflectionDelayTime( nn::TimeSpan delayTime ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ルームゲインを基準とした後期残響音のレベルを取得します。
    //!
    //! @return   ルームゲインを基準とした後期残響音のレベル [mB] です。
    //!
    //! @see  SetReverbGain
    //! @see  GetRoomGain
    //---------------------------------------------------------------------------
    float GetReverbGain() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     ルームゲインを基準とした後期残響音のレベルを設定します。
    //!            値の範囲は @ref ReverbGainMin ～ @ref ReverbGainMax です。
    //!
    //! @param[in] reverbGain ルームゲインを基準とした後期残響音のレベル [mB] です。
    //!
    //! @see  GetReverbGain
    //! @see  SetRoomGain
    //---------------------------------------------------------------------------
    void SetReverbGain( float reverbGain ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    初期反射音を基準とする後期残響音の遅延時間を取得します。
    //!
    //! @return   初期反射音を基準とする後期残響音の遅延時間の値です。
    //!
    //! @see  SetLateReverbDelayTime
    //---------------------------------------------------------------------------
    nn::TimeSpan GetLateReverbDelayTime() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     初期反射音を基準とする後期残響音の遅延時間を設定します。
    //!            値の範囲は @ref GetLateReverbDelayTimeMin ～ @ref GetLateReverbDelayTimeMax です。
    //!
    //! @param[in] delayTime 初期反射音を基準とする後期残響音の遅延時間の値です。
    //!
    //! @see  GetLateReverbDelayTime
    //---------------------------------------------------------------------------
    void SetLateReverbDelayTime( nn::TimeSpan delayTime ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    後期残響音のエコー密度を取得します。
    //!
    //! @return   後期残響音のエコー密度の値です。
    //!
    //! @see  SetLateReverbDiffusion
    //---------------------------------------------------------------------------
    float GetLateReverbDiffusion() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     後期残響音のエコー密度を設定します。
    //!            値の範囲は @ref LateReverbDiffusionMin ～ @ref LateReverbDiffusionMax です。
    //!
    //! @param[in] diffusion 後期残響音のエコー密度の値です。
    //!
    //! @see  GetLateReverbDiffusion
    //---------------------------------------------------------------------------
    void SetLateReverbDiffusion( float diffusion ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    後期残響音のモーダル密度を取得します。
    //!
    //! @return   後期残響音のモーダル密度の値です。
    //!
    //! @see  SetLateReverbDensity
    //---------------------------------------------------------------------------
    float GetLateReverbDensity() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     後期残響音のモーダル密度を設定します。
    //!            値の範囲は @ref LateReverbDensityMin ～ @ref LateReverbDensityMax です。
    //!
    //! @param[in] density 後期残響音のモーダル密度の値です。
    //!
    //! @see  GetLateReverbDensity
    //---------------------------------------------------------------------------
    void SetLateReverbDensity( float density ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    高周波の参照周波数を取得します。
    //!
    //! @return   高周波の参照周波数の値です。
    //!
    //! @see  SetHfReference
    //---------------------------------------------------------------------------
    float GetHfReference() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     高周波の参照周波数を設定します。
    //!            値の範囲は @ref HfReferenceMin ～ @ref HfReferenceMax です。
    //!
    //! @param[in] hfReference 高周波の参照周波数の値です。
    //!
    //! @see  GetHfReference
    //---------------------------------------------------------------------------
    void SetHfReference( float hfReference ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ドライ成分のゲインを取得します。
    //!
    //! @return   ドライ成分のゲインの値です。
    //!
    //! @see  SetDryGain
    //---------------------------------------------------------------------------
    float GetDryGain() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     ドライ成分のゲインを設定します。
    //!            値の範囲は @ref DryGainMin ～ @ref DryGainMax です。
    //!
    //! @param[in] dryGain ドライ成分のゲインの値です。
    //!
    //! @see  GetDryGain
    //---------------------------------------------------------------------------
    void SetDryGain( float dryGain ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    I3DL2 リバーブの有効・無効を取得します。
    //!
    //! @return   I3DL2 リバーブが有効であれば true を返します。
    //!
    //! @see  SetEnabled
    //---------------------------------------------------------------------------
    bool IsEnabled() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    I3DL2 リバーブの有効・無効を設定します。
    //!           初期値は false です。
    //!
    //!           @ref SoundSystem::ClearEffect または @ref SoundSystem::RemoveEffect を呼び出す前には、
    //!           まず本関数で無効に設定した後に、@ref IsRemovable で削除可能であることを確認する必要があります。
    //!           本関数で無効にした後すぐに削除可能になるわけではない点に注意してください。
    //!
    //! @param[in] isEnabled I3DL2リバーブの有効・無効です。
    //!
    //! @see  IsEnabled
    //---------------------------------------------------------------------------
    void SetEnabled( bool isEnabled ) NN_NOEXCEPT;

    //@}

private:
    void ResetChannelIndex() NN_NOEXCEPT;
private:
    // I3DL2 リバーブエフェクトパラメータの構造体
    struct I3dl2ReverbParam
    {
        float roomGain;
        float roomHfGain;
        nn::TimeSpan lateReverbDecayTime;
        float lateReverbHfDecayRatio;
        float reflectionsGain;
        nn::TimeSpan reflectionDelayTime;
        float reverbGain;
        nn::TimeSpan lateReverbDelayTime;
        float lateReverbDiffusion;
        float lateReverbDensity;
        float hfReference;
        float dryGain;
        bool isEnabled;

        I3dl2ReverbParam() NN_NOEXCEPT;
    };

    // 時間関連の I3DL2 リバーブパラメータのミリ秒
    static const int64_t LateReverbDecayTimeMilliSecondsMin;
    static const int64_t LateReverbDecayTimeMilliSecondsMax;
    static const int64_t ReflectionDelayTimeMilliSecondsMin;
    static const int64_t ReflectionDelayTimeMilliSecondsMax;
    static const int64_t LateReverbDelayTimeMilliSecondsMin;
    static const int64_t LateReverbDelayTimeMilliSecondsMax;

    static const int64_t DefaultLateReverbDecayTimeMilliSeconds;
    static const int64_t DefaultReflectionDelayTimeMilliSeconds;
    static const int64_t DefaultLateReverbDelayTimeMilliSeconds;

    // ChannelSettingCountMax はエフェクトが設定できる最大チャンネル数に依存
    static const int ChannelSettingCountMax = 6;

    I3dl2ReverbParam m_Param;
    std::atomic<uint64_t> m_AudioRendererUpdateCountWhenAddedI3d2lReverb;

    nn::audio::I3dl2ReverbType m_I3dl2ReverbType;
    ChannelMode      m_ChannelMode;
    ChannelIndex m_ChannelSetting[ChannelSettingCountMax];
};

} // namespace nn::atk
} // namespace nn
