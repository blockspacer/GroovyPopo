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
//! @brief  指定されたチャンネル数に対するリバーブエフェクトです。
//!
//!         EffectReverb は nn::audio::ReverbType に関連する API をラッピングしたクラスです。
//!         EffectReverb は SoundSystem::AppendEffect で追加されたのち、
//!         SoundSystem::ClearEffect か SoundSystem::RemoveEffect によって
//!         エフェクトが削除されるまでクラスを保持しておく必要があります。
//!
//!         この型は std::atomic 型のメンバを持っているため
//!         メモリ上に確保する場合にはアライメントを満たす必要があります。
//!         @ref nn::atk::EffectBase::EffectAlignment バイト境界に配置するようにしてください。
//!
//! @see EffectBase クラス
//---------------------------------------------------------------------------
class EffectReverb : public EffectBase
{
    /* ------------------------------------------------------------------------
            constant definition
       ------------------------------------------------------------------------ */
public:
    static const float EarlyGainMin;               //!< GetEarlyGain の最小値 (0.0f) です。
    static const float EarlyGainMax;               //!< GetEarlyGain の最大値 (1.0f) です。
    static const float LateGainMin;                //!< GetLateGain の最小値 (0.0f) です。
    static const float LateGainMax;                //!< GetLateGain の最大値 (1.0f) です。
    static const float HighFrequencyDecayRatioMin; //!< GetHighFrequencyDecayRatio の最小値 (0.1f) です。
    static const float HighFrequencyDecayRatioMax; //!< GetHighFrequencyDecayRatio の最大値 (1.0f) です。
    static const float ColorationMin;              //!< GetColoration の最小値 (0.0f) です。
    static const float ColorationMax;              //!< GetColoration の最大値 (1.0f) です。
    static const float ReverbGainMin;              //!< GetReverbGain の最小値 (0.0f) です。
    static const float ReverbGainMax;              //!< GetReverbGain の最大値 (1.0f) です。
    static const float OutGainMin;                 //!< GetOutGain の最小値 (0.0f) です。
    static const float OutGainMax;                 //!< GetOutGain の最大値 (1.0f) です。
    static const float DryGainMin;                 //!< GetDryGain の最小値 (0.0f) です。
    static const float DryGainMax;                 //!< GetDryGain の最大値 (1.0f) です。

    static const float DefaultEarlyGain;               //!< 初期反射音の出力ゲインのデフォルト値 (0.7f) です。
    static const float DefaultLateGain;                //!< 後期残響音の出力ゲインのデフォルト値 (0.7f) です。
    static const float DefaultHighFrequencyDecayRatio; //!< リバーブの高周波数成分の減衰率のデフォルト値 (0.8f) です。
    static const float DefaultColoration;              //!< リバーブの残響音の音色パラメータのデフォルト値 (0.7f) です。
    static const float DefaultReverbGain;              //!< リバーブに対する入力ゲインのデフォルト値 (0.2f) です。
    static const float DefaultOutGain;                 //!< リバーブに対する出力ゲインのデフォルト値 (1.0f) です。
    static const float DefaultDryGain;                 //!< ドライ成分のゲインのデフォルト値 (0.7f) です。

    /* ------------------------------------------------------------------------
            typename definition
       ------------------------------------------------------------------------ */
public:
    /**
     * @brief 初期反射音モードのプリセット値です。
     */
    enum EarlyMode
    {
        EarlyMode_SmallRoom = 0,                //!< 小さい部屋
        EarlyMode_LargeRoom,                    //!< 大きい部屋
        EarlyMode_Hall,                         //!< ホール
        EarlyMode_Cavern,                       //!< 洞窟
        EarlyMode_NoEarlyReflection,            //!< 初期反射音なし
        EarlyMode_Count,
        EarlyMode_Default = EarlyMode_SmallRoom //!< デフォルトのモード
    };

    /**
     * @brief 後期残響音モードのプリセット値です。
     */
    enum LateMode
    {
        LateMode_Room,                        //!< 部屋
        LateMode_Hall,                        //!< ホール
        LateMode_MetalCorridor,               //!< 金属製の回廊
        LateMode_Cavern,                      //!< 洞窟
        LateMode_MaximumDelay,                //!< 最大ディレイ（リバーブ）
        LateMode_Count,
        LateMode_Default = LateMode_Room //!< デフォルトのモード
    };

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
    //!         - GetEarlyMode               = EarlyMode_Default
    //!         - GetEarlyGain               = DefaultEarlyGain
    //!         - GetPredelayTime            = GetDefaultPredelayTime
    //!         - GetLateMode                = LateMode_Default
    //!         - GetLateGain                = DefaultLateGain
    //!         - GetDecayTime               = GetDefaultDecayTime
    //!         - GetHighFrequencyDecayRatio = DefaultHighFrequencyDecayRatio
    //!         - GetColoration              = DefaultColoration
    //!         - GetReverbGain              = DefaultReverbGain
    //!         - GetOutGain                 = DefaultOutGain
    //!         - GetDryGain                 = DefaultDryGain
    //!         - IsEnabled                  = true
    //!
    //!         また、デフォルトのチャンネルモードは ChannelMode_4ch で、
    //!         nn::atk::ChannelIndex の小さな値から順にエフェクトが適用されます。
    //---------------------------------------------------------------------------
    EffectReverb() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~EffectReverb() NN_NOEXCEPT NN_OVERRIDE;

    //@}

    //! @name メモリ割り当て
    //@{

    //---------------------------------------------------------------------------
    //! @brief    EffectReverb が必要とするメモリのサイズを取得します。
    //!
    //!           EffectReverb が現在のパラメータ設定でエフェクト処理を行うために必要とする
    //!           メモリのサイズを取得します。
    //!
    //! @return   EffectReverb が必要とするメモリのサイズを返します。
    //---------------------------------------------------------------------------
    virtual size_t GetRequiredMemSize() const NN_NOEXCEPT NN_OVERRIDE;
    //@}

    //! @name リバーブエフェクト定数取得
    //@{

    //---------------------------------------------------------------------------
    //! @brief    初期反射音から後期残響音になるまでの時間の最小値 (0 msec) を取得します。
    //!
    //! @return   初期反射音から後期残響音になるまでの時間の最小値 (0 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetPredelayTimeMin() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    初期反射音から後期残響音になるまでの時間の最大値 (300 msec) を取得します。
    //!
    //! @return   初期反射音から後期残響音になるまでの時間の最大値 (300 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetPredelayTimeMax() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    後期残響音の減衰量が 60dB になるまでの時間の最小値 (100 msec) を取得します。
    //!
    //! @return   後期残響音の減衰量が 60dB になるまでの時間の最小値 (100 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetDecayTimeMin() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    後期残響音の減衰量が 60dB になるまでの時間の最大値 (20 sec) を取得します。
    //!
    //! @return   後期残響音の減衰量が 60dB になるまでの時間の最大値 (20 sec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetDecayTimeMax() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    初期反射音から後期残響音になるまでの時間のデフォルト値 (15 msec) を取得します。
    //!
    //! @return   初期反射音から後期残響音になるまでの時間のデフォルト値 (15 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetDefaultPredelayTime() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    後期残響音の減衰量が 60dB になるまでの時間のデフォルト値 (1.5 sec) を取得します。
    //!
    //! @return   後期残響音の減衰量が 60dB になるまでの時間のデフォルト値 (1.5 sec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetDefaultDecayTime() NN_NOEXCEPT;

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

    //! @name リバーブエフェクト操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief    エフェクトが削除可能かどうかを取得します。
    //!
    //!           @ref SoundSystem::ClearEffect または @ref SoundSystem::RemoveEffect を呼び出す前に、
    //!           まず @ref SetEnabled でリバーブを無効に設定した後、本関数で削除可能であることを確認してください。
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
    //! @brief    エフェクトが削除可能かどうかを取得します。
    //!
    //! @deprecated この関数は廃止予定です。代わりに @ref IsRemovable を使用してください。
    //---------------------------------------------------------------------------
    NN_DEPRECATED bool IsClearable() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    リバーブエフェクトの最大チャンネル数を取得します。
    //!
    //! @return   リバーブエフェクトの最大チャンネル数です。
    //!
    //! @see  SetChannelMode
    //---------------------------------------------------------------------------
    ChannelMode GetChannelMode() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    リバーブエフェクトの最大チャンネル数を設定します。
    //!
    //!           この関数でチャンネル数を指定した場合、
    //!           nn::atk::ChannelIndex の小さな値から順に、設定したチャンネル数分のリバーブが適用されます。
    //!           リバーブを適用するチャンネルも併せて設定したい場合は、この関数ではなく @ref SetChannelIndex を利用してください。
    //!           入力チャンネル数は 1/2/4/6 チャンネルのいずれかを利用可能です。
    //!           初期値は ChannelMode_4ch です。
    //!
    //! @param[in] mode チャンネル数を指定する列挙型です。
    //!
    //! @return   最大チャンネル数の設定に成功すれば true を返します
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
    //! @brief    リバーブエフェクトの現在のチャンネル設定を取得します。
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
    //! @brief    リバーブエフェクトのチャンネル設定を行います。
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
    //! @brief    初期反射音のモードを取得します。
    //!
    //! @return   初期反射音のモードです。
    //!
    //! @see  SetEarlyMode
    //---------------------------------------------------------------------------
    EarlyMode GetEarlyMode() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     初期反射音のモードを設定します。
    //!
    //! @param[in] earlyMode 初期反射音のモードです。
    //!
    //! @see  GetEarlyMode
    //---------------------------------------------------------------------------
    void SetEarlyMode( EarlyMode earlyMode ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    初期反射音の出力ゲインを取得します。
    //!
    //! @return   初期反射音の出力ゲインの値です。
    //!
    //! @see  SetEarlyGain
    //---------------------------------------------------------------------------
    float GetEarlyGain() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     初期反射音の出力ゲインを設定します。
    //!            値の範囲は @ref EarlyGainMin ～ @ref EarlyGainMax です。
    //!
    //! @param[in] earlyGain 初期反射音の出力ゲインの値です。
    //!
    //! @see  GetEarlyGain
    //---------------------------------------------------------------------------
    void SetEarlyGain( float earlyGain ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    初期反射音から後期残響音になるまでの時間を取得します。
    //!
    //! @return   初期反射音から後期残響音になるまでの時間です。
    //!
    //! @see  SetPredelayTime
    //---------------------------------------------------------------------------
    nn::TimeSpan GetPredelayTime() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     初期反射音から後期残響音になるまでの時間を設定します。
    //!            値の範囲は @ref GetPredelayTimeMin ～ @ref GetPredelayTimeMax です。
    //!
    //! @param[in] predelayTime 初期反射音から後期残響音になるまでの時間です。
    //!
    //! @see  GetPredelayTime
    //---------------------------------------------------------------------------
    void SetPredelayTime( nn::TimeSpan predelayTime ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    後期残響音のモードを取得します。
    //!
    //! @return   後期残響音のモードです。
    //!
    //! @see  SetLateMode
    //---------------------------------------------------------------------------
    LateMode GetLateMode() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     後期残響音のモードを設定します。
    //!            初期値は LateMode_Room です。
    //!
    //! @param[in] lateMode 後期残響音のモードです。
    //!
    //! @see  GetLateMode
    //---------------------------------------------------------------------------
    void SetLateMode( LateMode lateMode ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    後期残響音の出力ゲインを取得します。
    //!
    //! @return   後期残響音の出力ゲインの値です。
    //!
    //! @see  SetLateGain
    //---------------------------------------------------------------------------
    float GetLateGain() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     後期残響音の出力ゲインを設定します。
    //!            値の範囲は @ref LateGainMin ～ @ref LateGainMax です。
    //!
    //! @param[in] lateGain 後期残響音の出力ゲインの値です。
    //!
    //! @see  GetLateGain
    //---------------------------------------------------------------------------
    void SetLateGain( float lateGain ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    後期残響音の減衰量が 60dB になるまでの時間を取得します。
    //!
    //! @return   後期残響音の減衰量が 60dB になるまでの時間です。
    //!
    //! @see  SetDecayTime
    //---------------------------------------------------------------------------
    nn::TimeSpan GetDecayTime() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     後期残響音の減衰量が 60dB になるまでの時間を設定します。
    //!            値の範囲は @ref GetDecayTimeMin ～ @ref GetDecayTimeMax (sec) です。
    //!
    //! @param[in] decayTime 後期残響音の減衰量が 60dB になるまでの時間です。
    //!
    //! @see  GetDecayTime
    //---------------------------------------------------------------------------
    void SetDecayTime( nn::TimeSpan decayTime ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    リバーブの高周波数成分の減衰率を取得します。
    //!
    //! @return   リバーブの高周波数成分の減衰率の値です。
    //!
    //! @see  SetHighFrequencyDecayRatio
    //---------------------------------------------------------------------------
    float GetHighFrequencyDecayRatio() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     リバーブの高周波数成分の減衰率を取得します。
    //!            値を小さくすると低周波成分が主流となり、高周波成分で急速な減衰が発生します。
    //!            値が 1.0 の場合は高周波成分と低周波成分の両方が同じ速度で減衰します。
    //!            値の範囲は @ref HighFrequencyDecayRatioMin ～ @ref HighFrequencyDecayRatioMax です。
    //!
    //! @param[in] highFrequencyDecayRatio リバーブの高周波数成分の減衰率の値です。
    //!
    //! @see  GetHighFrequencyDecayRatio
    //---------------------------------------------------------------------------
    void SetHighFrequencyDecayRatio( float highFrequencyDecayRatio ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    リバーブの残響音の音色パラメータを取得します。
    //!
    //! @return   リバーブの残響音の音色パラメータの値です。
    //!
    //! @see  SetColoration
    //---------------------------------------------------------------------------
    float GetColoration() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     リバーブの残響音の音色パラメータを取得します。
    //!            値が小さくなるほど残響音の密度が粗くなります。
    //!            値の範囲は @ref ColorationMin ～ @ref ColorationMax です。
    //!
    //! @param[in] coloration リバーブの残響音の音色パラメータの値です。
    //!
    //! @see  GetColoration
    //---------------------------------------------------------------------------
    void SetColoration( float coloration ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    リバーブアルゴリズムに対する入力ゲインを取得します。
    //!
    //! @return   リバーブアルゴリズムに対する入力ゲインの値です。
    //!
    //! @see  SetReverbGain
    //---------------------------------------------------------------------------
    float GetReverbGain() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     リバーブアルゴリズムに対する入力ゲインを設定します。
    //!            値の範囲は @ref ReverbGainMin ～ @ref ReverbGainMax です。
    //!
    //! @param[in] reverbGain リバーブアルゴリズムに対する入力ゲインの値です。
    //!
    //! @see  GetReverbGain
    //---------------------------------------------------------------------------
    void SetReverbGain( float reverbGain ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    リバーブアルゴリズムに対する出力ゲインを取得します。
    //!
    //! @return   リバーブアルゴリズムに対する出力ゲインの値です。
    //!
    //! @see  SetOutGain
    //---------------------------------------------------------------------------
    float GetOutGain() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     リバーブアルゴリズムに対する出力ゲインを設定します。
    //!            値の範囲は @ref OutGainMin ～ @ref OutGainMax です。
    //!
    //! @param[in] outGain リバーブアルゴリズムに対する出力ゲインの値です。
    //!
    //! @see  GetOutGain
    //---------------------------------------------------------------------------
    void SetOutGain( float outGain ) NN_NOEXCEPT;

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
    //! @brief    リバーブの有効・無効を取得します。
    //!
    //! @return   リバーブが有効であれば true を返します。
    //!
    //! @see  SetEnabled
    //---------------------------------------------------------------------------
    bool IsEnabled() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    リバーブの有効・無効を設定します。
    //!           初期値は false です。
    //!
    //!           @ref SoundSystem::ClearEffect または @ref SoundSystem::RemoveEffect を呼び出す前には、
    //!           まず本関数で無効に設定した後に、@ref IsRemovable で削除可能であることを確認する必要があります。
    //!           本関数で無効にした後すぐに削除可能になるわけではない点に注意してください。
    //!
    //! @param[in] isEnabled リバーブの有効・無効です。
    //!
    //! @see  IsEnabled
    //---------------------------------------------------------------------------
    void SetEnabled( bool isEnabled ) NN_NOEXCEPT;

    //@}

private:
    void ResetChannelIndex() NN_NOEXCEPT;

private:
    // リバーブエフェクトパラメータの構造体
    struct ReverbParam
    {
        EarlyMode earlyMode;
        float earlyGain;
        nn::TimeSpan predelayTime;
        LateMode lateMode;
        float lateGain;
        nn::TimeSpan decayTime;
        float highFrequencyDecayRatio;
        float coloration;
        float reverbGain;
        float outGain;
        float dryGain;
        bool isEnabled;

        ReverbParam() NN_NOEXCEPT;
    };

    // 時間関連のリバーブパラメータのミリ秒
    static const int64_t PredelayTimeMilliSecondsMin;
    static const int64_t PredelayTimeMilliSecondsMax;
    static const int64_t DecayTimeMilliSecondsMin;
    static const int64_t DecayTimeMilliSecondsMax;
    static const int64_t DefaultPredelayTimeMilliSeconds;
    static const int64_t DefaultDecayTimeMilliSeconds;

    // ChannelSettingCountMax はエフェクトが設定できる最大チャンネル数に依存
    static const int ChannelSettingCountMax = 6;

    ReverbParam m_Param;
    std::atomic<uint64_t> m_AudioRendererUpdateCountWhenAddedReverb;
    nn::audio::ReverbType m_ReverbType;
    ChannelMode      m_ChannelMode;

    ChannelIndex m_ChannelSetting[ChannelSettingCountMax];
};

} // namespace nn::atk
} // namespace nn
