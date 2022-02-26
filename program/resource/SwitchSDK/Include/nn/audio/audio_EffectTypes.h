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

#include <nn/nn_TimeSpan.h>
#include <nn/audio/audio_AudioRendererTypes.h>

namespace nn {
namespace audio {

struct EffectInfo;

/**
 * @brief       BufferMixer エフェクトです。
 *
 * @details
 * BufferMixer エフェクトは、SubMix/FinalMix が管理するミックスバッファ内でのミックスを行います。@n
 * 入力に指定したミックスバッファの内容を、出力に指定したミックスバッファにミックスします。@n
 * このミックス処理における入出力関係は nn::audio::SetBufferMixerInputOutput() で設定されます。@n
 * またミックス時にはボリューム操作も同時に施します。この入出力関係それぞれのミックス処理で用いられるボリュームは nn::audio::SetBufferMixerVolume() で指定します。@n
 *
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref AddBufferMixer(AudioRendererConfig* pOutConfig, BufferMixerType* pBufferMixer, SubMixType* pSubMix)
 * - @ref AddBufferMixer(AudioRendererConfig* pOutConfig, BufferMixerType* pBufferMixer, FinalMixType* pFinalMix)
 * - @ref RemoveBufferMixer(AudioRendererConfig* pOutConfig, BufferMixerType* pBufferMixer, SubMixType* pSubMix)
 * - @ref RemoveBufferMixer(AudioRendererConfig* pOutConfig, BufferMixerType* pBufferMixer, FinalMixType* pFinalMix)
 * - @ref GetBufferMixerInputOutput(const BufferMixerType* pBufferMixer, int8_t* outInput, int8_t* outOutput, int* pOutCount, int count)
 * - @ref SetBufferMixerInputOutput(BufferMixerType* pBufferMixer, const int8_t* input, const int8_t* output, int count)
 * - @ref GetBufferMixerVolume(const BufferMixerType* pBufferMixer, int index)
 * - @ref SetBufferMixerVolume(BufferMixerType* pBufferMixer, int index, float volume)
 * - @ref IsBufferMixerEnabled(const BufferMixerType* pBufferMixer)
 * - @ref SetBufferMixerEnabled(BufferMixerType* pBufferMixer, bool enable)
 * - @ref IsBufferMixerRemovable(const BufferMixerType* pBufferMixer)
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct BufferMixerType
{
    EffectInfo* _pEffectInfo;
};

/**
 * @brief       Delay エフェクトです。
 *
 * @details
 * Delay エフェクトでは一般的なディレイエフェクトを提供します。@n
 *
 * @n
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref AddDelay(AudioRendererConfig* pOutConfig, DelayType* pDelay, void* buffer, size_t bufferSize, SubMixType* pSubMix, nn::TimeSpan delayTimeMax, int channelCountMax)
 * - @ref AddDelay(AudioRendererConfig* pOutConfig, DelayType* pDelay, void* buffer, size_t bufferSize, FinalMixType* pFinalMix, nn::TimeSpan delayTimeMax, int channelCountMax)
 * - @ref RemoveDelay(AudioRendererConfig* pOutConfig, DelayType* pDelay, SubMixType* pSubMix)
 * - @ref RemoveDelay(AudioRendererConfig* pOutConfig, DelayType* pDelay, FinalMixType* pFinalMix)
 * - @ref GetRequiredBufferSizeForDelay(nn::TimeSpan delayTimeMax, int sampleRate, int channelCountMax)
 * - @ref GetDelayInputOutput(const DelayType* pDelay, int8_t* outInput, int8_t* outOutput, int* pOutCount, int count)
 * - @ref SetDelayInputOutput(DelayType* pDelay, const int8_t* input, const int8_t* output, int count)
 * - @ref GetDelayTimeMax(const DelayType* pDelay)
 * - @ref GetDelayChannelCountMax(const DelayType* pDelay)
 * - @ref GetDelayTime(const DelayType* pDelay)
 * - @ref SetDelayTime(DelayType* pDelay, nn::TimeSpan time)
 * - @ref GetDelayFeedbackGain(const DelayType* pDelay)
 * - @ref SetDelayFeedbackGain(DelayType* pDelay, float feedbackGain)
 * - @ref GetDelayDryGain(const DelayType* pDelay)
 * - @ref SetDelayDryGain(DelayType* pDelay, float dryGain)
 * - @ref GetDelayInGain(const DelayType* pDelay)
 * - @ref SetDelayInGain(DelayType* pDelay, float inGain)
 * - @ref GetDelayChannelSpread(const DelayType* pDelay)
 * - @ref SetDelayChannelSpread(DelayType* pDelay, float channelSpread)
 * - @ref GetDelayLowPassAmount(const DelayType* pDelay)
 * - @ref SetDelayLowPassAmount(DelayType* pDelay, float lowPassAmount)
 * - @ref IsDelayEnabled(const DelayType* pDelay)
 * - @ref SetDelayEnabled(DelayType* pDelay, bool enable)
 * - @ref IsDelayRemovable(const DelayType* pDelay)
 * - @ref GetDelayParameters(const DelayType* pDelay)
 * - @ref SetDelayParameters(DelayType* pOutDelay, const DelayParameterSet* pParameterSet)
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct DelayType
{
    EffectInfo* _pEffectInfo;
};

/**
 * @brief       Reverb エフェクトです。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref AddReverb(AudioRendererConfig* pOutConfig, ReverbType* pReverb, void* buffer, size_t bufferSize, FinalMixType* pFinalMix, int channelCountMax)
 * - @ref AddReverb(AudioRendererConfig* pOutConfig, ReverbType* pReverb, void* buffer, size_t bufferSize, SubMixType* pSubMix, int channelCountMax)
 * - @ref RemoveReverb(AudioRendererConfig* pOutConfig, ReverbType* pReverb, FinalMixType* pFinalMix)
 * - @ref RemoveReverb(AudioRendererConfig* pOutConfig, ReverbType* pReverb, SubMixType* pSubMix)
 * - @ref GetRequiredBufferSizeForReverb(int sampleRate, int channelCountMax)
 * - @ref SetReverbInputOutput(ReverbType* pReverb, const int8_t* input, const int8_t* output, int count)
 * - @ref GetReverbInputOutput(const ReverbType* pReverb, int8_t* outInput, int8_t* outOutput, int* pOutCount, int count)
 * - @ref GetReverbEarlyMode(const ReverbType* pReverb)
 * - @ref SetReverbEarlyMode(ReverbType* pReverb, ReverbType::EarlyMode mode)
 * - @ref GetReverbEarlyGain(const ReverbType* pReverb)
 * - @ref SetReverbEarlyGain(ReverbType* pReverb, float gain)
 * - @ref GetReverbPredelayTime(const ReverbType* pReverb)
 * - @ref SetReverbPredelayTime(ReverbType* pReverb, nn::TimeSpan time)
 * - @ref GetReverbLateMode(const ReverbType* pReverb)
 * - @ref SetReverbLateMode(ReverbType* pReverb, ReverbType::LateMode mode)
 * - @ref GetReverbLateGain(const ReverbType* pReverb)
 * - @ref SetReverbLateGain(ReverbType* pReverb, float gain)
 * - @ref GetReverbDecayTime(const ReverbType* pReverb)
 * - @ref SetReverbDecayTime(ReverbType* pReverb, nn::TimeSpan time)
 * - @ref GetReverbHighFrequencyDecayRatio(const ReverbType* pReverb)
 * - @ref SetReverbHighFrequencyDecayRatio(ReverbType* pReverb, float ratio)
 * - @ref GetReverbColoration(const ReverbType* pReverb)
 * - @ref SetReverbColoration(ReverbType* pReverb, float value)
 * - @ref GetReverbReverbGain(const ReverbType* pReverb)
 * - @ref SetReverbReverbGain(ReverbType* pReverb, float gain)
 * - @ref GetReverbOutGain(const ReverbType* pReverb)
 * - @ref SetReverbOutGain(ReverbType* pReverb, float gain)
 * - @ref GetReverbDryGain(const ReverbType* pReverb)
 * - @ref SetReverbDryGain(ReverbType* pReverb, float gain)
 * - @ref IsReverbEnabled(const ReverbType* pReverb)
 * - @ref SetReverbEnabled(ReverbType* pReverb, bool enable)
 * - @ref IsReverbRemovable(const ReverbType* pReverb)
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct ReverbType
{
    /**
     * @brief 初期反射音モードのプリセット値です。
     */
    enum EarlyMode
    {
        EarlyMode_SmallRoom = 0,     //!< 小さい部屋
        EarlyMode_LargeRoom,         //!< 大きい部屋
        EarlyMode_Hall,              //!< ホール
        EarlyMode_Cavern,            //!< 洞窟
        EarlyMode_NoEarlyReflection, //!< 初期反射音なし
        EarlyMode_Count
    };

    /**
     * @brief 後期残響音モードのプリセット値です。
     */
    enum LateMode
    {
        LateMode_Room,               //!< 部屋
        LateMode_Hall,               //!< ホール
        LateMode_MetalCorridor,      //!< 金属製の回廊
        LateMode_Cavern,             //!< 洞窟
        LateMode_MaximumDelay,       //!< 最大ディレイ（リバーブ）
        LateMode_Count
    };

    /**
     * @brief decayTime に設定できる最大時間です。
     */
    static const nn::TimeSpan GetDecayTimeMax() NN_NOEXCEPT
    {
        return nn::TimeSpan::FromSeconds(20);
    }

    /**
     * @brief predelayTime に設定できる最大時間です。
     */
    static const nn::TimeSpan GetPredelayTimeMax() NN_NOEXCEPT
    {
        return nn::TimeSpan::FromMilliSeconds(100);
    }

    EffectInfo* _pEffectInfo;
};



/**
 * @brief       I3dl2Reverb エフェクトです。
 *
 * @details
 * Interactive 3D Audio Level 2 (I3DL2) 仕様に部分的に準拠した環境リバーブです。
 *
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * (ワークバッファサイズ取得)
 * - @ref GetRequiredBufferSizeForI3dl2Reverb(int sampleRate, int channelCountMax)
 *
 * (確保・解放)
 * - @ref AddI3dl2Reverb(AudioRendererConfig* pOutConfig, I3dl2ReverbType* pOutReverb, void* buffer, size_t bufferSize, FinalMixType* pFinalMix, int channelCountMax)
 * - @ref AddI3dl2Reverb(AudioRendererConfig* pOutConfig, I3dl2ReverbType* pOutReverb, void* buffer, size_t bufferSize, SubMixType* pSubMix, int channelCountMax)
 * - @ref RemoveI3dl2Reverb(AudioRendererConfig* pOutConfig, I3dl2ReverbType* pReverb, FinalMixType* pFinalMix)
 * - @ref RemoveI3dl2Reverb(AudioRendererConfig* pOutConfig, I3dl2ReverbType* pReverb, SubMixType* pSubMix)
 * - @ref IsI3dl2ReverbRemovable(const I3dl2ReverbType* pReverb)
 *
 * (接続先設定)
 * - @ref GetI3dl2ReverbInputOutput(const I3dl2ReverbType* pReverb, int8_t* outInput, int8_t* outOutput, int* pOutCount, int count)
 * - @ref SetI3dl2ReverbInputOutput(I3dl2ReverbType* pReverb, const int8_t* input, const int8_t* output, int count)
 *
 * (有効・無効設定)
 * - @ref SetI3dl2ReverbEnabled(I3dl2ReverbType* pReverb, bool enable)
 * - @ref IsI3dl2ReverbEnabled(const I3dl2ReverbType* pReverb)
 *
 * (パラメータ)
 * - @ref SetI3dl2ReverbRoomGain(I3dl2ReverbType* pReverb, float room)
 * - @ref GetI3dl2ReverbRoomGain(const I3dl2ReverbType* pReverb)
 * - @ref SetI3dl2ReverbRoomHfGain(I3dl2ReverbType* pReverb, float roomHf)
 * - @ref GetI3dl2ReverbRoomHfGain(const I3dl2ReverbType* pReverb)
 * - @ref SetI3dl2ReverbLateReverbDecayTime(I3dl2ReverbType* pReverb, nn::TimeSpan time)
 * - @ref GetI3dl2ReverbLateReverbDecayTime(const I3dl2ReverbType* pReverb)
 * - @ref SetI3dl2ReverbLateReverbHfDecayRatio(I3dl2ReverbType* pReverb, float hfDecayRatio)
 * - @ref GetI3dl2ReverbLateReverbHfDecayRatio(const I3dl2ReverbType* pReverb)
 * - @ref SetI3dl2ReverbReflectionsGain(I3dl2ReverbType* pReverb, float reflections)
 * - @ref GetI3dl2ReverbReflectionsGain(const I3dl2ReverbType* pReverb)
 * - @ref SetI3dl2ReverbReflectionDelayTime(I3dl2ReverbType* pReverb, nn::TimeSpan time)
 * - @ref GetI3dl2ReverbReflectionDelayTime(const I3dl2ReverbType* pReverb)
 * - @ref SetI3dl2ReverbReverbGain(I3dl2ReverbType* pReverb, float reverb)
 * - @ref GetI3dl2ReverbReverbGain(const I3dl2ReverbType* pReverb)
 * - @ref SetI3dl2ReverbLateReverbDelayTime(I3dl2ReverbType* pReverb, nn::TimeSpan time)
 * - @ref GetI3dl2ReverbLateReverbDelayTime(const I3dl2ReverbType* pReverb)
 * - @ref SetI3dl2ReverbLateReverbDiffusion(I3dl2ReverbType* pReverb, float diffusion)
 * - @ref GetI3dl2ReverbLateReverbDiffusion(const I3dl2ReverbType* pReverb)
 * - @ref SetI3dl2ReverbLateReverbDensity(I3dl2ReverbType* pReverb, float density)
 * - @ref GetI3dl2ReverbLateReverbDensity(const I3dl2ReverbType* pReverb)
 * - @ref SetI3dl2ReverbHfReference(I3dl2ReverbType* pReverb, float hfReference)
 * - @ref GetI3dl2ReverbHfReference(const I3dl2ReverbType* pReverb)
 * - @ref SetI3dl2ReverbDryGain(I3dl2ReverbType* pReverb, float dryGain)
 * - @ref GetI3dl2ReverbDryGain(const I3dl2ReverbType* pReverb)
 *
 * (プリセット)
 * - @ref LoadI3dl2ReverbPreset(I3dl2ReverbType* pReverb, I3dl2ReverbType::Preset preset)
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct I3dl2ReverbType
{

    /**
     * @brief I3dl2ReverbType に指定できるプリセットパラメータを表す列挙型です。
     */
    enum Preset
    {
        Preset_SmallRoom,            // !< 小さな部屋
        Preset_LargeRoom,            // !< 大きな部屋
        Preset_Hall,                 // !< ホール
        Preset_CavernousCathedral,   // !< 大聖堂
        Preset_MetalCorridor,        // !< 金属製の回廊
        Preset_Count                 // !< プリセットの個数
    };

    EffectInfo* _pEffectInfo;
};

/**
* @brief       I3dl2Reverb エフェクトパラメータです。
*
* @details
* I3dl2Reverb が施す I3dl2Reverb エフェクト処理のパラメータセットです。
* 各パラメータが持つ意味は@confluencelink{93362777, オーディオエフェクトの説明の「I3dl2 リバーブパラメータ」を参照してください。}
*/
struct I3dl2ReverbParameterSet
{
    float roomGain;               //!< ルームゲイン [mB]
    float roomHfGain;             //!< 低周波成分を基準とする高周波成分の減衰量 [mB]
    float lateReverbDecayTime;    //!< 残響時間 [s]
    float lateReverbHfDecayRatio; //!< 高周数成分の減衰率
    float reflectionsGain;        //!< 初期反射音のレベル [mB]
    float reflectionsDelayTime;   //!< 初期反射音の遅延時間 [s]
    float reverbGain;             //!< リバーブゲイン[mB]
    float reverbDelayTime;        //!< ディレイタイム [s]
    float reverbDiffusion;        //!< 後期残響音のエコー密度
    float reverbDensity;          //!< 後期残響音のモーダル密度
    float hfReference;            //!< 高周波の参照周波数 [Hz]
    float dryGain;                //!< ドライゲイン [mB]
};

/**
* @brief       Reverb エフェクトパラメータです。
*
* @details
* Reverb が施す Reverb エフェクト処理のパラメータセットです。
* 各パラメータが持つ意味は@confluencelink{93362777, オーディオエフェクトの説明の「リバーブパラメータ」を参照してください。}
*/
struct ReverbParameterSet
{
    ReverbType::EarlyMode earlyMode; //!< 初期反射モード
    float earlyGain;                 //!< 初期反射音のゲイン
    float predelayTimeMilliSeconds;  //!< プリディレイの時間
    ReverbType::LateMode lateMode;   //!< 後期残響音モード
    float lateGain;                  //!< 後記残響音のゲイン
    float decayTimeSeconds;          //!< 残響時間
    float highFreqDecayRatio;        //!< 高周波成分の減衰率
    float coloration;                //!< 残響音の音色パラメータ
    float reverbGain;                //!< リバーブゲイン
    float outGain;                   //!< 出力ゲイン
    float dryGain;                   //!< ドライゲイン
};

/**
* @brief       Delay エフェクトパラメータです。
*
* @details
* Delay が施す Delay エフェクト処理のパラメータセットです。
* 各パラメータが持つ意味は@confluencelink{93362777, オーディオエフェクトの説明の「ディレイパラメータ」を参照してください。}
*/
struct DelayParameterSet
{
    nn::TimeSpan delayTime; //!< ディレイタイム
    float   inGain;         //!< インゲイン
    float   feedbackGain;   //!< フィードバックゲイン
    float   dryGain;        //!< ドライゲイン
    float   channelSpread;  //!< チャンネルスプレッド
    float   lowPassAmount;  //!< ローパスフィルタのパラメータ
};

/**
* @brief       Limiter エフェクトパラメータです。
*
* @details
* Limiter が施す Limiter エフェクト処理のパラメータセットです。
*/
struct LimiterParameterSet
{
    nn::TimeSpan    attackTime;   //!< アタックタイム
    nn::TimeSpan    releaseTime;  //!< リリースタイム
    int32_t         threshold;    //!< スレッショルド
};

/**
 * @brief  ユーザ―が独自に定義可能なエフェクト処理を組み込むためのエフェクト
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref GetRequiredBufferSizeForAuxSendReturnBuffer()
 * - @ref AddAux(AudioRendererConfig* pOutConfig, AuxType* pAux, SubMixType* pSubMix, void* pSendBuffer, void* pReturnBuffer, size_t bufferSize);
 * - @ref AddAux(AudioRendererConfig* pOutConfig, AuxType* pAux, FinalMixType* pFinalMix, void* pSendBuffer, void* pReturnBuffer, size_t bufferSize);
 * - @ref RemoveAux(AudioRendererConfig* pOutConfig, AuxType* pAux, SubMixType* pSubMix)
 * - @ref RemoveAux(AudioRendererConfig* pOutConfig, AuxType* pAux, FinalMixType* pFinalMix)
 * - @ref SetAuxEnabled()
 * - @ref IsAuxEnabled()
 * - @ref GetAuxInputOutput()
 * - @ref SetAuxInputOutput()
 * - @ref ReadAuxSendBuffer()
 * - @ref WriteAuxReturnBuffer()
 * - @ref GetAuxSampleRate()
 * - @ref GetAuxSampleCount()
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct AuxType
{
    EffectInfo* _pEffectInfo;
};

/**
 * @brief       BiquadFilter エフェクトです。
 *
 * @details
 * BiquadFilter エフェクトでは一般的なバイクアッドフィルタエフェクトを提供します。@n
 *
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref AddBiquadFilter(AudioRendererConfig* pOutConfig, BiquadFilterType* pBiquadFilter, SubMixType* pSubMix)
 * - @ref AddBiquadFilter(AudioRendererConfig* pOutConfig, BiquadFilterType* pBiquadFilter, FinalMixType* pFinalMix)
 * - @ref RemoveBiquadFilter(AudioRendererConfig* pOutConfig, BiquadFilterType* pBiquadFilter, SubMixType* pSubMix)
 * - @ref RemoveBiquadFilter(AudioRendererConfig* pOutConfig, BiquadFilterType* pBiquadFilter, FinalMixType* pFinalMix)
 * - @ref GetBiquadFilterInputOutput(const BiquadFilterType* pBiquadFilter, int8_t* outInput, int8_t* outOutput, int* pOutCount, int count)
 * - @ref SetBiquadFilterInputOutput(BiquadFilterType* pBiquadFilter, const int8_t* input, const int8_t* output, int count)
 * - @ref IsBiquadFilterEnabled(const BiquadFilterType* pBiquadFilter)
 * - @ref SetBiquadFilterParameter(BiquadFilterType* pBiquadFilter, const BiquadFilterParameter& parameter)
 * - @ref BiquadFilterParameter GetBiquadFilterParameter(const BiquadFilterType* pBiquadFilter)
 * - @ref SetBiquadFilterEnabled(BiquadFilterType* pBiquadFilter, bool enable)
 * - @ref IsBiquadFilterRemovable(const BiquadFilterType* pBiquadFilter)
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct BiquadFilterType
{
    EffectInfo* _pEffectInfo;

    /**
     * @brief       BiquadFilterType に設定可能なチャンネルの最大数
     */
    static const int ChannelCountMax = 6;
};
}  // namespace audio
}  // namespace nn
