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
 * :include nn/atk/atk_SoundSystem.h
 *
 * @file atk_SoundSystem.h
 */

#pragma once

#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_HardwareManager.h>
#include <nn/atk/atk_MultiVoiceManager.h>
#include <nn/atk/atk_ChannelManager.h>
#include <nn/atk/atk_SoundThread.h>
#include <nn/atk/atk_MmlParser.h>
#include <nn/atk/atk_TaskThread.h>
#include <nn/atk/atk_TaskProfileReader.h>
#include <nn/atk/atk_TaskManager.h>
#include <nn/atk/atk_ThreadInfoReader.h>
#include <nn/atk/atk_ThreadStack.h>
#include <nn/atk/atk_Util.h>
#include <nn/atk/fnd/basis/atkfnd_Inlines.h>
#include <nn/atk/atk_WarningCallback.h>

namespace nn {
namespace atk {

class AudioRendererPerformanceReader;

//---------------------------------------------------------------------------
//! @brief  サウンドシステム全体に関わる設定を行います。
//!
//---------------------------------------------------------------------------
class SoundSystem
{
public:

    //---------------------------------------------------------------------------
    //! @brief  サウンドシステムの初期化に使用する構造体です。
    //!
    //! @see SoundSystem::Initialize
    //!
    //---------------------------------------------------------------------------
    struct SoundSystemParam
    {
        //! @brief 内部で使用する nn::audio オーディオレンダラのサンプルレートのデフォルト値です。
        static const int DefaultRendererSampleRate = detail::driver::HardwareManager::DefaultRendererSampleRate;

        //! @brief サウンドスレッドプライオリティのデフォルト値です。
        static const int32_t DefaultSoundThreadPriority = 4;

        //! @brief タスクスレッドプライオリティのデフォルト値です。
        static const int32_t DefaultTaskThreadPriority = 3;

        //! @brief オーディオレンダラで使用可能なエフェクト数のデフォルト値です。
        static const int DefaultUserEffectCount = detail::driver::HardwareManager::DefaultRendererUserEffectCount;

        //! @brief オーディオレンダラで使用する最大ボイス数のデフォルト値です。
        static const int DefaultVoiceCountMax = detail::driver::HardwareManager::DefaultRendererVoiceCountMax;

        //! @brief サウンドスレッドスタックサイズのデフォルト値です。
        static const uint32_t DefaultSoundThreadStackSize = 16 * 1024;

        //! @brief タスクスレッドスタックサイズのデフォルト値です。
        static const uint32_t DefaultTaskThreadStackSize = 16 * 1024;

        //---------------------------------------------------------------------------
        //! @briefprivate
        //!
        //! @brief    サウンドスレッドのコマンドバッファサイズのデフォルト値です。
        //---------------------------------------------------------------------------
        static const uint32_t DefaultSoundThreadCommandBufferSize = 128 * 1024;

        //---------------------------------------------------------------------------
        //! @briefprivate
        //!
        //! @brief    タスクスレッドのコマンドバッファサイズのデフォルト値です。
        //---------------------------------------------------------------------------
        static const uint32_t DefaultTaskThreadCommandBufferSize = 8 * 1024;

        //---------------------------------------------------------------------------
        //! @briefprivate
        //!
        //! @brief    サウンドスレッドのコマンドキューカウントのデフォルト値です。
        //---------------------------------------------------------------------------
        static const int DefaultSoundThreadCommandQueueCount = 32;

        //---------------------------------------------------------------------------
        //! @briefprivate
        //!
        //! @briefprivate タスクスレッドのコマンドキューカウントのデフォルト値です。
        //---------------------------------------------------------------------------
        static const int DefaultTaskThreadCommandQueueCount = 32;

        //! @brief 最終出力波形を録音するバッファのサイズのデフォルト値です。
        static const int DefaultRecordingAudioFrameCount = detail::driver::HardwareManager::DefaultRecordingAudioFrameCount;

        //! @brief タスクスレッドが動作するコア番号のデフォルト値です。
        static const int DefaultTaskThreadCoreNumber = 0;

        //! @brief サウンドスレッドが動作するコア番号のデフォルト値です。
        static const int DefaultSoundThreadCoreNumber = 0;

        //! @briefprivate
        //! @brief ユーザー定義サブミックスが利用する最大バス数のデフォルト値です。
        static const int DefaultBusCountMax = 4;

        //! @internal
        static const int InvalidMixBufferCount = -1;

        //! @brief ストリームサウンドの再生など、タスクスレッドを使用したファイルロードを行う際の fs アクセス優先度のデフォルト値です。
        static const FsPriority DefaultTaskThreadFsPriority = FsPriority_Normal;

        //---------------------------------------------------------------------------
        //! @brief    コンストラクタです。
        //---------------------------------------------------------------------------
        SoundSystemParam() NN_NOEXCEPT;

        //---------------------------------------------------------------------------
        //! @brief    サウンドスレッドのプライオリティです。初期値は @ref DefaultSoundThreadPriority です。
        //---------------------------------------------------------------------------
        int32_t soundThreadPriority;

        //---------------------------------------------------------------------------
        //! @brief    サウンドスレッドのスタックサイズです。@ref nn::os::ThreadStackAlignment の倍数で指定してください。
        //!           初期値は @ref DefaultSoundThreadStackSize です。
        //---------------------------------------------------------------------------
        size_t soundThreadStackSize;

        //---------------------------------------------------------------------------
        //! @briefprivate
        //!
        //! @brief    サウンドスレッドのコマンドバッファサイズです。4 の倍数で指定してください。
        //!           初期値は @ref DefaultSoundThreadCommandBufferSize です。
        //---------------------------------------------------------------------------
        size_t soundThreadCommandBufferSize;

        //---------------------------------------------------------------------------
        //! @briefprivate
        //!
        //! @brief    サウンドスレッドのコマンドキューカウントです。
        //!           初期値は @ref DefaultSoundThreadCommandQueueCount です。
        //---------------------------------------------------------------------------
        int soundThreadCommandQueueCount;

        //! @briefprivate
        size_t voiceCommandBufferSize;

        //---------------------------------------------------------------------------
        //! @brief    タスクスレッドのプライオリティです。初期値は @ref DefaultTaskThreadPriority です。
        //---------------------------------------------------------------------------
        int32_t taskThreadPriority;

        //---------------------------------------------------------------------------
        //! @brief    タスクスレッドのスタックサイズです。
        //!           @ref nn::os::ThreadStackAlignment の倍数で指定してください。初期値は @ref DefaultTaskThreadStackSize です。
        //---------------------------------------------------------------------------
        size_t taskThreadStackSize;

        //---------------------------------------------------------------------------
        //! @briefprivate
        //!
        //! @brief    タスクスレッドのコマンドバッファサイズです。
        //!           4 の倍数で指定してください。初期値は @ref DefaultTaskThreadCommandBufferSize です。
        //---------------------------------------------------------------------------
        size_t taskThreadCommandBufferSize;

        //---------------------------------------------------------------------------
        //! @briefprivate
        //!
        //! @brief    タスクスレッドのコマンドキューカウントです。
        //!           初期値は @ref DefaultTaskThreadCommandQueueCount です。
        //---------------------------------------------------------------------------
        int taskThreadCommandQueueCount;

        //---------------------------------------------------------------------------
        //! @brief ストリームサウンドの再生など、タスクスレッドを使用したファイルロードを行う際の fs 優先度です。初期値は @ref DefaultTaskThreadFsPriority です。
        //---------------------------------------------------------------------------
        FsPriority taskThreadFsPriority;

        //---------------------------------------------------------------------------
        //! @briefprivate
        //! @deprecated SoundSystemParam::rendererSampleRate で 48kHz を指定することで制約もなく 48kHz レンダリングできるためそちらを使用してください。
        //!             互換性のために残されていますが、これらの API はすでに使用できない状態になっていて、今後削除される予定です。
        //!
        //! @brief  NW レンダラを利用するかどうかを示すフラグです。
        //!         初期値は false です。
        //!         true にすると、次の制約がありますが、
        //!         48KHz でボイスのレンダリングすることが可能です。
        //!
        //!         - @ref GetRequiredMemSize のサイズが増えます
        //!         - サウンドスレッドの処理負荷が増えます
        //!         - エフェクト・フィルターは効きません
        //---------------------------------------------------------------------------
        bool enableNwRenderer;

        //---------------------------------------------------------------------------
        //! @briefprivate
        //! @deprecated SoundSystemParam::rendererSampleRate で 48kHz を指定することで制約もなく 48kHz レンダリングできるためそちらを使用してください。
        //!             互換性のために残されていますが、これらの API はすでに使用できない状態になっていて、今後削除される予定です。
        //!
        //! @brief    NW レンダラ用シンセサイズバッファの数です。
        //---------------------------------------------------------------------------
        uint32_t nwVoiceSynthesizeBufferCount;

        //---------------------------------------------------------------------------
        //! @brief    内部で使用する nn::audio のオーディオレンダラのサンプルレートの値です。初期値は @ref DefaultRendererSampleRate です。
        //---------------------------------------------------------------------------
        int rendererSampleRate;

        //---------------------------------------------------------------------------
        //! @brief  オーディオレンダラで使用可能なエフェクト数です。初期値は @ref DefaultUserEffectCount です。
        //---------------------------------------------------------------------------
        int effectCount;

        //---------------------------------------------------------------------------
        //! @brief  オーディオレンダラで使用する最大ボイス数です。初期値は @ref DefaultVoiceCountMax です。
        //---------------------------------------------------------------------------
        int voiceCountMax;

        //! @internal
        int voiceCommandWaveBufferPacketCount;

        //! @brief  プロファイラを利用するかどうかを示すフラグです。初期値は false です。
        bool enableProfiler;

        //! @internal
        bool enableDetailSoundThreadProfile;

        //! @internal
        //! @brief 最終出力波形を録音する機能を利用するかどうかを示すフラグです。初期値は false です。
        //!        @ref nn::atk::DeviceOutRecorder を用いて最終出力波形を録音する場合は、このフラグを true に設定する必要があります。
        bool enableRecordingFinalOutputs;

        //! @brief   CircularBufferSink を利用するかどうかを示すフラグです。初期値は false です。
        //! @details CircularBufferSink を用いてレンダリング結果を録音する場合は、このフラグを true に設定する必要があります。
        //!          録音に使用するバッファの長さは @ref recordingAudioFrameCount により決定されます。録音波形の取りこぼしが発生する場合はこの値を増やしてください。
        bool enableCircularBufferSink;

        //! @brief レンダリング結果を録音するバッファに確保できるオーディオフレーム数です。初期値は @ref DefaultRecordingAudioFrameCount です。
        //! @details フレーム数を減らすことで使用メモリを切り詰められますが、録音波形の取りこぼしが起きる可能性が大きくなります。
        int recordingAudioFrameCount;

        //! @brief サウンドスレッドが動作するコアの番号です。初期値は @ref DefaultSoundThreadCoreNumber です。
        int soundThreadCoreNumber;

        //! @brief タスクスレッドが動作するコアの番号です。初期値は @ref DefaultTaskThreadCoreNumber です。
        int taskThreadCoreNumber;

        //! @briefprivate
        //! @brief サブミックスを追加し、追加のエフェクトバスを使用可能にする評価用機能です。
        //!        従来の 6ch x 4bus の構成に加えて、2ch x 3bus のサブミックスが追加されます。初期値は false です。
        //!        追加のエフェクトバスに nn::atk::SoundSystem::AppendEffect でエフェクトを追加する場合は、2ch 以下のチャンネル数のものを利用し、
        //!        エフェクトのチャンネル設定は、 nn::atk::ChannelIndex_FrontLeft および nn::atk::ChannelIndex_FrontRight のみを利用してください
        //!
        //!        この設定を使用している場合は、 @ref enableCustomSubMix で設定できるユーザー定義サブミックスの設定は反映されませんのでご注意ください。
        bool enableAdditionalEffectBus;

        //! @briefprivate
        //! @brief 追加のサブミックスを使用可能にする評価用機能です。
        //!        通常のサブミックス, 追加のサブミックス, ファイナルミックスの順にミックスされるようになります。
        //!        追加のサブミックスには nn::atk::EffectAux を追加することができます。
        //!        操作は通常のサブミックスにエフェクトを追加するときと同じように nn::atk::SoundSystem::AppendEffectToAdditionalSubMix で追加し、
        //!        nn::atk::SoundSystem::ClearEffectFromAdditionalSubMix または nn::atk::SoundSystem::RemoveEffectFromAdditionalSubMix で削除し、
        //!        nn::atk::SoundSystem::IsClearEffectFromAdditionalSubMixFinished で削除の確認を行ってください。
        //!
        //!        この設定を使用している場合は、 @ref enableCustomSubMix で設定できるユーザー定義サブミックスの設定は反映されませんのでご注意ください。
        bool enableAdditionalSubMix;

        //! @briefprivate
        //! @brief タスクスレッドを利用するかどうかを示すフラグです。初期値は true です。
        //!        この値を false にする場合は、ストリームサウンド、プレイヤーヒープといった
        //!        タスクスレッドを必要とする再生手段を使わないことを必ず確認してください。
        //!
        //!        この値が false の場合、タスクスレッドは作られなくなり
        //!        @ref taskThreadStackSize, @ref taskThreadCommandBufferSize の値に 0 を設定する事が可能になります。
        //!        また、ストリームサウンドやプレイヤーヒープを用いた再生を行おうとするとアサートが発生します。
        bool enableTaskThread;

        //! @briefprivate
        //! @brief サウンドスレッドを利用するかどうかを示すフラグです。初期値は true です。
        //!        この値を false にする場合は、アプリ側で VoiceCommand 版 atk ライブラリを使用していて
        //!        かつ VoiceCommandUpdate() でサウンドスレッドと同様のオーディオフレーム更新処理を呼び出していることを必ず確認してください。
        //!
        //!        この値が false の場合、サウンドスレッドは作られなくなり soundThreadStackSize の値に 0 を設定する事が可能になります。
        //!        また、VoiceCommand 版 atk ライブラリでサウンドスレッドを利用しない場合、
        //!        アプリ側のタイミングでオーディオフレームの更新処理を呼び出す事が可能になります。
        //!        更新処理が行われない場合、再生が正常に行われないほか、最悪 atk ライブラリの停止が発生します。
        bool enableSoundThread;

        //! @brief サウンドシステムが必要とするメモリプールを
        //!        サウンドシステムが管理するかどうかを示すフラグです。初期値は true です。
        //! @details メモリプールの管理をアプリケーション側で行う場合は　false を設定し、
        //!          @ref SoundSystem::GetRequiredMemSizeForMemoryPool で取得したサイズ分のバッファを
        //!          @ref SoundSystem::Initialize の memoryPoolMem に渡してください。
        //! @see SoundSystem::GetRequiredMemSizeForMemoryPool
        //! @see SoundSystem::Initialize
        bool enableMemoryPoolManagement;

        //! @briefprivate
        //! @brief   enableCircularBufferSink を true にした場合に消費するバッファを
        //!          サウンドシステムの初期化バッファ（workMem）から確保するかどうかを示すフラグです。初期値は true です。
        //! @details サウンドシステムの初期化バッファ以外からメモリを確保したい場合は　false を設定し、
        //!          @ref SoundSystem::GetRequiredMemSizeForCircularBufferSink で取得したサイズ分のバッファを
        //!          @ref SoundSystem::InitializeBufferSet::circularBufferSinkMem に渡し、
        //!          @ref SoundSystem::InitializeBufferSet を使用する @ref SoundSystem::Initialize で初期化してください。
        //!
        //! @see SoundSystem::GetRequiredMemSizeForCircularBufferSink
        //! @see SoundSystem::Initialize
        bool enableCircularBufferSinkBufferManagement;

        //! @briefprivate
        //! @brief エフェクトを利用するかどうかを示すフラグです。初期値は true です。
        //!        この値を false にする場合はエフェクトを使用しないことを必ず確認してください。
        //!
        //!        この値が false の場合、エフェクトの使用に用いるバッファが不要になり
        //!        サウンドシステムの初期化に必要なバッファのサイズを減らすことができます。
        //!        また、サウンドシステムへエフェクトの着脱を行おうとするとアサートが発生します。
        bool enableEffect;

        //! @briefprivate
        //! @brief SubMix を利用するかどうかを示すフラグです。初期値は true です。
        //!        この値を false にする場合は SubMix へのエフェクトの追加をしないことを必ず確認してください。
        //!
        //!        この値が false の場合、Voice の出力先が SubMix を経由しなくなるため
        //!        オーディオレンダラの処理負荷を軽減することができます。
        //!        また、SubMix へエフェクトの着脱を行おうとするとアサートが発生します。
        bool enableSubMix;

        //! @briefprivate
        //! @brief 内部的に 3ch 以上の計算処理を行わないようにするフラグです。初期値は false です。
        //!
        //! @details この値が true の場合、処理対象のチャンネル数が減るため
        //!          オーディオレンダラの処理負荷を軽減することができます。
        //!          2ch の処理で十分な場合のみ使用するようにしてください。
        bool enableStereoMode;

        //! @brief ボイスドロップを利用するかどうかを示すフラグです。初期値は true です。
        bool enableVoiceDrop;

        //! @brief NW4F の nw::snd 互換のダウンミックス処理を有効にするかどうかを示すフラグです。初期値は false です。
        //!
        //! @details この値を true にすると、 Atk ライブラリの出力チャンネル数と実際のオーディオデバイスの出力チャンネル数
        //!          に食い違いがあった場合に行われるダウンミックス処理を NW4F の nw::snd 互換の処理に変更します。
        //!          NW4F の nw::snd 互換の処理を有効にした場合、システムデフォルトのダウンミックス処理ではなく、
        //!          Atk ライブラリが内部で管理するオーディオレンダラによるダウンミックス処理となるため、
        //!          アプリケーション側の処理負荷となってしまう点に注意してください。
        bool enableCompatibleDownMixSetting;

        //! @brief   NintendoSDK 0.16.5 以前の LPF の計算処理を行うフラグです。初期値は false です。
        //! @details NintendoSDK 0.16.5 以前との計算の互換性を保つために用意されています。
        //!          SetLowPassFilter などの API を使用する際、 NintendoSDK 0.16.5 以前の LPF のフィルタ処理と同様のフィルタ処理を行いたい場合、
        //!          こちらの値を true にしてください。このパラメーターは将来的に削除予定です。
        bool enableCompatibleLowPassFilter;

        //! @briefprivate
        //! @brief エフェクトが有効でないチャンネルをミュートするかどうかのフラグです。初期値は false です。
        bool enableUnusedEffectChannelMuting;

        //! @briefprivate
        //! @brief NW4F の nw::snd 互換のエフェクトバスの音量計算を有効にするかどうかを示すフラグです。初期値は false です。
        //!
        //! @details この値を true にすると、エフェクトを使用していないバスに
        //!          エフェクトセンド量を設定した場合の挙動を NW4F の nw::snd 互換の処理に変更します。
        //!
        //!          通常はエフェクトを使用していないバスにエフェクトセンド量を指定すると
        //!          メインセンドとミックスされて最終出力に反映されますが、
        //!          この値が true の場合は、エフェクトを使用していないバスの音量はミュートされ
        //!          エフェクトセンド量を指定しても最終出力に反映されなくなります。
        bool enableCompatibleBusVolume;

        //! @brief NW4F の nw::snd 互換のパンカーブを有効にするかどうかを示すフラグです。初期値は false です。
        //!
        //! @details この値を true にすると、パンカーブが NW4F の nw::snd 相当のものとなります。
        //!          具体的には、以下の 2 つのことが起こります。
        //!          - ストリームサウンドの再生時、パンカーブが nn::atk::PanCurve_Sqrt 、パンモードが nn::atk::PanMode_Dual に固定されます
        //!          - @ref nn::atk::SoundSystem::SetOutputMode で @ref nn::atk::OutputMode_Surround を指定した上で、
        //!            pan の値が -0.5f 以下または 0.5f 以上になると、サウンドのボリュームが小さくなります。
        //!              - パンモードにデュアルパンを設定してステレオのサウンドを再生した場合も、
        //!                内部処理により pan の値がこの条件を満たすため、
        //!                同様にボリューム変化がある点に注意してください。 (L, R に対して -1.0f, 1.0f の pan が指定されます)
        //!                詳細なパンカーブに関しては、 @confluencelink{83955834, パンの動作} のページをご参照ください。
        bool enableCompatiblePanCurve;

        //! @internal
        //! @brief ユーザースレッドによるレンダリング処理を利用するかどうかを示すフラグです。初期値は false です。
        //         機能を有効化した場合、サウンドフレーム処理は ExecuteRendering() が呼び出される度に、サウンドスレッド上で実行されます。
        //         よって、ExecuteRendering() をユーザが定期的に呼び出す必要があります。
        //         また、サウンドレンダリング処理は本 API の内部で実行されるため、要求されている再生内容によって本 API の処理時間は変動します。
        //
        //         本機能は以下の機能と排他利用です。以下の機能は設定を行っても実際には有効にはなりません。
        //         - レンダリング結果の出力先デバイスからの再生
        //             -もし再生したい場合は、ReadCircularBufferSink() を利用してレンダリング結果を取得して再生を行ってください。
        //         - ボイスドロップ機能
        //         - オーディオレンダラのレンダリング時間の割合指定
        bool enableUserThreadRendering;

        //! @brief ユーザ定義のサブミックス構成を利用できるようにするかどうかを示すフラグです。初期値は false です。
        //!        ユーザ定義のサブミックス構成を利用するためには、このフラグを true に設定し、
        //!        @ref subMixCount, @ref subMixTotalChannelCount を設定してください。
        bool enableCustomSubMix;

        //! @brief ユーザ定義のサブミックス構成を利用する際に、同時に使用されるサブミックスの最大数を設定します。
        //!        ユーザ定義のサブミックス構成の利用時、デフォルトでは 0 の値が設定されます。
        int subMixCount;

        //! @brief ユーザ定義のサブミックス構成を利用する際に、同時に使用されるチャンネルの合計数を設定します。
        //!        1 つのサブミックスが使用するチャンネルの合計数は、 (サブミックスのバス数) * (サブミックスのチャンネル数) で表され、
        //!        使用するサブミックスすべてのチャンネルの合計数を足したものを設定します。
        //!        ユーザ定義のサブミックス構成を利用する場合、デフォルトは 0 が設定されます。
        int subMixTotalChannelCount;

        //! @internal
        //! @deprecated このパラメーターは廃止予定です。代わりに @ref subMixTotalChannelCount を使用してください。
        int mixBufferCount;

        //! @brief   ユーザー定義サブミックスが利用する最大のバス数です。初期値は DefaultBusCountMax です。
        //!          DefaultBusCountMax 以上のバス数を利用したい場合、この値を変更してください。
        //!          設定できる数は 1 ～ @ref OutputReceiver::BusCountMax の範囲です。
        int busCountMax;

        //! @briefprivate
        //! @brief   メモリプールアタッチのチェック機能を有効にするかどうかを示すフラグです。初期値は false です。
        //! @details チェック機能を有効にした場合、メモリプールへのアタッチが必要なバッファがアタッチされずに渡されると
        //!          オーディオレンダラの更新処理時にアボートが発生します。
        bool enableMemoryPoolAttachCheck;

        //! @briefprivate
        //! @brief   バスごとにミックスボリュームを設定する機能を有効にするかどうかを示すフラグです。初期値は false です。
        bool enableBusMixVolume;

        //! @briefprivate
        //! @brief   バスごとにボリュームの設定をスルーする機能を有効にするかどうかを示すフラグです。初期値は false です。
        bool enableVolumeThroughMode;
    };

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  サウンドシステムの初期化に使用する構造体です。
    //!
    //! @see SoundSystem::Initialize
    //!
    //! @pre
    //! - workMem が WorkMemoryAlignSize にアラインしている
    //! - memoryPoolMem が nn::audio::BufferAlignSize にアラインしている（使用する場合）
    //---------------------------------------------------------------------------
    struct InitializeBufferSet
    {
        //! @brief サウンドシステムのワークメモリへのポインタです。
        uintptr_t workMem;

        //! @brief サウンドシステムのワークメモリのサイズです。
        size_t workMemSize;

        //! @brief サウンドシステムのワークメモリのうち、メモリプール管理されるメモリへのポインタです。
        uintptr_t memoryPoolMem;

        //! @brief サウンドシステムのワークメモリのうち、メモリプール管理されるメモリのサイズです。
        size_t memoryPoolMemSize;

        //! @brief CircularBufferSink 用のメモリへのポインタです。
        uintptr_t circularBufferSinkMem;

        //! @brief CircularBufferSink 用のメモリのサイズです。
        size_t circularBufferSinkMemSize;
    };

    //---------------------------------------------------------------------------
    //! @brief      サウンドライブラリが扱える最大ボイス数です。
    //---------------------------------------------------------------------------
    static const int VoiceCountMax = detail::driver::HardwareManager::AtkVoiceCountMax;

    //---------------------------------------------------------------------------
    //! @brief      ワークメモリのアラインサイズです。
    //---------------------------------------------------------------------------
    static const uint32_t WorkMemoryAlignSize = 4;

    //----------------------------------------
    //! @name システム
    //@{
    //---------------------------------------------------------------------------
    //! @brief  サウンドライブラリの初期化を行います。
    //!
    //!         サウンドシステムが使用するワークメモリをユーザが渡します。
    //!         @ref SoundSystemParam 構造体に、初期化に必要なパラメータを設定し、
    //!         @ref nn::atk::SoundSystem::GetRequiredMemSize で、
    //!         必要なメモリサイズを取得してください。
    //!
    //!         @ref SoundSystemParam 構造体で指定した優先度・スタックサイズで、
    //!         サウンド処理を行うスレッドと、
    //!         ストレージからサウンドデータをロードするためのスレッドの初期化、実行します。
    //!
    //! @param[in]    param         初期化パラメータです。
    //! @param[in]    workMem       サウンドシステムのワークメモリへのポインタです。
    //! @param[in]    workMemSize   ワークメモリのサイズです。
    //! @return       サウンドライブラリの初期化に成功した場合は true を返します。
    //!
    //! @pre
    //! - workMem が WorkMemoryAlignSize にアラインしている
    //! - param.soundThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.taskThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.rendererSampleRate == 32000 || param.rendererSampleRate == 48000
    //! - param.voiceCountMax >= 1 && param.voiceCountMax <= SoundSystem::VoiceCountMax
    //! - param.busCountMax >= 1 && param.busCountMax <= OutputReceiver::BusCountMax
    //!
    //! @see Finalize
    //! @see GetRequiredMemSize
    //! @see SoundSystemParam
    //! @see IsInitialized
    //---------------------------------------------------------------------------
    static bool Initialize(
        const SoundSystemParam& param,
        uintptr_t workMem,
        size_t workMemSize
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  サウンドライブラリの初期化を行います。
    //!
    //!         サウンドシステムが使用するワークメモリをユーザが渡します。
    //!         @ref SoundSystemParam 構造体に、初期化に必要なパラメータを設定し、
    //!         @ref nn::atk::SoundSystem::GetRequiredMemSize で、
    //!         必要なメモリサイズを取得してください。
    //!
    //!         @ref SoundSystemParam 構造体で指定した優先度・スタックサイズで、
    //!         サウンド処理を行うスレッドと、
    //!         ストレージからサウンドデータをロードするためのスレッドの初期化、実行します。
    //!
    //! @param[out]   pOutResult    処理の結果を格納する Result です。
    //!                             処理の結果が必要ない場合は nullptr を渡すか、もしくは引数に pOutResult をもたない Initialize を使用してください。
    //!                             返る Result の値は @ref nn::audio::OpenAudioRenderer が返す値と一緒です。詳細は @ref nn::audio::OpenAudioRenderer を参照してください。
    //! @param[in]    param         初期化パラメータです。
    //! @param[in]    workMem       サウンドシステムのワークメモリへのポインタです。
    //! @param[in]    workMemSize   ワークメモリのサイズです。
    //! @return       処理の結果が返ります。
    //!
    //! @pre
    //! - workMem が WorkMemoryAlignSize にアラインしている
    //! - param.soundThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.taskThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.rendererSampleRate == 32000 || param.rendererSampleRate == 48000
    //! - param.voiceCountMax >= 1 && param.voiceCountMax <= SoundSystem::VoiceCountMax
    //! - param.busCountMax >= 1 && param.busCountMax <= OutputReceiver::BusCountMax
    //!
    //! @see Finalize
    //! @see GetRequiredMemSize
    //! @see SoundSystemParam
    //! @see IsInitialized
    //---------------------------------------------------------------------------
    static bool Initialize(
        Result* pOutResult,
        const SoundSystemParam& param,
        uintptr_t workMem,
        size_t workMemSize
        ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  サウンドライブラリの初期化を行います。
    //!
    //!         サウンドシステムが使用するワークメモリとメモリプール管理されるメモリをユーザが渡します。
    //!         @ref SoundSystemParam 構造体に、初期化に必要なパラメータを設定し、
    //!         @ref nn::atk::SoundSystem::GetRequiredMemSize で、ワークメモリに必要なメモリサイズを取得し、
    //!         @ref nn::atk::SoundSystem::GetRequiredMemSizeForMemoryPool で、メモリプール管理されるメモリに必要なメモリサイズを取得してください。
    //!
    //!         メモリプール管理されるメモリは初期化後にメモリプールにアタッチしてください。
    //!
    //!         @ref SoundSystemParam 構造体で指定した優先度・スタックサイズで、
    //!         サウンド処理を行うスレッドと、
    //!         ストレージからサウンドデータをロードするためのスレッドの初期化、実行します。
    //!
    //! @param[in]    param               初期化パラメータです。
    //! @param[in]    workMem             サウンドシステムのワークメモリへのポインタです。
    //! @param[in]    workMemSize         ワークメモリのサイズです。
    //! @param[in]    memoryPoolMem       メモリプール管理されるメモリへのポインタです。
    //! @param[in]    memoryPoolMemSize   メモリプール管理されるメモリのサイズです。
    //! @return       サウンドライブラリの初期化に成功した場合は true を返します。
    //!
    //! @pre
    //! - workMem が WorkMemoryAlignSize にアラインしている
    //! - memoryPoolMem が nn::audio::BufferAlignSize にアラインしている
    //! - param.soundThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.taskThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.rendererSampleRate == 32000 || param.rendererSampleRate == 48000
    //! - param.voiceCountMax >= 1 && param.voiceCountMax <= SoundSystem::VoiceCountMax
    //! - param.busCountMax >= 1 && param.busCountMax <= OutputReceiver::BusCountMax
    //!
    //! @see Finalize
    //! @see GetRequiredMemSize
    //! @see GetRequiredMemSizeForMemoryPool
    //! @see SoundSystemParam
    //! @see IsInitialized
    //---------------------------------------------------------------------------
    static bool Initialize(
        const SoundSystemParam& param,
        uintptr_t workMem,
        size_t workMemSize,
        uintptr_t memoryPoolMem,
        size_t memoryPoolMemSize
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  サウンドライブラリの初期化を行います。
    //!
    //!         サウンドシステムが使用するワークメモリとメモリプール管理されるメモリをユーザが渡します。
    //!         @ref SoundSystemParam 構造体に、初期化に必要なパラメータを設定し、
    //!         @ref nn::atk::SoundSystem::GetRequiredMemSize で、ワークメモリに必要なメモリサイズを取得し、
    //!         @ref nn::atk::SoundSystem::GetRequiredMemSizeForMemoryPool で、メモリプール管理されるメモリに必要なメモリサイズを取得してください。
    //!
    //!         メモリプール管理されるメモリは初期化後にメモリプールにアタッチしてください。
    //!
    //!         @ref SoundSystemParam 構造体で指定した優先度・スタックサイズで、
    //!         サウンド処理を行うスレッドと、
    //!         ストレージからサウンドデータをロードするためのスレッドの初期化、実行します。
    //!
    //! @param[out]   pOutResult          処理の結果を格納する Result です。
    //!                                   処理の結果が必要ない場合は nullptr を渡すか、もしくは引数に pOutResult をもたない Initialize を使用してください。
    //!                                   返る Result の値は @ref nn::audio::OpenAudioRenderer が返す値と一緒です。詳細は @ref nn::audio::OpenAudioRenderer を参照してください。
    //! @param[in]    param               初期化パラメータです。
    //! @param[in]    workMem             サウンドシステムのワークメモリへのポインタです。
    //! @param[in]    workMemSize         ワークメモリのサイズです。
    //! @param[in]    memoryPoolMem       メモリプール管理されるメモリへのポインタです。
    //! @param[in]    memoryPoolMemSize   メモリプール管理されるメモリのサイズです。
    //! @return       サウンドライブラリの初期化に成功した場合は true を返します。
    //!
    //! @pre
    //! - workMem が WorkMemoryAlignSize にアラインしている
    //! - memoryPoolMem が nn::audio::BufferAlignSize にアラインしている
    //! - param.soundThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.taskThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.rendererSampleRate == 32000 || param.rendererSampleRate == 48000
    //! - param.voiceCountMax >= 1 && param.voiceCountMax <= SoundSystem::VoiceCountMax
    //! - param.busCountMax >= 1 && param.busCountMax <= OutputReceiver::BusCountMax
    //!
    //! @see Finalize
    //! @see GetRequiredMemSize
    //! @see GetRequiredMemSizeForMemoryPool
    //! @see SoundSystemParam
    //! @see IsInitialized
    //---------------------------------------------------------------------------
    static bool Initialize(
        Result* pOutResult,
        const SoundSystemParam& param,
        uintptr_t workMem,
        size_t workMemSize,
        uintptr_t memoryPoolMem,
        size_t memoryPoolMemSize
        ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  サウンドライブラリの初期化を行います。
    //!
    //!         @ref SoundSystemParam 構造体に、初期化に必要なパラメータを設定し、
    //!         @ref InitializeBufferSet 構造体に、必要なバッファを設定してください。
    //!
    //!         @InitializeBufferSet でバッファを設定した場合でも、
    //!         @ref SoundSystemParam で対応するフラグが有効になっていない場合
    //!         設定したバッファは無視される点にご注意ください。
    //!
    //!         メモリプール管理されるメモリは初期化後にメモリプールにアタッチしてください。
    //!
    //!         @ref SoundSystemParam 構造体で指定した優先度・スタックサイズで、
    //!         サウンド処理を行うスレッドと、
    //!         ストレージからサウンドデータをロードするためのスレッドの初期化、実行します。
    //!
    //! @param[in]    param               初期化パラメータです。
    //! @param[in]    bufferSet           初期化に使用するバッファをまとめて格納する構造体です。
    //! @return       サウンドライブラリの初期化に成功した場合は true を返します。
    //!
    //! @pre
    //! - InitializeBufferSet の事前条件を満たしている
    //! - param.soundThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.taskThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.rendererSampleRate == 32000 || param.rendererSampleRate == 48000
    //! - param.voiceCountMax >= 1 && param.voiceCountMax <= SoundSystem::VoiceCountMax
    //! - param.busCountMax >= 1 && param.busCountMax <= OutputReceiver::BusCountMax
    //!
    //! @see Finalize
    //! @see SoundSystemParam
    //! @see InitializeBufferSet
    //! @see IsInitialized
    //---------------------------------------------------------------------------
    static bool Initialize(
        const SoundSystemParam& param,
        InitializeBufferSet& bufferSet
        ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  サウンドライブラリの初期化を行います。
    //!
    //!         @ref SoundSystemParam 構造体に、初期化に必要なパラメータを設定し、
    //!         @ref InitializeBufferSet 構造体に、必要なバッファを設定してください。
    //!
    //!         メモリプール管理されるメモリは初期化後にメモリプールにアタッチしてください。
    //!
    //!         @ref SoundSystemParam 構造体で指定した優先度・スタックサイズで、
    //!         サウンド処理を行うスレッドと、
    //!         ストレージからサウンドデータをロードするためのスレッドの初期化、実行します。
    //!
    //! @param[out]   pOutResult          処理の結果を格納する Result です。
    //!                                   処理の結果が必要ない場合は nullptr を渡すか、もしくは引数に pOutResult をもたない Initialize を使用してください。
    //!                                   返る Result の値は @ref nn::audio::OpenAudioRenderer が返す値と一緒です。詳細は @ref nn::audio::OpenAudioRenderer を参照してください。
    //! @param[in]    param               初期化パラメータです。
    //! @param[in]    bufferSet           初期化に使用するバッファをまとめて格納する構造体です。
    //! @return       サウンドライブラリの初期化に成功した場合は true を返します。
    //!
    //! @pre
    //! - InitializeBufferSet の事前条件を満たしている
    //! - param.soundThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.taskThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.rendererSampleRate == 32000 || param.rendererSampleRate == 48000
    //! - param.voiceCountMax >= 1 && param.voiceCountMax <= SoundSystem::VoiceCountMax
    //! - param.busCountMax >= 1 && param.busCountMax <= OutputReceiver::BusCountMax
    //!
    //! @see Finalize
    //! @see SoundSystemParam
    //! @see InitializeBufferSet
    //! @see IsInitialized
    //---------------------------------------------------------------------------
    static bool Initialize(
        Result* pOutResult,
        const SoundSystemParam& param,
        InitializeBufferSet& bufferSet
        ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        サウンドライブラリの初期化に必要なメモリサイズを取得します。
    //!
    //!               param に設定されている初期化パラメータに基づいて、
    //!               サウンドシステムのセットアップに必要なメモリサイズを計算します。
    //!
    //!               この関数で取得したサイズのメモリ領域を
    //!               @ref nn::atk::SoundSystem::Initialize に渡してください。
    //!
    //! @param[in]    param   初期化パラメータです。
    //!
    //! @return       サウンドシステムの初期化に必要なメモリサイズを返します。
    //! @pre
    //! - param.soundThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.taskThreadStackSize が nn::os::ThreadStackAlignment にアラインしている
    //! - param.rendererSampleRate == 32000 || param.rendererSampleRate == 48000
    //! - param.voiceCountMax >= 1 && param.voiceCountMax <= SoundSystem::VoiceCountMax
    //! - param.busCountMax >= 1 && param.busCountMax <= OutputReceiver::BusCountMax
    //!
    //! @see  Initialize
    //! @see  SoundSystemParam
    //!
    //---------------------------------------------------------------------------
    static size_t GetRequiredMemSize( const SoundSystemParam& param ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        サウンドライブラリのメモリプールに必要なメモリサイズを取得します。
    //!
    //!               param に設定されている初期化パラメータに基づいて、
    //!               サウンドシステムのメモリプールに必要なメモリサイズを計算します。
    //!
    //!               この関数で取得したサイズのメモリ領域を
    //!               @ref nn::atk::SoundSystem::Initialize の memoryPoolMem に渡してください。
    //!
    //! @param[in]    param   初期化パラメータです。
    //!
    //! @return       サウンドシステムのメモリプールに必要なメモリサイズを返します。
    //!
    //! @see  Initialize
    //! @see  SoundSystemParam
    //!
    //---------------------------------------------------------------------------
    static size_t GetRequiredMemSizeForMemoryPool( const SoundSystemParam& param ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief        CircularBufferSink を使用するために必要なメモリサイズを取得します。
    //!
    //!               param に設定されている初期化パラメータに基づいて、
    //!               CircularBufferSink を使用するために必要なメモリサイズを計算します。
    //!
    //!               この関数で取得したサイズのメモリ領域を
    //!               @ref nn::atk::SoundSystem::InitializeBufferSet の circularBufferSinkMem に渡してください。
    //!
    //! @param[in]    param   初期化パラメータです。
    //!
    //! @return       CircularBufferSink を使用するために必要なメモリサイズを返します。
    //!
    //! @see  Initialize
    //! @see  InitializeBufferSet
    //! @see  SoundSystemParam
    //---------------------------------------------------------------------------
    static size_t GetRequiredMemSizeForCircularBufferSink( const SoundSystemParam& param ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        サウンドシステムの終了処理を行います。
    //!
    //!               @ref SoundSystem::Initialize で作成された、
    //!               サウンド処理を行うスレッドと
    //!               ストレージからサウンドデータをロードするためのスレッドを終了させます。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    static void Finalize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        サウンドフレーム処理から呼び出されるコールバック関数を設定します。
    //!
    //! @param[in] callback   サウンドフレーム処理から呼び出されるコールバック関数です。
    //! @param[in] arg        コールバック関数に渡されるユーザー引数です。
    //!
    //! @see Initialize
    //! @see SoundFrameUserCallback
    //! @see ClearSoundFrameUserCallback
    //---------------------------------------------------------------------------
    static void SetSoundFrameUserCallback( SoundFrameUserCallback callback, uintptr_t arg ) NN_NOEXCEPT
    {
        detail::driver::SoundThread::GetInstance().RegisterSoundFrameUserCallback(
                callback, arg );
    }

    //---------------------------------------------------------------------------
    //! @brief        サウンドフレーム処理から呼び出されるコールバック関数を削除します。
    //!
    //! @see SetSoundFrameUserCallback
    //---------------------------------------------------------------------------
    static void ClearSoundFrameUserCallback() NN_NOEXCEPT
    {
        detail::driver::SoundThread::GetInstance().ClearSoundFrameUserCallback();
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief        サウンドスレッド処理の開始時に呼び出されるコールバック関数を設定します。
    //!               オーディオレンダラ更新後、イベント待ちからの復帰の後に呼び出されます。
    //!
    //!               @ref nn::atk::SoundSystem::RegisterSoundThreadUpdateProfileReader で
    //!               プロファイルリーダーを登録している場合は、イベント待ちの直後ではなく
    //!               プロファイル情報の記録処理が行われた後にコールバックが呼び出されます。
    //!
    //! @param[in] callback   サウンドスレッド処理の開始時に呼び出されるコールバック関数です。
    //! @param[in] arg        コールバック関数に渡されるユーザー引数です。
    //!
    //! @see Initialize
    //! @see SoundThreadUserCallback
    //! @see ClearSoundThreadBeginUserCallback
    //---------------------------------------------------------------------------
    static void SetSoundThreadBeginUserCallback(SoundThreadUserCallback callback, uintptr_t arg) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief        サウンドスレッド処理の開始時に呼び出されるコールバック関数を削除します。
    //!
    //! @see SetSoundThreadBeginUserCallback
    //---------------------------------------------------------------------------
    static void ClearSoundThreadBeginUserCallback() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief        サウンドスレッド処理の終了時に呼び出されるコールバック関数を設定します。
    //!               オーディオレンダラ更新のリクエスト呼び出し直後に呼び出されます。
    //!
    //! @param[in] callback   サウンドスレッド処理の終了時に呼び出されるコールバック関数です。
    //! @param[in] arg        コールバック関数に渡されるユーザー引数です。
    //!
    //! @see Initialize
    //! @see SoundThreadUserCallback
    //! @see ClearSoundThreadEndUserCallback
    //---------------------------------------------------------------------------
    static void SetSoundThreadEndUserCallback(SoundThreadUserCallback callback, uintptr_t arg) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief        サウンドスレッド処理の終了時に呼び出されるコールバック関数を削除します。
    //!
    //! @see SetSoundThreadEndUserCallback
    //---------------------------------------------------------------------------
    static void ClearSoundThreadEndUserCallback() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        サウンドシステムが初期化されているかどうかを取得します。
    //!
    //!               @ref Initialize が呼び出され、
    //!               サウンドシステムの初期化が完了しているかどうかを調べます。
    //!
    //! @return       サウンドシステムの初期化が完了していれば true を、
    //!               そうでなければ false を返します。
    //!
    //! @see  Initialize
    //! @see  Finalize
    //!
    //---------------------------------------------------------------------------
    static bool IsInitialized() NN_NOEXCEPT;


    //---------------------------------------------------------------------------
    //! @brief      サウンドスレッドをロックします。
    //!
    //!             長時間ロックし続けると、 nn::atk のサウンド処理が滞り、
    //!             音が途切れるなどの悪影響があります。
    //!
    //! @see  UnlockSoundThread
    //!
    //---------------------------------------------------------------------------
    static void LockSoundThread() NN_NOEXCEPT
    {
        detail::driver::SoundThread::GetInstance().Lock();
    }

    //---------------------------------------------------------------------------
    //! @brief      サウンドスレッドをアンロックします。
    //!
    //!             @ref LockSoundThread 関数の説明もあわせてご覧ください。
    //!
    //! @see  LockSoundThread
    //!
    //---------------------------------------------------------------------------
    static void UnlockSoundThread() NN_NOEXCEPT
    {
        detail::driver::SoundThread::GetInstance().Unlock();
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief      サウンドシステムが利用するオーディオレンダラを停止します。その結果、サウンドスレッドの処理も停止します。
    //!             複数回こちらを呼んだ場合、同じ回数 @ref ResumeAudioRenderer を呼ぶことでオーディオレンダラを再開するようになります。
    //!             停止した状態で nn::atk に関する終了処理や、音源の再生などを行わないように注意してください。
    //!
    //! @param[in] fadeTimes フェードアウトにかかる時間です。現状、機能は実装されていません。
    //! @see  ResumeAudioRenderer
    //!
    //---------------------------------------------------------------------------
    static void SuspendAudioRenderer(nn::TimeSpan fadeTimes) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief      サウンドシステムが利用するオーディオレンダラを開始します。
    //!
    //! @param[in] fadeTimes フェードインにかかる時間です。現状、機能は実装されていません。
    //! @see  SuspendAudioRenderer
    //!
    //---------------------------------------------------------------------------
    static void ResumeAudioRenderer(nn::TimeSpan fadeTimes) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief      サウンドシステムが利用するオーディオレンダラのレンダリング処理を実行します。
    //!
    //---------------------------------------------------------------------------
    static void ExecuteRendering() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      オブジェクトの生成時からオブジェクトの存在するスコープを抜けるまでの間、
    //!             サウンドスレッドをロックします。
    //!
    //!             使用に関する注意点は、@ref LockSoundThread 関数と同じです。
    //!
    //! @see LockSoundThread
    //! @see UnlockSoundThread
    //!
    //---------------------------------------------------------------------------
    class SoundThreadScopedLock
    {
    public:
        //---------------------------------------------------------------------------
        //! @brief  コンストラクタです。
        //!
        //!         内部で @ref LockSoundThread 関数を呼び出します。
        //!
        //! @see  LockSoundThread
        //!
        //---------------------------------------------------------------------------
        SoundThreadScopedLock() NN_NOEXCEPT
        {
            LockSoundThread();
        }
        //---------------------------------------------------------------------------
        //! @brief  デストラクタです。
        //!
        //!         内部で @ref UnlockSoundThread 関数を呼び出します。
        //!
        //! @see  UnlockSoundThread
        //!
        //---------------------------------------------------------------------------
        ~SoundThreadScopedLock() NN_NOEXCEPT
        {
            UnlockSoundThread();
        }
    private:
        NN_DISALLOW_COPY( SoundThreadScopedLock );
    };


    //---------------------------------------------------------------------------
    //! @brief   サウンドシステムで使用しているオーディオレンダラのコンフィグ設定を取得します。
    //!
    //! @return  サウンドシステムで使用しているオーディオレンダラのコンフィグ設定です。
    //---------------------------------------------------------------------------
    static nn::audio::AudioRendererConfig& GetAudioRendererConfig() NN_NOEXCEPT
    {
        return detail::driver::HardwareManager::GetInstance().GetAudioRendererConfig();
    }


    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief   サウンドシステムで使用しているオーディオレンダラのパラメータを取得します。
    //!
    //! @return  サウンドシステムで使用しているオーディオレンダラのパラメータです。
    //!
    //! @pre
    //! - SoundSystem が初期化されている
    //---------------------------------------------------------------------------
    static const nn::audio::AudioRendererParameter& GetAudioRendererParameter() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( IsInitialized(), "SoundSystem is not initialized.\n" );
        return detail::driver::HardwareManager::GetInstance().GetAudioRendererParameter();
    }


    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief   サウンドシステムで使用している FinalMix を取得します。
    //!
    //! @return  サウンドシステムで使用している FinalMix です。
    //!
    //! @pre
    //! - SoundSystem が初期化されている
    //---------------------------------------------------------------------------
    static FinalMix& GetFinalMix() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( IsInitialized(), "SoundSystem is not initialized.\n" );
        return detail::driver::HardwareManager::GetInstance().GetFinalMix();
    }

    //---------------------------------------------------------------------------
    //! @brief        指定したバッファをメモリプールにアタッチします。
    //!
    //!               この関数を実行すると、メモリプールへのアタッチが完了するまで処理が返ってこない点にご注意ください。
    //!               この関数はミリ秒単位で処理を止める可能性があるため、アプリケーションのメインループ内で呼ばないでください。
    //!
    //! @param[in,out] pMemoryPool アタッチするメモリプールです。
    //! @param[in] address メモリプールにアタッチするメモリのアドレスです。
    //! @param[in] size メモリプールにアタッチするメモリのサイズです。
    //!
    //! @see nn::atk::SoundSystem::DetachMemoryPool
    //!
    //! @pre
    //! - SoundSystem が初期化されている
    //! - pMemoryPool != nullptr
    //! - pMemoryPool がアタッチしていない
    //! - address, size が @ref nn::audio::AcquireMemoryPool() の事前条件を満たしている
    //---------------------------------------------------------------------------
    static void AttachMemoryPool(nn::audio::MemoryPoolType* pMemoryPool, void* address, size_t size) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        指定したバッファをメモリプールからデタッチします。
    //!
    //!               この関数を実行すると、メモリプールからのデタッチが完了するまで処理が返ってこない点にご注意ください。
    //!               この関数はミリ秒単位で処理を止める可能性があるため、アプリケーションのメインループ内で呼ばないでください。
    //!
    //! @param[in,out]  pMemoryPool デタッチするメモリプールです。
    //!
    //! @see nn::atk::SoundSystem::AttachMemoryPool
    //!
    //! @pre
    //! - SoundSystem が初期化されている
    //! - pMemoryPool != nullptr
    //! - pMemoryPool がアタッチしている
    //---------------------------------------------------------------------------
    static void DetachMemoryPool(nn::audio::MemoryPoolType* pMemoryPool) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  サウンドライブラリのスリープ準備処理を実行します。
    //!
    //!         スリープに入る前に、本関数を必ず呼び出してください。
    //!
    //!         スリープの準備では、
    //!         タスクスレッドをロックします。
    //!
    //!         タスクスレッドでロード中のデータがある場合は、
    //!         ロードが終わるまで本関数内でブロックされます。
    //!
    //!         スリープを解除する場合は、@ref LeaveSleep 関数を呼び出してください。
    //!
    //! @see  LeaveSleep
    //!
    //---------------------------------------------------------------------------
    static void EnterSleep() NN_NOEXCEPT
    {
        if ( g_IsEnterSleep )
        {
            return;
        }

        g_IsEnterSleep = true;
        g_IsStreamLoadWait = true;
        detail::TaskThread::GetInstance().Lock();

        // サウンドスレッドは SDK 側で止められる
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  サウンドライブラリのスリープ終了処理を実行します。
    //!
    //!         スリープを解除した後に、必ず呼び出してください。
    //!
    //! @see  EnterSleep
    //!
    //---------------------------------------------------------------------------
    static void LeaveSleep() NN_NOEXCEPT
    {
        if ( ! g_IsEnterSleep )
        {
            return;
        }

        // サウンドスレッドは SDK 側で復帰する
        detail::TaskThread::GetInstance().Unlock();
        g_IsStreamLoadWait = false;
        g_IsEnterSleep = false;
    }

    //! @brief サウンドシステムが利用するオーディオレンダラのレンダリング時間の上限を取得します。
    //!        オーディオレンダラのレンダリング時間についての詳細は @ref nn::audio::SetAudioRendererRenderingTimeLimit を参照ください。
    //! @return サウンドシステムが利用するオーディオレンダラに割り当てられたレンダリング時間の上限を返します。単位はパーセントです。
    //! @pre
    //! - SoundSystem が初期化されている
    static int GetAudioRendererRenderingTimeLimit() NN_NOEXCEPT;

    //! @brief サウンドシステムが利用するオーディオレンダラのレンダリング時間の上限を設定します。
    //!        オーディオレンダラのレンダリング時間についての詳細は @ref nn::audio::SetAudioRendererRenderingTimeLimit を参照ください。
    //! @param[in] limitPercent サウンドシステムが利用するオーディオレンダラに割り当てるレンダリング時間の上限を指定します。単位はパーセントです。
    //! @pre
    //! - SoundSystem が初期化されている
    //! - limitPercent > 0 && limitPercent <= 100
    static void SetAudioRendererRenderingTimeLimit(int limitPercent) NN_NOEXCEPT;

    //!@}
    //! @internal
    static void DumpMemory() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static size_t GetAudioRendererBufferSize() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static size_t GetRecorderBufferSize() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static size_t GetUserCircularBufferSinkBufferSize() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static size_t GetLowLevelVoiceAllocatorBufferSize() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static size_t GetMultiVoiceManagerBufferSize() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static size_t GetChannelManagerBufferSize() NN_NOEXCEPT;

    //! @internal
    //! @deprecated SoundSystemParam::rendererSampleRate で 48kHz を指定することで制約もなく 48kHz レンダリングできるためそちらを使用してください。
    //!             互換性のために残されていますが、これらの API はすでに使用できない状態になっていて、今後削除される予定です。
    static size_t GetSynthesizeBufferSize() NN_NOEXCEPT
    {
        return 0u;
    }

    //! @internal
    //! @return :private
    static size_t GetSoundThreadCommandTotalBufferSize() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static size_t GetTaskThreadCommandTotalBufferSize() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static size_t GetLowLevelVoiceCommandTotalBufferSize() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static size_t GetVoiceReplyCommandTotalBufferSize() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static size_t GetDriverCommandBufferSize() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static size_t GetAllocatableDriverCommandSize() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static size_t GetAllocatedDriverCommandBufferSize() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static int GetAllocatedDriverCommandCount() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static bool detail_IsStreamLoadWait() NN_NOEXCEPT
    {
        return g_IsStreamLoadWait;
    }

    //----------------------------------------
    //! @name システム
    //@{
    //! @briefprivate
    //! @deprecated SoundSystemParam::rendererSampleRate で 48kHz を指定することで制約もなく 48kHz レンダリングできるためそちらを使用してください。
    //!             互換性のために残されていますが、これらの API はすでに使用できない状態になっていて、今後削除される予定です。
    //! @return :private
    static bool detail_IsInitializedNwRenderer() NN_NOEXCEPT
    {
        return false;
    }

    //! @briefprivate
    //! @brief ストリームサウンドのファイルが見つからないなどの理由でファイルロードの失敗が起こったとき、アボートするかどうかを設定します。
    //!        NintendoSDK 4.0.0 以降、この設定はすべてのビルドタイプにおいて適用されます。
    //! @param isEnabled :private
    static void detail_SetStreamOpenFailureHaltEnabled(bool isEnabled) NN_NOEXCEPT
    {
        g_IsStreamOpenFailureHalt = isEnabled;
    }
    //@}

    //! @internal
    //! @return :private
    static bool detail_IsStreamOpenFailureHaltEnabled() NN_NOEXCEPT
    {
        return g_IsStreamOpenFailureHalt;
    }

    //! @internal
    //! @return :private
    static bool detail_IsTaskThreadEnabled() NN_NOEXCEPT
    {
        return g_IsTaskThreadEnabled;
    }

    //! @internal
    //! @return :private
    static bool detail_IsSoundThreadEnabled() NN_NOEXCEPT
    {
        return g_IsSoundThreadEnabled;
    }

    //! @internal
    //! @return :private
    static detail::SoundInstanceConfig GetSoundInstanceConfig() NN_NOEXCEPT;

    //! @internal
    //! @return :private
    static detail::SoundInstanceConfig GetSoundInstanceConfig(const SoundSystemParam& param) NN_NOEXCEPT;

#if defined(NN_ATK_CONFIG_ENABLE_THREAD_CORE_NUMBER_OBSERVATION)
    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  サウンドスレッドの動作コア番号を取得します。
    //!
    //! @pre
    //! - SoundSystem が初期化されている
    //---------------------------------------------------------------------------
    static int GetSoundThreadCoreNumber() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( IsInitialized(), "SoundSystem is not initialized.\n" );
        return nn::atk::detail::driver::SoundThread::GetInstance().GetThreadCoreNumber();
    }
    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  タスクスレッドの動作コア番号を取得します。
    //!
    //! @pre
    //! - SoundSystem が初期化されている
    //---------------------------------------------------------------------------
    static int GetTaskThreadCoreNumber() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( IsInitialized(), "SoundSystem is not initialized.\n" );
        return nn::atk::detail::TaskThread::GetInstance().GetThreadCoreNumber();
    }
#endif

    //----------------------------------------
    //! @name 出力モード
    //@{
    //---------------------------------------------------------------------------
    //! @brief  サウンドの出力モードを設定します。
    //!
    //!         出力モードの初期値は @ref OutputMode_Stereo です。
    //!         現在の出力モードを知りたい場合は、@ref GetOutputMode を使用してください。
    //!
    //!         ■ ミックスパラメータの計算方法
    //!
    //!         本関数で設定する出力モードは、ミックスパラメータ
    //!         (フロントの L/R、リアの L/R の音量。
    //!         以下、それぞれ fL, fR, rL, rR と表現します)
    //!         の計算方法に影響を与えます。
    //!
    //!         たとえば OutputMode_Monaural を設定すると、SoundHandle などで設定する
    //!         パン・サラウンドパンの値によらず、ミックスパラメータのフロント L/R
    //!         が同じ値になり、リアの L/R にはゼロが設定されます。
    //!
    //!         - fL = fR = M (ボリュームに応じた値)
    //!         - rL = rR = 0.0f
    //!
    //!         OutputMode_Stereo を設定すると、パンの値を反映して、
    //!         フロントの L/R が別の値 (パンを中央に振っている場合は同じ値)
    //!         になりますが、サラウンドパンは無視され、リアの L/R はゼロが設定されます。
    //!
    //!         - fL = L (ボリュームとパンに応じた値)
    //!         - fR = R (ボリュームとパンに応じた値)
    //!         - rL = rR = 0.0f
    //!
    //!         OutputMode_Surround を設定すると、パンとサラウンドパンの値が反映され、
    //!         フロントの L/R、リアの L/R がそれぞれ別の値が設定されます
    //!         (それぞれパンを中央に振っている場合は同じ値になります)。
    //!
    //!         - fL = surFL (ボリュームとパン・サラウンドパンに応じた値)
    //!         - fR = surFR (ボリュームとパン・サラウンドパンに応じた値)
    //!         - rL = surRL (ボリュームとパン・サラウンドパンに応じた値)
    //!         - rR = surRR (ボリュームとパン・サラウンドパンに応じた値)
    //!
    //!         ■ ミックスパラメータのダウンミックス
    //!
    //!         上記のように計算されたフロントの L/R およびリアの L/R に加え、
    //!         最終的なスピーカの出力数に応じてダウンミックス処理が行われることになります。
    //!         ダウンミックスの詳細な処理内容は、 @confluencelink{89991492, オーディオレンダラ} の Sink の項をご参照ください。
    //!
#if 0 // 参考用 : NW4F 実装
    //!         ■ ミックスパラメータのダウンミックス
    //!
    //!         上記のように計算されたフロントの L/R およびリアの L/R は、
    //!         さらに、本体メニューの設定に応じてダウンミックスされます。
    //!
    //!         本体メニューがモノラルの場合、ここで設定するモードが
    //!         OutputMode_Stereo および OutputMode_Surround
    //!         のときに、次のようにダウンミックスされます。
    //!
    //!         (OutputMode_Stereo)
    //!         - fL = fR = L+R/2
    //!
    //!         (OutputMode_Surround)
    //!         - fL = fR = ( (surFL + surRL) + (surFR + surRR) ) / 2
    //!
    //!         本体メニューがステレオの場合、ここで設定するモードが、
    //!         OutputMode_Surround のときに次のようにダウンミックスされます。
    //!
    //!         - fL = surFL + sufRL
    //!         - rL = surFR + sufRR
    //!
    //!         そのほかのケースではダウンミックスは行われません。
    //!
    //!
    //!         ■ ダウンミックスによる音量感の変化
    //!
    //!         パンやサラウンドパンを振ると、
    //!         ダウンミックス後の音量感が変わる可能性があります。
    //!
    //!         たとえば、SetPan(1.0f) および SetSurroundPan(2.0f) とすると、
    //!         OutputMode_Monaural では、
    //!
    //!         - M = 0.71f
    //!
    //!         OutputMode_Stereo では、
    //!
    //!         - L = 0.00f
    //!         - R = 1.00f
    //!
    //!         OutputMode_Surround では、
    //!
    //!         - surFL = 0.00f
    //!         - surFR = 0.00f
    //!         - surRL = 0.00f
    //!         - surRR = 0.80f
    //!
    //!         のような値が計算されます。本体メニューのモードがモノラルの場合、
    //!         上で示した計算式によってダウンミックスされるため、
    //!
    //!         - 本体メニューがモノラル : fL( 0.71) fR( 0.71) rL( 0.00) rR( 0.00)
    //!         - 本体メニューがステレオ : fL( 0.50) fR( 0.50) rL( 0.00) rR( 0.00)
    //!         - 本体メニューがサラウンド: fL( 0.40) fR( 0.40) rL( 0.00) rR( 0.00)
    //!
    //!         となり、かなり音量が変わってしまうことになります。
    //!
    //!         このような音量の変化を懸念される場合は、
    //!         本関数で出力モードを変化できるようにするシーンを絞ったりするなど、
    //!         ユーザー側で工夫する必要があります。
#endif
    //!
    //! @param[in]  mode    サウンドの出力モードです。
    //! @param[in]  device  出力先デバイスです。
    //!
    //! @see  OutputMode
    //! @see  OutputDevice
    //! @see  GetOutputMode
    //---------------------------------------------------------------------------
    static void SetOutputMode( OutputMode mode, OutputDevice device = OutputDevice_Main ) NN_NOEXCEPT
    {
        detail::driver::HardwareManager::GetInstance().SetOutputMode( mode, device );
    }

    //---------------------------------------------------------------------------
    //! @brief        現在のサウンド出力モードを取得します。
    //!
    //! @return       現在のサウンド出力モードを返します。
    //!
    //! @param[in]  device  出力先デバイスです。
    //!
    //! @see  OutputMode
    //! @see  OutputDevice
    //! @see  SetOutputMode
    //---------------------------------------------------------------------------
    static OutputMode GetOutputMode( OutputDevice device = OutputDevice_Main ) NN_NOEXCEPT
    {
        return detail::driver::HardwareManager::GetInstance().GetOutputMode( device );
    }
    //@}

    //! @internal
    static void detail_UpdateEndUserOutputMode() NN_NOEXCEPT
    {
        detail::driver::HardwareManager::GetInstance().UpdateEndUserOutputMode();
    }

    //----------------------------------------
    //! @name 出力先
    //@{
    //---------------------------------------------------------------------------
    //! @brief        指定した出力ラインについて、出力先デバイスのビットフラグを設定します。
    //!               ただし OUTPUT_LINE_INDEX_RESERVED_MAX より前の出力ラインは
    //!               予約済みの出力ラインとなっておりまして、出力先デバイスの変更はできません。
    //!
    //! @param[in]  index             出力ラインのインデックスです。
    //! @param[in]  outputDeviceFlag  出力先デバイスのビットフラグです。
    //!
    //! @see  OutputDeviceIndex
    //! @see  GetOutputDeviceFlag
    //---------------------------------------------------------------------------
    static void SetOutputDeviceFlag( OutputLineIndex index, uint8_t outputDeviceFlag ) NN_NOEXCEPT
    {
        detail::driver::HardwareManager::GetInstance().SetOutputDeviceFlag( index, outputDeviceFlag );
    }

    //---------------------------------------------------------------------------
    //! @brief        指定した出力ラインについて、出力先デバイスのビットフラグを取得します。
    //!
    //! @return       指定した出力ラインについて出力先デバイスのビットフラグを返します。
    //!
    //! @param[in]  index  出力ラインのインデックスです。
    //!
    //! @see  OutputDeviceIndex
    //! @see  SetOutputDeviceFlag
    //---------------------------------------------------------------------------
    static uint8_t GetOutputDeviceFlag( OutputLineIndex index ) NN_NOEXCEPT
    {
        return detail::driver::HardwareManager::GetInstance().GetOutputDeviceFlag( index );
    }
    //@}


    //----------------------------------------
    //! @name マスターボリューム
    //@{
    //---------------------------------------------------------------------------
    //! @brief        マスターボリュームを設定します。
    //!
    //!               マスターボリュームは、
    //!               サウンドライブラリで発音される音全てに対して影響を与えます。
    //!               サウンドライブラリ以外で発音された音には影響を与えません。
    //!
    //!               この関数で指定する値は、他のどの音量パラメータとも独立して動作し、
    //!               それらは全て重ね合わされます。
    //!
    //!               音量 volume は、0.0 以上の倍率で指定します。
    //!               すなわち、1.0 を指定すると音量に影響を与えません。
    //!               0.0 を指定すると発音されなくなります。
    //!               デフォルト値は 1.0 です。
    //!
    //!               他の音量パラメータと重ね合わされたあと、
    //!               最終的な音量は nn::audio::VoiceType::GetVolumeMin() ～ nn::audio::VoiceType::GetVolumeMax() の範囲でクランプされます。
    //!               音量の重ね合わせの詳細につきましては @confluencelink{83955829, サウンドパラメータ制御の動作} を参照ください。
    //!
    //!               音量の変化は fadeTimes で指定した時間をかけて行われます。
    //!               音量の変化途中にさらにこの関数を呼び出した場合は、
    //!               その時点での変化途中の音量値を基点として、
    //!               新しく指定した時間をかけて音量を変化させます。
    //!
    //! @param[in] volume     音量の倍率 ( 0.0 ～ ) です。
    //! @param[in] fadeTimes  音量変化にかける時間 (ミリ秒) です。
    //!
    //! @see  GetMasterVolume
    //!
    //---------------------------------------------------------------------------
    static void SetMasterVolume( float volume, int fadeTimes ) NN_NOEXCEPT
    {
        detail::driver::HardwareManager::GetInstance().SetMasterVolume( volume, fadeTimes );
    }
    // TODO: ★ 「AUX に送られる前に反映されるので～」を書いていない

    //---------------------------------------------------------------------------
    //! @brief        現在のマスターボリュームを取得します。
    //!
    //!               現在設定されている @ref SetMasterVolume で設定された
    //!               マスターボリュームの値を取得します。
    //!
    //!               音量が時間変化している間に値を取得した場合は、
    //!               その時点での変化途中の音量の値を返します。
    //!
    //! @return       現在のマスターボリュームの値を返します。
    //!
    //! @see          SetMasterVolume
    //!
    //---------------------------------------------------------------------------
    static float GetMasterVolume() NN_NOEXCEPT
    {
        return detail::driver::HardwareManager::GetInstance().GetMasterVolume();
    }
    //@}

    //----------------------------------------
    //! @name エフェクト
    //@{

    //---------------------------------------------------------------------------
    //! @brief      指定したエフェクトを AUX バスに追加します。
    //!
    //!             SoundSystem::AppendEffect で追加されたエフェクトは、
    //!             SoundSystem::ClearEffect が呼ばれるまで破棄しないでください。
    //!
    //!             エフェクトは、指定したバスそれぞれで独立して処理されます。
    //!             エフェクト (@ref nn::atk::EffectBase を継承したエフェクト) は、
    //!             1 つのバスに複数のエフェクトを登録することができます。
    //!             エフェクトは、追加した順番で処理されます。
    //!
    //!             途中でエフェクトの接続順を変更したい場合は、
    //!             @ref nn::atk::SoundSystem::ClearEffect
    //!             を呼び出して一度すべてのエフェクトの登録を削除した後、
    //!             改めて追加しなおしてください。
    //!
    //!             エフェクト処理の概要については @ref SoundSystem クラスの説明をご覧ください。
    //!
    //! @param[in]  bus                 エフェクトを追加するバスです。
    //! @param[in]  pEffect             エフェクトです。
    //! @param[in]  effectBuffer        エフェクトで利用するバッファです。
    //! @param[in]  effectBufferSize    effectBuffer で指定したバッファのサイズです。
    //!
    //! @return     エフェクトの開始に成功したら true を、失敗したら false を返します。
    //!
    //! @pre
    //! - effectBuffer が nn::audio::BufferAlignSize にアラインしている
    //!
    //! @see Initialize
    //! @see GetRequiredMemSize
    //! @see AuxBus
    //! @see EffectBase
    //! @see ClearEffect
    //! @see OutputDevice
    //---------------------------------------------------------------------------
    static bool AppendEffect( AuxBus bus, EffectBase* pEffect, void* effectBuffer, size_t effectBufferSize ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      指定したエフェクトを AUX バスに追加します。
    //!
    //!             SoundSystem::AppendEffect で追加されたエフェクトは、
    //!             SoundSystem::ClearEffect が呼ばれるまで破棄しないでください。
    //!
    //!             エフェクトは、指定したバスそれぞれで独立して処理されます。
    //!             エフェクト (@ref nn::atk::EffectBase を継承したエフェクト) は、
    //!             1 つのバスに複数のエフェクトを登録することができます。
    //!             エフェクトは、追加した順番で処理されます。
    //!
    //!             途中でエフェクトの接続順を変更したい場合は、
    //!             @ref nn::atk::SoundSystem::ClearEffect
    //!             を呼び出して一度すべてのエフェクトの登録を削除した後、
    //!             改めて追加しなおしてください。
    //!
    //!             エフェクト処理の概要については @ref SoundSystem クラスの説明をご覧ください。
    //!
    //! @param[in]  bus                 エフェクトを追加するバスです。
    //! @param[in]  pEffect             エフェクトです。
    //! @param[in]  effectBuffer        エフェクトで利用するバッファです。
    //! @param[in]  effectBufferSize    effectBuffer で指定したバッファのサイズです。
    //! @param[in]  device              エフェクトを設定するデバイスです。
    //!
    //! @return     エフェクトの開始に成功したら true を、失敗したら false を返します。
    //!
    //! @pre
    //! - effectBuffer が nn::audio::BufferAlignSize にアラインしている
    //!
    //! @see Initialize
    //! @see GetRequiredMemSize
    //! @see AuxBus
    //! @see EffectBase
    //! @see ClearEffect
    //! @see OutputDevice
    //---------------------------------------------------------------------------
    static bool AppendEffect( AuxBus bus, EffectBase* pEffect, void* effectBuffer, size_t effectBufferSize, OutputDevice device ) NN_NOEXCEPT;

    //! @briefprivate
    static bool AppendEffect( AuxBus bus, EffectBase* pEffect, void* effectBuffer, size_t effectBufferSize, OutputDevice device, int subMixIndex ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      ユーザーエフェクトを AUX バスに追加します。
    //!
    //!             SoundSystem::AppendEffect で追加されたエフェクトは、
    //!             SoundSystem::ClearEffect が呼ばれるまで破棄しないでください。
    //!
    //!             エフェクトは、指定したバスそれぞれで独立して処理されます。
    //!             @ref nn::atk::EffectAux を継承したエフェクトは、 @ref nn::atk::EffectBase を継承したエフェクト
    //!             と同様に 1 つのバスに複数のエフェクトを登録することができます。
    //!             エフェクトは、追加した順番で処理されます。
    //!
    //!             途中でエフェクトの接続順を変更したい場合は、
    //!             @ref nn::atk::SoundSystem::ClearEffect
    //!             を呼び出して一度すべてのエフェクトの登録を削除した後、
    //!             改めて追加しなおしてください。
    //!
    //!             @ref EffectAux::Initialize() は、この関数実行後に、
    //!             サウンドスレッドでエフェクトの追加処理が実行される際に呼び出されます。
    //!
    //!             ユーザーエフェクトの概要については @ref EffectAux クラスの説明をご覧ください。
    //!
    //! @param[in]  bus                 エフェクトを追加するバスです。
    //! @param[in]  pEffect             エフェクトです。
    //! @param[in]  effectBuffer        エフェクトで利用するバッファです。
    //! @param[in]  effectBufferSize    effectBuffer で指定したバッファのサイズです。
    //!
    //! @return     エフェクトの追加に成功したら true を、失敗したら false を返します。
    //!
    //! @pre
    //! - effectBuffer が nn::audio::BufferAlignSize にアラインしている
    //!
    //! @see GetRequiredEffectAuxBufferSize
    //! @see AuxBus
    //! @see ClearEffect
    //---------------------------------------------------------------------------
    static bool AppendEffect( AuxBus bus, EffectAux* pEffect, void* effectBuffer, size_t effectBufferSize ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      ユーザーエフェクトを AUX バスに追加します。
    //!
    //!             SoundSystem::AppendEffect で追加されたエフェクトは、
    //!             SoundSystem::ClearEffect が呼ばれるまで破棄しないでください。
    //!
    //!             エフェクトは、指定したバスそれぞれで独立して処理されます。
    //!             @ref nn::atk::EffectAux を継承したエフェクトは、 @ref nn::atk::EffectBase を継承したエフェクト
    //!             と同様に 1 つのバスに複数のエフェクトを登録することができます。
    //!             エフェクトは、追加した順番で処理されます。
    //!
    //!             途中でエフェクトの接続順を変更したい場合は、
    //!             @ref nn::atk::SoundSystem::ClearEffect
    //!             を呼び出して一度すべてのエフェクトの登録を削除した後、
    //!             改めて追加しなおしてください。
    //!
    //!             @ref EffectAux::Initialize() は、この関数実行後に、
    //!             サウンドスレッドでエフェクトの追加処理が実行される際に呼び出されます。
    //!
    //!             ユーザーエフェクトの概要については @ref EffectAux クラスの説明をご覧ください。
    //!
    //! @param[in]  bus                 エフェクトを追加するバスです。
    //! @param[in]  pEffect             エフェクトです。
    //! @param[in]  effectBuffer        エフェクトで利用するバッファです。
    //! @param[in]  effectBufferSize    effectBuffer で指定したバッファのサイズです。
    //! @param[in]  device              エフェクトを設定するデバイスです。
    //!
    //! @return     エフェクトの追加に成功したら true を、失敗したら false を返します。
    //!
    //! @pre
    //! - effectBuffer が nn::audio::BufferAlignSize にアラインしている
    //!
    //! @see GetRequiredEffectAuxBufferSize
    //! @see AuxBus
    //! @see ClearEffect
    //! @see OutputDevice
    //---------------------------------------------------------------------------
    static bool AppendEffect( AuxBus bus, EffectAux* pEffect, void* effectBuffer, size_t effectBufferSize, OutputDevice device ) NN_NOEXCEPT;

    //@}
    //! @internal
    static bool AppendEffect( AuxBus bus, EffectAux* pEffect, void* effectBuffer, size_t effectBufferSize, OutputDevice device, int subMixIndex ) NN_NOEXCEPT;

    //! @internal
    static bool AppendEffectToFinalMix( EffectAux* pEffect, void* effectBuffer, size_t effectBufferSize ) NN_NOEXCEPT;

    //! @internal
    static bool AppendEffectToAdditionalSubMix( EffectAux* pEffect, void* effectBuffer, size_t effectBufferSize ) NN_NOEXCEPT;

    //----------------------------------------
    //! @name エフェクト
    //@{
    //---------------------------------------------------------------------------
    //! @brief        指定したユーザーエフェクトを使用するために必要なバッファサイズを取得します。
    //!
    //!               この関数で取得したサイズのバッファを @ref nn::atk::SoundSystem::AppendEffect() に渡してください。
    //!
    //!               この関数の返り値は、@ref nn::atk::EffectAux::SetChannelCount() と
    //!               @ref nn::atk::EffectAux::SetAudioFrameCount() の値、および SoundSystem の初期化時
    //!               に渡しているパラメータに依存して値が変動します。
    //!               そのため、この関数でサイズを取得した後に、これらの値を変更しないでください。
    //!
    //! @param[in]    pEffect 対象となるユーザーエフェクトです。
    //!
    //! @see          AppendEffect
    //! @see          nn::atk::EffectAux::SetChannelCount
    //! @see          nn::atk::EffectAux::SetAudioFrameCount
    //---------------------------------------------------------------------------
    static size_t GetRequiredEffectAuxBufferSize( const EffectAux* pEffect ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief       指定した AUX バスに登録されているエフェクトを削除します。
    //!              エフェクトを削除する際には、必ず事前にエフェクトの SetEnabled 関数を呼んで無効化した後
    //!              IsRemovable 関数を呼んで削除が可能である事を確認してください。
    //!
    //!              @ref SoundSystem::RemoveEffect() と @ref SoundSystem::ClearEffect() は
    //!              サウンドスレッドでエフェクトの削除が行われるまで待機するため、最大 1 オーディオフレーム分の時間がかかります。
    //!              複数のエフェクトをバスから削除する場合は、処理時間を軽減するために @ref SoundSystem::ClearEffect() をご検討ください。
    //!
    //! @param[in]  bus         エフェクトバスです。
    //! @param[in]  pEffect     bus から削除するエフェクトです。
    //!
    //! @see AuxBus
    //! @see EffectAux
    //! @see OutputDevice
    //---------------------------------------------------------------------------
    static void RemoveEffect( AuxBus bus, EffectBase* pEffect ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief       指定した AUX バスに登録されているエフェクトを削除します。
    //!              エフェクトを削除する際には、必ず事前にエフェクトの SetEnabled 関数を呼んで無効化した後
    //!              IsRemovable 関数を呼んで削除が可能である事を確認してください。
    //!
    //!              @ref SoundSystem::RemoveEffect() と @ref SoundSystem::ClearEffect() は
    //!              サウンドスレッドでエフェクトの削除が行われるまで待機するため、最大 1 オーディオフレーム分の時間がかかります。
    //!              複数のエフェクトをバスから削除する場合は、処理時間を軽減するために @ref SoundSystem::ClearEffect() をご検討ください。
    //!
    //! @param[in]  bus         エフェクトバスです。
    //! @param[in]  pEffect     bus から削除するエフェクトです。
    //! @param[in]  device      エフェクトを設定したデバイスです。
    //!
    //! @see AuxBus
    //! @see EffectAux
    //! @see OutputDevice
    //---------------------------------------------------------------------------
    static void RemoveEffect( AuxBus bus, EffectBase* pEffect, OutputDevice device ) NN_NOEXCEPT;

    //! @briefprivate
    static void RemoveEffect( AuxBus bus, EffectBase* pEffect, OutputDevice device, int subMixIndex ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief       指定した AUX バスに登録されているエフェクトを削除します。
    //!              エフェクトを削除する際には、必ず事前にエフェクトの SetEnabled 関数を呼んで無効化した後
    //!              IsRemovable 関数を呼んで削除が可能である事を確認してください。
    //!
    //!              @ref SoundSystem::RemoveEffect() と @ref SoundSystem::ClearEffect() は
    //!              サウンドスレッドでエフェクトの削除が行われるまで待機するため、最大 1 オーディオフレーム分の時間がかかります。
    //!              複数のエフェクトをバスから削除する場合は、処理時間を軽減するために @ref SoundSystem::ClearEffect() をご検討ください。
    //!
    //!              @ref EffectAux::Finalize() はこの関数の実行中に、
    //!              サウンドスレッドでエフェクトの削除処理が実行される際に呼び出されます。
    //!              追加されている @ref EffectAux を @ref SoundSystem::RemoveEffect() か @ref SoundSystem::ClearEffect() で削除せずに
    //!              終了処理を行った場合には、@ref SoundSystem::Finalize() 内部で呼び出されます。
    //!
    //! @param[in]  bus         エフェクトバスです。
    //! @param[in]  pEffect     bus から削除するエフェクトです。
    //!
    //! @see AuxBus
    //! @see EffectAux
    //! @see OutputDevice
    //---------------------------------------------------------------------------
    static void RemoveEffect( AuxBus bus, EffectAux* pEffect ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief       指定した AUX バスに登録されているエフェクトを削除します。
    //!              エフェクトを削除する際には、必ず事前にエフェクトの SetEnabled 関数を呼んで無効化した後
    //!              IsRemovable 関数を呼んで削除が可能である事を確認してください。
    //!
    //!              @ref SoundSystem::RemoveEffect() と @ref SoundSystem::ClearEffect() は
    //!              サウンドスレッドでエフェクトの削除が行われるまで待機するため、最大 1 オーディオフレーム分の時間がかかります。
    //!              複数のエフェクトをバスから削除する場合は、処理時間を軽減するために @ref SoundSystem::ClearEffect() をご検討ください。
    //!
    //!              @ref EffectAux::Finalize() はこの関数の実行中に、
    //!              サウンドスレッドでエフェクトの削除処理が実行される際に呼び出されます。
    //!              追加されている @ref EffectAux を @ref SoundSystem::RemoveEffect() か @ref SoundSystem::ClearEffect() で削除せずに
    //!              終了処理を行った場合には、@ref SoundSystem::Finalize() 内部で呼び出されます。
    //!
    //! @param[in]  bus         エフェクトバスです。
    //! @param[in]  pEffect     bus から削除するエフェクトです。
    //! @param[in]  device      エフェクトを設定したデバイスです。
    //!
    //! @see AuxBus
    //! @see EffectAux
    //! @see OutputDevice
    //---------------------------------------------------------------------------
    static void RemoveEffect( AuxBus bus, EffectAux* pEffect, OutputDevice device ) NN_NOEXCEPT;

    //@}
    //! @internal
    static void RemoveEffect( AuxBus bus, EffectAux* pEffect, OutputDevice device, int subMixIndex ) NN_NOEXCEPT;

    //! @internal
    static void RemoveEffectFromFinalMix( EffectAux* pEffect ) NN_NOEXCEPT;

    //! @internal
    static void RemoveEffectFromAdditionalSubMix( EffectAux* pEffect ) NN_NOEXCEPT;

    //----------------------------------------
    //! @name エフェクト
    //@{
    //---------------------------------------------------------------------------
    //! @brief       指定した AUX バスに登録されているエフェクトを全てバスから削除します。
    //!              エフェクトを削除する際には、必ず事前に各エフェクトの SetEnabled 関数を呼んで無効化した後
    //!              IsRemovable 関数を呼んで削除が可能である事を確認してください。
    //!
    //!              @ref SoundSystem::RemoveEffect() と @ref SoundSystem::ClearEffect() は
    //!              サウンドスレッドでエフェクトの削除が行われるまで待機するため、最大 1 オーディオフレーム分の時間がかかります。
    //!
    //!              @ref EffectAux::Finalize() はこの関数の実行中に、
    //!              サウンドスレッドでエフェクトの削除処理が実行される際に呼び出されます。
    //!              @ref EffectAux が追加されているエフェクトバスの @ref SoundSystem::ClearEffect() を実行せずに
    //!              終了処理を行った場合には、@ref SoundSystem::Finalize() 内部で呼び出されます。
    //!
    //! @param[in]  bus         エフェクトバスです。
    //!
    //! @see IsFinishedClearEffect
    //! @see AuxBus
    //! @see EffectBase
    //! @see OutputDevice
    //---------------------------------------------------------------------------
    static void ClearEffect( AuxBus bus ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief       指定した AUX バスに登録されているエフェクトを全てバスから削除します。
    //!              エフェクトを削除する際には、必ず事前に各エフェクトの SetEnabled 関数を呼んで無効化した後
    //!              IsRemovable 関数を呼んで削除が可能である事を確認してください。
    //!
    //!              @ref SoundSystem::RemoveEffect() と @ref SoundSystem::ClearEffect() は
    //!              サウンドスレッドでエフェクトの削除が行われるまで待機するため、最大 1 オーディオフレーム分の時間がかかります。
    //!
    //!              @ref EffectAux::Finalize() はこの関数の実行中に、
    //!              サウンドスレッドでエフェクトの削除処理が実行される際に呼び出されます。
    //!              @ref EffectAux が追加されているエフェクトバスの @ref SoundSystem::ClearEffect() を実行せずに
    //!              終了処理を行った場合には、@ref SoundSystem::Finalize() 内部で呼び出されます。
    //!
    //! @param[in]  bus         エフェクトバスです。
    //! @param[in]  device      エフェクトを設定したデバイスです。
    //!
    //! @see IsFinishedClearEffect
    //! @see AuxBus
    //! @see EffectBase
    //! @see OutputDevice
    //---------------------------------------------------------------------------
    static void ClearEffect( AuxBus bus, OutputDevice device ) NN_NOEXCEPT;

    //@}
    //! @internal
    static void ClearEffect( AuxBus bus, OutputDevice device, int subMixIndex ) NN_NOEXCEPT;

    //! @internal
    static void ClearEffectFromFinalMix() NN_NOEXCEPT;

    //! @internal
    static void ClearEffectFromAdditionalSubMix() NN_NOEXCEPT;

    //----------------------------------------
    //! @name エフェクト
    //@{
    //---------------------------------------------------------------------------
    //! @brief        ClearEffect が完了したかどうかを調べます。
    //!
    //!               @ref ClearEffect が呼ばれる前の動作は不定です。
    //!
    //! @param[in]  bus     エフェクトバスです。
    //!
    //! @return       @ref ClearEffect の処理が完了していれば true を、
    //!               そうでなければ false を返します。
    //!
    //! @see ClearEffect
    //! @see AuxBus
    //---------------------------------------------------------------------------
    static bool IsClearEffectFinished( AuxBus bus ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        ClearEffect が完了したかどうかを調べます。
    //!
    //!               @ref ClearEffect が呼ばれる前の動作は不定です。
    //!
    //! @param[in]  bus     エフェクトバスです。
    //! @param[in]  device  エフェクトを設定したデバイスです。
    //!
    //! @return       @ref ClearEffect の処理が完了していれば true を、
    //!               そうでなければ false を返します。
    //!
    //! @see ClearEffect
    //! @see AuxBus
    //---------------------------------------------------------------------------
    static bool IsClearEffectFinished( AuxBus bus, OutputDevice device ) NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name エフェクト
    //@{
    //! @briefprivate
    static bool IsClearEffectFinished( AuxBus bus, OutputDevice device, int subMixIndex ) NN_NOEXCEPT;

    //! @briefprivate
    static bool IsClearEffectFromFinalMixFinished() NN_NOEXCEPT;

    //! @briefprivate
    static bool IsClearEffectFromAdditionalSubMixFinished() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief        AuxBus のボリュームを設定します。
    //!
    //!               @ref SetAuxBusVolume で設定した値は、 AuxBus に送られた音すべてに対して影響を与えます。
    //!
    //!               値の範囲は nn::audio::SubMixType::GetVolumeMin() ～ nn::audio::SubMixType::GetVolumeMax() でクランプされます。
    //!               デフォルト値は 1.0 です。
    //!
    //!               音量の変化は fadeTimes で指定した時間をかけて行われます。
    //!               音量の変化途中にさらにこの関数を呼び出した場合は、
    //!               その時点での変化途中の音量値を基点として、
    //!               新しく指定した時間をかけて音量を変化させます。
    //!               この関数で設定した値が @ref GetAuxBusVolume に反映されるまでは、
    //!               最大 1 オーディオフレーム分の時間がかかる点にご注意ください。
    //!
    //! @param[in] bus        音量を変化する AuxBus です。
    //! @param[in] volume     音量の倍率です。
    //! @param[in] fadeTimes  音量変化にかける時間です。
    //!
    //! @see  GetAuxBusVolume
    //!
    //---------------------------------------------------------------------------
    static void SetAuxBusVolume(AuxBus bus, float volume, nn::TimeSpan fadeTimes) NN_NOEXCEPT;

    //@}
    //! @internal
    static void SetAuxBusVolume(AuxBus bus, float volume, nn::TimeSpan fadeTimes, int subMixIndex) NN_NOEXCEPT;

    //----------------------------------------
    //! @name エフェクト
    //@{
    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief        現在の AuxBus のボリュームを取得します。
    //!
    //!               現在設定されている AuxBus のボリュームの値を取得します。
    //!
    //!               音量が時間変化している間に値を取得した場合は、
    //!               その時点での変化途中の音量の値を返します。
    //!
    //! @param[in] bus     音量を変化する AuxBus です。
    //!
    //! @return       現在の AuxBus のボリュームの値を返します。
    //!
    //! @see          SetAuxBusVolume
    //!
    //---------------------------------------------------------------------------
    static float GetAuxBusVolume(AuxBus bus) NN_NOEXCEPT;

    //@}
    //! @internal
    static float GetAuxBusVolume(AuxBus bus, int subMixIndex) NN_NOEXCEPT;

    //----------------------------------------
    //! @name エフェクト
    //@{
    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief        追加のエフェクトバス使用時に、
    //!               サブミックスのメインバスの特定チャンネルから
    //!               ファイナルミックスの特定チャンネルへのミックスの音量を設定します。
    //!
    //!               値の範囲は nn::audio::SubMixType::GetVolumeMin() ～ nn::audio::SubMixType::GetVolumeMax() でクランプされます。
    //!
    //!               音量の初期値は FrontLeft→FrontLeft, FrontRight→FrontRight の場合は 1.0f、
    //!               それ以外の場合は 0.0f になります。
    //!
    //! @param[in] volume           音量の倍率です。
    //! @param[in] srcChannelIndex  サブミックス側のチャンネル番号です。
    //! @param[in] destChannelIndex ファイナルミックス側のチャンネル番号です。
    //---------------------------------------------------------------------------
    static void SetMainBusChannelVolumeForAdditionalEffect(float volume, int srcChannelIndex, int destChannelIndex) NN_NOEXCEPT;
    //! @briefprivate
    static float GetMainBusChannelVolumeForAdditionalEffect(int srcChannelIndex, int destChannelIndex) NN_NOEXCEPT;
    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief        追加のエフェクトバス使用時に、
    //!               サブミックスのエフェクトバスの特定チャンネルから
    //!               ファイナルミックスの特定チャンネルへのミックスの音量を設定します。
    //!
    //!               値の範囲は nn::audio::SubMixType::GetVolumeMin() ～ nn::audio::SubMixType::GetVolumeMax() でクランプされます。
    //!
    //!               音量の初期値は FrontLeft→FrontLeft, FrontRight→FrontRight の場合は 1.0f、
    //!               それ以外の場合は 0.0f になります。
    //!
    //! @param[in] bus              設定を行うエフェクトバスです。
    //! @param[in] volume           音量の倍率です。
    //! @param[in] srcChannelIndex  サブミックス側のチャンネル番号です。
    //! @param[in] destChannelIndex ファイナルミックス側のチャンネル番号です。
    //---------------------------------------------------------------------------
    static void SetAuxBusChannelVolumeForAdditionalEffect(AuxBus bus, float volume, int srcChannelIndex, int destChannelIndex) NN_NOEXCEPT;
    //! @briefprivate
    static float GetAuxBusChannelVolumeForAdditionalEffect(AuxBus bus, int srcChannelIndex, int destChannelIndex) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief        追加のエフェクトバス使用時に、
    //!               サブミックスの全てのエフェクトバスの特定チャンネルから
    //!               ファイナルミックスの特定チャンネルへのミックスの音量を設定します。
    //!
    //!               エフェクトバス毎の @ref SetAuxBusChannelVolumeForAdditionalEffect() をまとめて呼ぶ場合と同じ処理を行います。
    //!
    //!               値の範囲は nn::audio::SubMixType::GetVolumeMin() ～ nn::audio::SubMixType::GetVolumeMax() でクランプされます。
    //!
    //!               音量の初期値は FrontLeft→FrontLeft, FrontRight→FrontRight の場合は 1.0f、
    //!               それ以外の場合は 0.0f になります。
    //!
    //! @param[in] volume           音量の倍率です。
    //! @param[in] srcChannelIndex  サブミックス側のチャンネル番号です。
    //! @param[in] destChannelIndex ファイナルミックス側のチャンネル番号です。
    //---------------------------------------------------------------------------
    static void SetAllAuxBusChannelVolumeForAdditionalEffect(float volume, int srcChannelIndex, int destChannelIndex) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief        追加のエフェクトバス使用時に、
    //!               サブミックスの全てのバスの特定チャンネルから
    //!               ファイナルミックスの特定チャンネルへのミックスの音量を設定します。
    //!
    //!               @ref SetMainBusChannelVolumeForAdditionalEffect() とエフェクトバス毎の @ref SetAuxBusChannelVolumeForAdditionalEffect() を
    //!               まとめて呼ぶ場合と同じ処理を行います。
    //!
    //!               値の範囲は nn::audio::SubMixType::GetVolumeMin() ～ nn::audio::SubMixType::GetVolumeMax() でクランプされます。
    //!
    //!               音量の初期値は FrontLeft→FrontLeft, FrontRight→FrontRight の場合は 1.0f、
    //!               それ以外の場合は 0.0f になります。
    //!
    //! @param[in] volume           音量の倍率です。
    //! @param[in] srcChannelIndex  サブミックス側のチャンネル番号です。
    //! @param[in] destChannelIndex ファイナルミックス側のチャンネル番号です。
    //---------------------------------------------------------------------------
    static void SetAllBusChannelVolumeForAdditionalEffect(float volume, int srcChannelIndex, int destChannelIndex) NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name SRC タイプ
    //@{
    //---------------------------------------------------------------------------
    //! @briefprivate
    //!
    //! @brief      SRC タイプ (サンプルレートコンバータの補間方法) を設定します。
    //!
    //!             デフォルトでは SampleRateConverterType_4Tap が設定されています。
    //!
    //!             ウェーブサウンドおよびストリームサウンドに影響がありますが、
    //!             シーケンスサウンドには影響がありません。
    //!             シーケンスサウンドは、SoundMaker のインストリストおよび
    //!             パーカッションリスト上で設定された「補間タイプ」が影響します。
    //!
    //! @param[in]    type    SRC タイプです。
    //!
    //! @see nn::atk::SampleRateConverterType
    //! @see GetSampleRateConverterType
    //!
    //---------------------------------------------------------------------------
    static void SetSampleRateConverterType( SampleRateConverterType type ) NN_NOEXCEPT
    {
        detail::driver::HardwareManager::GetInstance().SetSrcType( type );
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //!
    //! @brief        現在設定されている SRC タイプ
    //!               (サンプルレートコンバータの補間方法) を取得します。
    //!
    //!               デフォルトでは SampleRateConverterType_4Tap が設定されています。
    //!
    //! @return       現在設定されている SRC タイプの値を返します。
    //!
    //! @see nn::atk::SampleRateConverterType
    //! @see SetSampleRateConverterType
    //!
    //---------------------------------------------------------------------------
    static SampleRateConverterType GetSampleRateConverterType() NN_NOEXCEPT
    {
        return detail::driver::HardwareManager::GetInstance().GetSrcType();
    }
    //@}

    //---------------------------------------------------------------------------
    //! @briefprivate
    //!
    //! @brief        リセット前に必要なサウンドの処理を行います。
    //!
    //!               この関数は、リセット時にノイズが発生しないように再生中の音を緩やかに消します。
    //!               一度この関数が呼ばれた後は、再び発音することができません。
    //!
    //!               ノイズが発生しないようにリセットを行うためには、
    //!               @ref WaitForResetReady を呼び出して、
    //!               この関数の処理が完了するのを待つ必要があります。
    //!
    //! @see  WaitForResetReady
    //---------------------------------------------------------------------------
    static void PrepareReset() NN_NOEXCEPT
    {
        detail::driver::HardwareManager::GetInstance().PrepareReset();
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //!
    //! @brief        サウンドのリセット準備処理が完了するのを待ちます。
    //!
    //!               この関数は、@ref PrepareReset が呼び出された後に、
    //!               その処理が完了するのを待ちます。
    //!
    //! @see  PrepareReset
    //---------------------------------------------------------------------------
    static void WaitForResetReady() NN_NOEXCEPT;

    //----------------------------------------
    //! @name パフォーマンス計測
    //@{
    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @deprecated SoundSystemParam::rendererSampleRate で 48kHz を指定することで制約もなく 48kHz レンダリングできるためそちらを使用してください。
    //!             互換性のために残されていますが、これらの API はすでに使用できない状態になっていて、今後削除される予定です。
    //!
    //! @param sampleRate :private
    //---------------------------------------------------------------------------
    static void SetNwVoiceSampleRate( uint32_t sampleRate ) NN_NOEXCEPT
    {
        NN_UNUSED(sampleRate);
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //!
    //! @brief        サウンドスレッド処理間隔をマイクロ秒換算した値です。
    //---------------------------------------------------------------------------
    static const int SoundThreadIntervalUsec = nn::atk::detail::driver::HardwareManager::SoundFrameIntervalUsec;
    //@}

    //----------------------------------------
    //! @name シーケンス変数出力設定
    //@{
    // シーケンス変数のプリント ON/OFF を設定する
    //---------------------------------------------------------------------------
    //! @brief        printvar シーケンスコマンドを有効にします。
    //!
    //!               デフォルトでは無効になっています。
    //!               詳しくは、シーケンスデータマニュアルをご覧ください。
    //!
    //!               printvar シーケンスコマンドを有効にすると、
    //!               無効のときと比べて printvar コマンド処理で余分に処理がかかるので、
    //!               シーケンス変数デバッグをしない場合は無効にしておくことを推奨します。
    //!
    //! @see  DisableSeqPrintVar
    //! @see  IsEnabledSeqPrintVar
    //!
    //---------------------------------------------------------------------------
    static void EnableSequencePrintVar() NN_NOEXCEPT { detail::driver::MmlParser::EnablePrintVar( true ); }

    //---------------------------------------------------------------------------
    //! @brief        printvar シーケンスコマンドを無効にします。
    //!
    //!               詳しくは、シーケンスデータマニュアルをご覧ください。
    //!
    //! @see  EnableSeqPrintVar
    //! @see  IsEnabledSeqPrintVar
    //!
    //---------------------------------------------------------------------------
    static void DisableSequencePrintVar() NN_NOEXCEPT { detail::driver::MmlParser::EnablePrintVar( false ); }

    //---------------------------------------------------------------------------
    //! @brief        printvar シーケンスコマンドが有効かどうかを取得します。
    //!
    //!               true のときは有効、false のときは無効です。
    //!
    //!               printvar シーケンスコマンドを有効にすると、
    //!               無効のときと比べて printvar コマンド処理で余分に処理がかかるので、
    //!               シーケンス変数デバッグをしない場合は無効にしておくことを推奨します。
    //!
    //! @return       現在、printvar シーケンスコマンドが有効か無効かを取得します。
    //!
    //! @see  EnableSeqPrintVar
    //! @see  DisableSeqPrintVar
    //!
    //---------------------------------------------------------------------------
    static bool IsEnabledSequencePrintVar() NN_NOEXCEPT
    {
        return detail::driver::MmlParser::IsEnabledPrintVar();
    }
    //@}

    //----------------------------------------
    //! @name 使用ボイス数
    //@{
    //---------------------------------------------------------------------------
    //! @brief        サウンドライブラリで現在発音中のボイス数を取得します。
    //!
    //! @return       現在発音中のボイス数を返します。
    //!
    //! @see GetVoiceCountMax
    //!
    //---------------------------------------------------------------------------
    static int GetVoiceCount() NN_NOEXCEPT
    {
        return detail::driver::MultiVoiceManager::GetInstance().GetVoiceCount();
    }

    //---------------------------------------------------------------------------
    //! @brief        サウンドライブラリで使用できる最大ボイス数を取得します。
    //!
    //!               デフォルトでは、@ref SoundSystemParam クラスで定義される DefaultVoiceCountMax の値を返し、
    //!               初期化後は @ref SoundSystemParam クラスで設定した voiceCountMax の値を返します。
    //!
    //! @return       最大ボイス数を返します。
    //!
    //! @see Initialize
    //! @see GetVoiceCount
    //! @see VoiceCountMax
    //!
    //---------------------------------------------------------------------------
    static int GetVoiceCountMax() NN_NOEXCEPT
    {
        return g_VoiceCountMax;
    }

    //---------------------------------------------------------------------------
    //! @brief        この関数は非推奨です。サウンドライブラリで使用する最大ボイス数を設定します。
    //! @deprecated   @ref SoundSystemParam に使用するボイス数を設定する変数が用意されました。そちらの利用に切り替えてください。
    //!
    //! @param[in] count  最大ボイス数です。
    //! @pre
    //! - 1 <= count <= SoundSystemParam::DefaultVoiceCountMax
    //!
    //! @see Initialize
    //! @see GetVoiceCount
    //! @see GetVoiceCountMax
    //! @see VoiceCountMax
    //!
    //---------------------------------------------------------------------------
    NN_DEPRECATED static void SetVoiceCountMax( int count ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_MINMAX( count, 1, SoundSystem::VoiceCountMax );
        count = detail::fnd::Clamp<int>( count, 1, SoundSystem::VoiceCountMax );
        g_VoiceCountMax = count;
    }
    //@}

    //! @internal
    static int GetRendererSampleRate() NN_NOEXCEPT
    {
        return g_RendererSampleRate;
    }

    //----------------------------------------
    //! @name   Biquad フィルタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief  ユーザー定義の Biquad フィルタを登録します。
    //!
    //!         @ref BiquadFilterCallback クラスを継承してユーザーが実装した
    //!         Biquad フィルタを登録します。
    //!
    //!         type には、@ref BiquadFilterType で定義されている、
    //!         BIQUAD_FILTER_TYPE_USER_MIN ～ BIQUAD_FILTER_TYPE_USER_MAX
    //!         の値を指定します。
    //!         範囲外の値を設定すると、Debug 版 / Development 版ではアサートで停止します。
    //!         Release 版では無視されます。
    //!
    //!         ここで設定したコールバック関数はサウンドスレッドから呼び出されます。
    //!         排他制御が必要な場合は、
    //!         @ref SoundThreadScopedLock クラスあるいは、
    //!         @ref LockSoundThread / @ref UnlockSoundThread 関数を利用する必要があります。
    //!
    //!         コールバック関数はすみやかに処理を終える必要があります。
    //!         処理が長引くと、ノイズが発生する可能性が高くなります。
    //!         たとえば、コールバック関数内でブロックする可能性のある API
    //!         (クリティカルセクションなど) を呼び出すと、
    //!         処理を終えるのに時間がかかる可能性があります。
    //!
    //! @param[in] type     フィルタの種類です
    //!                     (BIQUAD_FILTER_TYPE_USER_MIN ～BIQUAD_FILTER_TYPE_USER_MAX)。
    //! @param[in] biquad   ユーザーが実装した Biquad フィルタクラスです。
    //!
    //! @see BiquadFilterCallback クラス
    //! @see BiquadFilterType
    //!
    //---------------------------------------------------------------------------
    static void SetBiquadFilterCallback( int type, const BiquadFilterCallback* biquad ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( type >= BiquadFilterType_UserMin && type <= BiquadFilterType_UserMax );
        detail::driver::HardwareManager::GetInstance().SetBiquadFilterCallback( type, biquad );
    }
    //@}

    //----------------------------------------
    //! @name   パフォーマンス計測
    //@{
    //---------------------------------------------------------------------------
    //! @brief  プロファイルリーダーを登録します。
    //!
    //! @param[in] profileReader 登録するプロファイルリーダーです。
    //!
    //! @see ProfileReader クラス
    //---------------------------------------------------------------------------
    static void RegisterProfileReader( ProfileReader& profileReader ) NN_NOEXCEPT
    {
        detail::driver::SoundThread::GetInstance().RegisterProfileReader( profileReader );
    }

    //---------------------------------------------------------------------------
    //! @brief  プロファイルリーダーの登録を解除します。
    //!
    //! @param[in] profileReader 登録を解除するプロファイルリーダーです。
    //!
    //! @see ProfileReader クラス
    //---------------------------------------------------------------------------
    static void UnregisterProfileReader( ProfileReader& profileReader ) NN_NOEXCEPT
    {
        detail::driver::SoundThread::GetInstance().UnregisterProfileReader( profileReader );
    }
    //@}

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  タスクプロファイルリーダーを登録します。
    //!
    //! @param[in] taskProfileReader 登録するタスクプロファイルリーダーです。
    //!
    //! @pre
    //! - taskProfileReader は初期化されている
    //!
    //! @see TaskProfileReader クラス
    //---------------------------------------------------------------------------
    static void RegisterTaskProfileReader(TaskProfileReader& taskProfileReader) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( taskProfileReader.IsInitialized() );
        detail::TaskManager::GetInstance().RegisterTaskProfileReader( taskProfileReader );
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  タスクプロファイルリーダーの登録を解除します。
    //!
    //! @param[in] taskProfileReader 登録を解除するタスクプロファイルリーダーです。
    //!
    //! @see TaskProfileReader クラス
    //---------------------------------------------------------------------------
    static void UnregisterTaskProfileReader(const TaskProfileReader& taskProfileReader) NN_NOEXCEPT
    {
        detail::TaskManager::GetInstance().UnregisterTaskProfileReader( taskProfileReader );
    }

    //! @briefprivate
    //! @brief  サウンドスレッドの更新プロファイルリーダーを登録します。
    //! @param[in] profileReader 登録するプロファイルリーダーです。
    static void RegisterSoundThreadUpdateProfileReader(SoundThreadUpdateProfileReader& profileReader) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief  サウンドスレッドの更新プロファイルリーダーの登録を解除します。
    //! @param[in] profileReader 登録を解除するプロファイルリーダーです。
    static void UnregisterSoundThreadUpdateProfileReader(SoundThreadUpdateProfileReader& profileReader) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief  サウンドスレッドの情報レコーダーを登録します。
    //! @param[in] recorder 登録する情報レコーダーです。
    static void RegisterSoundThreadInfoRecorder(SoundThreadInfoRecorder& recorder) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief  サウンドスレッドの情報レコーダーの登録を解除します。
    //! @param[in] recorder 登録を解除する情報レコーダーです。
    static void UnregisterSoundThreadInfoRecorder(SoundThreadInfoRecorder& recorder) NN_NOEXCEPT;

    //----------------------------------------
    //! @name   CircularBufferSink のレンダリング結果取得
    //@{
    //---------------------------------------------------------------------------
    //! @brief CircularBufferSink を使ってレンダリング結果を取得します。
    //!
    //!        CircularBufferSink に保持されているレンダリング結果のサンプルデータを pOutBuffer に読み込みます。
    //!        読み込むサンプルデータはチャンネルごとに @ref nn::atk::ChannelIndex で定義される順でブロックインタリーブされます。
    //!        レンダリング結果の取得を有効にするには @ref SoundSystemParam::enableCircularBufferSink を true にして SoundSystem を初期化してください。
    //!        取得できるレンダリング結果のサンプルデータの詳細につきましては @ref nn::audio::ReadCircularBufferSink を参照ください。
    //!
    //!        CircularBufferSink のレンダリング結果の取得をオーディオフレームの周期で行う事で
    //!        録音波形の取りこぼしを軽減できる場合があります。
    //!        @ref SetSoundFrameUserCallback を使用することでサウンドスレッド上で処理を行うことが可能になりますが、
    //!        コールバック上で処理時間の大きな処理を行った場合、サウンドスレッドの更新処理に遅延が発生するため注意してください。
    //!
    //!        @ref SoundSystemParam::enableCircularBufferSink を false のまま SoundSystem を初期化してこの関数を呼び出した場合は、
    //!        pOutBuffer には何も読み込まれず、関数の返り値として 0 が返されます。このとき、1 度だけ CircularBufferSink が無効である警告が表示されます。
    //!
    //! @param[out] pOutBuffer   読み込み先となるバッファです。
    //! @param[in]  bufferSize   pOutBuffer に指定したバッファのサイズです。
    //!
    //! @return                  pOutBuffer へ読み込まれたサンプルデータのサイズです。
    //! @pre                     pOutBuffer != nullptr
    //---------------------------------------------------------------------------
    static size_t ReadCircularBufferSink(void* pOutBuffer, size_t bufferSize) NN_NOEXCEPT;
    //---------------------------------------------------------------------------
    //! @brief CircularBufferSink が保持できるサンプルデータのサイズを取得します。
    //!
    //!        CircularBufferSink が保持できるサンプルデータのサイズを示しており
    //!        @ref SoundSystemParam::recordingAudioFrameCount の値に依存します。
    //!        @ref ReadCircularBufferSink でサンプルデータを読み込んだ場合は、最大でこのサイズのデータを取得する事が可能です。
    //!
    //! @return CircularBufferSink が保持できるサンプルデータのサイズです。
    //---------------------------------------------------------------------------
    static size_t GetCircularBufferSinkBufferSize() NN_NOEXCEPT;
    //---------------------------------------------------------------------------
    //! @brief レンダラが１オーディオフレームあたりにレンダリングするサンプル数を取得します。
    //!
    //!        @ref ReadCircularBufferSink でサンプルデータを取得した場合、
    //!        ( @ref GetRendererSampleCount * @ref GetRendererChannelCountMax * 2 (サンプルデータはPCM16のため1サンプルあたり2byte) )
    //!        の整数倍のサイズのサンプルデータを一度に取得します。
    //!
    //!        取得したサンプルデータはブロックインターリーブされているため、
    //!        サンプルデータの処理のためにこの関数で取得したサンプル数をご利用ください。
    //!        @ref ReadCircularBufferSink で取得したサンプルデータの詳細につきましては @ref nn::audio::ReadCircularBufferSink を参照ください。
    //!
    //! @return レンダラが１オーディオフレームあたりにレンダリングするサンプル数です。
    //---------------------------------------------------------------------------
    static int GetRendererSampleCount() NN_NOEXCEPT;
    //---------------------------------------------------------------------------
    //! @brief レンダラがレンダリングするチャンネル数を取得します。
    //!
    //!        @ref ReadCircularBufferSink でサンプルデータを取得した場合、
    //!        ( @ref GetRendererSampleCount * @ref GetRendererChannelCountMax * 2 (サンプルデータはPCM16のため1サンプルあたり2byte) )
    //!        の整数倍のサイズのサンプルデータを一度に取得します。
    //!
    //!        取得したサンプルデータはブロックインターリーブされているため、
    //!        サンプルデータの処理のためにこの関数で取得したチャンネル数をご利用ください。
    //!        @ref ReadCircularBufferSink で取得したサンプルデータの詳細につきましては @ref nn::audio::ReadCircularBufferSink を参照ください。
    //!
    //! @return レンダラがレンダリングするチャンネル数です。
    //---------------------------------------------------------------------------
    static int GetRendererChannelCountMax() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief CircularBufferSink の動作を停止します。
    //!
    //!        この関数は、 SoundSystem の初期化時に @ref SoundSystemParam::enableCircularBufferSink を true にし、
    //!        CircularBufferSink を利用する設定にした場合のみ使用することができます。
    //!        それ以外の場合、 CircularBufferSink が利用できないことを示す警告が表示されます。
    //!
    //!        通常、 SoundSystem が利用する CircularBufferSink は、 @ref ReadCircularBufferSink を利用しない場合でも動作し続けますが、
    //!        この関数の呼び出しにより CircularBufferSink の動作を停止することができます。
    //!        これにより、 CircularBufferSink が動作することによるパフォーマンスへの影響を抑えることができます。
    //!        停止している間は、@ref ReadCircularBufferSink の pOutBuffer には何も読み込まれず、@ref ReadCircularBufferSink の返り値として 0 が返されます。
    //!        再度動作させるためには @ref StartCircularBufferSink を呼び出します。
    //!
    //! @see SoundSystemParam::enableCircularBufferSink
    //! @see ReadCircularBufferSink
    //! @see StartCircularBufferSink
    //! @see GetCircularBufferSinkState
    //---------------------------------------------------------------------------
    static void StopCircularBufferSink() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief CircularBufferSink を動作させます。
    //!
    //!        この関数は、 SoundSystem の初期化時に @ref SoundSystemParam::enableCircularBufferSink を true にし、
    //!        CircularBufferSink を利用する設定にした場合のみ使用することができます。
    //!        それ以外の場合、 CircularBufferSink が利用できないことを示す警告が表示されます。
    //!
    //!        @ref StopCircularBufferSink で停止させた CircularBufferSink を、再度動作させるために利用します。
    //!
    //! @see SoundSystemParam::enableCircularBufferSink
    //! @see ReadCircularBufferSink
    //! @see StopCircularBufferSink
    //! @see GetCircularBufferSinkState
    //---------------------------------------------------------------------------
    static void StartCircularBufferSink() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief SoundSystem が利用する CircularBufferSink の動作状態を取得します。
    //!
    //! @return CircularBufferSink の動作状態です。
    //!
    //! @see SoundSystemParam::enableCircularBufferSink
    //! @see StopCircularBufferSink
    //! @see StartCircularBufferSink
    //---------------------------------------------------------------------------
    static CircularBufferSinkState GetCircularBufferSinkState() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name   オーディオレンダラのパフォーマンス情報の計測
    //@{
    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  オーディオレンダラのパフォーマンス情報リーダーを登録します。
    //!         パフォーマンス情報リーダーは 1 つしか登録できません。
    //!
    //!         パフォーマンス情報リーダーを再登録するときは、
    //!         @ref UnregisterAudioRendererPerformanceReader で登録を解除してから再登録してください。
    //!
    //! @param[in] performanceReader 登録するパフォーマンス情報リーダーです。
    //! @pre
    //! - performanceReader を初期化済み
    //! - @ref SoundSystemParam::enableProfiler を有効にして @ref Initialize している
    //! - AudioRendererPerformanceReader のインスタンスが 1 つも登録されていない
    //!
    //! @see AudioRendererPerformanceReader クラス
    //! @see UnregisterAudioRendererPerformanceReader
    //---------------------------------------------------------------------------
    static void RegisterAudioRendererPerformanceReader( AudioRendererPerformanceReader& performanceReader ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  オーディオレンダラのパフォーマンス情報リーダーの登録を解除します。
    //!
    //! @see AudioRendererPerformanceReader クラス
    //! @see RegisterAudioRendererPerformanceReader
    //---------------------------------------------------------------------------
    static void UnregisterAudioRendererPerformanceReader() NN_NOEXCEPT
    {
        detail::driver::SoundThread::GetInstance().UnregisterAudioRendererPerformanceReader();
    }
    //@}

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief @ref nn::atk::FinalMixCallback をリストの末尾に追加します。
    //!
    //!        追加した @ref nn::atk::FinalMixCallback は先頭から順に呼び出されます。
    //!
    //! @param[in] userCallback 追加する @ref nn::atk::FinalMixCallback です。
    //!
    //! @see FinalMixCallback クラス
    //---------------------------------------------------------------------------
    static void AppendFinalMixCallback( FinalMixCallback* userCallback ) NN_NOEXCEPT
    {
#if 0 // 参考用 : NW4F 実装
        detail::driver::HardwareManager::GetInstance().AppendFinalMixCallback( userCallback );
#else
        // TODO: FinalMix の対応
        NN_UNUSED( userCallback );
#endif
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief @ref nn::atk::FinalMixCallback をリストの先頭に追加します。
    //!
    //!        追加した @ref nn::atk::FinalMixCallback は先頭から順に呼び出されます。
    //!
    //! @param[in] userCallback 追加する @ref nn::atk::FinalMixCallback です。
    //!
    //! @see FinalMixCallback クラス
    //---------------------------------------------------------------------------
    static void PrependFinalMixCallback( FinalMixCallback* userCallback ) NN_NOEXCEPT
    {
#if 0 // 参考用 : NW4F 実装
        detail::driver::HardwareManager::GetInstance().PrependFinalMixCallback( userCallback );
#else
        // TODO: FinalMix の対応
        NN_UNUSED( userCallback );
#endif
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief @ref nn::atk::FinalMixCallback をリストから削除します。
    //!
    //! @param[in] userCallback 削除する @ref nn::atk::FinalMixCallback です。
    //!
    //! @see FinalMixCallback クラス
    //---------------------------------------------------------------------------
    static void EraseFinalMixCallback( FinalMixCallback* userCallback ) NN_NOEXCEPT
    {
#if 0 // 参考用 : NW4F 実装
        detail::driver::HardwareManager::GetInstance().EraseFinalMixCallback( userCallback );
#else
        // TODO: FinalMix の対応
        NN_UNUSED( userCallback );
#endif
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief 警告表示時に実行されるコールバックを設定します。
    //!
    //! @param[in] callback 設定する WarningCallback です。
    //!
    //! @see WarningCallback
    //---------------------------------------------------------------------------
    static void SetWarningCallback(WarningCallback callback) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief 警告表示時に実行されるコールバックをクリアします。
    //---------------------------------------------------------------------------
    static void ClearWarningCallback() NN_NOEXCEPT;

    //! @briefprivate
    //! @brief  警告が出力されたときに呼び出すコールバックが設定されていれば実行します。
    static void CallWarningCallback(WarningId id, IWarningCallbackInfo* pInfo) NN_NOEXCEPT;

    //! @briefprivate
    //! @param updateType :private
    //! @param frameCount :private
    static void VoiceCommandProcess(UpdateType updateType, uint32_t frameCount) NN_NOEXCEPT;

    //! @briefprivate
    //! @param frameCount :private
    static void VoiceCommandProcess(uint32_t frameCount) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief サウンドスレッドの１オーディオフレームにあたる更新処理を呼び出します。
    //!        この API を使用する場合は VoiceCommand を使用していて
    //!        かつ SoundSystem::enableSoundThread でサウンドスレッドが無効化されていることを必ず確認してください。
    static void VoiceCommandUpdate() NN_NOEXCEPT;

    //! @briefprivate
    static size_t GetPerformanceFrameBufferSize() NN_NOEXCEPT;

    //! @briefprivate
    //! @brief  ボイスドロップ使用時に、直前のオーディオフレームでドロップしたSDKレイヤのボイス数を取得します。
    //!
    //! @return 直前のオーディオフレームでドロップしたSDKレイヤのボイス数を返します。
    static int GetDroppedLowLevelVoiceCount() NN_NOEXCEPT;

#if 0 // 参考用 : NW4F 実装
    //----------------------------------------
    //! @name   メモリ管理
    //@{
    //---------------------------------------------------------------------------
    //! @brief     指定メモリ領域が DSP レンダリング時に有効かどうかを調べます。
    //!
    //!            ハードウェアの制約により、DSP レンダリングに利用する波形データ領域は
    //!            512 MB 境界をまたぐことができません。
    //!
    //!            上記制約に該当すると、この関数は false を返します。
    //!
    //!            詳しくは CafeSDK リファレンスマニュアルの AXCheckVoiceOffsets() を参照ください。
    //!
    //! @param[in] ptr  メモリ領域へのポインタです。
    //! @param[in] size メモリ領域のサイズです。
    //!
    //! @return    有効なメモリ領域の場合は true, そうでない場合は false を返します。
    //---------------------------------------------------------------------------
    static bool IsValidMemoryForDsp( const void* ptr, size_t size ) NN_NOEXCEPT
    {
        return detail::Util::IsValidMemoryForDsp( ptr, size );
    }
    //@}
#endif

private:
    static bool detail_InitializeSoundSystem(
        Result* pOutResult,
        const SoundSystemParam& param,
        InitializeBufferSet& bufferSet
    ) NN_NOEXCEPT;

    static void detail_InitializeDriverCommandManager(
        const SoundSystemParam& param,
        uintptr_t workMem,
        size_t workMemSize,
        uintptr_t memoryPoolMem,
        size_t memoryPoolMemSize ) NN_NOEXCEPT;

    static void SetupInitializeBufferSet(
        InitializeBufferSet* pOutValue,
        const SoundSystemParam& param,
        const InitializeBufferSet& bufferSet) NN_NOEXCEPT;

    static void SetupHardwareManagerParameterFromCurrentSetting(
        nn::atk::detail::driver::HardwareManager::HardwareManagerParameter* pOutValue) NN_NOEXCEPT;

private:

    static void* g_LoadThreadStackPtr;
    static size_t g_LoadThreadStackSize;
    static FsPriority g_TaskThreadFsPriority;
    static void* g_SoundThreadStackPtr;
    static size_t g_SoundThreadStackSize;
    static void* g_PerformanceFrameBuffer;
    static size_t g_PerformanceFrameBufferSize;

    static bool g_IsInitialized;
    static bool g_IsStreamLoadWait;
    static bool g_IsEnterSleep;
    static bool g_IsInitializedDriverCommandManager;
    static bool g_IsStreamOpenFailureHalt;
    static bool g_IsTaskThreadEnabled;
    static bool g_IsManagingMemoryPool;

    static size_t g_SoundThreadCommandBufferSize;
    static int g_SoundThreadCommandQueueCount;
    static size_t g_TaskThreadCommandBufferSize;
    static int g_TaskThreadCommandQueueCount;
    static size_t g_VoiceCommandBufferSize;
    static nn::audio::MemoryPoolType g_MemoryPoolForSoundSystem;

    // HardwareManager で使用するパラメータ
    static int g_RendererSampleRate;
    static int g_VoiceCountMax;
    static int g_UserEffectCount;
    static int g_CustomSubMixSubMixCount;
    static int g_CustomSubMixMixBufferCount;
    static bool g_IsProfilerEnabled;
    static bool g_IsDetailSoundThreadProfilerEnabled;
    static bool g_IsAdditionalEffectBusEnabled;
    static bool g_IsAdditionalSubMixEnabled;
    static bool g_IsEffectEnabled;
    static bool g_IsRecordingEnabled;
    static bool g_IsCircularBufferSinkEnabled;
    static bool g_IsCircularBufferSinkWarningDisplayed;
    static bool g_IsSubMixEnabled;
    static bool g_IsPresetSubMixEnabled;
    static bool g_IsStereoModeEnabled;
    static bool g_IsSoundThreadEnabled;
    static bool g_IsVoiceDropEnabled;
    static bool g_IsPreviousSdkVersionLowPassFilterCompatible;
    static bool g_IsUnusedEffectChannelMutingEnabled;
    static bool g_IsUserThreadRenderingEnabled;
    static bool g_IsCustomSubMixEnabled;
    static bool g_IsMemoryPoolAttachCheckEnabled;
    static bool g_IsBusMixVolumeEnabled;
    static bool g_IsVolumeThroughModeEnabled;
    static int g_BusCountMax;

    static WarningCallback g_WarningCallback;
};

} // namespace nn::atk
} // namespace nn

