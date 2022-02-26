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
 * @brief   スレッドに関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/os/os_Result.h>
#include <nn/os/os_ThreadCommon.h>
#include <nn/os/os_Types.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn {

class   TimeSpan;

namespace os {

struct  ThreadType;
struct  MultiWaitHolderType;

//! @name スレッド関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   スレッドを生成します。
 *
 * @overloadlist{nn_os_CreateThread}
 *
 * @param[in] thread            ThreadType オブジェクトへのポインタ
 * @param[in] function          スレッドの関数エントリ
 * @param[in] argument          スレッドに渡される引数
 * @param[in] stack             スレッドのスタック領域の先頭アドレス
 * @param[in] stackSize         スレッドのスタックサイズ
 * @param[in] priority          スレッドの優先度
 * @param[in] idealCoreNumber   スレッドを作成するコア番号
 *
 * @retresult
 *   @handleresult{nn::os::ResultOutOfMemory}
 *   @handleresult{nn::os::ResultOutOfResource}
 * @endretresult
 *
 * @pre
 *  - thread が NotInitialized 状態もしくは初期化されていない状態である
 *  - stack が nn::os::ThreadStackAlignment でアライメントされている
 *  - stackSize > 0
 *  - stackSize % nn::os::ThreadStackAlignment == 0
 *  - nn::os::IsMemoryLocked (stack, stackSize) == false
 *  - nn::os::HighestThreadPriority <= priority <= nn::os::LowestThreadPriority
 *  - idealCoreNumber がターゲット動作環境毎に定義された有効な値である
 *
 * @post
 *  - thread が Initialized 状態である
 *
 */
nn::Result  CreateThread(ThreadType* thread, ThreadFunction function, void* argument, void* stack, size_t stackSize, int priority, int idealCoreNumber) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   スレッドを生成します。
 *
 * @overloadlist{nn_os_CreateThread}
 *
 * @param[in] thread      ThreadType オブジェクトへのポインタ
 * @param[in] function    スレッドの関数エントリ
 * @param[in] argument    スレッドに渡される引数
 * @param[in] stack       スレッドのスタック領域の先頭アドレス
 * @param[in] stackSize   スレッドのスタックサイズ
 * @param[in] priority    スレッドの優先度
 *
 * @retresult
 *   @handleresult{nn::os::ResultOutOfMemory}
 *   @handleresult{nn::os::ResultOutOfResource}
 * @endretresult
 *
 * @pre
 *  - thread が NotInitialized 状態もしくは初期化されていない状態である
 *  - stack が nn::os::ThreadStackAlignment でアライメントされている
 *  - stackSize > 0
 *  - stackSize % nn::os::ThreadStackAlignment == 0
 *  - nn::os::IsMemoryLocked (stack, stackSize) == false
 *  - nn::os::HighestThreadPriority <= priority <= nn::os::LowestThreadPriority
 *
 * @post
 *  - thread が Initialized 状態である
 *
 * @details
 *  スレッドを生成すると共に、スレッドオブジェクトを初期化します。
 *
 *  基本的にもう１つの CreateThread() と機能的に等価ですが、こちらはコア番号を
 *  指定できません。コア番号には各ターゲット環境における規定値が適用されます。
 *
 */
nn::Result  CreateThread(ThreadType* thread, ThreadFunction function, void* argument, void* stack, size_t stackSize, int priority) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   スレッドを削除します。
 *
 * @param[in] thread    ThreadType オブジェクトへのポインタ
 *
 * @pre
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *
 * @post
 *  - thread が NotInitialized 状態である
 *
 * @details
 *  スレッドオブジェクトを破棄すると共に、スレッドを削除します。
 *  スレッドが Started 状態の場合は、Exited になるまで待機してから削除します。
 */
void    DestroyThread(ThreadType* thread)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   スレッドの実行を開始します。
 *
 * @param[in] thread    ThreadType オブジェクトへのポインタ
 *
 * @pre
 *  - thread が Initialized 状態である
 *
 * @post
 *  - thread が Started 状態である
 *
 * @details
 *  対象スレッドの実行を開始します。
 */
void    StartThread(ThreadType* thread) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   自スレッドの ThreadType オブジェクトを取得します。
 *
 * @return  ThreadType オブジェクトへのポインタです。
 *
 * @pre
 *  - なし
 *
 * @post
 *  - 自スレッドの ThreadType オブジェクトへのポインタを返す
 *
 * @details
 *  自スレッドの ThreadType オブジェクトへのポインタを返します。
 */
ThreadType* GetCurrentThread()  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   対象スレッドが終了するのを待ちます。
 *
 * @param[in] thread    ThreadType オブジェクトへのポインタ
 *
 * @pre
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *
 * @post
 *  - thread が Exited 状態である
 *
 * @details
 *  対象スレッドが終了するまで待機（ブロッキング）します。
 */
void    WaitThread(ThreadType* thread)  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   同一優先度の他のスレッドに実行機会を譲ります。
 *
 * @pre
 *  - なし
 *
 * @post
 *  - 自スレッドと同一優先度を持つ他のスレッドに実行機会を譲る
 *
 * @details
 *  自スレッドが動作しているコアで動作可能なスレッドのうち、
 *  自スレッドと同じ優先度を持つ他のスレッドに実行機会を譲ります。
 *
 *  同一の優先度を持つ他スレッドが同一コアに存在しない場合は、
 *  そのまま自スレッドが動作を継続する場合があります。
 */
void    YieldThread()   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   自スレッドを指定された時間だけ休止します。
 *
 * @param[in] time      自スレッドを休止させる時間
 *
 * @pre
 *  - time >= 0
 *
 * @post
 *  - time で指定された時間だけ自スレッドを休止し、
 *    その後自動的に起床する
 *
 * @details
 *  自スレッドを指定された時間だけ休止します。@n
 *  他スレッドから起床させることは出来ません。@n
 *  休止時間の単位は、 TimeSpan 型に依存します。
 */
void    SleepThread(TimeSpan time)  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   対象スレッドの優先度を変更します。
 *
 * @param[in] thread    ThreadType オブジェクトへのポインタ
 * @param[in] priority  設定したい優先度
 *
 * @return  優先度変更前のスレッド優先度
 *
 * @pre
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *  - nn::os::HighestThreadPriority <= priority <= nn::os::LowestThreadPriority
 *
 * @post
 *  - thread が初期化状態である
 *  - thread の優先度を変更し、その優先度を持つスレッド群
 *    の中で最低の優先順位になる
 *
 */
int    ChangeThreadPriority(ThreadType* thread, int priority)  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   対象スレッドの本来の優先度を取得します。
 *
 * @param[in] thread    ThreadType オブジェクトへのポインタ
 *
 * @return  対象スレッドの本来の優先度です。
 *
 * @pre
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *
 * @post
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *  - thread の本来の優先度を返す
 *
 * @details
 *  対象スレッドの本来の優先度を返します。@n
 */
int     GetThreadPriority(const ThreadType* thread)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   対象スレッドの現在の優先度を取得します。
 *
 * @param[in] thread    ThreadType オブジェクトへのポインタ
 *
 * @return  対象スレッドの現在の優先度です。
 *
 * @pre
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *
 * @post
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *  - thread の現在の優先度を返す
 *
 * @details
 *  対象スレッドの現在の優先度を返します。@n
 *  ミューテックスの優先度継承が発動している場合は、継承中の優先度を返します。
 *
 *  スレッドの優先度は、他のスレッドによる ChangeThreadPriority() や、
 *  ミューテックス操作による優先度継承などによって、いつ変化するかは
 *  定かではありません。
 *
 *  現在の優先度の参照は、あくまで瞬間的な値を取得するものであるため、
 *  本 API の利用時にはこの点に注意して下さい。
 */
int     GetThreadCurrentPriority(const ThreadType* thread)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   対象スレッドのスレッド名を設定します。
 *
 * @param[in] thread        ThreadType オブジェクトへのポインタ
 * @param[in] threadName    ヌル文字で終端するスレッド名文字列の格納アドレス
 *
 * @pre
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *  - threadName の文字列の長さが nn::os::ThreadNameLengthMax 未満である
 *
 * @post
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *  - thread のスレッド名が threadName で示される名前に変更される
 *
 */
void    SetThreadName(ThreadType* thread, const char* threadName)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   対象スレッドのスレッド名が格納されているアドレスを設定します。
 *
 * @param[in] thread        ThreadType オブジェクトへのポインタ
 * @param[in] threadName    ヌル文字で終端するスレッド名文字列の格納アドレス
 *
 * @pre
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *
 * @post
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *  - thread のスレッド名が threadName で示される名前に変更される
 *
 */
void    SetThreadNamePointer(ThreadType* thread, const char* threadName)   NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   対象スレッドのスレッド名が格納されているアドレスを取得します。
 *
 * @param[in] thread        ThreadType オブジェクトへのポインタ
 *
 * @return  対象スレッドのスレッド名が格納されているアドレスです。
 *
 * @pre
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *
 * @post
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *
 */
const char* GetThreadNamePointer(const ThreadType* thread) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   現在自スレッドが動作しているコア番号を取得します。
 *
 * @deprecated
 *
 * @return  現在自スレッドが動作しているコア番号です。
 *
 * @details
 *  この API は削除予定です。
 *  nn::os::GetCurrentCoreNumber() を使用して下さい。
 *
 */
int     GetCurrentProcessorNumber() NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   現在自スレッドが動作しているコア番号を取得します。
 *
 * @return  現在自スレッドが動作しているコア番号です。
 *
 * @details
 *  現在自スレッドが動作しているコア番号を取得します。
 *  コア番号の取りうる範囲はターゲット環境依存です。
 *
 */
int     GetCurrentCoreNumber() NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   対象のスレッドのコア割り当てを設定します。
 *
 * @param[in] thread        ThreadType オブジェクトへのポインタ
 * @param[in] idealCore     優先して動作するコア番号
 * @param[in] affinityMask  コアごとのアフィニティマスク
 *
 * @pre
 *  - idealCore は指定可能な特別な値、もしくは有効なコア番号である
 *  - idealCore がコア番号の場合、idealCore で指定されるコアは affinityMask に含まれている
 *  - idealCore != nn::os::IdealCoreUseDefaultValue の場合、
 *    affinityMask には、使用可能なコアのみが 1 つ以上含まれている
 *
 * @details
 *  idealCore で優先して動作するコアを指定します。
 *  idealCore にはコア番号の他、以下の値を指定することが可能です。
 *
 *  - nn::os::IdealCoreUseDefaultValue: 自プログラムに設定された優先コアのみで動作するように
 *                                      優先コア、及びアフィニティマスクの両方を設定します。
 *                                      affinityMask で指定された値は無視されます。
 *  - nn::os::IdealCoreNoUpdate:        優先コア番号の更新を行いません。
 *  - nn::os::IdealCoreDontCare:        優先コア番号なしに設定します。
 *
 *  また、affinityMask にはスレッドが動作可能なコアを指定します。
 *  affinityMask のビット n が 1 であれば、コア番号 n のコア上で動作可能であることを表します。
 *
 *  idealCore に nn::os::IdealCoreNoUpdate を指定した場合、
 *  affinityMask のうち、現在設定されている優先コアを表すビットは 1 である必要があります。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では、優先して動作するコアを指定しても無視されるなど、
 *    Windows のスケジューラによる制約を受けます。
 *
 * @platformend
 *
 * @b 実機環境の場合
 *
 *  - 実機環境では、本 API 実行後すぐに対象スレッドのコアが移動するわけではありません。
 *    実際のコア移動は対象スレッドが再度動き出す際に実施されます。
 *
 */
void    SetThreadCoreMask(ThreadType* thread, int idealCore, Bit64 affinityMask)    NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   対象のスレッドのコア割り当てを取得します。
 *
 * @param[out] pOutIdealCore     優先して動作するコア番号を格納するバッファへのポインタ
 * @param[out] pOutAffinityMask  コアごとのアフィニティマスクを格納するバッファへのポインタ
 * @param[in]  thread            ThreadType オブジェクトへのポインタ
 *
 * @details
 *  pOutIdealCore には優先コアとして指定されているコア番号、または nn::os::IdealCoreDontCare が格納されます。@n
 *  *pOutAffinityMask のビット n が 1 であれば、対象のスレッドがコア番号 n のコア上で動作可能であることを表します。
 *
 *  pOutIdealCore、または pOutAffinityMask に NULL を指定した場合、値は取得しません。
 *
 */
void    GetThreadCoreMask(int* pOutIdealCore, Bit64* pOutAffinityMask, const ThreadType* thread)    NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   割り当て可能なコアを取得します。
 *
 * @return  割り当て可能なコア
 *
 * @details
 *  スレッドに対して割り当て可能なコアを取得します。
 *  割り当て可能なコアは、全てのスレッドで共通です。@n
 *  返り値のビット n が 1 であれば、コア番号 n のコア上で動作可能であることを表します。
 *
 */
nn::Bit64    GetThreadAvailableCoreMask()    NN_NOEXCEPT;

//--------------------------------------------------------------------------
/*
 * @brief   対象スレッドのスレッド識別子を取得します。
 *
 * @param[in] thread    ThreadType オブジェクトへのポインタ
 *
 * @return  対象スレッドのスレッド識別子です。
 *
 * @pre
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *
 * @details
 *  スレッドを識別可能な ID を取得します。
 *
 */
ThreadId    GetThreadId(const ThreadType* thread)    NN_NOEXCEPT;

//! @}

//! @name 多重待ち関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   multiWaitHolder を初期化し、thread と関連付けます。
 *
 * @overloadlist{nn_os_InitializeMultiWaitHolder}
 *
 * @param[in] multiWaitHolder   多重待ちオブジェクトへのポインタ
 * @param[in] thread            紐付ける先のスレッドへのポインタ
 *
 * @pre
 *  - multiWaitHolder が未初期化状態である
 *  - thread が Initialized, Started, Exited のいずれかの状態である
 *
 * @post
 *  - multiWaitHolder が初期化状態である（thread に関連付けられる）
 *  - @ref nn::os::GetMultiWaitHolderUserData "nn::os::GetMultiWaitHolderUserData(multiWaitHolder)" == 0
 *
 */
void    InitializeMultiWaitHolder(MultiWaitHolderType* multiWaitHolder, ThreadType* thread)   NN_NOEXCEPT;

//! @}


}} // namespace nn::os

