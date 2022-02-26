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
 * @brief ネットワークサービス利用のためのミドルウェアで発生したエラーの表示用APIの定義
 */

#pragma once

#include <nn/nn_Result.h>
#include <nn/account/account_Types.h>
#include <nn/err/err_Types.h>

namespace nn { namespace err {

//! @name ネットワークサービスの利用中に発生したエラーの表示用API
//! @{

/**
 * @brief 指定されたエラーがネットワークサービスの利用で発生したものかを判定します。
 * @param[in] errorCode 発生したエラーに対応する ErrorCode データ
 * @return ネットワークサービスの利用で発生したものの場合は true
 * @pre
 *  - errorCode が有効なエラーコードを表すデータである
 * @details
 * - 本関数はネットワークサービス利用のためのミドルウェアで指定のあった場合に限り使用してください。
 * - 本関数が true を返すエラーを表示するには ShowNetworkServiceError() を使用してください。
 */
bool IsNetworkServiceError(ErrorCode errorCode) NN_NOEXCEPT;

/**
 * @brief ネットワークサービスの利用で発生したエラーを表示します。
 * @param[in] errorCode 発生したエラーに対応する ErrorCode データ
 * @param[in] userHandle エラーの発生したセッションに紐づくユーザーアカウントのハンドル
 * @pre
 *  - IsNetworkServiceError(errorCode) が true を返す
 * @details
 * - 本関数はネットワークサービス利用のためのミドルウェアで指定のあった場合に限り使用してください。
 * - エラー表示を行っている間は呼び出し元をブロックします。
 * - 本関数は「エラー表示関数」の1つです。
 */
void ShowNetworkServiceError(ErrorCode errorCode, const account::UserHandle& userHandle) NN_NOEXCEPT;

/**
 * @brief ネットワークサービスの利用で発生したエラーを表示します。
 * @param[in] errorCode 発生したエラーに対応する ErrorCode データ
 * @pre
 *  - IsNetworkServiceError(errorCode) が true を返す
 *  - Open状態のユーザーアカウントが1人である
 * @details
 * - 本関数はネットワークサービス利用のためのミドルウェアで指定のあった場合に限り使用してください。
 * - 本関数は、指定されたエラーを、Open状態のユーザーアカウントについて発生したエラーとして扱います。
 * - エラー表示を行っている間は呼び出し元をブロックします。
 * - 本関数は「エラー表示関数」の1つです。
 */
void ShowNetworkServiceError(ErrorCode errorCode) NN_NOEXCEPT;


/**
 * @brief 指定されたエラーがネットワークサービスの利用で発生したものかを判定します。
 * @param[in] result エラーの nn::Result 値
 * @return ネットワークサービスの利用で発生したものの場合は true
 * @pre
 *  - result.IsFailure() == true
 * @details
 * - 本関数はネットワークサービス利用のためのミドルウェアで指定のあった場合に限り使用してください。
 * - 本関数が true を返すエラーを表示するには ShowNetworkServiceError() を使用してください。
 */
bool IsNetworkServiceError(Result result) NN_NOEXCEPT;

/**
 * @brief ネットワークサービスの利用で発生したエラーを表示します。
 * @param[in] result エラーの nn::Result 値
 * @param[in] userHandle エラーの発生したセッションに紐づくユーザーアカウントのハンドル
 * @pre
 *  - IsNetworkServiceError(result) が true を返す
 * @details
 * - 本関数はネットワークサービス利用のためのミドルウェアで指定のあった場合に限り使用してください。
 * - エラー表示を行っている間は呼び出し元をブロックします。
 * - 本関数は「エラー表示関数」の1つです。
 */
void ShowNetworkServiceError(Result result, const account::UserHandle& userHandle) NN_NOEXCEPT;

/**
 * @brief ネットワークサービスの利用で発生したエラーを表示します。
 * @param[in] result エラーの nn::Result 値
 * @pre
 *  - IsNetworkServiceError(result) が true を返す
 *  - Open状態のユーザーアカウントが1人である
 * @details
 * - 本関数はネットワークサービス利用のためのミドルウェアで指定のあった場合に限り使用してください。
 * - 本関数は、指定されたエラーを、Open状態のユーザーアカウントについて発生したエラーとして扱います。
 * - エラー表示を行っている間は呼び出し元をブロックします。
 * - 本関数は「エラー表示関数」の1つです。
 */
void ShowNetworkServiceError(Result result) NN_NOEXCEPT;

//! @}
//-----------------------------------------------------------------------------

}} // namespace nn::err
