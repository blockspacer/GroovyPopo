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
* @brief   Berkeley socket スタイルでホスト・ターゲット間通信を行う API の型と定数
*/

#include <nn/nn_Common.h>

namespace nn { namespace htcs {

/**
 * @brief Shutdown() に与えるフラグです。
 */
enum ShutdownType
{
    HTCS_SHUT_RD        = 0,           //!< 受信
    HTCS_SHUT_WR        = 1,           //!< 送信
    HTCS_SHUT_RDWR      = 2            //!< 送受信
};

/**
 * @brief Fcntl() に与える操作モードです。
 */
enum FcntlOperation
{
    HTCS_F_GETFL        = 3,            //!< ソケットのフラグ値取得
    HTCS_F_SETFL        = 4             //!< ソケットのフラグ値セット
};

/**
 * @brief Fcntl() の操作モードに HTC_F_GETFL HTC_F_SETFL を指定したときのフラグです。
 */
enum FcntlFlag
{
    HTCS_O_NONBLOCK     = 0x04          //!< ソケットをノンブロッキングモードに変更します。
};

/**
 * @brief Recv() や Send() に与えるフラグです。
 */
enum MessageFlag
{
    HTCS_MSG_PEEK       = 0x01,        //!< データを読み出すだけで、取り出さずそのまま残します。
    HTCS_MSG_WAITALL    = 0x02         //!< Recv() において、指定したバッファサイズと同じだけのデータが受信できるまでブロックします。
};

/**
 * @brief SockAddrHtcs に与えるアドレスファミリです。
 */
enum AddressFamily
{
    HTCS_AF_HTCS = 0
};

/**
 * @brief HTCS API が返すエラー値です。
 */
enum SocketError
{
    HTCS_ENONE            = 0,        //!< No error
    HTCS_EACCES           = 2,        //!< 使用する権限がありません
    HTCS_EADDRINUSE       = 3,        //!< アドレスがすでに使用中です
    HTCS_EADDRNOTAVAIL    = 4,        //!< アドレスが使用できません
    HTCS_EAGAIN           = 6,        //!< ノンブロッキングの操作が行われた、もしくはタイムアウトが発生したためリトライが必要です。
    HTCS_EWOULDBLOCK      = HTCS_EAGAIN,//!< 要求した操作はブロッキングでないと実行できません。HTCS_EAGAIN と同値であることは保証されません。
    HTCS_EALREADY         = 7,
    HTCS_EBADF            = 8,        //!< 不正なソケット記述子です
    HTCS_EBUSY            = 10,
    HTCS_ECONNABORTED     = 13,       //!< 接続が中断されました
    HTCS_ECONNREFUSED     = 14,       //!< 接続が拒否されました
    HTCS_ECONNRESET       = 15,       //!< 接続がリセットされました
    HTCS_EDESTADDRREQ     = 17,       //!< 終点アドレスが指定されていません
    HTCS_EFAULT           = 21,
    HTCS_EINPROGRESS      = 26,       //!< 接続がまだ完了していません
    HTCS_EINTR            = 27,       //!< 中止されました
    HTCS_EINVAL           = 28,       //!< 無効な処理
    HTCS_EIO              = 29,       //!< 入出力エラー
    HTCS_EISCONN          = 30,       //!< ソケットがすでに接続されています
    HTCS_EMFILE           = 33,       //!< ソケット記述子をこれ以上作れません
    HTCS_EMSGSIZE         = 35,       //!< 送信するにはサイズが大きすぎます
    HTCS_ENETDOWN         = 38,       //!< 該当インスタンスがダウンしています
    HTCS_ENETRESET        = 39,       //!< HTCS ライブラリが初期化されていません
    HTCS_ENOBUFS          = 42,       //!< リソース不足
    HTCS_ENOMEM           = 49,       //!< メモリ不足
    HTCS_ENOTCONN         = 56,       //!< 接続されていません
    HTCS_ETIMEDOUT        = 76,       //!< 時間切れ
    HTCS_EUNKNOWN         = 79        //!< 不明なエラーです
};

/**
* @brief 端末名の文字列を格納するバッファの長さです。終端のヌル文字を含みます。
*/
const int PeerNameBufferLength = 32;

/**
* @brief ポート名の文字列を格納するバッファの長さです。終端のヌル文字を含みます。
*/
const int PortNameBufferLength = 32;

/**
 * @brief サイズまたは負のエラー値を保持する符号つき整数型です。
 */
// TODO: ssize_t の型
// TODO: コーディング規約違反に対処
typedef intptr_t ssize_t;

/**
 * @brief アドレスファミリを保持する符号なし整数型です。
 */
typedef uint16_t AddressFamilyType;

/**
 * @brief 端末名です。ヌル終端です。
 *        TCP/IP における IP アドレスと対応します。
 */
struct HtcsPeerName
{
    char name[PeerNameBufferLength];
};

/**
 * @brief リモート側からの接続を受け付けるときのポート名です。ヌル終端です。
 *        TCP/IP におけるポート番号と対応します。
 */
struct HtcsPortName
{
    char name[PortNameBufferLength];
};

/**
 * @brief アドレスファミリが HTCS_AF_HTCS であるソケットのアドレスです。
 */
struct SockAddrHtcs
{
    AddressFamilyType    family;          //!< アドレスファミリです。HTCS_AF_HTCS を設定してください。
    HtcsPeerName         peerName;        //!< 端末名です。ヌル終端です。
    HtcsPortName         portName;        //!< リモート側からの接続を受け付けるときのポート名です。ヌル終端です。
};

/**
 * @deprecated こちらのセッション数の制限はなくなりました。参照不要です。
 * @brief 同時に操作可能なセッション数の最大値です。
 */
const int SessionCountMax = 16;

/**
 * @brief 同時に作成可能なソケット数の最大値です。
 */
const int SocketCountMax = 32;

}}

