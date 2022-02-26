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
#include <nn/spy/spy_SpyModule.h>
#include <nn/spy/spy_PlotFloat.h>

namespace nn {
namespace spy {

class SpyController;

//! @brief  Spy に nn::spy 自身のデバッグ情報を送信するモジュールです。
class DebugModule : public SpyModule
{
    typedef ::nn::spy::SpyModule Base;

#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

public:
    NN_FORCEINLINE DebugModule() NN_NOEXCEPT {}

    NN_FORCEINLINE bool PushDataBufferUsage() NN_NOEXCEPT
    {
        return false;
    }

#else // NN_BUILD_CONFIG_SPY_ENABLED

public:
    //! @brief  コンストラクタです。
    DebugModule() NN_NOEXCEPT;

    //! @brief  データバッファの使用量を出力します。
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    bool PushDataBufferUsage() NN_NOEXCEPT;

protected:
    virtual void OnRegistered() NN_NOEXCEPT NN_OVERRIDE;
    virtual void OnUnregistered() NN_NOEXCEPT NN_OVERRIDE;
    virtual void OnSessionStarted() NN_NOEXCEPT NN_OVERRIDE;

private:
    //! @brief  データバッファの長さを出力します。
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    void PushDataBufferLength() NN_NOEXCEPT;

private:
    nn::spy::PlotFloat m_DataBufferLength;
    nn::spy::PlotFloat m_DataBufferUsage;
    bool m_IsDataBufferLengthSent;
#endif // NN_BUILD_CONFIG_SPY_ENABLED
};

} // namespace nn::spy
} // namespace nn
