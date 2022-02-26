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

#include <cstdint>
#include <cstddef>

namespace nn {
namespace audio {

/**
 * @brief       オーディオレンダラの状態を表す列挙型です。
 */
enum AudioRendererState
{
    AudioRendererState_Started,               //!< 動作しています
    AudioRendererState_Stopped,               //!< 停止しています
};

/**
 * @brief       オーディオレンダラがレンダリング処理を行うデバイスを表す列挙型です。
 */
enum AudioRendererRenderingDevice
{
    AudioRendererRenderingDevice_AudioCoprocessor,    //!< オーディオ用コプロセッサ
    AudioRendererRenderingDevice_Cpu,                 //!< CPU
};

/**
 * @brief       オーディオレンダラのレンダリング処理実行モードを表す列挙型です。
 */
enum AudioRendererExecutionMode
{
    AudioRendererExecutionMode_AutoExecution,           //!< 自動でレンダリング処理を実行
    AudioRendererExecutionMode_ManualExecution,         //!< 手動でレンダリング処理を実行
};

/**
 * @brief       同時に利用可能な AudioRenderer の数です。
 */
const int AudioRendererCountMax = 2;

/**
 * @brief       単一の SubMix・FinalMix 上に作成可能なミックスバッファ数の上限です。
 */
const int MixBufferCountMax = 24;

/**
 * @brief       オーディオレンダラハンドルです。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref OpenAudioRenderer(AudioRendererHandle *outHandle, const AudioRendererParameter &parameter, void *workBuffer, size_t workBufferSize)
 * - @ref OpenAudioRenderer(AudioRendererHandle *outHandle, nn::os::SystemEvent *pOutSystemEvent, const AudioRendererParameter &parameter, void *workBuffer, size_t workBufferSize)
 * - @ref CloseAudioRenderer()
 * - @ref StartAudioRenderer()
 * - @ref StopAudioRenderer()
 * - @ref RequestUpdateAudioRenderer()
 * - @ref GetAudioRendererState()
 * - @ref GetAudioRendererSampleRate()
 * - @ref GetAudioRendererSampleCount()
 * - @ref GetAudioRendererMixBufferCount()
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct AudioRendererHandle
{
    void* _handle;
    void* _context;
};

/**
 * @brief       オーディオレンダラのパラメータです。
 * @details
 * nn::audio のオーディオレンダラは nn::audio::OpenAudioRenderer() で取得することができます。
 * 取得の際には parameter 引数にこの構造体を、また workBuffer 引数にオーディオレンダラが利用するワークバッファを指定する必要があります。
 * この workBuffer で必要となるバッファサイズは、parameter で指定する各種値によって変動します。
 *
 * workBuffer のサイズは、この構造体を以下の手順で初期化・設定することで、取得することができます。
 * - nn::audio::InitializeAudioRendererParameter() で、この構造体を初期化する
 * - アプリケーションの設計に応じて各メンバー変数を必要な値に設定する
 * - メンバー変数の指定が完了した構造体を nn::audio::GetAudioRendererConfigWorkBufferSize() の引数に指定して、サイズを取得する
 *
 * 各メンバー変数の意味は以下の通りです。
 * - sampleRate はオーディオレンダラが出力するサンプルデータのサンプルレートです。
 *   FinalMixType から出力されるオーディオレンダラの出力は、ここで指定したサンプルレートとなります。
 *   プラットフォームがサポートするサンプルレートが、この値と異なる場合、オーディオレンダラは適切にサンプルレート変換を行います。
 * - sampleCount は単一のミックスバッファが保持するサンプル数です。
 *   ミックスバッファの詳しい説明は @confluencelink{89991492, オーディオレンダラの説明の「ミックスバッファについて」} を参照してください。
 * - mixBufferCount はオーディオレンダラが利用するミックスバッファの最大数です。
 *   各 FinalMixType / SubMixType で利用可能なミックスバッファの総数はこのメンバー変数によって決定されます。
 * - subMixCount はオーディオレンダラで利用する SubMixType の最大数です。
 *   アプリケーションの設計に応じて、利用しうる最大数を指定してください。
 * - voiceCount はオーディオレンダラで利用する VoiceType の最大数です。
 *   オーディオレンダラの同時発音数の上限は、このメンバー変数によって決定されます。
 *   ただしプラットフォーム毎に処理負荷および VoiceType の優先度に応じた発音制限機能により、ここで指定した数以下の発音数でも発音が制限される場合があります。
 *   (現在のリリースでは発音制限機能は実装されていません。)
 * - sinkCount はオーディオレンダラの出力を接続する先の個数を指定します。
 *   詳しくは SinkType および FinalMixType の説明を参照してください。
 * - effectCount  はオーディオレンダラで利用可能なエフェクトの最大数です。
 *   エフェクトによっては、 nn::audio::OpenAudioRenderer() で workBuffer として指定するバッファとは別に、個別のワークバッファが必要となる場合があります。
 *   詳しくは各エフェクトの説明を参照してください。
 * - performanceFrameCount はオーディオレンダラで蓄積するパフォーマンス情報の最大オーディオフレーム数を指定します。
 *   詳しくは nn::audio::GetRequiredBufferSizeForPerformanceFrames() の説明を参照してください。
 *
 * また各メンバー変数には、各プラットフォームのサポート状況に応じて、有効な数値の組み合わせが存在します。
 * 指定した各メンバ変数の組み合わせが利用可能な組み合わせかどうかは nn::audio::IsValidAudioRendererParameter() で確認することができます。
 *
 * - @ref nn::audio::InitializeAudioRendererParameter()
 */
struct AudioRendererParameter
{
    int32_t sampleRate;                                    //!< オーディオレンダラが出力するサンプルデータのサンプルレート
    int32_t sampleCount;                                   //!< 1 つのミックスバッファが保持するサンプル数
    int32_t mixBufferCount;                                //!< オーディオレンダラで利用するミックスバッファの最大数
    int32_t subMixCount;                                   //!< オーディオレンダラで利用する SubMixType の最大数
    int32_t voiceCount;                                    //!< オーディオレンダラで利用する VoiceType の最大数
    int32_t sinkCount;                                     //!< オーディオレンダラで利用する SinkType の最大数
    int32_t effectCount;                                   //!< オーディオレンダラで利用するエフェクトの最大数
    int32_t performanceFrameCount;                         //!< 蓄積するパフォーマンス情報の最大オーディオフレーム数
    bool isVoiceDropEnabled;                               //!< ボイスドロップ機能を有効にするかどうか
    int32_t splitterCount;                                 //!< オーディオレンダラで利用する SplitterType の最大数
    int32_t splitterSendChannelCount;                      //!< SplitterType から出力するチャンネルの総数
    AudioRendererRenderingDevice renderingDevice;          //!< レンダリング処理を実行するデバイス
    AudioRendererExecutionMode executionMode;              //!< レンダリング処理の実行モード
    int32_t _extra;
    uint32_t _magic;

    static const int32_t MixBufferCountMax = 256;         //!< オーディオレンダラで利用可能なミックスバッファの最大数
    static const int32_t SubMixCountMax = 256;            //!< オーディオレンダラで利用可能な SubMixType の最大数
    static const int32_t VoiceCountMax = 1024;            //!< オーディオレンダラで利用可能な VoiceType の最大数
    static const int32_t SinkCountMax = 256;              //!< オーディオレンダラで利用可能な SinkType の最大数
    static const int32_t EffectCountMax = 256;            //!< オーディオレンダラで利用可能な EffectType の最大数
    static const int32_t PerformanceFrameCountMax = 4096; //!< オーディオレンダラで利用可能な PerformanceFrameCount の最大数
    static const int32_t SplitterCountMax = 256;          //!< オーディオレンダラで利用可能な SplitterType の最大数
};

class VoiceInfoManager;
class MixManager;
class EffectManager;
class SinkManager;
class PerformanceBufferManager;
class MemoryPoolManager;
class BehaviorManager;
class SplitterInfoManager;
struct RendererInfo;
class ExtraManager;

/**
 * @brief       オーディオレンダラの設定を管理する構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * (初期化)
 * - @ref InitializeAudioRendererConfig()
 *
 * (Voice)
 * - @ref AcquireVoiceSlot()
 * - @ref ReleaseVoiceSlot()
 * - @ref SetVoiceDestination(AudioRendererConfig *pOutConfig, VoiceType *pSource, SubMixType *pDestination)
 * - @ref SetVoiceDestination(AudioRendererConfig *pOutConfig, VoiceType *pSource, FinalMixType *pDestination)
 * - @ref ClearVoiceDestination()
 *
 * (SubMix)
 * - @ref AcquireSubMix()
 * - @ref ReleaseSubMix()
 * - @ref SetSubMixDestination(AudioRendererConfig *pOutConfig, SubMixType *pSource, FinalMixType *pDestination)
 * - @ref SetSubMixDestination(AudioRendererConfig *pOutConfig, SubMixType *pSource, SubMixType *pDestination)
 * - @ref ClearSubMixDestination()
 *
 * (FinalMix)
 * - @ref AcquireFinalMix()
 * - @ref ReleaseFinalMix()
 *
 * (Splitter)
 * - @ref AcquireSplitter()
 * - @ref ReleaseSplitter()
 * - @ref SetSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex, SubMixType* pDestination)
 * - @ref SetSplitterDestination(AudioRendererConfig* pOutConfig, SplitterType* pOutSource, int destinationIndex, FinalMixType* pDestination)
 * - @ref ClearSplitterDestination()
 *
 * (Sink)
 * - @ref AddDeviceSink()
 * - @ref RemoveDeviceSink()
 *
 * (BufferMixer エフェクト)
 * - @ref AddBufferMixer(AudioRendererConfig *pOutConfig, BufferMixerType *pBufferMixer, SubMixType *pSubMix)
 * - @ref AddBufferMixer(AudioRendererConfig *pOutConfig, BufferMixerType *pBufferMixer, FinalMixType *pFinalMix)
 * - @ref RemoveBufferMixer(AudioRendererConfig *pOutConfig, BufferMixerType *pBufferMixer, SubMixType *pSubMix)
 * - @ref RemoveBufferMixer(AudioRendererConfig *pOutConfig, BufferMixerType *pBufferMixer, FinalMixType *pFinalMix)
 *
 * (Delay エフェクト)
 * - @ref AddDelay(AudioRendererConfig *pOutConfig, DelayType *pDelay, void* buffer, size_t bufferSize, SubMixType *pSubMix, nn::TimeSpan delayTimeMax, int channelCountMax)
 * - @ref AddDelay(AudioRendererConfig *pOutConfig, DelayType *pDelay, void* buffer, size_t bufferSize, FinalMixType *pFinalMix, nn::TimeSpan delayTimeMax, int channelCountMax)
 * - @ref RemoveDelay(AudioRendererConfig *pOutConfig, DelayType *pDelay, SubMixType *pSubMix)
 * - @ref RemoveDelay(AudioRendererConfig *pOutConfig, DelayType *pDelay, FinalMixType *pFinalMix)
 *
 * (Reverb エフェクト)
 * - @ref AddReverb(AudioRendererConfig *pOutConfig, ReverbType *pReverb, void* buffer, size_t bufferSize, SubMixType *pSubMix, int channelCountMax)
 * - @ref AddReverb(AudioRendererConfig *pOutConfig, ReverbType *pReverb, void* buffer, size_t bufferSize, FinalMixType *pFinalMix, int channelCountMax)
 * - @ref RemoveReverb(AudioRendererConfig *pOutConfig, ReverbType *pReverb, SubMixType *pSubMix)
 * - @ref RemoveReverb(AudioRendererConfig *pOutConfig, ReverbType *pReverb, FinalMixType *pFinalMix)
 *
 * (Aux エフェクト)
 * - @ref AddAux(AudioRendererConfig *pOutConfig, AuxType *pAux, SubMixType *pSubMix, void *pSendBuffer, void *pReturnBuffer, size_t bufferSize)
 * - @ref AddAux(AudioRendererConfig *pOutConfig, AuxType *pAux, FinalMixType *pFinalMix, void *pSendBuffer, void *pReturnBuffer, size_t bufferSize)
 * - @ref RemoveAux(AudioRendererConfig *pOutConfig, AuxType *pAux, SubMixType *pSubMix)
 * - @ref RemoveAux(AudioRendererConfig *pOutConfig, AuxType *pAux, FinalMixType *pFinalMix)
 *
 * (I3dl2Reverb エフェクト)
 * - @ref AddI3dl2Reverb(AudioRendererConfig *pOutConfig, I3dl2ReverbType *pOutI3dl2Reverb, void* buffer, size_t bufferSize, SubMixType *pSubMix, int channelCountMax)
 * - @ref AddI3dl2Reverb(AudioRendererConfig *pOutConfig, I3dl2ReverbType *pOutI3dl2Reverb, void* buffer, size_t bufferSize, FinalMixType *pFinalMix, int channelCountMax)
 * - @ref RemoveI3dl2Reverb(AudioRendererConfig *pOutConfig, I3dl2ReverbType *pOutI3dl2Reverb, SubMixType *pSubMix)
 * - @ref RemoveI3dl2Reverb(AudioRendererConfig *pOutConfig, I3dl2ReverbType *pOutI3dl2Reverb, FinalMixType *pFinalMix)
 *
 * (BiquadFilter エフェクト)
 * - @ref AddBiquadFilter(AudioRendererConfig *pOutConfig, BiquadFilterType *pBiquadFilter, SubMixType *pSubMix)
 * - @ref AddBiquadFilter(AudioRendererConfig *pOutConfig, BiquadFilterType *pBiquadFilter, FinalMixType *pFinalMix)
 * - @ref RemoveBiquadFilter(AudioRendererConfig *pOutConfig, BiquadFilterType *pBiquadFilter, SubMixType *pSubMix)
 * - @ref RemoveBiquadFilter(AudioRendererConfig *pOutConfig, BiquadFilterType *pBiquadFilter, FinalMixType *pFinalMix)
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct AudioRendererConfig
{
    VoiceInfoManager* _pVoiceInfoManager;
    MixManager* _pMixManager;
    EffectManager* _pEffectManager;
    SinkManager* _pSinkManager;
    PerformanceBufferManager* _pPerformanceBufferManager;
    MemoryPoolManager* _pMemoryPoolManager;
    BehaviorManager* _pBehaviorManager;
    SplitterInfoManager* _pSplitterInfoManager;
    RendererInfo* _pRendererInfo;
    ExtraManager* _pExtraManager;

    void* _pInParameter;
    size_t _pInParameterSize;
    void* _pOutStatus;
    size_t _pOutStatusSize;

    void* _pConfigBuffer;
    size_t _configBufferSize;
};

}  // namespace audio
}  // namespace nn
