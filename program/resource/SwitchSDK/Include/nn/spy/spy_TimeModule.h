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

#include <atomic>

#include <nn/nn_SdkAssert.h>
#include <nn/os/os_Tick.h>

#include <nn/spy/spy_Config.h>
#include <nn/spy/spy_SpyModule.h>
#include <nn/spy/detail/fnd/os/spyfnd_CriticalSection.h>

namespace nn {
namespace spy {

//! @brief  Spy の時間を管理するモジュールです。
class TimeModule : public SpyModule
{

#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

public:
    NN_FORCEINLINE TimeModule() NN_NOEXCEPT {};

    NN_FORCEINLINE uint32_t GetCurrentApplicationFrame() const NN_NOEXCEPT
    {
        return 0;
    }

    NN_FORCEINLINE void SetCurrentApplicationFrame(uint32_t value) NN_NOEXCEPT
    {
        NN_UNUSED(value);
    }

    NN_FORCEINLINE uint32_t GetCurrentAudioFrame() const NN_NOEXCEPT
    {
        return 0;
    }

    NN_FORCEINLINE void SetCurrentAudioFrame(uint32_t value) NN_NOEXCEPT
    {
        NN_UNUSED(value);
    }

#else // NN_BUILD_CONFIG_SPY_ENABLED

public:
    //! @brief  コンストラクタです。
    TimeModule() NN_NOEXCEPT;

    //! @brief  現在のアプリケーションフレームを取得します。
    //!
    //! @return  現在のアプリケーションフレームを返します。
    //!
    //! @details
    //! SetCurrentApplicationFrame() で設定した値が返ります。
    //!
    //! @sa SetCurrentApplicationFrame
    int GetCurrentApplicationFrame() const NN_NOEXCEPT
    {
        return m_CurrentApplicationFrame;
    }

    //! @brief  現在のアプリケーションフレームを設定します。
    //!
    //! @param[in]  value  現在のアプリケーションフレーム値を指定します。
    //!
    //! @details
    //! nn::spy が送信するデータには、この関数で設定したアプリケーションフレームが付加されます。
    //! アプリケーションフレームを設定しておくと、Spy.exe のタイムラインにてアプリケーションフレーム単位のグリッド表示が可能になります。
    //! この関数は、できるだけアプリケーションフレーム処理の先頭で呼び出すようにしてください。
    //!
    //! @sa GetCurrentApplicationFrame
    void SetCurrentApplicationFrame(int value) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief  現在のオーディオフレームを取得します。（この関数は使用しないでください。）
    //!
    //! @return  現在のオーディオフレームを返します。
    //!
    //! @details
    //! この関数は、互換性維持のために残されています。
    //! アプリケーションからは呼び出さないでください。
    int GetCurrentAudioFrame() const NN_NOEXCEPT
    {
        return m_CurrentAudioFrame;
    }

    //! @briefprivate
    //! @brief 現在のオーディオフレームを設定します。（この関数は使用しないでください。）
    //!
    //! @param[in]  value  現在のオーディオフレーム値を指定します。
    //!
    //! @details
    //! この関数は、互換性維持のために残されています。
    //! アプリケーションからは呼び出さないでください。
    void SetCurrentAudioFrame(int value) NN_NOEXCEPT;

private:
    //! @briefprivate
    //! @brief 現在の時間データを送信します。
    void PushCurrentTimeData(nn::os::Tick tick, int appFrame, int audioFrame) NN_NOEXCEPT;

private:
    detail::fnd::CriticalSection m_Mutex;
    std::atomic<int> m_CurrentApplicationFrame;
    std::atomic<int> m_CurrentAudioFrame;

#endif // NN_BUILD_CONFIG_SPY_ENABLED
};

} // namespace nn::spy
} // namespace nn
