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
#include <nn/g3d/g3d_Configuration.h>
#include <nn/nn_Macro.h>

// g3d ライブラリーの計測。
// マルチスレッドでの計測は非対応。
// 初期化と終了処理以外は PERF のマクロをそのまま呼び出している。
// アプリ側で別途 perf::LoadMeterCenter を初期化している場合は、NN_G3D_PERF_INITIALIZE による初期化は不要（perf::LoadMeterCenter はシングルトンクラスなため）。
// 内部では UserMeter を使用して計測するため、アプリ側で初期化する際は LoadMeterCenterInfo.SetUserMeterCount() で UserMeter を確保しておく必要がある。
#if NN_G3D_CONFIG_PERF_ENABLE

#define NN_PERF_PROFILE_ENABLED
#include <nn/perf.h>

#define NN_G3D_PERF_INITIALIZE(pAllocateFunction) nn::g3d::detail::G3dPerfManager::Initialize(pAllocateFunction)
#define NN_G3D_PERF_FINALIZE(pFreeFunction) nn::g3d::detail::G3dPerfManager::Finalize(pFreeFunction)
#define NN_G3D_PERF_RESET() NN_PERF_CLEAR()
#define NN_G3D_PERF_BEGIN_FRAME() NN_PERF_BEGIN_FRAME()
#define NN_G3D_PERF_END_FRAME() NN_PERF_END_FRAME_NO_CLEAR_PROFILE()
#define NN_G3D_PERF_PRINT() NN_PERF_DUMP()

#if NN_G3D_CONFIG_PERF_LEVEL >= 1
#define NN_G3D_PERF_LEVEL1(name) NN_PERF_AUTO_MEASURE_INDEX_NAME(0, name)
#endif

#if NN_G3D_CONFIG_PERF_LEVEL >= 2
#define NN_G3D_PERF_LEVEL2(name) NN_PERF_AUTO_MEASURE_INDEX_NAME(0, name)
#endif

#if NN_G3D_CONFIG_PERF_LEVEL >= 3
#define NN_G3D_PERF_LEVEL3(name) NN_PERF_AUTO_MEASURE_INDEX_NAME(0, name)
#endif

namespace nn
{
namespace g3d
{
namespace detail
{

// nn::perf::LoadMeterCenter の初期化と破棄を扱うクラス。
class G3dPerfManager
{
    NN_DISALLOW_COPY(G3dPerfManager);
public:
    static void Initialize(nn::AlignedAllocateFunction pAllocateFunction) NN_NOEXCEPT
    {
        // ユーザー計測用メーターを 1つ用意
        // 最大 2048 区間計測できるように設定
        nn::perf::LoadMeterCenterInfo info;
        info.SetCoreCount(0);
        info.SetUserMeterCount(1);
        info.SetCpuSectionCountMax(2048);
        info.SetCpuBufferCount(2);

        G3dPerfManager::GetInstance().m_MemorySize = NN_PERF_GET_BUFFER_SIZE(info);
        size_t memoryAlignment = NN_PERF_GET_BUFFER_ALIGNMENT();
        G3dPerfManager::GetInstance().m_pMemory = pAllocateFunction(G3dPerfManager::GetInstance().m_MemorySize, memoryAlignment);
        NN_PERF_INITIALIZE_METER_CPU_ONLY(info, G3dPerfManager::GetInstance().m_pMemory, G3dPerfManager::GetInstance().m_MemorySize);
    }

    static void Finalize(nn::DeallocateFunction pFreeFunction) NN_NOEXCEPT
    {
        NN_PERF_FINALIZE_METER_CPU_ONLY();
        pFreeFunction(G3dPerfManager::GetInstance().m_pMemory, G3dPerfManager::GetInstance().m_MemorySize);
    }

private:
    static G3dPerfManager& GetInstance() NN_NOEXCEPT
    {
        static G3dPerfManager m_Instance;
        return m_Instance;
    }

    G3dPerfManager() NN_NOEXCEPT
    {}

    void* m_pMemory;
    size_t m_MemorySize;
};

}}}

#else

#define NN_G3D_PERF_INITIALIZE(pAllocateFunction)
#define NN_G3D_PERF_FINALIZE(pFreeFunction)
#define NN_G3D_PERF_RESET()
#define NN_G3D_PERF_BEGIN_FRAME()
#define NN_G3D_PERF_END_FRAME()
#define NN_G3D_PERF_PRINT()

#define NN_G3D_PERF_LEVEL1(name)
#define NN_G3D_PERF_LEVEL2(name)
#define NN_G3D_PERF_LEVEL3(name)

#endif
