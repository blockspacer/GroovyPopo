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
 * @brief   Readers-Writer ロックに関する API の宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Macro.h>
#include <nn/os/os_ReaderWriterLockCommon.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn { namespace os {

struct  ReaderWriterLockType;

//! @name Readers-Writer ロック関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   ReaderWriterLockType オブジェクトを初期化します。
 *
 * @param[in] rwlock    ReaderWriterLockType オブジェクトへのポインタ
 *
 * @pre
 *  - rwlock が未初期化状態もしくは初期化されていない状態である
 *
 * @post
 *  - rwlock がアンロック状態である
 *
 * @details
 *  rwlock オブジェクトを初期化します。
 *
 *  Readers-Writer ロック機能は、スレッド間で排他制御を行なうための
 *  同期オブジェクトの一つです。何かしらの共有リソースに対して、
 *  読み込みアクセス目的の複数の Readers スレッドと、
 *  書き込みアクセス目的の単一の Writer スレッドが排他を行なう場面で使用します。
 *
 *  ReaderWriterLockType オブジェクトは以下の状態をとります。
 *  また、排他ロック状態では特定のスレッドがロックオーナーとして管理されます。
 *
 *  - 未初期化状態
 *  - アンロック状態
 *  - 共有ロック状態（ロックオーナーなし）
 *  - 排他ロック状態（ロックオーナーあり）
 *
 *  アンロック状態は、いずれのスレッドもロックを取得していない状態です。
 *
 *  共有ロック状態は、ロックオーナーが未設定の状態で、
 *  一つ以上のスレッドが Readers ロックを獲得済みの状態です。
 *  共有ロック状態は、AcquireReadLock() および TryAcquireReadLock() された
 *  回数と同じ数だけ ReleaseReadLock() が呼ばれた時にアンロック状態に戻ります。
 *
 *  排他ロック状態は、ロックオーナーが設定された状態で、
 *  単一のスレッドが Readers ロックもしくは Writer ロックを獲得済みの状態です。
 *  最初にロックオーナーが設定されるのは、単一のスレッドが
 *  AcquireWriteLock() もしくは TryAcquireWriteLock() に成功した時で、
 *  呼出元のスレッドが rwlock オブジェクトのロックオーナーとして設定されます。
 *
 *  一旦ロックオーナーとなったスレッドはさらに以下のような制御が可能になります。
 *
 *  - 追加の Readers ロックをブロッキングせずに重ねて取得できます。
 *  - 追加の Writer ロックをブロッキングせずに重ねて取得できます。
 *  - 全ての Readers/Writer ロックを解放した時点でロックオーナーが解除され、アンロック状態となります。
 *  - 上記の Readers/Write ロックの取得順序や解放順序に制限はありません。
 *
 *  なお、Readers/Wirte ロックを取得する際にブロッキングされたスレッドは、
 *  次にロックを取得できる条件が整った段階で自動的に再度ロック取得を試みます。
 *  この時、複数のスレッドによって、Readers ロック要求と Writer ロック要求の
 *  両方が存在していた場合、Writer ロック要求が優先されます。
 *
 *  なお、本 API はスレッドセーフではありません。@n
 *  本 API の動作中に対象 rwlock オブジェクトに対する操作は行なわないで下さい。
 *
 */
void InitializeReaderWriterLock(ReaderWriterLockType* rwlock) NN_NOEXCEPT;


/**
 * @brief   ReaderWriterLockType オブジェクトを破棄します。
 *
 * @param[in] rwlock    ReaderWriterLockType オブジェクトへのポインタ
 *
 * @pre
 *  - rwlock がアンロック状態である
 *
 * @post
 *  - rwlock が未初期化状態である
 *
 * @details
 *  rwlock オブジェクトを破棄します。
 *
 *  なお、本 API はスレッドセーフではありません。@n
 *  本 API の動作中に対象 rwlock オブジェクトに対する操作は行なわないで下さい。
 *
 */
void FinalizeReaderWriterLock(ReaderWriterLockType* rwlock) NN_NOEXCEPT;


/**
 * @brief   Readers ロックを取得します。
 *
 * @param[in] rwlock    ReaderWriterLockType オブジェクトへのポインタ
 *
 * @pre
 *  - rwlock がアンロック、共有ロック、排他ロックのいずれかの状態である
 *
 * @post
 *  - IsReadLockHeld() == true
 *  - rwlock が共有ロック状態である（自スレッドがロックオーナーでない場合）
 *  - rwlock が排他ロック状態である（自スレッドがロックオーナーの場合）
 *
 * @details
 *  rwlock オブジェクトの Readers ロックを取得します。
 *
 *  rwlock オブジェクトがアンロック状態もしくは共有ロック状態であれば、
 *  Readers ロックの取得に成功します。
 *
 *  rwlock オブジェクトが排他ロック状態の場合、
 *  ロックオーナーが自スレッドであれば、Readers ロックの取得に成功します。
 *  ロックオーナーが他スレッドであれば、自スレッドは Readers ロックの取得に
 *  成功するまでブロッキングします。
 *
 */
void AcquireReadLock(ReaderWriterLockType* rwlock) NN_NOEXCEPT;


/**
 * @brief   Readers ロックの取得を試みます。
 *
 * @param[in] rwlock    ReaderWriterLockType オブジェクトへのポインタ
 *
 * @return  Readers ロックに成功したか否か
 *
 * @pre
 *  - rwlock がアンロック、共有ロック、排他ロックのいずれかの状態である
 *
 * @post
 *  - 返値が true の場合、以下の状態となる
 *    - IsReadLockHeld() == true
 *    - rwlock が共有ロック状態である（自スレッドがロックオーナーでない場合）
 *    - rwlock が排他ロック状態である（自スレッドがロックオーナーの場合）
 *
 * @details
 *  rwlock オブジェクトの Readers ロックの取得を試み、
 *  ロックの成否を bool 型で返します。
 *
 *  返値が true の場合、Readers ロックの取得に成功しています。
 *  返値が false の場合、Readers ロックの取得に失敗し、
 *  rwlock オブジェクトの状態は変更されません。
 *
 */
bool TryAcquireReadLock(ReaderWriterLockType* rwlock) NN_NOEXCEPT;


/**
 * @brief   Readers ロックを 1 つだけ解放します。
 *
 * @param[in] rwlock    ReaderWriterLockType オブジェクトへのポインタ
 *
 * @pre
 *  - IsReadLockHeld() == true
 *  - rwlock が共有ロック状態である（自スレッドがロックオーナーでない場合）
 *  - rwlock が排他ロック状態である（自スレッドがロックオーナーの場合）
 *
 * @post
 *  - rwlock がアンロック、共有ロック、排他ロックのいずれかの状態である
 *
 * @details
 *  rwlock オブジェクトの Readers ロックを 1 つだけ解放します。
 *
 *  全ての Readers/Writer ロックが解放されるとアンロック状態となり、
 *  同時にロック待ちでブロッキングしているスレッドがあればそれらを起床させます。
 *  この時、複数のスレッドによって、Readers ロック要求と Writer ロック要求の
 *  両方が存在していた場合、Writer ロック要求が優先されます。
 *
 */
void ReleaseReadLock(ReaderWriterLockType* rwlock) NN_NOEXCEPT;


/**
 * @brief   Writer ロックを取得します。
 *
 * @param[in] rwlock    ReaderWriterLockType オブジェクトへのポインタ
 *
 * @pre
 *  - rwlock がアンロック、共有ロック、排他ロックのいずれかの状態である
 *
 * @post
 *  - IsWriteLockHeldByCurrentThread() == true
 *  - IsReaderWriterLockOwnerThread() == true
 *  - rwlock が排他ロック状態である
 *
 * @details
 *  rwlock オブジェクトの Writer ロックを取得します。
 *  ロックに成功した後は、自スレッドがロックオーナーとなります。
 *
 *  Writer ロックが取得できない状態の場合、
 *  自スレッドは Writer ロックの取得に成功するまでブロッキングします。
 *
 */
void AcquireWriteLock(ReaderWriterLockType* rwlock) NN_NOEXCEPT;


/**
 * @brief   Writer ロックの取得を試みます。
 *
 * @param[in] rwlock    ReaderWriterLockType オブジェクトへのポインタ
 *
 * @return  Writer ロックに成功したか否か
 *
 * @pre
 *  - rwlock がアンロック、共有ロック、排他ロックのいずれかの状態である
 *
 * @post
 *  - 返値が true の場合、以下の状態となる
 *    - IsWriteLockHeldByCurrentThread() == true
 *    - IsReaderWriterLockOwnerThread() == true
 *    - rwlock が排他ロック状態である
 *
 * @details
 *  rwlock オブジェクトの Writer ロックの取得を試み、
 *  ロックの成否を bool 型で返します。
 *
 *  返値が true の場合、Writer ロックの取得に成功し、
 *  自スレッドがロックオーナーとなります。
 *
 */
bool TryAcquireWriteLock(ReaderWriterLockType* rwlock) NN_NOEXCEPT;


/**
 * @brief   Writer ロックを 1 つだけ解放します。
 *
 * @param[in] rwlock    ReaderWriterLockType オブジェクトへのポインタ
 *
 * @pre
 *  - IsWriteLockHeldByCurrentThread() == true
 *  - IsReaderWriterLockOwnerThread() == true
 *  - rwlock が排他ロック状態である
 *
 * @post
 *  - rwlock がアンロック、共有ロック、排他ロックのいずれかの状態である
 *
 * @details
 *  rwlock オブジェクトの Writer ロックを 1 つだけ解放します。
 *
 *  全ての Readers/Writer ロックが解放されるとアンロック状態となり、
 *  同時にロック待ちでブロッキングしているスレッドがあればそれらを起床させます。
 *  この時、複数のスレッドによって、Readers ロック要求と Writer ロック要求の
 *  両方が存在していた場合、Writer ロック要求が優先されます。
 *
 */
void ReleaseWriteLock(ReaderWriterLockType* rwlock) NN_NOEXCEPT;


/**
 * @brief   いずれかのスレッドで Readers ロックが取得済みか否かを返します。
 *
 * @param[in] rwlock    ReaderWriterLockType オブジェクトへのポインタ
 *
 * @return  Readers ロック取得済みか否か
 *
 * @pre
 *  - rwlock がアンロック、共有ロック、排他ロックのいずれかの状態である
 *
 * @details
 *  いずれかのスレッドが rwlock オブジェクトの Readers ロックを
 *  取得済みであれば true を、そうでなければ false を返します。
 *
 */
bool IsReadLockHeld(const ReaderWriterLockType* rwlock) NN_NOEXCEPT;


/**
 * @brief   自スレッドで Writer ロックを取得済みか否かを返します。
 *
 * @param[in] rwlock    ReaderWriterLockType オブジェクトへのポインタ
 *
 * @return  自スレッドで Writer ロックを取得済みか否か
 *
 * @pre
 *  - rwlock がアンロック、共有ロック、排他ロックのいずれかの状態である
 *
 * @details
 *  自スレッドが rwlock オブジェクトの Writer ロックを
 *  取得済みであれば true を、そうでなければ false を返します。
 *
 */
bool IsWriteLockHeldByCurrentThread(const ReaderWriterLockType* rwlock) NN_NOEXCEPT;


/**
 * @brief   自スレッドがロックオーナーか否かを返します。
 *
 * @param[in] rwlock    ReaderWriterLockType オブジェクトへのポインタ
 *
 * @return  自スレッドがロックオーナーか否か
 *
 * @pre
 *  - rwlock がアンロック、共有ロック、排他ロックのいずれかの状態である
 *
 * @details
 *  自スレッドが rwlock オブジェクトのロックオーナーなら true を、
 *  そうでなければ false を返します。
 *
 *  IsWriteLockHeldByCurrentThread() とは以下のケースにおいて返値が異なります。
 *
 *  ロックオーナーが設定されるのは Writer ロック取得時ですが、
 *  その後、Readers ロックを取得し、Writer ロックだけを解放すると、
 *  ロックオーナーが設定された状態で Readers ロックのみ獲得した状態になります。
 *  これは rwlock オブジェクトが排他ロック状態を維持した状態であり、
 *  この状態での各 API の返値は以下の通りです。
 *
 *  - IsReaderWriterLockOwnerThread() では true が返る
 *  - IsWriteLockHeldByCurrentThread() では false が返る
 *  - IsReadLockHeld() では true が返る
 *
 */
bool IsReaderWriterLockOwnerThread(const ReaderWriterLockType* rwlock) NN_NOEXCEPT;


//! @}

}} // namespace nn::os

