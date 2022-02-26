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
 * @brief   メッセージキューに関する公開ヘッダファイル
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_TimeSpan.h>
#include <nn/os/os_MessageQueueTypes.h>
#include <nn/os/os_MessageQueueApi.h>

namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   メッセージキューを扱うためのクラスです。
 *
 * @details
 *  メッセージキューはデータの送受信を使ってスレッド間で同期を
 *  確立するための機能です。
 *
 *  nn::os::MessageQueue クラスは、@ref nn::os::MessageQueueType オブジェクトを
 *  使用する同期制御 API の呼び出しをラッピングしたユーティリティクラスです。@n
 *  本クラスが提供するメンバ関数は、@ref nn::os::MessageQueueType 型を
 *  引数にとる @ref nn::os::SendMessageQueue() などの API を発行する形で
 *  実装されています。
 */
class MessageQueue
{
    NN_DISALLOW_COPY( MessageQueue );
    NN_DISALLOW_MOVE( MessageQueue );

public:
    /**
     * @brief   メッセージキューオブジェクトを構築し初期化します。
     *
     * @param[in] buffer    メッセージキューのバッファ先頭アドレス
     * @param[in] count     メッセージキューに格納できるデータの最大個数
     *
     * @details
     *  コンストラクタです。@n
     *  メッセージキューオブジェクトを構築し、指定されたパラメータで
     *  初期化します。@n
     *  メッセージキューのバッファ領域には、uintptr_t 型の配列を渡して下さい。
     *  ここには count * sizeof(uintptr_t) だけの領域が確保されていなければ
     *  なりません。@n
     *  詳細は nn::os::InitializeMessageQueue() を参照して下さい。
     */
    explicit MessageQueue(uintptr_t* buffer, size_t count) NN_NOEXCEPT
    {
        nn::os::InitializeMessageQueue(&m_MessageQueue, buffer, count);
    }

    /**
     * @brief  デストラクタです。
     *
     * @details
     *  メッセージキューオブジェクトを破棄します。@n
     *  詳細は nn::os::FinalizeMessageQueue() を参照して下さい。
     */
    ~MessageQueue() NN_NOEXCEPT
    {
        nn::os::FinalizeMessageQueue(&m_MessageQueue);
    }


    /**
     * @brief   メッセージキューの最後尾にデータを送信します。
     *
     * @param[in] data      送信データ
     *
     * @details
     *  メッセージキューの最後尾へデータを送信します。@n
     *  メッセージキューに空きがない場合、データが送信可能になるまで待機してから
     *  データを送信します。@n
     *  詳細は nn::os::SendMessageQueue() を参照して下さい。
     */
    void Send(uintptr_t data) NN_NOEXCEPT
    {
        nn::os::SendMessageQueue(&m_MessageQueue, data);
    }

    /**
     * @brief   メッセージキューの最後尾へデータ送信を試みます。
     *
     * @param[in] data      送信データ
     *
     * @return  データ送信の成否を返します。
     *
     * @details
     *  メッセージキューの最後尾へデータ送信を試みます。@n
     *  メッセージキューに空きがある場合、データ送信して true を返します。@n
     *  メッセージキューに空きがない場合、データ送信せずに false を返します。@n
     *  詳細は nn::os::TrySendMessageQueue() を参照して下さい。
     */
    bool TrySend(uintptr_t data) NN_NOEXCEPT
    {
        return nn::os::TrySendMessageQueue(&m_MessageQueue, data);
    }

    /**
     * @brief   メッセージキューの最後尾へデータ送信を時限付きで行ないます。
     *
     * @param[in] data      送信データ
     * @param[in] timeout   最大待ち時間
     *
     * @return  データ送信の成否を返します。
     *
     * @details
     *  メッセージキューの最後尾へデータを送信します。@n
     *  データ送信に成功した場合は true を返します。@n
     *  メッセージキューに空きがない場合、データが送信可能になるまで待機してから
     *  データを送信します。ただし、timeout で指定した時間までにデータを
     *  送信できなかった場合 false を返します。@n
     *  詳細は nn::os::TimedSendMessageQueue() を参照して下さい。
     */
    bool TimedSend(uintptr_t data, nn::TimeSpan timeout) NN_NOEXCEPT
    {
        return nn::os::TimedSendMessageQueue(&m_MessageQueue, data, timeout);
    }


    /**
     * @brief   メッセージキューの先頭にデータを送信します。
     *
     * @param[in] data      送信データ
     *
     * @details
     *  メッセージキューの先頭へデータを送信します。@n
     *  メッセージキューに空きがない場合、データが送信可能になるまで待機してから
     *  データを送信します。@n
     *  詳細は nn::os::JamMessageQueue() を参照して下さい。
     */
    void Jam(uintptr_t data) NN_NOEXCEPT
    {
        nn::os::JamMessageQueue(&m_MessageQueue, data);
    }

    /**
     * @brief   メッセージキューの先頭へのデータ送信を試みます。
     *
     * @param[in] data      送信データ
     *
     * @return  データ送信の成否を返します。
     *
     * @details
     *  メッセージキューの先頭へデータ送信を試みます。@n
     *  メッセージキューに空きがある場合、データ送信して true を返します。@n
     *  メッセージキューに空きがない場合、データ送信せずに false を返します。@n
     *  詳細は nn::os::TryJamMessageQueue() を参照して下さい。
     */
    bool TryJam(uintptr_t data) NN_NOEXCEPT
    {
        return nn::os::TryJamMessageQueue(&m_MessageQueue, data);
    }

    /**
     * @brief   メッセージキューの先頭へデータ送信を時限付きで行ないます。
     *
     * @param[in] data      送信データ
     * @param[in] timeout   最大待ち時間
     *
     * @return  データ送信の成否を返します。
     *
     * @details
     *  メッセージキューの先頭へデータを送信します。@n
     *  データ送信に成功した場合は true を返します。@n
     *  メッセージキューに空きがない場合、データが送信可能になるまで待機してから
     *  データを送信します。ただし、timeout で指定した時間までにデータを
     *  送信できなかった場合 false を返します。@n
     *  詳細は nn::os::TimedJamMessageQueue() を参照して下さい。
     */
    bool TimedJam(uintptr_t data, nn::TimeSpan timeout) NN_NOEXCEPT
    {
        return nn::os::TimedJamMessageQueue(&m_MessageQueue, data, timeout);
    }


    /**
     * @brief   メッセージキューの先頭からデータを受信します。
     *
     * @param[out] outData  受信データの格納先
     *
     * @details
     *  メッセージキューの先頭からデータを１つ受信します。@n
     *  メッセージキューにデータがない場合、データが送信されるまで待機してから
     *  データを受信します。@n
     *  詳細は nn::os::ReceiveMessageQueue() を参照して下さい。
     */
    void Receive(uintptr_t* outData) NN_NOEXCEPT
    {
        nn::os::ReceiveMessageQueue(outData, &m_MessageQueue);
    }

    /**
     * @brief   メッセージキューの先頭からデータ受信を試みます。
     *
     * @param[out] outData  受信データの格納先
     *
     * @return  データ受信の成否を返します。
     *
     * @details
     *  メッセージキューの先頭からデータ受信を試みます。@n
     *  メッセージキューにデータがある場合、データを受信して true を返します。@n
     *  メッセージキューにデータがない場合、データを受信せずに false を返します。@n
     *  詳細は nn::os::TryReceiveMessageQueue() を参照して下さい。
     */
    bool TryReceive(uintptr_t* outData) NN_NOEXCEPT
    {
        return nn::os::TryReceiveMessageQueue(outData, &m_MessageQueue);
    }

    /**
     * @brief   メッセージキューの先頭からデータ受信を時限付きで行ないます。
     *
     * @param[out] outData  受信データの格納先
     * @param[in]  timeout  最大待ち時間
     *
     * @return  データ受信の成否を返します。
     *
     * @details
     *  メッセージキューの先頭からデータを１つ受信します。@n
     *  データ受信に成功した場合は true を返します。@n
     *  メッセージキューにデータがない場合、データが送信されるまで待機してから
     *  データを受信します。ただし、timeout で指定した時間までにデータを
     *  受信できなかった場合 false を返します。@n
     *  詳細は nn::os::TimedReceiveMessageQueue() を参照して下さい。
     */
    bool TimedReceive(uintptr_t* outData, nn::TimeSpan timeout) NN_NOEXCEPT
    {
        return nn::os::TimedReceiveMessageQueue(outData, &m_MessageQueue, timeout);
    }


    /**
     * @brief   メッセージキューの先頭データを参照します。
     *
     * @param[out] outData  参照データの格納先
     *
     * @details
     *  メッセージキューの先頭にあるデータを参照します。@n
     *  メッセージキューにデータがない場合、データが送信されるまで待機してから
     *  データを参照します。@n
     *  データ参照後も、メッセージキュー内のデータは取り出されずに残ります。@n
     *  詳細は nn::os::PeekMessageQueue() を参照して下さい。
     */
    void Peek(uintptr_t* outData) NN_NOEXCEPT
    {
        nn::os::PeekMessageQueue(outData, &m_MessageQueue);
    }

    /**
     * @brief   メッセージキューの先頭データの参照を試みます。
     *
     * @param[out] outData  参照データの格納先
     *
     * @return  データ参照の成否を返します。
     *
     * @details
     *  メッセージキューの先頭にあるデータの参照を試みます。@n
     *  メッセージキューにデータがある場合、データを参照して true を返します。@n
     *  メッセージキューにデータがない場合、データを参照せずに false を返します。@n
     *  データ参照後も、メッセージキュー内のデータは取り出されずに残ります。@n
     *  詳細は nn::os::TryPeekMessageQueue() を参照して下さい。
     */
    bool TryPeek(uintptr_t* outData) NN_NOEXCEPT
    {
        return nn::os::TryPeekMessageQueue(outData, &m_MessageQueue);
    }

    /**
     * @brief   メッセージキューの先頭データの参照を時限付きで行ないます。
     *
     * @param[out] outData  参照データの格納先
     * @param[in]  timeout  最大待ち時間
     *
     * @return  データ参照の成否を返します。
     *
     * @details
     *  メッセージキューの先頭にあるデータを参照します。@n
     *  メッセージキューにデータがある場合、true を返します。@n
     *  メッセージキューにデータがない場合、データが送信されるまで待機してから
     *  データを参照します。timeout で指定した時間までデータを参照できなかった
     *  場合 false を返します。@n
     *  データ参照後も、メッセージキュー内のデータは取り出されずに残ります。@n
     *  詳細は nn::os::TimedPeekMessageQueue() を参照して下さい。
     */
    bool TimedPeek(uintptr_t* outData, nn::TimeSpan timeout) NN_NOEXCEPT
    {
        return nn::os::TimedPeekMessageQueue(outData, &m_MessageQueue, timeout);
    }


    /**
     * @brief   MessageQueueType オブジェクトへの参照を返します。
     *
     * @return  MessageQueueType オブジェクトへの参照
     *
     * @details
     *  自インスタンスが持つ MessageQueueType オブジェクトへの参照を返します。
     *  この変換演算子があるため、MessageQueueType& を引数にとる関数に対して、
     *  MessageQueue& を指定することが可能です。
     */
    NN_IMPLICIT operator MessageQueueType&() NN_NOEXCEPT
    {
        return m_MessageQueue;
    }

    /**
     * @brief   MessageQueueType オブジェクトへの const 参照を返します。
     *
     * @return  MessageQueueType オブジェクトへの const 参照
     *
     * @details
     *  自インスタンスが持つ MessageQueueType オブジェクトへの const 参照を返します。
     *  この変換演算子があるため、const MessageQueueType& を引数にとる関数に対して、
     *  MessageQueue& を指定することが可能です。
     */
    NN_IMPLICIT operator const MessageQueueType&() const NN_NOEXCEPT
    {
        return m_MessageQueue;
    }

    /**
     * @brief   自インスタンスの MessageQueueType オブジェクトへのポインタを返します。
     *
     * @return  MessageQueueType オブジェクトへのポインタ
     *
     * @details
     *  自インスタンスが持つ MessageQueueType オブジェクトへのポインタを返します。
     */
    MessageQueueType* GetBase() NN_NOEXCEPT
    {
        return &m_MessageQueue;
    }


private:
    MessageQueueType   m_MessageQueue;
};

//--------------------------------------------------------------------------

}} // namespace nn::os

