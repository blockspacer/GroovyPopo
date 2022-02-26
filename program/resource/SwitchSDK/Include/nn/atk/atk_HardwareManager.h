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

#include <nn/util/util_IntrusiveList.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_MoveValue.h>
#include <nn/atk/atk_Util.h>
#include <nn/atk/atk_FinalMixCallback.h>
#include <nn/atk/atk_EffectBase.h>
#include <nn/atk/atk_EffectAux.h>
#include <nn/atk/atk_BiquadFilterCallback.h>
#include <nn/atk/atk_BiquadFilterPresets.h>
#include <nn/atk/atk_LowLevelVoice.h>
#include <nn/atk/atk_SubMix.h>
#include <nn/atk/atk_FinalMix.h>
#include <nn/audio.h>

namespace nn {
namespace atk {

class DeviceOutRecorder;

}
}

namespace nn {
namespace atk {
namespace detail {
namespace driver {

class HardwareManager : public Util::Singleton<HardwareManager>
{
public:
    //! @internal
    //! @brief HardwareManager の初期化用パラメータです。
    class HardwareManagerParameter
    {
    public:
        //! @internal
        //! @brief コンストラクタです。
        HardwareManagerParameter() NN_NOEXCEPT
            : m_RendererSampleRate( DefaultRendererSampleRate )
            , m_UserEffectCount( DefaultRendererUserEffectCount )
            , m_VoiceCount( DefaultRendererVoiceCountMax )
            , m_RecordingAudioFrameCount( DefaultRecordingAudioFrameCount )
            , m_SubMixCount( DefaultRendererSubMixCount )
            , m_MixBufferCount( DefaultRendererMixBufferCount )
            , m_IsProfilerEnabled( false )
            , m_IsAdditionalEffectBusEnabled( false )
            , m_IsAdditionalSubMixEnabled( false )
            , m_IsEffectEnabled( true )
            , m_IsRecordingEnabled( false )
            , m_IsUserCircularBufferSinkEnabled( false )
            , m_IsPresetSubMixEnabled( true )
            , m_IsStereoModeEnabled( false )
            , m_IsSoundThreadEnabled( true )
            , m_IsVoiceDropEnabled( false )
            , m_IsCompatibleDownMixSettingEnabled( false )
            , m_IsPreviousSdkVersionLowPassFilterCompatible( false )
            , m_IsUnusedEffectChannelMutingEnabled( false )
            , m_IsCompatibleBusVolumeEnabled( false )
            , m_IsCompatiblePanCurveEnabled( false )
            , m_IsUserThreadRenderingEnabled( false )
            , m_IsCustomSubMixEnabled( false )
            , m_IsMemoryPoolAttachCheckEnabled( false )
        {
        }
        //! @internal
        //! @brief オーディオレンダラのサンプルレートを取得します。
        //! @return オーディオレンダラのサンプルレートです。
        int GetRendererSampleRate() const NN_NOEXCEPT
        {
            return m_RendererSampleRate;
        }
        //! @internal
        //! @brief オーディオレンダラのサンプルレートを設定します。
        //! @param[in] rendererSampleRate オーディオレンダラのサンプルレートです。
        void SetRendererSampleRate( int rendererSampleRate ) NN_NOEXCEPT
        {
            m_RendererSampleRate = rendererSampleRate;
        }
        //! @internal
        //! @brief ユーザが使用するエフェクトの数を取得します。
        //! @return ユーザが使用するエフェクトの数です。
        int GetUserEffectCount() const NN_NOEXCEPT
        {
            return m_UserEffectCount;
        }
        //! @internal
        //! @brief ユーザが使用するエフェクトの数を設定します。オーディオレンダラ初期化時に指定するエフェクト数に影響します。
        //! @param[in] userEffectCount ユーザが使用するエフェクトの数です。
        void SetUserEffectCount( int userEffectCount ) NN_NOEXCEPT
        {
            m_UserEffectCount = userEffectCount;
        }
        //! @internal
        //! @brief プロファイラの有効・無効を取得します。
        //! @return プロファイラが有効であれば true を返します。
        bool IsProfilerEnabled() const NN_NOEXCEPT
        {
            return m_IsProfilerEnabled;
        }
        //! @internal
        //! @brief プロファイラの有効・無効を設定します。
        //! @param[in] isProfilerEnabled プロファイラの有効・無効です。
        void SetProfilerEnabled( bool isProfilerEnabled ) NN_NOEXCEPT
        {
            m_IsProfilerEnabled = isProfilerEnabled;
        }
        //! @internal
        //! @brief オーディオレンダラで使用する最大ボイス数を取得します。
        //! @return オーディオレンダラで使用する最大ボイス数です。
        int GetVoiceCount() const NN_NOEXCEPT
        {
            return m_VoiceCount;
        }
        //! @internal
        //! @brief オーディオレンダラで使用する最大ボイス数を設定します。
        //! @param[in] voiceCount オーディオレンダラで使用する最大ボイス数です。
        void SetVoiceCount( int voiceCount ) NN_NOEXCEPT
        {
            m_VoiceCount = voiceCount;
        }
        //! @internal
        //! @brief サブミックスを追加し、追加のエフェクトバスを使用可能にする評価用機能の有効・無効を取得します。
        //! @return サブミックスを追加し、追加のエフェクトバスを使用可能にする評価用機能が有効であれば true を返します。
        bool IsAdditionalEffectBusEnabled() const NN_NOEXCEPT
        {
            return m_IsAdditionalEffectBusEnabled;
        }
        //! @internal
        //! @brief 追加のサブミックスを使用可能にする評価用機能の有効・無効を取得します。
        //! @return 追加のサブミックスを使用可能にする評価用機能が有効であれば true を返します。
        bool IsAdditionalSubMixEnabled() const NN_NOEXCEPT
        {
            return m_IsAdditionalSubMixEnabled;
        }
        //! @internal
        //! @brief エフェクトの有効・無効を取得します。
        //! @return エフェクトが有効であれば true を返します。
        bool IsEffectEnabled() const NN_NOEXCEPT
        {
            return m_IsEffectEnabled;
        }
        //! @internal
        //! @brief エフェクトの有効・無効を設定します。オーディオレンダラ初期化時に指定するエフェクト数に影響します。
        //! @param[in] isEffectEnabled エフェクトの有効・無効です。
        void SetEffectEnabled( bool isEffectEnabled ) NN_NOEXCEPT
        {
            m_IsEffectEnabled = isEffectEnabled;
        }
        //! @internal
        //! @brief DeviceOutRecorder を用いた録音の有効・無効を取得します。
        //! @return DeviceOutRecorder を用いた録音が有効であれば true を返します。
        bool IsRecordingEnabled() const NN_NOEXCEPT
        {
            return m_IsRecordingEnabled;
        }
        //! @internal
        //! @brief DeviceOutRecorder を用いた録音の有効・無効を設定します。
        //! @param[in] isRecordingEnabled DeviceOutRecorder を用いた録音の有効・無効です。
        void SetRecordingEnabled( bool isRecordingEnabled ) NN_NOEXCEPT
        {
            m_IsRecordingEnabled = isRecordingEnabled;
        }
        //! @internal
        //! @brief DeviceOutRecorder を用いた録音で一度に録音を行うオーディオフレーム数を取得します。
        //! @return DeviceOutRecorder を用いた録音で一度に録音を行うオーディオフレーム数です。
        int GetRecordingAudioFrameCount() const NN_NOEXCEPT
        {
            return m_RecordingAudioFrameCount;
        }
        //! @internal
        //! @brief DeviceOutRecorder を用いた録音で一度に録音を行うオーディオフレーム数を設定します。
        //! @param[in] recordingAudioFrameCount DeviceOutRecorder を用いた録音で一度に録音を行うオーディオフレーム数です。
        void SetRecordingAudioFrameCount( int recordingAudioFrameCount ) NN_NOEXCEPT
        {
            m_RecordingAudioFrameCount = recordingAudioFrameCount;
        }
        //! @internal
        //! @brief ユーザ用の CircularBufferSink の有効・無効を取得します。
        //! @return ユーザ用の CircularBufferSink が有効であれば true を返します。
        bool IsUserCircularBufferSinkEnabled() const NN_NOEXCEPT
        {
            return m_IsUserCircularBufferSinkEnabled;
        }
        //! @internal
        //! @brief ユーザ用の CircularBufferSink の有効・無効を設定します。
        //! @param[in] isUserCircularBufferSinkEnabled ユーザ用の CircularBufferSink の有効・無効です。
        void SetUserCircularBufferSinkEnabled( bool isUserCircularBufferSinkEnabled ) NN_NOEXCEPT
        {
            m_IsUserCircularBufferSinkEnabled = isUserCircularBufferSinkEnabled;
        }
        //! @internal
        //! @brief Hardwaremanager が用意するプリセットの SubMix の有効・無効を取得します。
        //! @return SubMix が有効であれば true を返します。
        bool IsPresetSubMixEnabled() const NN_NOEXCEPT
        {
            return m_IsPresetSubMixEnabled;
        }
        //! @internal
        //! @brief 最大で 2ch までしか使わないモードの有効・無効を取得します。
        //! @return 最大で 2ch までしか使わないモードが有効であれば true を返します。
        bool IsStereoModeEnabled() const NN_NOEXCEPT
        {
            return m_IsStereoModeEnabled;
        }
        //! @internal
        //! @brief 最大で 2ch までしか使わないモードの有効・無効を設定します。
        //! @param[in] isStereoModeEnabled 最大で 2ch までしか使わないモードの有効・無効です。
        void SetStereoModeEnabled( bool isStereoModeEnabled ) NN_NOEXCEPT
        {
            m_IsStereoModeEnabled = isStereoModeEnabled;
        }
        //! @internal
        //! @brief サウンドスレッドの有効・無効を取得します。
        //! @return サウンドスレッドが有効であれば true を返します。
        bool IsSoundThreadEnabled() const NN_NOEXCEPT
        {
            return m_IsSoundThreadEnabled;
        }
        //! @internal
        //! @brief サウンドスレッドの有効・無効を設定します。
        //! @param[in] isSoundThreadEnabled サウンドスレッドの有効・無効です。
        void SetSoundThreadEnabled( bool isSoundThreadEnabled ) NN_NOEXCEPT
        {
            m_IsSoundThreadEnabled = isSoundThreadEnabled;
        }
        //! @internal
        //! @brief ボイスドロップの有効・無効を取得します。
        //! @return ボイスドロップが有効であれば true を返します。
        bool IsVoiceDropEnabled() const NN_NOEXCEPT
        {
            return m_IsVoiceDropEnabled;
        }
        //! @internal
        //! @brief ボイスドロップの有効・無効を設定します。
        //! @param[in] isVoiceDropEnabled ボイスドロップの有効・無効です。
        void SetVoiceDropEnabled( bool isVoiceDropEnabled ) NN_NOEXCEPT
        {
            m_IsVoiceDropEnabled = isVoiceDropEnabled;
        }
        //! @brief NW4F の nw::snd 互換のダウンミックス処理の有効・無効を取得します。
        //! @return NW4F の nw::snd 互換のダウンミックス処理が有効であれば true を返します。
        bool IsCompatibleDownMixSettingEnabled() const NN_NOEXCEPT
        {
            return m_IsCompatibleDownMixSettingEnabled;
        }
        //! @brief NW4F の nw::snd 互換のダウンミックス処理の有効・無効を設定します。
        //! @param[in] isCompatibleDownMixSettingEnabled NW4F の nw::snd 互換のダウンミックス処理の有効・無効です。
        void SetCompatibleDownMixSettingEnabled( bool isCompatibleDownMixSettingEnabled ) NN_NOEXCEPT
        {
            m_IsCompatibleDownMixSettingEnabled = isCompatibleDownMixSettingEnabled;
        }
        //! @internal
        //! @brief NintendoSDK 0.16.5 以前の LPF の計算処理を行うかどうかを取得します。
        //!        true の場合、レンダラのサンプルレートではなくボイスのサンプルレートが参照されます。
        //! @return NintendoSDK 0.16.5 以前の LPF の計算処理を行う場合は true を返します。
        bool IsPreviousSdkVersionLowPassFilterCompatible() const NN_NOEXCEPT
        {
            return m_IsPreviousSdkVersionLowPassFilterCompatible;
        }
        //! @internal
        //! @brief NintendoSDK 0.16.5 以前の LPF の計算処理を行うかどうかを設定します。
        //! @param[in] isPreviousSdkVersionLowPassFilterCompatible NintendoSDK 0.16.5 以前の LPF の計算処理を行う場合は true を指定します。
        void SetPreviousSdkVersionLowPassFilterCompatible(bool isPreviousSdkVersionLowPassFilterCompatible) NN_NOEXCEPT
        {
            m_IsPreviousSdkVersionLowPassFilterCompatible = isPreviousSdkVersionLowPassFilterCompatible;
        }
        //! @internal
        //! @brief 有効でないエフェクトチャンネルをミュートする機能が有効であるかを取得します。
        //! @return 有効でないエフェクトチャンネルをミュートする機能が有効であれば true を返します。
        bool IsUnusedEffectChannelMutingEnabled() const NN_NOEXCEPT
        {
            return m_IsUnusedEffectChannelMutingEnabled;
        }
        //! @internal
        //! @brief 有効でないエフェクトチャンネルをミュートする機能の有効・無効を設定します。
        //! @param[in] isUnusedEffectChannelMutingEnabled 有効でないエフェクトチャンネルをミュートする機能の有効・無効です。
        void SetUnusedEffectChannelMutingEnabled( bool isUnusedEffectChannelMutingEnabled ) NN_NOEXCEPT
        {
            m_IsUnusedEffectChannelMutingEnabled = isUnusedEffectChannelMutingEnabled;
        }
        //! @internal
        //! @brief NW4F の nw::snd 互換のエフェクトバスの音量計算の有効・無効を取得します。
        //!        この機能が有効な場合、エフェクト未使用のバスは音量がミュートされ、エフェクトセンド量を指定しても最終出力に反映されなくなります。
        //! @return NW4F の nw::snd 互換のエフェクトバスの音量計算が有効であれば true を返します。
        bool IsCompatibleBusVolumeEnabled() const NN_NOEXCEPT
        {
            return m_IsCompatibleBusVolumeEnabled;
        }
        //! @internal
        //! @brief NW4F の nw::snd 互換のエフェクトバスの音量計算の有効・無効を設定します。
        //! @param[in] isCompatibleBusVolumeEnabled NW4F の nw::snd 互換のエフェクトバスの音量計算の有効・無効です。
        void SetCompatibleBusVolumeEnabled( bool isCompatibleBusVolumeEnabled ) NN_NOEXCEPT
        {
            m_IsCompatibleBusVolumeEnabled = isCompatibleBusVolumeEnabled;
        }
        //! @internal
        //! @brief NW4F の nw::snd 互換のパンカーブの有効・無効を取得します。
        //!        この機能が有効な場合、サラウンドモード時のパンを左右に振ったときのボリューム、およびストリームサウンド再生時のボリュームが減少します。
        //! @return NW4F の nw::snd 互換のパンカーブが有効であれば true を返します。
        bool IsCompatiblePanCurveEnabled() const NN_NOEXCEPT
        {
            return m_IsCompatiblePanCurveEnabled;
        }
        //! @internal
        //! @brief NW4F の nw::snd 互換のパンカーブの有効・無効を設定します。
        //! @param[in] isCompatiblePanCurveEnabled NW4F の nw::snd 互換のパンカーブの有効・無効です。
        void SetCompatiblePanCurveEnabled( bool isCompatiblePanCurveEnabled ) NN_NOEXCEPT
        {
            m_IsCompatiblePanCurveEnabled = isCompatiblePanCurveEnabled;
        }
        //! @internal
        //! @brief ユーザスレッドレンダリング機能の有効・無効を取得します。
        //! @return ユーザスレッドレンダリング機能が有効であれば true を返します。
        bool IsUserThreadRenderingEnabled() const NN_NOEXCEPT
        {
            return m_IsUserThreadRenderingEnabled;
        }
        //! @internal
        //! @brief ユーザスレッドレンダリング機能の有効・無効を設定します。
        //! @param[in] isUserThreadRenderingEnabled ユーザスレッドレンダリングの有効・無効です。
        void SetUserThreadRenderingEnabled( bool isUserThreadRenderingEnabled ) NN_NOEXCEPT
        {
            m_IsUserThreadRenderingEnabled = isUserThreadRenderingEnabled;
        }

        //! @internal
        //! @brief ユーザ定義のサブミックス構成を利用するかどうかを取得します。
        //! @return ユーザ定義のサブミックス構成を利用するかどうかを表わします。
        int IsCustomSubMixEnabled() const NN_NOEXCEPT
        {
            return m_IsCustomSubMixEnabled;
        }
        //! @internal
        //! @brief オーディオレンダラで使用する最大サブミックス数を取得します。
        //! @return オーディオレンダラで使用する最大サブミックス数です。
        int GetSubMixCount() const NN_NOEXCEPT
        {
            return m_SubMixCount;
        }
        //! @internal
        //! @brief オーディオレンダラで使用する最大ミックスバッファ数を取得します。
        //! @return オーディオレンダラで使用する最大ミックスバッファ数です。
        int GetMixBufferCount() const NN_NOEXCEPT
        {
            return m_MixBufferCount;
        }
        //! @internal
        //! @brief サブミックス構成に関わる設定を行います。
        //! @param[in] isStereoModeEnabled 最大で 2ch までしか使わないモードの有効・無効です。
        //! @param[in] isEffectEnabled エフェクトの有効・無効です。
        //! @param[in] isSubMixEnabled サブミックスの有効・無効です。
        //! @param[in] isAdditionalEffectBusEnabled 追加のエフェクトバスを使用可能にする評価用機能の有効・無効です。
        //! @param[in] isAdditionalSubMixEnabled 追加のサブミックスを使用可能にする評価用機能の有効・無効です。
        //! @param[in] isCustomSubMixEnabled ユーザ定義のサブミックス構成を利用するかどうかを表します。
        //! @param[in] customSubMixCount ユーザ定義のサブミックス構成をを利用する場合の最大サブミックス数です。
        //! @param[in] customMixBufferCount ユーザ定義のサブミックス構成をを利用する場合の最大ミックスバッファ数です。
        void SetSubMixParameter(bool isStereoModeEnabled, bool isEffectEnabled, bool isSubMixEnabled, bool isAdditionalEffectBusEnabled, bool isAdditionalSubMixEnabled, bool isCustomSubMixEnabled, int customSubMixCount, int customMixTotalChannelCount) NN_NOEXCEPT;

        //! @internal
        //! @brief メモリプールアタッチのチェック機能の有効・無効を取得します。
        //! @return メモリプールアタッチのチェック機能が有効であれば true を返します。
        bool IsMemoryPoolAttachCheckEnabled() const NN_NOEXCEPT
        {
            return m_IsMemoryPoolAttachCheckEnabled;
        }
        //! @internal
        //! @brief メモリプールアタッチのチェック機能の有効・無効を設定します。
        //! @param[in] isMemoryPoolAttachCheckEnabled メモリプールアタッチのチェック有効・無効です。
        void SetMemoryPoolAttachCheckEnabled( bool isMemoryPoolAttachCheckEnabled ) NN_NOEXCEPT
        {
            m_IsMemoryPoolAttachCheckEnabled = isMemoryPoolAttachCheckEnabled;
        }

    private:
        int  m_RendererSampleRate;
        int  m_UserEffectCount;
        int  m_VoiceCount;
        int  m_RecordingAudioFrameCount;
        int  m_SubMixCount;
        int  m_MixBufferCount;

        bool m_IsProfilerEnabled;
        bool m_IsAdditionalEffectBusEnabled;
        bool m_IsAdditionalSubMixEnabled;
        bool m_IsEffectEnabled;
        bool m_IsRecordingEnabled;
        bool m_IsUserCircularBufferSinkEnabled;
        bool m_IsPresetSubMixEnabled;
        bool m_IsStereoModeEnabled;
        bool m_IsSoundThreadEnabled;
        bool m_IsVoiceDropEnabled;
        bool m_IsCompatibleDownMixSettingEnabled;
        bool m_IsPreviousSdkVersionLowPassFilterCompatible;
        bool m_IsUnusedEffectChannelMutingEnabled;
        bool m_IsCompatibleBusVolumeEnabled;
        bool m_IsCompatiblePanCurveEnabled;
        bool m_IsUserThreadRenderingEnabled;
        bool m_IsCustomSubMixEnabled;
        bool m_IsMemoryPoolAttachCheckEnabled;
    };

    //! @internal
    //! @brief インスタンスが生存するスコープの間、オーディオレンダラの更新処理をロックします。
    //!        ロック期間が長くなるとサウンドスレッドの更新処理に遅延が発生するため、注意してください。
    class UpdateAudioRendererScopedLock
    {
    public:
        UpdateAudioRendererScopedLock() NN_NOEXCEPT
        {
            HardwareManager::GetInstance().LockUpdateAudioRenderer();
        }

        ~UpdateAudioRendererScopedLock() NN_NOEXCEPT
        {
            HardwareManager::GetInstance().UnlockUpdateAudioRenderer();
        }

    private:
        NN_DISALLOW_COPY( UpdateAudioRendererScopedLock );
    };

    //! @internal
    //! @brief インスタンスが生存するスコープの間、ユーザエフェクトの更新処理をロックします。
    class EffectAuxListScopedLock
    {
    public:
        EffectAuxListScopedLock() NN_NOEXCEPT;
        ~EffectAuxListScopedLock() NN_NOEXCEPT;
    private:
        NN_DISALLOW_COPY(EffectAuxListScopedLock);
    };

    //! @internal
    //! @brief インスタンスが生存するスコープの間、ユーザエフェクトの更新処理をロックします。
    class EffectAuxListForFinalMixScopedLock
    {
    public:
        EffectAuxListForFinalMixScopedLock() NN_NOEXCEPT;
        ~EffectAuxListForFinalMixScopedLock() NN_NOEXCEPT;
    private:
        NN_DISALLOW_COPY(EffectAuxListForFinalMixScopedLock);
    };

    //! @internal
    //! @brief インスタンスが生存するスコープの間、ユーザエフェクトの更新処理をロックします。
    class EffectAuxListForAdditionalSubMixScopedLock
    {
    public:
        EffectAuxListForAdditionalSubMixScopedLock() NN_NOEXCEPT;
        ~EffectAuxListForAdditionalSubMixScopedLock() NN_NOEXCEPT;
    private:
        NN_DISALLOW_COPY(EffectAuxListForAdditionalSubMixScopedLock);
    };

    //! @internal
    //! @brief インスタンスが生存するスコープの間、サブミックスリストの更新処理をロックします。
    class SubMixListScopedLock
    {
    public:
        SubMixListScopedLock() NN_NOEXCEPT;
        ~SubMixListScopedLock() NN_NOEXCEPT;
    private:
        NN_DISALLOW_COPY(SubMixListScopedLock);
    };

public:
    typedef void (*HardwareCallback)();
    struct CallbackListNode
    {
        util::IntrusiveListNode link;
        HardwareCallback callback;
    };
    typedef util::IntrusiveList<CallbackListNode, util::IntrusiveListMemberNodeTraits<CallbackListNode,&CallbackListNode::link>> CallbackList;

public:
    static const uint32_t SoundFrameIntervalMsec = 5;
    static const uint32_t SoundFrameIntervalUsec = 5 * 1000;

    // AudioRendererParameter の初期値
    static const int DefaultRendererSampleRate            = 48000; // SoundSystemParam::rendererSampleRate の初期値で使用
    static const int DefaultRendererUserEffectCount       = 10;    // TODO: 適切な初期値か要検討
    static const int DefaultRendererVoiceCountMax         = 96;
    static const int DefaultRecordingAudioFrameCount      = 7 + 1;

    static const int AtkVoiceCountMax = 192; // atk のシステムのボイス上限値、この数を越えるボイス数はシステムの都合上設定できません。

    HardwareManager() NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // 初期化
    void SetupAudioRendererParameter(nn::audio::AudioRendererParameter* pOutValue, const HardwareManagerParameter& parameter) const NN_NOEXCEPT;
    size_t GetRequiredMemSize(const HardwareManagerParameter& parameter) const NN_NOEXCEPT;
    size_t GetRequiredMemSizeForMemoryPool(int voiceCount) const NN_NOEXCEPT;
    size_t GetRequiredRecorderWorkBufferSize(const HardwareManagerParameter& parameter) const NN_NOEXCEPT;
    size_t GetRequiredCircularBufferSinkWithMemoryPoolBufferSize(const HardwareManagerParameter& parameter) const NN_NOEXCEPT;
    size_t GetRequiredCircularBufferSinkBufferSize(const HardwareManagerParameter& parameter) const NN_NOEXCEPT;
    size_t GetRequiredPerformanceFramesBufferSize(const HardwareManagerParameter& parameter) const NN_NOEXCEPT;
    nn::Result Initialize(
        void* buffer,
        size_t bufferSize,
        void* memoryPoolBuffer,
        size_t memoryPoolBufferSize,
        void* circularBufferSinkBuffer,
        size_t circularBufferSinkBufferSize,
        const HardwareManagerParameter& parameter) NN_NOEXCEPT;
    bool IsInitialized() const NN_NOEXCEPT { return m_IsInitialized != 0; }
    void Finalize() NN_NOEXCEPT;

    void Update() NN_NOEXCEPT;
    void UpdateEffect() NN_NOEXCEPT;

    void SuspendAudioRenderer() NN_NOEXCEPT;
    void ResumeAudioRenderer() NN_NOEXCEPT;
    void ExecuteAudioRendererRendering() NN_NOEXCEPT;

    // オーディオレンダラの更新処理
    nn::Result RequestUpdateAudioRenderer() NN_NOEXCEPT;
    void WaitAudioRendererEvent() NN_NOEXCEPT;
    bool TimedWaitAudioRendererEvent(nn::TimeSpan timeout) NN_NOEXCEPT;
    uint64_t GetAudioRendererUpdateCount() const NN_NOEXCEPT
    {
        return m_AudioRendererUpdateCount;
    }

    // オーディオレンダラのレンダリング時間の割合指定
    nn::Result SetAudioRendererRenderingTimeLimit(int limitPercent) NN_NOEXCEPT;
    int GetAudioRendererRenderingTimeLimit() NN_NOEXCEPT;

    static void FlushDataCache( void* address, size_t length ) NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // 出力モード
    void SetOutputMode( OutputMode mode, OutputDevice device = OutputDevice_Main ) NN_NOEXCEPT;
    OutputMode GetOutputMode( OutputDevice device = OutputDevice_Main ) const NN_NOEXCEPT
    {
        return m_OutputMode[device];
    }
    OutputMode GetEndUserOutputMode(OutputDevice device = OutputDevice_Main ) const NN_NOEXCEPT
    {
        return m_EndUserOutputMode[device];
    }
    //! @internal
    //! @brief  本体設定のサウンド出力モードを設定します。
    //!         出力モードの初期値は @ref OutputMode_Surround です。
    //!         現在の出力モードを知りたい場合は、@ref GetEndUserOutputMode を使用してください。
    //! @param[in] mode サウンドの出力モードです。
    void SetEndUserOutputMode(OutputMode mode) NN_NOEXCEPT;

    void UpdateEndUserOutputMode() NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // 出力先デバイス
    void SetOutputDeviceFlag( uint32_t outputLineIndex, uint8_t outputDeviceFlag ) NN_NOEXCEPT;
    uint8_t GetOutputDeviceFlag( uint32_t outputLineIndex ) const NN_NOEXCEPT
    {
        if ( outputLineIndex < OutputLineIndex_Max )
        {
            return m_OutputDeviceFlag[outputLineIndex];
        }

        return 0;
    }

    // ------------------------------------------------------------------------
    // エフェクト
    size_t GetRequiredEffectAuxBufferSize(const EffectAux* pEffect) const NN_NOEXCEPT;

    // リターンボリューム
    void SetAuxBusVolume( AuxBus bus, float volume, int fadeFrames, int subMixIndex ) NN_NOEXCEPT;
    float GetAuxBusVolume( AuxBus bus, int subMixIndex ) const NN_NOEXCEPT;
    void SetMainBusChannelVolumeForAdditionalEffect( float volume, int srcCh, int dstCh ) NN_NOEXCEPT;
    float GetMainBusChannelVolumeForAdditionalEffect(int srcCh, int dstCh ) const NN_NOEXCEPT;
    void SetAuxBusChannelVolumeForAdditionalEffect( AuxBus bus, float volume, int srcCh, int dstCh ) NN_NOEXCEPT;
    float GetAuxBusChannelVolumeForAdditionalEffect( AuxBus bus, int srcCh, int dstCh ) const NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    float GetOutputVolume() const NN_NOEXCEPT;

    // ------------------------------------------------------------------------
    // マスターボリューム
    void SetMasterVolume( float volume, int fadeTimes ) NN_NOEXCEPT;
    float GetMasterVolume() const NN_NOEXCEPT { return m_MasterVolume.GetValue(); }

    // ------------------------------------------------------------------------
    // SRC タイプ
    void SetSrcType( SampleRateConverterType type ) NN_NOEXCEPT;
    SampleRateConverterType GetSrcType() const NN_NOEXCEPT { return m_SrcType; }

    // ------------------------------------------------------------------------
    // Biquad フィルタ
    void SetBiquadFilterCallback( int type, const BiquadFilterCallback* cb ) NN_NOEXCEPT;
    const BiquadFilterCallback* GetBiquadFilterCallback( int type ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( type >= BiquadFilterType_DataMin && type <= BiquadFilterType_Max );
        return m_BiquadFilterCallbackTable[ type ];
    }

    // ------------------------------------------------------------------------
    // リセット前準備
    void PrepareReset() NN_NOEXCEPT;
    bool IsResetReady() const NN_NOEXCEPT;

    // === TODO: FinalMix の対応
    // void AppendFinalMixCallback( FinalMixCallback* userCallback ) NN_NOEXCEPT;
    // void PrependFinalMixCallback( FinalMixCallback* userCallback ) NN_NOEXCEPT;
    // void EraseFinalMixCallback( FinalMixCallback* userCallback ) NN_NOEXCEPT;

    // void AppendReadOnlyFinalMixCallback( ReadOnlyFinalMixCallback* userCallback ) NN_NOEXCEPT;
    // void EraseReadOnlyFinalMixCallback( ReadOnlyFinalMixCallback* userCallback ) NN_NOEXCEPT;

    nn::audio::AudioRendererConfig& GetAudioRendererConfig() NN_NOEXCEPT
    {
        return m_Config;
    }

    const nn::audio::AudioRendererParameter& GetAudioRendererParameter() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT(m_IsInitialized);
        return m_AudioRendererParameter;
    }

    FinalMix& GetFinalMix() NN_NOEXCEPT
    {
        return m_FinalMix;
    }

    typedef util::IntrusiveList< SubMix, util::IntrusiveListMemberNodeTraits<SubMix, &SubMix::m_Link> > SubMixList;

    void AddSubMix(SubMix* pSubMix) NN_NOEXCEPT;
    void RemoveSubMix(SubMix* pSubMix) NN_NOEXCEPT;

    SubMix& GetSubMix(int subMixNumber) NN_NOEXCEPT;
    const SubMix& GetSubMix(int subMixNumber) const NN_NOEXCEPT;

    int GetSubMixCount() const NN_NOEXCEPT;

    SubMix& GetAdditionalSubMix() NN_NOEXCEPT
    {
        NN_SDK_ASSERT(IsPresetSubMixEnabled(), "SubMix is not initialized\n");
        return m_AdditionalSubMix;
    }

    const SubMix& GetAdditionalSubMix() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT(IsPresetSubMixEnabled(), "SubMix is not initialized\n");
        return m_AdditionalSubMix;
    }

    nn::audio::DeviceSinkType& GetDeviceSink() NN_NOEXCEPT
    {
        NN_SDK_ASSERT(!IsUserThreadRenderingEnabled(), "DeviceSink is not initialized\n");
        return m_Sink;
    }

    const nn::audio::DeviceSinkType& GetDeviceSink() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT(!IsUserThreadRenderingEnabled(), "DeviceSink is not initialized\n");
        return m_Sink;
    }

    LowLevelVoiceAllocator& GetLowLevelVoiceAllocator() NN_NOEXCEPT
    {
        return m_LowLevelVoiceAllocator;
    }

    int GetChannelCount() const NN_NOEXCEPT;
    int GetChannelCountMax() const NN_NOEXCEPT;

    int GetAuxBusTypeCount() NN_NOEXCEPT { return AuxBus_Count; }
    int GetAuxBusTypeCountForAdditionalEffect() NN_NOEXCEPT
    {
        return AuxBusCountForAdditionalEffect;
    }
    int GetChannelCountForAdditionalEffect() NN_NOEXCEPT
    {
        return ChannelCountForAdditionalEffect;
    }

    int GetRendererSampleCount() NN_NOEXCEPT
    {
        NN_SDK_ASSERT(m_IsInitialized);
        return m_AudioRendererParameter.sampleCount;
    }

    int GetRendererSampleRate() NN_NOEXCEPT
    {
        NN_SDK_ASSERT(m_IsInitialized);
        return m_AudioRendererParameter.sampleRate;
    }

    bool IsEffectInitialized() const NN_NOEXCEPT
    {
        return m_IsInitializedEffect;
    }

    bool IsPresetSubMixEnabled() const NN_NOEXCEPT
    {
        return m_IsPresetSubMixEnabled;
    }

    bool IsAdditionalEffectEnabled() const NN_NOEXCEPT
    {
        return m_IsAdditionalEffectEnabled;
    }

    bool IsStereoModeEnabled() const NN_NOEXCEPT
    {
        return m_IsStereoModeEnabled;
    }

    bool IsSoundThreadInitialized() const NN_NOEXCEPT
    {
        return m_IsInitializedSoundThread;
    }

    bool IsPreviousSdkVersionLowPassFilterCompatible() NN_NOEXCEPT
    {
        return m_IsPreviousSdkVersionLowPassFilterCompatible;
    }

    bool IsCompatibleBusVolumeEnabled() const NN_NOEXCEPT
    {
        return m_IsCompatibleBusVolumeEnabled;
    }

    bool IsCompatiblePanCurveEnabled() const NN_NOEXCEPT
    {
        return m_IsCompatiblePanCurveEnabled;
    }

    bool IsUserThreadRenderingEnabled() const NN_NOEXCEPT
    {
        return m_IsUserThreadRenderingEnabled;
    }

    // 録音用関数
    bool RegisterRecorder(nn::atk::DeviceOutRecorder* pRecorder) NN_NOEXCEPT;
    void UnregisterRecorder(nn::atk::DeviceOutRecorder* pRecorder) NN_NOEXCEPT;
    void UpdateRecorder() NN_NOEXCEPT;
    nn::audio::CircularBufferSinkType* AllocateRecordingCircularBufferSink() NN_NOEXCEPT;
    void FreeRecordingCircularBufferSink(nn::audio::CircularBufferSinkType* pSink) NN_NOEXCEPT;
    void StartRecordingCircularBufferSink() NN_NOEXCEPT;
    size_t ReadRecordingCircularBufferSink(void * pOutBuffer, size_t bufferSize) NN_NOEXCEPT;

    // UserCircularBufferSink 関連
    nn::audio::CircularBufferSinkType& GetUserCircularBufferSink() NN_NOEXCEPT
    {
        return m_UserCircularBufferSink;
    }
    const nn::audio::CircularBufferSinkType& GetUserCircularBufferSink() const NN_NOEXCEPT
    {
        return m_UserCircularBufferSink;
    }
    size_t GetUserCircularBufferSinkBufferSize() NN_NOEXCEPT
    {
        return m_IsInitialized ? m_UserCircularBufferSinkBufferSize : 0;
    }
    CircularBufferSinkState GetUserCircularBufferSinkState() const NN_NOEXCEPT
    {
        return m_UserCircularBufferSinkState;
    }
    void StopUserCircularBufferSink() NN_NOEXCEPT;
    void StartUserCircularBufferSink(bool isForceStartMode) NN_NOEXCEPT;
    size_t ReadUserCircularBufferSink(void* pOutBuffer, size_t bufferSize) NN_NOEXCEPT;

    // SoundThread が有効でなく、手動でアタッチ、デタッチ待ちの処理が必要な場合は isSoundThreadEnabled を false にしてください
    void AttachMemoryPool( nn::audio::MemoryPoolType* pPool, void* buffer, size_t bufferSize, bool isSoundThreadEnabled ) NN_NOEXCEPT;
    void DetachMemoryPool( nn::audio::MemoryPoolType* pPool, bool isSoundThreadEnabled ) NN_NOEXCEPT;

    // こちらの関数は基本的には使用せず、UpdateAudioRendererScopedLock を使用するようにしてください。
    void LockUpdateAudioRenderer() NN_NOEXCEPT
    {
        m_UpdateAudioRendererLock.Lock();
    }
    void UnlockUpdateAudioRenderer() NN_NOEXCEPT
    {
        m_UpdateAudioRendererLock.Unlock();
    }

    // これらの関数は基本的には使用せず、ScopedLock 版を使用するようにしてください。
    void LockEffectAuxList() NN_NOEXCEPT;
    void UnlockEffectAuxList() NN_NOEXCEPT;
    void LockEffectAuxListForFinalMix() NN_NOEXCEPT;
    void UnlockEffectAuxListForFinalMix() NN_NOEXCEPT;
    void LockEffectAuxListForAdditionalSubMix() NN_NOEXCEPT;
    void UnlockEffectAuxListForAdditionalSubMix() NN_NOEXCEPT;
    void LockSubMixList() NN_NOEXCEPT;
    void UnlockSubMixList() NN_NOEXCEPT;

    int GetDroppedLowLevelVoiceCount() const NN_NOEXCEPT;
    int64_t GetElapsedAudioFrameCount() const NN_NOEXCEPT;

private:
    //! @internal
    //! @brief メンバ変数を初期状態に戻します
    void ResetParameters() NN_NOEXCEPT;
    nn::audio::MemoryPoolType::State GetMemoryPoolState( nn::audio::MemoryPoolType* pPool ) NN_NOEXCEPT;

    // === TODO: FinalMix の対応
    // typedef nw::internal::winext::AX_FINAL_MIX_CB_STRUCT AX_FINAL_MIX_CB_STRUCT;
    // typedef nw::internal::winext::AXUserFinalMixCallback AXUserFinalMixCallback;
    // typedef nw::internal::winext::AXAUXCBSTRUCT AXAUXCBSTRUCT;
    // static const uint32_t  AxMaxNumDevices = nw::internal::winext::AX_MAX_NUM_DEVICES;

    // === TODO: FinalMix の対応
    // static void MainFinalMixCallbackFunc( AX_FINAL_MIX_CB_STRUCT* info );
    // void OnMainFinalMixCallback( AX_FINAL_MIX_CB_STRUCT* info );

private:

    // static void AuxCallbackFunc( int32_t** data, void* context, AXAUXCBSTRUCT* info );
    // typedef void (*AuxCallback)( int32_t** data, void* context, AXAUXCBSTRUCT* info );

    static const int MixerCount = AuxBus_Count;
    static const int ChannelCountMax = 6;
    static const int BusCount = 1 + AuxBus_Count; // Main, AuxA, AuxB, AuxC

    // AudioRendererParameter の初期値
    static const int DefaultRendererSampleCount           = DefaultRendererSampleRate * SoundFrameIntervalMsec / 1000;
    static const int DefaultRendererMixBufferCount        = (1 + BusCount) * ChannelCountMax; // (FinalMix + SubMix[MainBus + AuxBusA,B,C]) * 6ch;
    static const int DefaultRendererSubMixCount           = 1;
    static const int DefaultRendererSinkCount             = 1;
    static const int DefaultRendererPerformanceFrameCount = 0;        // rendererPerformanceFrameCount が 0 の時 PerformanceMetrics は無効化される
    static const int DefaultRendererSystemEffectCount     = BusCount; // 各バスに DeviceOutRecorder を設定することを想定 TODO: 適切な初期値か要検討
    static const bool DefaultRendererIsVoiceDropEnabled   = false;

    // 追加エフェクトバス用の評価用機能 TODO: 正式対応時は変更可能なパラメータとして提供する
    static const int SubMixCountMax                        = 2;
    static const int SubMixCountForAdditionalEffect        = 1;
    static const int ChannelCountForAdditionalEffect       = 2;
    static const int AuxBusCountForAdditionalEffect        = 2; // AuxA, AuxB
    static const int MixBufferCountForAdditionalEffect     = (1 + AuxBusCountForAdditionalEffect) * ChannelCountForAdditionalEffect; // (MainBus + AuxBusA,B) * 2ch)

    // レンダラのパフォーマンス情報を受け取る最大のオーディオフレーム数
#if !defined(NN_ATK_CONFIG_ENABLE_VOICE_COMMAND)
    static const int MaxRendererPerformanceFrameCount = 3;
#else
    // VoiceCommand 版の場合、VoiceCommandProcess の呼び出し間隔分のオーディオフレームが必要になる
    // 20fps の場合は 1000ms / 20fps = 50ms (=10オーディオフレーム) の間隔で呼ばれるため
    // さらに余裕を考え15オーディオフレーム分のバッファを持たせておく
    static const int MaxRendererPerformanceFrameCount = 15;
#endif

    // static const BiquadFilterLpf        BIQUAD_FILTER_LPF;
    // static const BiquadFilterHpf        BIQUAD_FILTER_HPF;
    // static const BiquadFilterBpf512     BIQUAD_FILTER_BPF_512;
    // static const BiquadFilterBpf1024    BIQUAD_FILTER_BPF_1024;
    // static const BiquadFilterBpf2048    BIQUAD_FILTER_BPF_2048;

    bool                m_IsInitialized;

    nn::audio::AudioRendererHandle m_RendererHandle;
    nn::audio::AudioRendererConfig m_Config;
    nn::os::SystemEvent m_SystemEvent;
    int m_AudioRendererSuspendCount;
    std::atomic<uint64_t> m_AudioRendererUpdateCount;

    void* m_pAudioRendererWorkBuffer;
    void* m_pAudioRendererConfigWorkBuffer;

    OutputMode          m_OutputMode[OutputDevice_Count];
    OutputMode          m_EndUserOutputMode[OutputDevice_Count];   // 本体設定上での値
    SampleRateConverterType             m_SrcType;

    MoveValue<float,int>  m_MasterVolume;
    MoveValue<float,int>  m_VolumeForReset;

    // === TODO: FinalMix の対応
    // typedef util::IntrusiveList< FinalMixCallback, util::IntrusiveListMemberNodeTraits<FinalMixCallback, &FinalMixCallback::m_Link> > FinalMixCallbackList;
    // typedef util::IntrusiveList< ReadOnlyFinalMixCallback, util::IntrusiveListMemberNodeTraits<ReadOnlyFinalMixCallback, &ReadOnlyFinalMixCallback::m_Link> > ReadOnlyFinalMixCallbackList;

    // FinalMixCallbackList             m_FinalMixCallbackList;
    // ReadOnlyFinalMixCallbackList     m_ReadOnlyFinalMixCallbackList;
    // AXUserFinalMixCallback           m_LastMainFinalMixCallback;
    //nn::atk::detail::fnd::CriticalSection              m_CriticalSection;

    const BiquadFilterCallback* m_BiquadFilterCallbackTable[BiquadFilterType_Max + 1];

    // コールバック登録後、バッファがクリアになるまで待つ
    // (AUX_CALLBACK_WAIT_FRAME [=2] フレーム)
    // uint8_t m_AuxCallbackWaitCounter[ OutputDevice_Count ][ AuxBus_Count ];

    uint8_t m_OutputDeviceFlag[ OutputLineIndex_Max ];

    LowLevelVoiceAllocator m_LowLevelVoiceAllocator;

    nn::atk::FinalMix m_FinalMix;
    nn::atk::SubMix   m_SubMix[SubMixCountMax];
    nn::atk::SubMix   m_AdditionalSubMix;
    SubMixList        m_SubMixList;
    fnd::CriticalSection m_SubMixListLock;

    nn::audio::AudioRendererParameter m_AudioRendererParameter; // Initialize() が呼ばれるまでは未初期化のため、初期値が必要な場合は DefaultRenderer～ 定数を参照
    nn::audio::DeviceSinkType m_Sink;

    MoveValue<float,int>  m_AuxUserVolume[ AuxBus_Count ];

    MoveValue<float,int>  m_AuxUserVolumeForAdditionalEffect[ AuxBusCountForAdditionalEffect ];

    bool m_IsInitializedEffect;
    bool m_IsPresetSubMixEnabled;
    bool m_IsAdditionalEffectEnabled;
    bool m_IsAdditionalSubMixEnabled;
    bool m_IsStereoModeEnabled;
    bool m_IsInitializedSoundThread;
    bool m_IsPreviousSdkVersionLowPassFilterCompatible;
    bool m_IsMemoryPoolAttachCheckEnabled;

    fnd::CriticalSection m_UpdateAudioRendererLock;
    mutable fnd::CriticalSection m_UpdateHardwareManagerLock;

    fnd::CriticalSection m_EffectAuxListLock;
    fnd::CriticalSection m_EffectAuxListForFinalMixLock;
    fnd::CriticalSection m_EffectAuxListForAdditionalSubMixLock;

    // 録音用
    nn::audio::CircularBufferSinkType m_RecordingCircularBufferSink;
    CircularBufferSinkState m_RecordingCircularBufferSinkState;
    nn::audio::MemoryPoolType m_RecordingCircularBufferSinkMemoryPool;
    void* m_RecordingCircularBufferSinkBuffer;
    bool m_IsRecordingCircularBufferSinkAllocated;
    void* m_RecordingBuffer;
    size_t m_RecordingBufferSize;
    nn::atk::DeviceOutRecorder* m_pRecorder;

    // UserCircularBufferSink 関連
    nn::audio::CircularBufferSinkType m_UserCircularBufferSink;
    nn::audio::MemoryPoolType m_UserCircularBufferSinkMemoryPool; // CircularBufferSink の出力先バッファ用メモリプール
    void* m_UserCircularBufferSinkBuffer;
    size_t m_UserCircularBufferSinkBufferSize;
    CircularBufferSinkState m_UserCircularBufferSinkState;

    bool m_IsCompatibleBusVolumeEnabled;
    bool m_IsCompatiblePanCurveEnabled;
    bool m_IsUserThreadRenderingEnabled;
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

