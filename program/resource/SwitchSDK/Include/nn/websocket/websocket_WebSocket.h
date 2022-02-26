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

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <cstdarg>
#include <nn/websocket/websocket_ConnectionBase.h>
#include <nn/websocket/websocket_ResultPublic.h>
#include <nn/websocket/websocket_Types.h>

namespace nn { namespace websocket { namespace detail { namespace core {

class WebSocketImpl;

}}}}

namespace nn { namespace websocket {

/**
 * @brief   WebSocket オブジェクト
 *
 * @details
 *  WebSocket の通信処理を行います。
 *
 *  本クラスは RFC7692 で規定されている圧縮拡張（permessage-deflate）をサポートしています。
 */
class WebSocket
{
private:
    NN_DISALLOW_COPY(WebSocket);
    NN_DISALLOW_MOVE(WebSocket);

public:
    /**
     * @brief   オープンイベントハンドラ
     *
     * @param[in]   pParam  ユーザーパラメータ
     *
     * @details
     *  本イベントハンドラは Open() が成功した時に 1 回呼び出されます。
     */
    using OpenEventHandler = void (*)(void* pParam);

    /**
     * @brief   メッセージ受信イベントハンドラ
     *
     * @param[in]   pBuffer 受信したデータを格納したバッファ
     * @param[in]   size    受信したデータのサイズ
     * @param[in]   isText  テキストデータかどうか
     * @param[in]   pParam  ユーザーパラメータ
     *
     * @details
     *  本イベントハンドラはメッセージを受信する度に呼び出されます。
     */
    using MessageEventHandler = void (*)(void* pBuffer, size_t size, bool isText, void* pParam);

    /**
     * @brief   クローズイベントハンドラ
     *
     * @param[in]   code    ステータスコード
     * @param[in]   pReason 切断理由
     * @param[in]   length  切断理由のバイト数
     * @param[in]   pParam  ユーザーパラメータ
     *
     * @details
     *  本イベントハンドラはサーバーとの接続が切断された時に 1 回呼び出されます。
     *
     *  切断理由は UTF-8 フォーマットの文字列です。
     *
     * @note
     *  ステータスコードの詳細は RFC6455#section-7.4 を参照してください。
     */
    using CloseEventHandler = void (*)(uint16_t code, const char* pReason, size_t length, void* pParam);

    /**
     * @brief   エラーイベントハンドラ
     *
     * @param[in]   result  処理結果
     * @param[in]   pParam  ユーザーパラメータ
     *
     * @details
     *  本イベントハンドラはエラーが発生した時に呼び出されます。
     */
    using ErrorEventHandler = void (*)(nn::Result result, void* pParam);

public:
    //! @name コンストラクタとデストラクタ
    //! @{

    /**
     * @brief   コンストラクタ
     *
     * @details
     */
    WebSocket() NN_NOEXCEPT;

    /**
     * @brief   デストラクタ
     *
     * @details
     */
    ~WebSocket() NN_NOEXCEPT;

    //! @}
    //! @name 初期化と終了
    //! @{

    /**
     * @brief   初期化します。
     *
     * @retresult
     *  @handleresult{
     *      nn::ResultSuccess,
     *      処理に成功しました。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultOutOfMemory,
     *      メモリの確保に失敗しました。
     *  }
     * @endretresult
     *
     * @pre
     *  - 未初期化状態である。
     *
     * @details
     */
    nn::Result Initialize() NN_NOEXCEPT;

    /**
     * @brief   終了します。
     *
     * @details
     */
    void Finalize() NN_NOEXCEPT;

    //! @}
    //! @name 設定
    //! @{

    /**
     * @brief   コネクションを設定します。
     *
     * @param[in]   pConnection コネクション
     *
     * @pre
     *  - pConnection != nullptr
     *  - 初期化済みである。
     *
     * @details
     *
     * @note
     *  コネクションは本インスタンスを破棄するか Finalize() を呼び出すまで保持しておく必要があります。
     */
    void SetConnection(ConnectionBase* pConnection) NN_NOEXCEPT;

    /**
     * @brief   接続処理のキャンセル関数を設定します。
     *
     * @param[in]   pFunction   関数
     * @param[in]   pParam      関数のユーザーパラメータ
     *
     * @pre
     *  - 初期化済みである。
     *
     * @details
     *  Open() の呼び出しから完了するまでの間、本関数で設定したキャンセル関数が定期的に呼び出されます。@n
     *  キャンセル関数が true を返すことで、接続処理を中断することができます。
     *
     * @note
     *  本関数で設定したキャンセル関数は、 SetConnection() で設定したコネクションに対しても自動で設定されます。
     */
    void SetCancellationFunction(CancellationFunction pFunction, void* pParam) NN_NOEXCEPT;

    /**
     * @brief   オープンイベントハンドラを設定します。
     *
     * @param[in]   pHandler    イベントハンドラ
     * @param[in]   pParam      イベントハンドラのユーザーパラメータ
     *
     * @pre
     *  - 初期化済みである。
     *
     * @details
     */
    void SetOpenEventHandler(OpenEventHandler pHandler, void* pParam) NN_NOEXCEPT;

    /**
     * @brief   メッセージ受信イベントハンドラを設定します。
     *
     * @param[in]   pHandler    イベントハンドラ
     * @param[in]   pParam      イベントハンドラのユーザーパラメータ
     *
     * @pre
     *  - 初期化済みである。
     *
     * @details
     */
    void SetMessageEventHandler(MessageEventHandler pHandler, void* pParam) NN_NOEXCEPT;

    /**
     * @brief   クローズイベントハンドラを設定します。
     *
     * @param[in]   pHandler    イベントハンドラ
     * @param[in]   pParam      イベントハンドラのユーザーパラメータ
     *
     * @pre
     *  - 初期化済みである。
     *
     * @details
     */
    void SetCloseEventHandler(CloseEventHandler pHandler, void* pParam) NN_NOEXCEPT;

    /**
     * @brief   エラーイベントハンドラを設定します。
     *
     * @param[in]   pHandler    イベントハンドラ
     * @param[in]   pParam      イベントハンドラのユーザーパラメータ
     *
     * @pre
     *  - 初期化済みである。
     *
     * @details
     */
    void SetErrorEventHandler(ErrorEventHandler pHandler, void* pParam) NN_NOEXCEPT;

    /**
     * @brief   リクエストヘッダーをクリアします。
     *
     * @pre
     *  - 初期化済みである。
     *
     * @details
     *  AddRequestHeader() や AddRequestHeaderWithVarargs() で追加したリクエストヘッダーをすべて削除します。
     */
    void ClearRequestHeader() NN_NOEXCEPT;

    /**
     * @brief   リクエストヘッダーを追加します。
     *
     * @param[in]   pName   フィールド名
     * @param[in]   pValue  フィールド値
     * @param[in]   ...     フィールド値の可変長パラメータ
     *
     * @retresult
     *  @handleresult{
     *      nn::ResultSuccess,
     *      処理に成功しました。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultOutOfMemory,
     *      メモリの確保に失敗しました。
     *  }
     * @endretresult
     *
     * @pre
     *  - pName != nullptr
     *  - pValue != nullptr
     *  - nn::util::Strnlen(pName, INT_MAX) > 0
     *  - 初期化済みである。
     *
     * @details
     */
    nn::Result AddRequestHeader(const char* pName, const char* pValue, ...) NN_NOEXCEPT;

    /**
     * @brief   リクエストヘッダーを追加します。
     *
     * @param[in]   pName   フィールド名
     * @param[in]   pValue  フィールド値
     * @param[in]   varargs フィールド値の可変長パラメータ
     *
     * @retresult
     *  @handleresult{
     *      nn::ResultSuccess,
     *      処理に成功しました。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultOutOfMemory,
     *      メモリの確保に失敗しました。
     *  }
     * @endretresult
     *
     * @pre
     *  - pName != nullptr
     *  - pValue != nullptr
     *  - nn::util::Strnlen(pName, INT_MAX) > 0
     *  - 初期化済みである。
     *
     * @details
     */
    nn::Result AddRequestHeaderWithVarargs(const char* pName, const char* pValue, std::va_list varargs) NN_NOEXCEPT;

    //! @}
    //! @name 接続
    //! @{

    /**
     * @brief   サーバーに接続します。
     *
     * @param[in]   pUri    URI
     *
     * @retresult
     *  @handleresult{
     *      nn::ResultSuccess,
     *      処理に成功しました。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultOutOfMemory,
     *      メモリの確保に失敗しました。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultNotSupported,
     *      この機能はサポートしていません。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultCommunicationError,
     *      通信エラーが発生しました。
     *  }
     * @endretresult
     *
     * @pre
     *  - pUri != nullptr
     *  - 初期化済みである。
     *  - コネクションが設定済みである。
     *
     * @details
     *  SetConnection() で設定したコネクションを使用してサーバーに接続し、 WebSocket のハンドシェイク処理を行います。
     *
     *  本関数は何度も呼び出すことが可能です。@n
     *  接続が有効な状態で本関数を呼び出した場合、現在の接続を強制的に切断して再度接続処理が行われます。
     *
     *  再接続処理が行われる場合、以下のような挙動になります。
     *
     *  - 強制切断時、ステータスコード 1006（異常切断）のクローズイベントハンドラが呼び出される。
     *  - 強制切断時、未処理の送信要求はすべて取り消される。
     *  - 再接続処理中、 IsAvailable() は false を返す。
     *
     *  接続に成功した後、 Perform() を定期的に呼び出すことで接続後の通信処理が行われます。
     */
    nn::Result Open(const char* pUri) NN_NOEXCEPT;

    //! @}
    //! @name 送信要求
    //! @{

    /**
     * @brief   テキストデータを送信キューに追加します。
     *
     * @param[in]   pText   テキスト
     *
     * @retresult
     *  @handleresult{
     *      nn::ResultSuccess,
     *      処理に成功しました。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultInvalidArgument,
     *      引数が不正です。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultNoMoreMessage,
     *      クローズ処理中またはクローズ済みなため、これ以上データを送信できません。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultOutOfMemory,
     *      メモリの確保に失敗しました。
     *  }
     * @endretresult
     *
     * @pre
     *  - pText != nullptr
     *  - 初期化済みである。
     *
     * @details
     *  本関数はデータを送信キューに追加するだけで、実際の送信は行われません。@n
     *  送信処理は Perform() で行われます。
     *
     * @attention
     *  テキストは UTF-8 フォーマットにする必要があります。
     */
    nn::Result SendText(const char* pText) NN_NOEXCEPT;

    /**
     * @brief   テキストデータを送信キューに追加します。
     *
     * @param[in]   pText   テキスト
     * @param[in]   length  テキストのバイト数
     *
     * @retresult
     *  @handleresult{
     *      nn::ResultSuccess,
     *      処理に成功しました。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultInvalidArgument,
     *      引数が不正です。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultNoMoreMessage,
     *      クローズ処理中またはクローズ済みなため、これ以上データを送信できません。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultOutOfMemory,
     *      メモリの確保に失敗しました。
     *  }
     * @endretresult
     *
     * @pre
     *  - pText != nullptr
     *  - 初期化済みである。
     *
     * @details
     *  本関数はデータを送信キューに追加するだけで、実際の送信は行われません。@n
     *  送信処理は Perform() で行われます。
     *
     * @attention
     *  テキストは UTF-8 フォーマットにする必要があります。
     */
    nn::Result SendText(const char* pText, size_t length) NN_NOEXCEPT;

    /**
     * @brief   バイナリデータを送信キューに追加します。
     *
     * @param[in]   pData   データ
     * @param[in]   size    データサイズ
     *
     * @retresult
     *  @handleresult{
     *      nn::ResultSuccess,
     *      処理に成功しました。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultInvalidArgument,
     *      引数が不正です。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultNoMoreMessage,
     *      クローズ処理中またはクローズ済みなため、これ以上データを送信できません。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultOutOfMemory,
     *      メモリの確保に失敗しました。
     *  }
     * @endretresult
     *
     * @pre
     *  - pData != nullptr
     *  - size > 0
     *  - 初期化済みである。
     *
     * @details
     *  本関数はデータを送信キューに追加するだけで、実際の送信は行われません。@n
     *  送信処理は Perform() で行われます。
     */
    nn::Result SendBinary(const void* pData, size_t size) NN_NOEXCEPT;

    /**
     * @brief   クローズ要求を送信キューに追加します。
     *
     * @retresult
     *  @handleresult{
     *      nn::ResultSuccess,
     *      処理に成功しました。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultNoMoreMessage,
     *      クローズ処理中またはクローズ済みなため、これ以上データを送信できません。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultOutOfMemory,
     *      メモリの確保に失敗しました。
     *  }
     * @endretresult
     *
     * @pre
     *  - 初期化済みである。
     *
     * @details
     *  本関数はクローズ要求を送信キューに追加するだけで、実際の送信は行われません。@n
     *  送信処理は Perform() で行われます。
     *
     *  クローズ時のステータスコードに 1000（正常切断）、切断理由に空文字列（指定なし）が設定されます。
     */
    nn::Result Close() NN_NOEXCEPT;

    /**
     * @brief   クローズ要求を送信キューに追加します。
     *
     * @param[in]   code    ステータスコード
     * @param[in]   pReason 切断理由
     *
     * @retresult
     *  @handleresult{
     *      nn::ResultSuccess,
     *      処理に成功しました。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultInvalidArgument,
     *      引数が不正です。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultNoMoreMessage,
     *      クローズ処理中またはクローズ済みなため、これ以上データを送信できません。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultOutOfMemory,
     *      メモリの確保に失敗しました。
     *  }
     * @endretresult
     *
     * @pre
     *  - pReason != nullptr
     *  - nn::util::Strnlen(pReason, INT_MAX) <= 123
     *  - 初期化済みである。
     *
     * @details
     *  本関数はクローズ要求を送信キューに追加するだけで、実際の送信は行われません。@n
     *  送信処理は Perform() で行われます。
     *
     * @attention
     *  切断理由は UTF-8 フォーマットにする必要があります。
     *
     * @note
     *  ステータスコードの詳細は RFC6455#section-7.4 を参照してください。
     */
    nn::Result Close(uint16_t code, const char* pReason) NN_NOEXCEPT;

    //! @}
    //! @name 実行
    //! @{

    /**
     * @brief   通信処理を実行します。
     *
     * @pre
     *  - 初期化済みである。
     *
     * @details
     *  本関数は、受信バッファに溜まっているデータの処理および送信キューに溜まっているデータの送信を行います。
     *
     *  1 回の呼び出しで、以下の条件を満たすまで処理が行われます。
     *
     *  - 受信バッファが空になる。
     *  - 送信キューが空になるか、送信処理中に送信バッファが溢れる。
     *
     *  IsAvailable() が true を返す間、本関数を定期的に呼び出す必要があります。
     */
    void Perform() NN_NOEXCEPT;

    /**
     * @brief   接続が有効かどうかを判定します。
     *
     * @return  接続が有効かどうか
     *
     * @details
     *  以下の理由により通信処理の継続が不可能になった場合、本関数は false を返します。
     *
     *  - クライアントまたはサーバーが切断要求を出した。
     *  - エラーが発生して切断された。
     */
    bool IsAvailable() const NN_NOEXCEPT;

private:
    //
    detail::core::WebSocketImpl* m_pImpl;
};

}}
