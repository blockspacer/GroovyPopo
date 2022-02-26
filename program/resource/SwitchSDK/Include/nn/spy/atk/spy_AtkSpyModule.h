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

#include <atomic>

#include <nn/atk.h>
#include <nn/atk/atk_AddonSoundArchive.h>
#include <nn/nn_Macro.h>
#include <nn/os/os_Tick.h>
#include <nn/spy.h>
#include <nn/spy/audio/spy_AudioSpyModule.h>
#include <nn/util/util_IntrusiveDic.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/spy/atk/detail/spy_AtkSpyPacket.h>

namespace nn {
namespace spy {
namespace atk {

//! @brief nn::atk の状態を収集し、Spy.exe に送信するモジュールです。
class AtkSpyModule : public ::nn::spy::SpyModule
{
    typedef ::nn::spy::SpyModule Base;

public:
    //! @brief AtkSpyModule の初期化パラメータです。
    //!
    //! @sa AtkSpyModule::GetRequiredMemorySize
    //! @sa AtkSpyModule::Initialize
    struct InitializeArg
    {
        //! @brief オーディオパフォーマンス情報の蓄積数の最小値です。
        //!
        //! @sa audioPerformanceMetricsCount
        static const int AudioPerformanceMetricsCountMin = ::nn::atk::AudioRendererPerformanceReader::PerformanceInfoCountMin;

        //! @brief nn::atk パフォーマンス情報の蓄積数の最小値です。
        //!
        //! @sa atkProfilesCount
        static const int AtkProfilesCountMin = AudioPerformanceMetricsCountMin;

        //! @brief コンストラクタです。
        InitializeArg() NN_NOEXCEPT
            : isLogEnabled(true)
            , isSoundStateEnabled(true)
            , isPerformanceMetricsInitializationEnabled(true)
            , isPerformanceMetricsEnabled(false)
            , audioPerformanceMetricsCount(5)  // ceiling(16.0 / 5.0 + 1.0)
            , isWaveformInitializationEnabled(true)
            , isWaveformEnabled(false)
            , isAtkProfilesInitializationEnabled(true)
            , isAtkProfilesEnabled(false)
            , isSoundArchivePlayerInfoEnabled(false)
            , isSoundSystemInfoEnabled(false)
            , isSoundHeapInfoEnabled(false)
            , isStreamSoundInfoEnabled(true)
            , streamSoundInfoCountMax(6)
            , atkProfilesCount(5)  // ceiling(16.0 / 5.0 + 1.0)
            , addonSoundArchiveCountMax(0)
        {}

        //! Atk のログの送信機能を有効化するフラグです。
        //!
        //! Atk のログを送信する場合は true を設定します。
        //! (デフォルトは true に設定されます。)
        //!
        //! 初期化後に @ref AtkSpyModule::SetLogEnabled() で有効にすることもできます。
        //!
        //! @sa AtkSpyModule::SetLogEnabled
        bool isLogEnabled;

        //! サウンドの再生状態の送信機能を有効化するフラグです。
        //!
        //! サウンドの再生状態を送信する場合は true を設定します。
        //! (デフォルトは true に設定されます。)
        //!
        //! 初期化後に @ref AtkSpyModule::SetSoundStateEnabled() で有効にすることもできます。
        //!
        //! そのうえで、定期的に @ref AtkSpyModule::PushSoundState() を呼び出してサウンドの再生状態の送信を行います。
        //!
        //! @sa AtkSpyModule::SetSoundStateEnabled
        //! @sa AtkSpyModule::PushSoundState
        bool isSoundStateEnabled;

        //! パフォーマンス情報の送信機能を初期化するフラグです。
        //!
        //! パフォーマンス情報の送信機能を初期化する場合は true を設定します。
        //! (デフォルトは true に設定されます。)
        //!
        //! この設定で初期化を行わなかった場合は、パフォーマンス情報の送信はできません。
        //!
        //! @sa isPerformanceMetricsEnabled
        //! @sa audioPerformanceMetricsCount
        //! @sa AtkSpyModule::IsPerformanceMetricsInitialized
        bool isPerformanceMetricsInitializationEnabled;

        //! パフォーマンス情報の送信機能を有効化するフラグです。
        //!
        //! パフォーマンス情報を送信する場合は true を設定します。
        //! (デフォルトは false に設定されます。)
        //!
        //! この設定を true にする場合は @ref isPerformanceMetricsInitializationEnabled も true である必要があります。
        //!
        //! 初期化後に @ref AtkSpyModule::SetPerformanceMetricsEnabled() で有効にすることもできます。
        //!
        //! そのうえで、定期的に @ref AtkSpyModule::PushPerformanceMetrics() を呼び出してパフォーマンス情報の送信を行います。
        //!
        //! @sa isPerformanceMetricsInitializationEnabled
        //! @sa AtkSpyModule::SetPerformanceMetricsEnabled
        //! @sa AtkSpyModule::PushPerformanceMetrics
        bool isPerformanceMetricsEnabled;

        //! nn::audio のパフォーマンス情報の計測結果を蓄積するバッファのエントリ数です。
        //!
        //! (デフォルトは 5 に設定されます。)
        //!
        //! nn::atk のサウンドスレッドが 1 オーディオフレーム (= 5ms) ごとにパフォーマンス情報を 1 回取得し、バッファに蓄積します。
        //!
        //! @ref AtkSpyModule::PushPerformanceMetrics() の呼び出しの周期を T ms としたとき、
        //! audioPerformanceMetricsCount の最小値は次の式で求めることができます。
        //! @code
        //!  (audioPerformanceMetricsCount の最小値) = max( AudioPerformanceMetricsCountMin, (T / 5) + 1 );
        //! @endcode
        //! フレーム落ちなどを考慮する場合はこれよりも大きな値を設定してください。
        //!
        //! @sa isPerformanceMetricsInitializationEnabled
        //! @sa AudioPerformanceMetricsCountMin
        //! @sa AtkSpyModule::PushPerformanceMetrics
        int audioPerformanceMetricsCount;

        //! 波形データの送信機能を初期化するフラグです。
        //!
        //! 波形データの送信機能を初期化する場合は true を設定します。
        //! (デフォルトは true に設定されます。)
        //!
        //! この設定で初期化を行わなかった場合は、波形データの送信はできません。
        //!
        //! @sa isWaveformEnabled
        //! @sa AtkSpyModule::IsWaveformInitialized
        bool isWaveformInitializationEnabled;

        //! 波形データの送信機能を有効化するフラグです。
        //!
        //! 波形データを送信する場合は true を設定します。
        //! (デフォルトは false に設定されます。)
        //!
        //! この設定を true にする場合は @ref isWaveformInitializationEnabled も true である必要があります。
        //!
        //! 初期化後に @ref AtkSpyModule::SetWaveformEnabled() で有効にすることもできます。
        //!
        //! そのうえで、定期的に @ref AtkSpyModule::PushWaveform() を呼び出して波形データの送信を行います。
        //!
        //! @sa isWaveformInitializationEnabled
        //! @sa AtkSpyModule::SetWaveformEnabled
        //! @sa AtkSpyModule::PusWaveform
        bool isWaveformEnabled;

        //! nn::atk パフォーマンス情報の送信機能を初期化するフラグです。
        //!
        //! この機能を初期化する場合は true を設定します。
        //! (デフォルトは true に設定されます。)
        //!
        //! この設定で初期化を行わなかった場合は、nn::atk パフォーマンス情報を送信できません。
        //!
        //! @sa atkProfilesCount
        //! @sa AtkSpyModule::IsAtkProfilesInitialized
        bool isAtkProfilesInitializationEnabled;

        //! nn::atk パフォーマンス情報の送信機能を有効化するフラグです。
        //!
        //! nn::atk パフォーマンス情報を送信する場合は true を設定します。
        //! (デフォルトは false に設定されます。)
        //!
        //! この設定を true にする場合は @ref isAtkProfilesInitializationEnabled も true である必要があります。
        //!
        //! 初期化後に @ref AtkSpyModule::SetAtkProfilesEnabled() で有効にすることもできます。
        //!
        //! そのうえで、定期的に @ref AtkSpyModule::PushAtkProfiles() を呼び出してnn::atk パフォーマンス情報の送信を行います。
        //!
        //! @sa isAtkProfilesInitializationEnabled
        //! @sa AtkSpyModule::SetAtkProfilesEnabled
        //! @sa AtkSpyModule::PusAtkProfiles
        bool isAtkProfilesEnabled;

        //! @briefprivate
        //! nn::atk サウンドアーカイブプレイヤー情報の送信機能を有効化するフラグです。
        //!
        //! nn::atk サウンドアーカイブプレイヤー情報を送信する場合は true を設定します。
        //! (デフォルトは false に設定されます。)
        //!
        //! 初期化後に @ref AtkSpyModule::SetSoundArchivePlayerInfoEnabled() で有効にすることもできます。
        //!
        //! そのうえで、定期的に @ref AtkSpyModule::PushSoundArchivePlayerInfo() を呼び出してnn::atk サウンドアーカイブプレイヤー情報の送信を行います。
        //!
        //! @sa AtkSpyModule::SetSoundArchivePlayerInfoEnabled
        //! @sa AtkSpyModule::PushSoundArchivePlayerInfo
        bool isSoundArchivePlayerInfoEnabled;

        //! @briefprivate
        //! nn::atk サウンドシステム情報の送信機能を有効化するフラグです。
        //!
        //! nn::atk サウンドシステム情報を送信する場合は true を設定します。
        //! (デフォルトは false に設定されます。)
        //!
        //! 初期化後に @ref AtkSpyModule::SetSoundSystemInfoEnabled() で有効にすることもできます。
        //!
        //! そのうえで、定期的に @ref AtkSpyModule::PushSoundSystemInfo() を呼び出してnn::atk サウンドシステム情報の送信を行います。
        //!
        //! @sa AtkSpyModule::SetSoundSystemInfoEnabled
        //! @sa AtkSpyModule::PushSoundSystemInfo
        bool isSoundSystemInfoEnabled;

        //! @briefprivate
        //! nn::atk サウンドヒープ情報の送信機能を有効化するフラグです。
        //!
        //! nn::atk サウンドヒープ情報を送信する場合は true を設定します。
        //! (デフォルトは false に設定されます。)
        //!
        //! 初期化後に @ref AtkSpyModule::SetSoundHeapInfoEnabled() で有効にすることもできます。
        //!
        //! そのうえで、定期的に @ref AtkSpyModule::PushSoundHeapInfo() を呼び出してnn::atk サウンドヒープ情報の送信を行います。
        //!
        //! @sa AtkSpyModule::SetSoundHeapInfoEnabled
        //! @sa AtkSpyModule::PushSoundHeapInfo
        bool isSoundHeapInfoEnabled;

        //! nn::atk ストリームサウンド情報の送信機能を有効化するフラグです。
        //!
        //! nn::atk ストリームサウンド情報を送信する場合は true を設定します。
        //! (デフォルトは true に設定されます。)
        //!
        //! 初期化後に @ref AtkSpyModule::SetStreamSoundInfoEnabled() で有効にすることもできます。
        //!
        //! そのうえで、定期的に @ref AtkSpyModule::PushStreamSoundInfo() を呼び出して nn::atk ストリームサウンド情報の送信を行います。
        //!
        //! @sa AtkSpyModule::SetStreamSoundInfoEnabled
        //! @sa AtkSpyModule::PushStreamSoundInfo
        bool isStreamSoundInfoEnabled;

        //! 表示する nn::atk ストリームサウンド情報の最大数です。
        //! ストリームサウンドを利用する数に応じて、こちらの値を増減させてください。
        //!
        //! (デフォルトは 6 に設定されます。)
        //!
        //! @sa AtkSpyModule::SetStreamSoundInfoEnabled
        //! @sa AtkSpyModule::PushStreamSoundInfo
        int streamSoundInfoCountMax;

        //! nn::atk パフォーマンス情報を蓄積するエントリ数です。
        //!
        //! (デフォルトは 5 に設定されます。)
        //!
        //! nn::atk のサウンドスレッドが 1 オーディオフレーム (= 5ms) ごとにパフォーマンス情報を 1 回取得し、バッファに蓄積します。
        //!
        //! @ref AtkSpyModule::PushAtkProfiles() の呼び出しの周期を T ms としたとき、
        //! atkProfilesCount の最小値は次の式で求めることができます。
        //! @code
        //!  (atkProfilesCount の最小値) = max( AtkProfilesCountMin, (T / 5) + 1 );
        //! @endcode
        //! フレーム落ちなどを考慮する場合はこれよりも大きな値を設定してください。
        //!
        //! @sa isAtkProfilesInitializationEnabled
        //! @sa AtkProfilesCountMin
        //! @sa AtkSpyModule::PushPerformanceMetrics
        int atkProfilesCount;

        //! 追加サウンドアーカイブの数の最大値です。
        //!
        //! (デフォルトは 0 に設定されます。)
        //!
        //! nn::atk::SoundArchivePlayer に登録する追加サウンドアーカイブの数以上になるように設定します。
        //!
        //! @sa AtkSpyModule::PushSoundState
        int addonSoundArchiveCountMax;
    };

#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

public:
    NN_FORCEINLINE static size_t GetRequiredMemorySize(const InitializeArg& args) NN_NOEXCEPT
    {
        NN_UNUSED(args);
        return 0;
    }

    NN_FORCEINLINE AtkSpyModule() NN_NOEXCEPT
    {
    }

    NN_FORCEINLINE ~AtkSpyModule() NN_NOEXCEPT
    {
    }

    NN_FORCEINLINE bool Initialize(const InitializeArg& args, void* buffer, size_t bufferSize) NN_NOEXCEPT
    {
        NN_UNUSED(args);
        NN_UNUSED(buffer);
        NN_UNUSED(bufferSize);
        return true;
    }

    NN_FORCEINLINE void Finalize() NN_NOEXCEPT
    {}

    NN_FORCEINLINE bool IsInitialized() const NN_NOEXCEPT
    {
        return true;
    }

    NN_FORCEINLINE bool IsPerformanceMetricsInitialized() const NN_NOEXCEPT
    {
        return true;
    }

    NN_FORCEINLINE bool IsWaveformInitialized() const NN_NOEXCEPT
    {
        return true;
    }

    NN_FORCEINLINE bool IsAtkProfilesInitialized() const NN_NOEXCEPT
    {
        return true;
    }

    NN_FORCEINLINE bool IsLogEnabled() const NN_NOEXCEPT
    {
        return false;
    }

    NN_FORCEINLINE void SetLogEnabled(bool value) NN_NOEXCEPT
    {
        NN_UNUSED(value);
    }

    NN_FORCEINLINE bool IsSoundStateEnabled() const NN_NOEXCEPT
    {
        return false;
    }

    NN_FORCEINLINE void SetSoundStateEnabled(bool value) NN_NOEXCEPT
    {
        NN_UNUSED(value);
    }

    NN_FORCEINLINE bool IsPerformanceMetricsEnabled() const NN_NOEXCEPT
    {
        return false;
    }

    NN_FORCEINLINE void SetPerformanceMetricsEnabled(bool value) NN_NOEXCEPT
    {
        NN_UNUSED(value);
    }

    NN_FORCEINLINE bool IsWaveformEnabled() const NN_NOEXCEPT
    {
        return false;
    }

    NN_FORCEINLINE void SetWaveformEnabled(bool value) NN_NOEXCEPT
    {
        NN_UNUSED(value);
    }

    NN_FORCEINLINE bool IsAtkProfilesEnabled() const NN_NOEXCEPT
    {
        return false;
    }

    NN_FORCEINLINE void SetAtkProfilesEnabled(bool value) NN_NOEXCEPT
    {
        NN_UNUSED(value);
    }

    NN_FORCEINLINE bool IsSoundArchivePlayerInfoEnabled() const NN_NOEXCEPT
    {
        return false;
    }

    NN_FORCEINLINE void SetSoundArchivePlayerInfoEnabled(bool value) NN_NOEXCEPT
    {
        NN_UNUSED(value);
    }

    NN_FORCEINLINE bool IsSoundSystemInfoEnabled() const NN_NOEXCEPT
    {
        return false;
    }

    NN_FORCEINLINE void SetSoundSystemInfoEnabled(bool value) NN_NOEXCEPT
    {
        NN_UNUSED(value);
    }

    NN_FORCEINLINE bool IsSoundHeapInfoEnabled() const NN_NOEXCEPT
    {
        return false;
    }

    NN_FORCEINLINE void SetSoundHeapInfoEnabled(bool value) NN_NOEXCEPT
    {
        NN_UNUSED(value);
    }

    NN_FORCEINLINE bool IsStreamSoundInfoEnabled() const NN_NOEXCEPT
    {
        return false;
    }

    NN_FORCEINLINE void SetStreamSoundInfoEnabled(bool value) NN_NOEXCEPT
    {
        NN_UNUSED(value);
    }


    NN_FORCEINLINE bool PushSoundState(::nn::atk::SoundArchivePlayer& soundArchivePlayer) NN_NOEXCEPT
    {
        NN_UNUSED(soundArchivePlayer);
        return false;
    }

    NN_FORCEINLINE bool PushPerformanceMetrics() NN_NOEXCEPT
    {
        return false;
    }

    NN_FORCEINLINE bool PushWaveform(
        const void* buffer,
        size_t bufferSize,
        nn::os::Tick tick) NN_NOEXCEPT
    {
        NN_UNUSED(buffer);
        NN_UNUSED(bufferSize);
        NN_UNUSED(tick);
        return false;
    }

    NN_FORCEINLINE bool PushAtkProfiles() NN_NOEXCEPT
    {
        return false;
    }

    NN_FORCEINLINE bool PushAtkProfiles(::nn::atk::SoundArchivePlayer& soundArchivePlayer) NN_NOEXCEPT
    {
        NN_UNUSED(soundArchivePlayer);
        return false;
    }

    NN_FORCEINLINE bool PushSoundArchivePlayerInfo(::nn::atk::SoundArchivePlayer& soundArchivePlayer) NN_NOEXCEPT
    {
        NN_UNUSED(soundArchivePlayer);
        return false;
    }

    NN_FORCEINLINE bool PushSequenceVariable(::nn::atk::SoundArchivePlayer& soundArchivePlayer) NN_NOEXCEPT
    {
        NN_UNUSED(soundArchivePlayer);
        return false;
    }

    NN_FORCEINLINE bool PushSoundSystemInfo() NN_NOEXCEPT
    {
        return false;
    }

    NN_FORCEINLINE bool PushSoundHeapInfo(::nn::atk::SoundHeap& soundHeap) NN_NOEXCEPT
    {
        NN_UNUSED(soundHeap);
        return false;
    }

    NN_FORCEINLINE bool PushStreamSoundInfo(::nn::atk::SoundArchivePlayer& soundArchivePlayer) NN_NOEXCEPT
    {
        NN_UNUSED(soundArchivePlayer);
        return false;
    }

#else // NN_BUILD_CONFIG_SPY_ENABLED

private:
    class AddonSoundArchiveInfo
    {
    public:
        AddonSoundArchiveInfo()
            : m_pAddonSoundArchive(nullptr)
            , m_IsPushed(false)
        {
        }

        ::nn::util::IntrusiveDicNode m_Dic;
        ::nn::util::IntrusiveListNode m_Link;
        ::nn::os::Tick m_AddTick;
        const ::nn::atk::AddonSoundArchive* m_pAddonSoundArchive;
        detail::AddonSoundArchiveDataPacket m_addonSoundArchiveDataPacket;
        bool m_IsPushed;
    };

    class  AddonSoundArchiveInfoDic : public ::nn::util::IntrusiveDic<AddonSoundArchiveInfo, ::nn::util::IntrusiveDicMemberNodeTraits<AddonSoundArchiveInfo, &AddonSoundArchiveInfo::m_Dic>>
    {
    public:
        AddonSoundArchiveInfo& RemoveSingle() NN_NOEXCEPT;
    };

    class AddonSoundArchiveInfoList : public ::nn::util::IntrusiveList<AddonSoundArchiveInfo, ::nn::util::IntrusiveListMemberNodeTraits<AddonSoundArchiveInfo, &AddonSoundArchiveInfo::m_Link>>
    {
    public:
        void Push(AddonSoundArchiveInfo& info) NN_NOEXCEPT;
        AddonSoundArchiveInfo& Pop() NN_NOEXCEPT;
    };

public:
    //! @brief 初期化に必要なメモリサイズを取得します。
    //!
    //! @param[in] args 初期化パラメータです。
    //!
    //! @return 初期化に必要なメモリサイズを返します。
    //!
    //! @pre
    //! - @ref nn::atk::SoundSystem::IsInitialized() == true
    //! - args.isPerformanceMetricsEnabled == true のときは args.isPerformanceMetricsInitializationEnabled == true
    //! - args.audioPerformanceMetricsCount >= InitializeArg::AudioPerformanceMetricsCountMin
    //! - args.isAtkProfilesEnabled == true のときは args.isAtkProfilesInitializationEnabled == true
    //! - args.atkProfilesCount >= InitializeArg::AtkProfilesCountMin
    //!
    //! @sa Initialize
    //! @sa InitializeArg
    static size_t GetRequiredMemorySize(const InitializeArg& args) NN_NOEXCEPT;

    //! @brief コンストラクタです。
    AtkSpyModule() NN_NOEXCEPT;

    //! @brief デストラクタです。
    ~AtkSpyModule() NN_NOEXCEPT;

    //! @brief 初期化を行います。
    //!
    //! @details
    //! AtkSpyModule が使用するワークメモリをユーザが渡します。
    //! @ref InitializeArg 構造体に初期化に必要なパラメータを設定し、
    //! @ref GetRequiredMemorySize() で必要なメモリサイズを取得してください。
    //!
    //! @param[in] args 初期化パラメータです。
    //! @param[in] buffer AtkSpyModule が使用するワークメモリへのポインタです。
    //! @param[in] bufferSize ワークメモリのサイズです。
    //!
    //! @return 初期化に成功した場合は true を返します。
    //!
    //! @pre
    //! - buffer は nn::DefaultAlignment にアラインしている
    //! - bufferSize >= @ref GetRequiredMemorySize()
    //! - @ref nn::atk::SoundSystem::IsInitialized() == true
    //! - @ref IsInitialized() == false
    //! - @ref IsRegistered() == false
    //! - args.isPerformanceMetricsEnabled == true のときは args.isPerformanceMetricsInitializationEnabled == true
    //! - args.audioPerformanceMetricsInfoCount >= InitializeArg::AudioPerformanceMetricsCountMin
    //! - args.isWaveformEnabled == true のときは args.isWaveformInitializationEnabled == true
    //! - args.isWaveformInitializationEnabled == true の時は @ref nn::atk::SoundSystem が @ref nn::atk::SoundSystem::SoundSystemParam::enableCircularBufferSink == true で初期化されている
    //!
    //! @sa GetRequiredMemorySize
    //! @sa Finalize
    //! @sa IsInitialized
    bool Initialize(const InitializeArg& args, void* buffer, size_t bufferSize) NN_NOEXCEPT;

    //! @brief 終了処理を行います。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //! - @ref IsRegistered() == false
    //!
    //! @sa Initialize
    void Finalize() NN_NOEXCEPT;

    //! @brief 初期化されているかどうかを取得します。
    //!
    //! @return 初期化が完了していれば true を返します。
    //!
    //! @sa Initialize
    bool IsInitialized() const NN_NOEXCEPT;

    //! @brief パフォーマンス計測のための初期化が行われているかを取得します。
    //!
    //! @details
    //! @ref InitializeArg::isPerformanceMetricsInitializationEnabled が true に指定されて
    //! @ref Initialize() が実行されたかを取得します。
    //!
    //! 初期化でこの指定が行われていない場合にはパフォーマンス情報を送信することはできません。
    //!
    //! @return パフォーマンス計測のための初期化が行われていれば true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa InitializeArg
    //! @sa SetPerformanceMetricsEnabled
    bool IsPerformanceMetricsInitialized() const NN_NOEXCEPT;

    //! @brief 波形データのための初期化が行われているかを取得します。
    //!
    //! @details
    //! @ref InitializeArg::isWaveformInitializationEnabled が true に指定されて
    //! @ref Initialize() が実行されたかを取得します。
    //!
    //! 初期化でこの指定が行われていない場合には波形データを送信することはできません。
    //!
    //! @return 波形データのための初期化が行われていれば true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa InitializeArg
    //! @sa SetWaveformEnabled
    bool IsWaveformInitialized() const NN_NOEXCEPT;

    //! @brief nn::atk パフォーマンス情報のための初期化が行われているかを取得します。
    //!
    //! @details
    //! @ref InitializeArg::isAtkProfilesInitializationEnabled が true に指定されて
    //! @ref Initialize() が実行されたかを取得します。
    //!
    //! 初期化でこの指定が行われていない場合には nn::atk パフォーマンス情報を送信することはできません。
    //!
    //! @return nn::atk パフォーマンス情報のための初期化が行われていれば true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa InitializeArg
    //! @sa SetAtkProfilesEnabled
    bool IsAtkProfilesInitialized() const NN_NOEXCEPT;

    //! @brief ログの送信が有効にされているかを取得します。
    //!
    //! @details
    //! @ref Initialize() の初期化パラメータで、または @ref SetLogEnabled() の呼び出しにより
    //! ログの送信が有効にされているかを取得します。
    //!
    //! @return ログの送信が有効にされていれば true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa SetLogEnabled
    bool IsLogEnabled() const NN_NOEXCEPT;

    //! @brief ログの送信の有効・無効を設定します。
    //!
    //! @details
    //! Atk ライブラリ実行中に出力されるデバッグログを送信することができます。
    //!
    //! Initialize() または本関数により有効・無効を設定することができ、
    //! アプリケーションの実行中の任意のタイミングで有効・無効を変更できます。
    //!
    //! @param[in] value ログの送信を有効にする場合は true を、無効にする場合には false を指定します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa IsLogEnabled
    void SetLogEnabled(bool value) NN_NOEXCEPT;

    //! @brief サウンド再生状態の送信が有効にされているかを取得します。
    //!
    //! @details
    //! @ref Initialize() の初期化パラメータで、または @ref SetSoundStateEnabled() の呼び出しにより
    //! サウンド再生状態の送信が有効にされているかを取得します。
    //!
    //! @return サウンド再生状態の送信が有効にされていれば true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa SetSoundStateEnabled
    bool IsSoundStateEnabled() const NN_NOEXCEPT;

    //! @brief サウンド再生状態の送信の有効・無効を設定します。
    //!
    //! @details
    //! Initialize() または本関数により有効・無効を設定することができ、
    //! アプリケーションの実行中の任意のタイミングで有効・無効を変更できます。
    //!
    //! データを送信するには別途、定期的な @ref PushSoundState() の呼び出しが必要です。
    //!
    //! @param[in] value サウンド再生状態の送信を有効にする場合は true を指定します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa IsSoundStateEnabled
    //! @sa PushSoundState
    void SetSoundStateEnabled(bool value) NN_NOEXCEPT;

    //! @brief パフォーマンス情報の送信が有効に設定されているかを取得します。
    //!
    //! @details
    //! @ref Initialize() の初期化パラメータで、または @ref SetPerformanceMetricsEnabled() の呼び出しにより
    //! パフォーマンス情報の送信が有効に設定されているかを取得します。
    //!
    //! @return パフォーマンス情報の送信が有効に設定されていれば true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa SetPerformanceMetricsEnabled
    bool IsPerformanceMetricsEnabled() const NN_NOEXCEPT;

    //! @brief パフォーマンス情報の送信の有効・無効を設定します。
    //!
    //! @details
    //! Initialize() または本関数により有効・無効を設定することができ、
    //! アプリケーションの実行中の任意のタイミングで有効・無効を変更できます。
    //!
    //! Initialize() の 初期化パラメータで
    //! @ref InitializeArg::isPerformanceMetricsInitializationEnabled が true に指定されていなければなりません。
    //!
    //! データを送信するには別途、定期的な @ref PushPerformanceMetrics() の呼び出しが必要です。
    //!
    //! @param[in] value パフォーマンス情報の送信を有効にする場合は true を指定します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //! - @ref IsPerformanceMetricsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa IsPerformanceMetricsEnabled
    //! @sa IsPerformanceMetricsInitialized
    //! @sa PushPerformanceMetrics
    void SetPerformanceMetricsEnabled(bool value) NN_NOEXCEPT;

    //! @brief 波形データの送信が有効に設定されているかを取得します。
    //!
    //! @details
    //! @ref Initialize() の初期化パラメータで、または @ref SetWaveformEnabled() の呼び出しにより
    //! 波形データの送信が有効に設定されているかを取得します。
    //!
    //! @return 波形データの送信が有効に設定されていれば true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa SetPerformanceMetricsEnabled
    bool IsWaveformEnabled() const NN_NOEXCEPT;

    //! @brief 波形データの送信の有効・無効を設定します。
    //!
    //! @details
    //! Initialize() または本関数により有効・無効を設定することができ、
    //! アプリケーションの実行中の任意のタイミングで有効・無効を変更できます。
    //!
    //! Initialize() の 初期化パラメータで
    //! @ref InitializeArg::isWaveformInitializationEnabled が true に指定されていなければなりません。
    //!
    //! データを送信するには別途、定期的な @ref PushWaveform() の呼び出しが必要です。
    //!
    //! @param[in] value 波形データの送信を有効にする場合は true を指定します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //! - @ref IsWaveformInitialized() == true
    //!
    //! @sa Initialize
    //! @sa IsWaveformEnabled
    //! @sa IsWaveformInitialized
    //! @sa PushWaveform
    void SetWaveformEnabled(bool value) NN_NOEXCEPT;

    //! @brief nn::atk パフォーマンス情報の送信が有効に設定されているかを取得します。
    //!
    //! @details
    //! @ref Initialize() の初期化パラメータで、または @ref SetAtkProfilesEnabled() の呼び出しにより
    //! nn::atk パフォーマンス情報の送信が有効に設定されているかを取得します。
    //!
    //! @return nn::atk パフォーマンス情報の送信が有効に設定されていれば true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa SetAtkProfilesEnabled
    bool IsAtkProfilesEnabled() const NN_NOEXCEPT;

    //! @brief nn::atk パフォーマンス情報の送信の有効・無効を設定します。
    //!
    //! @details
    //! Initialize() または本関数により有効・無効を設定することができ、
    //! アプリケーションの実行中の任意のタイミングで有効・無効を変更できます。
    //!
    //! Initialize() の 初期化パラメータで
    //! @ref InitializeArg::isAtkProfilesInitializationEnabled が true に指定されていなければなりません。
    //!
    //! データを送信するには別途、定期的な @ref PushAtkProfiles() の呼び出しが必要です。
    //!
    //! @param[in] value nn::atk パフォーマンス情報の送信を有効にする場合は true を指定します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //! - @ref IsAtkProfilesInitialized() == true
    //!
    //! @sa Initialize
    //! @sa IsAtkProfilesEnabled
    //! @sa IsAtkProfilesInitialized
    //! @sa PushAtkProfiles
    void SetAtkProfilesEnabled(bool value) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief nn::atk サウンドアーカイブプレイヤー情報の送信が有効に設定されているかを取得します。
    //!
    //! @details
    //! @ref Initialize() の初期化パラメータで、または @ref SetSoundArchivePlayerInfoEnabled() の呼び出しにより
    //! nn::atk サウンドアーカイブプレイヤー情報の送信が有効に設定されているかを取得します。
    //!
    //! @return nn::atk サウンドアーカイブプレイヤー情報の送信が有効に設定されていれば true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa SetSoundArchivePlayerInfoEnabled
    bool IsSoundArchivePlayerInfoEnabled() const NN_NOEXCEPT;

    //! @briefprivate
    //! @brief nn::atk サウンドアーカイブプレイヤー情報の送信の有効・無効を設定します。
    //!
    //! @details
    //! Initialize() または本関数により有効・無効を設定することができ、
    //! アプリケーションの実行中の任意のタイミングで有効・無効を変更できます。
    //!
    //! データを送信するには別途、定期的な @ref PushSoundArchivePlayerInfo() の呼び出しが必要です。
    //!
    //! @param[in] value nn::atk サウンドアーカイブプレイヤー情報の送信を有効にする場合は true を指定します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa IsSoundArchivePlayerInfoEnabled
    //! @sa PushSoundArchivePlayerInfo
    void SetSoundArchivePlayerInfoEnabled(bool value) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief nn::atk サウンドシステム情報の送信が有効に設定されているかを取得します。
    //!
    //! @details
    //! @ref Initialize() の初期化パラメータで、または @ref SetSoundSystemInfoEnabled() の呼び出しにより
    //! nn::atk サウンドシステム情報の送信が有効に設定されているかを取得します。
    //!
    //! @return nn::atk サウンドシステム情報の送信が有効に設定されていれば true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa SetSoundSystemInfoEnabled
    bool IsSoundSystemInfoEnabled() const NN_NOEXCEPT;

    //! @briefprivate
    //! @brief nn::atk サウンドシステム情報の送信の有効・無効を設定します。
    //!
    //! @details
    //! Initialize() または本関数により有効・無効を設定することができ、
    //! アプリケーションの実行中の任意のタイミングで有効・無効を変更できます。
    //!
    //! データを送信するには別途、定期的な @ref PushSoundSystemInfo() の呼び出しが必要です。
    //!
    //! @param[in] value nn::atk サウンドシステム情報の送信を有効にする場合は true を指定します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa IsSoundSystemInfoEnabled
    //! @sa PushSoundSystemInfo
    void SetSoundSystemInfoEnabled(bool value) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief nn::atk サウンドヒープ情報の送信が有効に設定されているかを取得します。
    //!
    //! @details
    //! @ref Initialize() の初期化パラメータで、または @ref SetSoundHeapInfoEnabled() の呼び出しにより
    //! nn::atk サウンドヒープ情報の送信が有効に設定されているかを取得します。
    //!
    //! @return nn::atk サウンドヒープ情報の送信が有効に設定されていれば true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa SetSoundHeapInfoEnabled
    bool IsSoundHeapInfoEnabled() const NN_NOEXCEPT;

    //! @briefprivate
    //! @brief nn::atk サウンドヒープ情報の送信の有効・無効を設定します。
    //!
    //! @details
    //! Initialize() または本関数により有効・無効を設定することができ、
    //! アプリケーションの実行中の任意のタイミングで有効・無効を変更できます。
    //!
    //! データを送信するには別途、定期的な @ref PushSoundHeapInfo() の呼び出しが必要です。
    //!
    //! @param[in] value nn::atk サウンドヒープ情報の送信を有効にする場合は true を指定します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa IsSoundHeapInfoEnabled
    //! @sa PushSoundHeapInfo
    void SetSoundHeapInfoEnabled(bool value) NN_NOEXCEPT;

    //! @brief nn::atk ストリームサウンド情報の送信が有効に設定されているかを取得します。
    //!
    //! @details
    //! @ref Initialize() の初期化パラメータで、または @ref SetStreamSoundInfoEnabled() の呼び出しにより
    //! nn::atk ストリームサウンド情報の送信が有効に設定されているかを取得します。
    //!
    //! @return nn::atk ストリームサウンド情報の送信が有効に設定されていれば true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa SetStreamSoundInfoEnabled
    bool IsStreamSoundInfoEnabled() const NN_NOEXCEPT;

    //! @brief nn::atk ストリームサウンド情報の送信の有効・無効を設定します。
    //!
    //! @details
    //! Initialize() または本関数により有効・無効を設定することができ、
    //! アプリケーションの実行中の任意のタイミングで有効・無効を変更できます。
    //!
    //! データを送信するには別途、定期的な @ref PushStreamSoundInfo() の呼び出しが必要です。
    //!
    //! @param[in] value nn::atk ストリームサウンド情報の送信を有効にする場合は true を指定します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa IsStreamSoundInfoEnabled
    //! @sa PushStreamSoundInfo
    void SetStreamSoundInfoEnabled(bool value) NN_NOEXCEPT;

    //! @brief サウンド再生状態を送信します。
    //!
    //! @details
    //! @ref IsSoundStateEnabled() == false のときは何もしません。
    //!
    //! @param[in] soundArchivePlayer サウンドアーカイブプレイヤーを指定します。
    //!
    //! @return 送信が正常に行われたときは true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //! - soundArchivePlayer.IsAvailable() == true
    //!
    //! @sa Initialize
    //! @sa SetSoundStateEnabled
    //! @sa nn::atk::SoundArchivePlayer::IsAvailable
    bool PushSoundState(::nn::atk::SoundArchivePlayer& soundArchivePlayer) NN_NOEXCEPT;

    //! @brief パフォーマンス情報を送信します。
    //!
    //! @details
    //! @ref IsPerformanceMetricsEnabled() == false のときは何もしません。
    //!
    //! @return 送信が正常に行われたときは true を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //! - @ref IsPerformanceMetricsInitialized() == true
    //!
    //! @sa Initialize
    //! @sa SetPerformanceMetricsEnabled
    bool PushPerformanceMetrics() NN_NOEXCEPT;

    //! @brief 波形データを送信します。
    //!
    //! @details
    //! @ref nn::atk::SoundSystem::ReadCircularBufferSink() で取得した波形データを送信します。
    //!
    //! nn::atk は @ref nn::audio::RequestUpdateAudioRenderer() の
    //! 完了時刻を提供しないので、代わりに、引数 tick には
    //! @ref nn::atk::SoundSystem::ReadCircularBufferSink() の呼び出し直前の Tick を指定します。
    //!
    //! @param[in] buffer 波形のサンプルデータが格納されたバッファへのポインタです。
    //! @param[in] bufferSize バッファに格納されたサンプルデータのサイズです。
    //! @param[in] tick nn::atk::SoundSystem::ReadCircularBufferSink() の呼び出し直前の Tick です。
    //!
    //! @return 成功したら true, 失敗したら false を返します。
    //!
    //! @pre
    //! - tick <= nn::os::GetSystemTick()
    //! - tick は前回の呼び出し時の tick 以上でなければなりません。
    //! - IsInitialized() == true
    //! - Initialize() で波形データ送信のためのパラメータが指定されている。
    //!
    //! @sa Initialize
    //! @sa nn::audio::ReadCircularBufferSink
    //! @sa nn::audio::RequestUpdateAudioRenderer
    //! @sa nn::os::GetSystemTick
    bool PushWaveform(
        const void* buffer,
        size_t bufferSize,
        nn::os::Tick tick) NN_NOEXCEPT;

    //! @brief nn::atk パフォーマンス情報を送信します。
    //!
    //! @details
    //! @ref nn::atk からパフォーマンス情報を取得し送信します。
    //!
    //! @return 成功したら true, 失敗したら false を返します。
    //!
    //! @pre
    //! - tick <= nn::os::GetSystemTick()
    //! - tick は前回の呼び出し時の tick 以上でなければなりません。
    //! - IsInitialized() == true
    //! - Initialize() で波形データ送信のためのパラメータが指定されている。
    //!
    //! @sa Initialize
    //! @sa nn::audio::ReadCircularBufferSink
    //! @sa nn::audio::RequestUpdateAudioRenderer
    //! @sa nn::os::GetSystemTick
    bool PushAtkProfiles() NN_NOEXCEPT;

    //! @brief nn::atk パフォーマンス情報を送信します。
    //! この機能は現在有用性を評価中です。予告なく破壊的な仕様変更が入る可能性がある点にご注意ください。
    //!
    //! @details
    //! @ref nn::atk からパフォーマンス情報を取得し送信します。
    //!      SoundArchivePlayer を指定することで、ストリームサウンドのファイルリードキャッシュを使用している場合に
    //!      ストリームサウンドパネルにキャッシュの残量が追加で表示されるようになります。
    //!
    //! @param[in] soundArchivePlayer サウンドアーカイブプレイヤーを指定します。
    //! @return 成功したら true, 失敗したら false を返します。
    //!
    //! @pre
    //! - tick <= nn::os::GetSystemTick()
    //! - tick は前回の呼び出し時の tick 以上でなければなりません。
    //! - IsInitialized() == true
    //! - Initialize() で波形データ送信のためのパラメータが指定されている。
    //!
    //! @sa Initialize
    //! @sa nn::audio::ReadCircularBufferSink
    //! @sa nn::audio::RequestUpdateAudioRenderer
    //! @sa nn::os::GetSystemTick
    bool PushAtkProfiles(::nn::atk::SoundArchivePlayer& soundArchivePlayer) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief nn::atk サウンドアーカイブプレイヤー情報を送信します。
    //! @return 成功したら true, 失敗したら false を返します。
    //! @pre
    //! - IsInitialized() == true
    //! - soundArchivePlayer.IsAvailable() == true
    //! - Initialize() でサウンドアーカイブプレイヤー情報送信のためのパラメータが指定されている。
    //! @sa Initialize
    bool PushSoundArchivePlayerInfo(::nn::atk::SoundArchivePlayer& soundArchivePlayer) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief nn::atk サウンドシステム情報を送信します。
    //! @return 成功したら true, 失敗したら false を返します。
    //! @pre
    //! - IsInitialized() == true
    //! - Atk サウンドシステムが初期化済み
    //! - Initialize() でサウンドシステム情報送信のためのパラメータが指定されている。
    //! @sa Initialize
    bool PushSoundSystemInfo() NN_NOEXCEPT;

    //! @briefprivate
    //! @brief nn::atk サウンドヒープ情報を送信します。
    //! @return 成功したら true, 失敗したら false を返します。
    //! @pre
    //! - IsInitialized() == true
    //! - Atk サウンドヒープが初期化済み
    //! - Initialize() でサウンドヒープ情報送信のためのパラメータが指定されている。
    //! @sa Initialize
    bool PushSoundHeapInfo(::nn::atk::SoundHeap& soundHeap) NN_NOEXCEPT;

    //! @brief nn::atk シーケンス変数情報を送信します。
    //! この機能は現在有用性を評価中です。予告なく破壊的な仕様変更が入る可能性がある点にご注意ください。
    //!
    //! @details
    //! @ref nn::atk からシーケンス変数情報を取得し送信します。
    //!
    //! @return 成功したら true, 失敗したら false を返します。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //! - soundArchivePlayer.IsAvailable() == true
    //!
    //! @sa Initialize
    //! @sa nn::atk::SoundArchivePlayer::IsAvailable
    bool PushSequenceVariable(::nn::atk::SoundArchivePlayer& soundArchivePlayer) NN_NOEXCEPT;

    //! @brief nn::atk ストリームサウンド情報を送信します。
    //!
    //! @details
    //! soundArchivePlayer にて再生されているストリームサウンドの情報を送信します。
    //! @ref IsStreamSoundInfoEnabled() == false のときは何もしません。
    //!
    //! ストリームサウンドのファイルリードキャッシュを使用している場合に
    //! @ref nn::atk::SoundArchivePlayer を引数にもつ @ref PushAtkProfiles() を併せて呼ぶと、
    //! ストリームサウンドパネルにキャッシュの残量が追加で表示されるようになります。
    //!
    //! @return 成功したら true, 失敗したら false を返します。
    //!
    //! @param[in] soundArchivePlayer サウンドアーカイブプレイヤーを指定します。
    //!
    //! @pre
    //! - IsInitialized() == true
    //! - soundArchivePlayer が初期化済み
    //!
    //! @sa Initialize
    bool PushStreamSoundInfo(::nn::atk::SoundArchivePlayer& soundArchivePlayer) NN_NOEXCEPT;

protected:
    virtual void OnRegistered() NN_NOEXCEPT NN_OVERRIDE;
    virtual void OnUnregistered() NN_NOEXCEPT NN_OVERRIDE;
    virtual void OnRequested(bool bRequested) NN_NOEXCEPT NN_OVERRIDE;
    virtual void OnSessionStarted() NN_NOEXCEPT NN_OVERRIDE;

private:
    static AtkSpyModule& GetInstance() NN_NOEXCEPT;

    bool IsDataRequested() NN_NOEXCEPT;

    static void SpyLog(const char* message) NN_NOEXCEPT;

    void WriteSoundStateToPacket(
        detail::SoundStatePacket* packet,
        const ::nn::atk::detail::BasicSound* sound) NN_NOEXCEPT;

    size_t WriteSequenceVariableToPacket(
        detail::SequenceVariablePacket::Item* item,
        const ::nn::atk::detail::SequenceSound* sequenceSound) NN_NOEXCEPT;
    int GetValidVariableLastIndex(int16_t* variables, int variableCount);

    bool PushSoundArchiveData(const ::nn::atk::SoundArchive& soundArchive) NN_NOEXCEPT;

    bool PushSoundData(const ::nn::atk::SoundArchive& soundArchive) NN_NOEXCEPT;

    bool PushPlayerData(const ::nn::atk::SoundArchive& soundArchive) NN_NOEXCEPT;

    bool PushAddonSoundArchiveData(const ::nn::atk::SoundArchivePlayer& soundArchivePlayer) NN_NOEXCEPT;

    bool PushStreamSoundData(const ::nn::atk::StreamSoundHandle& soundHandle, const char* const label, int streamSoundCount) NN_NOEXCEPT;

    bool PushSoundDataPacket(
        const detail::SoundDataPacket& packet,
        size_t itemAreaSize) NN_NOEXCEPT;

    bool PushPlayerDataPacket(
        const detail::PlayerDataPacket& packet,
        size_t itemAreaSize) NN_NOEXCEPT;

    bool PushSequenceVariablePacket(
        const detail::SequenceVariablePacket& packet,
        size_t itemAreaSize) NN_NOEXCEPT;

    void PushAtkSoundThreadAndDspProfiles() NN_NOEXCEPT;
    bool PushAtkProfiles(::nn::atk::SoundArchivePlayer* soundArchivePlayer) NN_NOEXCEPT;
    void PushRemainingCachePercentage(nn::atk::SoundArchivePlayer* soundArchivePlayer, nn::atk::detail::driver::StreamSoundPlayer* streamSoundPlayer, float remainingCachePercentage) NN_NOEXCEPT;
    void PushAtkTaskThreadProfiles(::nn::atk::SoundArchivePlayer* soundArchivePlayer) NN_NOEXCEPT;

    void InitializeLog() NN_NOEXCEPT;

    void FinalizeLog() NN_NOEXCEPT;

    void InitializeAudioSpyModule(
        const InitializeArg& args,
        void** pBuffer,
        void* bufferEnd) NN_NOEXCEPT;

    void FinalizeAudioSpyModule() NN_NOEXCEPT;

    void InitializeAtkProfiles(
        const InitializeArg& args,
        void** pBuffer,
        void* bufferEnd) NN_NOEXCEPT;

    void FinalizeAtkProfiles() NN_NOEXCEPT;

    void RegisterPlotItems() NN_NOEXCEPT;
    void UnregisterPlotItems() NN_NOEXCEPT;

    void InitializeAddonSoundArchive(
        const InitializeArg& args,
        void** pBuffer,
        void* bufferEnd) NN_NOEXCEPT;

    void FinalizeAddonSoundArchive() NN_NOEXCEPT;

    void ClearPushedAddonSoundArchiveInfos() NN_NOEXCEPT;

    bool IsAddonSoundArchiveEnabled() const NN_NOEXCEPT
    {
        return m_AddonSoundArchiveInfoCountMax > 0;
    }

    void InitializeStreamSoundInfo(
        const InitializeArg& args,
        void** pBuffer,
        void* bufferEnd) NN_NOEXCEPT;

    void FinalizeStreamSoundInfo() NN_NOEXCEPT;

private:
    static const int LabelNameLengthMax = 127;
    static const int StreamStateNameLengthMax = LabelNameLengthMax + 1 + 7 + 1; // LabelNameLengthMax + " " + "Delayed" + '\0'

    struct StreamSoundPlotNode
    {
        ::nn::spy::PlotNode streamSoundNode;
        ::nn::spy::PlotFloat streamSoundRateFloat;
        ::nn::spy::PlotState streamSoundState;
        char stateString[StreamStateNameLengthMax];
        ::nn::spy::PlotFloat remainingCachePercentageFloat;
    };

    static AtkSpyModule* g_pInstance;

    ::nn::spy::audio::AudioSpyModule m_AudioSpyModule;
    ::nn::atk::AudioRendererPerformanceReader m_AudioRendererPerformanceReader;
    ::nn::atk::ProfileReader* m_pAtkProfileReader;
    ::nn::atk::TaskProfileReader* m_pAtkTaskProfileReader;
    void* m_pAtkTaskProfileReaderBuffer;
    std::atomic<bool> m_IsDataRequested;
    bool m_IsInitialized;
    std::atomic<bool> m_IsLogEnabled;
    std::atomic<bool> m_IsSoundStateEnabled;
    bool m_IsPerformanceMetricsInitialized;
    std::atomic<bool> m_IsPerformanceMetricsEnabled;
    bool m_IsWaveformInitialized;
    std::atomic<bool> m_IsWaveformEnabled;
    bool m_IsAtkProfilesInitialized;
    std::atomic<bool> m_IsAtkProfilesEnabled;
    std::atomic<bool> m_IsSoundArchivePlayerInfoEnabled;
    std::atomic<bool> m_IsSoundSystemInfoEnabled;
    std::atomic<bool> m_IsSoundHeapInfoEnabled;
    std::atomic<bool> m_IsStreamSoundInfoEnabled;

    AddonSoundArchiveInfo* m_AddonSoundArchiveInfoArray;
    AddonSoundArchiveInfoDic m_PushedAddonSoundArchiveInfoDic;
    AddonSoundArchiveInfoList m_FreeAddonSoundArchiveInfoList;
    int m_AddonSoundArchiveInfoCountMax;
    ::nn::os::Tick m_AddonSoundArchiveLastAddTick;

    ::nn::spy::PlotState m_AtkSoundThreadState;
    ::nn::spy::PlotState m_AtkTaskThreadState;
    ::nn::spy::PlotState m_DspState;
    ::nn::os::Tick m_LastNwFrameProcessEnd;
    ::nn::os::Tick m_LastDspFrameProcessEnd;

    ::nn::spy::PlotNode m_AtkSoundArchivePlayerNode;
    ::nn::spy::PlotNode m_AtkSoundSystemNode;
    ::nn::spy::PlotNode m_AtkSoundHeapNode;

    ::nn::spy::PlotFloat m_AtkWaveSoundActiveCount;
    ::nn::spy::PlotFloat m_AtkStreamSoundActiveCount;
    ::nn::spy::PlotFloat m_AtkSequenceSoundActiveCount;

    ::nn::spy::PlotFloat m_AtkAllocatedDriverCommandBufferSize;
    ::nn::spy::PlotFloat m_AtkDriverCommandBufferSize;
    ::nn::spy::PlotFloat m_AtkAllocatedTaskCommandBufferSize;
    ::nn::spy::PlotFloat m_AtkTaskCommandBufferSize;

    ::nn::spy::PlotFloat m_AtkAllocatedSoundHeapSize;
    ::nn::spy::PlotFloat m_AtkSoundHeapSize;

    StreamSoundPlotNode* m_StreamSoundPlots;
    int m_StreamSoundPlotCount;
    int m_PrevStreamCount;

#endif // NN_BUILD_CONFIG_SPY_ENABLED
};

} // namespace nn::spy::atk
} // namespace nn::spy
} // namespace nn
