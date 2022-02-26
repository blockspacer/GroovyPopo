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

#include <nn/atk/atk_Config.h>
#ifdef NN_ATK_CONFIG_ENABLE_DEV

#include <nn/atk/viewer/detail/hio/atk_HioProtocol.h>
#include <nn/atk/viewer/detail/res/atk_ResTypes.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

//---------------------------------------------------------------------------
//! @internal
//! @brief  サウンド情報の更新を通知するパケットです。
//---------------------------------------------------------------------------
class UpdateSoundInfoPacket : public viewer::detail::HioPacket
{
public:
    static const uint32_t SoundCount = 4;    //!< サウンド数です。（暫定なので 4 固定）

    enum SoundState
    {
        SoundState_Disabled = 0,
        SoundState_Stopped = 1,
        SoundState_Playing = 2,
        SoundState_Paused = 3
    };

public:
    class Body : public viewer::detail::HioPacketBody
    {
    public:
        Body() NN_NOEXCEPT : m_Count(SoundCount)
        {
            for(auto i = 0; i < SoundCount; ++i)
            {
                m_States[i] = 0;
            }
        }

    public:
        void SetSoundState(uint32_t index, SoundState value) NN_NOEXCEPT
        {
            NN_SDK_ASSERT(index < SoundCount);
            m_States[index] = static_cast<uint32_t>(value);
        }

    private:
        atk::detail::fnd::BinU32 m_Count; //!< 対象サウンドハンドル数です。
        atk::detail::fnd::BinU32 m_States[SoundCount];
    };

public:
    UpdateSoundInfoPacket() NN_NOEXCEPT :
      HioPacket(viewer::detail::HioMessageType_UpdateSoundInfo, sizeof(Body))
    {
    }

public:
    static size_t GetRequiredSize() NN_NOEXCEPT
    {
        // 固定パケットサイズ
        return sizeof(UpdateSoundInfoPacket);
    }

    Body&       GetBody() NN_NOEXCEPT { return m_Body; }
    const Body& GetBody() const NN_NOEXCEPT { return m_Body; }

private:
    Body m_Body;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
