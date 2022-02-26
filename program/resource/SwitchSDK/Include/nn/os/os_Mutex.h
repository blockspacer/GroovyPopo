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
 * @brief   ミューテックスに関する公開ヘッダファイル
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/os/os_MutexCommon.h>
#include <nn/os/os_MutexTypes.h>
#include <nn/os/os_MutexApi.h>


namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   ミューテックスを扱うためのクラスです。
 *
 * @details
 *  ミューテックスはスレッド間で排他制御を行なうための同期オブジェクトです。
 *
 *  nn::os::Mutex クラスは、@ref nn::os::MutexType オブジェクトと
 *  関連する同期制御 API の呼び出しをラッピングしたユーティリティクラスです。
 *  本クラスが提供するメンバ関数は、@ref nn::os::MutexType 型を引数にとる
 *  @ref nn::os::LockMutex() などの API を発行する形で実装されています。
 *
 *  また、このクラスは C++ 標準の BasicLockable 要件と Lockable 要件を
 *  満たしているため、std::lock_guard や std::unique_lock などの
 *  クラステンプレートの型引数とすることができます。@n
 *  このため、Lock() と lock() のように同じ処理を行なうメンバ関数が
 *  複数存在しています。
 */
class Mutex
{
    NN_DISALLOW_COPY( Mutex );
    NN_DISALLOW_MOVE( Mutex );

public:
    /**
     * @brief   ミューテックスオブジェクトを構築し初期化します。
     *
     * @overloadlist{nn_os_Mutex_Mutex}
     *
     * @param[in] recursive ミューテックスの再帰ロックの可否
     *
     * @details
     *  コンストラクタです。
     *  ミューテックスオブジェクトを構築し初期化します。@n
     *  詳細は @ref nn::os::InitializeMutex を参照して下さい。@n
     *  本コンストラクタを使用した場合、ロックレベルのチェックは行なわれません。
     */
    explicit Mutex(bool recursive) NN_NOEXCEPT
    {
        InitializeMutex(&m_Mutex, recursive, 0);
    }

    /**
     * @brief   ミューテックスオブジェクトを構築し初期化します。
     *
     * @overloadlist{nn_os_Mutex_Mutex}
     *
     * @param[in] recursive ミューテックスの再帰ロックの可否
     * @param[in] lockLevel ロックレベル
     *
     * @details
     *  コンストラクタです。
     *  ミューテックスオブジェクトを構築し初期化します。@n
     *  Release 版ビルドでは処理速度向上目的のため、ロックレベル機能は無効になります。@n
     *  詳細は nn::os::InitializeMutex() を参照して下さい。
     */
    explicit Mutex(bool recursive, int lockLevel) NN_NOEXCEPT
    {
        InitializeMutex(&m_Mutex, recursive, lockLevel);
    }

    /**
     * @brief  デストラクタです。
     *
     * @details
     *  ミューテックスオブジェクトを破棄します。
     */
    ~Mutex() NN_NOEXCEPT
    {
        FinalizeMutex(&m_Mutex);
    }

    /**
     * @brief   ミューテックスをロックします。
     *
     * @details
     *  ミューテックスをロックします。
     *  ロックに成功するまでブロッキングします。@n
     *  詳細は nn::os::LockMutex() を参照して下さい。
     */
    void Lock() NN_NOEXCEPT
    {
        LockMutex(&m_Mutex);
    }

    /**
     * @brief   ミューテックスのロックを試みます。
     *
     * @return  ロックに成功したか否か
     *
     * @details
     *  ミューテックスのロックを試み、ロックの成否を bool 型で返します。@n
     *  本関数は、如何なる時もブロッキングしません。@n
     *  詳細は nn::os::TryLockMutex() を参照して下さい。
     */
    bool TryLock() NN_NOEXCEPT
    {
        return TryLockMutex(&m_Mutex);
    }

    /**
     * @brief   ミューテックスをアンロックします。
     *
     * @details
     *  ミューテックスをアンロックします。
     *  ミューテックスは事前にロックされていなければなりません。@n
     *  詳細は nn::os::UnlockMutex() を参照して下さい。
     */
    void Unlock() NN_NOEXCEPT
    {
        UnlockMutex(&m_Mutex);
    }

    /**
     * @brief   ミューテックスが自スレッドによって獲得済みか否かを返します。
     *
     * @details
     *  ミューテックスが自スレッドによって獲得済みなら true を、
     *  そうでなければ false を返します。@n
     *  詳細は nn::os::IsMutexLockedByCurrentThread() を参照して下さい。
     */
    bool IsLockedByCurrentThread() const NN_NOEXCEPT
    {
        return IsMutexLockedByCurrentThread(&m_Mutex);
    }


    /**
     * @brief   ミューテックスをロックします。
     *
     * @details
     *  @ref Lock() と等価です。@n
     */
    void lock() NN_NOEXCEPT
    {
        Lock();
    }

    /**
     * @brief   ミューテックスのロックを試みます。
     *
     * @return  ロックに成功したか否か
     *
     * @details
     *  @ref TryLock() と等価です。@n
     */
    bool try_lock() NN_NOEXCEPT
    {
        return TryLock();
    }

    /**
     * @brief   ミューテックスをアンロックします。
     *
     * @details
     *  @ref Unlock() と等価です。@n
     */
    void unlock() NN_NOEXCEPT
    {
        Unlock();
    }

    /**
     * @brief   MutexType オブジェクトへの参照を返します。
     *
     * @return  MutexType オブジェクトへの参照
     *
     * @details
     *  自インスタンスが持つ MutexType オブジェクトへの参照を返します。
     *  この変換演算子があるため、MutexType& を引数にとる関数に対して、
     *  Mutex& を指定することが可能です。
     */
    NN_IMPLICIT operator MutexType&() NN_NOEXCEPT
    {
        return m_Mutex;
    }

    /**
     * @brief   MutexType オブジェクトへの const 参照を返します。
     *
     * @return  MutexType オブジェクトへの const 参照
     *
     * @details
     *  自インスタンスが持つ MutexType オブジェクトへの const 参照を返します。
     *  この変換演算子があるため、const MutexType& を引数にとる関数に対して、
     *  Mutex& を指定することが可能です。
     */
    NN_IMPLICIT operator const MutexType&() const NN_NOEXCEPT
    {
        return m_Mutex;
    }

    /**
     * @brief   自インスタンスの MutexType オブジェクトへのポインタを返します。
     *
     * @return  MutexType オブジェクトへのポインタ
     *
     * @details
     *  自インスタンスが持つ MutexType オブジェクトへのポインタを返します。
     */
    MutexType* GetBase() NN_NOEXCEPT
    {
        return &m_Mutex;
    }

private:
    MutexType   m_Mutex;
};

//--------------------------------------------------------------------------

}} // namespace nn::os

