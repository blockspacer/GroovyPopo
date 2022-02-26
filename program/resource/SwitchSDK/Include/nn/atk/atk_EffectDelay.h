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
//! @brief  指定されたチャンネル数に対するディレイエフェクトです。
//!
//!         EffectDelay は nn::audio::DelayType に関連する API をラッピングしたクラスです。
//!         EffectDelay は SoundSystem::AppendEffect で追加されたのち、
//!         SoundSystem::ClearEffect か SoundSystem::RemoveEffect によって
//!         エフェクトが削除されるまでクラスを保持しておく必要があります。
//!
//!         この型は std::atomic 型のメンバを持っているため
//!         メモリ上に確保する場合にはアライメントを満たす必要があります。
//!         @ref nn::atk::EffectBase::EffectAlignment バイト境界に配置するようにしてください。
//!
//! @see EffectBase クラス
//---------------------------------------------------------------------------
class EffectDelay : public EffectBase
{
    /* ------------------------------------------------------------------------
            constant definition
       ------------------------------------------------------------------------ */
public:
    static const float InGainMin;            //!< SetDelayInGain の最小値 (0.0f) です。
    static const float InGainMax;            //!< SetDelayInGain の最大値 (1.0f) です。
    static const float FeedbackGainMin;      //!< SetDelayFeedbackGain の最小値 (0.0f) です。
    static const float FeedbackGainMax;      //!< SetDelayFeedbackGain の最大値 (1.0f) です。
    static const float DryGainMin;           //!< SetDelayDryGain の最小値 (0.0f) です。
    static const float DryGainMax;           //!< SetDelayDryGain の最大値 (1.0f) です。
    static const float LowPassAmountMin;     //!< SetDelayLowPassAmount の最小値 (0.0f) です。
    static const float LowPassAmountMax;     //!< SetDelayLowPassAmount の最大値 (1.0f) です。
    static const float ChannelSpreadMin;     //!< SetDelayChannelSpread の最小値 (0.0f) です。
    static const float ChannelSpreadMax;     //!< SetDelayChannelSpread の最大値 (1.0f) です。

    static const float DefaultInGain;              //!< ディレイに入力するゲインのデフォルト値 (0.5f) です。
    static const float DefaultFeedbackGain;        //!< ディレイ音のフィードバックゲインのデフォルト値 (0.4f) です。
    static const float DefaultDryGain;             //!< ドライ成分のゲインのデフォルト値 (0.5f) です。
    static const float DefaultLowPassAmount;       //!< 低域の通過量のデフォルト値 (0.8f) です。
    static const float DefaultChannelSpread;       //!< 隣接するチャンネルに配分するサンプルの量のデフォルト値 (0.3f) です。

    /* ------------------------------------------------------------------------
            typename definition
       ------------------------------------------------------------------------ */
public:
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
    //!         - GetDelayTimeMax  = GetDefaultDelayTimeMax
    //!         - GetDelayTime     = GetDefaultDelayTime
    //!         - GetInGain        = DefaultInGain
    //!         - GetFeedbackGain  = DefaultFeedbackGain
    //!         - GetDryGain       = DefaultDryGain
    //!         - GetLowPassAmount = DefaultLowPassAmount
    //!         - GetChannelSpread = DefaultChannelSpread
    //!         - IsEnabled        = true
    //!
    //!         また、デフォルトのチャンネルモードは ChannelMode_4ch で、
    //!         nn::atk::ChannelIndex の小さな値から順にエフェクトが適用されます。
    //---------------------------------------------------------------------------
    EffectDelay() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~EffectDelay() NN_NOEXCEPT NN_OVERRIDE;

    //@}

    //! @name メモリ割り当て
    //@{

    //---------------------------------------------------------------------------
    //! @brief    EffectDelay が必要とするメモリのサイズを取得します。
    //!
    //!           EffectDelay が現在のパラメータ設定でエフェクト処理を行うために必要とする
    //!           メモリのサイズを取得します。
    //!
    //! @return   EffectDelay が必要とするメモリのサイズを返します。
    //---------------------------------------------------------------------------
    virtual size_t GetRequiredMemSize() const NN_NOEXCEPT NN_OVERRIDE;

    //@}

    //! @name ディレイエフェクト定数取得
    //@{

    //---------------------------------------------------------------------------
    //! @brief    ディレイタイムに設定できるデフォルトの最大値 (2000 msec) を取得します。
    //!
    //! @return   ディレイタイムに設定できるデフォルトの最大値 (2000 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetDefaultDelayTimeMax() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ディレイタイムのデフォルト値 (160 msec) を取得します。
    //!
    //! @return   ディレイタイムのデフォルト値 (160 msec) です。
    //---------------------------------------------------------------------------
    static nn::TimeSpan GetDefaultDelayTime() NN_NOEXCEPT;

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

    //! @name ディレイエフェクト操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief    エフェクトが削除可能かどうかを取得します。
    //!
    //!           @ref SoundSystem::ClearEffect または @ref SoundSystem::RemoveEffect を呼び出す前に、
    //!           まず @ref SetEnabled でディレイを無効に設定した後、本関数で削除可能であることを確認してください。
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
    //! @brief    ディレイエフェクトの最大チャンネル数を取得します。
    //!
    //! @return   ディレイエフェクトの最大チャンネル数です。
    //!
    //! @see  SetChannelMode
    //---------------------------------------------------------------------------
    ChannelMode GetChannelMode() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ディレイエフェクトの最大チャンネル数を設定します。
    //!
    //!           この関数でチャンネル数を指定した場合、
    //!           nn::atk::ChannelIndex の小さな値から順に、設定したチャンネル数分のディレイが適用されます。
    //!           ディレイを適用するチャンネルも併せて設定したい場合は、この関数ではなく @ref SetChannelIndex を利用してください。
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
    //! @brief    ディレイエフェクトの現在のチャンネル設定を取得します。
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
    //! @brief    ディレイエフェクトのチャンネル設定を行います。
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
    bool SetChannelIndex( const ChannelIndex* pChannel, ChannelMode channelMode ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ディレイタイムに設定できる最大値を取得します。
    //!
    //! @return   ディレイタイムに設定できる最大値です。
    //!
    //! @see  SetDelayTimeMax
    //---------------------------------------------------------------------------
    nn::TimeSpan GetDelayTimeMax() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ディレイタイムに設定できる最大値を設定します。
    //!           この値は SoundSystem::AppendEffect を行う前に設定する必要があります。
    //!           この値を大きくするとエフェクトが必要とするメモリサイズも増加します。
    //!
    //! @param[in] delayTimeMax ディレイタイムに設定できる最大値です。
    //!
    //! @return    ディレイタイムの最大値の設定に成功した場合は true を、
    //!            失敗した場合は false を返します。
    //!
    //! @see  GetRequiredMemSize
    //! @see  GetDelayTimeMax
    //---------------------------------------------------------------------------
    bool SetDelayTimeMax( nn::TimeSpan delayTimeMax ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ディレイタイムを取得します。
    //!
    //! @return   ディレイタイムの値です。
    //!
    //! @see  SetDelayTime
    //---------------------------------------------------------------------------
    nn::TimeSpan GetDelayTime() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ディレイタイムを設定します。
    //!           値の範囲は 0 ～ @ref GetDelayTimeMax です。
    //!
    //! @param[in] delayTime ディレイタイムの値です。
    //!
    //! @see  GetDelayTime
    //---------------------------------------------------------------------------
    void SetDelayTime( nn::TimeSpan delayTime ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ディレイに入力するゲインを取得します。
    //!
    //! @return   ディレイに入力するゲインの値です。
    //!
    //! @see  SetInGain
    //---------------------------------------------------------------------------
    float GetInGain() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ディレイに入力するゲインを設定します。
    //!           値の範囲は @ref InGainMin ～ @ref InGainMax です。
    //!
    //! @param[in] inGain ディレイに入力するゲインの値です。
    //!
    //! @see  GetInGain
    //---------------------------------------------------------------------------
    void SetInGain( float inGain ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ディレイ音のフィードバックゲインを取得します。
    //!
    //! @return   ディレイ音のフィードバックゲインの値です。
    //!
    //! @see  SetFeedbackGain
    //---------------------------------------------------------------------------
    float GetFeedbackGain() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ディレイ音のフィードバックゲインを設定します。
    //!           値の範囲は @ref FeedbackGainMin ～ @ref FeedbackGainMax  です。
    //!
    //! @param[in] feedbackGain ディレイ音のフィードバックゲインの値です。
    //!
    //! @see  GetFeedbackGain
    //---------------------------------------------------------------------------
    void SetFeedbackGain( float feedbackGain ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ドライ成分のゲインを取得します。
    //!
    //! @return   ドライ成分のゲインの値です。
    //!
    //! @see  SetDryGain
    //---------------------------------------------------------------------------
    float GetDryGain() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ドライ成分のゲインを設定します。
    //!           値の範囲は @ref DryGainMin ～ @ref DryGainMax です。
    //!
    //! @param[in] dryGain ドライ成分のゲインの値です。
    //!
    //! @see  GetDryGain
    //---------------------------------------------------------------------------
    void SetDryGain( float dryGain ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    低域の通過量を取得します。
    //!
    //! @return   低域の通過量の値です。
    //!
    //! @see  SetLowPassAmount
    //---------------------------------------------------------------------------
    float GetLowPassAmount() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    低域の通過量を設定します。
    //!           値の範囲は @ref LowPassAmountMin ～ @ref LowPassAmountMax です。
    //!
    //! @param[in] lowPassAmount 低域の通過量の値です。
    //!
    //! @see  GetLowPassAmount
    //---------------------------------------------------------------------------
    void SetLowPassAmount( float lowPassAmount ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    隣接するチャンネルに配分するサンプルの量を取得します。
    //!
    //! @return   隣接するチャンネルに配分するサンプルの量の値です。
    //!
    //! @see  SetChannelSpread
    //---------------------------------------------------------------------------
    float GetChannelSpread() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    隣接するチャンネルに配分するサンプルの量を設定します。
    //!           値の範囲は @ref ChannelSpreadMin ～ @ref ChannelSpreadMax です。
    //!
    //! @param[in] channelSpread 隣接するチャンネルに配分するサンプルの量の値です。
    //!
    //! @see  GetChannelSpread
    //---------------------------------------------------------------------------
    void SetChannelSpread( float channelSpread ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ディレイの有効・無効を取得します。
    //!
    //! @return   ディレイが有効であれば true を返します。
    //!
    //! @see  SetEnabled
    //---------------------------------------------------------------------------
    bool IsEnabled() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    ディレイの有効・無効を設定します。
    //!           初期値は false です。
    //!
    //!           @ref SoundSystem::ClearEffect または @ref SoundSystem::RemoveEffect を呼び出す前には、
    //!           まず本関数で無効に設定した後に、@ref IsRemovable で削除可能であることを確認する必要があります。
    //!           本関数で無効にした後すぐに削除可能になるわけではない点に注意してください。
    //!
    //! @param[in] isEnabled ディレイの有効・無効です。
    //!
    //! @see  IsEnabled
    //---------------------------------------------------------------------------
    void SetEnabled( bool isEnabled ) NN_NOEXCEPT;

    //@}

private:
    void ResetChannelIndex() NN_NOEXCEPT;

private:
    // ディレイエフェクトパラメータの構造体
    struct DelayParam
    {
        nn::TimeSpan delayTimeMax;
        nn::TimeSpan delayTime;
        float inGain;
        float feedbackGain;
        float dryGain;
        float lowPassAmount;
        float channelSpread;
        bool isEnabled;

        DelayParam() NN_NOEXCEPT;
    };

    // 時間関連のディレイパラメータのミリ秒
    static const int64_t DefaultDelayTimeMaxMilliSeconds;
    static const int64_t DefaultDelayTimeMilliSeconds;

    // ChannelSettingCountMax はエフェクトが設定できる最大チャンネル数に依存
    static const int ChannelSettingCountMax = 6;

    DelayParam           m_Param;
    std::atomic<uint64_t> m_AudioRendererUpdateCountWhenAddedDelay;

    nn::audio::DelayType m_DelayType;
    ChannelMode          m_ChannelMode;
    ChannelIndex m_ChannelSetting[ChannelSettingCountMax];
};

} // namespace nn::atk
} // namespace nn
