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

#include <nn/spy/spy_SpyModule.h>

namespace nn {
namespace spy {

class PlotItem;

//! @briefprivate
//! @brief (調整中の機能です)
class PlotModule : public SpyModule
{
#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

public:
    NN_FORCEINLINE PlotModule() NN_NOEXCEPT {}

    NN_FORCEINLINE bool AttachItem(PlotItem& item) NN_NOEXCEPT
    {
        NN_UNUSED(&item);

        return false;
    }

    NN_FORCEINLINE void DetachItem(PlotItem& item) NN_NOEXCEPT
    {
        NN_UNUSED(&item);
    }

#else // NN_BUILD_CONFIG_SPY_ENABLED

public:
    PlotModule() NN_NOEXCEPT;

    bool AttachItem(PlotItem& item) NN_NOEXCEPT;
    void DetachItem(PlotItem& item) NN_NOEXCEPT;

protected:
    virtual void OnSessionStarted() NN_NOEXCEPT NN_OVERRIDE;
    virtual void OnRequested(bool isRequested) NN_NOEXCEPT NN_OVERRIDE;

private:
    bool PushData(const void* buffer, size_t length) NN_NOEXCEPT;
    bool PushDataAt(const void* buffer, size_t length, nn::os::Tick tick) NN_NOEXCEPT;

private:
    PlotItem* m_pItemTop;
    PlotItem* m_pItemLast;

    bool m_IsMetadataPushed;
    bool m_IsResetPushed;

    friend class PlotItem;
#endif // NN_BUILD_CONFIG_SPY_ENABLED
};

} // namespace nn::spy
} // namespace nn
