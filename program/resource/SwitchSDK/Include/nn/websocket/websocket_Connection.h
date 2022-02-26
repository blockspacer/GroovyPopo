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
#include <nn/ssl.h>

namespace nn { namespace websocket {

/**
 * @brief   コネクション
 *
 * @details
 *  本クラスは SOCKET ライブラリおよび SSL ライブラリを使用して実装されたコネクションです。
 */
class Connection : public ConnectionBase
{
private:
    NN_DISALLOW_COPY(Connection);
    NN_DISALLOW_MOVE(Connection);

public:
    /**
     * @brief   SSL コンテキストの証明書インポート関数
     *
     * @param[in]   pContext    SSL コンテキスト
     * @param[in]   pParam      ユーザーパラメータ
     *
     * @return  インポートに成功したかどうか
     *
     * @details
     */
    using SslContextCertificateImportFunction = bool (*)(nn::ssl::Context* pContext, void* pParam);

public:
    //! @name コンストラクタとデストラクタ
    //! @{

    /**
     * @brief   コンストラクタ
     *
     * @details
     */
    Connection() NN_NOEXCEPT;

    /**
     * @brief   デストラクタ
     *
     * @details
     */
    virtual ~Connection() NN_NOEXCEPT;

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
    virtual void SetCancellationFunction(CancellationFunction pFunction, void* pParam) NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief   SSL コンテキストの証明書インポート関数を設定します。
     *
     * @param[in]   pFunction   関数
     * @param[in]   pParam      関数のユーザーパラメータ
     *
     * @details
     *  本クラスは SSL 通信時に SSL コンテキストを作成します。@n
     *  SSL コンテキストに対してアプリケーション独自の証明書をインポートしたい場合、インポート関数を設定してください。
     *
     *  インポート関数には作成済みの SSL コンテキストが渡されます。@n
     *  インポート関数内で以下の関数を使用して証明書をインポートすることができます。
     *
     *  - nn::ssl::Context::ImportClientPki()
     *  - nn::ssl::Context::ImportServerPki()
     */
    void SetSslContextCertificateImportFunction(SslContextCertificateImportFunction pFunction, void* pParam) NN_NOEXCEPT;

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
     *      nn::websocket::ResultCommunicationError,
     *      通信エラーが発生しました。
     *  }
     *  @handleresult{
     *      nn::websocket::ResultOutOfMemory,
     *      メモリの確保に失敗しました。
     *  }
     * @endretresult
     *
     * @pre
     *  - pHostName != nullptr
     *
     * @details
     *  本クラスはプロキシ接続をサポートしています。@n
     *  現在のネットワーク接続に利用されているプロキシ設定を取得し、設定が有効であれば HTTP CONNECT メソッドによるトンネリングを行います。
     *
     *  SSL 通信を行う場合、ソケットの接続を確立した後に SSL ハンドシェイクを行います。
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
    int m_Socket;
    //
    nn::ssl::Context m_SslContext;
    nn::ssl::Connection m_SslConnection;
    bool m_IsSslContextCreated;
    bool m_IsSslConnectionCreated;
    //
    CancellationFunction m_pCancellationFunction;
    void* m_pCancellationFunctionParam;
    //
    SslContextCertificateImportFunction m_pSslContextCertificateImportFunction;
    void* m_pSslContextCertificateImportFunctionParam;

private:
    //
    nn::Result InitializeSocketObject() NN_NOEXCEPT;
    nn::Result InitializeSslObjects(const char* pHostName) NN_NOEXCEPT;
};

}}
