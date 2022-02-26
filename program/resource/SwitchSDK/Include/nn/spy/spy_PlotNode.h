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

#include <nn/spy/detail/fnd/spyfnd_base.h>
#include <nn/spy/detail/spy_PacketUtility.h>
#include <nn/spy/detail/spy_PlotItemPacket.h>
#include <nn/spy/spy_PlotItem.h>
#include <nn/spy/spy_PlotModule.h>

namespace nn {
namespace spy {

//! @briefprivate
//!
//! @brief  (調整中の機能です)
//!         プロットアイテムをグループ化するクラスです。
class PlotNode : public PlotItem
{
#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

// public メソッドの空実装
public:
    NN_FORCEINLINE PlotNode() NN_NOEXCEPT
    {
    }

    NN_FORCEINLINE explicit PlotNode(const char* name) NN_NOEXCEPT
    {
        NN_UNUSED(name);
    }

#else // NN_BUILD_CONFIG_SPY_ENABLED

public:
    PlotNode() NN_NOEXCEPT;

    explicit PlotNode(const char* name) NN_NOEXCEPT;

    virtual ~PlotNode() NN_NOEXCEPT;

private:
    //! @brief 親ノードを設定します。
    //!
    //! @detail
    //! 現在は PlotNode に親ノードを設定することはできません。
    //!
    //! @param[in] pNode 親ノード
    //!
    virtual void SetParent(PlotNode* pNode) NN_NOEXCEPT NN_OVERRIDE;

    void InitializePacketCommonData() NN_NOEXCEPT;

    void PushDetachPacket() NN_NOEXCEPT;

    virtual void PushOwnMetaData() NN_NOEXCEPT NN_OVERRIDE;

private:
    nn::spy::detail::SpyPlotItemPacket::NodeMetadataPacketPayload m_MetadataPacketPayload;

#endif // NN_BUILD_CONFIG_SPY_ENABLED
};

} // namespace nn::spy
} // namespace nn
