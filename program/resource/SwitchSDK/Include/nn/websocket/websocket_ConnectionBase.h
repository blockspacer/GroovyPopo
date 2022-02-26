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
#include <nn/socket/socket_Errno.h>
#include <nn/socket/socket_Types.h>
#include <nn/websocket/websocket_Types.h>

namespace nn { namespace websocket {

/**
 * @brief   コネクションの抽象基底クラス
 *
 * @details
 *  サーバーとの接続や通信を行うコネクションの抽象基底クラスです。
 *
 * @note
 *  本クラスを継承するクラスは、送受信処理を非ブロッキングモードで動作するように実装する必要があります。
 */
class ConnectionBase
{
private:
    NN_DISALLOW_COPY(ConnectionBase);
    NN_DISALLOW_MOVE(ConnectionBase);

public:
    //! @name コンストラクタとデストラクタ
    //! @{

    /**
     * @brief   コンストラクタ
     *
     * @details
     */
    ConnectionBase() NN_NOEXCEPT = default;

    /**
     * @brief   デストラクタ
     *
     * @details
     */
    virtual ~ConnectionBase() NN_NOEXCEPT = default;

    //! @}
    //! @name 設定
    //! @{

    /**
     * @brief   接続処理のキャンセル関数を設定します。
     *
     * @param[in]   pFunction   関数
     * @param[in]   pParam      関数のユーザーパラメータ
     *
     * @details
     *  Connect() の呼び出しから完了するまでの間、本関数で設定したキャンセル関数が定期的に呼び出されます。@n
     *  キャンセル関数が true を返すことで、接続処理を中断することができます。
     */
    virtual void SetCancellationFunction(CancellationFunction pFunction, void* pParam) NN_NOEXCEPT = 0;

    //! @}
    //! @name 接続
    //! @{

    /**
     * @brief   サーバーに接続します。
     *
     * @param[in]   pHostName   ホスト名
     * @param[in]   portNumber  ポート番号
     * @param[in]   isSecure    SSL 通信を行うかどうか
     *
     * @return  処理結果
     *
     * @pre
     *  - pHostName != nullptr
     *
     * @details
     */
    virtual nn::Result Connect(const char* pHostName, uint16_t portNumber, bool isSecure) NN_NOEXCEPT = 0;

    /**
     * @brief   接続を閉じます。
     *
     * @details
     */
    virtual void Close() NN_NOEXCEPT = 0;

    //! @}
    //! @name 送受信
    //! @{

    /**
     * @brief   データを送信します。
     *
     * @param[in]   pData   データ
     * @param[in]   size    データサイズ
     *
     * @return  処理結果
     *
     * @retval ≧0   送信または送信予約したサイズです。
     * @retval -1   エラーが発生しました。詳細なエラーは GetLastError() で取得できます。
     *
     * @pre
     *  - pData != nullptr
     *  - size > 0
     *
     * @details
     */
    virtual ssize_t Send(const void* pData, size_t size) NN_NOEXCEPT = 0;

    /**
     * @brief   データを受信します。
     *
     * @param[in]   pBuffer バッファ
     * @param[in]   size    バッファサイズ
     *
     * @return  処理結果
     *
     * @retval ≧1   受信したサイズです。
     * @retval 0    サーバーとの接続が閉じられました。
     * @retval -1   エラーが発生しました。詳細なエラーは GetLastError() で取得できます。
     *
     * @pre
     *  - pBuffer != nullptr
     *  - size > 0
     *
     * @details
     */
    virtual ssize_t Receive(void* pBuffer, size_t size) NN_NOEXCEPT = 0;

    /**
     * @brief   受信バッファの先頭を参照します。
     *
     * @param[in]   pBuffer バッファ
     * @param[in]   size    バッファサイズ
     *
     * @return  処理結果
     *
     * @retval ≧1   参照したサイズです。
     * @retval 0    サーバーとの接続が閉じられました。
     * @retval -1   エラーが発生しました。詳細なエラーは GetLastError() で取得できます。
     *
     *
     * @pre
     *  - pBuffer != nullptr
     *  - size > 0
     *
     * @details
     *  本関数は Receive() と違い、受信バッファからデータを取り出しません。
     */
    virtual ssize_t Peek(void* pBuffer, size_t size) NN_NOEXCEPT = 0;

    //! @}
    //! @name エラー情報
    //! @{

    /**
     * @brief   最後に発生したソケットエラーを取得します。
     *
     * @return  最後に発生したソケットエラー
     *
     * @details
     *  受信データがない、もしくは、送信バッファに空きがない場合、 nn::socket::Errno::EAgain が返ります。
     */
    virtual nn::socket::Errno GetLastError() NN_NOEXCEPT = 0;

    //! @}
};

}}
