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
 * @brief   Readers-Writer ロックに関する公開ヘッダファイル
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/os/os_ReaderWriterLockCommon.h>
#include <nn/os/os_ReaderWriterLockTypes.h>
#include <nn/os/os_ReaderWriterLockApi.h>


namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   Readers-Writer ロックを扱うためのクラスです。
 *
 * @details
 *  Readers-Writer ロック機能は、スレッド間で排他制御を行なうための
 *  同期オブジェクトの一つです。何かしらの共有リソースに対して、
 *  読み込みアクセス目的の複数の Readers スレッドと、
 *  書き込みアクセス目的の単一の Writer スレッドが排他を行なう場面で使用します。
 *
 *  nn::os::ReaderWriterLock クラスは nn::os::ReaderWriterLockType 型の
 *  オブジェクトを扱う同期制御 API の呼び出しをラッピングしたユーティリティ
 *  クラスです。本クラスが提供するメンバ関数は nn::os::ReaderWriterLockType 型
 *  を引数にとる nn::os::AcquireReadLock() などの API を発行する形で
 *  実装されています。
 *
 *  また、このクラスは C++ 標準の BasicLockable 要件と Lockable 要件を
 *  満たしているため、std::lock_guard や std::unique_lock などの
 *  クラステンプレートの型引数とすることができます。
 *  これらのロックでは、Writer ロックを取得します。
 *
 *  更に、このクラスは lock_shared() および unlock_shared() 関数を持つため、
 *  std::shared_lock のクラステンプレートの型引数とすることができます。
 *  これらのロックでは、Readers ロックを取得します。
 *
 *  このため、
 *  @ref ReaderWriterLock::AcquireWriteLock "AcquireWriteLock()" と lock() や、
 *  @ref ReaderWriterLock::AcquireReadLock "AcquireReadLock()" と lock_shared() のように、
 *  同じ処理を行なうメンバ関数が複数存在しています。
 */
class ReaderWriterLock
{
    NN_DISALLOW_COPY( ReaderWriterLock );
    NN_DISALLOW_MOVE( ReaderWriterLock );

public:
    /**
     * @brief   Readers-Writer ロックオブジェクトを構築し初期化します。
     *
     * @details
     *  コンストラクタです。
     *  Readers-Writer ロックオブジェクトを構築し初期化します。
     *
     *  詳細は nn::os::InitializeReaderWriterLock() を参照して下さい。
     *
     */
    explicit ReaderWriterLock() NN_NOEXCEPT
    {
        nn::os::InitializeReaderWriterLock(&m_ReaderWriterLock);
    }

    /**
     * @brief  デストラクタです。
     *
     * @details
     *  Readers-Writer ロックオブジェクトを破棄します。
     *
     *  詳細は nn::os::FinalizeReaderWriterLock() を参照して下さい。
     *
     */
    ~ReaderWriterLock() NN_NOEXCEPT
    {
        nn::os::FinalizeReaderWriterLock(&m_ReaderWriterLock);
    }


    /**
     * @brief   Readers ロックを取得します。
     *
     * @details
     *  Readers-Writer ロックオブジェクトの Readers ロックを取得します。
     *
     *  詳細は nn::os::AcquireReadLock() を参照して下さい。
     *
     */
    void AcquireReadLock() NN_NOEXCEPT
    {
        nn::os::AcquireReadLock(&m_ReaderWriterLock);
    }

    /**
     * @brief   Readers ロックの取得を試みます。
     *
     * @return  Readers ロックに成功したか否か
     *
     * @details
     *  Readers-Writer ロックオブジェクトの Readers ロックの取得を試み、
     *  ロックの成否を bool 型で返します。
     *
     *  詳細は nn::os::TryAcquireReadLock() を参照して下さい。
     *
     */
    bool TryAcquireReadLock() NN_NOEXCEPT
    {
        return nn::os::TryAcquireReadLock(&m_ReaderWriterLock);
    }

    /**
     * @brief   Readers ロックを解除します。
     *
     * @details
     *  Readers-Writer ロックオブジェクトの Readers ロックを解除します。
     *
     *  詳細は nn::os::ReleaseReadLock() を参照して下さい。
     *
     */
    void ReleaseReadLock() NN_NOEXCEPT
    {
        nn::os::ReleaseReadLock(&m_ReaderWriterLock);
    }


    /**
     * @brief   Writer ロックを取得します。
     *
     * @details
     *  Readers-Writer ロックオブジェクトの Writer ロックを取得します。
     *
     *  詳細は nn::os::AcquireWriteLock() を参照して下さい。
     *
     */
    void AcquireWriteLock() NN_NOEXCEPT
    {
        nn::os::AcquireWriteLock(&m_ReaderWriterLock);
    }

    /**
     * @brief   Writer ロックの取得を試みます。
     *
     * @return  Writer ロックに成功したか否か
     *
     * @details
     *  Readers-Writer ロックオブジェクトの Writer ロックの取得を試み、
     *  ロックの成否を bool 型で返します。
     *
     *  詳細は nn::os::TryAcquireWriteLock() を参照して下さい。
     *
     */
    bool TryAcquireWriteLock() NN_NOEXCEPT
    {
        return nn::os::TryAcquireWriteLock(&m_ReaderWriterLock);
    }

    /**
     * @brief   Writer ロックを解除します。
     *
     * @details
     *  Readers-Writer ロックオブジェクトの Writer ロックを解除します。
     *  自スレッドは事前に Writer ロックを取得済みでなければなりません。
     *
     *  詳細は nn::os::ReleaseWriteLock() を参照して下さい。
     *
     */
    void ReleaseWriteLock() NN_NOEXCEPT
    {
        nn::os::ReleaseWriteLock(&m_ReaderWriterLock);
    }


    /**
     * @brief   Readers ロック状態か否かを返します。
     *
     * @details
     *  Readers-Writer ロックオブジェクトが Readers ロック状態ならば true を、
     *  そうでなければ false を返します。
     *
     *  詳細は nn::os::IsReadLockHeld() を参照して下さい。
     *
     */
    bool IsReadLockHeld() const NN_NOEXCEPT
    {
        return nn::os::IsReadLockHeld(&m_ReaderWriterLock);
    }

    /**
     * @brief   Writer ロックが自スレッドによって取得済みか否かを返します。
     *
     * @details
     *  Readers-Writer ロックオブジェクトの Writer ロックが、
     *  自スレッドによって取得済みなら true を、
     *  そうでなければ false を返します。
     *
     *  詳細は nn::os::IsWriteLockHeldByCurrentThread() を参照して下さい。
     *
     */
    bool IsWriteLockHeldByCurrentThread() const NN_NOEXCEPT
    {
        return nn::os::IsWriteLockHeldByCurrentThread(&m_ReaderWriterLock);
    }

    /**
     * @brief   自スレッドがロックオーナーか否かを返します。
     *
     * @details
     *  自スレッドが rwlock オブジェクトのロックオーナーなら true を、
     *  そうでなければ false を返します。
     *
     *  詳細は nn::os::IsReaderWriterLockOwnerThread() を参照して下さい。
     *
     */
    bool IsLockOwner() const NN_NOEXCEPT
    {
        return nn::os::IsReaderWriterLockOwnerThread(&m_ReaderWriterLock);
    }


    /**
     * @brief   Readers ロックを取得します。
     *
     * @details
     *  @ref AcquireReadLock() と等価です。
     *
     */
    void lock_shared() NN_NOEXCEPT
    {
        AcquireReadLock();
    }

    /**
     * @brief   Readers ロックの取得を試みます。
     *
     * @return  Readers ロックに成功したか否か
     *
     * @details
     *  @ref TryAcquireReadLock() と等価です。
     *
     */
    bool try_lock_shared() NN_NOEXCEPT
    {
        return TryAcquireReadLock();
    }

    /**
     * @brief   Readers ロックを解除します。
     *
     * @details
     *  @ref ReleaseReadLock() と等価です。
     *
     */
    void unlock_shared() NN_NOEXCEPT
    {
        ReleaseReadLock();
    }

    /**
     * @brief   Writer ロックを取得します。
     *
     * @details
     *  @ref AcquireWriteLock() と等価です。
     *
     */
    void lock() NN_NOEXCEPT
    {
        AcquireWriteLock();
    }

    /**
     * @brief   Writer ロックの取得を試みます。
     *
     * @return  Writer ロックに成功したか否か
     *
     * @details
     *  @ref TryAcquireWriteLock() と等価です。
     *
     */
    bool try_lock() NN_NOEXCEPT
    {
        return TryAcquireWriteLock();
    }

    /**
     * @brief   Writer ロックを解除します。
     *
     * @details
     *  @ref ReleaseWriteLock() と等価です。
     *
     */
    void unlock() NN_NOEXCEPT
    {
        ReleaseWriteLock();
    }


    /**
     * @brief   ReaderWriterLockType オブジェクトへの参照を返します。
     *
     * @return  ReaderWriterLockType オブジェクトへの参照
     *
     * @details
     *  自インスタンスが持つ ReaderWriterLockType オブジェクトへの参照を返します。
     *  この変換演算子があるため、ReaderWriterLockType& を引数にとる関数に対して、
     *  ReaderWriterLock& を指定することが可能です。
     *
     */
    NN_IMPLICIT operator ReaderWriterLockType&() NN_NOEXCEPT
    {
        return m_ReaderWriterLock;
    }

    /**
     * @brief   ReaderWriterLockType オブジェクトへの const 参照を返します。
     *
     * @return  ReaderWriterLockType オブジェクトへの const 参照
     *
     * @details
     *  自インスタンスが持つ ReaderWriterLockType オブジェクトへの const 参照を返します。
     *  この変換演算子があるため、const ReaderWriterLockType& を引数にとる関数に対して、
     *  ReaderWriterLock& を指定することが可能です。
     *
     */
    NN_IMPLICIT operator const ReaderWriterLockType&() const NN_NOEXCEPT
    {
        return m_ReaderWriterLock;
    }

    /**
     * @brief   ReaderWriterLockType オブジェクトへのポインタを返します。
     *
     * @return  ReaderWriterLockType オブジェクトへのポインタ
     *
     * @details
     *  自インスタンスが持つ ReaderWriterLockType オブジェクトへのポインタを返します。
     *
     */
    ReaderWriterLockType* GetBase() NN_NOEXCEPT
    {
        return &m_ReaderWriterLock;
    }

private:
    ReaderWriterLockType    m_ReaderWriterLock;
};

//--------------------------------------------------------------------------

}} // namespace nn::os

