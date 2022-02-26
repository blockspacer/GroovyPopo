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
//!         浮動小数点数をプロットするクラスです。
class PlotFloat : public PlotItem
{
public:
    //! @brief  チャートの補間モードです。
    enum InterpolationMode
    {
        InterpolationMode_None,       //!< 補間なし
        InterpolationMode_Linear,     //!< 線形補間
    };

#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

// public メソッドの空実装
public:
    NN_FORCEINLINE PlotFloat() NN_NOEXCEPT { }

    NN_FORCEINLINE explicit PlotFloat(const char* name) NN_NOEXCEPT
    {
        NN_UNUSED( name );
    }

    NN_FORCEINLINE PlotFloat(const char* name, double minValue, double maxValue, uint8_t r, uint8_t g, uint8_t b) NN_NOEXCEPT
    {
        NN_UNUSED( name );
        NN_UNUSED( minValue );
        NN_UNUSED( maxValue );
        NN_UNUSED( r );
        NN_UNUSED( g );
        NN_UNUSED( b );
    }

    NN_FORCEINLINE void SetRange(double minValue, double maxValue) NN_NOEXCEPT
    {
        NN_UNUSED( minValue );
        NN_UNUSED( maxValue );
    }

    NN_FORCEINLINE void SetInterpolationMode(InterpolationMode mode) NN_NOEXCEPT
    {
        NN_UNUSED( mode );
    }

    NN_FORCEINLINE void PushValue(double value) NN_NOEXCEPT
    {
        NN_UNUSED( value );
    }

    NN_FORCEINLINE void PushValueAt(double value, nn::os::Tick tick) NN_NOEXCEPT
    {
        NN_UNUSED(value);
        NN_UNUSED(tick);
    }

#else // NN_BUILD_CONFIG_SPY_ENABLED

public:
    PlotFloat() NN_NOEXCEPT;

    explicit PlotFloat(const char* name) NN_NOEXCEPT;

    PlotFloat(const char* name, double minValue, double maxValue, uint8_t r, uint8_t g, uint8_t b) NN_NOEXCEPT;

    virtual ~PlotFloat() NN_NOEXCEPT;

    void SetRange(double minValue, double maxValue) NN_NOEXCEPT;

    void SetInterpolationMode(InterpolationMode mode) NN_NOEXCEPT;

    void PushValue(double value) NN_NOEXCEPT;

    void PushValueAt(double value, nn::os::Tick tick) NN_NOEXCEPT;

private:
    void InitializePacketCommonData() NN_NOEXCEPT;

    virtual void PushOwnMetaData() NN_NOEXCEPT NN_OVERRIDE;

private:
    nn::spy::detail::SpyPlotItemPacket::FloatMetadataPacketPayload m_MetadataPacketPayload;
    nn::spy::detail::SpyPlotItemPacket::FloatValuePacketPayload m_ValuePacketPayload;

#endif // NW_SND_SPY_ENABLE
};

} // namespace nn::spy
} // namespace nn
