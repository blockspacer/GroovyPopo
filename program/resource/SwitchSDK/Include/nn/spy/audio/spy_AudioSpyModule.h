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

#include <nn/spy.h>

#include <nn/audio/audio_PerformanceMetrics.h>
#include <nn/os/os_Tick.h>

namespace nn {
namespace spy {
namespace audio {

namespace detail
{
    class PlotAudioPerformanceMetrics;
    class WaveformModule;
}

//! @brief  nn::audio の状態を収集し、Spy.exe に出力するモジュールです。
class AudioSpyModule : public nn::spy::SpyModule
{
    typedef nn::spy::SpyModule Base;

public:
    //! @brief チャンネルの種別を定義します。
    enum ChannelType
    {
        ChannelType_FrontLeft,      //!< フロント左チャンネル
        ChannelType_FrontRight,     //!< フロント右チャンネル
        ChannelType_RearLeft,       //!< リア左チャンネル
        ChannelType_RearRight,      //!< リア右チャンネル
        ChannelType_FrontCenter,    //!< フロントセンターチャンネル
        ChannelType_Lfe,            //!< LFE チャンネル

        //! @briefprivate
        //! @brief ChannelType の最大値
        ChannelType_Max = ChannelType_Lfe,
    };

    //! @brief  AudioSpyModule の初期化パラメータです。
    //!
    //! @sa AudioSpyModule::GetRequiredMemorySize
    //! @sa AudioSpyModule::Initialize
    struct InitializeArg
    {
        //! @brief コンストラクタです。
        InitializeArg() NN_NOEXCEPT
            : pAudioRendererParameter(nullptr)
            , pWaveformChannelTypes(nullptr)
            , waveformChannelCount(0)
            , waveformSampleFormat(nn::audio::SampleFormat_Invalid)
        {}

        //! 情報を取得したい AudioRenderer のパラメータです。
        //! 必ず指定する必要があります。
        const nn::audio::AudioRendererParameter* pAudioRendererParameter;

        //! 波形データの各チャンネルの意味を定義します。
        //! 波形データを送信する場合に指定する必要があります。
        //!
        //! @sa AudioSpyModule::PushWaveform
        const ChannelType* pWaveformChannelTypes;

        //! 波形データのチャンネル数です。
        //! 波形データを送信する場合には 0 より大きい値を指定します。
        //!
        //! @sa AudioSpyModule::PushWaveform
        int waveformChannelCount;

        //! 波形データのサンプルフォーマットです。
        //! 波形データを送信する場合に指定する必要があります。
        //!
        //! @sa AudioSpyModule::PushWaveform
        nn::audio::SampleFormat waveformSampleFormat;
    };

#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

public:
    NN_FORCEINLINE AudioSpyModule() NN_NOEXCEPT
    {
    }

    NN_FORCEINLINE static size_t GetRequiredMemorySize(const InitializeArg& args) NN_NOEXCEPT
    {
        NN_UNUSED(&args);
        return 0;
    }

    NN_FORCEINLINE bool Initialize(
        const InitializeArg& args,
        void* buffer,
        size_t bufferSize) NN_NOEXCEPT
    {
        NN_UNUSED(&args);
        NN_UNUSED(buffer);
        NN_UNUSED(bufferSize);
        return true;
    }

    NN_FORCEINLINE void Finalize() NN_NOEXCEPT
    {
    }

    NN_FORCEINLINE bool IsInitialized() const NN_NOEXCEPT
    {
        return true;
    }

    NN_FORCEINLINE bool PushPerformanceMetrics(
        const void* performanceFrameBuffer,
        size_t performanceFrameBufferSize) NN_NOEXCEPT
    {
        NN_UNUSED(performanceFrameBuffer);
        NN_UNUSED(performanceFrameBufferSize);
        return true;
    }

    NN_FORCEINLINE bool PushPerformanceMetrics(
        const void* performanceFrameBuffer,
        size_t performanceFrameBufferSize,
        nn::os::Tick tick) NN_NOEXCEPT
    {
        NN_UNUSED(performanceFrameBuffer);
        NN_UNUSED(performanceFrameBufferSize);
        NN_UNUSED(tick);
        return true;
    }

    NN_FORCEINLINE bool PushWaveform(
        const void* buffer,
        size_t bufferSize,
        nn::os::Tick tick) NN_NOEXCEPT
    {
        NN_UNUSED(buffer);
        NN_UNUSED(bufferSize);
        NN_UNUSED(tick);
        return true;
    }

#else

public:
    //! @brief  コンストラクタです。
    AudioSpyModule() NN_NOEXCEPT;

    //! @brief  初期化に必要なメモリサイズを取得します。
    //!
    //! @param[in]  args  初期化パラメータです。
    //!
    //! @return  初期化に必要なメモリサイズを返します。
    //!
    //! @pre
    //! - args.pAudioRendererParameter != nullptr
    //!
    //! @sa Initialize
    //! @sa InitializeArg
    static size_t GetRequiredMemorySize(const InitializeArg& args) NN_NOEXCEPT;

    //! @brief  初期化を行います。
    //!
    //! @details
    //! AudioSpyModule が使用するワークメモリをユーザが渡します。
    //! @ref InitializeArg 構造体に初期化に必要なパラメータを設定し、
    //! @ref GetRequiredMemorySize() で必要なメモリサイズを取得してください。
    //!
    //! @param[in]  args  初期化パラメータです。
    //! @param[in]  buffer  AudioSpyModule が使用するワークメモリへのポインタです。
    //! @param[in]  bufferSize  ワークメモリのサイズです。
    //!
    //! @return  成功したら true, 失敗したら false を返します。
    //!
    //! @pre
    //! - buffer は nn::DefaultAlignment にアラインしている
    //! - bufferSize >= @ref GetRequiredMemorySize()
    //! - @ref IsInitialized() == false
    //! - @ref IsRegistered() == false
    //! - args.pAudioRendererParameter != nullptr
    //!
    //! @sa GetRequiredMemorySize
    //! @sa Finalize
    bool Initialize(
        const InitializeArg& args,
        void* buffer,
        size_t bufferSize) NN_NOEXCEPT;

    //! @brief  ファイナライズを行います。
    //!
    //! @pre
    //! - @ref IsInitialized() == true
    //! - @ref IsRegistered() == false
    //!
    //! @sa Initialize
    void Finalize() NN_NOEXCEPT;

    //! @brief  初期化されているかどうかを取得します。
    //!
    //! @return  初期化されていれば true を返します。
    //!
    //! @sa Initialize
    bool IsInitialized() const NN_NOEXCEPT;

    //! @brief  パフォーマンス情報を送信します。
    //!
    //! @param[in]  performanceFrameBuffer      nn::audio::SetPerformanceFrameBuffer に渡した
    //!                                         nn::audio パフォーマンス情報が格納されたバッファへのポインタです。
    //! @param[in]  performanceFrameBufferSize  performanceFrameBuffer のサイズです。
    //!
    //! @return  成功したら true, 失敗したら false を返します。
    //!
    //! @pre
    //! - IsInitialized() == true
    //!
    //! @details
    //! この関数を呼び出す前には、必ず Initialize() で初期化しておいてください。
    //!
    //! nn::audio パフォーマンス情報は本関数を呼び出す直前に取得されたものである必要があります。
    //!
    //! @sa Initialize
    bool PushPerformanceMetrics(
        const void* performanceFrameBuffer,
        size_t performanceFrameBufferSize) NN_NOEXCEPT;

    //! @brief  パフォーマンス情報を送信します。
    //!
    //! @param[in]  performanceFrameBuffer      nn::audio::SetPerformanceFrameBuffer に渡した
    //!                                         nn::audio パフォーマンス情報が格納されたバッファへのポインタです。
    //! @param[in]  performanceFrameBufferSize  performanceFrameBuffer のサイズです。
    //! @param[in]  tick                        nn::audio パフォーマンス情報を取得した時の Tick です。
    //!
    //! @return  成功したら true, 失敗したら false を返します。
    //!
    //! @pre
    //! - tick <= nn::os::GetSystemTick()
    //! - tick は前回の呼び出し時の tick 以上でなければなりません。
    //! - IsInitialized() == true
    //!
    //! @details
    //! この関数を呼び出す前には、必ず Initialize() で初期化しておいてください。
    //!
    //! @sa Initialize
    bool PushPerformanceMetrics(
        const void* performanceFrameBuffer,
        size_t performanceFrameBufferSize,
        nn::os::Tick tick) NN_NOEXCEPT;

    //! @brief 波形データを送信します。
    //!
    //! @details
    //! @ref nn::audio::ReadCircularBufferSink() で取得した波形データを送信します。
    //!
    //! 引数 tick には nn::audio::ReadCircularBufferSink() の前に呼んだ
    //! nn::audio::RequestUpdateAudioRenderer() の完了時の Tick を指定します。
    //!
    //! @param[in] buffer 波形のサンプルデータが格納されたバッファへのポインタです。
    //! @param[in] bufferSize バッファに格納されたサンプルデータのサイズです。
    //! @param[in] tick nn::audio::RequestUpdateAudioRenderer() の完了時の Tick です。
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

protected:
    virtual void OnRegistered() NN_NOEXCEPT NN_OVERRIDE;

    virtual void OnUnregistered() NN_NOEXCEPT NN_OVERRIDE;

private:
    detail::PlotAudioPerformanceMetrics* m_pPlotAudioPerformanceMetrics;
    detail::WaveformModule* m_pWaveformModule;
    bool m_IsInitialized;

#endif
};

} // namespace audio
} // namespace spy
} // namespace nn
