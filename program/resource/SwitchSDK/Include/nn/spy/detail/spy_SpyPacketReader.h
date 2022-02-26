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

#include <nn/spy/spy_Config.h>
#ifdef NN_BUILD_CONFIG_SPY_ENABLED

#include <nn/spy/detail/fnd/hio/spyfnd_HioChannel.h>
#include <nn/spy/detail/spy_Packet.h>

namespace nn {
namespace spy {
namespace detail {

struct PacketBase;

class SpyPacketReader
{
public:
    enum State
    {
        State_NotInitialized,
        State_Idle,
        State_ReadingHeader,
        State_ReadHeader,
        State_ReadingBody,
        State_ReadBody
    };

public:
    SpyPacketReader() NN_NOEXCEPT;

public:
    void Initialize(nn::spy::detail::fnd::HioChannel* channel, void* buffer) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    bool IsInitialized() const NN_NOEXCEPT { return m_State > State_NotInitialized; }

    bool ReadPacketHeader() NN_NOEXCEPT;

    bool ReadPacketBody(void* buffer, size_t length) NN_NOEXCEPT;

    void Next() NN_NOEXCEPT;

    State GetState() const NN_NOEXCEPT { return m_State; }
    bool  CanReadHeader() const NN_NOEXCEPT { return m_State == State_Idle; }

    const PacketHeader* GetCurrentPacketHeader() const NN_NOEXCEPT
    {
        return m_ReadPacketLength >= sizeof(PacketHeader) ? m_pPacketHeader : NULL;
    }

private:
    static const char* StateToString(State value) NN_NOEXCEPT;

private:
    nn::spy::detail::fnd::HioChannel* m_Channel;

    State         m_State;
    PacketHeader* m_pPacketHeader;
    size_t        m_ReadPacketLength;
};

} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn

#endif // NN_BUILD_CONFIG_SPY_ENABLED
