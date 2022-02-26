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
* @brief   Berkeley socket スタイルでホスト・ターゲット間通信を行う API の関数
*/

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/htcs/htcs_Types.h>

namespace nn { namespace htcs {

//! @{
//! @name 関数

/**
* @brief        新しいソケットを作成します。
*
* @return       処理の結果が返ります。
* @retval ≧0   作成されたソケットのソケット記述子です。
* @retval -1    処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
int Socket() NN_NOEXCEPT;

/**
* @brief     ソケットを閉じます。閉じられたソケットは以後使用できなくなります。
*
* @param[in]    descriptor 対象のソケットに紐づいたソケット記述子を指定します。
*
* @return    処理の結果が返ります。
* @retval  0 処理に成功しました。
* @retval -1 処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
int Close(int descriptor) NN_NOEXCEPT;

/**
* @brief リモートホスト上のサービスに接続します。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[in]    address         接続対象のホストとポートを指定します。
*
* @return    処理の結果が返ります。
* @retval  0 処理に成功しました。
* @retval -1 処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
int Connect(int descriptor, const SockAddrHtcs* address) NN_NOEXCEPT;

/**
* @brief ソケットにサービス名を割り当てます。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[in]    address         バインド対象のターゲットとポートを指定します。
*
* @return    処理の結果が返ります。
* @retval  0 処理に成功しました。
* @retval -1 処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
int Bind(int descriptor, const SockAddrHtcs* address) NN_NOEXCEPT;

/**
* @brief サービスへの接続受付を開始します。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[in]    backlogCount    descriptor について、保留中の接続のキューの最大長を指定します。
*
* @return    処理の結果が返ります。
* @retval  0 処理に成功しました。
* @retval -1 処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
int Listen(int descriptor, int backlogCount) NN_NOEXCEPT;

/**
* @brief サービスへの着信を受け付けます。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[out]   address         接続先ホストのアドレスとポート名の格納先を指定します。
*
*
* @return        処理の結果が返ります。
* @retval ≧0    着信の受付にともなって作成された新しいソケット記述子です。
* @retval -1     処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*
* @details 新しいソケットは、descriptor に指定したソケットに Fcntl() で設定したフラグを継承します。
*/
int Accept(int descriptor, SockAddrHtcs* address) NN_NOEXCEPT;

/**
* @brief リモートホストからデータを受信します。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[out]   buffer          受信データの格納先メモリを指定します。
* @param[in]    bufferByteSize  buffer のバイトサイズを指定します。
* @param[in]    flags           MessageFlag を指定します。
*
* @return    処理の結果が返ります。
* @retval ≧1   受信したデータのバイト数です。
* @retval  0    リモートホストがデータの送信を終了しました。
* @retval -1    処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
ssize_t Recv(int descriptor, void* buffer, size_t bufferByteSize, int flags) NN_NOEXCEPT;

/**
* @brief リモートホストへデータを送信します。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[out]   buffer          送信データの格納先メモリを指定します。
* @param[in]    bufferByteSize  buffer のバイトサイズを指定します。
* @param[in]    flags           MessageFlag を指定します。
*
* @return    処理の結果が返ります。
* @retval ≧0   送信あるいは送信予約したデータのバイト数です。
* @retval -1    処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
ssize_t Send(int descriptor, const void* buffer, size_t bufferByteSize, int flags) NN_NOEXCEPT;

/**
* @brief     ソケットの送受信処理の一部もしくは全部を遮断します。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[in]    how             ShutdownType を指定します。
*
* @return    処理の結果が返ります。
* @retval  0 処理に成功しました。
* @retval -1 処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
int Shutdown(int descriptor, int how) NN_NOEXCEPT;

/**
* @brief     ソケットの動作設定フラグを制御します。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[in]    command         FcntlOperation を指定します。
* @param[in]    value           FcntlFlag を指定します。
*
* @return        処理の結果が返ります。
* @retval -1     処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
* @retval その他 処理に成功しました。
*
* @attention  以下の引数のみサポートされます。
*  @li command == nn::htcs::HTCS_F_GETFL
*  @li (command == nn::htcs::HTCS_F_SETFL) && (value == 0 || value == nn::htcs::HTCS_O_NONBLOCK)
*
* @details      ソケットの動作設定フラグを制御します。@n
*               command == nn::htcs::HTCS_F_GETFL の場合は現在の動作設定フラグの設定値を返します。@n
*               command == nn::htcs::HTCS_F_SETFL の場合は value の値を動作設定フラグに設定します。
*/
int Fcntl(int descriptor, int command, int value) NN_NOEXCEPT;

/**
 * @brief 同一スレッドで最後に発生した htcs ライブラリの関数のエラーの詳細を取得します。
 *        htcs ライブラリの関数を呼び出してエラー値が返された直後、
 *        他の関数（他のライブラリのものを含む）を呼び出す前にのみ使用できます。
 *
 * @return    エラーの詳細を表す SocketError 列挙型の値が返ります。
 */
int GetLastError() NN_NOEXCEPT;

/**
 * @brief Bind の際、HtcsPeerName に自分自身を指定する場合に使用します。socket における INADDR_ANY に該当します。
 *
 * @return 自分自身を表す HtcsPeerName を返します。
 */
const HtcsPeerName GetPeerNameAny() NN_NOEXCEPT;

/**
 * @brief Connect の際、HtcsPeerName に既定のホスト PC を指定する場合に使用します。
 *
 * @return 既定のホスト PC を表す HtcsPeerName を返します。
 */
const HtcsPeerName GetDefaultHostName() NN_NOEXCEPT;

//! @}

}}

