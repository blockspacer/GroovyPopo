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

#include <nn/nn_SdkAssert.h>

#include <nn/spy/spy_Config.h>
#include <nn/spy/audio/spy_AudioSpyModule.h>

#if defined(NN_BUILD_CONFIG_SPY_ENABLED)

namespace nn {
namespace spy {
namespace audio {
namespace detail {

struct WaveformPacketData
{
    static const uint32_t PacketVersion = NN_DETAIL_SPY_FND_BINARY_MAKE_U32_SIGNATURE(0, 1, 1, 0);

    static const int MaxChannelCount = 32;
    static const int MaxSamplePerFrame = 240;

    //! @breif パケットのタイプ。
    enum PacketType
    {
        PacketType_WaveformMetadata = 1,
        PacketType_Waveform,
        PacketType_DeviceOutputVolume,
    };

    //! @brief サンプルデータのフォーマット。
    //! nn::audio::SampleFormat と対応しています。
    enum SampleFormat
    {
        SampleFormat_Invalid = 0, //!< 無効なフォーマットです
        SampleFormat_PcmInt16 = 2, //!< 16 bit 符号付き整数 PCM フォーマットです。
    };

    //! @brief チャンネル情報。
    struct ChannelInfo
    {
        //! チャンネルの意味です。
        //! (WaveformModule::ChannelType)
        uint8_t channelType;

        //! 未使用です。
        uint8_t reserved[3];
    };

    struct WaveformMetadataPacket
    {
        //! パケットのタイプです。
        //! (PacketType)
        uint8_t packetType;

        //! サンプルのデータ形式です。
        uint8_t sampleFormat;

        //! チャンネル数です。
        uint8_t channelCount;

        //! 未使用です。
        uint8_t reserved;

        //! サンプルレート（１秒あたりのサンプル数）です。
        int32_t sampleRate;

        //! オーディオフレームあたりのサンプル数です。
        int32_t sampleCountPerAudioFrame;

        //! チャンネルの情報です。
        //! サンプルデータ内でのチャンネルの格納順に、各ジャンネルの情報を格納します。
        ChannelInfo channelInfos[MaxChannelCount]; // [channelCount]
    };

    struct WaveformPacket
    {
        //! パケットのタイプです。
        //! (PacketType)
        uint8_t packetType;

        //! 未使用です。
        uint8_t reserved[1];

        //! このパケットに含まれるサンプルの数です。
        uint16_t sampleCount;

        //! パケットの先頭サンプルを基準にした、tick におけるサンプルのインデックスです。
        //! 例えば WaveformModule::PushWaveform() に２フレーム分のバッファが与えられた場合、
        //! tick の値は２つ目のフレームの再生時間を表しています。
        //! WaveformModule はフレーム毎に１つのパケットを送信するので、
        //! １つ目のパケットでは sampleIndexOnTick = sampleCount となり、
        //! ２つ目のパケットでは sampleIndexOnTick = 0 となります。
        //!
        //! |----- packet 0 ----|---- packet 1 ----|
        //! ....................|................... sample
        //!                    tick
        int32_t sampleIndexOnTick;

        // 以下のデータが続きます。
        // uint16_t sampleData[channelCount][sampleCount];
    };

    struct DeviceOutputVolumePacket
    {
        //! パケットのタイプです。
        //! (PacketType)
        uint8_t packetType;

        //! 未使用です。
        uint8_t reserved[3];

        //! デバイスのボリューム(0.0f ~ 128.0f)です。
        float volume;
    };
};

//! @brief 波形データを Spy.exe に出力するモジュールです。
class WaveformModule : public SpyModule
{
    typedef SpyModule Base;

public:
    typedef AudioSpyModule::ChannelType ChannelType;

    //! @brief  WaveformModule の初期化パラメータです。
    //!
    //! @sa WaveformModule::Initialize
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
        //! @sa WaveformModule::PushWaveform
        const ChannelType* pWaveformChannelTypes;

        //! 波形データのチャンネル数です。
        //! 波形データを送信する場合には 0 より大きい値を指定します。
        //!
        //! @sa WaveformModule::PushWaveform
        int waveformChannelCount;

        //! 波形データのサンプルフォーマットです。
        //! 波形データを送信する場合に指定する必要があります。
        //!
        //! @sa WaveformModule::PushWaveform
        nn::audio::SampleFormat waveformSampleFormat;
    };

    //! @brief コンストラクタです。
    WaveformModule() NN_NOEXCEPT;

    bool Initialize(const InitializeArg& initializeArg) NN_NOEXCEPT;

    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_IsInitialized;
    }

    void Finalize() NN_NOEXCEPT;

    bool PushWaveform(
        const void* buffer,
        size_t bufferSize,
        nn::os::Tick tick) NN_NOEXCEPT;

protected:
    virtual void OnRegistered() NN_NOEXCEPT NN_OVERRIDE;
    virtual void OnRequested(bool isRequested) NN_NOEXCEPT NN_OVERRIDE;

private:
    void PushWaveformMetadata() NN_NOEXCEPT;

private:
    WaveformPacketData::WaveformMetadataPacket m_WaveformMetadataPacket;
    float m_DeviceVolume;
    std::atomic<bool> m_IsDataRequested;
    bool m_IsAudioTvOutputAvailable;
    bool m_IsInitialized;
};

} // namespace nn::spy::audio::detail
} // namespace nn::spy::audio
} // namespace nn::spy
} // namespace nn

#endif
