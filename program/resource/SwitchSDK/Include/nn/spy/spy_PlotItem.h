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
#include <nn/spy/spy_PlotModule.h>

namespace nn {
namespace spy {

class PlotNode;

//! @briefprivate
//!
//! @brief  (調整中の機能です)
//!         Spy プロットアイテムを制御します。
class PlotItem
{
    NN_DISALLOW_COPY(PlotItem);
    NN_DISALLOW_MOVE(PlotItem);

public:
    //! プロットアイテム名の文字列長の最大値。
    static const int MaxNameLength = nn::spy::detail::SpyPlotItemPacket::MaxNameLength;

#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

protected:
    NN_FORCEINLINE PlotItem() NN_NOEXCEPT { }

public:
    NN_FORCEINLINE bool IsAttached() const NN_NOEXCEPT { return false; }

    NN_FORCEINLINE const char* GetName() const NN_NOEXCEPT { return ""; }

    NN_FORCEINLINE void SetName(const char* name) NN_NOEXCEPT
    {
        NN_UNUSED( name );
    }

    NN_FORCEINLINE void SetColor(uint8_t r, uint8_t g, uint8_t b) NN_NOEXCEPT
    {
        NN_UNUSED( r );
        NN_UNUSED( g );
        NN_UNUSED( b );
    }

    NN_FORCEINLINE PlotNode* GetParent() const NN_NOEXCEPT { return NULL; }

    NN_FORCEINLINE void SetParent(PlotNode* pNode) NN_NOEXCEPT
    {
        NN_UNUSED( pNode );
    }

#else

protected:
    PlotItem() NN_NOEXCEPT;

public:
    virtual ~PlotItem() NN_NOEXCEPT;

    bool IsAttached() const NN_NOEXCEPT { return m_pModule != NULL; }

    const char* GetName() const NN_NOEXCEPT;

    void SetName(const char* name) NN_NOEXCEPT;

    void SetColor(uint8_t r, uint8_t g, uint8_t b) NN_NOEXCEPT;

    //! @brief 親ノードを取得します。
    //!
    //! @return 親ノードへのポインタ、または NULL を返します。
    //!
    PlotNode* GetParent() const NN_NOEXCEPT { return m_pParent; }

    //! @brief 親ノードを設定します。
    //!
    //! @param[in] pNode 親ノード
    //!
    virtual void SetParent(PlotNode* pNode) NN_NOEXCEPT;

protected:
    void InitializeItemMetaData(nn::spy::detail::SpyPlotItemPacket::ItemMetaData* pItemMetaData) NN_NOEXCEPT;

    bool IsRequested() const NN_NOEXCEPT
    {
        return IsAttached() && m_pModule->IsRequested();
    }

    PlotItem* GetPrevious() const NN_NOEXCEPT { return m_pPrevious; }

    PlotItem* GetNext() const NN_NOEXCEPT { return m_pNext; }

    PlotModule* GetModule() const NN_NOEXCEPT { return m_pModule; }

    bool IsMetadataPushed() const NN_NOEXCEPT { return m_IsMetadataPushed; }

    void InvalidateMetadata() NN_NOEXCEPT { m_IsMetadataPushed = false; }

    void PushMetaData() NN_NOEXCEPT;

    bool PushData(const void* buffer, size_t length) NN_NOEXCEPT;

    template <typename T>
    bool PushData(const T& data) NN_NOEXCEPT
    {
        return PushData(&data, sizeof(T));
    }

    bool PushDataAt(const void* buffer, size_t length, nn::os::Tick tick) NN_NOEXCEPT;

    template <typename T>
    bool PushDataAt(const T& data, nn::os::Tick tick) NN_NOEXCEPT
    {
        return PushDataAt(&data, sizeof(T), tick);
    }

    //! @brief プロットタイミング (Tick) がさかのぼっていないかチェックします。
    //!
    //! @param[in] tick 評価する Tick 値です。
    //!
    void CheckTick(nn::os::Tick tick) NN_NOEXCEPT;

private:
    virtual void PushOwnMetaData() NN_NOEXCEPT {}

    void SetPrevious(PlotItem* pPrevious) NN_NOEXCEPT { m_pPrevious = pPrevious; }

    void SetNext(PlotItem* pNext) NN_NOEXCEPT { m_pNext = pNext; }

    void Attach(PlotModule& module) NN_NOEXCEPT;

    void Detach() NN_NOEXCEPT;

    nn::spy::detail::SpyPlotItemPacket::ItemMetaData& GetItemMetaData() NN_NOEXCEPT;

    const nn::spy::detail::SpyPlotItemPacket::ItemMetaData& GetItemMetaData() const NN_NOEXCEPT;

private:
    PlotModule* m_pModule;

    PlotNode* m_pParent;

    PlotItem* m_pPrevious;
    PlotItem* m_pNext;

    nn::spy::detail::SpyPlotItemPacket::ItemMetaData* m_pItemMetaData;

    nn::os::Tick m_LastTick;

    bool m_IsMetadataPushed;

    bool m_IsCheckTickEnabled;

    friend class PlotModule;

#endif // NW_SND_SPY_ENABLE
};

} // namespace nn::spy
} // namespace nn
