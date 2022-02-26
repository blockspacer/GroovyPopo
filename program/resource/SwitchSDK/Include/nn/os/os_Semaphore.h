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
 * @brief   セマフォに関する公開ヘッダファイル
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_TimeSpan.h>
#include <nn/os/os_SemaphoreTypes.h>
#include <nn/os/os_SemaphoreApi.h>


namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   セマフォを扱うためのクラスです。
 *
 * @details
 *  セマフォは計数カウンタを使ってスレッド間で同期を確立するための機能です。
 *
 *  nn::os::Semaphore クラスは、@ref nn::os::SemaphoreType オブジェクトを使用する
 *  同期制御 API の呼び出しをラッピングしたユーティリティクラスです。@n
 *  本クラスが提供するメンバ関数は、@ref nn::os::SemaphoreType 型を引数にとる
 *  @ref nn::os::AcquireSemaphore() などの API を発行する形で実装されています。
 */
class Semaphore
{
    NN_DISALLOW_COPY( Semaphore );
    NN_DISALLOW_MOVE( Semaphore );

public:
    /**
     * @brief   セマフォオブジェクトを構築し初期化します。
     *
     * @param[in] initialCount  セマフォのカウンタ初期値
     * @param[in] maxCount      セマフォのカウンタ最大値
     *
     * @details
     *  コンストラクタです。@n
     *  セマフォオブジェクトを構築し、指定されたパラメータで初期化します。@n
     *  詳細は nn::os::InitializeSemaphore() を参照して下さい。
     */
    explicit Semaphore(int initialCount, int maxCount) NN_NOEXCEPT
    {
        nn::os::InitializeSemaphore(&m_Semaphore, initialCount, maxCount);
    }

    /**
     * @brief  デストラクタです。
     *
     * @details
     *  セマフォオブジェクトを破棄します。@n
     *  詳細は nn::os::FinalizeSemaphore() を参照して下さい。
     */
    ~Semaphore() NN_NOEXCEPT
    {
        nn::os::FinalizeSemaphore(&m_Semaphore);
    }

    /**
     * @brief   セマフォを獲得するまで待機します。
     *
     * @details
     *  セマフォから 1 カウント分を獲得するまで待機します。@n
     *  詳細は nn::os::AcquireSemaphore() を参照して下さい。
     */
    void Acquire() NN_NOEXCEPT
    {
        nn::os::AcquireSemaphore(&m_Semaphore);
    }

    /**
     * @brief   セマフォの獲得を試みます。
     *
     * @return  セマフォ待機の成否を返します。
     *
     * @details
     *  セマフォから 1 カウント分の獲得を試みます。@n
     *  セマフォを獲得できたら true を、そうでなければ false を返します。@n
     *  詳細は nn::os::TryAcquireSemaphore() を参照して下さい。
     */
    bool TryAcquire() NN_NOEXCEPT
    {
        return nn::os::TryAcquireSemaphore(&m_Semaphore);
    }

    /**
     * @brief   セマフォの獲得を時限付きで待機します。
     *
     * @param[in] timeout   最大待ち時間
     *
     * @return  セマフォ待機の成否を返します。
     *
     * @details
     *  セマフォから 1 カウント分を獲得するまで時限付きで待機します。@n
     *  セマフォを獲得できたら true を返します。@n
     *  指定された timeout 時間までにセマフォを獲得できなければ
     *  false を返します。@n
     *  詳細は nn::os::TimedAcquireSemaphore() を参照して下さい。
     */
    bool TimedAcquire(nn::TimeSpan timeout) NN_NOEXCEPT
    {
        return nn::os::TimedAcquireSemaphore(&m_Semaphore, timeout);
    }

    /**
     * @brief   セマフォを 1 カウントだけ返却します
     *
     * @overloadlist{nn_os_Semaphore_Release}
     *
     * @details
     *  セマフォに 1 カウントだけ返却します。
     *  詳細は @ref nn::os::ReleaseSemaphore(SemaphoreType*) を参照して下さい。
     */
    void Release() NN_NOEXCEPT
    {
        nn::os::ReleaseSemaphore(&m_Semaphore);
    }

    /**
     * @brief   セマフォを複数カウント返却します
     *
     * @overloadlist{nn_os_Semaphore_Release}
     *
     * @param[in] count         返却したいセマフォカウント数
     *
     * @details
     *  セマフォに count で指定されたカウント数だけ返却します。
     *  詳細は @ref nn::os::ReleaseSemaphore(SemaphoreType*, int) を参照して下さい。
     */
    void Release(int count) NN_NOEXCEPT
    {
        nn::os::ReleaseSemaphore(&m_Semaphore, count);
    }

    /**
     * @brief   セマフォの現在のカウンタ値を取得します。
     *
     * @return  現在のカウンタ値を返します。
     *
     * @details
     *  セマフォの現在のカウンタ値を取得します。
     *  詳細は nn::os::GetCurrentSemaphoreCount() を参照して下さい。
     */
    int GetCurrentCount() const NN_NOEXCEPT
    {
        return nn::os::GetCurrentSemaphoreCount(&m_Semaphore);
    }


    /**
     * @brief   SemaphoreType オブジェクトへの参照を返します。
     *
     * @return  SemaphoreType オブジェクトへの参照
     *
     * @details
     *  自インスタンスが持つ SemaphoreType オブジェクトへの参照を返します。
     *  この変換演算子があるため、SemaphoreType& を引数にとる関数に対して、
     *  Semaphore& を指定することが可能です。
     */
    NN_IMPLICIT operator SemaphoreType&() NN_NOEXCEPT
    {
        return m_Semaphore;
    }

    /**
     * @brief   SemaphoreType オブジェクトへの const 参照を返します。
     *
     * @return  SemaphoreType オブジェクトへの const 参照
     *
     * @details
     *  自インスタンスが持つ SemaphoreType オブジェクトへの const 参照を返します。
     *  この変換演算子があるため、const SemaphoreType& を引数にとる関数に対して、
     *  Semaphore& を指定することが可能です。
     */
    NN_IMPLICIT operator const SemaphoreType&() const NN_NOEXCEPT
    {
        return m_Semaphore;
    }

    /**
     * @brief   自インスタンスの SemaphoreType オブジェクトへのポインタを返します。
     *
     * @return  SemaphoreType オブジェクトへのポインタ
     *
     * @details
     *  自インスタンスが持つ SemaphoreType オブジェクトへのポインタを返します。
     */
    SemaphoreType* GetBase() NN_NOEXCEPT
    {
        return &m_Semaphore;
    }


private:
    SemaphoreType   m_Semaphore;
};

//--------------------------------------------------------------------------

}} // namespace nn::os

