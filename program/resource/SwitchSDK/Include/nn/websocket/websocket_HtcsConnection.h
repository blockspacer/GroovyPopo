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

#include <nn/websocket/websocket_ConnectionBase.h>

namespace nn { namespace websocket {

/**
 * @brief   HTCS コネクション
 *
 * @details
 *  本クラスは HTCS ライブラリを使用して実装されたコネクションです。
 *
 *  HTCS 経由でホストターゲット上に立てたサーバーと直接通信することができます。@n
 *  通信を開始する前に、以下の手続きが必要です。
 *
 *  - ターゲットマネージャを起動する。
 *  - サーバーが、ターゲットマネージャに対してポートを登録する。
 *
 *  詳しくは HTCS のドキュメントを参照してください。
 */
class HtcsConnection : public ConnectionBase
{
private:
    NN_DISALLOW_COPY(HtcsConnection);
    NN_DISALLOW_MOVE(HtcsConnection);

public:
    //! @name コンストラクタとデストラクタ
    //! @{

    /**
     * @brief   コンストラクタ
     *
     * @details
     */
    HtcsConnection() NN_NOEXCEPT;

    /**
     * @brief   コンストラクタ
     *
     * @param[in]   pPortName   ポート名
     *
     * @pre
     *  - pPortName != nullptr
     *
     * @details
     *  コンストラクタで HTCS 接続用のポート名を設定します。
     */
    explicit HtcsConnection(const char* pPortName) NN_NOEXCEPT;

    /**
     * @brief   デストラクタ
     *
     * @details
     */
    virtual ~HtcsConnection() NN_NOEXCEPT;

    //! @}
    //! @name 設定
    //! @{

    /**
     * @brief   HTCS 接続用のポート名を設定します。
     *
     * @param[in]   pPortName   ポート名
     *
     * @pre
     *  - pPortName != nullptr
     *
     * @details
     */
    void SetPortName(const char* pPortName) NN_NOEXCEPT;

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
    virtual void SetCancellationFunction(CancellationFunction pFunction, void* pParam) NN_NOEXCEPT NN_OVERRIDE;

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
     * @retresult
     *  @handleresult{
     *      nn::ResultSuccess,
     *      処理に成功しました。
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
     *  - pHostName != nullptr
     *  - HTCS 接続用のポート名が設定済みである。
     *
     * @details
     *  HTCS コネクションはホストターゲットと通信する機能であるため、引数の内容は無視されます。@n
     *  コンストラクタや SetPortName() で指定したポート名に対して接続が行われます。
     *
     * @attention
     *  HTCS コネクションは SSL 通信をサポートしていません。@n
     *  isSecure に true を設定した場合、本関数は nn::websocket::ResultNotSupported を返します。
     */
    virtual nn::Result Connect(const char* pHostName, uint16_t portNumber, bool isSecure) NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief   接続を閉じます。
     *
     * @details
     */
    virtual void Close() NN_NOEXCEPT NN_OVERRIDE;

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
    virtual ssize_t Send(const void* pData, size_t size) NN_NOEXCEPT NN_OVERRIDE;

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
    virtual ssize_t Receive(void* pBuffer, size_t size) NN_NOEXCEPT NN_OVERRIDE;

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
    virtual ssize_t Peek(void* pBuffer, size_t size) NN_NOEXCEPT NN_OVERRIDE;

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
    virtual nn::socket::Errno GetLastError() NN_NOEXCEPT NN_OVERRIDE;

    //! @}

private:
    //
    const char* m_pPortName;
    //
    int m_Socket;
    //
    CancellationFunction m_pCancellationFunction;
    void* m_pCancellationFunctionParam;
};

}}
