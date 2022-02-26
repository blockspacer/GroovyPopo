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
#include <nn/atk/atk_Global.h>

namespace nn { namespace atk { namespace detail {

struct BusMixVolumePacket
{
public:
    static size_t GetRequiredMemSize(int busCount) NN_NOEXCEPT;

    BusMixVolumePacket() NN_NOEXCEPT : m_IsUsed( false ), m_pIsEnabledTable( nullptr ) {}

    bool Initialize(void* buffer, size_t size, int busCount) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;
    void Reset() NN_NOEXCEPT;

    int GetBusCount() const NN_NOEXCEPT
    {
        return m_BusCount;
    }

    float GetBusMixVolume(int waveChannel, int mixChannel) const NN_NOEXCEPT
    {
        return m_BusMixVolume.volume[waveChannel][mixChannel];
    }

    const OutputBusMixVolume& GetBusMixVolume() const NN_NOEXCEPT
    {
        return m_BusMixVolume;
    }

    void SetBusMixVolume(int waveChannel, int mixChannel, float volume) NN_NOEXCEPT
    {
        m_BusMixVolume.volume[waveChannel][mixChannel] = volume;
    }

    void SetBusMixVolume(const OutputBusMixVolume& busMixVolume) NN_NOEXCEPT
    {
        m_BusMixVolume = busMixVolume;
    }

    bool IsUsed() const NN_NOEXCEPT
    {
        return m_IsUsed;
    }

    void SetUsed(bool isUsed) NN_NOEXCEPT
    {
        m_IsUsed = isUsed;
    }

    bool IsEnabled(int bus) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE(bus, 0, m_BusCount);
        return m_pIsEnabledTable[bus];
    }

    void SetEnabled(int bus, bool isEnabled) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE(bus, 0, m_BusCount);
        m_pIsEnabledTable[bus] = isEnabled;
    }

private:
    OutputBusMixVolume m_BusMixVolume;
    bool m_IsUsed;
    bool* m_pIsEnabledTable;
    int m_BusCount;
};

}}}
