/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * @file
 * @brief   OS-API 内部実装用のクリティカルセクションの型宣言（Windows 版）
 */

#pragma once

#include <nn/os/os_Config.h>

#if !defined(NN_BUILD_CONFIG_OS_SUPPORTS_WIN32)
    #error  "OS 種別として Win32 が指定されていません。"
#endif

#include <nn/nn_Macro.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/os/os_Macro.h>
#include <nn/os/detail/os_MacroImpl.h>

//--------------------------------------------------------------------------

namespace nn { namespace os {
namespace detail {

//--------------------------------------------------------------------------
//  InternalCriticalSectionImpl の Windows 実装
//

//  Windows 用の前方宣言（グローバルスコープ）
class   InternalConditionVariableImplByWin32;
struct  CriticalSectionWin32;

typedef nn::util::TypedStorage<CriticalSectionWin32,NN_OS_DETAIL_CALC_OBJECT_SIZE(8,4),NN_ALIGNOF(void*)> CriticalSectionWin32Storage;

class   InternalCriticalSectionImplByWin32
{
    friend class    InternalConditionVariableImplByWin32;

public:
    // コンストラクタ と デストラクタ
    InternalCriticalSectionImplByWin32()    NN_NOEXCEPT;
    ~InternalCriticalSectionImplByWin32()   NN_NOEXCEPT;

    // メンバ関数
    void Initialize() NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;
    void    Enter()         NN_NOEXCEPT;
    bool    TryEnter()      NN_NOEXCEPT;
    void    Leave()         NN_NOEXCEPT;
    bool IsLockedByCurrentThread() const NN_NOEXCEPT;

    // std::lock_guard 用の定義
    void    lock()          NN_NOEXCEPT
    {
        Enter();
    }
    void    unlock()        NN_NOEXCEPT
    {
        Leave();
    }

private:
    // Windows のクリティカルセクション
    CriticalSectionWin32Storage     m_criticalSectionWin32;
};

typedef InternalCriticalSectionImplByWin32    InternalCriticalSectionImpl;

}   // namespace detail
}}  // namespace nn::os

