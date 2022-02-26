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

#include <nn/atk/atk_Channel.h>
#include <nn/atk/atk_InstancePool.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

class ChannelManager
{
public:
    static NN_NOINLINE ChannelManager& GetInstance() NN_NOEXCEPT;

    size_t GetObjectSize( const detail::SoundInstanceConfig& config ) NN_NOEXCEPT;
    size_t GetRequiredMemSize( int channelCount, const detail::SoundInstanceConfig& config ) NN_NOEXCEPT;
    void Initialize( void* mem, size_t memSize, int channelCount, const detail::SoundInstanceConfig& config ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;
    Channel* Alloc() NN_NOEXCEPT;
    void Free( Channel* channel ) NN_NOEXCEPT;

    void UpdateAllChannel() NN_NOEXCEPT;
    void UpdateAudioFrameChannel() NN_NOEXCEPT;

private:
    typedef InstancePool<Channel> ChannelPool;
    typedef util::IntrusiveList<Channel, util::IntrusiveListMemberNodeTraits<Channel,&Channel::m_Link>> ChannelList;

    ChannelManager() NN_NOEXCEPT;

    ChannelPool m_Pool;
    ChannelList m_ChannelList;
    bool m_IsInitialized;
    int m_ChannelCount;

    InstancePool<OutputAdditionalParam>* m_pAdditionalParamPool;
    BufferPool* m_pAdditionalParamBufferPool;
    size_t m_AdditionalParamBufferSizePerChannel;
    SoundInstanceConfig m_SoundInstanceConfig;
};

} // namespace nn::atk::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

