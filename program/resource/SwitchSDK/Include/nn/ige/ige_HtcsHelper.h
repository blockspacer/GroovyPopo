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
* @file         ige_HtcsHelper.h
* @briefprivate nn::htcs と HIO(Cafe) を同一 API で扱える関数を宣言します。
*/

#include <nn/nn_Common.h>
#include <nn/htcs/htcs_Types.h>

namespace nn { namespace ige {

//! @{
//! @name 関数

/**
* @briefprivate 新しいソケットを作成します。
*
* @return       処理の結果が返ります。
* @retval 0以上  作成されたソケットのソケット記述子です。
* @retval -1    処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
int HtcsHelperSocket() NN_NOEXCEPT;

/**
* @briefprivate ソケットを閉じます。閉じられたソケットは以後使用できなくなります。
*
* @param[in]    descriptor 対象のソケットに紐づいたソケット記述子を指定します。
*
* @return       処理の結果が返ります。
* @retval  0    処理に成功しました。
* @retval -1    処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
int HtcsHelperClose(int descriptor) NN_NOEXCEPT;

/**
* @briefprivate リモートホスト上のサービスに接続します。
*
* @param[in]    descriptor  対象のソケットに紐づいたソケット記述子を指定します。
* @param[in]    address     接続対象のホストとポートを指定します。
*
* @platformbegin{Cafe}
* Cafeはクライアントにはなれないので、Connectは使用できません。
* Cafe上で呼び出した場合は常に処理が失敗となります。
* @platformend
*
* @return    処理の結果が返ります。
* @retval  0 処理に成功しました。
* @retval -1 処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
int HtcsHelperConnect(int descriptor, const htcs::SockAddrHtcs* address) NN_NOEXCEPT;

/**
* @briefprivate ソケットにサービス名を割り当てます。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[in]    address         バインド対象のターゲットとポートを指定します。
*
* @return    処理の結果が返ります。
* @retval  0 処理に成功しました。
* @retval -1 処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
int HtcsHelperBind(int descriptor, const htcs::SockAddrHtcs* address) NN_NOEXCEPT;

/**
* @briefprivate サービスへの接続受付を開始します。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[in]    backlogCount    descriptor について、保留中の接続のキューの最大長を指定します。
*
* @return    処理の結果が返ります。
* @retval  0 処理に成功しました。
* @retval -1 処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
int HtcsHelperListen(int descriptor, int backlogCount) NN_NOEXCEPT;

/**
* @briefprivate サービスへの着信を受け付けます。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[out]   address         接続先ホストのアドレスとポート名の格納先を指定します。
*
* @return       処理の結果が返ります。
* @retval  0以上 着信の受付にともなって作成された新しいソケット記述子です。
* @retval -1    処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*
* @details 新しいソケットは、descriptor に指定したソケットに Fcntl() で設定したフラグを継承します。
*/
int HtcsHelperAccept(int descriptor, htcs::SockAddrHtcs* address) NN_NOEXCEPT;

/**
* @briefprivate リモートホストからデータを受信します。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[out]   buffer          受信データの格納先メモリを指定します。
* @param[in]    bufferByteSize  buffer のバイトサイズを指定します。
* @param[in]    flags           MessageFlag を指定します。
*
* @return       処理の結果が返ります。
* @retval 1以上  受信したデータのバイト数です。
* @retval  0    リモートホストがデータの送信を終了しました。
* @retval -1    処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
htcs::ssize_t HtcsHelperRecv(int descriptor, void* buffer, size_t bufferByteSize, int flags) NN_NOEXCEPT;

/**
* @briefprivate リモートホストへデータを送信します。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[out]   buffer          送信データの格納先メモリを指定します。
* @param[in]    bufferByteSize  buffer のバイトサイズを指定します。
* @param[in]    flags           MessageFlag を指定します。
*
* @return       処理の結果が返ります。
* @retval 0以上  送信あるいは送信予約したデータのバイト数です。
* @retval -1    処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
htcs::ssize_t HtcsHelperSend(int descriptor, const void* buffer, size_t bufferByteSize, int flags) NN_NOEXCEPT;

/**
* @briefprivate ソケットの送受信処理の一部もしくは全部を遮断します。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[in]    how             ShutdownType を指定します。
*
* @return       処理の結果が返ります。
* @retval  0    処理に成功しました。
* @retval -1    処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*/
int HtcsHelperShutdown(int descriptor, int how) NN_NOEXCEPT;

/**
* @briefprivate ソケットの動作設定フラグを制御します。
*
* @param[in]    descriptor      対象のソケットに紐づいたソケット記述子を指定します。
* @param[in]    command         FcntlOperation を指定します。
* @param[in]    value           FcntlFlag を指定します。
*
* @return       処理の結果が返ります。
* @retval  0    処理に成功しました。
* @retval -1    処理に失敗しました。詳細なエラーは GetLastError() で取得できます。
*
* @platformbegin{Cafe}
* Cafeではノンブロッキングモードは利用できません。
* Cafe上で設定を変更した場合の動作は未保証となります。
* @platformend
*
* @attention    以下の引数のみサポートされます。
*  @li command == NN_SOCKET_F_GETFL
*  @li (command == NN_SOCKET_F_SETFL) && (value == 0 || value == NN_SOCKET_O_NONBLOCK)
*/
int HtcsHelperFcntl(int descriptor, int command, int value) NN_NOEXCEPT;

/**
 * @briefprivate htcs ライブラリの関数で最後に発生したエラーの詳細を取得します。
 *               htcs ライブラリの関数を呼び出してエラー値が返された直後、
 *               他の関数（他のライブラリのものを含む）を呼び出す前にのみ使用できます。
 *
 * @return       エラーの詳細を表す SocketError 列挙型の値が返ります。
 * @attention    現時点では、HTCS_EWOULDBLOCK (HTCS_EAGAIN) あるいは HTCS_EUNKNOWN のみが返ります。
 */
int HtcsHelperGetLastError() NN_NOEXCEPT;

/**
 * @briefprivate Bind の際、HtcsPeerName に自分自身を指定する場合に使用します。socket における INADDR_ANY に該当します。
 *
 * @return       自分自身を表す HtcsPeerName を返します。
 */
const htcs::HtcsPeerName HtcsHelperGetPeerNameAny() NN_NOEXCEPT;

/**
 * @briefprivate Connect の際、HtcsPeerName に既定のホスト PC を指定する場合に使用します。
 *
 * @return       既定のホスト PC を表す HtcsPeerName を返します。
 */
const htcs::HtcsPeerName HtcsHelperGetDefaultHostName() NN_NOEXCEPT;

//! @}

}}
