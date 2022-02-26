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

#include <nn/atk/atk_ValueArray.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_VolumeThroughModePacket.h>
#include <nn/atk/atk_BusMixVolumePacket.h>

namespace nn { namespace atk { namespace detail {

typedef ValueArray<float> SendArray;

class OutputAdditionalParam
{
public:
    OutputAdditionalParam() NN_NOEXCEPT: m_pAdditionalSend(nullptr), m_pBusMixVolumePacket(nullptr), m_pVolumeThroughModePacket( nullptr ) {}
    static size_t GetRequiredMemSize(const SoundInstanceConfig& config) NN_NOEXCEPT;
    void Initialize(void* buffer, size_t bufferSize, const SoundInstanceConfig& config) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    void Reset() NN_NOEXCEPT;
    void* GetBufferAddr() NN_NOEXCEPT;

    SendArray* GetAdditionalSendAddr() NN_NOEXCEPT;
    const SendArray* const GetAdditionalSendAddr() const NN_NOEXCEPT;
    bool IsAdditionalSendEnabled() const NN_NOEXCEPT;
    float TryGetAdditionalSend(int bus) const NN_NOEXCEPT;
    void TrySetAdditionalSend(int bus, float send) NN_NOEXCEPT;

    BusMixVolumePacket* GetBusMixVolumePacketAddr() NN_NOEXCEPT;
    const BusMixVolumePacket* const GetBusMixVolumePacketAddr() const NN_NOEXCEPT;
    bool IsBusMixVolumeEnabled() const NN_NOEXCEPT;
    float GetBusMixVolume(int waveChannel, int mixChannel) const NN_NOEXCEPT;
    const OutputBusMixVolume& GetBusMixVolume() const NN_NOEXCEPT;
    void SetBusMixVolume(int waveChannel, int mixChannel, float volume) NN_NOEXCEPT;
    void SetBusMixVolume(const OutputBusMixVolume & param) NN_NOEXCEPT;
    bool IsBusMixVolumeUsed() const NN_NOEXCEPT;
    void SetBusMixVolumeUsed(bool isUsed) NN_NOEXCEPT;
    // TODO: API 名整理
    bool IsBusMixVolumeEnabledForBus(int bus) const NN_NOEXCEPT;
    void SetBusMixVolumeEnabledForBus(int bus, bool isEnabled) NN_NOEXCEPT;

    VolumeThroughModePacket* GetVolumeThroughModePacketAddr() NN_NOEXCEPT;
    const VolumeThroughModePacket* const GetVolumeThroughModePacketAddr() const NN_NOEXCEPT;
    bool IsVolumeThroughModeEnabled() const NN_NOEXCEPT;
    float GetBinaryVolume() const NN_NOEXCEPT;
    void SetBinaryVolume(float volume) NN_NOEXCEPT;
    uint8_t TryGetVolumeThroughMode(int bus) const NN_NOEXCEPT;
    void TrySetVolumeThroughMode(int bus, uint8_t mode) NN_NOEXCEPT;
    bool IsVolumeThroughModeUsed() const NN_NOEXCEPT;
    void SetVolumeThroughModeUsed(bool isUsed) NN_NOEXCEPT;

    OutputAdditionalParam& operator=(const OutputAdditionalParam& rhs) NN_NOEXCEPT;

private:
    SendArray* m_pAdditionalSend;
    BusMixVolumePacket* m_pBusMixVolumePacket;
    VolumeThroughModePacket* m_pVolumeThroughModePacket;
};
}}}
