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
 * @brief   条件変数に関する公開ヘッダファイル
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/os/os_MutexTypes.h>
#include <nn/os/os_ConditionVariableCommon.h>
#include <nn/os/os_ConditionVariableTypes.h>
#include <nn/os/os_ConditionVariableApi.h>

namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   条件変数を扱うためのクラスです。
 *
 * @details
 *  条件変数クラスは、ミューテックスクラスと併用し特定の条件を満たすまで
 *  スレッドの実行を待機するためのクラスです。
 *
 *  nn::os::ConditionVariable クラスは、@ref nn::os::ConditionVariableType
 *  オブジェクトと関連する同期制御 API の呼び出しをラッピングした
 *  ユーティリティクラスです。
 *  本クラスが提供するメンバ関数は、@ref nn::os::ConditionVariableType 型を
 *  引数にとる nn::os::WaitConditionVariable() などの API を発行する形で
 *  実装されています。
 *
 */
class ConditionVariable
{
    NN_DISALLOW_COPY( ConditionVariable );
    NN_DISALLOW_MOVE( ConditionVariable );

public:
    /**
     * @brief   条件変数オブジェクトを構築し初期化します。
     *
     * @details
     *  コンストラクタです。
     *  条件変数オブジェクトを構築し初期化します。@n
     *  詳細は nn::os::InitializeConditionVariable() を参照して下さい。
     */
    ConditionVariable() NN_NOEXCEPT
    {
        InitializeConditionVariable(&m_ConditionVariable);
    }

    /**
     * @brief  デストラクタです。
     *
     * @details
     *  条件変数オブジェクトを破棄します。@n
     *  詳細は nn::os::FinalizeConditionVariable() を参照して下さい。
     */
    ~ConditionVariable() NN_NOEXCEPT
    {
        FinalizeConditionVariable(&m_ConditionVariable);
    }

    /**
     * @brief   条件変数を待機します。
     *
     * @param[in] mutex     現在ロック中の MutexType オブジェクトへの参照
     *
     * @details
     *  指定されたミューテックスのアンロックと、条件変数への待機を
     *  アトミックに行ないます。 Signal() や Broadcast() によって条件変数が
     *  シグナル化されると、再び対象ミューテックスをロックしてから
     *  呼び出し元へ返ります。
     *
     *  mutex には @ref nn::os::Mutex クラスインスタンスへの参照を直接指定する
     *  ことも可能です。この場合、Mutex クラスの暗黙の変換演算子が適用されます。
     *
     *  詳細は nn::os::WaitConditionVariable() を参照して下さい。
     */
    void Wait(nn::os::MutexType& mutex) NN_NOEXCEPT
    {
        WaitConditionVariable(&m_ConditionVariable, &mutex);
    }

    /**
     * @brief   条件変数をタイムアウト付きで待機します。
     *
     * @param[in] mutex     現在ロック中の MutexType オブジェクトへの参照
     * @param[in] timeout   タイムアウトまでの時間
     *
     * @return  条件変数待機が解除された要因を返します
     *
     * @details
     *  指定されたミューテックスのアンロックと、条件変数への待機を
     *  アトミックに行ないます。 Signal() や Broadcast() によって条件変数が
     *  シグナル化されると、再び対象ミューテックスをロックしてから
     *  呼び出し元へ返ります。
     *
     *  mutex には @ref nn::os::Mutex クラスインスタンスへの参照を直接指定する
     *  ことも可能です。この場合、Mutex クラスの暗黙の変換演算子が適用されます。
     *
     *  条件変数待ちに入った後、timeout 時間経過するとタイムアウトが成立し、
     *  自動的に待ちが解除されます。この時も再度ミューテックスをロックしてから
     *  呼び出し元へ返ります。
     *
     *  詳細は nn::os::TimedWaitConditionVariable() を参照して下さい。
     */
    ConditionVariableStatus TimedWait(nn::os::MutexType& mutex, nn::TimeSpan timeout) NN_NOEXCEPT
    {
        return TimedWaitConditionVariable(&m_ConditionVariable, &mutex, timeout);
    }

    /**
     * @brief   条件変数へシグナルを通知します。
     *
     * @details
     *  条件変数へシグナルを通知し、対象の条件変数を待機しているスレッドを
     *  １つだけ起床します。@n
     *  詳細は nn::os::SignalConditionVariable() を参照して下さい。
     */
    void Signal() NN_NOEXCEPT
    {
        SignalConditionVariable(&m_ConditionVariable);
    }

    /**
     * @brief   条件変数へブロードキャスト通知を行ないます。
     *
     * @details
     *  条件変数へブロードキャスト通知を行ない、対象の条件変数を待機している
     *  全てのスレッドを起床します。@n
     *  詳細は nn::os::BroadcastConditionVariable() を参照して下さい。
     */
    void Broadcast() NN_NOEXCEPT
    {
        BroadcastConditionVariable(&m_ConditionVariable);
    }

    /**
     * @brief   ConditionVariableType オブジェクトへの参照を返します。
     *
     * @return  ConditionVariableType オブジェクトへの参照
     *
     * @details
     *  自インスタンスが持つ ConditionVariableType オブジェクトへの参照を返します。
     *  この変換演算子があるため、ConditionVariableType& を引数にとる関数に対して、
     *  ConditionVariable& を指定することが可能です。
     */
    NN_IMPLICIT operator ConditionVariableType&() NN_NOEXCEPT
    {
        return m_ConditionVariable;
    }

    /**
     * @brief   ConditionVariableType オブジェクトへの const 参照を返します。
     *
     * @return  ConditionVariableType オブジェクトへの const 参照
     *
     * @details
     *  自インスタンスが持つ ConditionVariableType オブジェクトへの const 参照を返します。
     *  この変換演算子があるため、const ConditionVariableType& を引数にとる関数に対して、
     *  ConditionVariable& を指定することが可能です。
     */
    NN_IMPLICIT operator const ConditionVariableType&() const NN_NOEXCEPT
    {
        return m_ConditionVariable;
    }

    /**
     * @brief   自インスタンスの ConditionVariableType オブジェクトへのポインタを返します。
     *
     * @return  ConditionVariableType オブジェクトへのポインタ
     *
     * @details
     *  自インスタンスが持つ ConditionVariableType オブジェクトへのポインタを返します。
     */
    ConditionVariableType* GetBase() NN_NOEXCEPT
    {
        return &m_ConditionVariable;
    }

private:
    ConditionVariableType   m_ConditionVariable;
};

//--------------------------------------------------------------------------

}}  // namespace nn::os

