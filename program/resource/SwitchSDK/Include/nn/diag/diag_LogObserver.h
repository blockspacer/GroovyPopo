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
* @brief   ログオブザーバ関連の型と関数の宣言
*/

#include <nn/nn_Common.h>
#include <nn/diag/diag_LogTypes.h>

namespace nn { namespace diag {

/**
*   @brief      ログの本体です。
*/
struct LogBody
{
    const char* message;    //!< ログの文字列を指すポインタ
    size_t messageBytes;    //!< ログの文字列のバイト数
    bool isHead;            //!< 分割されたログの先頭か
    bool isTail;            //!< 分割されたログの末尾か
};

/**
*   @brief      ログオブザーバの型です。
*
*   @details    ログが diag ライブラリのバッファに収まりきらないときは、
*               ログが分割されて、ログオブザーバが複数回呼び出されます。
*
*   @param[in]  metaInfo        ログのメタ情報
*   @param[in]  logBody         ログの本体
*   @param[in]  argument        InitializeLogObserverHolder で渡した引数
*/
typedef void (*LogObserver)(
    const LogMetaData& metaInfo,
    const LogBody& logBody,
    void* argument);

/**
*   @brief      ログオブザーバのホルダです。
*
*   @details    InitializeLogObserverHolder() で初期化してから使用します。
*/
struct LogObserverHolder
{
    LogObserver logObserver;    //!< ログオブザーバへのポインタ
    LogObserverHolder* next;    //!< SDK が内部で使用するメンバ
    bool isRegistered;          //!< SDK が内部で使用するメンバ
    void* argument;             //!< ログオブザーバに渡す引数
};

/**
*   @brief      ログオブザーバのホルダを初期化します。
*
*   @param      logObserverHolder   登録を解除するログオブザーバのホルダ
*   @param      logObserver         ログオブザーバへの関数ポインタ
    @param      argument            ログオブザーバに渡す引数
*   @pre        logObserverHolder は未登録である必要があります。
*   @post       logObserverHolder は初期化され、logObserver が設定されます。
*/
void InitializeLogObserverHolder(
    LogObserverHolder* logObserverHolder,
    LogObserver logObserver,
    void* argument) NN_NOEXCEPT;

/**
*   @brief      ログオブザーバを登録します。
*
*   @details    登録されたログオブザーバは NN_LOG などでログが出力されるときに呼び出されます。
*
*   @param      logObserverHolder   登録するログオブザーバのホルダ
*   @pre        logObserverHolder 初期化済みで、かつ未登録である必要があります。
*   @post       logObserverHolder が登録されます。
*/
void RegisterLogObserver(LogObserverHolder* logObserverHolder) NN_NOEXCEPT;

/**
*   @brief      ログオブザーバの登録を解除します。
*
*   @param      logObserverHolder   登録を解除するログオブザーバのホルダ
*   @pre        logObserverHolder は登録済みである必要があります。
*   @post       logObserverHolder は未登録状態になります。
*/
void UnregisterLogObserver(LogObserverHolder* logObserverHolder) NN_NOEXCEPT;

}}

