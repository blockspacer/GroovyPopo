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
 * @brief   メッセージキューに関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/os/os_MultipleWaitCommon.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn {

class   TimeSpan;

namespace os {

struct  MessageQueueType;
struct  MultiWaitHolderType;

//! @name メッセージキュー関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューオブジェクトを初期化します。
 *
 * @param[in] messageQueue  MessageQueueType オブジェクトへのポインタ
 * @param[in] buffer        メッセージキューのバッファ先頭アドレス
 * @param[in] count         メッセージキューに格納できるデータの最大個数
 *
 * @pre
 *  - messageQueue が未初期化状態もしくは初期化されていない状態である
 *  - buffer の先に count * sizeof(uintptr_t) バイト分のメモリ領域がある
 *  - count >= 1
 *
 * @post
 *  - messageQueue が初期化状態（Empty 状態）である
 *
 */
void    InitializeMessageQueue( MessageQueueType* messageQueue, uintptr_t* buffer, size_t count)    NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューオブジェクトをファイナライズします。
 *
 * @param[in] messageQueue  MessageQueueType オブジェクトへのポインタ
 *
 * @pre
 *  - messageQueue が初期化状態である
 *  - messageQueue を待機しているスレッドがいない
 *
 * @post
 *  - messageQueue が未初期化状態である
 *
 */
void    FinalizeMessageQueue(MessageQueueType* messageQueue)    NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューの最後尾にデータを１つ送信します。
 *
 * @param[in] messageQueue  MessageQueueType オブジェクトへのポインタ
 * @param[in] data          送信データ
 *
 * @pre
 *  - messageQueue が初期化状態である
 *
 * @post
 *  - messageQueue が NotEmpty 状態である
 *  - messageQueue のデータ列の最後尾に data を追加する
 *
 */
void    SendMessageQueue(MessageQueueType* messageQueue, uintptr_t data)    NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューの最後尾にデータを１つ送信します。
 *
 * @param[in] messageQueue  MessageQueueType オブジェクトへのポインタ
 * @param[in] data          送信データ
 *
 * @return  メッセージキューへの送信の成否を示します。
 *
 * @pre
 *  - messageQueue が初期化状態である
 *
 * @post
 *  - 返値が true の場合
 *    - messageQueue が NotEmpty 状態である
 *    - messageQueue のデータ列の最後尾に data を追加
 *  - 返値が false の場合
 *    - messageQueue が初期化状態である （状態は変化しない）
 *
 */
bool    TrySendMessageQueue(MessageQueueType* messageQueue, uintptr_t data) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューの最後尾にデータを１つ送信します。
 *
 * @param[in] messageQueue  MessageQueueType オブジェクトへのポインタ
 * @param[in] data          送信データ
 * @param[in] timeout       最大待ち時間
 *
 * @return  メッセージキューへの送信の成否を示します。
 *
 * @pre
 *  - messageQueue が初期化状態である
 *  - timeout >= 0
 *
 * @post
 *  - 返値が true の場合
 *    - messageQueue が NotEmpty 状態である
 *    - messageQueue のデータ列の最後尾に data を追加
 *  - 返値が false の場合
 *    - messageQueue が初期化状態である （状態は変化しない）
 *
 */
bool    TimedSendMessageQueue(MessageQueueType* messageQueue, uintptr_t data, TimeSpan timeout) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューの先頭にデータを１つ送信します。
 *
 * @param[in] messageQueue  MessageQueueType オブジェクトへのポインタ
 * @param[in] data          送信データ
 *
 * @pre
 *  - messageQueue が初期化状態である
 * @post
 *  - messageQueue が NotEmpty 状態である
 *  - messageQueue のデータ列の先頭に data を追加する
 *
 */
void    JamMessageQueue(MessageQueueType* messageQueue, uintptr_t data) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューの先頭にデータを１つ送信します。
 *
 * @param[in] messageQueue  MessageQueueType オブジェクトへのポインタ
 * @param[in] data          送信データ
 *
 * @return  メッセージキューへの送信の成否を示します。
 *
 * @pre
 *  - messageQueue が初期化状態である
 *
 * @post
 *  - 返値が true の場合
 *    - messageQueue が NotEmpty 状態である
 *    - messageQueue のデータ列の先頭に data を追加する
 *  - 返値が false の場合
 *    - messageQueue が初期化状態である （状態は変化しない）
 *
 */
bool    TryJamMessageQueue(MessageQueueType* messageQueue, uintptr_t data)  NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューの先頭にデータを１つ送信します。
 *
 * @param[in] messageQueue  MessageQueueType オブジェクトへのポインタ
 * @param[in] data          送信データ
 * @param[in] timeout       最大待ち時間
 *
 * @return  メッセージキューへの送信の成否を示します。
 *
 * @pre
 *  - messageQueue が初期化状態である
 *  - timeout >= 0
 *
 * @post
 *  - 返値が true の場合
 *    - messageQueue が NotEmpty 状態である
 *    - messageQueue のデータ列の先頭に data を追加する
 *  - 返値が false の場合
 *    - messageQueue が初期化状態である （状態は変化しない）
 *
 */
bool    TimedJamMessageQueue(MessageQueueType* messageQueue, uintptr_t data, TimeSpan timeout)  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューの先頭からデータを１つ受信します。
 *
 * @param[out] outData      受信データの格納先
 * @param[in]  messageQueue MessageQueueType オブジェクトへのポインタ
 *
 * @pre
 *  - messageQueue が初期化状態である
 *
 * @post
 *  - messageQueue が NotFull 状態である
 *  - messageQueue のデータ列の先頭のデータを取得し outData に格納する
 *
 */
void    ReceiveMessageQueue(uintptr_t* outData, MessageQueueType* messageQueue) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューの先頭からデータを１つ受信します。
 *
 * @param[out] outData      受信データの格納先
 * @param[in]  messageQueue MessageQueueType オブジェクトへのポインタ
 *
 * @return  メッセージキューからの受信の成否を示します。
 *
 * @pre
 *  - messageQueue が初期化状態である
 *
 * @post
 *  - 返値が true の場合
 *    - messageQueue が NotFull 状態である
 *    - messageQueue のデータ列の先頭のデータを取得し outData に格納する
 *  - 返値が false の場合
 *    - messageQueue が初期化状態である （状態は変化しない）
 *
 */
bool    TryReceiveMessageQueue(uintptr_t* outData, MessageQueueType* messageQueue)  NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューの先頭からデータを１つ受信します。
 *
 * @param[out] outData      受信データの格納先
 * @param[in]  messageQueue MessageQueueType オブジェクトへのポインタ
 * @param[in]  timeout      最大待ち時間
 *
 * @return  メッセージキューからの受信の成否を示します。
 *
 * @pre
 *  - messageQueue が初期化状態である
 *  - timeout >= 0
 *
 * @post
 *  - 返値が true の場合
 *    - messageQueue が NotFull 状態である
 *    - messageQueue のデータ列の先頭のデータを取得し outData に格納する
 *  - 返値が false の場合
 *    - messageQueue が初期化状態である （状態は変化しない）
 *
 */
bool    TimedReceiveMessageQueue(uintptr_t* outData, MessageQueueType* messageQueue, TimeSpan timeout)  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューの先頭データを参照します。
 *
 * @param[out] outData      参照データの格納先
 * @param[in]  messageQueue MessageQueueType オブジェクトへのポインタ
 *
 * @pre
 *  - messageQueue が初期化状態である
 *
 * @post
 *  - messageQueue が初期化状態である （状態は変化しない）
 *  - messageQueue のデータ列の先頭のデータを参照し、outData に格納する
 *
 */
void    PeekMessageQueue(uintptr_t* outData, const MessageQueueType* messageQueue)    NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューの先頭データを参照します。
 *
 * @param[out] outData      参照データの格納先
 * @param[in]  messageQueue MessageQueueType オブジェクトへのポインタ
 *
 * @return  メッセージキューからのデータ参照の成否を示します。
 *
 * @pre
 *  - messageQueue が初期化状態である
 * @post
 *  - 返値が true の場合
 *    - messageQueue が初期化状態である （状態は変化しない）
 *    - messageQueue のデータ列の先頭のデータを参照し outData に格納する
 *
 *  - 返値が false の場合
 *    - messageQueue が初期化状態である （状態は変化しない）
 *
 */
bool    TryPeekMessageQueue(uintptr_t* outData, const MessageQueueType* messageQueue) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューの先頭データを参照します。
 *
 * @param[out] outData      参照データの格納先
 * @param[in]  messageQueue MessageQueueType オブジェクトへのポインタ
 * @param[in]  timeout      最大待ち時間
 *
 * @return  メッセージキューからのデータ参照の成否を示します。
 *
 * @pre
 *  - messageQueue が初期化状態である
 *  - timeout >= 0
 *
 * @post
 *  - 返値が true の場合
 *    - messageQueue が初期化状態である （状態は変化しない）
 *    - messageQueue のデータ列の先頭のデータを参照し outData に格納する
 *  - 返値が false の場合
 *    - messageQueue が初期化状態である （状態は変化しない）
 *
 */
bool    TimedPeekMessageQueue(uintptr_t* outData, const MessageQueueType* messageQueue, TimeSpan timeout) NN_NOEXCEPT;

//! @}

//! @name 多重待ち関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   multiWaitHolder を初期化し、messageQueue と関連付けます。
 *
 * @overloadlist{nn_os_InitializeMultiWaitHolder}
 *
 * @param[in] multiWaitHolder   多重待ちオブジェクトへのポインタ
 * @param[in] messageQueue      紐付ける先のメッセージキューへのポインタ
 * @param[in] waitType          メッセージキューの待ち種別
 *
 * @pre
 *  - multiWaitHolder が未初期化状態である
 *  - messageQueue が初期化状態である
 *
 * @post
 *  - multiWaitHolder が初期化状態である（messageQueue に関連付けられる）
 *  - @ref nn::os::GetMultiWaitHolderUserData "nn::os::GetMultiWaitHolderUserData(multiWaitHolder)" == 0
 *
 */
void    InitializeMultiWaitHolder(MultiWaitHolderType* multiWaitHolder, MessageQueueType* messageQueue, MessageQueueWaitType waitType)    NN_NOEXCEPT;

//! @}


}} // namespace nn::os

