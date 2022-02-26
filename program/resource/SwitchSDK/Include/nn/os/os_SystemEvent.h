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
 * @brief   SystemEvent に関する公開ヘッダファイル
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Abort.h>
#include <nn/nn_TimeSpan.h>
#include <nn/os/os_EventCommon.h>
#include <nn/os/os_SystemEventTypes.h>
#include <nn/os/os_SystemEventApi.h>


namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   システムイベントを扱うためのクラスです。
 *
 * @details
 *  システムイベントは SDK がイベントの通知に使用する同期オブジェクトです。
 *  詳細は関連ドキュメントを参照して下さい。
 *
 *  nn::os::SystemEvent クラスは、@ref nn::os::SystemEventType
 *  オブジェクトを使用する同期制御 API の呼び出しをラッピングした
 *  ユーティリティクラスです。@n
 *  本クラスが提供するメンバ関数は、@ref nn::os::SystemEventType 型を
 *  引数にとる @ref nn::os::WaitSystemEvent() などの API を発行する形で
 *  実装されています。
 */
class SystemEvent
{
    NN_DISALLOW_COPY( SystemEvent );
    NN_DISALLOW_MOVE( SystemEvent );

public:
    /**
     * @brief   システムイベントオブジェクトを未初期化で用意します。
     *
     * @details
     *  コンストラクタです。@n
     *  内部のシステムイベントオブジェクトは未初期化状態になります。
     *  この後に Attach() することで、
     *  システムイベントが使用できるようになります。
     */
    SystemEvent() NN_NOEXCEPT
    {
        m_SystemEvent._state = SystemEventType::State_NotInitialized;
    }


    //! @cond   SECTION_selectable_DeviceDriverKit
    /**
     * @brief   システムイベントオブジェクトを生成し初期化します。
     *
     * @param[in] clearMode     システムイベントのクリアモード
     * @param[in] interProcess  プロセス間での同期を行なえるか否か
     *
     * @details
     *  コンストラクタです。@n
     *  内部の SystemEventType オブジェクトを指定されたパラメータで
     *  初期化します。システムイベントの初期状態は非シグナル状態となります。
     *
     *  interProcess == true の場合、システムイベントはプロセス間で同期を行なう
     *  イベント機能としてハンドルの生成および初期化が行なわれます。@n
     *  interProcess == false の場合、システムイベントは nn::os::Event 相当の
     *  機能として初期化が行なわれます。@n
     *  詳細は nn::os::CreateSystemEvent() を参照して下さい。
     *
     *  ただし、本クラスでは nn::os::CreateSystemEvent() とは異なり、
     *  システムイベントの作成に必要なリソースが足りない場合アボートします。
     */
    explicit SystemEvent(EventClearMode clearMode, bool interProcess) NN_NOEXCEPT
    {
        auto result = nn::os::CreateSystemEvent(&m_SystemEvent, clearMode, interProcess);
        NN_ABORT_UNLESS_RESULT_SUCCESS(result);
    }
    //! @endcond

    //! @cond   SECTION_selectable_DeviceDriverKit
    /**
     * @brief   指定されたハンドルを使ってシステムイベントオブジェクトを初期化します。
     *
     * @param[in] readableHandle          ReadableEvent のハンドル
     * @param[in] isReadableHandleManaged ReadableEvent のハンドル管理フラグ
     * @param[in] writableHandle          WritableEvent のハンドル
     * @param[in] isWritableHandleManaged WritableEvent のハンドル管理フラグ
     * @param[in] clearMode               イベントのクリアモード
     *
     * @details
     *  コンストラクタです。@n
     *  指定されたハンドルを使って、プロセス間で同期を行なえるイベント機能
     *  として、システムイベントオブジェクトを初期化します。@n
     *  詳細は nn::os::AttachSystemEvent() を参照して下さい。
     */
    explicit SystemEvent(NativeHandle readableHandle, bool isReadableHandleManaged, NativeHandle writableHandle, bool isWritableHandleManaged, EventClearMode clearMode) NN_NOEXCEPT
    {
        nn::os::AttachSystemEvent(&m_SystemEvent, readableHandle, isReadableHandleManaged, writableHandle, isWritableHandleManaged, clearMode);
    }
    //! @endcond

    /**
     * @brief  デストラクタです。
     *
     * @details
     *  システムイベントオブジェクトを破棄します。@n
     *  詳細は nn::os::DestroySystemEvent() を参照して下さい。
     *
     *  ただし、本クラスでは nn::os::DestroySystemEvent() とは異なり、
     *  システムイベントオブジェクトが未初期化の場合、何もしません。
     */
    ~SystemEvent() NN_NOEXCEPT
    {
        if (m_SystemEvent._state == SystemEventType::State_NotInitialized)
        {
            return;
        }
        nn::os::DestroySystemEvent(&m_SystemEvent);
    }


    //! @cond   SECTION_selectable_DeviceDriverKit
    /**
     * @brief   指定されたハンドルをプロセス間同期イベントとして関連付けます。
     *
     * @param[in] readableHandle          ReadableEvent のハンドル
     * @param[in] isReadableHandleManaged ReadableEvent のハンドル管理フラグ
     * @param[in] writableHandle          WritableEvent のハンドル
     * @param[in] isWritableHandleManaged WritableEvent のハンドル管理フラグ
     * @param[in] clearMode               イベントの自動クリアモード
     *
     * @details
     *  指定されたハンドルを使って、プロセス間で同期を行なえるイベント機能
     *  として、システムイベントオブジェクトを初期化します。@n
     *  詳細は nn::os::AttachSystemEvent() を参照して下さい。
     */
    void Attach(NativeHandle readableHandle, bool isReadableHandleManaged, NativeHandle writableHandle, bool isWritableHandleManaged, EventClearMode clearMode) NN_NOEXCEPT
    {
        NN_ABORT_UNLESS(m_SystemEvent._state == SystemEventType::State_NotInitialized);
        nn::os::AttachSystemEvent(&m_SystemEvent, readableHandle, isReadableHandleManaged, writableHandle, isWritableHandleManaged, clearMode);
    }
    //! @endcond


    //! @cond   SECTION_selectable_DeviceDriverKit
    /**
     * @brief   指定された readableHandle をプロセス間同期イベントとして関連付けます。
     *
     * @param[in] readableHandle          ReadableEvent のハンドル
     * @param[in] isReadableHandleManaged ReadableEvent のハンドル管理フラグ
     * @param[in] clearMode               イベントの自動クリアモード
     *
     * @details
     *  指定された readableHandle を使って、プロセス間で同期を行なえるイベント機能
     *  として、システムイベントオブジェクトを初期化します。@n
     *  詳細は nn::os::AttachReadableHandleToSystemEvent() を参照して下さい。
     */
    void AttachReadableHandle(NativeHandle readableHandle, bool isReadableHandleManaged, EventClearMode clearMode) NN_NOEXCEPT
    {
        NN_ABORT_UNLESS(m_SystemEvent._state == SystemEventType::State_NotInitialized);
        nn::os::AttachReadableHandleToSystemEvent(&m_SystemEvent, readableHandle, isReadableHandleManaged, clearMode);
    }
    //! @endcond


    //! @cond   SECTION_selectable_DeviceDriverKit
    /**
     * @brief   指定された writableHandle をプロセス間同期イベントとして関連付けます。
     *
     * @param[in] writableHandle          WritableEvent のハンドル
     * @param[in] isWritableHandleManaged WritableEvent のハンドル管理フラグ
     * @param[in] clearMode               イベントの自動クリアモード
     *
     * @details
     *  指定された writableHandle を使って、プロセス間で同期を行なえるイベント機能
     *  として、システムイベントオブジェクトを初期化します。@n
     *  詳細は nn::os::AttachWritableHandleToSystemEvent() を参照して下さい。
     */
    void AttachWritableHandle(NativeHandle writableHandle, bool isWritableHandleManaged, EventClearMode clearMode) NN_NOEXCEPT
    {
        NN_ABORT_UNLESS(m_SystemEvent._state == SystemEventType::State_NotInitialized);
        nn::os::AttachWritableHandleToSystemEvent(&m_SystemEvent, writableHandle, isWritableHandleManaged, clearMode);
    }
    //! @endcond


    //! @cond   SECTION_selectable_DeviceDriverKit
    /**
     * @brief   システムイベントから ReadableHandle を切り離します。
     *
     * @return  ReadableHandle を返します。
     *
     * @details
     *  システムイベントオブジェクトから ReadableHandle を切り離します。@n
     *  詳細は nn::os::DetachReadableHandleOfSystemEvent() を参照して下さい。
     */
    NativeHandle DetachReadableHandle() NN_NOEXCEPT
    {
        return nn::os::DetachReadableHandleOfSystemEvent(&m_SystemEvent);
    }
    //! @endcond

    //! @cond   SECTION_selectable_DeviceDriverKit
    /**
     * @brief   システムイベントから WritableHandle を切り離します。
     *
     * @return  WritableHandle を返します。
     *
     * @details
     *  システムイベントオブジェクトから WritableHandle を切り離します。@n
     *  詳細は nn::os::DetachWritableHandleOfSystemEvent() を参照して下さい。
     */
    NativeHandle DetachWritableHandle() NN_NOEXCEPT
    {
        return nn::os::DetachWritableHandleOfSystemEvent(&m_SystemEvent);
    }
    //! @endcond


    /**
     * @brief   システムイベントがシグナル状態になるまで待機します。
     *
     * @details
     *  システムイベントがシグナル状態になるまで待機します。@n
     *  システムイベントが nn::os::EventClearMode_AutoClear の場合、
     *  シグナル状態を検知すると同時にシステムイベントは
     *  非シグナル状態にクリアされます。@n
     *  詳細は nn::os::WaitSystemEvent() を参照して下さい。
     */
    void Wait() NN_NOEXCEPT
    {
        nn::os::WaitSystemEvent(&m_SystemEvent);
    }

    /**
     * @brief   システムイベントがシグナル状態かをポーリングします。
     *
     * @return  システムイベントのポーリング成否を返します。
     *
     * @details
     *  システムイベントがシグナル状態かどうかをポーリングします。@n
     *  システムイベントがシグナル状態なら true を、
     *  そうでなければ false を返します。@n
     *  システムイベントが nn::os::EventClearMode_AutoClear の場合、
     *  シグナル状態を検知すると同時にシステムイベントは
     *  非シグナル状態にクリアされます。@n
     *  詳細は nn::os::TryWaitSystemEvent() を参照して下さい。
     */
    bool TryWait() NN_NOEXCEPT
    {
        return nn::os::TryWaitSystemEvent(&m_SystemEvent);
    }

    /**
     * @brief   システムイベントがシグナル状態になるまで時限付きで待機します。
     *
     * @param[in] timeout   最大待ち時間
     *
     * @return  システムイベント待機の成否を返します。
     *
     * @details
     *  システムイベントをシグナル状態になるまで時限付きで待機します。@n
     *  指定された timeout 時間までにシステムイベントがシグナル状態に
     *  なれば true を、そうでなければ false を返します。@n
     *  システムイベントが nn::os::EventClearMode_AutoClear の場合、
     *  シグナル状態を検知すると同時にシステムイベントは
     *  非シグナル状態にクリアされます。@n
     *  詳細は nn::os::TimedWaitSystemEvent() を参照して下さい。
     */
    bool TimedWait(nn::TimeSpan timeout) NN_NOEXCEPT
    {
        return nn::os::TimedWaitSystemEvent(&m_SystemEvent, timeout);
    }

    //! @cond   SECTION_selectable_DeviceDriverKit
    /**
     * @brief   システムイベントをシグナル状態にセットします。
     *
     * @details
     *  システムイベントをシグナル状態にセットします。@n
     *  詳細は nn::os::SignalSystemEvent() を参照して下さい。
     */
    void Signal() NN_NOEXCEPT
    {
        nn::os::SignalSystemEvent(&m_SystemEvent);
    }
    //! @endcond

    /**
     * @brief   システムイベントを非シグナル状態にクリアします。
     *
     * @details
     *  システムイベントを非シグナル状態にクリアします。@n
     *  詳細は nn::os::ClearSystemEvent() を参照して下さい。
     */
    void Clear() NN_NOEXCEPT
    {
        nn::os::ClearSystemEvent(&m_SystemEvent);
    }


    //! @cond   SECTION_selectable_DeviceDriverKit
    /**
     * @brief   システムイベントの ReadableHandle を返します。
     *
     * @return  ReadableHandle を返します。
     *
     * @details
     *  システムイベントオブジェクトに関連付けられている
     *  ReadableHandle を返します。@n
     *  詳細は nn::os::GetReadableHandleOfSystemEvent() を参照して下さい。
     */
    NativeHandle GetReadableHandle() const NN_NOEXCEPT
    {
        return nn::os::GetReadableHandleOfSystemEvent(&m_SystemEvent);
    }
    //! @endcond

    //! @cond   SECTION_selectable_DeviceDriverKit
    /**
     * @brief   システムイベントの WritableHandle を返します。
     *
     * @return  WritableHandle を返します。
     *
     * @details
     *  システムイベントオブジェクトに関連付けられている
     *  WritableHandle を返します。@n
     *  詳細は nn::os::GetWritableHandleOfSystemEvent() を参照して下さい。
     */
    NativeHandle GetWritableHandle() const NN_NOEXCEPT
    {
        return nn::os::GetWritableHandleOfSystemEvent(&m_SystemEvent);
    }
    //! @endcond


    /**
     * @brief   SystemEventType オブジェクトへの参照を返します。
     *
     * @return  SystemEventType オブジェクトへの参照
     *
     * @details
     *  自インスタンスが持つ SystemEventType オブジェクトへの参照を返します。
     *  この変換演算子があるため、 SystemEventType& を引数にとる関数に対して、
     *  SystemEvent& を指定することが可能です。
     */
    NN_IMPLICIT operator SystemEventType&() NN_NOEXCEPT
    {
        return m_SystemEvent;
    }

    /**
     * @brief   SystemEventType オブジェクトへの const 参照を返します。
     *
     * @return  SystemEventType オブジェクトへの const 参照
     *
     * @details
     *  自インスタンスが持つ SystemEventType オブジェクトへの const 参照を返します。
     *  この変換演算子があるため、const SystemEventType& を引数にとる関数に対して、
     *  SystemEvent& を指定することが可能です。
     */
    NN_IMPLICIT operator const SystemEventType&() const NN_NOEXCEPT
    {
        return m_SystemEvent;
    }

    /**
     * @brief   自インスタンスの SystemEventType オブジェクトへのポインタを返します。
     *
     * @return  SystemEventType オブジェクトへのポインタ
     *
     * @details
     *  自インスタンスが持つ SystemEventType オブジェクトへのポインタを返します。
     */
    SystemEventType* GetBase() NN_NOEXCEPT
    {
        return &m_SystemEvent;
    }

private:
    SystemEventType     m_SystemEvent;
};

//--------------------------------------------------------------------------

}} // namespace nn::os

