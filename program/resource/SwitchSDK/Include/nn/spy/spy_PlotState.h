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

#include <nn/os/os_Tick.h>

#include <nn/spy/detail/fnd/spyfnd_base.h>
#include <nn/spy/detail/spy_PlotItemPacket.h>
#include <nn/spy/spy_PlotItem.h>
#include <nn/spy/spy_PlotModule.h>

namespace nn {
namespace spy {

//! @briefprivate
//!
//! @brief  (調整中の機能です)
//!         状態をプロットするクラスです。
class PlotState : public PlotItem
{
public:
    //! 状態の文字列長の最大値。
    static const int MaxStateValueLength = nn::spy::detail::SpyPlotItemPacket::MaxStateValueLength;

#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

// public メソッドの空実装
public:
    NN_FORCEINLINE PlotState() NN_NOEXCEPT { }

    NN_FORCEINLINE explicit PlotState(const char* name) NN_NOEXCEPT
    {
        NN_UNUSED(name);
    }

    NN_FORCEINLINE PlotState(const char* name, uint8_t r, uint8_t g, uint8_t b) NN_NOEXCEPT
    {
        NN_UNUSED(name );
        NN_UNUSED(r);
        NN_UNUSED(g);
        NN_UNUSED(b);
    }

    NN_FORCEINLINE void PushValue(const char* state) NN_NOEXCEPT
    {
        NN_UNUSED(state);
    }

    NN_FORCEINLINE void PushValue(const char* state, uint8_t r, uint8_t g, uint8_t b) NN_NOEXCEPT
    {
        NN_UNUSED(state);
        NN_UNUSED(r);
        NN_UNUSED(g);
        NN_UNUSED(b);
    }

    NN_FORCEINLINE void PushValueAt(const char* state, nn::os::Tick tick) NN_NOEXCEPT
    {
        NN_UNUSED(state);
        NN_UNUSED(tick);
    }

    NN_FORCEINLINE void PushValueAt(const char* state, uint8_t r, uint8_t g, uint8_t b, nn::os::Tick tick) NN_NOEXCEPT
    {
        NN_UNUSED(state);
        NN_UNUSED(r);
        NN_UNUSED(g);
        NN_UNUSED(b);
        NN_UNUSED(tick);
    }

#else

public:
    PlotState() NN_NOEXCEPT;

    explicit PlotState(const char* name) NN_NOEXCEPT;

    PlotState(const char* name, uint8_t r, uint8_t g, uint8_t b) NN_NOEXCEPT;

    virtual ~PlotState() NN_NOEXCEPT;

    void PushValue(const char* state) NN_NOEXCEPT;

    void PushValue(const char* state, uint8_t r, uint8_t g, uint8_t b) NN_NOEXCEPT;

    void PushValueAt(const char* state, nn::os::Tick tick) NN_NOEXCEPT;

    void PushValueAt(const char* state, uint8_t r, uint8_t g, uint8_t b, nn::os::Tick tick) NN_NOEXCEPT;

private:
    void InitializePacketCommonData() NN_NOEXCEPT;

    virtual void PushOwnMetaData() NN_NOEXCEPT NN_OVERRIDE;

private:
    nn::spy::detail::SpyPlotItemPacket::StateMetadataPacketPayload m_MetadataPacketPayload;
    nn::spy::detail::SpyPlotItemPacket::StateValuePacketPayload m_ValuePacketPayload;

#endif // NN_BUILD_CONFIG_SPY_ENABLED
};

} // namespace nn::spy
} // namespace nn
