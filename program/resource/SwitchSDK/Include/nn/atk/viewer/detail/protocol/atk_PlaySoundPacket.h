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
//! @brief  サウンドの再生を要求するパケットです。
//---------------------------------------------------------------------------
class PlaySoundPacket : public viewer::detail::HioPacket
{
public:
    class Body : public viewer::detail::HioPacketBody
    {
    public:
        static size_t GetRequiredSize(int maxItemName) NN_NOEXCEPT
        {
            return viewer::detail::ResName::GetRequiredSize(maxItemName);
        }

        uint32_t GetSoundHandleIndex() const NN_NOEXCEPT { return m_SoundHandleIndex; }

        const char* GetItemName() const NN_NOEXCEPT { return m_ItemName.str; }

        int GetItemNameLength() const NN_NOEXCEPT { return m_ItemName.len; }

    private:
        atk::detail::fnd::BinU32  m_SoundHandleIndex; //!< 対象サウンドハンドルのインデックスです。
        viewer::detail::ResNameData m_ItemName;         //!< 対象アイテム名です。
    };

public:
    PlaySoundPacket() NN_NOEXCEPT :
      viewer::detail::HioPacket(viewer::detail::HioMessageType_PlaySound, 0)
    {
    }

public:
    static size_t GetRequiredSize(int maxItemName) NN_NOEXCEPT
    {
        // 固定パケットサイズ + 名前の長さ
        return sizeof(HioPacket) + Body::GetRequiredSize(maxItemName);
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
