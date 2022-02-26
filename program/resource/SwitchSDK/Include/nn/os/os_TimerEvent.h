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
 * @brief   タイマーイベントに関する公開ヘッダファイル
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_TimeSpan.h>
#include <nn/os/os_TimerEventTypes.h>
#include <nn/os/os_TimerEventApi.h>

namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   タイマーイベントを扱うためのクラスです。
 *
 * @details
 *  タイマーイベントは、通常のイベント機能に加え、指定した時間だけ遅延して
 *  シグナル化させることのできる機能を備えた同期機能です。
 *
 *  nn::os::TimerEvent クラスは、nn::os::TimerEventType オブジェクトを使用する
 *  同期制御 API の呼び出しをラッピングしたユーティリティクラスです。@n
 *  本クラスが提供するメンバ関数は、@ref nn::os::TimerEventType 型を引数にとる
 *  nn::os::WaitTimerEvent() などの API を発行する形で実装されています。
 */
class TimerEvent
{
    NN_DISALLOW_COPY( TimerEvent );
    NN_DISALLOW_MOVE( TimerEvent );

public:
    /**
     * @brief   タイマーイベントオブジェクトを構築し初期化します。
     *
     * @param[in] clearMode タイマーイベントのクリアモード
     *
     * @details
     *  コンストラクタです。@n
     *  タイマーイベントオブジェクトを構築し、指定されたパラメータで初期化します。@n
     *  タイマーイベントの初期状態は非シグナル状態となります。@n
     *  詳細は nn::os::InitializeTimerEvent() を参照して下さい。
     */
    explicit TimerEvent(EventClearMode clearMode) NN_NOEXCEPT
    {
        InitializeTimerEvent(&m_TimerEvent, clearMode);
    }

    /**
     * @brief  デストラクタです。
     *
     * @details
     *  タイマーイベントオブジェクトを破棄します。@n
     *  詳細は nn::os::FinalizeTimerEvent() を参照して下さい。
     */
    ~TimerEvent() NN_NOEXCEPT
    {
        FinalizeTimerEvent(&m_TimerEvent);
    }

    /**
     * @brief   タイマーイベントのワンショットタイマー動作を開始します。
     *
     * @param[in] first     最初のタイマー通知までの時間
     *
     * @details
     *  タイマーイベントのワンショットタイマー動作を開始します。@n
     *  first には、タイマーイベントがシグナル化されるまでの時間を指定します。@n
     *  既にタイマー動作が開始されていた場合、そのタイマーは一旦停止させられ、
     *  新しいワンショットタイマーとして動作を開始します。@n
     *  詳細は nn::os::StartOneShotTimerEvent() を参照して下さい。
     */
    void StartOneShot(nn::TimeSpan first) NN_NOEXCEPT
    {
        StartOneShotTimerEvent(&m_TimerEvent, first);
    }

    /**
     * @brief   タイマーイベントの周期タイマー動作を開始します。
     *
     * @param[in] first     最初のタイマー通知までの時間
     * @param[in] interval  2 度目以降のタイマー通知の間隔
     *
     * @details
     *  タイマーイベントの周期タイマー動作を開始します。@n
     *  first には、タイマーイベントがシグナル化されるまでの時間を指定し、
     *  interval には 2 度目以降のタイマー通知の間隔を指定します。@n
     *  既にタイマー動作が開始されていた場合、そのタイマーは一旦停止させられ、
     *  新しい周期タイマーとして動作を開始します。@n
     *  詳細は nn::os::StartPeriodicTimerEvent() を参照して下さい。
     */
    void StartPeriodic(nn::TimeSpan first, nn::TimeSpan interval) NN_NOEXCEPT
    {
        StartPeriodicTimerEvent(&m_TimerEvent, first, interval);
    }

    /**
     * @brief   タイマーイベントのタイマー動作を停止します。
     *
     * @details
     *  タイマーイベントのタイマー動作を停止します。@n
     *  本 API によってタイマーイベントのシグナル状態が変化することは
     *  ありません。また、待機スレッドがあっても、そのスレッドが本 API に
     *  よって起床されることもありません。@n
     *  詳細は nn::os::StopTimerEvent() を参照して下さい。
     */
    void Stop() NN_NOEXCEPT
    {
        StopTimerEvent(&m_TimerEvent);
    }

    /**
     * @brief   タイマーイベントがシグナル状態になるまで待機します。
     *
     * @details
     *  タイマーイベントがシグナル状態になるまで待機します。@n
     *  タイマーイベントが nn::os::EventClearMode_AutoClear の場合、シグナル状態を
     *  検知すると同時にタイマーイベントは非シグナル状態にクリアされます。@n
     *  詳細は nn::os::WaitTimerEvent() を参照して下さい。@n
     */
    void Wait() NN_NOEXCEPT
    {
        WaitTimerEvent(&m_TimerEvent);
    }

    /**
     * @brief   タイマーイベントがシグナル状態かをポーリングします。
     *
     * @return  タイマーイベントのポーリング成否を返します。
     *
     * @details
     *  タイマーイベントがシグナル状態かどうかをポーリングします。@n
     *  タイマーイベントがシグナル状態なら true を、そうでなければ false を
     *  返します。@n
     *  タイマーイベントが nn::os::EventClearMode_AutoClear の場合、シグナル状態を
     *  検知すると同時にタイマーイベントは非シグナル状態にクリアされます。@n
     *  詳細は nn::os::TryWaitTimerEvent() を参照して下さい。
     */
    bool TryWait() NN_NOEXCEPT
    {
        return TryWaitTimerEvent(&m_TimerEvent);
    }

    /**
     * @brief   タイマーイベントをシグナル状態にセットします。
     *
     * @details
     *  タイマーイベントをシグナル状態にセットします。@n
     *  詳細は nn::os::SignalTimerEvent() を参照して下さい。
     */
    void Signal() NN_NOEXCEPT
    {
        SignalTimerEvent(&m_TimerEvent);
    }

    /**
     * @brief   タイマーイベントを非シグナル状態にクリアします。
     *
     * @details
     *  タイマーイベントを非シグナル状態にクリアします。@n
     *  詳細は nn::os::ClearTimerEvent() を参照して下さい。
     */
    void Clear() NN_NOEXCEPT
    {
        ClearTimerEvent(&m_TimerEvent);
    }


    /**
     * @brief   TimerEventType オブジェクトへの参照を返します。
     *
     * @return  TimerEventType オブジェクトへの参照
     *
     * @details
     *  自インスタンスが持つ TimerEventType オブジェクトへの参照を返します。
     *  この変換演算子があるため、TimerEventType& を引数にとる関数に対して、
     *  TimerEvent& を指定することが可能です。
     */
    NN_IMPLICIT operator TimerEventType&() NN_NOEXCEPT
    {
        return m_TimerEvent;
    }

    /**
     * @brief   TimerEventType オブジェクトへの const 参照を返します。
     *
     * @return  TimerEventType オブジェクトへの const 参照
     *
     * @details
     *  自インスタンスが持つ TimerEventType オブジェクトへの const 参照を返します。
     *  この変換演算子があるため、const TimerEventType& を引数にとる関数に対して、
     *  TimerEvent& を指定することが可能です。
     */
    NN_IMPLICIT operator const TimerEventType&() const NN_NOEXCEPT
    {
        return m_TimerEvent;
    }

    /**
     * @brief   自インスタンスの TimerEventType オブジェクトへのポインタを返します。
     *
     * @return  TimerEventType オブジェクトへのポインタ
     *
     * @details
     *  自インスタンスが持つ TimerEventType オブジェクトへのポインタを返します。
     */
    TimerEventType* GetBase() NN_NOEXCEPT
    {
        return &m_TimerEvent;
    }

private:
    TimerEventType  m_TimerEvent;
};

//--------------------------------------------------------------------------

}} // namespace nn::os

