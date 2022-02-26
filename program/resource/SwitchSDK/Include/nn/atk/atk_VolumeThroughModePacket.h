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

namespace nn { namespace atk { namespace detail {

typedef ValueArray<uint8_t> VolumeThroughModeArray;

class VolumeThroughModePacket
{
public:
    VolumeThroughModePacket() NN_NOEXCEPT : m_VolumeThroughMode(), m_IsVolumeThroughModeUsed( false ), m_BinaryVolume( 1.0f ) {};

    static size_t GetRequiredMemSize(int busCount) NN_NOEXCEPT;
    bool Initialize(void* buffer, size_t size, int busCount) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    void Reset() NN_NOEXCEPT;

    int GetBusCount() const NN_NOEXCEPT
    {
        return m_VolumeThroughMode.GetCount();
    }
    uint8_t GetVolumeThroughMode(int bus) const NN_NOEXCEPT
    {
        return m_VolumeThroughMode.GetValue(bus);
    }
    void SetVolumeThroughMode(int bus, uint8_t volumeThroughMode) NN_NOEXCEPT
    {
        m_VolumeThroughMode.SetValue(bus, volumeThroughMode);
    }
    uint8_t TryGetVolumeThroughMode(int bus) const NN_NOEXCEPT
    {
        return m_VolumeThroughMode.TryGetValue(bus);
    }
    bool TrySetVolumeThroughMode(int bus, uint8_t volumeThroughMode) NN_NOEXCEPT
    {
        return m_VolumeThroughMode.TrySetValue(bus, volumeThroughMode);
    }

    bool IsVolumeThroughModeUsed() const NN_NOEXCEPT
    {
        return m_IsVolumeThroughModeUsed;
    }
    void SetVolumeThroughModeUsed(bool isUsed) NN_NOEXCEPT
    {
        m_IsVolumeThroughModeUsed = isUsed;
    }

    float GetBinaryVolume() const NN_NOEXCEPT
    {
        return m_BinaryVolume;
    }
    void SetBinaryVolume(float volume) NN_NOEXCEPT
    {
        m_BinaryVolume = volume;
    }

    VolumeThroughModePacket& operator=(const VolumeThroughModePacket& rhs) NN_NOEXCEPT;

private:
    VolumeThroughModeArray m_VolumeThroughMode;
    bool m_IsVolumeThroughModeUsed;
    float m_BinaryVolume;
};

}}}

