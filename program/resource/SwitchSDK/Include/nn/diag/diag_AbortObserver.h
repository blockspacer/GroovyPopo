/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

/**
* @file
* @brief   アボートオブザーバ関連の API 宣言
*/

#include <nn/nn_Common.h>
#include <nn/diag/diag_AbortTypes.h>

namespace nn { namespace diag {

/**
*   @brief      アボートオブザーバの型です。
*
*   @param[in]  abortInfo   アボートに関する情報
*
*   @details
* アボートオブザーバは、アボート時に呼び出されるコールバック関数です。
*
* 複数のスレッドで同時に呼び出される可能性がありますので、再入可能である必要があります。
*
* 例外安全でない関数で呼び出される可能性がありますので、例外を発生させてはいけません。
*
* 関数内では、ログの出力などの処理を行うことが想定されています。
*
* アボートオブザーバが、return されない場合の挙動は未定義です。
*
*   @attention
* アボートオブザーバは、製品環境でも無効化されません。
*
*/
typedef void (*AbortObserver)(const AbortInfo& abortInfo);

/**
*   @brief      アボートオブザーバのホルダです。
*
*   @details    InitializeAbortObserverHolder() で初期化してから使用します。
*/
struct AbortObserverHolder
{
    AbortObserver observer;     //!< アボートオブザーバへのポインタ
    AbortObserverHolder* next;  //!< SDK が内部で使用するメンバ
    bool isRegistered;          //!< SDK が内部で使用するメンバ
};

/**
*   @brief      アボートオブザーバのホルダを初期化します。
*
*   @param      abortObserverHolder   初期化するアボートオブザーバのホルダ
*   @param      abortObserver         アボートオブザーバへの関数ポインタ
*   @pre        abortObserverHolder は未登録である必要があります。
*   @post       abortObserverHolder は初期化され、abortObserver が設定されます。
*/
void InitializeAbortObserverHolder(
    AbortObserverHolder* abortObserverHolder,
    AbortObserver abortObserver) NN_NOEXCEPT;

#if !defined(NN_SDK_BUILD_LIBRARY) // SDK はコールバック関数を差し替えてはいけない
/**
*   @brief      アボートオブザーバを登録します。
*
*   @details    登録されたアボートオブザーバは、アボート時に呼び出されます。
*
*   @param[in]  abortObserverHolder 登録するアボートオブザーバのホルダのポインタ
*   @pre        abortObserverHolder は未登録である必要があります。
*   @post       abortObserverHolder が登録されます。
*/
void RegisterAbortObserver(AbortObserverHolder* abortObserverHolder) NN_NOEXCEPT;

/**
*   @brief      アボートオブザーバの登録を解除します。
*
*   @param[in]  abortObserverHolder 登録を解除するアボートオブザーバのホルダのポインタ
*   @pre        abortObserverHolder は登録済みである必要があります。
*   @post       abortObserverHolder は未登録状態になります。
*/
void UnregisterAbortObserver(AbortObserverHolder* abortObserverHolder) NN_NOEXCEPT;

/**
*   @brief      アボート時に SDK がデフォルトで呼び出すアボートオブザーバを有効化するかどうか設定します。
*
*   @param[in]  enable  有効化するときは true、無効化するときは false を渡す。
*/
void EnableDefaultAbortObserver(bool enable) NN_NOEXCEPT;
#endif

}}
