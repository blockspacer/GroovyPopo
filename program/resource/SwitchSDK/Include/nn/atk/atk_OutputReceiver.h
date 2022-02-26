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

#include <nn/audio.h>
#include <nn/nn_Macro.h>

namespace nn {
namespace atk {

//! @brief サブミックスなどの出力先を表すクラスです。
class OutputReceiver
{
public:
    //! @internal
    enum class ReceiverType
    {
        ReceiverType_SubMix,
        ReceiverType_FinalMix
    };

    //! @brief 設定できる最大のバス数です。
    static const int BusCountMax = nn::audio::MixBufferCountMax;

    //! @internal
    virtual ReceiverType GetReceiverType() const NN_NOEXCEPT = 0;
    //! @brief チャンネル数を取得します。
    //! @return チャンネル数です。
    virtual int GetChannelCount() const NN_NOEXCEPT = 0;
    //! @brief バス数を取得します。
    //! @return バス数です。
    virtual int GetBusCount() const NN_NOEXCEPT = 0;
    //! @internal
    virtual void AddReferenceCount(int value) NN_NOEXCEPT = 0;
    //! @internal
    virtual bool IsSoundSendClampEnabled(int bus) const NN_NOEXCEPT = 0;
};

} // namesapace nn::atk
} // namesapace nn
