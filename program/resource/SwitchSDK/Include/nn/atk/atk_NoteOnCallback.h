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

#include <nn/atk/atk_Channel.h>     // Channel::ChannelCallback

namespace nn {
namespace atk {
namespace detail {
namespace driver {

class SequenceSoundPlayer;

struct NoteOnInfo
{
    int prgNo;
    int key;
    int velocity;
    int length;
    int initPan;
    int priority;
    Channel::ChannelCallback channelCallback;
    void* channelCallbackData;
    OutputReceiver* pOutputReceiver;
    UpdateType updateType;
};

class NoteOnCallback
{
public:
    virtual ~NoteOnCallback() NN_NOEXCEPT {}

    virtual Channel* NoteOn(
        SequenceSoundPlayer* sequenceSoundPlayer,
        uint8_t bankIndex,
        const NoteOnInfo& noteOnInfo
    ) NN_NOEXCEPT = 0;
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

