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
 * @brief   イベントに関する公開ヘッダファイル
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_TimeSpan.h>
#include <nn/os/os_EventCommon.h>
#include <nn/os/os_EventTypes.h>
#include <nn/os/os_EventApi.h>


namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   イベントを扱うためのクラスです。
 *
 * @details
 *  イベントはスレッド間で同期を確立するための機能です。
 *
 *  nn::os::Event クラスは、@ref nn::os::EventType オブジェクトを使用する
 *  同期制御 API の呼び出しをラッピングしたユーティリティクラスです。@n
 *  本クラスが提供するメンバ関数は、@ref nn::os::EventType 型を引数にとる
 *  @ref nn::os::WaitEvent() などの API を発行する形で実装されています。
 */
class Event
{
    NN_DISALLOW_COPY( Event );
    NN_DISALLOW_MOVE( Event );

public:
    /**
     * @brief   イベントオブジェクトを構築し初期化します。
     *
     * @param[in] clearMode イベントのクリアモード
     *
     * @details
     *  コンストラクタです。@n
     *  イベントオブジェクトを構築し、指定されたパラメータで初期化します。@n
     *  イベントの初期状態は非シグナル状態となります。@n
     *  詳細は nn::os::InitializeEvent() を参照して下さい。
     */
    explicit Event(EventClearMode clearMode) NN_NOEXCEPT
    {
        nn::os::InitializeEvent(&m_Event, false, clearMode);
    }

    /**
     * @brief  デストラクタです。
     *
     * @details
     *  イベントオブジェクトを破棄します。@n
     *  詳細は nn::os::FinalizeEvent() を参照して下さい。
     */
    ~Event() NN_NOEXCEPT
    {
        nn::os::FinalizeEvent(&m_Event);
    }

    /**
     * @brief   イベントがシグナル状態になるまで待機します。
     *
     * @details
     *  イベントがシグナル状態になるまで待機します。@n
     *  イベントが nn::os::EventClearMode_AutoClear の場合、シグナル状態を検知すると
     *  同時にイベントは非シグナル状態にクリアされます。@n
     *  詳細は nn::os::WaitEvent() を参照して下さい。
     */
    void Wait() NN_NOEXCEPT
    {
        nn::os::WaitEvent(&m_Event);
    }

    /**
     * @brief   イベントがシグナル状態かをポーリングします。
     *
     * @return  イベントのポーリング成否を返します。
     *
     * @details
     *  イベントがシグナル状態かどうかをポーリングします。@n
     *  イベントがシグナル状態なら true を、そうでなければ false を返します。@n
     *  イベントが nn::os::EventClearMode_AutoClear の場合、シグナル状態を検知すると
     *  同時にイベントは非シグナル状態にクリアされます。@n
     *  詳細は nn::os::TryWaitEvent() を参照して下さい。
     */
    bool TryWait() NN_NOEXCEPT
    {
        return nn::os::TryWaitEvent(&m_Event);
    }

    /**
     * @brief   イベントがシグナル状態になるまで時限付きで待機します。
     *
     * @param[in] timeout   最大待ち時間
     *
     * @return  イベント待機の成否を返します。
     *
     * @details
     *  イベントをシグナル状態になるまで時限付きで待機します。@n
     *  指定された timeout 時間までにイベントがシグナル状態になれば true を、
     *  そうでなければ false を返します。@n
     *  イベントが nn::os::EventClearMode_AutoClear の場合、シグナル状態を検知すると
     *  同時にイベントは非シグナル状態にクリアされます。@n
     *  詳細は nn::os::TimedWaitEvent() を参照して下さい。
     */
    bool TimedWait(nn::TimeSpan timeout) NN_NOEXCEPT
    {
        return nn::os::TimedWaitEvent(&m_Event, timeout);
    }

    /**
     * @brief   イベントをシグナル状態にセットします。
     *
     * @details
     *  イベントをシグナル状態にセットします。@n
     *  詳細は nn::os::SignalEvent() を参照して下さい。
     */
    void Signal() NN_NOEXCEPT
    {
        nn::os::SignalEvent(&m_Event);
    }

    /**
     * @brief   イベントを非シグナル状態にクリアします。
     *
     * @details
     *  イベントを非シグナル状態にクリアします。@n
     *  詳細は nn::os::ClearEvent() を参照して下さい。
     */
    void Clear() NN_NOEXCEPT
    {
        nn::os::ClearEvent(&m_Event);
    }


    /**
     * @brief   EventType オブジェクトへの参照を返します。
     *
     * @return  EventType オブジェクトへの参照
     *
     * @details
     *  自インスタンスが持つ EventType オブジェクトへの参照を返します。
     *  この変換演算子があるため、EventType& を引数にとる関数に対して、
     *  Event& を指定することが可能です。
     */
    NN_IMPLICIT operator EventType&() NN_NOEXCEPT
    {
        return m_Event;
    }

    /**
     * @brief   EventType オブジェクトへの const 参照を返します。
     *
     * @return  EventType オブジェクトへの const 参照
     *
     * @details
     *  自インスタンスが持つ EventType オブジェクトへの const 参照を返します。
     *  この変換演算子があるため、const EventType& を引数にとる関数に対して、
     *  Event& を指定することが可能です。
     */
    NN_IMPLICIT operator const EventType&() const NN_NOEXCEPT
    {
        return m_Event;
    }

    /**
     * @brief   自インスタンスの EventType オブジェクトへのポインタを返します。
     *
     * @return  EventType オブジェクトへのポインタ
     *
     * @details
     *  自インスタンスが持つ EventType オブジェクトへのポインタを返します。
     */
    EventType* GetBase() NN_NOEXCEPT
    {
        return &m_Event;
    }

private:
    EventType   m_Event;
};

//--------------------------------------------------------------------------

}} // namespace nn::os

