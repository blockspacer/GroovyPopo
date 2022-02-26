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

#ifdef NN_BUILD_CONFIG_SPY_ENABLED

#include <nn/nn_Macro.h>
#include <nn/nn_SdkAssert.h>

#include <nn/audio/audio_PerformanceMetrics.h>
#include <nn/os/os_Tick.h>
#include <nn/spy/spy_PlotFloat.h>
#include <nn/spy/spy_PlotModule.h>
#include <nn/spy/spy_PlotNode.h>
#include <nn/spy/spy_SpyController.h>

namespace nn {
namespace spy {
namespace audio {
namespace detail {

class PlotPerformanceItem
{
    NN_DISALLOW_COPY(PlotPerformanceItem);
    NN_DISALLOW_MOVE(PlotPerformanceItem);

public:
    PlotPerformanceItem() NN_NOEXCEPT;

    void Initialize(
        const char* name,
        uint8_t r,
        uint8_t g,
        uint8_t b) NN_NOEXCEPT;

    void Finalize() NN_NOEXCEPT;

    void AttachToModule(::nn::spy::PlotModule& module) NN_NOEXCEPT;

    bool IsAttachedToModule() const NN_NOEXCEPT
    {
        return m_pPlotModule != nullptr;
    }

    bool PushValueAt(int startTime, int processingTime, nn::os::Tick tick) NN_NOEXCEPT;

private:
    void DetachFromModule() NN_NOEXCEPT;

private:
    ::nn::spy::PlotModule* m_pPlotModule;
    ::nn::spy::PlotNode m_Node;
    ::nn::spy::PlotFloat m_StartTime;
    ::nn::spy::PlotFloat m_ProcessingTime;
};

class PlotPerformanceEntry
{
    NN_DISALLOW_COPY(PlotPerformanceEntry);
    NN_DISALLOW_MOVE(PlotPerformanceEntry);

public:
    PlotPerformanceEntry() NN_NOEXCEPT;

    void Initialize(
        const char* name,
        uint8_t r,
        uint8_t g,
        uint8_t b) NN_NOEXCEPT;

    void Finalize() NN_NOEXCEPT;

    void AttachToModule(::nn::spy::PlotModule& module) NN_NOEXCEPT
    {
        return m_PlotItem.AttachToModule(module);
    }

    bool IsAttachedToModule() const NN_NOEXCEPT
    {
        return m_PlotItem.IsAttachedToModule();
    }

    // 対応づけられた PerformanceEntry の id を取得します。
    ::nn::audio::NodeId GetNodeId() const NN_NOEXCEPT
    {
        return m_NodeId;
    }

    // 対応づける PerformanceEntry の id を設定します。
    void SetNodeId(::nn::audio::NodeId nodeId) NN_NOEXCEPT
    {
        m_NodeId = nodeId;
    }

    // 使用中であれば true を返します。
    bool IsActive() const NN_NOEXCEPT
    {
        return m_IsActive;
    }

    // 使用中かを設定します。
    void SetIsActive(bool value) NN_NOEXCEPT
    {
        m_IsActive = value;
    }

    // 値をプッシュしていれば true を返します。
    bool IsPushed() const NN_NOEXCEPT
    {
        return m_IsPushed;
    }

    // 値をプッシュしたかを設定します。
    void SetIsPushed(bool value) NN_NOEXCEPT
    {
        m_IsPushed = value;
    }

    // PerformanceEntry の内容をプッシュします。
    // IsPushed() は true に設定されます。
    bool PushValueAt(
        const ::nn::audio::PerformanceEntry* pPerformanceEntry,
        ::nn::os::Tick tick) NN_NOEXCEPT;

    // 指定した値をプッシュします。
    // IsPushed() の値は変わりません。
    bool PushValueAt(
        int startTime,
        int processingTime,
        ::nn::os::Tick tick) NN_NOEXCEPT;

private:
    PlotPerformanceItem m_PlotItem;
    ::nn::audio::NodeId m_NodeId;
    bool m_IsActive;
    bool m_IsPushed;
};

class PlotPerformanceSummary
{
    NN_DISALLOW_COPY(PlotPerformanceSummary);
    NN_DISALLOW_MOVE(PlotPerformanceSummary);

public:
    PlotPerformanceSummary() NN_NOEXCEPT;

    void Initialize(
        const char* name,
        uint8_t r,
        uint8_t g,
        uint8_t b,
        PlotPerformanceEntry* list,
        int count) NN_NOEXCEPT;

    void Finalize() NN_NOEXCEPT;

    void AttachToModule(::nn::spy::PlotModule& module) NN_NOEXCEPT
    {
        return m_PlotItem.AttachToModule(module);
    }

    bool IsAttachedToModule() const NN_NOEXCEPT
    {
        return m_PlotItem.IsAttachedToModule();
    }

    void PrepareForFrame() NN_NOEXCEPT;

    // 前フレームの続きの PerformanceEntry を送信します。
    void PushSucceedingPerformanceEntry(
        const ::nn::audio::PerformanceEntry& entry,
        ::nn::os::Tick tick) NN_NOEXCEPT;

    // 今フレームで現れた PerformanceEntry を送信します。
    void PushNewPerformanceEntry(
        const ::nn::audio::PerformanceEntry& entry,
        ::nn::os::Tick tick) NN_NOEXCEPT;

    // PerformanceFrame に情報の無かったアイテムについて無効値を送信し未使用状態に設定します。
    void PushValueForInactiveItem(::nn::os::Tick lastTick) NN_NOEXCEPT;

    // サマリーを送信します。
    bool PushSummaryAt(::nn::os::Tick tick) NN_NOEXCEPT;

    // アクティブなエントリー数を取得します。
    int GetActiveEntryCount() const NN_NOEXCEPT;

private:
    // 指定した nodeId を持つ Entry のインデックスを得ます。
    // 対応するエントリーが無いときは -1 を返します。
    int GetEntryIndex(::nn::audio::NodeId nodeId) NN_NOEXCEPT;

    // 未使用のエントリーのインデックスを得ます。
    // 未使用のエントリーが無いときは -1 を返します。
    int GetInactiveEntryIndex() NN_NOEXCEPT;

private:
    PlotPerformanceItem m_PlotItem;
    PlotPerformanceEntry* m_EntryList;
    int m_CountMax;
    int m_Count;
    int m_NewCount;
    int m_StartTime;
    int m_EndTime;
};

class PlotAudioPerformanceMetrics
{
    NN_DISALLOW_COPY(PlotAudioPerformanceMetrics);
    NN_DISALLOW_MOVE(PlotAudioPerformanceMetrics);

public:
    struct InitializeArg
    {
        InitializeArg() NN_NOEXCEPT
            : pAudioRendererParameter(nullptr)
        {}

        const nn::audio::AudioRendererParameter* pAudioRendererParameter;
    };

    static size_t GetRequiredMemorySize(const InitializeArg& args) NN_NOEXCEPT;

    PlotAudioPerformanceMetrics() NN_NOEXCEPT;

    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_IsInitialized;
    }

    bool Initialize(
        const InitializeArg& args,
        void* buffer,
        size_t bufferSize) NN_NOEXCEPT;

    void Finalize() NN_NOEXCEPT;

    void AttachToModules(::nn::spy::SpyController& spyController) NN_NOEXCEPT;

    void DetachFromModules() NN_NOEXCEPT;

    bool IsAttachedToModules() const NN_NOEXCEPT
    {
        return m_pSpyController != nullptr;
    }

    bool PushPerformanceMetrics(
        const void* performanceFrameBuffer,
        size_t performanceFrameBufferSize,
        ::nn::os::Tick tick) NN_NOEXCEPT;

private:
    void PrepareForFrame() NN_NOEXCEPT;

    // PerformanceFrame に情報の無かったアイテムについて無効値を送信し未使用状態に設定します。
    void PushValueForInactiveItem(::nn::os::Tick lastTick) NN_NOEXCEPT;

private:
    ::nn::spy::SpyController* m_pSpyController;
    PlotPerformanceEntry* m_pVoiceInfos;
    PlotPerformanceEntry* m_pSubMixInfos;
    PlotPerformanceEntry* m_pSinkInfos;
    PlotPerformanceSummary m_VoiceSummaryInfo;
    PlotPerformanceSummary m_SubMixSummaryInfo;
    PlotPerformanceSummary m_SinkSummaryInfo;
    PlotPerformanceEntry m_FinalMixInfo;
    ::nn::spy::PlotNode m_RootNode;
    ::nn::spy::PlotFloat m_TotalProcessingTime;
    ::nn::spy::PlotFloat m_VoiceCount;
    ::nn::spy::PlotFloat m_VoiceDropCount;
    ::nn::spy::PlotFloat m_IsRenderingTimeLimitExceeded;
    ::nn::os::Tick m_LastTick;
    int m_VoiceCountMax;
    int m_SubMixCountMax;
    int m_SinkCountMax;
    bool m_IsInitialized;
};

} // namespace detail
} // namespace audio
} // namespace spy
} // namespace nn

#endif
