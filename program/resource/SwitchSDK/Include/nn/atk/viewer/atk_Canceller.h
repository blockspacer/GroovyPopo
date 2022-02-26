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

#include <nn/atk/atk_Config.h>
#ifdef NN_ATK_CONFIG_ENABLE_DEV

#include <nn/atk/fnd/basis/atkfnd_Time.h>

namespace nn {
namespace atk {
namespace viewer {

//---------------------------------------------------------------------------
//! @brief  【β版】キャンセルをサポートするインターフェイスです。
//---------------------------------------------------------------------------
class ICanceller
{
public:
    //! @internal
    virtual ~ICanceller() NN_NOEXCEPT { }

public:
    //---------------------------------------------------------------------------
    //! @brief      キャンセルの有無を取得します。
    //!
    //!             対象の処理が完了するか、この関数が true を返すまで、処理が続行されます。
    //!
    //! @param[in]  param   ユーザーパラメータを指定します。
    //!
    //! @return     true を返すと、対象をキャンセルします。
    //!             @n
    //!             false を返すと、処理を続行します。
    //---------------------------------------------------------------------------
    virtual bool IsCanceled(void* param) NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief      キャンセルオブジェクトの状態をリセットします。
    //!
    //!             主に処理開始前に呼び出され、
    //!             タイムアウト計算用の基準値を設定するため等の目的に利用します。
    //!
    //! @param[in]  param   ユーザーパラメータを指定します。
    //---------------------------------------------------------------------------
    virtual void Reset(void* param) NN_NOEXCEPT = 0;
};

//---------------------------------------------------------------------------
//! @brief  【β版】キャンセルオブジェクトです。
//---------------------------------------------------------------------------
class Canceller : public ICanceller
{
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //---------------------------------------------------------------------------
    Canceller() NN_NOEXCEPT : m_IsCanceled(false) { }

public:
    //---------------------------------------------------------------------------
    //! @brief      キャンセルの有無を取得します。
    //!
    //!             対象の処理が完了するか、この関数が true を返すまで、処理が続行されます。
    //!
    //! @param[in]  param   ユーザーパラメータを指定します。
    //!
    //! @return     Cancel() を呼び出す前は false、呼び出した後は true を返します。
    //---------------------------------------------------------------------------
    virtual bool IsCanceled(void* param) NN_NOEXCEPT NN_OVERRIDE { (void)param; return m_IsCanceled; }

    //---------------------------------------------------------------------------
    //! @brief  キャンセルオブジェクトのリセット実装ですが、ここでは何もしません。
    //!
    //! @param[in]  param   ユーザーパラメータを指定します。
    //---------------------------------------------------------------------------
    virtual void Reset(void* param) NN_NOEXCEPT NN_OVERRIDE { (void)param; m_IsCanceled = false; }

    //---------------------------------------------------------------------------
    //! @brief  キャンセルオブジェクトをキャンセル状態にします。
    //!
    //!         次回の IsCanceled() から true を返すようになります。
    //---------------------------------------------------------------------------
    void Cancel() NN_NOEXCEPT { m_IsCanceled = true; }

private:
    bool m_IsCanceled;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t m_Padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
};

//---------------------------------------------------------------------------
//! @brief  【β版】タイムアウトでキャンセルするキャンセルオブジェクトです。
//---------------------------------------------------------------------------
class TimeoutCanceller : public ICanceller
{
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //!
    //!         このコンストラクタを使用する場合、別途 Start() を呼び出して、
    //!         タイマを開始する必要があります。
    //---------------------------------------------------------------------------
    TimeoutCanceller() NN_NOEXCEPT : m_Timeout(0)
    {
    }

    //---------------------------------------------------------------------------
    //! @brief      コンストラクタです。
    //!
    //!             このコンストラクタを使用すると、指定タイムアウト値でタイマを開始します。
    //!             @n
    //!             別途 Start() を呼び出す必要はありません。
    //!
    //! @param[in]  timeoutMsec  タイムアウト値（ミリ秒）を指定します。
    //---------------------------------------------------------------------------
    explicit TimeoutCanceller(uint32_t timeoutMsec) NN_NOEXCEPT : m_Timeout(timeoutMsec)
    {
        Start(timeoutMsec);
    }

public:
    //---------------------------------------------------------------------------
    //! @brief      タイマを開始します。
    //!
    //! @param[in]  timeoutMsec  タイムアウト値（ミリ秒）を指定します。
    //---------------------------------------------------------------------------
    void Start(uint32_t timeoutMsec) NN_NOEXCEPT
    {
        if(m_StopWatch.IsStarted())
        {
            return;
        }

        m_Timeout = timeoutMsec;
        m_StopWatch.Start();
    }

    //---------------------------------------------------------------------------
    //! @brief      タイマを停止します。
    //---------------------------------------------------------------------------
    void Stop() NN_NOEXCEPT
    {
        m_StopWatch.Start();
    }

    //---------------------------------------------------------------------------
    //! @brief  タイマをリセットします。
    //---------------------------------------------------------------------------
    void Reset() NN_NOEXCEPT
    {
        m_StopWatch.Reset();
    }

    //---------------------------------------------------------------------------
    //! @brief      キャンセルの有無を取得します。
    //!
    //!             対象の処理が完了するか、この関数が true を返すまで、処理が続行されます。
    //!
    //! @param[in]  param   ユーザーパラメータを指定します。
    //!
    //! @return     true を返すと、対象をキャンセルします。
    //!             @n
    //!             false を返すと、処理を続行します。
    //---------------------------------------------------------------------------
    virtual bool IsCanceled(void* param) NN_NOEXCEPT NN_OVERRIDE
    {
        (void)param;
        return m_StopWatch.GetElapsedTime().ToMilliSeconds() > m_Timeout;
    }

    //---------------------------------------------------------------------------
    //! @brief      キャンセルオブジェクトの状態をリセットします。
    //!
    //!             主に処理開始前に呼び出され、
    //!             タイムアウト計算用の基準値を設定するため等の目的に利用します。
    //!
    //! @param[in]  param   ユーザーパラメータを指定します。
    //---------------------------------------------------------------------------
    virtual void Reset(void* param) NN_NOEXCEPT NN_OVERRIDE
    {
        (void)param;
        Reset();
    }

private:
    atk::detail::fnd::StopWatch m_StopWatch;
    uint32_t                           m_Timeout;
};

} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
