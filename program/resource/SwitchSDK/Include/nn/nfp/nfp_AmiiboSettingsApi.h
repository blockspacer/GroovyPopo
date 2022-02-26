/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/** @file
    @brief      amiibo 設定に関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nfp/nfp_Result.h>
#include <nn/nfp/nfp_Types.h>

namespace nn {
namespace nfp {

/**
 * @name    amiibo 設定
 * @{
 */

/**
 * @brief   ニックネームとオーナーの設定を行う amiibo 設定を開始します。
 *
 * @param[out] pOutDeviceHandle タグを更新した NFC デバイスのハンドルを格納するバッファへのポインタを指定します。
 * @param[out] pOutIsRegistered 更新後の登録情報の有無を格納するバッファへのポインタを指定します。
 * @param[out] pOutRegisterInfo 更新後の登録情報を格納するバッファへのポインタを指定します。@a pOutIsRegistered にて登録情報が「有り」を示す場合のみ参照してください。
 * @param[in]  startParam       amiibo 設定開始時必要な共通パラメータです。
 * @param[in]  tagInfo          更新の対象となるタグを示す情報です。
 * @param[in]  registerInfo     本 API 呼び出し時点での登録情報です。@ref GetRegisterInfo 関数で取得した値を指定してください。
 *
 * @pre
 *  - GetState() == @ref State_Init
 *  - @a pOutDeviceHandle はアクセス可能なアドレスを指している必要があります。
 *  - @a pOutIsRegistered はアクセス可能なアドレスを指している必要があります。
 *  - @a pOutRegisterInfo はアクセス可能なアドレスを指している必要があります。
 *
 * @retresult
 *     @handleresult{ResultSuccess,           タグの更新に成功しました。}
 *     @handleresult{ResultNotUpdated,        タグは更新されていません。}
 * @endretresult
 *
 * @details
 *  ニックネームとオーナーの設定を行う amiibo 設定を開始します。本 API は、amiibo 設定が完了するまで戻りません。@n
 *  @ref ResultNotUpdated が返ってきた場合、@a pOutDeviceHandle 、@a pOutIsRegistered および @a pOutRegisterInfo には有効な情報が格納されていませんので、参照しないでください。@n
 *  @a registerInfo は、 amiibo 設定開始後に改めてタグを読み込むことなく、設定前のニックネームとオーナーを参照するために利用されます。登録情報が登録されている場合には、必ず指定してください。@n
 *  登録情報が未登録の場合は、本 API ではなく、@ref RegisterInfo を引数に指定しない@n
 *  StartNicknameAndOwnerSettings(DeviceHandle* pOutDeviceHandle, bool* pOutIsRegistered, RegisterInfo* pOutRegisterInfo, const AmiiboSettingsStartParam& startParam, const TagInfo& tagInfo) を利用してください。
 *
 */
nn::Result StartNicknameAndOwnerSettings(DeviceHandle* pOutDeviceHandle, bool* pOutIsRegistered, RegisterInfo* pOutRegisterInfo, const AmiiboSettingsStartParam& startParam, const TagInfo& tagInfo, const RegisterInfo& registerInfo) NN_NOEXCEPT;

/**
 * @brief   ニックネームとオーナーの設定を行う amiibo 設定を開始します。
 *
 * @param[out] pOutDeviceHandle タグを更新した NFC デバイスのハンドルを格納するバッファへのポインタを指定します。
 * @param[out] pOutIsRegistered 更新後の登録情報の有無を格納するバッファへのポインタを指定します。
 * @param[out] pOutRegisterInfo 更新後の登録情報を格納するバッファへのポインタを指定します。@a pOutIsRegistered にて登録情報が「有り」を示す場合のみ参照してください。
 * @param[in]  startParam       amiibo 設定開始時必要な共通パラメータです。
 * @param[in]  tagInfo          更新の対象となるタグを示す情報です。
 *
 * @pre
 *  - GetState() == @ref State_Init
 *  - @a pOutDeviceHandle はアクセス可能なアドレスを指している必要があります。
 *  - @a pOutIsRegistered はアクセス可能なアドレスを指している必要があります。
 *  - @a pOutRegisterInfo はアクセス可能なアドレスを指している必要があります。
 *
 * @retresult
 *     @handleresult{ResultSuccess,           タグの更新に成功しました。}
 *     @handleresult{ResultNotUpdated,        タグは更新されていません。}
 * @endretresult
 *
 * @details
 *  ニックネームとオーナーの設定を行う amiibo 設定を開始します。本 API は、amiibo 設定が完了するまで戻りません。@n
 *  @ref ResultNotUpdated が返ってきた場合、@a pOutDeviceHandle 、@a pOutIsRegistered および @a pOutRegisterInfo には有効な情報が格納されていませんので、参照しないでください。@n
 *  登録情報が登録されている場合は、本 API ではなく、@ref RegisterInfo を引数に指定する@n
 *  StartNicknameAndOwnerSettings(DeviceHandle* pOutDeviceHandle, bool* pOutIsRegistered, RegisterInfo* pOutRegisterInfo, const AmiiboSettingsStartParam& startParam, const TagInfo& tagInfo, const RegisterInfo& registerInfo) を利用してください。
 *
 */
nn::Result StartNicknameAndOwnerSettings(DeviceHandle* pOutDeviceHandle, bool* pOutIsRegistered, RegisterInfo* pOutRegisterInfo, const AmiiboSettingsStartParam& startParam, const TagInfo& tagInfo) NN_NOEXCEPT;

/**
 * @brief   ゲームデータの消去を行う amiibo 設定を開始します。
 *
 * @param[out] pOutDeviceHandle タグを更新した NFC デバイスのハンドルを格納するバッファへのポインタを指定します。
 * @param[in]  startParam       amiibo 設定開始時必要な共通パラメータです。
 * @param[in]  tagInfo          更新の対象となるタグを示す情報です。
 *
 * @pre
 *  - GetState() == @ref State_Init
 *  - @a pOutDeviceHandle はアクセス可能なアドレスを指している必要があります。
 *
 * @retresult
 *     @handleresult{ResultSuccess,           タグの更新に成功しました。}
 *     @handleresult{ResultNotUpdated,        タグは更新されていません。}
 * @endretresult
 *
 * @details
 *  ゲームデータの消去を行う amiibo 設定を開始します。本 API は、amiibo 設定が完了するまで戻りません。@n
 *  @ref ResultNotUpdated が返ってきた場合、@a pOutDeviceHandle には有効な情報が格納されていませんので、参照しないでください。
 *
 */
nn::Result StartGameDataEraser(DeviceHandle* pOutDeviceHandle, const AmiiboSettingsStartParam& startParam, const TagInfo& tagInfo) NN_NOEXCEPT;

/**
 * @brief   データの復旧を行う amiibo 設定を開始します。
 *
 * @param[out] pOutDeviceHandle タグを更新した NFC デバイスのハンドルを格納するバッファへのポインタを指定します。
 * @param[in]  startParam       amiibo 設定開始時必要な共通パラメータです。
 * @param[in]  tagInfo          更新の対象となるタグを示す情報です。
 *
 * @pre
 *  - GetState() == @ref State_Init
 *  - @a pOutDeviceHandle はアクセス可能なアドレスを指している必要があります。
 *
 * @retresult
 *     @handleresult{ResultSuccess,           タグの更新に成功しました。}
 *     @handleresult{ResultNotUpdated,        タグは更新されていません。}
 * @endretresult
 *
 * @details
 *  データの復旧を行う amiibo 設定を開始します。本 API は、amiibo 設定が完了するまで戻りません。@n
 *  @ref ResultNotUpdated が返ってきた場合、@a pOutDeviceHandle には有効な情報が格納されていませんので、参照しないでください。
 *
 */
nn::Result StartRestorer(DeviceHandle* pOutDeviceHandle, const AmiiboSettingsStartParam& startParam, const TagInfo& tagInfo) NN_NOEXCEPT;

/**
 * @}
 */

}  // nfp
}  // nn
