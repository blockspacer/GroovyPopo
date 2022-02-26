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
 * @brief   コントローラーサポートの UI 呼び出しに関する API の宣言
 */

#pragma once

#include <nn/nn_Result.h>
#include <nn/hid/hid_ResultControllerSupport.h>
#include <nn/hid/hid_ControllerSupportArg.h>

namespace nn { namespace hid {

//------------------------------------------------------------------------
/**
 * @brief コントローラーの接続をサポートするUIを表示します。
 *
 * @param[in]  showControllerSupportArg 表示時に必要な設定パラメータです。
 *
 * @return 処理の結果
 * @retval nn::ResultSuccess
 *         コントローラーの接続が適切に完了した場合の返り値です。
 * @retval nn::hid::ResultControllerSupportCanceled
 *         指定した条件を満たせていない場合の返り値です。
 * @retval nn::hid::ResultControllerSupportNotSupportedNpadStyle
 *         コントローラーサポートで対応していない操作形態が設定されている場合の返り値です。
 *
 * @details
 *  本 API は完了するまで処理をブロックします。
 */
nn::Result ShowControllerSupport( const ControllerSupportArg& showControllerSupportArg ) NN_NOEXCEPT;

//------------------------------------------------------------------------
/**
 * @brief コントローラーの接続をサポートするUIを表示します。格納先を指定することで、接続状況に関する追加の情報を取得できます。
 *
 * @param[out] pOutValue                接続状況に関する追加情報の格納先です。
 * @param[in]  showControllerSupportArg 表示時に必要な設定パラメータです。
 *
 * @return 処理の結果
 * @retval nn::ResultSuccess
 *         コントローラーの接続が適切に完了した場合の返り値です。
 * @retval nn::hid::ResultControllerSupportCanceled
 *         指定した条件を満たせていない場合の返り値です。
 * @retval nn::hid::ResultControllerSupportNotSupportedNpadStyle
 *         コントローラーサポートで対応していない操作形態が設定されている場合の返り値です。
 *
 * @details
 *  本 API は完了するまで処理をブロックします。
 */
nn::Result ShowControllerSupport( ControllerSupportResultInfo* pOutValue,
    const ControllerSupportArg& showControllerSupportArg ) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief コントローラー番号ごとの説明用の文字列を設定します。
 *
 * @param[in,out] pOutControllerSupportArg コントローラーサポート表示用パラメータです。
 * @param[in]     pStr                     設定する文字列 (UTF-8) です。
 * @param[in]     id                       何番目のコントローラー用の説明か指定します。(NpadId::No1～NpadId::No4)
 *
 * @details
 *  本 API は完了するまで処理をブロックします。
 */
void SetExplainText( ControllerSupportArg* pOutControllerSupportArg, const char* pStr, const NpadIdType& id ) NN_NOEXCEPT;

//------------------------------------------------------------------------
/**
* @brief コントローラーのストラップ着用案内を表示します。
*
* @details
*  本 API は完了するまで処理をブロックします。
*/
void ShowControllerStrapGuide() NN_NOEXCEPT;

//------------------------------------------------------------------------
/**
* @brief コントローラーのファームウェア更新を行うUIを表示します。
*
* @param[in]  showControllerFirmwareUpdateArg 表示時に必要な設定パラメータです。
*
* @return 処理の結果
* @retval nn::ResultSuccess
*         コントローラーのファームウェア更新が適切に完了しました。
* @retval nn::hid::ResultControllerFirmwareUpdateFailed
*         ファームウェア更新に失敗しました。更新が完了したコントローラーが存在する場合があります。
*
* @details
*  本 API は完了するまで処理をブロックします。
*  接続中のコントローラー全てを更新対象として、最新のファームウェアに更新します。
*  コントローラーのファームウェア更新が適切に完了して nn::ResultSuccess を返した場合でも、
*  本 API による更新処理の後に、未更新のコントローラーが接続されている場合があります。
*/
nn::Result ShowControllerFirmwareUpdate( const ControllerFirmwareUpdateArg& showControllerFirmwareUpdateArg ) NN_NOEXCEPT;

}} // namespace nn::hid

