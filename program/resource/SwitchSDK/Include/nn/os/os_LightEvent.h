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
 * @brief   軽量イベントに関する公開ヘッダファイル
 *
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_TimeSpan.h>
#include <nn/os/os_EventCommon.h>
#include <nn/os/os_LightEventTypes.h>
#include <nn/os/os_LightEventApi.h>


namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   軽量イベントを扱うためのクラスです。
 *
 * @details
 *  軽量イベントはスレッド間でフラグの通知を行なう同期機能です。
 *  軽量イベントは nn::os::Event に比べて多重待ちができない
 *  代わりに高速に動作する特徴を持っています。
 *
 *  nn::os::LightEvent クラスは nn::os::LightEventType オブジェクトを使用する
 *  同期制御 API の呼び出しをラッピングしたユーティリティクラスです。
 *
 *  本クラスが提供するメンバ関数は nn::os::LightEventType 型を引数にとる
 *  nn::os::WaitLightEvent() などの API を発行する形で実装されています。
 *
 */
class LightEvent
{
    NN_DISALLOW_COPY( LightEvent );
    NN_DISALLOW_MOVE( LightEvent );

public:
    /**
     * @brief   初期化を行なうコンストラクタです。
     *
     * @param[in] clearMode 軽量イベントのクリアモード
     *
     * @details
     *  指定されたパラメータで初期化を行なうコンストラクタです。
     *
     *  軽量イベントの初期状態は非シグナル状態となります。
     *  詳細は nn::os::InitializeLightEvent() を参照して下さい。
     *
     */
    explicit LightEvent(EventClearMode clearMode) NN_NOEXCEPT
    {
        nn::os::InitializeLightEvent(&m_LightEvent, false, clearMode);
    }

    /**
     * @brief  デストラクタです。
     *
     * @details
     *  軽量イベントオブジェクトを破棄します。
     *  詳細は nn::os::FinalizeLightEvent() を参照して下さい。
     *
     */
    ~LightEvent() NN_NOEXCEPT
    {
        nn::os::FinalizeLightEvent(&m_LightEvent);
    }

    /**
     * @brief   軽量イベントがシグナル状態になるまで待機します。
     *
     * @details
     *  軽量イベントがシグナル状態になるまで待機します。
     *
     *  軽量イベントが nn::os::EventClearMode_AutoClear の場合、
     *  本 API によるスレッド起床と共に対象イベントを自動的にクリアします。
     *  軽量イベントが nn::os::EventClearMode_ManualClear の場合、
     *  本 API によってスレッドが起床しても対象イベントはクリアされません。
     *
     *  詳細は nn::os::WaitLightEvent() を参照して下さい。
     *
     */
    void Wait() NN_NOEXCEPT
    {
        nn::os::WaitLightEvent(&m_LightEvent);
    }

    /**
     * @brief   軽量イベントがシグナル状態かをポーリングします。
     *
     * @return  軽量イベントのポーリング成否を返します。
     *
     * @details
     *  軽量イベントがシグナル状態かどうかをポーリングします。
     *  軽量イベントがシグナル状態なら true を、そうでなければ false を返します。@n
     *
     *  軽量イベントが nn::os::EventClearMode_AutoClear の場合、
     *  シグナル状態の検知と共に対象イベントを自動的にクリアします。
     *  軽量イベントが nn::os::EventClearMode_ManualClear の場合、
     *  本 API でシグナル状態を検知しても対象イベントはクリアされません。
     *
     *  詳細は nn::os::TryWaitLightEvent() を参照して下さい。
     *
     */
    bool TryWait() NN_NOEXCEPT
    {
        return nn::os::TryWaitLightEvent(&m_LightEvent);
    }

    /**
     * @brief   軽量イベントがシグナル状態になるまで時限付きで待機します。
     *
     * @param[in] timeout   最大待ち時間
     *
     * @return  軽量イベント待機の成否を返します。
     *
     * @details
     *  軽量イベントがシグナル状態になるまで時限付きで待機します。
     *  指定された timeout 時間までに軽量イベントがシグナル状態になれば true を、
     *  そうでなければ false を返します。
     *
     *  軽量イベントが nn::os::EventClearMode_AutoClear の場合、
     *  シグナル状態の検知と共に対象イベントを自動的にクリアします。
     *  軽量イベントが nn::os::EventClearMode_ManualClear の場合、
     *  本 API でシグナル状態を検知しても対象イベントはクリアされません。
     *
     *  詳細は nn::os::TimedWaitLightEvent() を参照して下さい。
     *
     */
    bool TimedWait(nn::TimeSpan timeout) NN_NOEXCEPT
    {
        return nn::os::TimedWaitLightEvent(&m_LightEvent, timeout);
    }

    /**
     * @brief   軽量イベントをシグナル状態にセットします。
     *
     * @details
     *  イベントをシグナル状態にセットします。
     *
     *  詳細は nn::os::SignalLightEvent() を参照して下さい。
     *
     */
    void Signal() NN_NOEXCEPT
    {
        nn::os::SignalLightEvent(&m_LightEvent);
    }

    /**
     * @brief   イベントを非シグナル状態にクリアします。
     *
     * @details
     *  イベントを非シグナル状態にクリアします。
     *
     *  詳細は nn::os::ClearLightEvent() を参照して下さい。
     *
     */
    void Clear() NN_NOEXCEPT
    {
        nn::os::ClearLightEvent(&m_LightEvent);
    }


    /**
     * @brief   LightEventType オブジェクトへの参照を返します。
     *
     * @return  LightEventType オブジェクトへの参照
     *
     * @details
     *  自身が持つ LightEventType オブジェクトへの参照を返します。
     *  この変換演算子があるため、LightEventType& を引数にとる関数に対して、
     *  LightEvent& を指定することが可能です。
     *
     */
    NN_IMPLICIT operator LightEventType&() NN_NOEXCEPT
    {
        return m_LightEvent;
    }

    /**
     * @brief   LightEventType オブジェクトへの const 参照を返します。
     *
     * @return  LightEventType オブジェクトへの const 参照
     *
     * @details
     *  自身が持つ LightEventType オブジェクトへの const 参照を返します。
     *  この変換演算子があるため、const LightEventType& を引数にとる
     *  関数に対して、LightEvent& を指定することが可能です。
     *
     */
    NN_IMPLICIT operator const LightEventType&() const NN_NOEXCEPT
    {
        return m_LightEvent;
    }

    /**
     * @brief   自身が持つ LightEventType オブジェクトへのポインタを返します。
     *
     * @return  LightEventType オブジェクトへのポインタ
     *
     * @details
     *  自身が持つ LightEventType オブジェクトへのポインタを返します。
     *
     */
    LightEventType* GetBase() NN_NOEXCEPT
    {
        return &m_LightEvent;
    }

private:
    LightEventType m_LightEvent;
};

//--------------------------------------------------------------------------

}} // namespace nn::os

