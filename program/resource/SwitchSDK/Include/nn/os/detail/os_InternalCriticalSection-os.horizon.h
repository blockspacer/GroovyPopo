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

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>

namespace nn { namespace os { namespace detail {

class InternalConditionVariableImplByHorizon;
class InternalCriticalSectionImplByHorizon
{
public:
    // コンストラクタ
    InternalCriticalSectionImplByHorizon()  NN_NOEXCEPT;

    // メンバ関数
    void Initialize() NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT
    {
    }
    void Enter() NN_NOEXCEPT;
    bool TryEnter() NN_NOEXCEPT;
    void Leave() NN_NOEXCEPT;
    bool IsLockedByCurrentThread() const NN_NOEXCEPT;

    // std::lock_guard 用の定義
    void lock() NN_NOEXCEPT
    {
        Enter();
    }

    void unlock() NN_NOEXCEPT
    {
        Leave();
    }

private:
    friend InternalConditionVariableImplByHorizon;
    Bit32 m_ThreadHandle;
};
typedef InternalCriticalSectionImplByHorizon InternalCriticalSectionImpl;

}}} // namespace nn::os::detail

